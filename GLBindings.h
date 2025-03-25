#include <GL/gl.h>
#include <GL/glx.h>
#include <stdio.h>


//retry
typedef void (*PointerCreateBuffers)(GLsizei, GLuint);
typedef void (*PointerBindBuffers)(GLenum, GLuint*);

extern PointerCreateBuffers myCreateBuffers;
extern PointerBindBuffers myBindBuffers;

//func call list, I want this to be relatively lightweight I guess so I will just retrieve func
/*
*
*   Buffer Creation
*
*/
extern APIENTRY void(*SINCreateBuffers)(GLsizei, GLuint*);
extern APIENTRY void(*SINDeleteBuffers)(GLsizei, GLuint*);
extern APIENTRY void(*SINBindBuffer)(GLenum, GLuint);
extern APIENTRY void(*SINBufferData)(GLenum, GLsizeiptr, const void*, GLenum);//mutable
extern APIENTRY void(*SINBufferStorage)(GLenum, GLsizeiptr, const void*, GLbitfield);//immutable
extern APIENTRY void(*SINBufferSubData)(GLenum, GLintptr, GLsizeiptr, const void*);

/*
*
* Program Creation and Linking
*
*/
extern APIENTRY GLuint(*SINCreateProgram)();
extern APIENTRY void(*SINAttachShader)(GLuint, GLuint);
extern APIENTRY void(*SINDetachShader)(GLuint, GLuint);
extern APIENTRY void(*SINLinkProgram)(GLuint);
extern APIENTRY void(*SINUseProgram)(GLuint);
extern APIENTRY void(*SINDeleteProgram)(GLuint);
//this func rolls vertexattribformat, vertexattribbinding, vertexbindvertexbuffer into one func, just needs to call
//call createvertexarrays, bindvertexarray, and ... bindbuffer beforehand
extern APIENTRY void(*SINVertexAttribPointer)(GLuint, GLuint, GLenum, GLboolean, GLsizei, const void *);
extern APIENTRY void(*SINCreateVertexArrays)(GLsizei, GLuint*);
extern APIENTRY void(*SINBindVertexArray)(GLuint);
extern APIENTRY GLuint(*SINCreateShader)(GLenum);
//P1 = Shader, P2 = String Amount (Always 1), P3 = string array, P4 = 0, always
extern APIENTRY void(*SINShaderSource)(GLuint, GLsizei, const GLchar **, const GLint *);
extern APIENTRY void(*SINCompileShader)(GLuint);
extern APIENTRY void(*SINAttachShader)(GLuint, GLuint);
extern APIENTRY void(*SINDeleteShader)(GLuint);
extern APIENTRY void(*SINReleaseShaderCompiler)();



/*
* 
* Returns 0 If Failed, 1 If All Funcs Bound
* 
*/
int retrieveFuncs();