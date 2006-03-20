#ifndef FREEFONT_HPP
#define FREEFONT_HPP

//FreeType Headers
#include <ft2build.h>
#include <freetype/freetype.h>
#include <freetype/ftglyph.h>
#include <freetype/ftoutln.h>
#include <freetype/fttrigon.h>

//Using the STL exception library increases the
//chances that someone else using our code will corretly
//catch any exceptions that we throw.
#include <stdexcept>

#include "texture.hpp"

using namespace std;

//This holds all of the information related to any
//freetype font that we want to create.  
class Font_Data:
	public Object
{
	public:
		Font_Data (string filename, unsigned int h);
		virtual ~Font_Data ();
		void print (float x, float y, string fmt, ...);
	
	private:
		unsigned int height;	//	Holds the height of the font.
		GLuint * textures;		//	Holds the texture id's
		GLuint list_base;		//	Holds the first display list id
};

#endif
