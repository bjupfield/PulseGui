#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> //this is for sleep remove plz

#include "GLInterface.h"

/*
* Event Handling Functions
*/
int handleEvent(XEvent* event, Display* display, Window* window);

int notifyChangeHandle(XConfigureEvent event, Window* window, Display* display);




int plzDestroy = 1;

int main(int argc, char **argv)
{
    Display* display = XOpenDisplay(NULL);

    XSetWindowAttributes wa = {
		.override_redirect = False,
  		//.background_pixmap = ParentRelative,
		.event_mask = 
            ButtonPressMask|ButtonReleaseMask|KeyPressMask|KeyReleaseMask|
            EnterWindowMask|LeaveWindowMask|ExposureMask|PointerMotionMask|
            VisibilityChangeMask|StructureNotifyMask,
		.background_pixel = BlackPixel(display, 0),
        .colormap = DefaultColormap(display, 0),
	};

    Window w = XCreateWindow(
        display, RootWindow(display, 0), 400, 400, 300, 300, 0, 					
        DefaultDepth(display, 0), CopyFromParent, DefaultVisual(display, 0), 
		CWBackPixel|CWEventMask|CWColormap|CWOverrideRedirect, &wa);



    XMapWindow(display, w);

    XFlush(display);
    /*
    * not really needed as XPending(), XNextEvent, and XWindowEvent(), all flush the display
    * buffer to the xserver,
    * same with XSync(display, 0), which flushes both the xservers and the displays buffers
    */
    //sleep(6);

    XEvent event;

    if(queryServer(display))
    {
        printf("Has X Server || Available Configs: %i\n", printConfigs(display));
    }

    while(plzDestroy)
    {
        XNextEvent(display, &event);
        handleEvent(&event, display, &w);
    }


    XDestroyWindow(display, w);
    XFlush(display);

    sleep(4);


    XCloseDisplay(display);

    return 0;

}
int handleEvent(XEvent* event, Display* display, Window* window)
{
    printf("EventType: %i\n", event->type);
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
    XWindowAttributes attri;
    XGetWindowAttributes(display, *window, &attri);
    printf("Old Window X: %i || Y: %i", attri.width, attri.height);
    //this displays that old windows get updated by the notifychanges :)

    return 0;
}