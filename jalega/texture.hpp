#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#ifdef WIN32
#include <windows.h>
#endif
#include <GL/glut.h>

#include "object.hpp"

using namespace std;

class Texture:
	public Object
{
	public:
		Texture (string filename, int new_id);		//	TGA texture filename
		virtual ~Texture ();
		virtual bool is_initialized () const;
		void load_TGA (string filename);
		void build_font () const;
		int get_id () const;

	private:
		GLubyte	* imageData;			// Image Data (Up To 32 Bits)
		GLuint	bpp;					// Image Color Depth In Bits Per Pixel
		GLuint	width;					// Image Width
		GLuint	height;					// Image Height
		GLuint	id;						// Texture ID Used To Select A Texture
};

#endif
