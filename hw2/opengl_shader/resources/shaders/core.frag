#version 330 core
out vec4 frag_color;

uniform sampler2D TEXTURE;
in vec2 frag_UV;

void main()
{
    frag_color = texture(TEXTURE, frag_UV);
}
