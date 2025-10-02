#version 330 core

struct FragData {
    vec3 normal;
    vec3 frag_pos;
    vec2 uv;
};

struct LightingData {
    vec3 light_diffuse;
    vec3 light_ambient;
    vec3 light_pos;
};

struct Properties {
    vec4 diffuse;
    vec4 specular;
    bool has_diffuse_map;
    bool has_roughness_map;
    bool has_metallic_map;
    float metallic;
    float roughness;
};

in FragData frag_data;
in LightingData frag_lighting_data;
in Properties frag_properties;

out vec4 FragColor;

uniform sampler2D diffuse_map;
uniform sampler2D roughness_map;
uniform sampler2D metallic_map;

float diffuse(vec3 light_dir, vec3 normal) {
    return max(dot(normal, light_dir), 0.0);
}

float specular(vec3 light_dir, vec3 normal, vec3 view_dir, float shininess) {
    vec3 reflect_dir = reflect(-light_dir, normal);
    return pow(max(dot(view_dir, reflect_dir), 0.0), shininess);
}

void main()
{
    vec3 norm = normalize(frag_data.normal);
    vec3 light_dir = normalize(frag_lighting_data.light_pos - frag_data.frag_pos);
    vec3 view_dir = normalize(-frag_data.frag_pos);

    vec4 amb;
    vec4 diff;
    if (frag_properties.has_diffuse_map) {
        vec4 amb = texture(diffuse_map, frag_data.uv);
        vec4 diff = texture(diffuse_map, frag_data.uv) * diffuse(light_dir, norm);
    } else {
        vec4 amb = frag_properties.diffuse;
        vec4 diff = frag_properties.diffuse * diffuse(light_dir, norm);
    }

    float shininess;
    if (frag_properties.has_metallic_map) {
        float shininess = pow(2.0, clamp(length(texture(metallic_map, frag_data.uv)) * 8.0, 0.0, 8.0));
    } else {
        float shininess = pow(2.0, clamp(frag_properties.metallic * 8.0, 0.0, 8.0));
    }

    float smoothness;
    if (frag_properties.has_roughness_map) {
        float smoothness = 1.0 - texture(roughness_map, frag_data.uv) * 1.0;
    } else {
        float smoothness = 1.0 - frag_properties.roughness * 1.0;
    }

    vec4 spec = smoothness * frag_properties.specular * specular(light_dir, norm, view_dir, shininess);

    vec4 result = amb + diff + spec;
    FragColor = result;
}