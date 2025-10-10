#version 330 core

struct FragData {
    vec3 normal;
    vec3 frag_pos;
    vec2 uv;
};

struct Properties {
    vec4 diffuse;
    vec4 specular;
    float gloss;
};

//@VERTEX

struct Transforms {
    mat4 model;
    mat4 view;
    mat4 proj;
};

/*@pos*/ in vec3 aPos;
/*@uv*/ in vec2 aUv;
/*@norm*/ in vec3 aNormal;

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

//@FRAGMENT

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

uniform bool has_diffuse_map;
uniform bool has_specular_map;

uniform vec3 ambient_light;
uniform PointLight point_light[128];
uniform int point_light_count;

uniform sampler2D diffuse_map;
uniform sampler2D specular_map;

uniform float gloss;

out vec4 FragColor;

float diffuse(vec3 light_dir, vec3 normal) {
    return max(dot(normal, light_dir), 0.0);
}

float specular(vec3 light_dir, vec3 normal, vec3 view_dir, float gloss) {
    vec3 reflect_dir = reflect(-light_dir, normal);
    return pow(max(dot(view_dir, reflect_dir), 0.0), gloss);
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
        vec3 light_f = light_i * point_light[i].light_color;

        vec4 col;
        if (has_diffuse_map) {
            col = texture(diffuse_map, frag_data.uv) * frag_properties.diffuse;
        } else {
            col = frag_properties.diffuse;
        }

        vec4 amb = vec4(col.rgb * ambient_light, col.a);
        vec4 diff = vec4(col.rgb * diffuse(light_dir, norm) * light_f, col.a);

        vec4 col;
        if (has_specular_map) {
            col = texture(specular_map, frag_data.uv) * frag_properties.specular;
        } else {
            col = frag_properties.specular;
        }

        vec4 spec = vec4(light_f * specular(light_dir, norm, view_dir, gloss), 1.0);

        result += amb + diff + spec;
    }

    FragColor += result;
}