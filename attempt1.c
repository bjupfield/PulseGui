#include "mainloop.h"
#include "introspect.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

pa_client_info* clientList;
uint16_t clientCount;
uint32_t selectedClient;
uint32_t sinkFound = 1;
uint32_t originalSink;
uint32_t stateMachine;
uint32_t inputSink;

void a(pa_context *context, const pa_client_info *client, int eol, void *userdata)
{
    if(eol > 0)
    {
        //pa_mainloop_quit((pa_mainloop *)userdata, 1);
        //this is actually what i "wrote", just cancelling callback once data is recieved
        return;
    }

    printf("Client Name: %s\nClient Index:%i\n|||||||||||||||||||\n", client->name, client->index);
    memcpy(&clientList[clientCount], client, sizeof(pa_client_info));
    clientList[clientCount].name = strdup(client->name);
    clientCount++;
    return;
}
void b (pa_context *c, const pa_sink_input_info *sink, int eol, void *userdata)
{
    if(eol > 0)
    {
        return;
    }
    printf("Selected: %i, Sink: %i\n",selectedClient, sink->client);
    if(sink->client == selectedClient)
    {
        printf("Sink Index: %i\n",sink->sink);
        //inputSink = sink->sink;
        sinkFound = 0;
    }
    return;
}
//this is just a pure copy paste from chatgpt..., seems to work
//really only need to handle PA_CONTEXT_READY and PA_CONTEXT_TERMINATED
void context_state_callback(pa_context *context, void *userdata) {
   switch (pa_context_get_state(context)) {
       case PA_CONTEXT_READY:
           // Context is ready, request sink info list
           switch(stateMachine)
           {
            case 2:
                pa_context_get_client_info_list(context, a, userdata);
                break;
            case 1: 
                pa_context_get_sink_input_info_list(context, b, userdata);
                break;
            default: break;
           }
           break;
       default:
           break;
   }
}

void setup(pa_mainloop** loop)
{
    if(*loop != NULL)
    {
         pa_mainloop_free(*loop);
    }
    *loop = pa_mainloop_new();
    pa_context *context = pa_context_new(pa_mainloop_get_api(*loop), "Fake");
    pa_context_connect(context, NULL, PA_CONTEXT_NOAUTOSPAWN, NULL);
    pa_context_set_state_callback(context, context_state_callback, *loop);
    return;
}
void mainloopRUN()
{
}
int main(void*)
{
    clientList = (pa_client_info*)malloc(sizeof(pa_client_info) * 32);
    clientCount = 0;
    stateMachine = 2;
    
    int32_t contextAlive = 0;

    char* badScan = (char*)malloc(8 * 32);

    pa_mainloop *loop = NULL;
    setup(&loop);
    //will neeed below later
    //pa_context_load_module(context, "Fake-Module", "module-null-sink", NULL, loop);
    
    printf("|||||||||||||||||||\n");
    while(clientCount == 0)
        contextAlive = pa_mainloop_iterate(loop, 1, 0);
    //this func captures the program... don't know how to make it not do this

    printf("Client count: %i\nChoose Client: ", clientCount);
    scanf("%32s",badScan);

    for(int i = 0; i < clientCount; i++)
    {
        printf("ClientName: %s\n", clientList[i].name);
        if(strcmp(badScan, clientList[i].name) == 0)
        {
            printf("ClientName: %s", clientList[i].name);
            selectedClient = clientList[i].index;
        }

    }

    stateMachine = 1;
    //pa_context_set_state_callback(context, context_state_callback2, loop);

    clientCount = 0;
    setup(&loop);
    while(sinkFound)
    {
        pa_mainloop_iterate(loop, 1, 0);
        
    }



    // pa_context_move_sink_input_by_name()


    //pa_mainloop_quit(loop, 0);
    return 0;

    

}
//ffmpeg -f -x11grab -video_size 1920,1280 -framerate 30 -i :0.0 -f pulse -i alsa_output.pci-0000_00_1f.3.analog_stereo.monitor filename.mp4