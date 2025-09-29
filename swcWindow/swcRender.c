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

    newStorage = (programGPUStorageUpdated*)allocSB(sizeof(programGPUStorageUpdated) * (1 + win->render->bufferDataChangedElementCount), win->manager);  
    newStorage->layer = 0;

    glXMakeCurrent(win->dis, win->glWindow, win->glContext);
    for(i = 0; i < win->render->bufferDataChangedElementCount; i++)
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
        win->glPointers.sigVertexAttribPointer(0, 3, GL_FLOAT, GL_TRUE, 0, 0);
        glDrawArrays(GL_TRIANGLES, 0, win->render->bufferDataChanged[i].cpuBufferDataSize);
    }
    if(win->render->bufferDataChangedElementCount > 0)
    {
        glXSwapBuffers(win->dis, win->glWindow);

        for(i = 1; i <= newStorage->layer; i++)//using first struct in struct list to hold the current element and max size of array
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
            //its definitly the below line, what is this even
            
            ((nameToDiv*)retrieveName(layerData->programGroups, win->manager))[j].gpuBufferDataSize = newStorage[i].newGPUStorageSize;

        }
    }

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