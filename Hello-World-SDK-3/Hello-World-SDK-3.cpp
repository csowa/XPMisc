// Downloaded from https://developer.x-plane.com/code-sample/hello-world-sdk-3/


#include "XPLMDataAccess.h"
#include "XPLMPlugin.h"
#include "XPLMProcessing.h"
#include "XPLMDisplay.h"
#include "XPLMGraphics.h"

//#include "XPWidgetDefs.h"
//#include "XPWidgets.h"
//#include "XPStandardWidgets.h"
//#include "XPWidgetUtils.h"
//#include "XPUIGraphics.h"

#include <string.h>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <list>
#include <vector>
#include <string>

#include "Leap.h"
#include "LMSample.h"

using namespace std;
using namespace Leap;


#if IBM
	#include <windows.h>
#endif
#if LIN
	#include <GL/gl.h>
#elif __GNUC__
	#include <OpenGL/gl.h>
#else
	#include <GL/gl.h>
#endif

#ifndef XPLM300
	#error This is made to be compiled against the XPLM300 SDK
#endif

#define LISTBOX_ITEM_HEIGHT 12

// An opaque handle to the window we will create
static XPLMWindowID	g_window;
static long indexflcb = 0;
static unsigned int maxlistitems;

//// same for widget
//static XPWidgetID g_widget;
//static XPWidgetID g_listWidget;

// Callbacks we will register when we create our window
void				draw_hello_world(XPLMWindowID in_window_id, void * in_refcon);
int					dummy_mouse_handler(XPLMWindowID in_window_id, int x, int y, int is_down, void * in_refcon) { return 0; }
XPLMCursorStatus	dummy_cursor_status_handler(XPLMWindowID in_window_id, int x, int y, void * in_refcon) { return xplm_CursorDefault; }
int					dummy_wheel_handler(XPLMWindowID in_window_id, int x, int y, int wheel, int clicks, void * in_refcon) { return 0; }
void				dummy_key_handler(XPLMWindowID in_window_id, char key, XPLMKeyFlags flags, char virtual_key, void * in_refcon, int losing_focus) { }

static list<string> ListItems;
static Frame frame;

//float	FlightLoopCallback(float elapsedMe, float elapsedSim, int counter, void * refcon);

PLUGIN_API int XPluginStart(
							char *		outName,
							char *		outSig,
							char *		outDesc)
{
	strcpy(outName, "HelloWorld3Plugin");
	strcpy(outSig, "xpsdk.examples.helloworld3plugin");
	strcpy(outDesc, "A Hello World plug-in for the XPLM300 SDK.");
	
	XPLMCreateWindow_t params;
	params.structSize = sizeof(params);
	params.visible = 1;
	params.drawWindowFunc = draw_hello_world;
	// Note on "dummy" handlers:
	// Even if we don't want to handle these events, we have to register a "do-nothing" callback for them
	params.handleMouseClickFunc = dummy_mouse_handler;
	params.handleRightClickFunc = dummy_mouse_handler;
	params.handleMouseWheelFunc = dummy_wheel_handler;
	params.handleKeyFunc = dummy_key_handler;
	params.handleCursorFunc = dummy_cursor_status_handler;
	params.refcon = NULL;
	params.layer = xplm_WindowLayerFloatingWindows;
	// Opt-in to styling our window like an X-Plane 11 native window
	// If you're on XPLM300, not XPLM301, swap this enum for the literal value 1.
	params.decorateAsFloatingWindow = xplm_WindowDecorationRoundRectangle;
	
	// Set the window's initial bounds
	// Note that we're not guaranteed that the main monitor's lower left is at (0, 0)...
	// We'll need to query for the global desktop bounds!
	int left, bottom, right, top;
	XPLMGetScreenBoundsGlobal(&left, &top, &right, &bottom);
	params.left = left + 50;
	params.bottom = bottom + 150;
	params.right = params.left + 800;
	params.top = params.bottom + 200;
	
	g_window = XPLMCreateWindowEx(&params);

	maxlistitems = (params.top - params.bottom) / LISTBOX_ITEM_HEIGHT;
	
	// Position the window as a "free" floating window, which the user can drag around
	XPLMSetWindowPositioningMode(g_window, xplm_WindowPositionFree, -1);
	// Limit resizing our window: maintain a minimum width/height of 100 boxels and a max width/height of 300 boxels
	XPLMSetWindowResizingLimits(g_window, 800, 200, 900, 300);
	XPLMSetWindowTitle(g_window, "Sample Window");
	
	/**********************/
	/* BEG StandardWidget */

	//// left, RIGHT, TOP, bottom - the order of params 2 & 3 is switched vs. the arguments to XPCreateWidget.
	//// if this is a bug, then a fix will break things.
	//// also, the containerindex for the child window doesn't appear to work as one might think 
	//// (the index of the parent widget described in the first array item is 0, right?)
	//// or maybe the drawing is handled differently...
	//XPWidgetCreate_t widgetdefs[] = {
	//	{ 100, 300, 400, 100, 1, "Parent", 1, NO_PARENT, xpWidgetClass_MainWindow },
	//	{ 120, 280, 260, 240, 1, "Text", 0, 0, xpWidgetClass_TextField }
	//};

	//XPWidgetID widgetids[WIDGET_COUNT(widgetdefs)];

	//XPUCreateWidgets(widgetdefs, WIDGET_COUNT(widgetdefs), NULL, widgetids);
	//g_widget = widgetids[0];

	//g_widget = XPCreateWidget(100, 400, 300, 100, 1, "Parent", 1, 0, xpWidgetClass_MainWindow);
	//g_listWidget = XPCreateWidget(120, 380, 280, 120, 1, "Text", 0, g_widget, xpWidgetClass_SubWindow);
	//XPSetWidgetProperty(g_listWidget, xpProperty_SubWindowType, xpSubWindowStyle_Screen);

	///* END StandardWidget */
	///**********************/

	//XPLMRegisterFlightLoopCallback(FlightLoopCallback, 0.0, NULL);
	//XPLMSetFlightLoopCallbackInterval(FlightLoopCallback, 0.01, 1, NULL);

	// Have the sample listener receive events from the controller
	//controller.addListener(listener);

	return g_window != NULL;
}

PLUGIN_API void	XPluginStop(void)
{
	// Since we created the window, we'll be good citizens and clean it up
	XPLMDestroyWindow(g_window);
	g_window = NULL;

	//XPDestroyWidget(g_widget, 1);
	//g_widget = NULL;
	//XPDestroyWidget(g_listWidget, 1);
	//g_listWidget = NULL;

	//XPLMUnregisterFlightLoopCallback(FlightLoopCallback, NULL);

	// Remove the sample listener when done
	//controller.removeListener(listener);
}

PLUGIN_API void XPluginDisable(void) { }
PLUGIN_API int  XPluginEnable(void)  { return 1; }
PLUGIN_API void XPluginReceiveMessage(XPLMPluginID inFrom, int inMsg, void * inParam) { }

//#define IN_RECT(x, y, l, t, r, b)	\
//	(((x) >= (l)) && ((x) <= (r)) && ((y) >= (b)) && ((y) <= (t)))
//
//static unsigned int ItemNumber = 0;
//
//float  FlightLoopCallback(
//	float                inElapsedSinceLastCall,
//	float                inElapsedTimeSinceLastFlightLoop,
//	int                  inCounter,
//	void *               inRefcon)
//{
//
//	if (ItemNumber++ < 10)
//	{
//		// write something to the list widget - ???
//		int Left, Top, Right, Bottom, x, y, ListBoxDataOffset, ListBoxIndex;
//		XPGetWidgetGeometry(g_listWidget, &Left, &Top, &Right, &Bottom);
//
//		// Calculate the item rect in global coordinates.
//		int ItemTop = Top - (ItemNumber * LISTBOX_ITEM_HEIGHT);
//		int ItemBottom = Top - ((ItemNumber * LISTBOX_ITEM_HEIGHT) + LISTBOX_ITEM_HEIGHT);
//
//		float col_black[] = { 0.0, 0.0, 0.0 }; // red, green, blue
//
//		ostringstream stream;
//		stream << "Hello " << ItemNumber;
//
//		XPLMDrawString(col_black,
//			Left, ItemBottom + 2,
//			strdup(stream.str().c_str()), NULL, xplmFont_Proportional);
//	}
//
//
//	return 0.5; // next callback interval, seconds; see XPLMSetFlightLoopCallbackInterval
//}


void	draw_hello_world(XPLMWindowID in_window_id, void * in_refcon)
{
	// Mandatory: We *must* set the OpenGL state before drawing
	// (we can't make any assumptions about it)
	XPLMSetGraphicsState(
						 0 /* no fog */,
						 0 /* 0 texture units */,
						 0 /* no lighting */,
						 0 /* no alpha testing */,
						 1 /* do alpha blend */,
						 1 /* do depth testing */,
						 0 /* no depth writing */
						 );

	int l, t, r, b;
	XPLMGetWindowGeometry(in_window_id, &l, &t, &r, &b);
	float col_white[] = { 1.0, 1.0, 1.0 }; // red, green, blue
	
	if (ListItems.size() == maxlistitems)
	{
		ListItems.pop_back();
	}

	//ostringstream stream;
	//stream << "Hello " << indexflcb++;
	//ListItems.push_front(stream.str().c_str());
	//SampleListener listener;

	const Controller controller = SampleListener::m_controller();
	Frame oldframe = frame;
	frame = controller.frame();
	Leap::Vector move = frame.translation(oldframe);

	ListItems.push_front(SampleListener::ShowFrame(controller).str().c_str());
	l += (int)(move.x);
	r += (int)(move.x);
	t += (int)(move.y);
	b += (int)(move.y);

	XPLMSetWindowGeometry(in_window_id, l, t, r, b);

	unsigned int listitem = 0;
	for (auto const& item : ListItems)
	{
		int ItemBottom = t - ((listitem * LISTBOX_ITEM_HEIGHT) + LISTBOX_ITEM_HEIGHT);

		XPLMDrawString(col_white, l + 10, ItemBottom, const_cast<char*>(item.c_str()), NULL, xplmFont_Basic);
		listitem++;
	}

}

