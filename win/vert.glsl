#version 330 core
in vec3 aPos;
in vec2 aTexCoord;

out vec2 TexCoord;

uniform mat4 camera;
uniform mat4 sprite;
uniform vec4 dimensions;

void main()
{
    gl_Position = camera * sprite * vec4(aPos, 1.0);
    TexCoord = aTexCoord * dimensions.zw + dimensions.xy;
}