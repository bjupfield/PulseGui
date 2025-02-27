#include "mainloop.h"
#include "introspect.h"
#include "subscribe.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

//States
uint32_t clientsSinkFound = 1;
uint32_t serverFound = 1;
uint32_t defaultSinkIndexFound = 1;
uint32_t eventOccured = 0;
uint32_t selectedSinkNameNotFound = 1;
uint32_t run = 1;
uint32_t loaded = 1;
uint32_t done = 1;
//Ids/Name/Info
pa_client_info* clientList;
uint16_t clientCount;

uint32_t selectedClientIdx;//this is the cleints id
pa_sink_input_info clientsInputSink;
pa_sink_info clientsSink;

uint32_t loadedModuleIndex;

pa_server_info server;//sever info
pa_context_state_t contextState;//contextstate
void clientListInfoCallback(pa_context *context, const pa_client_info *client, int eol, void *userdata)
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
void sinkListInfoCallback(pa_context *c, const pa_sink_input_info *sink, int eol, void *userdata)
{
    if(eol > 0)
    {
        return;
    }
    printf("Selected: %i, Sink: %i\n",selectedClientIdx, sink->client);
    if(sink->client == selectedClientIdx)
    {

        printf("Sink Index: %i\n",sink->sink);
        clientsSinkFound = 0;
        memcpy(&clientsInputSink, sink, sizeof(pa_sink_input_info));
        clientsInputSink.name = (sink->name);

    }
    return;
}
void sinkListInfoCallback2(pa_context *c, const pa_sink_info *sink, int eol, void *userdata)
{
    if(eol > 0)
    {
        return;
    }
    printf("Client Name: %s\nClient Index:%i\n|||||||||||||||||||\n", sink->name, sink->index);
    run = 0;
    return;
}
void serverInfoCallback(pa_context *context, const pa_server_info *servers, void *userdata)
{
    printf("%s\n|||||||||||||||||||\n", servers->default_sink_name);
    server.default_sink_name = strdup(servers->default_sink_name);
    server.default_source_name = strdup(servers->default_source_name);
    server.host_name = strdup(servers->host_name);
    server.server_name = strdup(servers->server_name);
    serverFound = 0;

    return;
}
void sinkNameInfoCallback(pa_context *context, const pa_sink_info *info, int eol, void *userdata)
{
    if(eol > 0 || info == NULL)
    {
        return;
    }
    defaultSinkIndexFound = 0;
    printf("Default Sink Index: %i\n", info->index);
    return;
}
void subscriptionAssignedCallback(pa_context *c, int success, void *userdata)
{
    
    printf("Success?: %i\n", success);
    eventOccured++;
    return;
}
void subscriptionEventCallback(pa_context *c, pa_subscription_event_type_t t, uint32_t idx, void *userdata)
{
    switch (t & PA_SUBSCRIPTION_EVENT_FACILITY_MASK)
    {
    case PA_SUBSCRIPTION_EVENT_SINK_INPUT:
        printf("\nsinnk input action\n");
        switch(t & PA_SUBSCRIPTION_EVENT_TYPE_MASK)
        {
            case PA_SUBSCRIPTION_EVENT_NEW:
                printf("New");
                break;
            case PA_SUBSCRIPTION_EVENT_CHANGE:
                printf("Change");
                break;
            case PA_SUBSCRIPTION_EVENT_REMOVE:
                printf("Remove");
                break;
            default:
                printf("Default");
                break;
        }
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
void sinkInfoByIdxCallBack(pa_context *context, const pa_sink_info *info, int eol, void *userdata)
{
    if(eol > 0)
    {
        return;
    }
    selectedSinkNameNotFound = 0;
    memcpy(&clientsSink, info, sizeof(pa_sink_info));
    clientsSink.name = strdup(info->name);
    //there is more info that needs to be manually copied due to being a pointer itself, but this is all that im 
    //willing to do right now
    
}
void loadModuleCallback(pa_context *c, uint32_t idx, void *userdata)
{
    loadedModuleIndex = idx;
    printf("\n\nCombine Sink Loaded || ID: %i\n\n", idx);
    loaded = 0;
}
void unloadModuleCallback(pa_context *c, int success, void *userdata)
{
    done = 0;
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
void printSinkInputData(pa_sink_input_info* sink)
{
    printf("Sink Input Name: %s\n", sink->name);
    if(sink->has_volume)
    {
        for(int i = 0; i < sink->volume.channels; i++)
        {
            printf("Channel[] Volume: %i\n", sink->volume.values[i]);
        }
    }
    printf("Sink Driver: %s \n", sink->driver);
    printf("Sink SampleMethod: %s \n", sink->resample_method);
    printf("'Corked Value': %i \n", sink->corked);
    printf("Client Idx: %i \n", sink->client);
    printf("Sink Idx: %i \n", sink->sink);
    printf("Owner Module Idx: %i \n", sink->owner_module);
    printf("Sample Rate: %i\n", sink->sample_spec.rate);
    printf("Sample Channels: %i\n", sink->sample_spec.channels);
    printf("Audio Clarity: %i\n", sink->sample_spec.format);
    for(int i = 0; i < sink->channel_map.channels; i++)
    {
        printf("Channel Position[%i]: %i\n", i, sink->channel_map.map[i]);
    }


}
void mainloopRUN()
{
}
int main(void*)
{
    clientList = (pa_client_info*)malloc(sizeof(pa_client_info) * 32);
    clientCount = 0;
    
    int32_t contextAlive = 0;

    char* badScan = (char*)malloc(8 * 32);

    pa_mainloop *loop = NULL;
    pa_context* context;
    setup(&loop, &context);
    //will neeed below later
    //pa_context_load_module(context, "Fake-Module", "module-null-sink", NULL, loop);
    
    printf("|||||||||||||||||||\n");
    while(contextState != PA_CONTEXT_READY)//iterating untill the context is ready for now... do something better later
        contextAlive = pa_mainloop_iterate(loop, 1, 0);
    
    pa_context_get_client_info_list(context, clientListInfoCallback, loop);
    pa_context_get_server_info(context, serverInfoCallback, loop);
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
            selectedClientIdx = clientList[i].index;
        }

    }

    printf("\n\n||||||||||||||\n");

    pa_context_get_sink_input_info_list(context, sinkListInfoCallback, loop);
    pa_context_get_sink_info_by_name(context, server.default_sink_name, sinkNameInfoCallback, loop);

    while(clientsSinkFound || defaultSinkIndexFound)
        pa_mainloop_iterate(loop, 1, 0);

    pa_context_get_sink_info_by_index(context, clientsInputSink.sink, sinkInfoByIdxCallBack, loop);
    while(selectedSinkNameNotFound)
    {
        pa_mainloop_iterate(loop, 1, 0);
    }

    // pa_context_subscribe(context, PA_SUBSCRIPTION_MASK_ALL, subscriptionAssignedCallback, loop);
    // pa_context_set_subscribe_callback(context, subscriptionEventCallback, loop);
    // while(eventOccured < 5)
    //     pa_mainloop_iterate(loop, 1, 0);
    /*
    * With the functions above will eventually need to establish a connection system that
    * identifys which client to target than checks to see if the client has already established a connection
    * with a sink. If the client has not established a connection than wait for client to establish connection
    * with a subscription event, once the client does establish a connection, capture it than send it to 
    * the combine sink shown below
    */

    // pa_context_move_sink_input_by_name()


    char loadArg [200] = "sink_name=PulseGUI_Sink slaves=";//obviously change sinks later to something more direct
    /*
    * okay so for the load_module for our module-combine-sink argument
    * we have the problem of strings in c not being defined in length or memory safe 
    * and chat even lied to me about pulseaudio having a defined name length, which Im sure it does, but I cant
    * find it anywhere
    * but because I need a maximum argument length for my string here, ill go with 200, because
    * using the module-combine-sink does actually run into an issue. if a user wants to capture audio
    * going to different into the same audio data, the problem is that a combine-sink would combine these,
    * which isnt what we want. I dont know how audio tech actually works, but what if in our fictional reality,
    * a user is like speaking into a microphone, and playing audio throguh a guitar jack  and they want to record
    * the information. If these streams are going to seperate speakers, than recording it through a combine-sink would
    * destroy this seperation of speakers. So because of this, we will only ever be using a single slave-sink with
    * the combine sink set up, so we don't neeed the loadArgument to be dynamically allocated, as it will always be within
    * a few hundred chars
    * to fix this problem we would have to use the null sink method and manually process the playback operations
    * where we would individually send the streams to the requisite sinks
    */

    strncat(loadArg, clientsSink.name, 150);

    pa_context_load_module(context, "module-combine-sink", loadArg, loadModuleCallback, loop);
    /*
    * When Loading a Module a client is required to handle the unloading of the module as well
    * so just remmeber to unload the module not to clog the pulseaudio space :3
    */
    while(loaded)
    {
        pa_mainloop_iterate(loop, 1, 0);
    }

    pa_context_get_sink_info_list(context, sinkListInfoCallback2, loop);
    while(run)
    {
        pa_mainloop_iterate(loop, 1, 0);
    }

    printSinkInputData(&clientsInputSink);

    pa_context_move_sink_input_by_name(context, clientsInputSink.index, "PulseGUI_Sink", unloadModuleCallback, loop);
    while(done)
    {
        pa_mainloop_iterate(loop, 1, 0);
    }

    clientsSinkFound = 1;
    pa_context_get_sink_input_info_list(context, sinkListInfoCallback, loop);
    while(clientsSinkFound)
    {
        pa_mainloop_iterate(loop, 1, 0);
    }

    sleep(10);

    printSinkInputData(&clientsInputSink);

    done = 1;
    pa_context_move_sink_input_by_index(context, clientsInputSink.index, clientsSink.index, unloadModuleCallback, loop);
    while(done)
    {
        pa_mainloop_iterate(loop, 1, 0);
    }


    done = 1;
    pa_context_unload_module(context, loadedModuleIndex, unloadModuleCallback, loop);//not making a callback for this for now
    while(done)
    {
        pa_mainloop_iterate(loop, 1, 0);
    }
    
    pa_mainloop_quit(loop, 0);
    return 0;

    /*
    * The rerouting through a combine sink works, just tested by sending spotify music through a different
    * "slave" than the one it was originally routed to. so now we just need to set up a monitor on that sink
    * and send the audio to ffmpeg or whatever program is desired I guess
    * the combine-sink isn't recieving the same volume data I guess, so I think it probably doesn't have any
    * of the original sink-inputs properties, so that needs to be transfered somehow, otherwise nice! cute!
    * awesome!
    */

}
//ffmpeg -f -x11grab -video_size 1920,1280 -framerate 30 -i :0.0 -f pulse -i alsa_output.pci-0000_00_1f.3.analog_stereo.monitor filename.mp4