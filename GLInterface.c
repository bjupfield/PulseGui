#include "GLInterface.h"

GLXFBConfig config;
GLXWindow mainWindow;
GLXContext context;
GLuint vertexBuffer;
GLuint program = 0;
GLuint vao = 0;


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
    printf("hitwentfour");

    mainWindow = glXCreateWindow(display, config, window, NULL);
    context = glXCreateNewContext(display, config,  GLX_RGBA_TYPE, NULL, True);

    glXMakeCurrent(display, mainWindow, context);
    printf("hitwentfour");

    retrieveFuncs();


    //we shouldnt do it like this... this is bad, dont do this... lol... going to keep like this for now
    //SINCreateBuffers(1, &vertexBuffer);
    //SINBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    printf("hitwentfour");

    for(int i = 0; i < 400000000; i++)
    {

    }

    myCreateBuffers((GLsizei)1, &vertexBuffer);
    printf("hitwentfour");
    // myBindBuffers(GL_ARRAY_BUFFER, vertexBuffer);


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
    SINUseProgram(0);
    SINDeleteProgram(program);
    glXDestroyContext(display, context);
    SINDeleteBuffers(1, vertexBuffer);
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

    SINBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_DYNAMIC_DRAW);

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
int createAttachProgram()
{
    program = SINCreateProgram();

    //create vertex shader and link
    GLuint vertShad = SINCreateShader(GL_VERTEX_SHADER);
    char *vertShadSource;
    fileReader(vertShadSource, "exampleVert.vert");
    SINShaderSource(vertShad, 1, vertShadSource, 0);
    SINCompileShader(vertShad);

    SINAttachShader(program, vertShad);
    //its attached to program so its only flagged for deletion, once non-attached will be deleted
    SINDeleteShader(vertShad);

    //create frag shader and link
    GLuint fragShad = SINCreateShader(GL_FRAGMENT_SHADER);
    char *fragShadSource;
    fileReader(fragShadSource, "exampleFrag.frag");
    SINShaderSource(fragShad, 1, fragShadSource, 0);
    SINCompileShader(fragShad);

    SINAttachShader(program, fragShad);

    SINDeleteShader(fragShad);
    
    /*
    * create vao (vertex array object), which stores like the linking information for buffers and locations/indexes
    * into a single object, so you can say this buffer will be attached to this index/location of a shader
    * and store it inside this object
    */

    SINCreateVertexArrays(1, &vao);
    SINBindVertexArray(vao);

    //bind attributes link program, and use
    //i really need to make tyedefs for these functions so their readable
    SINVertexAttribPointer(0, 3, GL_BYTE, GL_TRUE, 0, 0);

    float data[3][3] = 
    {
        {0.0, 0.0, 0.0},
        {0.5, 1.0, 0.0},
        {1.0, 0.0, 0.0}
    };

    SINBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_DYNAMIC_DRAW);

    SINLinkProgram(program);

    //activae the sleeper agent
    SINUseProgram(program);

    return 1;

}
//returns 0 if fails, string = store file in, file = file name
int fileReader(char *string, const char* file)
{
    FILE *File = fopen(file, "r");
    if(!File)
    {
        return 0;
    }
    fseek(File, 0, SEEK_END);
    int fileLength = ftell(File);
    rewind(File);

    char *str = (char *)malloc((fileLength + 1) * sizeof(char));
    fread(str, 1, fileLength, File);
    string[fileLength] = '\0';

    fclose(File);
    return 1;
}