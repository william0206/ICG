#version 330 core

in vec3 ourColor;
in vec3 FragPos;//p
in vec3 Normal;

out vec4 color;

uniform vec3 lightPos;
uniform vec3 viewPos;
uniform float kd;
uniform float ks;


void main()
{
    //phong
    //ambient
    float ambientS = 0.5f;//ka
    vec3 ambient = ambientS * vec3(0.2f, 0.2f, 0.2f);//ka * Ia
    
    //diffuse
    vec3 norm = normalize(Normal);//n
    vec3 lightDir = normalize(lightPos - FragPos);//l
    float diff = max(dot(norm, lightDir), 0.0);//(n dot l)
    vec3 diffuse = diff * vec3(0.5f, 0.5f, 0.5f) * kd;//
    
    //specular
    float specularS = ks;//Ks
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 3.6);//
    vec3 specular = specularS * spec * vec3(0.8f, 0.8f, 0.8f);
    
    vec3 result = (ambient + diffuse + specular) * ourColor;
    color = vec4(result, 1.0f);
    
}
