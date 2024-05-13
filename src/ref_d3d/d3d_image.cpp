// d3d_image.cpp
//

#include "d3d_local.h"


/*
=================================================================

PCX LOADING

=================================================================
*/


/*
==============
LoadPCX
==============
*/
void LoadPCX(char* filename, byte** pic, byte** palette, int* width, int* height)
{
	byte* raw;
	pcx_t* pcx;
	int		x, y;
	int		len;
	int		dataByte, runLength;
	byte* out, * pix;

	*pic = NULL;
	*palette = NULL;

	//
	// load the file
	//
	len = ri.FS_LoadFile(filename, (void**)&raw);
	if (!raw)
	{
		ri.Con_Printf(PRINT_DEVELOPER, "Bad pcx file %s\n", filename);
		return;
	}

	//
	// parse the PCX file
	//
	pcx = (pcx_t*)raw;

	pcx->xmin = (pcx->xmin);
	pcx->ymin = (pcx->ymin);
	pcx->xmax = (pcx->xmax);
	pcx->ymax = (pcx->ymax);
	pcx->hres = (pcx->hres);
	pcx->vres = (pcx->vres);
	pcx->bytes_per_line = (pcx->bytes_per_line);
	pcx->palette_type = (pcx->palette_type);

	raw = &pcx->data;

	if (pcx->manufacturer != 0x0a
		|| pcx->version != 5
		|| pcx->encoding != 1
		|| pcx->bits_per_pixel != 8
		|| pcx->xmax >= 640
		|| pcx->ymax >= 480)
	{
		ri.Con_Printf(PRINT_ALL, "Bad pcx file %s\n", filename);
		return;
	}

	out = (byte *)malloc((pcx->ymax + 1) * (pcx->xmax + 1));

	*pic = out;

	pix = out;

	if (palette)
	{
		*palette = (byte*)malloc(768);
		memcpy(*palette, (byte*)pcx + len - 768, 768);
	}

	if (width)
		*width = pcx->xmax + 1;
	if (height)
		*height = pcx->ymax + 1;

	for (y = 0; y <= pcx->ymax; y++, pix += pcx->xmax + 1)
	{
		for (x = 0; x <= pcx->xmax; )
		{
			dataByte = *raw++;

			if ((dataByte & 0xC0) == 0xC0)
			{
				runLength = dataByte & 0x3F;
				dataByte = *raw++;
			}
			else
				runLength = 1;

			while (runLength-- > 0)
				pix[x++] = dataByte;
		}

	}

	if (raw - (byte*)pcx > len)
	{
		ri.Con_Printf(PRINT_DEVELOPER, "PCX file %s was malformed", filename);
		free(*pic);
		*pic = NULL;
	}

	ri.FS_FreeFile(pcx);
}

/*
=========================================================

TARGA LOADING

=========================================================
*/

typedef struct _TargaHeader {
	unsigned char 	id_length, colormap_type, image_type;
	unsigned short	colormap_index, colormap_length;
	unsigned char	colormap_size;
	unsigned short	x_origin, y_origin, width, height;
	unsigned char	pixel_size, attributes;
} TargaHeader;


/*
=============
LoadTGA
=============
*/
void LoadTGA(char* name, byte** pic, int* width, int* height)
{
	int		columns, rows, numPixels;
	byte* pixbuf;
	int		row, column;
	byte* buf_p;
	byte* buffer;
	int		length;
	TargaHeader		targa_header;
	byte* targa_rgba;
	byte tmp[2];

	*pic = NULL;

	//
	// load the file
	//
	length = ri.FS_LoadFile(name, (void**)&buffer);
	if (!buffer)
	{
		ri.Con_Printf(PRINT_DEVELOPER, "Bad tga file %s\n", name);
		return;
	}

	buf_p = buffer;

	targa_header.id_length = *buf_p++;
	targa_header.colormap_type = *buf_p++;
	targa_header.image_type = *buf_p++;

	tmp[0] = buf_p[0];
	tmp[1] = buf_p[1];
	targa_header.colormap_index = (*((short*)tmp));
	buf_p += 2;
	tmp[0] = buf_p[0];
	tmp[1] = buf_p[1];
	targa_header.colormap_length = (*((short*)tmp));
	buf_p += 2;
	targa_header.colormap_size = *buf_p++;
	targa_header.x_origin = (*((short*)buf_p));
	buf_p += 2;
	targa_header.y_origin = (*((short*)buf_p));
	buf_p += 2;
	targa_header.width = (*((short*)buf_p));
	buf_p += 2;
	targa_header.height = (*((short*)buf_p));
	buf_p += 2;
	targa_header.pixel_size = *buf_p++;
	targa_header.attributes = *buf_p++;

	if (targa_header.image_type != 2
		&& targa_header.image_type != 10)
		ri.Sys_Error(ERR_DROP, "LoadTGA: Only type 2 and 10 targa RGB images supported\n");

	if (targa_header.colormap_type != 0
		|| (targa_header.pixel_size != 32 && targa_header.pixel_size != 24))
		ri.Sys_Error(ERR_DROP, "LoadTGA: Only 32 or 24 bit images supported (no colormaps)\n");

	columns = targa_header.width;
	rows = targa_header.height;
	numPixels = columns * rows;

	if (width)
		*width = columns;
	if (height)
		*height = rows;

	targa_rgba = (byte*)malloc(numPixels * 4);
	*pic = targa_rgba;

	if (targa_header.id_length != 0)
		buf_p += targa_header.id_length;  // skip TARGA image comment

	if (targa_header.image_type == 2) {  // Uncompressed, RGB images
		for (row = rows - 1; row >= 0; row--) {
			pixbuf = targa_rgba + row * columns * 4;
			for (column = 0; column < columns; column++) {
				unsigned char red, green, blue, alphabyte;
				switch (targa_header.pixel_size) {
				case 24:

					blue = *buf_p++;
					green = *buf_p++;
					red = *buf_p++;
					*pixbuf++ = red;
					*pixbuf++ = green;
					*pixbuf++ = blue;
					*pixbuf++ = 255;
					break;
				case 32:
					blue = *buf_p++;
					green = *buf_p++;
					red = *buf_p++;
					alphabyte = *buf_p++;
					*pixbuf++ = red;
					*pixbuf++ = green;
					*pixbuf++ = blue;
					*pixbuf++ = alphabyte;
					break;
				}
			}
		}
	}
	else if (targa_header.image_type == 10) {   // Runlength encoded RGB images
		unsigned char red, green, blue, alphabyte, packetHeader, packetSize, j;
		for (row = rows - 1; row >= 0; row--) {
			pixbuf = targa_rgba + row * columns * 4;
			for (column = 0; column < columns; ) {
				packetHeader = *buf_p++;
				packetSize = 1 + (packetHeader & 0x7f);
				if (packetHeader & 0x80) {        // run-length packet
					switch (targa_header.pixel_size) {
					case 24:
						blue = *buf_p++;
						green = *buf_p++;
						red = *buf_p++;
						alphabyte = 255;
						break;
					case 32:
						blue = *buf_p++;
						green = *buf_p++;
						red = *buf_p++;
						alphabyte = *buf_p++;
						break;
					}

					for (j = 0; j < packetSize; j++) {
						*pixbuf++ = red;
						*pixbuf++ = green;
						*pixbuf++ = blue;
						*pixbuf++ = alphabyte;
						column++;
						if (column == columns) { // run spans across rows
							column = 0;
							if (row > 0)
								row--;
							else
								goto breakOut;
							pixbuf = targa_rgba + row * columns * 4;
						}
					}
				}
				else {                            // non run-length packet
					for (j = 0; j < packetSize; j++) {
						switch (targa_header.pixel_size) {
						case 24:
							blue = *buf_p++;
							green = *buf_p++;
							red = *buf_p++;
							*pixbuf++ = red;
							*pixbuf++ = green;
							*pixbuf++ = blue;
							*pixbuf++ = 255;
							break;
						case 32:
							blue = *buf_p++;
							green = *buf_p++;
							red = *buf_p++;
							alphabyte = *buf_p++;
							*pixbuf++ = red;
							*pixbuf++ = green;
							*pixbuf++ = blue;
							*pixbuf++ = alphabyte;
							break;
						}
						column++;
						if (column == columns) { // pixel packet run spans across rows
							column = 0;
							if (row > 0)
								row--;
							else
								goto breakOut;
							pixbuf = targa_rgba + row * columns * 4;
						}
					}
				}
			}
		breakOut:;
		}
	}

	ri.FS_FreeFile(buffer);
}


/*
=========
LoadWal
=========
*/
bool LoadWal(const char* name, byte** pic, int* width, int* height)
{
	miptex_t* mt;

	ri.FS_LoadFile((char *)name, (void**)&mt);
	if (!mt)
	{
		ri.Con_Printf(PRINT_ALL, "GL_FindImage: can't load %s\n", name);
		return false;
	}

	int s = mt->width * mt->height;

	*width = (mt->width);
	*height = (mt->height);
	int ofs = (mt->offsets[0]);

	byte* data = (byte*)mt + ofs;

	*pic = (byte *)malloc(mt->width * mt->height * 4);

	for (int i = 0; i < mt->width * mt->height; i++)
	{
		byte p = data[i];

		*pic[(i * 4) + 0] = ((byte*)&tr.d_8to24table[p])[0];
		*pic[(i * 4) + 1] = ((byte*)&tr.d_8to24table[p])[1];
		*pic[(i * 4) + 2] = ((byte*)&tr.d_8to24table[p])[2];
		*pic[(i * 4) + 3] = 255;
	}
}

/*
===============
idRenderSystemLocal::InitPalette
===============
*/
void idRenderSystemLocal::InitPalette(void)
{
	int		i;
	int		r, g, b;
	unsigned	v;
	byte* pic, * pal;
	int		width, height;

	// get the palette

	LoadPCX("pics/colormap.pcx", &pic, &pal, &width, &height);
	if (!pal)
		ri.Sys_Error(ERR_FATAL, "Couldn't load pics/colormap.pcx");

	for (i = 0; i < 256; i++)
	{
		r = pal[i * 3 + 0];
		g = pal[i * 3 + 1];
		b = pal[i * 3 + 2];

		v = (255 << 24) + (r << 0) + (g << 8) + (b << 16);
		d_8to24table[i] = (v);
	}

	d_8to24table[255] &= (0xffffff);	// 255 is transparent

	free(pic);
	free(pal);
}

void idImage::Upload32(byte* buffer, int width, int height) {
	if (m_texture != nullptr)
	{
		if (this->width != width || this->height != height)
		{
			tr_destroy_texture(renderer, m_texture);
			m_texture = nullptr;
		}
	}

	this->width = width;
	this->height = height;
	sl = 0;
	sh = 1;
	tl = 0;
	th = 1;

	if (m_texture == nullptr)
	{
		tr_create_texture_2d(renderer, width, height, tr_sample_count_1, tr_format_r8g8b8a8_unorm, tr_max_mip_levels, NULL, false, tr_texture_usage_sampled_image, &m_texture);
	}
	int image_row_stride = width * 4;
	tr_util_update_texture_uint8(renderer->graphics_queue, width, height, image_row_stride, buffer, 4, m_texture, NULL, NULL);
}

idImage* idRenderSystemLocal::FindImage(const char* path) {
	byte* data = nullptr;
	int width;
	int height;

	if (!path)
		return NULL;	//	ri.Sys_Error (ERR_DROP, "GL_FindImage: NULL name");
	int len = strlen(path);
	if (len < 5)
		return NULL;	//	ri.Sys_Error (ERR_DROP, "GL_FindImage: bad name: %s", name);

	for (int i = 0; i < images.size(); i++)
	{
		if (!strcmp(images[i]->GetName(), path))
			return images[i];
	}

	if (!strcmp(path + len - 4, ".wal")) {
		if (!LoadWal(path, &data, &width, &height)) {
			return nullptr;
		}
	}
	else if (!strcmp(path + len - 4, ".pcx")) {
		byte* palette;
		byte* tempbuffer;

		LoadPCX((char *)path, &tempbuffer, &palette, &width, &height);

		if (tempbuffer == nullptr) {
			return nullptr;
		}

		data = (byte *)malloc(width * height * 4);
		for (int i = 0; i < width * height; i++) {
			byte p = tempbuffer[i];

			data[(i * 4) + 0] = ((byte*)&tr.d_8to24table[p])[0];
			data[(i * 4) + 1] = ((byte*)&tr.d_8to24table[p])[1];
			data[(i * 4) + 2] = ((byte*)&tr.d_8to24table[p])[2];
			data[(i * 4) + 3] = 255;
		}

		free(tempbuffer);
		free(palette);
	}
	else if (!strcmp(path + len - 4, ".tga")) {
		LoadTGA((char *)path, &data, &width, &height);
	}
	else {
		// Assume PCX.
		char temp[512];
		sprintf(temp, "pics/%s.pcx", path);

		// I hate having this code duplicated. 
		{
			byte* palette;
			byte* tempbuffer;

			LoadPCX(temp, &tempbuffer, &palette, &width, &height);

			if (tempbuffer == nullptr) {
				return nullptr;
			}

			data = (byte*)malloc(width * height * 4);
			for (int i = 0; i < width * height; i++) {
				byte p = tempbuffer[i];

				data[(i * 4) + 0] = ((byte*)&tr.d_8to24table[p])[0];
				data[(i * 4) + 1] = ((byte*)&tr.d_8to24table[p])[1];
				data[(i * 4) + 2] = ((byte*)&tr.d_8to24table[p])[2];
				data[(i * 4) + 3] = 255;
			}

			free(tempbuffer);
			free(palette);
		}
	}

	if (data == nullptr) {
		return nullptr;
	}

	idImage* image = new idImage(path, tr.m_renderer);
	image->Upload32(data, width, height);
	images.push_back(image);

	free(data);
	return image;
}

struct image_s* R_RegisterSkin(char* name) {
	return (struct image_s*)tr.FindImage(name);
}

struct image_s* R_RegisterPic(char* name) {
	return (struct image_s*)tr.FindImage(name);
}