// shr.hpp: interface for the SHR class.
//
//////////////////////////////////////////////////////////////////////

#ifndef SHR_HPP
#define SHR_HPP

#include "texture.hpp"	//	"tga_loader.hpp"
#include "vector_3d.hpp"

class SHR:
	public Object
{
	public:
		SHR (int argc, char * * argv);						//	constructor
		virtual ~SHR ();									//	destructor
		void initGL () const;
		void draw_start () const;
		void draw_stop () const;
		void texturing_2d (bool flag) const;
//		Texture loadTexture (string location) const;
		void bind_texture (Texture * texture) const;
		void render_quad (Vector_3D * pos, float width,
			float height, float rotation, float tx, float ty) const;
//		void renderPointSprite (float xpos, float ypos, int size) const;
		void render_triangle (Vector_3D * a, Vector_3D * b, Vector_3D * c)
			const;
		void use_color (Vector_3D * new_color) const;
//		void enable2D (int xscale, int yscale) const;
//		void disable2D () const;
		void glPrint (int x, int y, unsigned int set, string fmt) const;
		void resize (int new_width, int new_height) const;

	private:
		int main_window;	//	GLUT window number
		vector <string> text;
		Texture * font_texture;
};

extern SHR * shr;
extern Texture * font_texture;

#endif	//	SHR_HPP
