#version 450

/*
*
* Change Version To 460 For Compilation
*
*/

layout(location = 0) in vec3 pos;

void main()
{
    gl_Position = vec4(pos.x, pox.y, pos.z, 1.0);
}