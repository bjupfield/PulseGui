#version 460 core

/*
*
* Change Version To 460 For Compilation
*
*/

layout(location = 0) in vec3 pos;
layout(location = 1) out vec3 Pos;

void main()
{
    gl_Position = vec4(pos.x, pos.y, pos.z, 1.0);
    Pos = pos;
}