#version 330 core

// #include "light_common.glsl"

in Varyings {
    vec4 color;
    vec2 tex_coord;
    vec3 normal;
    vec3 view;
    vec3 world;
} fs_in;

out vec4 frag_color;

uniform vec4 tint;

//Initialize one sampler for each type
struct Material {
    sampler2D albedo;
    sampler2D specular;
    sampler2D roughness;
    sampler2D ambient_occlusion;
    sampler2D emissive;
};

#define DIRECTIONAL 0
#define POINT 1
#define SPOT 2

// struct Light {
//     int type;
//     vec3 position;
//     vec3 direction;
//     vec3 color;
//     vec3 attenuation;
//     vec2 cone_angles;
// };

struct SkyLight {
    vec3 sky, horizon, ground;
};

float lambert(vec3 n, vec3 l){
    return max(0, dot(n, l));
}

float phong(vec3 n, vec3 l, vec3 v, float shininess){
    vec3 r = reflect(-l, n);
    return pow(max(0, dot(v, r)), shininess);
}

vec3 compute_sky_light(vec3 normal, SkyLight sky_light) {
    float y = normal.y;
    float sky_factor = max(0, y);
    float ground_factor = max(0, -y);
    sky_factor *= sky_factor;
    ground_factor *= ground_factor;
    float horizon_factor = 1 - sky_factor - ground_factor;
    return sky_light.sky * sky_factor + sky_light.horizon * horizon_factor + sky_light.ground * ground_factor;
}

struct Light {
    int type;
    //will be calculated by the entity component
    vec3 position;
    //will be calculated by the entity component
    vec3 direction;
    vec3 specular, diffuse;
    //x*d^2 + y*d + z
    vec3 attenuation;
    //x: the inner angle & y: the outer angle
    vec2 cone_angles;
};

struct SkyStruct {
    vec3 top, middle, bottom;
};

uniform SkyStruct sky;

//From angle between normal and light direction on the object
float lambert(vec3 N, vec3 L){
    return max(0.0f, dot(N, L));
}

#define MAX_LIGHTS 8

uniform Material material;
uniform int light_count;
uniform Light lights[MAX_LIGHTS];

void main(){

    vec3 normal = normalize(fs_in.normal);//normal on point
    vec3 view = normalize(fs_in.view);//from camera to POINT

    //we will read from all texures simultanously since we created different texture units
    vec3 material_diffuse = texture(material.albedo, fs_in.tex_coord).rgb;
    vec3 material_specular = texture(material.specular, fs_in.tex_coord).rgb;
    vec3 material_ambient = material_diffuse * texture(material.ambient_occlusion, fs_in.tex_coord).r;
    
    //take only the r channel since it is grayscale:
    float material_roughness = texture(material.roughness, fs_in.tex_coord).r;
    float material_shininess = 2.0 / pow(clamp(material_roughness, 0.001, 0.999), 4.0) - 2.0;
    vec3 material_emissive = texture(material.emissive, fs_in.tex_coord).rgb;

//float material_ao = texture(material.ambient_occlusion, fs_in.tex_coord).r;
    
    if(normal.y>0)
        vec3 sky_light= mix(sky.middle, sky.top, normal.y * normal.y);
    else
        vec3 sky_light=mix(sky.middle, sky.bottom, normal.y * normal.y);


    frag_color = vec4(material_emissive + material_ambient  , 1.0);
    // frag_color = vec4(0.0,0.0,0.0,1.0);

    //applying light on the material constants
    for(int i = 0; i < min(MAX_LIGHTS, light_count); i++){
        Light light = lights[i];

        //We multiply by -ve since lambart assumes the light direction from point to light and we want from light to point
        vec3 direction_to_light = light.direction;

        //Will not implement if directional because it has constant direction (also no position)
        if(light.type != DIRECTIONAL){
            //Calculate direction as a light ray
            direction_to_light= normalize(light.position - fs_in.world);
        }
        
        vec3 diffuse = light.diffuse * material_diffuse * lambert(normal, direction_to_light);;
        
        vec3 reflected = reflect(-direction_to_light, normal);
        
        vec3 specular = light.specular * material_specular * pow(max(0, dot(view, reflected)), material_shininess);

        float attenuation = 1;

        //Will not implement if directional because it has constant intensity anywhere in the scene
        if(light.type != DIRECTIONAL){
            float d = distance(light.position, fs_in.world);
            attenuation /= dot(light.attenuation, vec3(d*d, d, 1));

           /*
            For Spot:
            The light direction is in the middle of the inner cone
            The inner cone has: full intensity
            In between the two layers: intensity gradually decreases
            The outer cone: has zero intensity

           */
            if(light.type == SPOT){
                //outer cone - inner cone
                float angle = acos(-dot(light.direction, direction_to_light));
                attenuation *= smoothstep(light.cone_angles.y, light.cone_angles.x, angle);
            }
        }

        frag_color.rgb += (diffuse + specular) * attenuation;
    }
}