#version 330

#define PI 3.14159265358979323846264338327950288

in vec3 surface_position;
in vec3 surface_normal;
in vec2 surface_texcoord;

out vec4 fragment_color;

uniform vec3 material_color_ambient;
uniform vec3 material_color_diffuse;
uniform vec3 material_color_specular;

uniform float material_brightness;

uniform float material_metallic;

uniform float material_roughness;

uniform float material_occlusion;

uniform vec3 material_fresnel_ior;

uniform float tone_mapping_exposure;

uniform bool tone_mapping_toggle;

uniform float tone_mapping_gamma;

uniform sampler2D texture_diffuse;

uniform sampler2D texture_metallic;

uniform sampler2D texture_roughness;

uniform sampler2D texture_occlusion;

uniform sampler2D texture_normalMap;

uniform float light_attenuation_factor_constant; 
//= 0.02

uniform float light_attenuation_factor_linear; //= 0.00003;

uniform float light_attenuation_factor_quadratic; 
// = 0.000003;

struct Light {
    vec3 position;
    vec3 color;
    float intensity;
};

#define NUM_LIGHT 4

uniform Light light[NUM_LIGHT];

float trowbridge_reitz(vec3 n, vec3 h, float roughness)
{
  float a = roughness * roughness;
  float a2 = a * a;
  float ndh = max(dot(n, h), 0.0);
  float ndh2 = ndh * ndh;
  float numerator = a2;
  float denominator = (ndh2 * (a2 - 1.0) + 1.0);
  denominator = denominator * denominator * PI;
  return numerator / denominator;
}

float schlick_beckmann(float costheta, float roughness)
{
  float r = (roughness + 1.0);
  float k = (r * r) / 8.0;
  float numerator = costheta;
  float denominator = costheta * (1.0 - k) + k;
  return numerator / denominator;
}

float smith(vec3 n, vec3 l, vec3 v, float roughness)
{
  float ndl = max(dot(n, l), 0.0);
  float ndv = max(dot(n, v), 0.0);
  float shadow = schlick_beckmann(ndl, roughness);
  float occlusion = schlick_beckmann(ndv, roughness);
  return shadow * occlusion;
}

vec3 schlick_fresnel(float costheta, vec3 f0)
{
  return f0 + (1.0 - f0) * pow(1.0 - costheta, 5.0);
}

vec3 tone_mapping_reinhard(vec3 x)
{
  return x / (x + vec3(1.0));
}

vec3 brdf_cook_torrance(vec3 l, vec3 v, vec3 n, float roughness,  vec3 albedo,  float metallic, vec3 ambient)
{
  vec3 h = normalize(l + v);

  float diffuse_reflection = max(dot(n, l), 0.0);

  float d = trowbridge_reitz(n, h, roughness);

  float g = smith(n, l, v, roughness);

  vec3 f0 = material_fresnel_ior;

  f0 = mix(f0, albedo, metallic);

  vec3 f = schlick_fresnel(max(dot(h, v), 0.0), f0);

  vec3 coor_torrance_numerator = d * f * g;

  float coor_torrance_denominator = 4.0 * max(dot(n, v), 0.0) * diffuse_reflection;

  vec3 specular = coor_torrance_numerator / max(coor_torrance_denominator, 0.001);

  specular = specular * material_color_specular;

  vec3 ks = f;

  vec3 kd = vec3(1.0) - ks;

  kd *= 1.0 - metallic;
  vec3 reflectance = (kd * albedo / PI + specular) * diffuse_reflection;

  vec3 color = (ambient + reflectance) * material_brightness;

  return color;
}

vec3 calculateLight (Light light, vec3 n, vec3 v, vec3 albedo, float metallic, float roughness, float occlusion,  vec3 ambient)
{
    vec3 l = normalize(light.position - surface_position);
    
  vec3 direction = normalize(light.position - surface_position);
    float distance = length(light.position - surface_position);

    float attenuation = 1.0 / (light_attenuation_factor_constant + light_attenuation_factor_linear * distance + light_attenuation_factor_quadratic * distance * distance);

    vec3 irradiance = light.color * light.intensity * attenuation;

    vec3 lightColor = vec3(0.0);
    if (dot(n, l) > 0.0) {
        vec3 h = normalize(l + v);
        float NdotL = dot(n, l);
        float NdotH = dot(n, h);
        float VdotH = dot(v, h);
        float roughness2 = roughness * roughness;
        float G = min(1.0, 2.0 * NdotH * min(NdotL, VdotH) / VdotH);
        float D = trowbridge_reitz(n, h, roughness2);
        vec3 F0 = vec3(0.04);
        vec3 F = schlick_fresnel(max(dot(h, v), 0.0), F0);
        vec3 specular = (D * G * F) / (4.0 * NdotL * max(dot(n, v), 0.001));
        vec3 diffuse = albedo * (1.0 - metallic) * NdotL;
        lightColor = irradiance * (diffuse + specular);
    }

    lightColor += brdf_cook_torrance(n, v, l, roughness, albedo, metallic, ambient);

    return lightColor * occlusion;
};


void main()
{

  vec3 n = normalize( texture(texture_normalMap, surface_texcoord).rgb * 2.0 - 1.0 );
  vec3 v = normalize(-surface_position);


  vec3 texture_sample_diffuse = texture(texture_diffuse, surface_texcoord).rgb;

  texture_sample_diffuse = pow(texture_sample_diffuse, vec3(tone_mapping_gamma));

  float texture_sample_metallic = texture(texture_metallic, surface_texcoord).r;

  float texture_sample_roughness = texture(texture_roughness, surface_texcoord).r;

  float texture_sample_occlusion = texture(texture_occlusion, surface_texcoord).r;

  float metallic = material_metallic * texture_sample_metallic;
  float roughness = material_roughness * texture_sample_roughness;
  float occlusion = material_occlusion * texture_sample_occlusion;

  vec3 albedo = material_color_diffuse * texture_sample_diffuse;

  vec3 ambient = material_color_ambient * albedo * occlusion;


  vec3 color;


  for (int i = 0; i < NUM_LIGHT; i++)
  {
    color += calculateLight(light[i], n, v, albedo, metallic, roughness, occlusion, ambient);   
  }

  color = vec3(1.0) - exp(-color * tone_mapping_exposure);


  color = tone_mapping_reinhard(color);

  color = pow(color, vec3(1.0 / tone_mapping_gamma));

  fragment_color = vec4(color, 1.0);
}
