// d3d_main.cpp
//

#include "d3d_local.h"

refimport_t ri;

/*
=============
R_Init
=============
*/
qboolean R_Init(void* hinstance, void* wndproc) {
	Vid_Init(hinstance, wndproc, 1024, 768);
	return true; 
}

/*
=============
R_Shutdown
=============
*/
void R_Shutdown(void) {
	
}

/*
=============
R_BeginRegistration
=============
*/
void R_BeginRegistration(char* map) {
	// Function body
}

/*
=============
R_EndRegistration
=============
*/
void R_EndRegistration(void) {
	// Function body
}

/*
=============
R_SetSky
=============
*/
void R_SetSky(char* name, float rotate, vec3_t axis) {
	// Function body
}

/*
===============
R_AppActivate
===============
*/
void R_AppActivate(qboolean activate) {
	// Function body
}

/*
=============
GetRefAPI
=============
*/
refexport_t GetRefAPI(refimport_t rimp)
{
	refexport_t	re;

	ri = rimp;

	re.api_version = API_VERSION;

	re.BeginRegistration = R_BeginRegistration;
	re.RegisterModel = R_RegisterModel;
	re.RegisterSkin = R_RegisterSkin;
	re.RegisterPic = R_RegisterPic;
	re.SetSky = R_SetSky;
	re.EndRegistration = R_EndRegistration;

	re.RenderFrame = R_RenderFrame;

	re.DrawGetPicSize = R_DrawGetPicSize;
	re.DrawPic = R_DrawPic;
	re.DrawStretchPic = R_DrawStretchPic;
	re.DrawChar = R_DrawChar;
	re.DrawTileClear = R_DrawTileClear;
	re.DrawFill = R_DrawFill;
	re.DrawFadeScreen = R_DrawFadeScreen;

	re.DrawStretchRaw = R_DrawStretchRaw;

	re.Init = R_Init;
	re.Shutdown = R_Shutdown;

	re.CinematicSetPalette = R_CinematicSetPalette;
	re.BeginFrame = R_BeginFrame;
	re.EndFrame = R_EndFrame;

	re.AppActivate = R_AppActivate;

	return re;
}