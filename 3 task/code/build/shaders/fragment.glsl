#version 330 core

out vec4 color;
in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;

struct Color {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

uniform Color material;

uniform int used_texture;
uniform sampler2D ourTexture;
uniform vec3 viewPos;
uniform vec3 lightPos; 

void main()
{

	vec3 ambient = vec3(0.4) * material.ambient;

    // diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = vec3(0.8) * (diff * material.diffuse);

    // specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = vec3(1.0) * (spec * material.specular);  

    vec3 result = ambient + diffuse + specular;
    color = vec4(result, 1.0);

    if(used_texture==1)
  		color = vec4(color)*texture(ourTexture, TexCoord);
}	