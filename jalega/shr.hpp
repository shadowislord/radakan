// shr.hpp: interface for the SHR class.
//
//////////////////////////////////////////////////////////////////////

//	What should be linked to avoind OpenGL linking errors? - Tinus

#ifndef SHR_HPP
#define SHR_HPP

#include "vector_3d.hpp"
#include "texture.hpp"

class SHR:
	public Object
{
	public:
		SHR();												//	Constructor
		virtual ~SHR();										//	Destructor
		void initGL();
		void texturing2D (bool flag);
		Texture loadTexture (string location);
		void bindTexture (int textureid);
		void renderQuad (int textureid, float xpos, float ypos, float width,
			float height, float zdepth, float rotation, float tx, float ty);
		void renderPointSprite (int textureid, float xpos,
			float ypos, int size);
		void enable2D (int xscale, int yscale);
		void disable2D ();
};

#endif	//	SHR_HPP
