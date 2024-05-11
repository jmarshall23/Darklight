// d3d_local.h
//

#include "../client/ref.h"

extern refimport_t ri;

// Export functions
qboolean R_Init(void* hinstance, void* wndproc);
void R_Shutdown(void);

void R_BeginRegistration(char* map);
struct model_s* R_RegisterModel(char* name);
struct image_s* R_RegisterSkin(char* name);
struct image_s* R_RegisterPic(char* name);
void R_SetSky(char* name, float rotate, vec3_t axis);
void R_EndRegistration(void);

void R_RenderFrame(refdef_t* fd);

void R_DrawGetPicSize(int* w, int* h, char* name);
void R_DrawPic(int x, int y, char* name);
void R_DrawStretchPic(int x, int y, int w, int h, char* name);
void R_DrawChar(int x, int y, int c);
void R_DrawTileClear(int x, int y, int w, int h, char* name);
void R_DrawFill(int x, int y, int w, int h, int c);
void R_DrawFadeScreen(void);

void R_DrawStretchRaw(int x, int y, int w, int h, int cols, int rows, byte* data);

void R_CinematicSetPalette(const unsigned char* palette);
void R_BeginFrame(float camera_separation);
void R_EndFrame(void);

void R_AppActivate(qboolean activate);

qboolean Vid_Init(void* hInstance, void* wndProc, int width, int height);

typedef struct
{
	HINSTANCE		hInstance;
	void* wndproc;
	HDC				hDC;				// global DC we're using
	HWND			hWnd;				// HWND of parent window

	qboolean		palettized;			// true if desktop is paletted
	qboolean		modex;

	qboolean		initializing;
} swwstate_t;

extern swwstate_t sww_state;