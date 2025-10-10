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
    //new actual loop
    glXMakeCurrent(win->dis, win->glWindow, win->glContext);
    if(win->render->reallocAddedSize > 0)
    {
        //this is a rather large operation it probably shouldn't be done often
            //needs to reallocate whole gpu buffer
            win->glBuffer.size = (uint32_t)((float)(win->glBuffer.size) * AdditionalGpuMem) > win->render->reallocAddedSize ? 
                ((uint32_t)(((float)win->glBuffer.size * AdditionalGpuMem) > MaxAdditionalGpuMem ? 
                    win->glBuffer.size + MaxAdditionalGpuMem : 
                    ((float)win->glBuffer.size * (1.0f + AdditionalGpuMem)))) : 
                win->glBuffer.size + win->render->reallocAddedSize * 2;
            
            win->glPointers.sigNamedBufferData(win->glBuffer.bufferName, (GLsizeiptr)win->glBuffer.size, NULL, GL_DYNAMIC_DRAW);
            
            deallocNamed(win->glBuffer.emptyMem, win->manager);
            win->glBuffer.emptyMem = swcAllocArray(200, xy, win->manager);
            xy fake = {.x = win->glBuffer.size, .y = 0};
            swcAddArray(win->glBuffer.emptyMem, fake, uint64_tSorter, win->manager);

            xy *real = swcRetrieveAtArray(win->glBuffer.emptyMem, sizeof(xy), 0, win->manager);

            swcArray *divLayers = retrieveArray(win->divLayers, win->manager);
            swcArray* divGroups;
            for(uint32_t i = 0; i < divLayers->curSize; i++)
            {
                divGroups = retrieveArray(((layerToDivGroups*)(divLayers->data))[i].divGroups, win->manager);
                for(uint32_t j = 0; j < divGroups->curSize; j++)
                {
                    if(((divGroupGpu*)divGroups->data)[j].gpuBufferDataSize != 0)
                    {
                        ((divGroupGpu*)divGroups->data)[j].gpuBufferDataLocation = real->y;//continuely pushing the first empty memory space outward
                        real->x = real->x - ((divGroupGpu*)divGroups->data)[j].gpuBufferDataSize;
                        real->y = real->y + ((divGroupGpu*)divGroups->data)[j].gpuBufferDataSize;

                        win->glPointers.sigNamedBufferSubData(win->glBuffer.bufferName, ((divGroupGpu*)divGroups->data)[j].gpuBufferDataLocation, 
                        ((divGroupGpu*)divGroups->data)[j].renderedDivsCount * ((divGroupGpu*)divGroups->data)[j].strideSize * ((divGroupGpu*)divGroups->data)[j].vertexCount, 
                        (void *)(retrieveArray(((divGroupGpu*)divGroups->data)[j].cpuSideBufferObjectData, win->manager)->data));
                    }
                }
            }
    }
    else
    {
        for(uint32_t i = 0; i < win->render->bufferDataChangedElementCount; i++)
        {
            //update gpu buffer
            win->glPointers.sigNamedBufferSubData(win->glBuffer.bufferName, win->render->bufferDataChanged[i].gpuOffset, 
            win->render->bufferDataChanged[i].dataSize, win->render->bufferDataChanged[i].cpuSideBufferObjectData);
        }
    }
    //render
    swcArray *layerArray = retrieveArray(win->divLayers, win->manager);
    layerToDivGroups *divLayers = (layerToDivGroups*)layerArray->data;
    swcArray *divGroupArray;
    divGroupGpu *divGroup;
    glViewport(0, 0, 1054, 1078);

    glClearColor(.5, .5, .5, 1);
    for(int32_t i = layerArray->curSize - 1; i >= 0; i--)
    {
        divGroupArray = retrieveArray(divLayers[i].divGroups, win->manager);
        divGroup = (divGroupGpu*)divGroupArray->data;
        for(uint32_t j = 0; j < divGroupArray->curSize; j++)
        {
            win->glPointers.sigBindVertexArray(divGroup[j].vaoName);
            win->glPointers.sigUseProgram(divGroup[j].programName);
            if(divGroup[j].renderType > GL_TRIANGLE_STRIP_ADJACENCY)
            {
                win->glPointers.sigPatchParameteri(GL_PATCH_VERTICES, divGroup[j].renderType - GL_TRIANGLE_STRIP_ADJACENCY);
                glDrawArrays(GL_PATCHES, divGroup[j].gpuBufferDataLocation / divGroup[j].strideSize, divGroup[j].renderedDivsCount * divGroup[j].vertexCount);
            }
            else{
                glDrawArrays(divGroup[j].renderType, divGroup[j].gpuBufferDataLocation / divGroup[j].strideSize, divGroup[j].renderedDivsCount * divGroup[j].vertexCount);
            }
            //TODO: rid the world of this func, and use a glpointer
            
        }
    }

    glXSwapBuffers(win->dis, win->glWindow);


























    //fake old loop
    //retrieving layers
    // swcArray* layers = retrieveArray(win->divLayers, win->manager);
    // layerToDivGroups* layerData;
    // layerData = (layerToDivGroups*)layers->data;

    // divGroupGpu program;
    // int32_t j, i;
    // programGPUStorageUpdated *newStorage;

    // newStorage = (programGPUStorageUpdated*)allocSB(sizeof(programGPUStorageUpdated) * (1 + win->render->bufferDataChangedElementCount), win->manager);  
    // newStorage->layer = 0;

    // glXMakeCurrent(win->dis, win->glWindow, win->glContext);
    // for(i = 0; i < win->render->bufferDataChangedElementCount; i++)
    // {
    //     if(win->render->bufferDataChanged[i].gpuBufferDataSize >= win->render->bufferDataChanged[i].cpuBufferDataSize)
    //     {
    //         //TODO: maybe find a way to not replace the whole data structure
    //         win->glPointers.sigNamedBufferSubData(win->render->bufferDataChanged[i].vaoName, 0, win->render->bufferDataChanged[i].gpuBufferDataSize, win->render->bufferDataChanged[i].cpuSideBufferObjectData);
            
    //     }
    //     else
    //     {
    //         win->glPointers.sigNamedBufferData(win->render->bufferDataChanged[i].vaoName, (uint32_t)((_Float32)(win->render->bufferDataChanged[i].cpuBufferDataSize) * AdditionalGpuMem > MaxAdditionalGpuMem ? win->render->bufferDataChanged[i].cpuBufferDataSize + MaxAdditionalGpuMem : (_Float32)win->render->bufferDataChanged[i].cpuBufferDataSize * (1 + AdditionalGpuMem)) , win->render->bufferDataChanged[i].cpuSideBufferObjectData, GL_DYNAMIC_DRAW);

    //         newStorage->layer++;
    //         newStorage[newStorage->layer].layer = win->render->bufferDataChanged[i].layer;
    //         newStorage[newStorage->layer].newGPUStorageSize = (uint32_t)((_Float32)(win->render->bufferDataChanged[i].cpuBufferDataSize) * AdditionalGpuMem > MaxAdditionalGpuMem ? win->render->bufferDataChanged[i].cpuBufferDataSize + MaxAdditionalGpuMem : (_Float32)win->render->bufferDataChanged[i].cpuBufferDataSize * (1 + AdditionalGpuMem));
    //         newStorage[newStorage->layer].program = win->render->bufferDataChanged[i].programName;
    //     }
    //     win->glPointers.sigUseProgram(win->render->bufferDataChanged[i].programName);
    //     win->glPointers.sigBindBuffer(GL_ARRAY_BUFFER ,win->render->bufferDataChanged[i].vaoName);
    //     win->glPointers.sigVertexAttribPointer(0, 3, GL_FLOAT, GL_TRUE, 0, 0);
    //     glDrawArrays(GL_TRIANGLES, 0, win->render->bufferDataChanged[i].cpuBufferDataSize);
    // }
    // if(win->render->bufferDataChangedElementCount > 0)
    // {
    //     glXSwapBuffers(win->dis, win->glWindow);

    //     for(i = 1; i <= newStorage->layer; i++)//using first struct in struct list to hold the current element and max size of array
    //     {
    //         layerData = (layerToDivGroups*)layers->data;
    //         j = layerData->layer;
    //         while(j != newStorage[i].layer)
    //         {
    //             j = ++layerData->layer;
    //         }
    //         program.programName = newStorage[i].program;
    //         j = swcContainsArray(layerData->divGroups, program, uint32_tSorter, win->manager);
    //         if(j == -1)
    //         {
    //             //should not occur something has crashed
    //             //TODO:
    //                 //add a error log...
    //         }
    //         //its definitly the below line, what is this even
            
    //         ((divGroupGpu*)retrieveName(layerData->divGroups, win->manager))[j].gpuBufferDataSize = newStorage[i].newGPUStorageSize;

    //     }
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
    win->render->reallocAddedSize = 0;
    
    swcArray *programArray = (swcArray*)retrieveArray(win->glProgramNames, win->manager);//grab the current program count
    swcArray *layerArray = (swcArray*)retrieveArray(win->divLayers, win->manager);//grab the layer count
    uint32_t i = programArray->curSize * layerArray->curSize;

    //TODO: improve this array, and how we handle it
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
 * @param layer DivLayer
 * @param div The Name of the Div
 * @param programName divs program
 * @param data the data that is equal in size to the divs stride size * divsvertexCount
 * @return 1 upon success | 0 Upon failure
 */
uint32_t updateRenderBuffer(uint32_t layer, swcName div, uint32_t programName, void* data, swcWin* win)
{

    layerToDivGroups *tempLayerToProgram = (layerToDivGroups *)allocSB(sizeof(layerToDivGroups), win->manager);
    tempLayerToProgram->layer = layer;
    tempLayerToProgram = (layerToDivGroups *)swcRetrieveArray(win->divLayers, *tempLayerToProgram, uint32_tSorter, win->manager);

    divGroupGpu *divGroup = (divGroupGpu*)allocSB(sizeof(divGroupGpu), win->manager);
    divGroup->programName = programName;
    int32_t index = swcContainsArray(tempLayerToProgram->divGroups, *divGroup, uint32_tSorter, win->manager);
    if(index < 0)
        return 0;
    

    uint32_t i = 0;
    for(i; i < win->render->bufferDataChangedElementCount; i++)
    {
        if(win->render->bufferDataChanged[i].programName == divGroup->programName && win->render->bufferDataChanged[i].layer == layer)
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

    divGroup = swcRetrieveAtArray(tempLayerToProgram->divGroups, divGroupGpu, index, win->manager);
    
    //below the index for the divs array within the divGroup struct is used to index the cpuSideBufferObjectData, essentially operating as a dictionary

    flagged_uint32_t divFlag = {.flag = 0, .x = div};
    index = swcContainsArray(divGroup->divs, divFlag, flagged_uint32_tSorter, win->manager);
    if(index < 0)
    {
        swcNameStruct *divsArray = retrieveNameL(divGroup->divs, win->manager);
        divFlag.flag = 1;
        index = swcContainsArray(divGroup->divs, divFlag, flagged_uint32_tSorter, win->manager);
        if(index < 0 )
            return 0;
        removeAtArray(divsArray, sizeof(flagged_uint32_t), index, win->manager);
        addAtArray(divsArray, sizeof(flagged_uint32_t), (void *)(&divFlag), index, win->manager);
        addAtArray(retrieveNameL(divGroup->cpuSideBufferObjectData, win->manager), divGroup->strideSize * divGroup->vertexCount, data, index, win->manager);
    }
    else
    {

        replaceAtArray(divGroup->cpuSideBufferObjectData, divGroup->strideSize * divGroup->vertexCount, data, index, win->manager);
        divGroup->renderedDivsCount += 1;
    }
    
    

    win->render->bufferDataChanged[i].programName = divGroup->programName;
    win->render->bufferDataChanged[i].layer = layer;
    win->render->bufferDataChanged[i].gpuOffset = divGroup->gpuBufferDataLocation;
    win->render->bufferDataChanged[i].dataSize = divGroup->strideSize * divGroup->renderedDivsCount * divGroup->vertexCount;
    win->render->bufferDataChanged[i].cpuSideBufferObjectData = (void *)(retrieveArray(divGroup->cpuSideBufferObjectData, win->manager)->data);

    return 1;
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
 * @brief Allocates memory of size Count * Size in Gpu
 * 
 * @param win 
 * @param stride Bytes Size To Allocate
 * @param count Amount of entries to Allocate
 * @return Memory Location or 0 if Memory Will Be Mass Reallocated at Start of Render Loop | Very Trusting Drawing System Requires Correct Calls No Checks
 */
uint32_t gpuAlloc(uint32_t count, uint32_t stride, swcWin *win)
{
    //TODO:
        //the uuuhh the uuhh the padding because of the vaoi
    xy fake = {.x = count * stride, .y = 0};
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
        //perfect memory location found... check for alignment
        real =  swcRetrieveAtArray(win->glBuffer.emptyMem, xy, index, win->manager);
        if(real->y % stride == 0)
        {   
            //perfect memory alignment too
            fake.y = real->y;
            swcRemoveAtArray(win->glBuffer.emptyMem, xy, index, win->manager);
            return fake.y;
        }
    }
    else
    {

        index = -index - 2;
    }
    uint32_t location = 0;
    //location not found, check index returned... because the index returned sorts on a index -1 and index
    //loop that means that -index -2 returned should be either between that which is less than it and index -1 and that which is greater at the index
    //position. This is two possibilities, either option 1 the index positiion contians enough space for the new allocation, or option 2 there is no index position
    //as it reached the end of the array. For option 1 the array retrieves the pointer to that location, it than deletes that location. Than it saves the 
    //position in memory that location describes, than it calculates the empty space remaining from that location and reinserts that into the array, than returning
    //the saved position. For option 2 it means there is not enough space left in gpu buffer, meaning it needs to be reallocated.


    if(win->render->reallocAddedSize > 0)
    {
        //will be realloced into existance at start of render buffer anyway
        return 0;
    }

    swcNameStruct *emptyMem = retrieveNameL(win->glBuffer.emptyMem, win->manager);
    uint32_t adjust;
    while(((swcArray*)emptyMem->pointer)->curSize <= index)
    {
        //check and loop untill there is no space  or the location and space allows for alignment
        adjust = 0;
        real = retrieveAtArray(emptyMem, sizeof(xy), index, win->manager);
        if(real->y % stride == 0)
        {
            location = real->y;
            break;
        }
        else
        {
            adjust = real->y % stride;
            if(real->x - adjust >= fake.x)
            {
                location = real->y + adjust;
                break;
            }
            index++;
            //not a very pretty solution...
        }
    }
    if(location == 0)
    {  
        //location cannot be found, add to size
        win->render->reallocAddedSize += count * stride;

        return 0;
    }
    fake.x = real->x;
    fake.y = real->y;
    removeAtArray(emptyMem, sizeof(xy), index, win->manager);

    if(adjust)
    {
        xy fake2 = {.x = adjust, .y = fake.y};
        swcAddArray(win->glBuffer.emptyMem, fake2, uint64_tSorter, win->manager);
        fake.x -= (count * stride) + adjust;
        fake.y += (count * stride) + adjust;
    }
    else
    {
        fake.x -= (count * stride);
        fake.y += (count * stride);
    }
    if(fake.x > 0)
        swcAddArray(win->glBuffer.emptyMem, fake, uint64_tSorter, win->manager);

    return location;

}
/**
 * @brief Reallocates Gpu Memory, same ystem as gpuAlloc
 * 
 * @param win window
 * @param divGroupGpu I might change this to div, but I currently am operating on the assumption that it will be the divgroups that call for allocating and reallocating memory
 * @param newSize byte size of memory to allocate
 * @return Memory Location
 */
uint32_t gpuRealloc(divGroupGpu* divGroupGpu, uint32_t count, swcWin* win)
{
    gpuFree(divGroupGpu, win);

    divGroupGpu->gpuBufferDataSize = 0;

    return gpuAlloc(count, divGroupGpu->strideSize * divGroupGpu->vertexCount, win);
}
/**
 * @brief Frees memory, this does not delete current memory within the gpu, it just allocates that space to the empty space array
 * 
 * @param win 
 * @param divGroupGpu As with realloc might change this to div, but probably not
 * @return Returns 1 on success, there is no way for this to fail, unless the arrray does not exist, so just returning 1
 */
uint32_t gpuFree(divGroupGpu* divGroupGpu, swcWin* win)
{
    //checks for empty space ahead and behind of the space we are emptying
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