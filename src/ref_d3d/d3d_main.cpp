// d3d_main.cpp
//

#include "d3d_local.h"

idRenderSystemLocal tr;

refimport_t ri;
const uint32_t      tr_image_count = 3;

/*
=============
R_Init
=============
*/
qboolean R_Init(void* hinstance, void* wndproc) {
	Vid_Init(hinstance, wndproc, 1024, 768);
	tr.Init(1024, 768, (HINSTANCE)hinstance, sww_state.hWnd);
	return true; 
}

/*
=============
renderer_log
=============
*/
void renderer_log(tr_log_type type, const char* msg, const char* component)
{
	char buffer[1024]; // Adjust size as needed

	switch (type) {
	case tr_log_type_info:
		sprintf_s(buffer, sizeof(buffer), "[INFO][%s] : %s", component, msg);
		OutputDebugStringA(buffer);
		break;
	case tr_log_type_warn:
		sprintf_s(buffer, sizeof(buffer), "[WARN][%s] : %s", component, msg);
		OutputDebugStringA(buffer);
		break;
	case tr_log_type_debug:
		sprintf_s(buffer, sizeof(buffer), "[DEBUG][%s] : %s", component, msg);
		OutputDebugStringA(buffer);
		break;
	case tr_log_type_error:
		sprintf_s(buffer, sizeof(buffer), "[ERROR][%s] : %s", component, msg);
		OutputDebugStringA(buffer);
		break;
	default:
		break;
	}
}

/*
=============
idRenderSystemLocal::Init
=============
*/
void idRenderSystemLocal::Init(int width, int height, HINSTANCE hinst, HWND hwnd) {
	tr_renderer_settings settings = {};

	s_window_width = width;
	s_window_height = height;

	settings.handle.hinstance = hinst;
	settings.handle.hwnd = hwnd;
	settings.width = s_window_width;
	settings.height = s_window_height;
	settings.swapchain.image_count = tr_image_count;
	settings.swapchain.sample_count = tr_sample_count_8;
	settings.swapchain.color_format = tr_format_b8g8r8a8_unorm;
	settings.swapchain.depth_stencil_format = tr_format_d32_float;
	settings.swapchain.depth_stencil_clear_value.depth = 1.0f;
	settings.swapchain.depth_stencil_clear_value.stencil = 255;
	settings.log_fn = renderer_log;

	tr_create_renderer(GAME_NAME, &settings, &m_renderer);
	tr_create_cmd_pool(m_renderer, m_renderer->graphics_queue, false, &m_cmd_pool);
	tr_create_cmd_n(m_cmd_pool, false, tr_image_count, &m_cmds);

}

/*
=============
R_Shutdown
=============
*/
void R_Shutdown(void) {
	tr.Shutdown();
}

/*
=============
idRenderSystemLocal::Shutdown
=============
*/
void idRenderSystemLocal::Shutdown(void) {
	tr_destroy_renderer(m_renderer);
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