#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#ifdef WIN32
#include <windows.h>
#endif
#include <GL/glut.h>

#include "object.hpp"

using namespace std;

typedef struct												// Create A Structure
{
	GLubyte	*imageData;										// Image Data (Up To 32 Bits)
	GLuint	bpp;											// Image Color Depth In Bits Per Pixel.
	GLuint	width;											// Image Width
	GLuint	height;											// Image Height
	GLuint	texID;											// Texture ID Used To Select A Texture
} Texture;												// Structure Name

#endif
