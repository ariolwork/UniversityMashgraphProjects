#version 330

layout(location = 0) in vec3 vertex;
layout(location = 1) in vec2 texCoord;
layout (location = 2) in vec3 normal;

out vec2 TexCoord;
out vec3 Normal;
out vec3 FragPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main(void)
{
  gl_Position  = projection * view * model * vec4(vertex,1.0);
  FragPos = vec3(model*vec4(vertex, 1.0f));
  TexCoord = texCoord;
  Normal = mat3(transpose(inverse(model)))*normal;
}
