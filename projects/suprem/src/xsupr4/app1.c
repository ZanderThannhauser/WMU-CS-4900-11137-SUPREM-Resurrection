
#if LINUX

#include <stdio.h>
#include <stdlib.h>
#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>

#include <X11/Shell.h>
#ifndef ardent
#include <X11/Xaw/Dialog.h>
#include <X11/Xaw/Paned.h>
#include <X11/Xaw/Box.h>
#include <X11/Xaw/Command.h>
#include <X11/Xaw/Toggle.h>
#include <X11/Xaw/AsciiText.h>
#else
#include <X11/Dialog.h>
#include <X11/VPaned.h>
#include <X11/Box.h>
#include <X11/Command.h>
#include <X11/Toggle.h>
#include <X11/AsciiText.h>
#endif

#define check_width 16
#define check_height 16

static char check_bits[] = {
	0x00, 0xc0, 0x00, 0xc0, 0x00, 0x60, 0x00, 0x20, 0x00, 0x30, 0x00, 0x18,
	0x00, 0x08, 0x00, 0x0c, 0x00, 0x04, 0x01, 0x06, 0x03, 0x03, 0x06, 0x01,
	0x8c, 0x01, 0xd8, 0x00, 0xf0, 0x00, 0x60, 0x00};


#include "Graph.h"
#include "DataRec.h"
#include "xgraph.h"

/*2020 includes:*/
#include <debug.h>
#include "app1.h"
#include "interface.h"
#include "help.h"
/*end of includes*/

/*2020 forward declarations*/
void make_lab_dialog(Widget lab_shell, GraphWin * win, GraphWidget graph);
void make_print_dialog(Widget print_shell, GraphWidget gw);
/*end of declarations*/


static void do_label(widget, client_data, call_data)
Widget widget;
XtPointer client_data;
XtPointer call_data;
{
    Widget ps = (Widget)client_data;
    Position x, y;

    XtTranslateCoords(widget, 0, 0, &x, &y);
    XtVaSetValues(ps, XtNx, x+10, XtNy, y+10, NULL);
    XtPopup(ps, XtGrabNonexclusive);
}


static void Printout(widget, client_data, call_data)
Widget widget;
XtPointer client_data;   
XtPointer call_data;    
{
    Widget ps = (Widget)client_data;
    Position x, y;

    XtTranslateCoords(widget, 0, 0, &x, &y);
    XtVaSetValues(ps, XtNx, x+10, XtNy, y+10, NULL);
    XtPopup(ps, XtGrabNonexclusive);
}


static void cleanup(widget, client_data, call_data)
Widget widget;
XtPointer client_data;   /* cast to bigBitmap */
XtPointer call_data;    /* unused */
{
	xgClear();
	XtDestroyApplicationContext(app_context);
	app_context = NULL;
	free(newwin);
	newwin = NULL;
}
		
void make_graph( GraphWin* win )
{
	ENTER;
	Widget tL1, vpane, buttonbox;
	Widget unz, label, help, output;
	Widget device, print_shell, lab_shell, quit;
	int one = 1;
	char *argv[2];

	argv[0] = "xsuprem";
	argv[1] = NULL;

	if ( !toolkitinit ) {
		XtToolkitInitialize();
		toolkitinit = TRUE;
	}
	app_context = XtCreateApplicationContext();

	dpy = XtOpenDisplay(app_context, (String) NULL, NULL, "XSuprem", NULL, 0, &one, argv);
	
	verpv(dpy);
/*	CHECK;*/
	
	if (dpy == NULL)
		XtErrorMsg("invalidDisplay","xtInitialize","XtToolkitError", "Can't Open display", (String *) NULL, (Cardinal *)NULL);

	tL1 = XtVaAppCreateShell( NULL, "XSuprem", applicationShellWidgetClass, dpy, XtNscreen, (XtArgVal)DefaultScreenOfDisplay(dpy), NULL);
	
	HERE;
	vpane = XtVaCreateManagedWidget("vpane", panedWidgetClass, tL1, NULL);
	buttonbox = XtVaCreateManagedWidget("buttonbox", boxWidgetClass, vpane, NULL);
	graph = (GraphWidget) XtVaCreateManagedWidget("graph", graphWidgetClass, vpane, XtNGraphWin, win, NULL);
	HERE;
	
	/*Make the Help Button*/
	help = XtVaCreateManagedWidget("Help", commandWidgetClass, buttonbox, NULL);
	XtAddCallback(help, XtNcallback, help_win, NULL);

	/*Make the Label Button*/
	label = XtVaCreateManagedWidget("Axis", commandWidgetClass, buttonbox, NULL);
	lab_shell = XtVaCreatePopupShell("lshell", transientShellWidgetClass, tL1, NULL);
	make_lab_dialog(lab_shell, win, graph);
	XtAddCallback(label, XtNcallback, do_label, lab_shell);

	/*Make the unzoom button*/
	unz = XtVaCreateManagedWidget("Unzoom", commandWidgetClass, buttonbox, NULL);
	XtAddCallback(unz, XtNcallback, unzoom, (XtPointer) graph);

	/*Make the HardCopy Button*/
	output = XtVaCreateManagedWidget("Print", commandWidgetClass, buttonbox, NULL);

	print_shell = XtVaCreatePopupShell("pshell", transientShellWidgetClass, tL1, NULL);
	make_print_dialog(print_shell, graph);
	XtAddCallback(output, XtNcallback, Printout, print_shell);

	quit = XtVaCreateManagedWidget("Close", commandWidgetClass, buttonbox, NULL);
	XtAddCallback(quit, XtNcallback, cleanup, NULL);
	
	verpv(tL1);
	
/*	XtSetMappedWhenManaged(tL1, TRUE);*/
/*	XtSetMappedWhenManaged(vpane, TRUE);*/
	
	XtRealizeWidget(tL1);
	
/*	XtMapWidget(tL1);*/
	
	verpvb(XtIsRealized(tL1));
	
	XFlush(dpy);
/*	XmUpdateDisplay(tL1);*/
	
/*	sleep(1);*/
	
/*	CHECK;*/
	
	EXIT;
}

#include "HardInfo.h"
print_calldata foo, bar;

void make_print_dialog(Widget print_shell, GraphWidget gw)
{
	ENTER;
	Widget vpane, bb, dialog;
	Widget temp, t1, t2;
	Widget name;

	foo.print_shell = print_shell;

	vpane = XtVaCreateManagedWidget("PrintDialog", panedWidgetClass, print_shell, NULL);

	temp = XtVaCreateManagedWidget("Device Dialog", labelWidgetClass, vpane, NULL);

	bb = XtVaCreateManagedWidget("buttonbox1", boxWidgetClass, vpane, NULL);

	foo.dev_file = XtVaCreateManagedWidget("Device", toggleWidgetClass, bb, XtNradioData, DEVICE, XtNstate, TRUE, NULL);

	t2 = XtVaCreateManagedWidget("Filename", toggleWidgetClass, bb, XtNradioGroup, foo.dev_file, XtNradioData, FILEN, XtNstate, FALSE, NULL);

	bb = XtVaCreateManagedWidget("box1", boxWidgetClass, vpane, NULL);
	temp = XtVaCreateManagedWidget("Name: ", labelWidgetClass, bb, NULL);
	foo.dev_name = XtVaCreateManagedWidget("Name", asciiTextWidgetClass, bb, XtNeditType, XawtextEdit, NULL);

	bb = XtVaCreateManagedWidget("buttonbox2", boxWidgetClass, vpane, NULL);
	foo.dev_type = XtVaCreateManagedWidget("Postscript", toggleWidgetClass, bb, XtNradioData, PS, XtNstate, TRUE, NULL);
	temp = XtVaCreateManagedWidget("Idraw", toggleWidgetClass, bb, XtNradioGroup, foo.dev_type, XtNradioData, IDRAW, XtNstate, FALSE, NULL);
	temp = XtVaCreateManagedWidget("HPGL", toggleWidgetClass, bb, XtNradioGroup, foo.dev_type, XtNradioData, HPGL, XtNstate, FALSE, NULL);

	bb = XtVaCreateManagedWidget("buttonbox2", boxWidgetClass, vpane, NULL);
	foo.do_include = XtVaCreateManagedWidget("Include in Document", toggleWidgetClass, bb, FALSE, NULL);

	bb = XtVaCreateManagedWidget("buttonbox2", boxWidgetClass, vpane, NULL);

	temp = XtVaCreateManagedWidget("Print", commandWidgetClass, bb, NULL);
	foo.graph = gw;
	XtAddCallback(temp, XtNcallback, do_pr, &foo);

	temp = XtVaCreateManagedWidget("Cancel", commandWidgetClass, bb, NULL);
	XtAddCallback(temp, XtNcallback, do_cn, &foo);
	
	EXIT;
}

static void cancelLabel(widget, client_data, call_data)
Widget widget;
XtPointer client_data;   /* cast to bigBitmap */
XtPointer call_data;    /* unused */
{
    Widget ps = (Widget)client_data;
    XtPopdown(ps);
}

void make_lab_dialog(Widget lab_shell, GraphWin * win, GraphWidget graph)
{
	ENTER;
	Widget vpane, bb, temp;

	vpane = XtVaCreateManagedWidget("AxisLabels", panedWidgetClass, lab_shell, NULL);

	temp = XtVaCreateManagedWidget("Axis Labels", labelWidgetClass, vpane, NULL);

	bb = XtVaCreateManagedWidget("bb1", boxWidgetClass, vpane, NULL);
	temp = XtVaCreateManagedWidget("X Axis: ", labelWidgetClass, bb, NULL);
	bar.dev_file = XtVaCreateManagedWidget("Name", asciiTextWidgetClass, bb,  XtNstring, win->XUnitText, XtNeditType, XawtextEdit, NULL);

	bb = XtVaCreateManagedWidget("bb2", boxWidgetClass, vpane, NULL);
	temp = XtVaCreateManagedWidget("Y Axis: ", labelWidgetClass, bb, NULL);
	bar.dev_type = XtVaCreateManagedWidget("Name", asciiTextWidgetClass, bb, XtNstring, win->YUnitText, XtNeditType, XawtextEdit, NULL);

	bb = XtVaCreateManagedWidget("bb3", boxWidgetClass, vpane, NULL);
	temp = XtVaCreateManagedWidget("Title: ", labelWidgetClass, bb, NULL);
	bar.dev_name = XtVaCreateManagedWidget("Name", asciiTextWidgetClass, bb, XtNstring, win->Title, XtNeditType, XawtextEdit, NULL);

	bb = XtVaCreateManagedWidget("bb4", boxWidgetClass, vpane, NULL);

	bar.graph = graph;
	temp = XtVaCreateManagedWidget("Done", commandWidgetClass, bb, NULL);
	XtAddCallback(temp, XtNcallback, add_label, &bar);
	XtAddCallback(temp, XtNcallback, cancelLabel, lab_shell);

	temp = XtVaCreateManagedWidget("Cancel", commandWidgetClass, bb, NULL);
	XtAddCallback(temp, XtNcallback, cancelLabel, lab_shell);
	
	EXIT;
}

void do_error(char *str)
{
	printf ("%s\n", str);
}

#endif












