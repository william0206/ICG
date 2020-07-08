#version 330 core

in vec3 ourColor;
in float ourLife;
in vec3 FragPos;
in vec3 Normal;

out vec4 color;

uniform vec3 lightPos;
uniform vec3 viewPos;
uniform float kd;
uniform float ks;


void main()
{
    /*
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
    */
     //0.5f, 0.8f, 0.9f
     //0.8 0.2 0.3
    vec3 e = (vec3(0.5f, 0.8f, 0.9f) + vec3(0.8f, 0.2f, 0.3f))/2;
    vec3 delta = (ourColor - e) / 5;
    if(ourLife > 2.6)
        color = vec4(ourColor , 1.0f);
    else if(ourLife > 2.0)
        color = vec4(ourColor - delta,1.0f);
    else if(ourLife > 1.6)
        color = vec4(ourColor - 2 * delta,1.0f);
    else if(ourLife > 1.2)
        color = vec4(ourColor - 3 * delta,1.0f);
    else if(ourLife > 0.4)
        color = vec4(ourColor - 4 * delta,1.0f);
    else
        color = vec4(ourColor - 5 * delta,1.0f);
        
    
}
