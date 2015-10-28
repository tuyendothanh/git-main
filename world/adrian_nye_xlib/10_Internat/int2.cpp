/*
 *    10.1.2 Text representation in an Internationalized Application
 *
 *    Let's assume we have a resource with non-english text and we have to
 *    display messages from this resource.
 *    See int1.cpp for details about the text resource.
 */


#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>

#include <nl_types.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>



int main(int argc, char **argv)
{
	// X/Open Message Catalog
	printf("\n\topen catalog: %s.msg", argv[0]);
	char catpath[250];
	strcpy(catpath, argv[0]);
	strcat(catpath, ".msg");

	nl_catd catd = catopen(catpath, 0);
	if(catd == (nl_catd)-1)
	{
		printf("\nCannot open catalog\n");
		return 1;
	}


	setvbuf(stdout, NULL, _IONBF, 0);
	Display* dpy = XOpenDisplay(NULL);
	if(!dpy) return -1;

	int scrno = DefaultScreen(dpy);
	int scrwidth = DisplayWidth(dpy, scrno);
	int scrheight = DisplayHeight(dpy, scrno);


	int x = 10;
	int y = 10;
	int winwidth = scrwidth / 5;
	int winheight = scrheight / 6 ;
	int winborder = 1;


	Window win = XCreateSimpleWindow(dpy,
	                                 RootWindow(dpy, scrno),
	                                 x, y,
	                                 winwidth, winheight,
	                                 winborder,
	                                 BlackPixel(dpy, scrno),
	                                 WhitePixel(dpy, scrno));


	/* X.h
	   line: 150 to 175   Event definitions;
	*/
	long event_mask =
		ExposureMask |
		ButtonPressMask |
		StructureNotifyMask;

	/* Select all events for our top window. */
	XSelectInput(dpy, win, event_mask);


	/* Select a nice font */
	int fontret = 0;
	XFontStruct* fontinfos;
	char** fontlist = XListFontsWithInfo(dpy,
	                                     "*",
	                                     1200,        // maxnames
	                                     &fontret,    // actual_count_return
	                                     &fontinfos); // info_return
	if(fontlist == NULL)
		return -1;



	/*  R5 bases its new text output rutines on a new Xlib abstraction, XFontSet.
	    The new R5 functions use an XFontSet rather than an XFontStruct or a
	    font specified in a graphics context.
	    The drawing and measuring routines interpret text in the encoding of the
	    locale of the fontset, and correctly map wide or multi-byte characters
	    to the corresponding font gplyphs.
	*/
	Font     latinFont;
	XFontSet latinFontSet;
	char **missing_charsets;
	int num_missing_charsets;
	char *default_string;


	// The Latin Font
	for(int i=0; i < fontret; ++i)
	{
		XFontStruct* fs = &(fontinfos[i]);
		XCharStruct* cs = &(fs->max_bounds);
		int fsz = cs->ascent + cs->descent;
		if(fsz < 12 || fsz > 14)
			continue;
		const char* fname = fontlist[i];
		// -medium-  -normal-  -r-
		if(strstr(fname, "-medium-") == 0 ||
		   strstr(fname, "-normal-") == 0 ||
		   strstr(fname, "-r-")      == 0 ||
		   strstr(fname, "iso8859-1")== 0 )
			continue;

		// found a Latin 8859-1 Font
		printf("\nFound Latin Font: %s", fname);

		latinFont = XLoadFont(dpy, fname);


		// set locale before XCreateFontSet
/*
		setlocale(LC_ALL, "en_US.UTF-8");
		if (!XSupportsLocale())
			printf("\nX does not support locale en_US.UTF-8");
		if (XSetLocaleModifiers("") == NULL)
			printf("\nX cannot set en_US.UTF-8 locale modifiers");
*/

		// XFontSet for drawing strings in Latin-1
		latinFontSet = XCreateFontSet(dpy,
		                              fname,
		                              &missing_charsets,
		                              &num_missing_charsets,
		                              &default_string);

		if (num_missing_charsets > 0)
		{
			printf("\nThe following charsets are missing:");
			for (int i=0; i < num_missing_charsets; ++i)
			{
				printf("\n%s", missing_charsets[i]);
			}
			XFreeStringList(missing_charsets);
			num_missing_charsets = 0;
		}

		break;
	}



	Font     russianFont;
	XFontSet russianFontSet;

	// The Cyrillic Font
	for(int i=0; i < fontret; ++i)
	{
		XFontStruct* fs = &(fontinfos[i]);
		XCharStruct* cs = &(fs->max_bounds);
		int fsz = cs->ascent + cs->descent;
		if(fsz < 12 || fsz > 14)
			continue;
		const char* fname = fontlist[i];

		// -medium-  -normal-  -r-
		if(strstr(fname, "-medium-") == 0 ||
		   strstr(fname, "-normal-") == 0 ||
		   strstr(fname, "-r-")      == 0 ||
		   strstr(fname, "koi8-r")   == 0)
			continue;

		// found a Cyrillic Russian koi8-r Font
		printf("\nFound Cyrillic Font: %s", fname);

		russianFont = XLoadFont(dpy, fname);


		// $ localectl list-locale
		// set locale before XCreateFontSet
/*
		setlocale(LC_ALL, "ru_RU.koi8r");
		if (!XSupportsLocale())
			printf("\nX does not support locale ru_RU.koi8r");
		if (XSetLocaleModifiers("") == NULL)
			printf("\nX cannot set ru_RU.koi8r locale modifiers");
*/

		russianFontSet = XCreateFontSet(dpy,
		                                fname,
		                                &missing_charsets,
		                                &num_missing_charsets,
		                                &default_string);

		if (num_missing_charsets > 0)
		{
			printf("\nThe following charsets are missing:");
			for (int i=0; i < num_missing_charsets; ++i)
			{
				printf("\n%s", missing_charsets[i]);
			}
			XFreeStringList(missing_charsets);
			num_missing_charsets = 0;
		}

		break;
	}


	// create a GC
	XGCValues gcvalues;
	gcvalues.line_width = 1;
	gcvalues.foreground = BlackPixel(dpy, scrno);
	gcvalues.background = WhitePixel(dpy, scrno);
	gcvalues.font = latinFont;
	unsigned long gcmask = GCLineWidth | GCForeground | GCBackground | GCFont;

	GC gc = XCreateGC(dpy,
	                  win,
	                  gcmask,
	                  &gcvalues);

	XMapWindow(dpy, win);

	// set={1,2,3}  id={1,2,3}
	int set = 1, id = 1;


	XEvent event;
	while (1)
	{
		XNextEvent(dpy, &event);

		if (event.type == Expose)
		{
			XExposeEvent* xexpose = &(event.xexpose);
			if (xexpose->count == 0)
			{
				XClearWindow(dpy, win);
				const char* msg = catgets(catd, set, id, "#translation error!");
				int len = strlen(msg);
				printf("\nset=%d  id=%d  [%d][%s]", set, id, len, msg);

				if (set == 1)
					XmbDrawImageString(dpy, win, latinFontSet, gc, 50, 100, msg, len);
				else if (set == 2)
					XmbDrawImageString(dpy, win, russianFontSet, gc, 50, 100, msg, len);
			}
		}

		if (event.type == ConfigureNotify)
		{
			XClearWindow(dpy, win);
			const char* msg = catgets(catd, set, id, "#translation error!");
			int len = strlen(msg);
			printf("\nset=%d  id=%d  [%d][%s]", set, id, len, msg);

			if (set == 1)
				XmbDrawImageString(dpy, win, latinFontSet, gc, 50, 100, msg, len);
			else if (set == 2)
				XmbDrawImageString(dpy, win, russianFontSet, gc, 50, 100, msg, len);
		}


		if (event.type == ButtonPress)
		{
			++id;
			if (id >= 4)
			{
				id = 1;
				++set;
				if (set >= 4)
					set = 1;

				if (set == 1)
				{
//					setlocale(LC_ALL, "en_US.UTF-8");
					XSetFont(dpy, gc, latinFont);
				}

				if (set == 2)
				{
//					setlocale(LC_ALL, "ru_RU.koi8r");
					XSetFont(dpy, gc, russianFont);
				}
			}
		}
	}


	XFreeFontSet(dpy, latinFontSet);
	XFreeFontSet(dpy, russianFontSet);

	XUnmapWindow(dpy, win);
	XDestroyWindow(dpy, win);
	XCloseDisplay(dpy);

	int ret = catclose(catd);
	printf("\n\tCatalog closed %s\n\n", (ret == 0 ? "OK" : "with error!"));

	return 0;
}
