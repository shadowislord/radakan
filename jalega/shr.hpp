// shr.hpp: interface for the SHR class.
//
//////////////////////////////////////////////////////////////////////

#ifndef SHR_HPP
#define SHR_HPP

#include "texture.hpp"	//	"tga_loader.hpp"
#include "d3.hpp"

class SHR:
	public Object
{
	public:
		SHR (int argc, char * * argv);						//	constructor
		virtual ~SHR ();									//	destructor
		void initGL () const;
		void draw_start () const;
		void draw_stop () const;
//		Texture loadTexture (string location) const;
		void bind_texture (Texture * texture) const;
		void render_quad (D3 * color, D3 * a, D3 * b, D3 * c, D3 * d)
			const;
//		void renderPointSprite (float xpos, float ypos, int size) const;
		void render_triangle (D3 * color, D3 * a, D3 * b, D3 * c)
			const;
//		void enable2D (int xscale, int yscale) const;
//		void disable2D () const;
		void print_tga (D3 * color, float x, float y, bool italic, string text)
			const;
		void print_bitmap (D3 * color, float x, float y, int font_number,
			string text) const;
		void resize (int new_width, int new_height) const;

	private:
		void texturing_2d (bool flag) const;
		void use_color (D3 * color) const;
		int main_window;	//	GLUT window number
		vector <string> text;
		Texture * font_texture;
};

extern SHR * shr;

#endif	//	SHR_HPP
