// shr.hpp: interface for the SHR class.
//
//////////////////////////////////////////////////////////////////////

#ifndef SHR_HPP
#define SHR_HPP

#include "tga_loader.hpp"
#include "vector_3d.hpp"

class SHR:
	public Object
{
	public:
		SHR ();												//	constructor
		virtual ~SHR ();									//	destructor
		void initGL ();
		void draw ();
		void texturing2D (bool flag);
		Texture loadTexture (string location);
		void bindTexture (int textureid);
		void renderQuad (int textureid, float xpos, float ypos, float width,
			float height, float zdepth, float rotation, float tx, float ty);
		void renderPointSprite (int textureid, float xpos,
			float ypos, int size);
		void enable2D (int xscale, int yscale);
		void disable2D ();
		void glPrint (int x, int y, unsigned int set, string fmt);

//	temporary public
		void build_font (Texture * texture);
		Texture * font_texture;
	private:
		vector <string> text;
};

#endif	//	SHR_HPP
