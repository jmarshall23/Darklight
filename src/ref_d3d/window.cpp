#include "d3d_local.h"
#include <windows.h>


#define	WINDOW_STYLE	(WS_OVERLAPPED|WS_BORDER|WS_CAPTION|WS_VISIBLE)

swwstate_t sww_state;

/*
** VID_CreateWindow
*/
void VID_CreateWindow(int width, int height)
{
	WNDCLASS		wc;
	RECT			r;
	cvar_t* vid_xpos, * vid_ypos, * vid_fullscreen;
	int				x, y, w, h;
	int				exstyle, stylebits;

	vid_xpos = ri.Cvar_Get("vid_xpos", "0", 0);
	vid_ypos = ri.Cvar_Get("vid_ypos", "0", 0);
	vid_fullscreen = ri.Cvar_Get("vid_fullscreen", "0", CVAR_ARCHIVE);

	if (vid_fullscreen->value)
	{
		exstyle = WS_EX_TOPMOST;
		stylebits = WS_POPUP | WS_VISIBLE;
	}
	else
	{
		exstyle = 0;
		stylebits = WINDOW_STYLE;
	}

	/* Register the frame class */
	wc.style = 0;
	wc.lpfnWndProc = (WNDPROC)sww_state.wndproc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = sww_state.hInstance;
	wc.hIcon = 0;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)COLOR_GRAYTEXT;
	wc.lpszMenuName = 0;
	wc.lpszClassName = GAME_NAME;

	if (!RegisterClass(&wc))
		ri.Sys_Error(ERR_FATAL, "Couldn't register window class");

	r.left = 0;
	r.top = 0;
	r.right = width;
	r.bottom = height;

	AdjustWindowRect(&r, stylebits, FALSE);

	w = r.right - r.left;
	h = r.bottom - r.top;
	x = vid_xpos->value;
	y = vid_ypos->value;

	sww_state.hWnd = CreateWindowEx(
		exstyle,
		GAME_NAME,
		GAME_NAME,
		stylebits,
		x, y, w, h,
		NULL,
		NULL,
		sww_state.hInstance,
		NULL);

	if (!sww_state.hWnd)
		ri.Sys_Error(ERR_FATAL, "Couldn't create window");

	ShowWindow(sww_state.hWnd, SW_SHOWNORMAL);
	UpdateWindow(sww_state.hWnd);
	SetForegroundWindow(sww_state.hWnd);
	SetFocus(sww_state.hWnd);

	// let the sound and input subsystems know about the new window
	ri.Vid_NewWindow(width, height);
}

/*
** SWimp_Init
**
** This routine is responsible for initializing the implementation
** specific stuff in a software rendering subsystem.
*/
qboolean Vid_Init(void* hInstance, void* wndProc, int width, int height)
{
	sww_state.hInstance = (HINSTANCE)hInstance;
	sww_state.wndproc = wndProc;
	VID_CreateWindow(width, height);
	return true;
}