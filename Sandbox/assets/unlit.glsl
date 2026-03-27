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

uniform sampler2D uColorMap;
uniform vec4 uColor;

void main() {
    vec3 albedo = texture(uColorMap, vTexCoords).rgb * uColor.rgb;
    FragColor = vec4(albedo, uColor.a);
}
