#version 330 core

struct FragData {
    vec3 normal;
    vec3 frag_pos;
    vec2 uv;
};

struct Properties {
    vec4 diffuse;
    float metallic;
    float roughness;
};

@VERTEX

struct Transforms {
    mat4 model;
    mat4 view;
    mat4 proj;
};

@pos in vec3 aPos;
@uv in vec2 aUv;
@norm in vec3 aNormal;

uniform Transforms transforms;
uniform Properties properties;

out FragData frag_data;
out Properties frag_properties;
out mat4x4 view_trans;

void main()
{
    gl_Position = transforms.proj * transforms.view * transforms.model * vec4(aPos, 1.0);

    frag_data.frag_pos = (transforms.view * transforms.model * vec4(aPos, 1.0)).xyz;
    frag_data.uv = aUv;
    frag_data.normal = mat3(transpose(inverse(transforms.view * transforms.model))) * aNormal;

    view_trans = transforms.view;

    frag_properties = properties;
}

@FRAGMENT

struct PointLight {
    vec3 light_color;
    vec3 light_pos;
    float k_const;
    float k_linear;
    float k_quadratic;
};

in FragData frag_data;
in Properties frag_properties;
in mat4x4 view_trans;

uniform int has_diffuse_map;
uniform int has_roughness_map;
uniform int has_metallic_map;
uniform vec3 ambient_light;
uniform PointLight point_light[128];

uniform int point_light_count;

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
    vec4 result = vec4(0.0);
    for (int i = 0; i < point_light_count; i++) {
        vec3 light_pos = (view_trans * vec4(point_light[i].light_pos, 1.0)).xyz;
        vec3 norm = normalize(frag_data.normal);
        vec3 light_dir = normalize(light_pos - frag_data.frag_pos);
        vec3 view_dir = normalize(-frag_data.frag_pos);

        float light_dist = length(light_pos - frag_data.frag_pos);
        float light_i = 1.0 / (point_light[i].k_const + point_light[i].k_linear * light_dist + point_light[i].k_quadratic * light_dist * light_dist);

        vec4 amb;
        vec4 diff;
        if (has_diffuse_map != 0) {
            vec4 col = texture(diffuse_map, frag_data.uv);
            amb = vec4(ambient_light * col.rgb, col.a);
            diff = vec4(col.rgb * diffuse(light_dir, norm) * point_light[i].light_color * light_i, col.a);
        } else {
            amb = vec4(ambient_light * frag_properties.diffuse.rgb, frag_properties.diffuse.a);
            diff = vec4(frag_properties.diffuse.rgb * diffuse(light_dir, norm) * point_light[i].light_color * light_i, frag_properties.diffuse.a);
        }

        float shininess;
        if (has_metallic_map != 0) {
            vec4 col = texture(metallic_map, frag_data.uv);
            shininess = pow(2.0, clamp((col.r + col.g + col.b) * 8.0 / 3.0, 0.0, 8.0));
        } else {
            shininess = pow(2.0, clamp(frag_properties.metallic * 8.0, 0.0, 8.0));
        }

        float smoothness;
        if (has_roughness_map != 0) {
            vec4 col = texture(roughness_map, frag_data.uv);
            smoothness = 1.0 - (col.r + col.g + col.b) / 3.0;
        } else {
            smoothness = 1.0 - frag_properties.roughness * 1.0;
        }

        vec4 spec = vec4(smoothness * point_light[i].light_color * light_i * specular(light_dir, norm, view_dir, shininess), 1.0);

        result += amb + diff + spec;
    }

    FragColor += result;
}