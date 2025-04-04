#version 460 core

out vec4 fragColor;
//notice the matching name and location for input and output variables
layout(location = 1) in vec3 Pos;
vec3 lame = vec3(0.5);

void main()
{
    fragColor = vec4(lame - Pos, 1.0);
}