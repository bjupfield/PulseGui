#include "GLBindings.h"

//this is the first time I've actually used a macro
#define funnyMacro(function, name) \
    function = glXGetProcAddressARB((const GLubyte*)name); \
    if(function == NULL) \
    { \
        printf("Failed to load %s", name); \
        return 0; \
    }

#define secondMacro(name2) printf("My name is %s", name2);

void(*SINCreateBuffers)(GLsizei, GLuint*) = 0;
void(*SINDeleteBuffers)(GLsizei, GLuint*) = 0;
void(*SINBindBuffer)(GLenum, GLuint) = 0;
void(*SINBufferData)(GLenum, GLsizeiptr, const void*, GLenum) = 0;
void(*SINBufferStorage)(GLenum, GLsizeiptr, const void*, GLbitfield) = 0;
void(*SINBufferSubData)(GLenum, GLintptr, GLsizeiptr, const void*) = 0;

GLuint(*SINCreateProgram)() = 0;
void(*SINAttachShader)(GLuint, GLuint) = 0;
void(*SINDetachShader)(GLuint, GLuint) = 0;
void(*SINLinkProgram)(GLuint) = 0;
void(*SINUseProgram)(GLuint) = 0;
void(*SINDeleteProgram)(GLuint) = 0;

void(*SINVertexAttribPointer)(GLuint, GLuint, GLenum, GLboolean, GLsizei, const void *) = 0;
void(*SINCreateVertexArrays)(GLsizei, GLuint*) = 0;
void(*SINBindVertexArray)(GLuint) = 0;
GLuint(*SINCreateShader)(GLenum) = 0;

void(*SINShaderSource)(GLuint, GLsizei, const GLchar **, const GLint *) = 0;
void(*SINCompileShader)(GLuint) = 0;
void(*SINDeleteShader)(GLuint) = 0;
void(*SINReleaseShaderCompiler)() = 0;


//retry
PointerCreateBuffers myCreateBuffers = NULL;
PointerBindBuffers myBindBuffers = NULL;

int retrieveFuncs(){
    /*
    *
    * Initialize all Funcs Here, Or At Least Thats probably what I will do
    * 
    */
    // SINCreateBuffers = glXGetProcAddress((const GLubyte*)"glCreateBuffers");
    // SINDeleteBuffers = glXGetProcAddress((const GLubyte*)"glDeleteBuffers");
    // SINBindBuffer = glXGetProcAddress((const GLubyte*)"glBindBuffer");
    // SINBufferData = glXGetProcAddress((const GLubyte*)"glBufferData");
    // SINBufferStorage = glXGetProcAddress((const GLubyte*)"glBufferStorage");
    // SINBufferSubData = glXGetProcAddress((const GLubyte*)"glBufferSubData");

    // SINCreateProgram = glXGetProcAddress((const GLubyte*)"glCreateProgram");
    // SINAttachShader = glXGetProcAddress((const GLubyte*)"glAttachShader");
    // SINDetachShader = glXGetProcAddress((const GLubyte*)"glDetachShader");
    // SINLinkProgram = glXGetProcAddress((const GLubyte*)"glLinkProgram");
    // SINUseProgram = glXGetProcAddress((const GLubyte*)"glUseProgram");
    // SINDeleteProgram = glXGetProcAddress((const GLubyte*)"glDeleteProgram");

    // SINVertexAttribPointer = glXGetProcAddress((const GLubyte*)"glVertexAttribPointer");
    // SINCreateShader = glXGetProcAddress((const GLubyte*)"glCreateShader");

    // SINShaderSource = glXGetProcAddress((const GLubyte*)"glShaderSource");
    // SINCompileShader = glXGetProcAddress((const GLubyte*)"glCompileShader");
    // SINDeleteShader = glXGetProcAddress((const GLubyte*)"glDeleteShader");
    // SINReleaseShaderCompiler = glXGetProcAddress((const GLubyte*)"glReleaseShaderCompiler");


    //retry 2
    funnyMacro(myBindBuffers, "glBindBuffers");
    funnyMacro(myCreateBuffers, "glCreateBuffers");
    // //retry
    // myBindBuffers = funnyMacro(myBindBuffers, "badBindBuffers");
    // myCreateBuffers = funnyMacro(myCreateBuffers, "glCreateBuffers");

    secondMacro("mine");

    return 1;
}