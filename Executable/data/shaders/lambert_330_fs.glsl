//lambert_330_fs.glsl

#version 330

in vec3 surface_position;
in vec3 surface_normal;

out vec4 fragment_color;

uniform vec3 color_ambient;
uniform vec3 color_diffuse;

uniform vec3 light_position;

void main(){
vec3 n = normalize(surface_normal);
vec3 l = normalize(light_position - surface_position);

float reflection_diffuse = max(dot(n, l), 0.0);

fragment_color = vec4(color_ambient + color_diffuse * reflection_diffuse, 1.0);

}