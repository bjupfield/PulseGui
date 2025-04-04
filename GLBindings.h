#include <GL/gl.h>
#include <GL/glx.h>
#include <stdio.h>


//retry
//buffer
typedef void(*PointerCreateBuffers)(GLsizei size, GLuint* bufferArray);
typedef void(*PointerBindBuffer)(GLenum type, GLuint buffer);
typedef void(*PointerBufferData)(GLenum type, GLsizeiptr size, const void *bufferData, GLenum usage);//mutable
typedef void(*PointerBufferStorage)(GLenum type, GLsizeiptr size, const void *bufferData, GLbitfield flags);//immutable
typedef void(*PointerBufferSubData)(GLenum type, GLintptr offset, GLsizeiptr size, const void* bufferData);
typedef void(*PointerDeleteBuffers)(GLsizei count, const GLuint* buffers);


extern PointerCreateBuffers sigCreateBuffers;
extern PointerBindBuffer sigBindBuffer;
extern PointerBufferData sigBufferData;
extern PointerBufferStorage sigBufferStorage;
extern PointerBufferSubData sigBufferSubData;
extern PointerDeleteBuffers sigDeleteBuffers;


//shader
typedef GLuint(*PointerCreateShader)(GLenum type);
//P1 = Shader, P2 = String Amount (Always 1), P3 = string array, P4 = 0, always
typedef void(*PointerShaderSource)(GLuint shader, GLsizei countSHOULDALWAYSBE1, const GLchar** fileString, const GLint* leaveASZERO);
typedef void(*PointerCompileShader)(GLuint shader);
typedef void(*PointerAttachShader)(GLuint program, GLuint shader);
typedef void(*PointerDeleteShader)(GLuint Shader);
typedef void(*PointerReleaseShaderCompiler)(void);
typedef void(*PointerGetShaderiv)(GLuint shader, GLenum pname, GLint *params);
typedef void(*PointerGetShaderInfoLog)(GLuint shader, GLsizei maxLength, GLsizei *length, GLchar *infoLog);

extern PointerCreateShader sigCreateShader;
extern PointerShaderSource sigShaderSource;
extern PointerCompileShader sigCompileShader;
extern PointerAttachShader sigAttachShader;
extern PointerDeleteShader sigDeleteShader;
extern PointerReleaseShaderCompiler sigReleaseShaderCompiler;
extern PointerGetShaderiv sigGetShaderiv;
extern PointerGetShaderInfoLog sigGetShaderInfoLog;

//fragshader
typedef void(*PointerBindFragDataLocation)(GLuint program, GLuint color_attachment_point, const char* color_name_identical_to_whats_in_the_shader);

extern PointerBindFragDataLocation sigBindFragDataLocation;

//vertex array objects
typedef void(*PointerCreateVertexArrays)(GLsizei count, GLuint *VAOArray);
typedef void(*PointerBindVertexArray)(GLuint array);
typedef void(*PointerVertexAttribPointer)(GLuint location, GLint componentSize, GLenum type, GLboolean normalized, GLsizei stride, const void *pointer);
typedef void(*PointerEnableVertexAttribArray)(GLuint location);
typedef void(*PointerDisableVertexAttribArray)(GLuint location);

extern PointerCreateVertexArrays sigCreateVertexArrays;
extern PointerBindVertexArray sigBindVertexArray;
//this func rolls vertexattribformat, vertexattribbinding, vertexbindvertexbuffer into one func, just needs to call
//call createvertexarrays, bindvertexarray, and ... bindbuffer beforehand
extern PointerVertexAttribPointer sigVertexAttribPointer;
//both use the currently bound vertex array object
extern PointerEnableVertexAttribArray sigEnableVertexAttribArray;
extern PointerDisableVertexAttribArray sigDisableVertexAttribArray;

//program
typedef GLuint(*PointerCreateProgram)(void);
typedef void(*PointerLinkProgram)(GLuint program);
typedef void(*PointerUseProgram)(GLuint program);
typedef void(*PointerDeleteProgram)(GLuint program);
typedef void(*PointerGetProgramiv)(GLuint program, GLenum pname, GLint *params);
typedef void(*PointerGetProgramInfoLog)(GLuint program, GLsizei maxLength, GLsizei *length, GLchar *infoLog);

extern PointerCreateProgram sigCreateProgram;
extern PointerLinkProgram sigLinkProgram;
extern PointerUseProgram sigUseProgram;
extern PointerDeleteProgram sigDeleteProgram;
extern PointerGetProgramiv sigGetProgramiv;
extern PointerGetProgramInfoLog sigGetProgramInfoLog;


/*
* 
* Returns 0 If Failed, 1 If All Funcs Bound
* 
*/
int retrieveFuncs();