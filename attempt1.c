#include "mainloop.h"
#include "introspect.h"
#include "subscribe.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

pa_client_info* clientList;
pa_server_info server;
pa_context_state_t contextState;
uint16_t clientCount;
uint32_t selectedClient;
uint32_t sinkFound = 1;
uint32_t originalSink;
uint32_t stateMachine;
uint32_t inputSink;
uint32_t serverFound = 1;
uint32_t realFound = 1;
uint32_t eventOccured = 0;

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
        sinkFound = 0;
    }
    return;
}
void c(pa_context *context, const pa_server_info *servers, void *userdata)
{
    printf("%s\n|||||||||||||||||||\n", servers->default_sink_name);
    server.default_sink_name = strdup(servers->default_sink_name);
    server.default_source_name = strdup(servers->default_source_name);
    server.host_name = strdup(servers->host_name);
    server.server_name = strdup(servers->server_name);
    serverFound = 0;

    return;
}
void d(pa_context *context, const pa_sink_info *info, int eol, void *userdata)
{
    if(eol > 0 || info == NULL)
    {
        return;
    }
    realFound = 0;
    printf("Info: %i\n", info->index);
    return;
}
void e(pa_context *c, int success, void *userdata)
{
    
    printf("Success?: %i\n", success);
    eventOccured++;
    return;
}
void f(pa_context *c, pa_subscription_event_type_t t, uint32_t idx, void *userdata)
{
    switch (t & PA_SUBSCRIPTION_EVENT_FACILITY_MASK)
    {
    case PA_SUBSCRIPTION_EVENT_SINK_INPUT:
        printf("\nsinnk input action\n");
        eventOccured++;
        break;
    case PA_SUBSCRIPTION_EVENT_SINK:
    /**< Event type: Sink */
        break;
    case PA_SUBSCRIPTION_EVENT_SOURCE:
    /**< Event type: Source */
        break;

    case PA_SUBSCRIPTION_EVENT_SOURCE_OUTPUT:
    /**< Event type: Source output */
        break;

    case PA_SUBSCRIPTION_EVENT_MODULE:
    /**< Event type: Module */
        break;

    case PA_SUBSCRIPTION_EVENT_CLIENT:
    /**< Event type: Client */
        break;

    case PA_SUBSCRIPTION_EVENT_SAMPLE_CACHE:
    /**< Event type: Sample cache item */
        break;

    case PA_SUBSCRIPTION_EVENT_SERVER:

        break;
    default:
        break;
    }
    return;
}
void context_state_callback(pa_context *context, void *userdata) {
    
    contextState = pa_context_get_state(context);
    
}

void setup(pa_mainloop** loop, pa_context** context)
{
    if(*loop != NULL)
    {
        pa_mainloop_free(*loop);
        printf("hi");
    }
    *loop = pa_mainloop_new();
    *context = pa_context_new(pa_mainloop_get_api(*loop), "Fake");
    pa_context_connect(*context, NULL, PA_CONTEXT_NOAUTOSPAWN, NULL);
    pa_context_set_state_callback(*context, context_state_callback, *loop);
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
    pa_context* context;
    setup(&loop, &context);
    //will neeed below later
    //pa_context_load_module(context, "Fake-Module", "module-null-sink", NULL, loop);
    
    printf("|||||||||||||||||||\n");
    while(contextState != PA_CONTEXT_READY)
        contextAlive = pa_mainloop_iterate(loop, 1, 0);
    
    pa_context_get_client_info_list(context, a, loop);
    pa_context_get_server_info(context, c, loop);
    while(clientCount == 0 || serverFound)
        pa_mainloop_iterate(loop, 1, 0);
    
    printf("Client count: %i\nDefault Sink: %s\nChoose Client: ", clientCount, server.default_sink_name);
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

    printf("Default sinkName: %s\n||||||||||||||\n", server.default_sink_name);

    pa_context_get_sink_input_info_list(context, b, loop);
    pa_context_get_sink_info_by_name(context, server.default_sink_name, d, loop);

    while(sinkFound || realFound)
        pa_mainloop_iterate(loop, 1, 0);

    pa_context_subscribe(context, PA_SUBSCRIPTION_MASK_ALL, e, loop);
    pa_context_set_subscribe_callback(context, f, loop);
    while(eventOccured < 5)
        pa_mainloop_iterate(loop, 1, 0);

    // pa_context_move_sink_input_by_name()


    //pa_mainloop_quit(loop, 0);
    return 0;

}
//ffmpeg -f -x11grab -video_size 1920,1280 -framerate 30 -i :0.0 -f pulse -i alsa_output.pci-0000_00_1f.3.analog_stereo.monitor filename.mp4