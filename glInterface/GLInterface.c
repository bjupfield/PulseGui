#include "GLInterface.h"

uint8_t funcsBound = 0;
GLXFBConfig initConfig;
GLXFBConfig config;
GLXWindow mainWindow;
GLXContext context;
GLuint vertexBuffer;
GLuint program = 0;
GLuint vao = 0;

typedef struct contextWinAssociation
{
    GLXWindow win;
    GLXContext context;
    uint64_t name;//size aligns the structs
}contextWinAssociation;

uint64_t winsSize;
uint64_t winCount;
contextWinAssociation* wins;


/**
 * @brief Update Uniforms attempt at "variadic Function"
 * Count Goes After Array Pointer For Vector and Matrix Assignments
 * Transpose Goes after Count for Matrix Assignments
 * 
 * Returns 1 if Success, 0 if Fail
 * 
 */
uint32_t update_Uniform(GLIProgram* program, const char* name, ...)
{
    // va_list args;
    va_list args;
    va_start(args, name);
    
    //uint32_t type = grabUnifromType(program, name);
    enum uniformType type = 0;
    // uint32_t location = grabUniformLocation(program, name);
    uint32_t location = 0;

    uint32_t sBuffer[4];
    uint32_t* buffer;
    if(type < 0 || type > 31)
        return 0;
    if(type < _1fv)
    {
        sBuffer[0] = va_arg(args, uint32_t);
        if(type > _1ui)
            sBuffer[1] = va_arg(args, uint32_t);
        if(type > _2ui)
            sBuffer[2] = va_arg(args, uint32_t);
        if(type > _3ui)
            sBuffer[3] = va_arg(args, uint32_t);
        switch(type)
        {
            case _1f: 
            {
                sigProgramUniform1f(program->program, location, (GLfloat)sBuffer[0]);
                break;
            }
            case _1i:
            {
                sigProgramUniform1i(program->program, location, (GLfloat)sBuffer[0]);
                break;
            }
            case _1ui:
            {
                sigProgramUniform1ui(program->program, location, (GLfloat)sBuffer[0]);
                break;
            }


            case _2f:
            {
                sigProgramUniform2f(program-program, location, (GLfloat)sBuffer[0], (GLfloat)sBuffer[1]);
                break;
            }
            case _2i:
            {
                sigProgramUniform2i(program->program, location, (GLfloat)sBuffer[0], (GLfloat)sBuffer[1]);
                break;
            }
            case _2ui:
            {
                sigProgramUniform2ui(program->program, location, (GLfloat)sBuffer[0], (GLfloat)sBuffer[1]);
                break;
            }


            case _3f:
            {
                sigProgramUniform3f(program->program, location, (GLfloat)sBuffer[0], (GLfloat)sBuffer[1], (GLfloat)sBuffer[2]);
                break;
            }
            case _3i:
            {
                sigProgramUniform3i(program->program, location, (GLfloat)sBuffer[0], (GLfloat)sBuffer[1], (GLfloat)sBuffer[2]);
                break;
            }
            case _3ui:
            {
                sigProgramUniform3ui(program->program, location, (GLfloat)sBuffer[0], (GLfloat)sBuffer[1], (GLfloat)sBuffer[2]);
                break;
            }


            case _4f:
            {
                sigProgramUniform4f(program->program, location, (GLfloat)sBuffer[0], (GLfloat)sBuffer[1], (GLfloat)sBuffer[2], (GLfloat)sBuffer[3]);
                break;
            }
            case _4i:
            {
                sigProgramUniform4i(program->program, location, (GLfloat)sBuffer[0], (GLfloat)sBuffer[1], (GLfloat)sBuffer[2], (GLfloat)sBuffer[3]);
                break;
            }
            case _4ui:
            {
                sigProgramUniform4ui(program->program, location, (GLfloat)sBuffer[0], (GLfloat)sBuffer[1], (GLfloat)sBuffer[2], (GLfloat)sBuffer[3]);
                break;
            }
        }
        va_end(args);
        return 1;

    }
    buffer = va_arg(args, uint32_t*);
    sBuffer[0] = va_arg(args, uint32_t);
    if(type > _4uiv)
        sBuffer[1] = va_arg(args, uint32_t);
    switch(type)
    {
        case _1fv:
        {
            sigProgramUniform1fv(program->program, location, (GLsizei)sBuffer[0], (GLfloat*)buffer);
            break;
        }
        case _2fv:
        {
            sigProgramUniform2fv(program->program, location, (GLsizei)sBuffer[0], (GLfloat*)buffer);
            break;
        }
        case _3fv:
        {
            sigProgramUniform3fv(program->program, location, (GLsizei)sBuffer[0], (GLfloat*)buffer);
            break;
        }
        case _4fv:
        {
            sigProgramUniform4fv(program->program, location, (GLsizei)sBuffer[0], (GLfloat*)buffer);
            break;
        }

        
        case _1iv:
        {
            sigProgramUniform1iv(program->program, location, (GLsizei)sBuffer[0], (GLfloat*)buffer);
            break;
        }
        case _2iv:
        {
            sigProgramUniform2iv(program->program, location, (GLsizei)sBuffer[0], (GLfloat*)buffer);
            break;
        }
        case _3iv:
        {
            sigProgramUniform3iv(program->program, location, (GLsizei)sBuffer[0], (GLfloat*)buffer);
            break;
        }
        case _4iv:
        {
            sigProgramUniform4iv(program->program, location, (GLsizei)sBuffer[0], (GLfloat*)buffer);
            break;
        }

        case _1uiv:
        {
            sigProgramUniform1uiv(program->program, location, (GLsizei)sBuffer[0], (GLfloat*)buffer);
            break;
        }
        case _2uiv:
        {
            sigProgramUniform2uiv(program->program, location, (GLsizei)sBuffer[0], (GLfloat*)buffer);
            break;
        }
        case _3uiv:
        {
            sigProgramUniform3uiv(program->program, location, (GLsizei)sBuffer[0], (GLfloat*)buffer);
            break;
        }
        case _4uiv:
        {
            sigProgramUniform4uiv(program->program, location, (GLsizei)sBuffer[0], (GLfloat*)buffer);
            break;
        }

        case _x2fv:
        {
            sigProgramUniformMatrix2fv(program->program, location, (GLsizei)sBuffer[0], (GLboolean)sBuffer[1], (GLfloat*)buffer);
            break;
        }
        case _x3fv:
        {
            sigProgramUniformMatrix3fv(program->program, location, (GLsizei)sBuffer[0], (GLboolean)sBuffer[1], (GLfloat*)buffer);
            break;
        }
        case _x4fv:
        {
            sigProgramUniformMatrix4fv(program->program, location, (GLsizei)sBuffer[0], (GLboolean)sBuffer[1], (GLfloat*)buffer);
            break;
        }
        case _2x4fv:
        {
            sigProgramUniformMatrix2x4fv(program->program, location, (GLsizei)sBuffer[0], (GLboolean)sBuffer[1], (GLfloat*)buffer);
            break;
        }
        case _3x4fv:
        {
            sigProgramUniformMatrix3x4fv(program->program, location, (GLsizei)sBuffer[0], (GLboolean)sBuffer[1], (GLfloat*)buffer);
            break;
        }
        case _3x2fv:
        {
            sigProgramUniformMatrix3x2fv(program->program, location, (GLsizei)sBuffer[0], (GLboolean)sBuffer[1], (GLfloat*)buffer);
            break;
        }
        case _4x2fv:
        {
            sigProgramUniformMatrix4x2fv(program->program, location, (GLsizei)sBuffer[0], (GLboolean)sBuffer[1], (GLfloat*)buffer);
            break;
        }
        case _4x3fv:
        {
            sigProgramUniformMatrix4x3fv(program->program, location, (GLsizei)sBuffer[0], (GLboolean)sBuffer[1], (GLfloat*)buffer);
            break;
        }
    }

    va_end(args);
    return 1;
}
/*
* stores window height and width for projections
*/
struct xysize
{
    uint32_t x;
    uint32_t y;
}; struct xysize width;


int shaderCreatorAssignerDestroyer(const char* file, int type);

/*
*   Returns True if the X Server has the Glx extension
*/
int queryServer(Display* display)
{
    int error_Base, event_Base;
    Bool bool = glXQueryExtension(display, &error_Base, &event_Base);

    return bool;
}
/*
*   Returns True if a Config was retrieved and Saved that Matches the Specifications
*/
int retrieveConfig(Display* display, uint32_t* confg)
{
    int count;
    GLXFBConfig* configs = glXChooseFBConfig(display, 0, confg, &count);
    if(count > 0)
    {
        
        config = configs[0];
        return count;
    }
    return False;
}

/*
*   Returns Null if No Visual can be Associated, retrieve config must be run before this function
*/
XVisualInfo *retrieveVisual(Display* display)
{
    return glXGetVisualFromFBConfig(display, config);
}



int initializeWindow(Display* display, Window window)
{

    mainWindow = glXCreateWindow(display, config, window, NULL);
    context = glXCreateNewContext(display, config,  GLX_RGBA_TYPE, NULL, True);

    

    glXMakeCurrent(display, mainWindow, context);

    retrieveFuncs();

    //we shouldnt do it like this... this is bad, dont do this... lol... going to keep like this for now

    sigCreateBuffers((GLsizei)1, &vertexBuffer);
    sigBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);


    /*
    * TODO:
    * I don't really know how to check if it was able to create the window, we would need to somehow 
    * check if the errorhandler was triggered and had this error inside it? I think? Don't want to do it now,
    * would rather work on actual rendering
    */

    return 1;
}
/*
* Destroy all objects associated with OpenGl
*/
int destroyGLX(Display* display)
{
    glXDestroyWindow(display, mainWindow);
    sigUseProgram(0);
    sigDeleteProgram(program);
    sigDeleteBuffers(1, &vertexBuffer);
    glXDestroyContext(display, context);
}
int createAttachProgram(Display* display, XWindowAttributes attributes)
{
    

    int csa[2];

    glXMakeCurrent(display, mainWindow, context);

    printf("Window Size: X: %i, Y: %i", attributes.width, attributes.height);

    sigGetIntegerv(GL_MAX_VIEWPORT_DIMS, csa);

    printf("Max size: %i || %i\n", csa[0], csa[1]);

    glViewport(0,0, attributes.width, attributes.height);
    glViewport(0,0, 1, 1);


    program = sigCreateProgram();

    //create vertex shader and link
    shaderCreatorAssignerDestroyer("GlShaders/exampleVert.vert", GL_VERTEX_SHADER);

    //create frag shader and link
    shaderCreatorAssignerDestroyer("GlShaders/exampleFrag.frag", GL_FRAGMENT_SHADER);

    // //release shader compiler here
    sigReleaseShaderCompiler();

    //sigBindFragDataLocation(program, 0, "frag_color");

    // /*
    // * create vao (vertex array object), which stores like the linking information for buffers and locations/indexes
    // * into a single object, so you can say this buffer will be attached to this index/location of a shader
    // * and store it inside this object
    // */

    sigCreateVertexArrays(1, &vao);
    sigBindVertexArray(vao);

    // //bind attributes link program, and use
    // //i really need to make tyedefs for these functions so their readable
    sigVertexAttribPointer(0, 3, GL_FLOAT, GL_TRUE, 0, 0);

    float bdata[6][3] = 
    {
        {0.0, 1.0, 0.0},
        {-1.0, -1.0, 0.0},
        {1.0, -1.0, 0.0},
        {0.5, 1.0, 0.0},
        {1.0, -1.0, 0.0},
        {1.0, 1.0, 0.0},

    };

    sigBufferData(GL_ARRAY_BUFFER, sizeof(bdata), bdata, GL_DYNAMIC_DRAW);

    sigEnableVertexAttribArray(0);

    sigLinkProgram(program);

    //activae the sleeper agent
    sigUseProgram(program);

    glDrawArrays(GL_TRIANGLES, 0, 6);

    glXSwapBuffers(display, mainWindow);

    //this is the suggested way to release the context
    glXMakeCurrent(display, None, NULL);

    return 1;

}
/*
* Resizes Viewport And Calls a Redraw Command, which later on we will need to have like  a draw command that dynamically calls all that we need to draw...
*
*
*/
// 248, 249, 346
// and distance, 250, 269
// onto a subspace, 308
int adjustViewport(Display* display, int width, int height)
{
    glXMakeCurrent(display, mainWindow, context);

    glViewport(0, 0, width, height);
    // glViewport(0, 0, 1, 1);


    //insert dynamic draw commands

    glClearColor(.5, .5, .5, 1);

    glDrawArrays(GL_TRIANGLES, 0, 6);

    glXSwapBuffers(display, mainWindow);

    glXMakeCurrent(display, None, NULL);

    return 1;
}


int shaderCreatorAssignerDestroyer(const char* file, int type)
{
    GLuint shader = sigCreateShader(type);
    GLchar *shadSource;
    FILE *File = fopen(file, "r");
    if(!File)
    {
        printf("Failed to open %s\n", file);
        return 0;
    }
    printf("Made it past\n");
    fseek(File, 0, SEEK_END);
    int fileLength = ftell(File);
    printf("FileLength: %i\n", fileLength);
    rewind(File);

    shadSource = (GLchar*)malloc((fileLength + 1) * sizeof(char));
    fread(shadSource, 1, fileLength, File);
    shadSource[fileLength] = '\0';

    printf("File\n%s\n", shadSource);

    fclose(File);
    sigShaderSource(shader, 1, &shadSource, 0);
    sigCompileShader(shader);
    
    //maybe errors in compilation are causing my issues
    int success;
    sigGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        printf("Compilation Error\n");
        char errorLog[400];
        int size;
        sigGetShaderInfoLog(shader, 400, &size, errorLog);
        printf("Error: %s\n", errorLog);
        sigDeleteShader(shader);
        free(shadSource);
        return 1;
    }


    sigAttachShader(program, shader);

    //I think linking is done collectively at the end, the above func doesnt "link" a program
    //together it just says to the program object this shader object is attached or whatever, leaving the code here though in case of future need
    // sigGetProgramiv(program, GL_LINK_STATUS, &success);
    // if(!success)
    // {
    //     printf("Linking Error\n");
    //     char errorLog[400];
    //     int size;
    //     sigGetProgramInfoLog(program, 400, &size, errorLog);
    //     printf("Error: %s\n", errorLog);
    //     sigDeleteShader(shader);
    //     free(shadSource);
    //     return 1;
    // }

    //its attached to program so its only flagged for deletion, once non-attached will be deleted
    sigDeleteShader(shader);
    free(shadSource);
    return 1;
}

/**
 * @brief Retrieves Visual, Returns NUll if the server does not exist or the config does not exist, 
 * 
 * If visual exist it sets curconfig
 *  
 * @param display 
 * @param config 
 * @return XVisualInfo* 
 */
XVisualInfo* retVisual(Display* display, uint32_t* config)
{
    uint32_t error_Base, event_Base;
    uint32_t bool = glXQueryExtension(display, &error_Base, &event_Base);

    if(!bool)
    {
        return NULL;
    }

    uint32_t count;
    GLXFBConfig* configs = glXChooseFBConfig(display, 0, config, &count);
    if(count == 0)
    {
        return NULL;
    }

    initConfig = configs[0];
    return glXGetVisualFromFBConfig(display, configs[0]);
}
/**
 * @brief Rets Zero if Fail, else returns handle to window association
 * 
 * @param display 
 * @param win 
 * @return uint64_t 
 */
uint64_t glInitWindow(Display* display, Window win)
{
    GLXWindow window = glXCreateWindow(display, initConfig, win, NULL);
    GLXContext cont = glXCreateNewContext(display, initConfig,  GLX_RGBA_TYPE, NULL, True);

    if(window == NULL || cont == NULL)
    {
        return 0;
    }

    if(!funcsBound)
    {
        retrieveFuncs();
        funcsBound = 1;
        //TODO: make memory allocation size choosen by the application
        wins = (contextWinAssociation*)malloc(sizeof(wins) * 512);
        winsSize = 512;

        wins->context = cont;
        wins->win = window;
        wins->name = 1;
        winCount++;
    }

    wins[winCount].context = cont;
    wins[winCount].win = window;
    wins[winCount].name = winCount++ + 1;

    return winCount;
}
