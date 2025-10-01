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
    layerToDivGroups* layerData;
    layerData = (layerToDivGroups*)layers->data;

    divGroupGpu program;
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
            layerData = (layerToDivGroups*)layers->data;
            j = layerData->layer;
            while(j != newStorage[i].layer)
            {
                j = ++layerData->layer;
            }
            program.programName = newStorage[i].program;
            j = swcContainsArray(layerData->divGroups, program, uint32_tSorter, win->manager);
            if(j == -1)
            {
                //should not occur something has crashed
                //TODO:
                    //add a error log...
            }
            //its definitly the below line, what is this even
            
            ((divGroupGpu*)retrieveName(layerData->divGroups, win->manager))[j].gpuBufferDataSize = newStorage[i].newGPUStorageSize;

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
/**
 * @brief Initializes gpu memory to InitialGpuMem
 * 
 * @param win 
 * @return 1 if Success | 0 if Failure
 */
uint32_t initGPUMemory(swcWin *win)
{

    GLuint buffer = glInitBuffer(win, InitialGpuMem);
    if(buffer == 0)
    {
        return 0;
    }
    win->glBuffer.bufferName = buffer;
    win->glBuffer.size = InitialGpuMem;

    win->glBuffer.emptyMem = swcAllocArray(200, xy, win->manager);
    if(win->glBuffer.emptyMem == 0)
    {
        return 0;
    }
    xy temp = {.x = InitialGpuMem, .y = 0};

    swcAddArray(win->glBuffer.emptyMem,  temp, uint64_tSorter, win->manager);

    return 1;
}
/**
 * @brief Okay so to be clear this is an alloc function, it "allocated memory in the gpu"
 * what this really means is that it checks to see if the gpu buffer has memory left, if it does
 * it sets aside memory for the program group, if it doesnt it reallocs the buffer
 * and reorders the memory within the buffer to take up as least space as possible (asssuming theres
 * empty space due to deallocated memory)
 * 
 * @param win 
 * @param size Bytes Size To Allocate
 * @return Memory Location | Very Trusting Drawing System Requires Correct Calls No Checks
 */
uint32_t gpuAlloc(swcWin *win, uint32_t size)
{
    xy fake = {.x = size, .y = 0};
    int32_t index = swcContainsArray(win->glBuffer.emptyMem, fake, uint64_tSorter, win->manager);
    xy *real;
    if(index == -1)
    {
        //failure
        //TODO: fix
        return 0;
    }
    else if (index >= 0)
    {
        //perfect memory location found... use
        real =  swcRetrieveAtArray(win->glBuffer.emptyMem, xy, index, win->manager);
        fake.y = real->y;
        swcRemoveAtArray(win->glBuffer.emptyMem, xy, index, win->manager);
        return fake.y;
    }
    else
    {
        index = -index - 2;
        uint32_t location;
        //location not found, check index returned... because the index returned sorts on a index -1 and index
        //loop that means that -index -2 returned should be either between that which is less than it and index -1 and that which is greater at the index
        //position. This is two possibilities, either option 1 the index positiion contians enough space for the new allocation, or option 2 there is no index position
        //as it reached the end of the array. For option 1 the array retrieves the pointer to that location, it than deletes that location. Than it saves the 
        //position in memory that location describes, than it calculates the empty space remaining from that location and reinserts that into the array, than returning
        //the saved position. For option 2 it means there is not enough space left in gpu buffer, meaning it needs to be reallocated.
        swcNameStruct *emptyMem = retrieveNameL(win->glBuffer.emptyMem, win->manager);
        if(((swcArray*)emptyMem->pointer)->curSize <= index)
        {
            //this is a rather large operation it probably shouldn't be done often
            //needs to reallocate whole gpu buffer
            win->glBuffer.size = (uint32_t)((float)(win->glBuffer.size) * AdditionalGpuMem) > size ? 
                ((uint32_t)(((float)win->glBuffer.size * AdditionalGpuMem) > MaxAdditionalGpuMem ? 
                    win->glBuffer.size + MaxAdditionalGpuMem : 
                    ((float)win->glBuffer.size * (1.0f + AdditionalGpuMem)))) : 
                win->glBuffer.size + size * 2;
            
            win->glPointers.sigNamedBufferData(win->glBuffer.bufferName, (GLsizeiptr)win->glBuffer.size, NULL, GL_DYNAMIC_DRAW);
            
            deallocNamed(win->glBuffer.emptyMem, win->manager);
            win->glBuffer.emptyMem = swcAllocArray(200, xy, win->manager);
            xy fake = {.x = size, .y = 0};
            swcAddArray(win->glBuffer.emptyMem, fake, uint64_tSorter, win->manager);

            emptyMem = retrieveNameL(win->glBuffer.emptyMem, win->manager);
            real = retrieveAtArray(emptyMem, sizeof(xy), 0, win->manager);

            swcArray *divLayers = retrieveArray(win->divLayers, win->manager);
            swcArray* divGroups;
            for(int i = 0; i < divLayers->curSize; i++)
            {
                divGroups = retrieveArray(((layerToDivGroups*)(divLayers->data))[i].divGroups, win->manager);
                for(int i = 0; i < divGroups->curSize; i++)
                {
                    if(((divGroupGpu*)divGroups->data)[i].gpuBufferDataSize != 0)
                    {
                        ((divGroupGpu*)divGroups->data)[i].gpuBufferDataLocation = real->y;//continuely pushing the first empty memory space outward
                        real->x = real->x - ((divGroupGpu*)divGroups->data)[i].gpuBufferDataSize;
                        real->y = real->y + ((divGroupGpu*)divGroups->data)[i].gpuBufferDataSize;

                        win->glPointers.sigNamedBufferSubData(win->glBuffer.bufferName, ((divGroupGpu*)divGroups->data)[i].gpuBufferDataLocation, 
                        ((divGroupGpu*)divGroups->data)[i].cpuBufferObjectDataElementSize, retrieveName(((divGroupGpu*)divGroups->data)[i].cpuSideBufferObjectData, win->manager));
                    }
                }
            }

            //adjust empty memory with newly allocated block and return location
            location = real->y;
            real->x = real->x - size;
            real->y = real->y + size;

            return location;

        }
        else
        {
            real = retrieveAtArray(emptyMem, sizeof(xy), index, win->manager);
            location = real->y;

            fake.x = real->x - size;
            fake.y = real->y + size;

            removeAtArray(emptyMem, sizeof(xy), index, win->manager);
            swcAddArray(win->glBuffer.emptyMem, fake, uint64_tSorter, win->manager);

            return location;
        }
    }

}
/**
 * @brief Reallocates Gpu Memory, same ystem as gpuAlloc
 * 
 * @param win window
 * @param divGroupGpu I might change this to div, but I currently am operating on the assumption that it will be the divgroups that call for allocating and reallocating memory
 * @param newSize byte size of memory to allocate
 * @return Memory Location
 */
uint32_t gpuRealloc(swcWin* win, divGroupGpu* divGroupGpu, uint32_t newSize)
{
    gpuFree(win, divGroupGpu);

    divGroupGpu->gpuBufferDataSize = 0;

    return gpuAlloc(win, newSize);
}
/**
 * @brief Frees memory, this does not delete current memory within the gpu, it just allocates that space to the empty space array
 * 
 * @param win 
 * @param divGroupGpu As with realloc might change this to div, but probably not
 * @return Returns 1 on success, there is no way for this to fail, unless the arrray does not exist, so just returning 1
 */
uint32_t gpuFree(swcWin* win, divGroupGpu* divGroupGpu)
{
    //checks for empty space ahead of the space we are emptying
    swcNameStruct *emptyMemL = retrieveNameL(win->glBuffer.emptyMem, win->manager);
    swcArray* emptyMem = (swcArray*)emptyMemL->pointer;
    xy* data = (xy*)emptyMem->data;
    uint32_t after = 0, before = 0;
    for(uint32_t i = 0; i < emptyMem->curSize; i++)
    {
        if(data[i].y == divGroupGpu->gpuBufferDataLocation + divGroupGpu->gpuBufferDataSize)
        {
            //empty space comes immediately after this newly made empty space
            after = i;
        }
        if(data[i].y + data[i].x == divGroupGpu->gpuBufferDataLocation)
        {
            before = i;
        }
        if(after && before)
            break;
    }

    xy fake;
    if(after && before)
    {
        xy fake2 = data[after];
        fake.x = data[before].x + divGroupGpu->gpuBufferDataSize + fake2.x;
        fake.y = data[before].y;
        removeAtArray(emptyMemL, sizeof(xy), before, win->manager);

        swcRemoveArray(win->glBuffer.emptyMem, fake2, uint64_tSorter, win->manager);

        swcAddArray(win->glBuffer.emptyMem, fake, uint64_tSorter, win->manager);
    }
    else if(before)
    {
        fake.x = data[before].x + divGroupGpu->gpuBufferDataSize;
        fake.y = data[before].y;
        removeAtArray(emptyMemL, sizeof(xy), before, win->manager);

        swcAddArray(win->glBuffer.emptyMem, fake, uint64_tSorter, win->manager);
    }
    else if(after)
    {
        fake.x = data[after].x + divGroupGpu->gpuBufferDataSize;
        fake.y = divGroupGpu->gpuBufferDataLocation;
        removeAtArray(emptyMemL, sizeof(xy), after, win->manager);

        swcAddArray(win->glBuffer.emptyMem, fake, uint64_tSorter, win->manager);
    }

    return 1;
}
/**
 * @brief Actually inserts the current divGroupGpu cpuSideBufferObjectData into the gpu, based of the assigned locatiopn and size
 * 
 * @param win 
 * @param divGroupGpu 
 * @return uint32_t 
 */
uint32_t insertGpuMem(swcWin* win, divGroupGpu* divGroupGpu)
{
    void* contianer = retrieveName(divGroupGpu->cpuSideBufferObjectData, win->manager);
    if(contianer == NULL)
        return 0;
    win->glPointers.sigNamedBufferSubData(win->glBuffer.bufferName, divGroupGpu->gpuBufferDataLocation, divGroupGpu->gpuBufferDataSize, contianer);
    return 1;
}