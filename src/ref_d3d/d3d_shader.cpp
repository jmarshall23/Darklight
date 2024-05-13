// d3d_shader.cpp
//

#include "d3d_local.h"

/*
====================
idRenderSystemLocal::LoadShader
====================
*/
tr_shader_program* idRenderSystemLocal::LoadShader(const char* shaderPath) {
	char* buffer;
	int bufferLen;
	tr_shader_program* shader = nullptr;

	bufferLen = ri.FS_LoadFile((char*)shaderPath, (void**)&buffer);
	if (bufferLen <= 0) {
		ri.Sys_Error(ERR_FATAL, "Failed to load shader %s\n", shaderPath);
		return nullptr;
	}

	tr_create_shader_program(m_renderer, bufferLen, buffer, "VSMain", bufferLen, buffer, "PSMain", &shader);
	if (shader == nullptr) {
		ri.Sys_Error(ERR_FATAL, "Failed to create shader %s\n", shaderPath);
		return nullptr;
	}

	ri.FS_FreeFile(buffer);

	return shader;
}