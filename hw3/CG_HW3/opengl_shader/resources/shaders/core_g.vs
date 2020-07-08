#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;
layout (location = 2) in vec3 normal;

//out vec4 ourColor;
out vec3 result;

uniform vec3 lightPos;
uniform vec3 viewPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;
uniform float kd;
uniform float ks;


void main()
{
    
    
    //ourColor= color;
    vec3 FragPos = vec3( model * vec4(position, 1.0f));
    vec3 Normal = normalize( mat3(transpose(inverse(model))) * normal);
    
    
    gl_Position = proj * view * model * vec4(position, 1.0f);
    //phong
    //ambient
    float ambientS = 0.5f;
    vec3 ambient = ambientS * vec3(0.2f, 0.2f, 0.2f);
    
    //diffuse
    //vec3 norm = normalize(Normal);
    vec3 lightDir = normalize( lightPos - FragPos);
    float diff = max(dot(Normal, lightDir), 0.0);
    vec3 diffuse = diff * vec3(0.5f, 0.5f, 0.5f) * kd;
    
    //specular
    float specularS = ks;//Ks
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect( -lightDir, Normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 3.6);//
    vec3 specular = specularS * spec * vec3(0.8f, 0.8f, 0.8f);
    
    result = (ambient + diffuse + specular) * color;
    
}
