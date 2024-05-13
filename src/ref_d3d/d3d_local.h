// d3d_local.h
//

#include "../client/ref.h"
#include "tinydx.h"

#include <string>
#include <vector>

struct Matrix4x4 {
	float m[4][4];

	// Set to identity matrix
	void LoadIdentity() {
		for (int i = 0; i < 4; ++i) {
			for (int j = 0; j < 4; ++j) {
				m[i][j] = (i == j) ? 1.0f : 0.0f;
			}
		}
	}

	// Translate by (x, y, z)
	void Translate(float x, float y, float z) {
		LoadIdentity();
		m[3][0] = x;
		m[3][1] = y;
		m[3][2] = z;
	}

	// Rotate around the Z-axis by angle degrees
	void RotateZ(float angle) {
		LoadIdentity();
		float radians = angle * 3.14159265358979323846f / 180.0f;  // Convert to radians
		float c = cosf(radians);
		float s = sinf(radians);

		m[0][0] = c;
		m[0][1] = -s;
		m[1][0] = s;
		m[1][1] = c;
	}
};

struct MatrixBuffer {
	Matrix4x4 modelViewMatrix;
	Matrix4x4 projectionMatrix;	
};

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

typedef enum
{
	it_none,
	it_skin,
	it_sprite,
	it_wall,
	it_pic,
	it_sky
} imagetype_t;

class idImage
{
public:
	idImage(const char *name, tr_renderer* renderer)
	{
		m_texture = nullptr;
		type = it_none;
		width = -1;
		height = -1;
		sl = -1;
		tl = -1;
		sh = -1;
		th = -1;
		this->name = name;
		this->renderer = renderer;
	}

	~idImage()
	{
		if (m_texture)
		{
			tr_destroy_texture(renderer, m_texture);
		}
	}

	void Upload32(byte* buffer, int width, int height);

	const char* GetName() {
		return name.c_str();
	}
protected:
	std::string name;
	imagetype_t	type;
	int		width, height;				// source image
	float	sl, tl, sh, th;				// 0,0 - 1,1 unless part of the scrap
	tr_texture* m_texture;
	tr_renderer* renderer;	
};

//
// idRenderSystemLocal
//
class idRenderSystemLocal {
public:
	void			Init(int width, int height, HINSTANCE hinst, HWND hwnd);
	void			Shutdown(void);

	idImage*		FindImage(const char* path);
	void			InitPalette(void);

	tr_shader_program* LoadShader(const char* shaderPath);
public:
	tr_renderer* m_renderer = nullptr;
	tr_descriptor_set* m_desc_set = nullptr;
	tr_cmd_pool* m_cmd_pool = nullptr;
	tr_cmd** m_cmds = nullptr;

	tr_cmd* cmd = nullptr;

	tr_shader_program* diffuseOnlyShader;

	int s_window_width;
	int s_window_height;

	MatrixBuffer mb;

	std::vector<idImage*> images;
	unsigned int d_8to24table[256];

};

//
// Matrix helper code.
//
Matrix4x4 glOrtho(float left, float right, float bottom, float top, float nearVal, float farVal);
void glTranslatef(Matrix4x4& matrix, float x, float y, float z);
void glLoadIdentity(Matrix4x4& matrix);
void glRotatef(Matrix4x4& matrix, float angle, float x, float y, float z);

void R_SetGL2D(void);

extern idRenderSystemLocal tr;