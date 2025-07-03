#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> //this is for sleep remove plz, do not remove this is now for fork
#include<stdalign.h>

//#include "GLInterface.h"
#include "swcWindow.h"

/*
* Event Handling Functions
*/
int handleEvent(XEvent* event, Display* display, Window* window);

int notifyChangeHandle(XConfigureEvent event, Window* window, Display* display);

int errorHandler(Display* display, XErrorEvent* ErrorEvent);

XWindowAttributes attri;


int plzDestroy = 1;

int main(int argc, char **argv)
{

    // uint32_t pid = fork();
    // if(pid > 0)
    // {
    //     return 0;
    // }
    // else if(pid < 0)
    // {
    //     printf("Failed to Init");
    //     return 0;
    // }

    printf("mapping should start\n");
    // return 0;

    initWindow(defConfiguration, defMask, 0, 0, 200, 200);
    Display* display = XOpenDisplay(NULL);

    //we will set an errorhandler down the line, but I don't really want to mess with it now
    XSetErrorHandler(errorHandler);

    XVisualInfo *info = NULL;
    if(queryServer(display))
    {
        printf("Has X Server || Has a Config: %i\n", retrieveConfig(display, defConfiguration));
        info = retrieveVisual(display);
    }


    XSetWindowAttributes wa = {
		.override_redirect = False,
  		//.background_pixmap = ParentRelative,
		.event_mask = 
            ButtonPressMask|ButtonReleaseMask|KeyPressMask|KeyReleaseMask|
            EnterWindowMask|LeaveWindowMask|ExposureMask|PointerMotionMask|
            VisibilityChangeMask|StructureNotifyMask,
		.background_pixel = BlackPixel(display, 0),
        .colormap = XCreateColormap(display, RootWindow(display, 0), info->visual, AllocNone),
	};

    Window w = XCreateWindow(
        display, RootWindow(display, 0), 400, 400, 300, 300, 0, 					
        info->depth, CopyFromParent, info->visual, 
		CWBackPixel|CWEventMask|CWColormap|CWOverrideRedirect, &wa);

    XFree(info);

    initializeWindow(display, w);

    XMapWindow(display, w);

    XFlush(display);
    /*
    * not really needed as XPending(), XNextEvent, and XWindowEvent(), all flush the display
    * buffer to the xserver,
    * same with XSync(display, 0), which flushes both the xservers and the displays buffers
    */
    //sleep(6);

    XEvent event;

    createAttachProgram(display, attri);
    //printf("Buffer Amount: %i\n", currenting(display));

    while(plzDestroy)
    {
        uint32_t eventsQueued = XEventsQueued(display, QueuedAfterFlush);
        //printf("%i\n", eventsQueued);

        XNextEvent(display, &event);
        handleEvent(&event, display, &w);
    }


    XDestroyWindow(display, w);
    XFlush(display);

    //sleep(4);

    destroyGLX(display);

    XCloseDisplay(display);

    printf("\n MaxAlignOf: %li\n", sizeof(swcDiv));

    uint32_t count = 1;
    printf("\n\n1++: %i\n", sizeof(funcPointer));


    return 0;

}
int handleEvent(XEvent* event, Display* display, Window* window)
{
    //printf("EventType: %i\n", event->type);
    if(event->type == KeyPress)
    {
        XKeyEvent* myKeys = &event->xkey;
        // myKeys->keycode == X_
        KeySym pressedKey;
        pressedKey = XKeycodeToKeysym(myKeys->display, myKeys->keycode, 0);
        // printf("PressedKey in String: %s || PressedKeyInKeyCode: %i", );
        if(pressedKey == XK_q)
        {
            plzDestroy = 0;
        }

    }
    switch(event->type)
    {
        case ConfigureNotify:
            notifyChangeHandle(event->xconfigure, window, display);
            break;
        default:

            break;
    }

    return 0;
}
int notifyChangeHandle(XConfigureEvent event, Window* window, Display* display)
{
    attri;

    XGetWindowAttributes(display, *window, &attri);
    //printf("Old Window X: %i || Y: %i ||| New Window? X: %i || Y: %i\n", attri.width, attri.height, event.width, event.height);
    //this displays that old windows get updated by the notifychanges :)

    XWindowAttributes attri2;
    XGetWindowAttributes(display, event.event, &attri2);
    printf("Old Window X: %i || Y: %i ||| New Window? X: %i || Y: %i ||| Third Value? X: %i, || Y: %i\n", attri.width, attri.height, event.width, event.height, attri2.width, attri2.height);

    //after some deliberation, I think we should use event.width, it looks like other windows use this, instead of querying for the attributes

    adjustViewport(display, event.width, event.height);

    return 0;
}

/*
* This is actually a crazy function, I don't really know how to interact with it yet, only because I don't
* Really want a centralized state machine, which is what this function requires. I'll see what I want eventually
* but just look up XSetErrorHandler, xorg gives a detailed description 
* 
*/
int errorHandler(Display* display, XErrorEvent* ErrorEvent)
{
    // printf(
    //     "X Errror of Failed Request:  %s\n  Major Opcode of Failed Request:  %s\n  Serial Number of Failed Request: %i\n  Current Serial Number in Output Stream:  %i",
    //     ErrorEvent->request_code, ErrorEvent->error_code, ErrorEvent->resourceid, ErrorEvent->serial
    //     );
    return 0;
}