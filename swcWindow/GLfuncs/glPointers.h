#ifndef GLPOINTERS_H
#define GLPOINTERS_H

#include <GL/gl.h>
#include <GL/glx.h>
#include <stdio.h>
#include "../swcDef.h"

//buffer
typedef void(*PointerCreateBuffers)(GLsizei size, GLuint* bufferArray);
typedef void(*PointerBindBuffer)(GLenum type, GLuint buffer);
typedef void(*PointerBufferData)(GLenum type, GLsizeiptr size, const void *bufferData, GLenum usage);//mutable
typedef void(*PointerNamedBufferData)(GLuint buffer, GLsizeiptr size, const void* bufferData, GLenum usage);
typedef void(*PointerBufferStorage)(GLenum type, GLsizeiptr size, const void *bufferData, GLbitfield flags);//immutable
typedef void(*PointerNamedBufferStorage)(GLuint buffer, GLsizeiptr size, const void* bufferData, GLbitfield flags);
typedef void(*PointerBufferSubData)(GLenum type, GLintptr offset, GLsizeiptr size, const void* bufferData);
typedef void(*PointerNamedBufferSubData)(GLuint buffer, GLintptr offset, GLsizeiptr size, const void* bufferData);
typedef void(*PointerDeleteBuffers)(GLsizei count, const GLuint* buffers);

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

//fragshader
typedef void(*PointerBindFragDataLocation)(GLuint program, GLuint color_attachment_point, const char* color_name_identical_to_whats_in_the_shader);

//vertex array objects
typedef void(*PointerCreateVertexArrays)(GLsizei count, GLuint *VAOArray);
typedef void(*PointerBindVertexArray)(GLuint array);
typedef void(*PointerVertexAttribPointer)(GLuint location, GLint componentSize, GLenum type, GLboolean normalized, GLsizei stride, const void *pointer);
typedef void(*PointerEnableVertexAttribArray)(GLuint location);
typedef void(*PointerDisableVertexAttribArray)(GLuint location);

//program
typedef GLuint(*PointerCreateProgram)(void);
typedef void(*PointerLinkProgram)(GLuint program);
typedef void(*PointerUseProgram)(GLuint program);
typedef void(*PointerDeleteProgram)(GLuint program);
typedef void(*PointerGetProgramiv)(GLuint program, GLenum pname, GLint *params);
typedef void(*PointerGetProgramInfoLog)(GLuint program, GLsizei maxLength, GLsizei *length, GLchar *infoLog);

//uniforms
typedef void(*PointerProgramUniform1f)(GLuint program, GLint location, GLfloat v0);
typedef void(*PointerProgramUniform2f)(GLuint program, GLint location, GLfloat v0, GLfloat v1);
typedef void(*PointerProgramUniform3f)(GLuint program, GLint location, GLfloat v0, GLfloat v1, GLfloat v2);
typedef void(*PointerProgramUniform4f)(GLuint program, GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);
typedef void(*PointerProgramUniformv)(GLuint program, GLint location, GLsizei count, const GLfloat* value);
typedef void(*PointerProgramUniformMatrix)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);

//query
typedef void(*PointerGetIntegerv)(GLenum pname, GLint *data);

typedef struct{
    PointerCreateBuffers sigCreateBuffers;
    PointerBindBuffer sigBindBuffer;
    PointerBufferData sigBufferData;
    PointerNamedBufferData sigNamedBufferData;
    PointerBufferStorage sigBufferStorage;
    PointerNamedBufferStorage sigNamedBufferStorage;
    PointerBufferSubData sigBufferSubData;
    PointerNamedBufferSubData sigNamedBufferSubData;
    PointerDeleteBuffers sigDeleteBuffers;
    PointerCreateShader sigCreateShader;
    PointerShaderSource sigShaderSource;
    PointerCompileShader sigCompileShader;
    PointerAttachShader sigAttachShader;
    PointerDeleteShader sigDeleteShader;
    PointerReleaseShaderCompiler sigReleaseShaderCompiler;
    PointerGetShaderiv sigGetShaderiv;
    PointerGetShaderInfoLog sigGetShaderInfoLog;
    PointerBindFragDataLocation sigBindFragDataLocation;
    PointerCreateVertexArrays sigCreateVertexArrays;
    PointerBindVertexArray sigBindVertexArray;
    PointerVertexAttribPointer sigVertexAttribPointer;
    PointerEnableVertexAttribArray sigEnableVertexAttribArray;
    PointerDisableVertexAttribArray sigDisableVertexAttribArray;
    PointerCreateProgram sigCreateProgram;
    PointerLinkProgram sigLinkProgram;
    PointerUseProgram sigUseProgram;
    PointerDeleteProgram sigDeleteProgram;
    PointerGetProgramiv sigGetProgramiv;
    PointerGetProgramInfoLog sigGetProgramInfoLog;
    PointerProgramUniform1f sigProgramUniform1f;
    PointerProgramUniform2f sigProgramUniform2f;
    PointerProgramUniform3f sigProgramUniform3f;
    PointerProgramUniform4f sigProgramUniform4f;
    PointerProgramUniform1f sigProgramUniform1i;
    PointerProgramUniform2f sigProgramUniform2i;
    PointerProgramUniform3f sigProgramUniform3i;
    PointerProgramUniform4f sigProgramUniform4i;
    PointerProgramUniform1f sigProgramUniform1ui;
    PointerProgramUniform2f sigProgramUniform2ui;
    PointerProgramUniform3f sigProgramUniform3ui;
    PointerProgramUniform4f sigProgramUniform4ui;
    PointerProgramUniformv sigProgramUniform1fv;
    PointerProgramUniformv sigProgramUniform2fv;
    PointerProgramUniformv sigProgramUniform3fv;
    PointerProgramUniformv sigProgramUniform4fv;
    PointerProgramUniformv sigProgramUniform1iv;
    PointerProgramUniformv sigProgramUniform2iv;
    PointerProgramUniformv sigProgramUniform3iv;
    PointerProgramUniformv sigProgramUniform4iv;
    PointerProgramUniformv sigProgramUniform1uiv;
    PointerProgramUniformv sigProgramUniform2uiv;
    PointerProgramUniformv sigProgramUniform3uiv;
    PointerProgramUniformv sigProgramUniform4uiv;
    PointerProgramUniformMatrix sigProgramUniformMatrix2fv;
    PointerProgramUniformMatrix sigProgramUniformMatrix3fv;
    PointerProgramUniformMatrix sigProgramUniformMatrix4fv;
    PointerProgramUniformMatrix sigProgramUniformMatrix2x3fv;
    PointerProgramUniformMatrix sigProgramUniformMatrix3x2fv;
    PointerProgramUniformMatrix sigProgramUniformMatrix2x4fv;
    PointerProgramUniformMatrix sigProgramUniformMatrix4x2fv;
    PointerProgramUniformMatrix sigProgramUniformMatrix3x4fv;
    PointerProgramUniformMatrix sigProgramUniformMatrix4x3fv;
    PointerGetIntegerv sigGetIntegerv;
}glFuncPointers;

#endif