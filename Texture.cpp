#include "Texture.h"
#include <wx/wx.h>

vector<Texture*> Texture::textures;

Texture::Texture(string in_filename, string in_name)
{
	loadBMP(in_filename);
	imageData = nullptr;
	name = in_name;
	textures.push_back(this);
}

Texture::~Texture()
{
	for (vector<Texture*>::iterator it = textures.begin(); it != textures.end(); it++)
	{
		if ((*it) == this)
		{
			textures.erase(it);
			break;
		}
	}
}

bool Texture::loadTGA(string filename)
{
	TGA_Header header;

	ifstream file(filename.data(), std::ios_base::binary);

	if (!file.is_open())
	{
		wxMessageBox(wxString(_T("Could not open file.")), wxString(_T("TGA Texture Loading Error")));
		return false;
	}

	if (!file.read((char*)&header, sizeof(TGA_Header)))
	{
		wxMessageBox(wxString(_T("Could not read in the header.")), wxString(_T("TGA Texture Loading Error")));
		return false;
	}

	if (header.ImageType != 2)
	{
		wxMessageBox(wxString(_T("The file type is not supported.")), wxString(_T("TGA Texture Loading Error")));
		return false;
	}

	width = header.ImageWidth;
	height = header.ImageHeight;
	bpp = header.PixelDepth;

	if (width <= 0 || height <= 0 || (bpp != 24 && bpp != 32))
	{
		wxMessageBox(wxString(_T("The file type is not supported.")), wxString(_T("TGA Texture Loading Error")));
		return false;
	}

	GLuint type = GL_RGBA;

	if (bpp == 24)
	{
		type = GL_RGB;
	}

	GLuint bytesPerPixel = bpp / 8;
	GLuint imageSize = width * height * bytesPerPixel;

	imageData = new GLubyte[imageSize];

	if (imageData == nullptr)
	{
		wxMessageBox(wxString(_T("Could not allocate the memory for the raw image data.")), wxString(_T("TGA Texture Loading Error")));
		return false;
	}

	if (!file.read((char*)imageData, imageSize))
	{
		wxMessageBox(wxString(_T("Could not read in the image data.")), wxString(_T("TGA Texture Loading Error")));
		delete[] imageData;
	}

	file.close();

	// bgr to rgb
	for (GLuint i = 0; i < imageSize; i += bytesPerPixel)
	{
		GLuint temp = imageData[i];
		imageData[i] = imageData[i + 2];
		imageData[i + 2] = temp;
	}

	createTexture(imageData, width, height, type);
	return true;
}

bool Texture::loadBMP(string filename)
{
	Bitmap_File_Header BF_Header;

	ifstream file(filename.data(), std::ios_base::binary);

	if (!file.is_open())
	{
		wxMessageBox(wxString(_T("Could not open file.")), wxString(_T("BMP Texture Loading Error")));
		return false;
	}

	int bfhs = sizeof(Bitmap_File_Header);
	if (!file.read((char*)&BF_Header, 14))
	{
		wxMessageBox(wxString(_T("Could not read in the header.")), wxString(_T("BMP Texture Loading Error")));
		return false;
	}

	Bitmap_Core_Header BC_Header;

	if (!file.read((char*)&BC_Header, sizeof(Bitmap_Core_Header)))
	{
		wxMessageBox(wxString(_T("Could not read in the header.")), wxString(_T("BMP Texture Loading Error")));
		return false;
	}

	if (BF_Header.bfType != 'BM' && BF_Header.bfType != 'MB')
	{
		wxMessageBox(wxString(_T("The file type is not supported.")), wxString(_T("BMP Texture Loading Error")));
		return false;
	}

	width = BC_Header.bcWidth;
	height = BC_Header.bcHeight;
	bpp = BC_Header.bcBitCount;

	if (width <= 0 || height <= 0 || bpp != 24)
	{
		wxMessageBox(wxString(_T("The file type is not supported.")), wxString(_T("BMP Texture Loading Error")));
		return false;
	}

	GLuint type = GL_RGB;

	GLuint bytesPerPixel = bpp / 8;
	GLuint imageSize = width * height * bytesPerPixel;

	imageData = new GLubyte[imageSize];

	if (imageData == nullptr)
	{
		wxMessageBox(wxString(_T("Could not allocate the memory for the raw image data.")), wxString(_T("BMP Texture Loading Error")));
		return false;
	}

	if (!file.seekg(BF_Header.bfOffBits, std::ios_base::beg))
	{
		wxMessageBox(wxString(_T("Could not seek the image data in the file.")), wxString(_T("BMP Texture Loading Error")));
		delete[] imageData;
	}

	if (!file.read((char*)imageData, imageSize))
	{
		wxMessageBox(wxString(_T("Could not read in the image data.")), wxString(_T("BMP Texture Loading Error")));
		delete[] imageData;
	}

	// bgr to rgb
	for (GLuint i = 0; i < imageSize; i += bytesPerPixel)
	{
		GLuint temp = imageData[i];
		imageData[i] = imageData[i + 2];
		imageData[i + 2] = temp;
	}

	file.close();

	createTexture(imageData, width, height, type);
	return true;
}

bool Texture::createTexture(unsigned char* imageData, int width, int height, int type)
{
	glGenTextures(1, &texID);
	glBindTexture(GL_TEXTURE_2D, texID);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, type, width, height, 0, type, GL_UNSIGNED_BYTE, imageData);
	return true;
}