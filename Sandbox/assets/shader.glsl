#version 330 core

#pragma vertex

in vec3 aPosition;
in vec3 aNormal;
in vec2 aTexCoords;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProj;

out vec3 vWorldPos;
out vec3 vNormal;
out vec2 vTexCoords;

void main() {
    vec4 worldPos = vec4(aPosition, 1.0) * uModel;
    mat3 normalMatrix = transpose(inverse(mat3(uModel)));

    vWorldPos = worldPos.xyz;
    vNormal = normalize(aNormal * normalMatrix);
    vTexCoords = aTexCoords;

    gl_Position = worldPos * uView * uProj;
}

#pragma fragment

in vec3 vWorldPos;
in vec3 vNormal;
in vec2 vTexCoords;

out vec4 FragColor;

// --- Textures (bind 1x1 white if unused) ---
uniform sampler2D uColorMap;
uniform sampler2D uMetallicMap;
uniform sampler2D uRoughnessMap;

// --- Material factors ---
uniform vec4 uColor;       // multiplied with uColorMap (rgb = color, a = alpha)
uniform float uMetallic;    // multiplied with uMetallicMap
uniform float uRoughness;   // multiplied with uRoughnessMap

// --- Ambient ---
uniform vec3 uAmbientColor;
uniform float uAmbientIntensity;

// --- Lights ---
#define MAX_LIGHTS 16
uniform int uNumLights;
uniform vec3 uLightPositions[MAX_LIGHTS];   // world-space pos, or direction if directional
uniform vec3 uLightColors[MAX_LIGHTS];      // RGB color
uniform float uLightIntensities[MAX_LIGHTS]; // scalar intensity
uniform float uLightRadii[MAX_LIGHTS];       // 0 = directional, >0 = point light radius
uniform mat4 uLightSpaceMatrices[MAX_LIGHTS];
uniform int uLightShadowMapIndices[MAX_LIGHTS];

uniform sampler2DArrayShadow uShadowMaps;

// --- Camera ---
uniform vec3 uCameraPosition;

// -------------------------------------------------------
const float PI = 3.14159265359;

vec3 sRGBToLinear(vec3 c) { return pow(c, vec3(2.2)); }
vec3 linearToSRGB(vec3 c) { return pow(c, vec3(1.0 / 2.2)); }

vec3 ACESToneMap(vec3 x) {
    x *= 0.9;
    float a = 2.51, b = 0.03, c = 2.43, d = 0.59, e = 0.14;
    return clamp((x * (a * x + b)) / (x * (c * x + d) + e), 0.0, 1.0);
}

float D_GGX(float NdotH, float roughness) {
    float a = roughness * roughness;
    float a2 = a * a;
    float d = NdotH * NdotH * (a2 - 1.0) + 1.0;
    return a2 / (PI * d * d);
}

float G_SchlickGGX(float NdotV, float roughness) {
    float r = roughness + 1.0;
    float k = (r * r) / 8.0;
    return NdotV / (NdotV * (1.0 - k) + k);
}

float G_Smith(float NdotV, float NdotL, float roughness) {
    return G_SchlickGGX(NdotV, roughness) * G_SchlickGGX(NdotL, roughness);
}

vec3 F_Schlick(float cosTheta, vec3 F0) {
    return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}

float calcShadow(int i, vec3 N, vec3 L)
{
    vec4 fragPosLS = vec4(vWorldPos, 1.0) * uLightSpaceMatrices[i];
    vec3 projCoords = fragPosLS.xyz / fragPosLS.w;
    projCoords = projCoords * 0.5 + 0.5;

    if (projCoords.x < 0.0 || projCoords.x > 1.0 ||
    projCoords.y < 0.0 || projCoords.y > 1.0 ||
    projCoords.z > 1.0)
    return 0.0;

    float current = projCoords.z;
    float NdotL = max(dot(N, L), 0.0);
    float bias = max(0.005 * (1.0 - NdotL), 0.0005);

    vec2 texelSize = 1.0 / vec2(textureSize(uShadowMaps, 0).xy);

    float sum = 0.0;
    int r = 1;
    for (int x = -r; x <= r; ++x)
    for (int y = -r; y <= r; ++y)
    {
        vec2 uv = projCoords.xy + vec2(x, y) * texelSize;
        sum += texture(uShadowMaps, vec4(uv, float(i), current - bias));
    }

    float visibility = sum / float((2 * r + 1) * (2 * r + 1));
    return 1.0 - visibility;
}

void main() {
    // --- Sample material ---
    vec3 albedo = sRGBToLinear(texture(uColorMap, vTexCoords).rgb) * uColor.rgb;
    float alpha = texture(uColorMap, vTexCoords).a * uColor.a;
    float metallic = texture(uMetallicMap, vTexCoords).r * uMetallic;
    float roughness = clamp(texture(uRoughnessMap, vTexCoords).r * uRoughness, 0.04, 1.0);

    vec3 N = normalize(vNormal);
    vec3 V = normalize(uCameraPosition - vWorldPos);
    float NdotV = max(dot(N, V), 0.0);

    vec3 F0 = mix(vec3(0.04), albedo, metallic);

    // --- Punctual lights ---
    vec3 Lo = vec3(0.0);

    for (int i = 0; i < uNumLights && i < MAX_LIGHTS; i++)
    {
        vec3 L;
        float attenuation;
        float shadow = 0.0;

        if (uLightRadii[i] <= 0.0)
        {
            // Directional light — position is treated as direction
            L = normalize(uLightPositions[i]);
            attenuation = 1.0;
            if (uLightShadowMapIndices[i] < 0) {
                shadow = 0.0;
            } else {
                shadow = calcShadow(uLightShadowMapIndices[i], N, L);
            }
        }
        else
        {
            // Point light
            vec3 toLight = uLightPositions[i] - vWorldPos;
            float dist = length(toLight);
            L = toLight / dist;
            attenuation = 1.0 / (dist * dist);
            // Smooth falloff at radius boundary
            float s = clamp(1.0 - pow(dist / uLightRadii[i], 4.0), 0.0, 1.0);
            attenuation *= s * s;
        }

        vec3 H = normalize(V + L);
        float NdotL = max(dot(N, L), 0.0);
        float NdotH = max(dot(N, H), 0.0);
        float HdotV = max(dot(H, V), 0.0);

        float D = D_GGX(NdotH, roughness);
        float G = G_Smith(NdotV, NdotL, roughness);
        vec3 F = F_Schlick(HdotV, F0);

        vec3 specular = (D * G * F) / (4.0 * NdotV * NdotL + 0.0001);
        vec3 kD = (1.0 - F) * (1.0 - metallic);

        vec3 radiance = uLightColors[i] * uLightIntensities[i] * attenuation;
        Lo += (kD * albedo / PI + specular) * radiance * NdotL * (1.0 - shadow);
    }

    // --- Ambient ---
    vec3 F_amb = F_Schlick(NdotV, F0);
    vec3 kD_amb = (1.0 - F_amb) * (1.0 - metallic);

    vec3 diffuse_ambient = kD_amb * albedo * uAmbientColor * uAmbientIntensity;
    vec3 specular_ambient = F_amb * uAmbientColor * uAmbientIntensity;

    vec3 ambient = diffuse_ambient + specular_ambient;

    // --- Combine, tone-map, gamma correct ---
    vec3 color = ACESToneMap(ambient + Lo);
    color = linearToSRGB(color);

    FragColor = vec4(color, alpha);
}
