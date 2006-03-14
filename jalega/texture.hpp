#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include <GL/gl.h>	//	Header for OpenGL32 library
#include <GL/glu.h>
//	#include <GL/glew.h>
//	#include <GL/wglew.h>
//	#include <GL/glaux.h>	//	opengl auxilliary library, has some usefull functions
//	#include <gl/glpng.h>	//	using tga instead
#include "object.hpp"

using namespace std;

class Texture:
	public Object	//	should be changed to GameObject once it's ready
{
	public:
		//	Constructor
		Texture ();
		//	Destructor
		virtual ~Texture ();
		virtual bool is_initialized () const;
		GLubyte	* imageData;	// Image Data (Up To 32 Bits)
		GLuint	bpp;			// Image Color Depth In Bits Per Pixel
		GLuint	width;			// Image Width
		GLuint	height;			// Image Height
		GLuint	texID;			// Texture ID Used To Select A Texture
		GLuint	type;			// Image Type (GL_RGB, GL_RGBA)
};

#endif
