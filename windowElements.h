//basic return struct... i want to like put make the return of these functions what we pass to glinterface functions
struct structer
{
    int vertexCount;
    int elements;
    float* vertexBuffer;
};

typedef struct structer vertexBI;

struct windowDiv
{
    int width;
    int height;
    int xPos;
    int yPos;
    struct windowDiv* children;
    //expand...
};
typedef struct windowDiv winDiv;
//will contain all relevant information to render the window with
//i think ill just let it expand overtime, this will like hold all the children 
struct theWindow
{
    int divArrSize;
    int divCount;
    struct windowDiv* divs;
    float* vBuffer;
};
extern struct theWindow mainWin;


int initializeWindow();
int adjustWinSize();
int addDiv();
vertexBI drawWindow();
