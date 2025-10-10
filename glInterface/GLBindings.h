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
// typedef void(*PointerEnableVertexAttribArray)(GLuint location);
// typedef void(*PointerDisableVertexAttribArray)(GLuint location);

extern PointerCreateVertexArrays sigCreateVertexArrays;
extern PointerBindVertexArray sigBindVertexArray;
//this func rolls vertexattribformat, vertexattribbinding, vertexbindvertexbuffer into one func, just needs to call
//call createvertexarrays, bindvertexarray, and ... bindbuffer beforehand
extern PointerVertexAttribPointer sigVertexAttribPointer;
//both use the currently bound vertex array object
// extern PointerEnableVertexAttribArray sigEnableVertexArrayAttrib;
// extern PointerDisableVertexAttribArray sigDisableVertexArrayAttrib;

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

//uniforms
typedef void(*PointerProgramUniform1f)(GLuint program, GLint location, GLfloat v0);
typedef void(*PointerProgramUniform2f)(GLuint program, GLint location, GLfloat v0, GLfloat v1);
typedef void(*PointerProgramUniform3f)(GLuint program, GLint location, GLfloat v0, GLfloat v1, GLfloat v2);
typedef void(*PointerProgramUniform4f)(GLuint program, GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);
typedef void(*PointerProgramUniformv)(GLuint program, GLint location, GLsizei count, const GLfloat* value);
typedef void(*PointerProgramUniformMatrix)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);

extern PointerProgramUniform1f sigProgramUniform1f;
extern PointerProgramUniform2f sigProgramUniform2f;
extern PointerProgramUniform3f sigProgramUniform3f;
extern PointerProgramUniform4f sigProgramUniform4f;
extern PointerProgramUniform1f sigProgramUniform1i;
extern PointerProgramUniform2f sigProgramUniform2i;
extern PointerProgramUniform3f sigProgramUniform3i;
extern PointerProgramUniform4f sigProgramUniform4i;
extern PointerProgramUniform1f sigProgramUniform1ui;
extern PointerProgramUniform2f sigProgramUniform2ui;
extern PointerProgramUniform3f sigProgramUniform3ui;
extern PointerProgramUniform4f sigProgramUniform4ui;
extern PointerProgramUniformv sigProgramUniform1fv;
extern PointerProgramUniformv sigProgramUniform2fv;
extern PointerProgramUniformv sigProgramUniform3fv;
extern PointerProgramUniformv sigProgramUniform4fv;
extern PointerProgramUniformv sigProgramUniform1iv;
extern PointerProgramUniformv sigProgramUniform2iv;
extern PointerProgramUniformv sigProgramUniform3iv;
extern PointerProgramUniformv sigProgramUniform4iv;
extern PointerProgramUniformv sigProgramUniform1uiv;
extern PointerProgramUniformv sigProgramUniform2uiv;
extern PointerProgramUniformv sigProgramUniform3uiv;
extern PointerProgramUniformv sigProgramUniform4uiv;
extern PointerProgramUniformMatrix sigProgramUniformMatrix2fv;
extern PointerProgramUniformMatrix sigProgramUniformMatrix3fv;
extern PointerProgramUniformMatrix sigProgramUniformMatrix4fv;
extern PointerProgramUniformMatrix sigProgramUniformMatrix2x3fv;
extern PointerProgramUniformMatrix sigProgramUniformMatrix3x2fv;
extern PointerProgramUniformMatrix sigProgramUniformMatrix2x4fv;
extern PointerProgramUniformMatrix sigProgramUniformMatrix4x2fv;
extern PointerProgramUniformMatrix sigProgramUniformMatrix3x4fv;
extern PointerProgramUniformMatrix sigProgramUniformMatrix4x3fv;


//query
typedef void(*PointerGetIntegerv)(GLenum pname, GLint *data);

extern PointerGetIntegerv sigGetIntegerv;

/*
* 
* Returns 0 If Failed, 1 If All Funcs Bound
* 
*/
int retrieveFuncs();