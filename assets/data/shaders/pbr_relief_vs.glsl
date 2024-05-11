#version 330

in vec4 position;
in vec4 normal;
in vec2 texcoord;

out vec3 surface_position;
out vec3 surface_normal;
out vec2 surface_texcoord;

uniform mat4x4 modelViewMatrix;
uniform mat4x4 projectionMatrix;

void main()
{
  mat4x4 normalMatrix = transpose(inverse(modelViewMatrix));

  surface_normal = vec3(normalMatrix * normal);

  surface_position = vec3(modelViewMatrix * position);

  surface_texcoord = texcoord;

  gl_Position = projectionMatrix * modelViewMatrix * position;
}
