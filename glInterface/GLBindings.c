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
PointerGetShaderiv sigGetShaderiv = NULL;
PointerGetShaderInfoLog sigGetShaderInfoLog = NULL;

//fragshader
PointerBindFragDataLocation sigBindFragDataLocation = NULL;

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
PointerGetProgramiv sigGetProgramiv = NULL;
PointerGetProgramInfoLog sigGetProgramInfoLog = NULL;

//uniforms
PointerProgramUniform1f sigProgramUniform1f = NULL;
PointerProgramUniform2f sigProgramUniform2f = NULL;
PointerProgramUniform3f sigProgramUniform3f = NULL;
PointerProgramUniform4f sigProgramUniform4f = NULL;
PointerProgramUniform1f sigProgramUniform1i = NULL;
PointerProgramUniform2f sigProgramUniform2i = NULL;
PointerProgramUniform3f sigProgramUniform3i = NULL;
PointerProgramUniform4f sigProgramUniform4i = NULL;
PointerProgramUniform1f sigProgramUniform1ui = NULL;
PointerProgramUniform2f sigProgramUniform2ui = NULL;
PointerProgramUniform3f sigProgramUniform3ui = NULL;
PointerProgramUniform4f sigProgramUniform4ui = NULL;
PointerProgramUniformv sigProgramUniform1fv = NULL;
PointerProgramUniformv sigProgramUniform2fv = NULL;
PointerProgramUniformv sigProgramUniform3fv = NULL;
PointerProgramUniformv sigProgramUniform4fv = NULL;
PointerProgramUniformv sigProgramUniform1iv = NULL;
PointerProgramUniformv sigProgramUniform2iv = NULL;
PointerProgramUniformv sigProgramUniform3iv = NULL;
PointerProgramUniformv sigProgramUniform4iv = NULL;
PointerProgramUniformv sigProgramUniform1uiv = NULL;
PointerProgramUniformv sigProgramUniform2uiv = NULL;
PointerProgramUniformv sigProgramUniform3uiv = NULL;
PointerProgramUniformv sigProgramUniform4uiv = NULL;
PointerProgramUniformMatrix sigProgramUniformMatrix2fv   = NULL;
PointerProgramUniformMatrix sigProgramUniformMatrix3fv   = NULL;
PointerProgramUniformMatrix sigProgramUniformMatrix4fv   = NULL;
PointerProgramUniformMatrix sigProgramUniformMatrix2x3fv = NULL;
PointerProgramUniformMatrix sigProgramUniformMatrix3x2fv = NULL;
PointerProgramUniformMatrix sigProgramUniformMatrix2x4fv = NULL;
PointerProgramUniformMatrix sigProgramUniformMatrix4x2fv = NULL;
PointerProgramUniformMatrix sigProgramUniformMatrix3x4fv = NULL;
PointerProgramUniformMatrix sigProgramUniformMatrix4x3fv = NULL;

//query
PointerGetIntegerv sigGetIntegerv = NULL;

//this is the first time ive genuinely used chatgpt, like I just made it write everything below for me, and like just write out 
//everything based on a template I made, so it saved me like ten minutes of typing by doing it instead... awesome
int retrieveFuncs(){
    //suffer
    procMacro(sigBindBuffer, "glBindBuffer");
    procMacro(sigCreateBuffers, "glCreateBuffers");
    procMacro(sigBufferData, "glBufferData");
    procMacro(sigBufferStorage, "glBufferStorage");
    procMacro(sigBufferSubData, "glBufferSubData");
    procMacro(sigDeleteBuffers, "glDeleteBuffers");

    //shader
    procMacro(sigCreateShader, "glCreateShader");
    procMacro(sigShaderSource, "glShaderSource");
    procMacro(sigCompileShader, "glCompileShader");
    procMacro(sigAttachShader, "glAttachShader");
    procMacro(sigDeleteShader, "glDeleteShader");
    procMacro(sigReleaseShaderCompiler, "glReleaseShaderCompiler");
    procMacro(sigGetShaderiv, "glGetShaderiv");
    procMacro(sigGetShaderInfoLog, "glGetShaderInfoLog");

    //fragshader
    procMacro(sigBindFragDataLocation, "glBindFragDataLocation");

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
    procMacro(sigGetProgramiv, "glGetProgramiv");
    procMacro(sigGetProgramInfoLog, "glGetProgramInfoLog");

    //uniforms
    procMacro(sigProgramUniform1f, "glProgramUniform1f");
    procMacro(sigProgramUniform2f, "glProgramUniform2f");
    procMacro(sigProgramUniform3f, "glProgramUniform3f");
    procMacro(sigProgramUniform4f, "glProgramUniform4f");

    procMacro(sigProgramUniform1i, "glProgramUniform1i");
    procMacro(sigProgramUniform2i, "glProgramUniform2i");
    procMacro(sigProgramUniform3i, "glProgramUniform3i");
    procMacro(sigProgramUniform4i, "glProgramUniform4i");

    procMacro(sigProgramUniform1ui, "glProgramUniform1ui");
    procMacro(sigProgramUniform2ui, "glProgramUniform2ui");
    procMacro(sigProgramUniform3ui, "glProgramUniform3ui");
    procMacro(sigProgramUniform4ui, "glProgramUniform4ui");

    procMacro(sigProgramUniform1fv, "glProgramUniform1fv");
    procMacro(sigProgramUniform2fv, "glProgramUniform2fv");
    procMacro(sigProgramUniform3fv, "glProgramUniform3fv");
    procMacro(sigProgramUniform4fv, "glProgramUniform4fv");

    procMacro(sigProgramUniform1iv, "glProgramUniform1iv");
    procMacro(sigProgramUniform2iv, "glProgramUniform2iv");
    procMacro(sigProgramUniform3iv, "glProgramUniform3iv");
    procMacro(sigProgramUniform4iv, "glProgramUniform4iv");

    procMacro(sigProgramUniform1uiv, "glProgramUniform1uiv");
    procMacro(sigProgramUniform2uiv, "glProgramUniform2uiv");
    procMacro(sigProgramUniform3uiv, "glProgramUniform3uiv");
    procMacro(sigProgramUniform4uiv, "glProgramUniform4uiv");

    procMacro(sigProgramUniformMatrix2fv,   "glProgramUniformMatrix2fv");
    procMacro(sigProgramUniformMatrix3fv,   "glProgramUniformMatrix3fv");
    procMacro(sigProgramUniformMatrix4fv,   "glProgramUniformMatrix4fv");
    procMacro(sigProgramUniformMatrix2x3fv, "glProgramUniformMatrix2x3fv");
    procMacro(sigProgramUniformMatrix3x2fv, "glProgramUniformMatrix3x2fv");
    procMacro(sigProgramUniformMatrix2x4fv, "glProgramUniformMatrix2x4fv");
    procMacro(sigProgramUniformMatrix4x2fv, "glProgramUniformMatrix4x2fv");
    procMacro(sigProgramUniformMatrix3x4fv, "glProgramUniformMatrix3x4fv");
    procMacro(sigProgramUniformMatrix4x3fv, "glProgramUniformMatrix4x3fv");

    //querry
    procMacro(sigGetIntegerv, "glGetIntegerv");

    return 1;
}
#pragma GCC diagnostic pop