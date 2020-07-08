#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;
layout (location = 2) in vec3 normal;

out vec3 Normal;
out vec3 FragPos;



uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

out vec3 ourColor;

void main()
{
    gl_Position = proj * view * model * vec4(position, 1.0f);
    ourColor= color;
    FragPos = vec3( model * vec4(position, 1.0f));
    Normal = mat3(transpose(inverse(model))) * normal;
    
}
