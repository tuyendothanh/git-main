/*    Cap 5. The Graphic Context
 *
 *    1. The GC is very much like the window attributes are set.
 *       The structure name is XGCValues and is made up of these symbols:
 *
 *       member
 *       --------------------------------------------------------------
 *       function
 *       plane_mask
 *       foreground
 *       background
 *       line_width
 *       line_style
 *       cap_style
 *       join_style
 *       fill_style
 *       fill_rule
 *       arc_mode
 *       tile
 *       stipple
 *       ts_x_origin
 *       ts_y_origin
 *       font
 *       subwindow_mode
 *       graphics_exposure
 *       clip_x_origin
 *       clip_y_origin
 *       clip_mask
 *       dash_offset
 *       dashes
 */

#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include <stdio.h>
#include <stdlib.h>




int main(int argc, char **argv)
{
	setvbuf(stdout, NULL, _IONBF, 0);
	Display* dpy = XOpenDisplay(NULL);
	if(!dpy) return -1;

	int scrno = DefaultScreen(dpy);
	int scrwidth = DisplayWidth(dpy, scrno);
	int scrheight = DisplayHeight(dpy, scrno);


	int winwidth = scrwidth / 3;
	int winheight = scrheight / 2;

	int winborder = 1;

	Window win = XCreateSimpleWindow(dpy,
	                                 RootWindow(dpy, scrno),
	                                 10, 10,
	                                 winwidth, winheight,
	                                 winborder,
	                                 BlackPixel(dpy, scrno),
	                                 WhitePixel(dpy, scrno));


	char* title = (char*)"gc1 - simple GC";
	XTextProperty titleprop;
	XStringListToTextProperty(&title, 1, &titleprop);
	XSetWMProperties(dpy,
	                 win,
	                 &titleprop,
	                 NULL,
	                 argv,
	                 argc,
	                 NULL,
	                 NULL,
	                 NULL);

	XSelectInput(dpy,
	             win,
	             ExposureMask |
	             KeyPressMask |
	             ButtonPressMask |
	             StructureNotifyMask);



	XGCValues gcvalues;
	gcvalues.line_width = 1;
	unsigned long gcmask = GCLineWidth;

	GC gc = XCreateGC(dpy,
	                  win,
	                  gcmask,
	                  &gcvalues);

	XMapWindow(dpy, win);

	XEvent event;
	while (1)
	{
		XNextEvent(dpy, &event);
		if(event.type == Expose &&
		   event.xexpose.count == 0)
		{
			XDrawRectangle(dpy,
			               win,
			               gc,
			               winwidth/4, winheight/4,
			               winwidth/2, winheight/2);
			printf("\nXDrawRectangle\n");
		}
	}

	XFreeGC(dpy, gc);
	XCloseDisplay(dpy);
	return 0;
}
