#version 330 core

#pragma vertex

in vec3 aPosition;
in vec3 aNormal;
in vec2 aTexCoords;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProj;

out VS_OUT {
    vec3 worldPos;
    vec3 worldNormal;
    vec2 uv;
    float viewDepth;
} vs_out;

void main()
{
    vec4 localPos  = vec4(aPosition, 1.0);
    vec4 worldPos4 = localPos * uModel;
    vec4 viewPos   = worldPos4 * uView;

    vec3 worldNormal = normalize((vec4(aNormal, 0.0) * transpose(inverse(uModel))).xyz);

    vs_out.worldPos    = worldPos4.xyz;
    vs_out.worldNormal = worldNormal;
    vs_out.uv          = aTexCoords;
    vs_out.viewDepth   = viewPos.z;

    gl_Position = viewPos * uProj;
}

#pragma fragment

out vec4 FragColor;

in VS_OUT {
    vec3 worldPos;
    vec3 worldNormal;
    vec2 uv;
    float viewDepth;
} fs_in;

uniform sampler2D uColorMap;
uniform sampler2D uMetallicMap;
uniform sampler2D uRoughnessMap;
uniform vec4 uColor;
uniform float uMetallic;
uniform float uRoughness;
uniform vec3 uAmbientColor;
uniform float uAmbientIntensity;

#define MAX_LIGHTS 16
#define MAX_SHADOW_CASCADES 8

uniform int uNumLights;
uniform vec3 uLightPositions[MAX_LIGHTS];
uniform vec3 uLightColors[MAX_LIGHTS];
uniform float uLightIntensities[MAX_LIGHTS];
uniform float uLightRadii[MAX_LIGHTS];

uniform mat4 uLightSpaceMatrices[MAX_LIGHTS * MAX_SHADOW_CASCADES];

uniform int uLightShadowMapIndices[MAX_LIGHTS];
uniform int uShadowCascadeCount;
uniform float uShadowCascadeRanges[MAX_SHADOW_CASCADES];
uniform sampler2DArrayShadow uShadowMaps;

uniform vec3 uCameraPosition;

const float PI = 3.14159265359;
const float EPSILON = 1e-4;

float saturate(float x)
{
    return clamp(x, 0.0, 1.0);
}

vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(1.0 - saturate(cosTheta), 5.0);
}

float distributionGGX(vec3 N, vec3 H, float roughness)
{
    float a  = roughness * roughness;
    float a2 = a * a;

    float NdotH  = saturate(dot(N, H));
    float NdotH2 = NdotH * NdotH;

    float denom = NdotH2 * (a2 - 1.0) + 1.0;
    return a2 / max(PI * denom * denom, EPSILON);
}

float geometrySchlickGGX(float NdotV, float roughness)
{
    float r = roughness + 1.0;
    float k = (r * r) / 8.0;
    return NdotV / max(NdotV * (1.0 - k) + k, EPSILON);
}

float geometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = saturate(dot(N, V));
    float NdotL = saturate(dot(N, L));
    float ggxV = geometrySchlickGGX(NdotV, roughness);
    float ggxL = geometrySchlickGGX(NdotL, roughness);
    return ggxV * ggxL;
}

int selectCascade(float viewDepth)
{
    for (int i = 0; i < uShadowCascadeCount; i++)
    {
        if (viewDepth <= uShadowCascadeRanges[i])
        return i;
    }

    return max(uShadowCascadeCount - 1, 0);
}

float sampleDirectionalShadow(int lightIndex, vec3 N, vec3 L)
{
    int shadowMapSet = uLightShadowMapIndices[lightIndex];
    if (shadowMapSet < 0 || uShadowCascadeCount <= 0) return 1.0;

    int cascade = selectCascade(length(fs_in.worldPos - uCameraPosition));
    int layerIndex = shadowMapSet * uShadowCascadeCount + cascade;

    vec4 lightClip = vec4(fs_in.worldPos, 1.0) * uLightSpaceMatrices[layerIndex];
    vec3 projCoords = lightClip.xyz / max(lightClip.w, EPSILON);
    projCoords = projCoords * 0.5 + 0.5;

    if (projCoords.z > 1.0 || projCoords.z < 0.0)
    return 1.0;
    if (projCoords.x < 0.0 || projCoords.x > 1.0 ||
    projCoords.y < 0.0 || projCoords.y > 1.0)
    return 1.0;

    float ndotl = saturate(dot(N, L));
    float bias = max(0.0005, 0.005 * (1.0 - ndotl));

    vec2 texelSize = 1.0 / vec2(textureSize(uShadowMaps, 0).xy);

    float shadow = 0.0;
    int kernelSize = 2;

    for (int y = -kernelSize; y <= kernelSize; ++y)
    {
        for (int x = -kernelSize; x <= kernelSize; ++x)
        {
            vec2 offset = vec2(x, y) * texelSize;
            shadow += texture(
                uShadowMaps,
                vec4(projCoords.xy + offset, float(layerIndex), projCoords.z - bias)
            );
        }
    }

    return shadow / pow(kernelSize * 2 + 1, 2);
}

vec3 evaluateLight(
    vec3 N,
    vec3 V,
    vec3 albedo,
    float metallic,
    float roughness,
    vec3 F0,
    int lightIndex
)
{
    vec3 L;
    vec3 radiance;
    float shadow = 1.0;

    if (uLightRadii[lightIndex] == 0.0)
    {
        L = normalize(uLightPositions[lightIndex]);
        radiance = uLightColors[lightIndex] * uLightIntensities[lightIndex];
        shadow = sampleDirectionalShadow(lightIndex, N, L);
    }
    else
    {
        vec3 toLight = uLightPositions[lightIndex] - fs_in.worldPos;
        float dist = length(toLight);
        if (dist <= EPSILON)
        return vec3(0.0);

        L = toLight / dist;

        float radius = uLightRadii[lightIndex];
        float falloff = clamp(1.0 - (dist * dist) / max(radius * radius, EPSILON), 0.0, 1.0);
        falloff *= falloff;

        float attenuation = falloff / max(dist * dist, 1.0);
        radiance = uLightColors[lightIndex] * uLightIntensities[lightIndex] * attenuation;
    }

    vec3 H = normalize(V + L);

    float NdotL = saturate(dot(N, L));
    float NdotV = saturate(dot(N, V));
    float HdotV = saturate(dot(H, V));

    if (NdotL <= 0.0 || NdotV <= 0.0)
    return vec3(0.0);

    float D = distributionGGX(N, H, roughness);
    float G = geometrySmith(N, V, L, roughness);
    vec3  F = fresnelSchlick(HdotV, F0);

    vec3 numerator = D * G * F;
    float denom = max(4.0 * NdotV * NdotL, EPSILON);
    vec3 specular = numerator / denom;

    vec3 kS = F;
    vec3 kD = (vec3(1.0) - kS) * (1.0 - metallic);
    vec3 diffuse = kD * albedo / PI;

    return (diffuse + specular) * radiance * NdotL * shadow;
}

void main()
{
    vec3 N = normalize(fs_in.worldNormal);
    vec3 V = normalize(uCameraPosition - fs_in.worldPos);

    vec4 baseSample = texture(uColorMap, fs_in.uv) * uColor;
    vec3 albedo = pow(baseSample.rgb, vec3(2.2));

    float metallic  = clamp(texture(uMetallicMap, fs_in.uv).r * uMetallic, 0.0, 1.0);
    float roughness = clamp(texture(uRoughnessMap, fs_in.uv).r * uRoughness, 0.045, 1.0);

    vec3 F0 = mix(vec3(0.04), albedo, metallic);

    vec3 ambient = albedo * uAmbientColor * uAmbientIntensity;

    vec3 Lo = vec3(0.0);
    for (int i = 0; i < uNumLights && i < MAX_LIGHTS; ++i)
    {
        Lo += evaluateLight(N, V, albedo, metallic, roughness, F0, i);
    }

    vec3 color = ambient + Lo;

    color = color / (color + vec3(1.0));
    color = pow(color, vec3(1.0 / 2.2));

    FragColor = vec4(color, baseSample.a);
}