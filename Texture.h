#pragma once

#ifdef WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

#if defined(__APPLE__) && defined(__MACH__)
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#endif

#include <string>
#include <vector>
#include <fstream>

using std::string;
using std::vector;
using std::ifstream;

struct TGA_Header
{
	GLubyte		ID_Length;
	GLubyte		ColorMapType;
	GLubyte		ImageType;
	GLubyte		ColorMapSpecifications[5];
	GLushort	xOrigin;
	GLushort	yOrigin;
	GLushort	ImageWidth;
	GLushort	ImageHeight;
	GLushort	PixelDepth;
};

// msdn bitmap code modified
#pragma pack(push)
#pragma pack(1)

struct Bitmap_File_Header {
	short	bfType;
	long	bfSize;
	short	bfReserved1;
	short	bfReserved2;
	long	bfOffBits;
};
struct Bitmap_Core_Header {
	GLuint		bcSize;
	GLuint		bcWidth;
	GLuint		bcHeight;
	GLushort	bcPlanes;
	GLushort	bcBitCount;
};

class Texture
{
public:
	Texture(string filename, string name = "");
	~Texture();
public:
	unsigned char* imageData;
	unsigned int bpp;
	unsigned int width;
	unsigned int height;
	unsigned int texID;
	string name;
	static vector<Texture*> textures;
private:
	bool loadTGA(string filename);
	bool loadBMP(string filename);
	bool createTexture(unsigned char* imageData, int width, int height, int type);
};
