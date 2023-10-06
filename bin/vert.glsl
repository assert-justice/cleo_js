#version 330 core
in vec3 aPos;
in vec2 aTexCoord;

out vec2 TexCoord;

uniform mat4 camera;
uniform mat4 sprite;
uniform mat4 coord;

void main()
{
    gl_Position = camera * sprite * vec4(aPos, 1.0);
    TexCoord = (coord * vec4(aTexCoord, 1.0, 1.0)).xy;
}