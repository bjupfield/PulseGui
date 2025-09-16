#include "swcRender.h"

typedef struct
{
    uint32_t layer;
    uint32_t program;
    uint32_t newGPUStorageSize;
}programGPUStorageUpdated;
/**
 * @brief 
 * 
 * @param win 
 * @return uint32_t 
 */
uint32_t renderMain(swcWin* win)
{   
    //retrieving layers
    swcArray* layers = retrieveArray(win->divLayers, win->manager);
    layerToProgram* layerData;
    layerData = (layerToProgram*)layers->data;

    nameToDiv program;
    int32_t j, i;
    programGPUStorageUpdated *newStorage;
    uint32_t fake = sizeof(typeof(newStorage)) * (1 + win->render->bufferDataChangedElementCount);

    newStorage = (programGPUStorageUpdated*)allocSB(sizeof(typeof(newStorage)) * (1 + win->render->bufferDataChangedElementCount), win->manager);  


    glXMakeCurrent(win->dis, win->glWindow, win->glContext);
    for(i = 0; i <= win->render->bufferDataChangedElementCount; i++)//using first struct in struct list to hold the current element and max size of array
    {
        if(win->render->bufferDataChanged[i].gpuBufferDataSize >= win->render->bufferDataChanged[i].cpuBufferDataSize)
        {
            //TODO: maybe find a way to not replace the whole data structure
            win->glPointers.sigNamedBufferSubData(win->render->bufferDataChanged[i].vertexBufferObjectName, 0, win->render->bufferDataChanged[i].gpuBufferDataSize, win->render->bufferDataChanged[i].cpuSideBufferObjectData);
            
        }
        else
        {
            win->glPointers.sigNamedBufferData(win->render->bufferDataChanged[i].vertexBufferObjectName, (uint32_t)((_Float32)(win->render->bufferDataChanged[i].cpuBufferDataSize) * AdditionalGpuMem > MaxAdditionalGpuMem ? win->render->bufferDataChanged[i].cpuBufferDataSize + MaxAdditionalGpuMem : (_Float32)win->render->bufferDataChanged[i].cpuBufferDataSize * (1 + AdditionalGpuMem)) , win->render->bufferDataChanged[i].cpuSideBufferObjectData, GL_DYNAMIC_DRAW);

            newStorage->layer++;
            newStorage[newStorage->layer].layer = win->render->bufferDataChanged[i].layer;
            newStorage[newStorage->layer].newGPUStorageSize = (uint32_t)((_Float32)(win->render->bufferDataChanged[i].cpuBufferDataSize) * AdditionalGpuMem > MaxAdditionalGpuMem ? win->render->bufferDataChanged[i].cpuBufferDataSize + MaxAdditionalGpuMem : (_Float32)win->render->bufferDataChanged[i].cpuBufferDataSize * (1 + AdditionalGpuMem));
            newStorage[newStorage->layer].program = win->render->bufferDataChanged[i].programName;
        }
        win->glPointers.sigUseProgram(win->render->bufferDataChanged[i].programName);
        win->glPointers.sigBindBuffer(GL_ARRAY_BUFFER ,win->render->bufferDataChanged[i].vertexBufferObjectName);
        glDrawArrays(GL_TRIANGLES, 0, win->render->bufferDataChanged->cpuBufferDataSize);
    }
    glXSwapBuffers(win->dis, win->glWindow);
    for(i = 1; i <= newStorage->layer; i++)//same trick
    {
        layerData = (layerToProgram*)layers->data;
        j = layerData->layer;
        while(j != newStorage[i].layer)
        {
            j = ++layerData->layer;
        }
        program.programName = newStorage[i].program;
        j = swcContainsArray(layerData->programGroups, program, nameToDivSorter, win->manager);
        if(j == -1)
        {
            //should not occur something has crashed
            //TODO:
                //add a error log...
        }
        ((nameToDiv*)(layerData->programGroups, win->manager))[j].gpuBufferDataSize = newStorage[i].newGPUStorageSize;

    }
    /*
    *   A comment on the above code... This should work, as you might notice we are using a literal pointer in this case to reference the cpu side changes...
    *   as long as there is no mass reallocation of memory until after each render loop than this should not cause an issue, as every single time a cpu side buffer is changed it should
    *   be able to store the correct pointer, even though it might look unsafe, it should be safe... again as long as any mass reallocation of memory is only done after each render loop, or 
    *   a mass reallocation will restart the render loop
    *  
    */
   /*
   *    I AM SCRAPPING THE BELOW LOOPS, why
   *    because as long as we use zbuffering we shouldn't need them... there are no camera changes so the position of these divs shouldn't actually change unless the above loop is changed
   *    because of zbuffering the divs below other divs shouldn't override the frame static divs, so because of this these static divs do not need to be drawn, which means we don't even need the loop below but can just draw
   *    everything based on the bufferdatachanged array vastly simplifieng the render pipeline, ill keep it commented for now just in case
   */
    // for(uint32_t i = 0; i < layers->curSize; i++)
    // {//iterate over layers
    //     layer = retrieveArray(layerData[i].programGroups);
    //     programGroups = (nameToDiv*)layer->data;
    //     for(uint32_t j = 0; j < layer->curSize; j++)
    //     {
    //         //load program
    //         win->glPointers.sigUseProgram(programGroups[j].programName);
    //         win->glPointers.sigBindBuffer(GL_ARRAY_BUFFER, programGroups[j].vertexBufferObjectName);
    //         glDrawArrays(GL_TRIANGLES, ((fakeObject*)retrieveName(programGroups[j].cpuSideBufferObjectData))->vectorcount)//figure out size issues

    //         /*
    //         *   TODO:
    //         *       Above I had to make a decision on how to render the divs, and the decision is over in what way would be faster to render the divs, use the name structure for reference to the draw arrays and 
    //         *       use this to render the divs, or make something more like what is done abover in the first for loop to change the data held in the vertex buffers. the problem with the second option is that there
    //         *       is no way to assure that all draw groups would be added to something like a win->render.drawArray without looping through every single layer and draw group like is done here. so the question is than:
    //         *           is it more effecient to loop through these within the glDrawArrays rendering cycle, or would it be better to conduct this loop after/before the cycle?
    //         *       obviously the second option would lead to a less efficient cpu side algo... but it might be more efficient for OpenGL to recieve all the draw commands immediately. Anyways, I have decided for now
    //         *       that I will just use the more efficient cpu sided algo, hopefully its better
    //         * 
    //          */

    //     }
       
                
    //     //now render the div

    //     /*
    //     *
    //     *   Okay this is the basic block of how to render an individual vertex buffer, or what need to be done to create one and load it into the program
    //     * 
    //     *   
    //         sigCreateVertexArrays(1, &vao);
    //         sigBindVertexArray(vao);

    //         // //bind attributes link program, and use
    //         // //i really need to make tyedefs for these functions so their readable
    //         sigVertexAttribPointer(0, 3, GL_FLOAT, GL_TRUE, 0, 0);

    //         float bdata[6][3] = 
    //         {
    //             {0.0, 1.0, 0.0},
    //             {-1.0, -1.0, 0.0},
    //             {1.0, -1.0, 0.0},
    //             {0.5, 1.0, 0.0},
    //             {1.0, -1.0, 0.0},
    //             {1.0, 1.0, 0.0},
    //         };

    //         sigBufferData(GL_ARRAY_BUFFER, sizeof(bdata), bdata, GL_DYNAMIC_DRAW);

    //         sigEnableVertexAttribArray(0);
    //     * 
    //     *
    //     *   Obviosuly we do not need to create a new vertexobject each time a div is drawn, however I do not know exactly what sigEnableVertexAttribArray is... it might not be necessary, nor
    //     *   do i really know what sigVertexAttribPointer is
    //     *   okay i see, vertexattribpointer describes the type of data that will be contianed in the bound array, as you can see the first number is index, 0 i guess don't really understand what that means,
    //     *   the second 3 is the amount of data points per vertex, 3 as in our case each point has 3 dimensions, 3rd the type of date GL_Floats, 4 the normalization, which should always b GL_TRUE, and the next two should always be 0
    //     * 
    //     *   i will get to this later
    //     * 
    //     *   also need to use glbindbuffer...
    //     */
    // }

    glXMakeCurrent(win->dis, None, NULL);
    return 0;
}
/**
 * @brief Set up for render loop, done at start of window loop or something, like it sets up the renderbuffer right now
 * 
 * @param win 
 * @return Returns 1 upon Success | 0 on Fail 
 */
uint32_t preRender(swcWin* win)
{   win->render = (struct render*)allocSB(sizeof(struct render), win->manager);
    if(win->render == NULL)
    {
        return 0;
    }
    win->render->bufferDataChangedElementCount = 0;
    swcArray *programArray = (swcArray*)retrieveArray(win->glProgramNames, win->manager);//grab the current program count
    swcArray *layerArray = (swcArray*)retrieveArray(win->divLayers, win->manager);//grab the layer count
    uint32_t i = programArray->curSize * layerArray->curSize;

    win->render->bufferDataChangedSize = i >= MinBufferDataChangedSize ? i : MinBufferDataChangedSize;//allow for more programs to be updated than currently exist, this should be excessive
    win->render->bufferDataChanged = allocSB(sizeof(bufferDataChanged) * win->render->bufferDataChangedSize, win->manager);

    if(win->render->bufferDataChanged == NULL)
    {
        return 0;
    }
    return 1;
}
/**
 * @brief Use to update the render buffer after any div changes their graphics state
 * 
 * @param win 
 * @param vertexBufferObjectName 
 * @param gpuBufferDataSize 
 * @param cpuBufferDataSize 
 * @param programName 
 * @param cpuSideBufferObjectData 
 * @return 1 upon success | 0 Upon failure
 */
uint32_t updateRenderBuffer(swcWin* win ,uint32_t vertexBufferObjectName, uint32_t gpuBufferDataSize, uint32_t cpuBufferDataSize, uint32_t programName, uint32_t layer, void* cpuSideBufferObjectData)
{

    //this doesnt work either... need to search buffer to find 
    uint32_t i = 0;
    for(i; i < win->render->bufferDataChangedElementCount; i++)
    {
        if(win->render->bufferDataChanged[i].programName == programName && win->render->bufferDataChanged[i].layer == layer)
        {
            break;
        }
    }
    
    if(i == win->render->bufferDataChangedElementCount)
    {
        win->render->bufferDataChangedElementCount++;
        if(i + 1 > win->render->bufferDataChangedSize)
        {
            //TODO: add more space
            return 0;
        }
    }
    win->render->bufferDataChanged[i].vertexBufferObjectName = vertexBufferObjectName;
    win->render->bufferDataChanged[i].gpuBufferDataSize = gpuBufferDataSize;
    win->render->bufferDataChanged[i].cpuBufferDataSize = cpuBufferDataSize;
    win->render->bufferDataChanged[i].programName = programName;
    win->render->bufferDataChanged[i].cpuSideBufferObjectData = cpuSideBufferObjectData;
    win->render->bufferDataChanged[i].layer = layer;

    return 0;
}