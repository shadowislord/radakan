#ifndef TGA_LOADER_HPP
#define TGA_LOADER_HPP

#include "texture.hpp"

typedef struct
{
	GLubyte		header[6];								// First 6 Useful Bytes From The Header
	GLuint		bytesPerPixel;							// Holds Number Of Bytes Per Pixel Used In The TGA File
	GLuint		imageSize;								// Used To Store The Image Size When Setting Aside Ram
	GLuint		temp;									// Temporary Variable
	GLuint		type;	
	GLuint		Height;									// Height of Image
	GLuint		Width;									// Width ofImage
	GLuint		Bpp;									// Bits Per Pixel
} TGA;


bool LoadTGA (Texture * texture, string filename);						// Load a TGA file

#endif
