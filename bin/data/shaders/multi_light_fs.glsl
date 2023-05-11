#version 330

struct Material{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};

struct DirLight{
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct PointLight {
    vec3 position;
    
    float constant;
    float linear;
    float quadratic;
    float intensity;
	
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct SpotLight {
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;
  
    float constant;
    float linear;
    float quadratic;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;       
};


#define NB_POINT_LIGHTS 4

in vec3 surface_position;
in vec3 surface_normal;

out vec4 fragment_color;


uniform vec3 viewPosition;
uniform Material material;
uniform PointLight pointLight[NB_POINT_LIGHTS];
uniform DirLight dirLight;

vec3 CalcDirLight(DirLight light, vec3 n, vec3 v);
vec3 CalcPointLight(PointLight light, vec3 n, vec3 surface_position, vec3 v);
vec3 CalcSpotLight(SpotLight light, vec3 n, vec3 surface_position, vec3 v);

void main()
{

vec3 n = normalize(surface_normal);
vec3 v = normalize(viewPosition - surface_position);


vec3 result = CalcDirLight(dirLight, n, v);

for (int i = 0; i < NB_POINT_LIGHTS; i++)
{
    result += CalcPointLight(pointLight[i], n, surface_position, v);
}


fragment_color = vec4(result, 1.0);

}


vec3 CalcDirLight(DirLight light, vec3 n, vec3 v)
{
    vec3 l = normalize(-light.direction);

    float diff = max(dot(n, l), 0.0);

    vec3 r = reflect(-l, n);
    float spec = pow(max(dot(v, r), 0.0), material.shininess);

    vec3 ambient = light.ambient * material.ambient;
    vec3 diffuse = light.diffuse * diff * material.diffuse;
    vec3 specular = light.specular * (spec * material.specular);
    return (ambient + diffuse + specular);
}


vec3 CalcPointLight(PointLight light, vec3 n, vec3 surface_position, vec3 v)
{
    vec3 l = normalize(light.position - surface_position);

    float diff = max(dot(n, l), 0.0);

    vec3 r = reflect(-l, n);
    float spec = pow(max(dot(v, r), 0.0), material.shininess);

    float distance = length(light.position - surface_position);
    float attenuation = light.intensity / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    

    vec3 ambient = light.ambient * material.ambient;
    vec3 diffuse = light.diffuse * diff * material.diffuse;
    vec3 specular = light.specular * (spec * material.specular);
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}

vec3 CalcSpotLight(SpotLight light, vec3 n, vec3 surface_position, vec3 v)
{
    vec3 l = normalize(light.position - surface_position);

    float diff = max(dot(n, l), 0.0);

    vec3 r = reflect(-l, n);
    float spec = pow(max(dot(v, r), 0.0), material.shininess);
   
    float distance = length(light.position - surface_position);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
   
    float theta = dot(l, normalize(-light.direction)); 
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
 
    vec3 ambient = light.ambient * material.ambient;
    vec3 diffuse = light.diffuse * diff * material.diffuse;
    vec3 specular = light.specular * (spec * material.specular);
    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;
    return (ambient + diffuse + specular);
}

