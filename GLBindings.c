#include "GLBindings.h"
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wincompatible-pointer-types"
//this is the first time I've actually used a macro
//sadly cant make the pragma target the macro... very undestandable
#define procMacro(function, name) \
    function = glXGetProcAddressARB((const GLubyte*)name); \
    if(function == NULL) \
    { \
        printf("Failed to load %s", name); \
        return 0; \
    }



//buffer
PointerCreateBuffers sigCreateBuffers = NULL;
PointerBindBuffer sigBindBuffer = NULL;
PointerBufferData sigBufferData = NULL;
PointerBufferStorage sigBufferStorage = NULL;
PointerBufferSubData sigBufferSubData = NULL;
PointerDeleteBuffers sigDeleteBuffers = NULL;

//shader
PointerCreateShader sigCreateShader = NULL;
PointerShaderSource sigShaderSource = NULL;
PointerCompileShader sigCompileShader = NULL;
PointerAttachShader sigAttachShader = NULL;
PointerDeleteShader sigDeleteShader = NULL;
PointerReleaseShaderCompiler sigReleaseShaderCompiler = NULL;

//vao
PointerCreateVertexArrays sigCreateVertexArrays = NULL;
PointerBindVertexArray sigBindVertexArray = NULL;
PointerVertexAttribPointer sigVertexAttribPointer = NULL;
PointerEnableVertexAttribArray sigEnableVertexAttribArray = NULL;
PointerDisableVertexAttribArray sigDisableVertexAttribArray = NULL;

//program
PointerCreateProgram sigCreateProgram = NULL;
PointerLinkProgram sigLinkProgram = NULL;
PointerUseProgram sigUseProgram = NULL;
PointerDeleteProgram sigDeleteProgram = NULL;

//this is the first time ive genuinely used chatgpt, like I just made it write everything below for me, and like just write out 
//everything based on a template I made, so it saved me like ten minutes of typing by doing it instead... awesome
int retrieveFuncs(){
    //Buffer
    procMacro(sigBindBuffer, "glBindBuffer");
    procMacro(sigCreateBuffers, "glCreateBuffers");
    procMacro(sigBufferData, "glBufferData");
    procMacro(sigBufferStorage, "glBufferStorage");
    procMacro(sigBufferSubData, "glBufferSubData");
    procMacro(sigDeleteBuffers, "glDeleteBuffers");

    //Shader
    procMacro(sigCreateShader, "glCreateShader");
    procMacro(sigShaderSource, "glShaderSource");
    procMacro(sigCompileShader, "glCompileShader");
    procMacro(sigAttachShader, "glAttachShader");
    procMacro(sigDeleteShader, "glDeleteShader");
    procMacro(sigReleaseShaderCompiler, "glReleaseShaderCompiler");

    //vao
    procMacro(sigCreateVertexArrays, "glCreateVertexArrays");
    procMacro(sigBindVertexArray, "glBindVertexArray");
    procMacro(sigVertexAttribPointer, "glVertexAttribPointer");
    procMacro(sigEnableVertexAttribArray, "glEnableVertexAttribArray");
    procMacro(sigDisableVertexAttribArray, "glDisableVertexAttribArray");


    //program
    procMacro(sigCreateProgram, "glCreateProgram");
    procMacro(sigLinkProgram, "glLinkProgram");
    procMacro(sigUseProgram, "glUseProgram");
    procMacro(sigDeleteProgram, "glDeleteProgram");

    return 1;
}
#pragma GCC diagnostic pop