#include "GLInterface.h"

GLXFBConfig config;
GLXWindow mainWindow;
GLXContext context;
GLuint vertexBuffer;
GLuint program = 0;
GLuint vao = 0;

int shaderCreatorAssignerDestroyer(const char* file);

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
int retrieveConfig(Display* display)
{
    int count;
    int fifty[] = 
    {
        GLX_DOUBLEBUFFER, True,
        GLX_RED_SIZE, 8,
        GLX_GREEN_SIZE, 8,
        GLX_BLUE_SIZE, 8,
        GLX_ALPHA_SIZE, 8,
        GLX_BUFFER_SIZE, 32,
        GLX_DEPTH_SIZE, 16,
        GLX_RENDER_TYPE, GLX_RGBA_BIT,
        GLX_DRAWABLE_TYPE, GLX_WINDOW_BIT,
        GLX_X_RENDERABLE, True,
        GLX_X_VISUAL_TYPE, GLX_TRUE_COLOR,
        GLX_CONFIG_CAVEAT, GLX_NONE,
        GLX_ACCUM_RED_SIZE, 1,
        GLX_ACCUM_GREEN_SIZE, 1,
        GLX_ACCUM_BLUE_SIZE, 1,
        
        None    
    };
    GLXFBConfig* configs = glXChooseFBConfig(display, 0, fifty, &count);
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
/*
*   Returns True if Initilization Was a Success
*/
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
/*
* Create and Release Current Context Test
* 
*/
int currenting(Display* display)
{

    glXMakeCurrent(display, mainWindow, context);

    glBegin(GL_TRIANGLES);
    float data[3][3] = 
    {
        {0.0, 0.0, 0.0},
        {0.5, 1.0, 0.0},
        {1.0, 0.0, 0.0}
    };

    sigBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_DYNAMIC_DRAW);

    glEnd();

    glXSwapBuffers(display, mainWindow);
    // glFinish();


    // /*
    // * This is how the Specification on pg 41 recommends releasing the context
    // * Only applies if releasing it fully, that is there is no other context to draw in
    // */
    glXMakeCurrent(display, None, NULL);

    return 0;
}
int createAttachProgram(Display* display)
{

    glXMakeCurrent(display, mainWindow, context);

    program = sigCreateProgram();

    // create vertex shader and link
    shaderCreatorAssignerDestroyer("GlShaders/exampleVert.vert");

    //create frag shader and link
    shaderCreatorAssignerDestroyer("GlShaders/exampleFrag.frag");

    // //release shader compiler here
    sigReleaseShaderCompiler();

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

    float bdata[3][3] = 
    {
        {0.5, 1.0, 0.0},
        {1.0, 0.0, 0.0},
        {0.0, 0.0, 0.0}
    };

    sigBufferData(GL_ARRAY_BUFFER, sizeof(bdata), bdata, GL_DYNAMIC_DRAW);

    sigEnableVertexAttribArray(0);

    sigLinkProgram(program);

    //activae the sleeper agent
    sigUseProgram(program);

    glDrawArrays(GL_TRIANGLES, 0, 3);

    glXSwapBuffers(display, mainWindow);

    glXMakeCurrent(display, None, NULL);

    return 1;

}
int shaderCreatorAssignerDestroyer(const char* file)
{
    GLuint shader = sigCreateShader(GL_VERTEX_SHADER);
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

    printf("File\n%s", shadSource);

    fclose(File);
    sigShaderSource(shader, 1, &shadSource, 0);
    sigCompileShader(shader);

    sigAttachShader(program, shader);
    //its attached to program so its only flagged for deletion, once non-attached will be deleted
    // sigDeleteShader(shader);
    return 1;
}