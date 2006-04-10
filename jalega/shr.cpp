/*Comments:
The graphics renderer

This should force no opengl commands are needed for use anywhere else.
They should all be accessed from this class.
This ensures that there are no graphics related dependancies from the game logic side.

I'm a bit worried about the texture id problem. If the renderer is switched to another rendering system other than Opengl.
Some classes relating to textures may need changing.
*/

#include "shr.hpp"

int font_index = 0;

void * bitmap_fonts_a[7] =
{
	GLUT_BITMAP_9_BY_15,
	GLUT_BITMAP_8_BY_13,
	GLUT_BITMAP_TIMES_ROMAN_10,
	GLUT_BITMAP_TIMES_ROMAN_24,
	GLUT_BITMAP_HELVETICA_10,
	GLUT_BITMAP_HELVETICA_12,
	GLUT_BITMAP_HELVETICA_18
};

SHR::
	SHR (int argc, char * * argv):
	Object::
	Object ("SHR")
{
	glutInit (&argc, argv);

	glutInitWindowSize (800, 600);
	glutInitDisplayMode (GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);
	main_window = glutCreateWindow ("tslrpg");
	
	glClearDepth (1.0);
	glDepthFunc (GL_LESS);
	glShadeModel (GL_SMOOTH);							// Enable Smooth Shading
	glHint (GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	//	really nice perspective

	for (unsigned int j = 0; j < 4; j++)
	{
		text.push_back (to_string (j) + ": ");
		for (unsigned int i = 0; i < 32; i++)
		{
			if (i + j == 0)	//	Skip zero - it's the null terminator
			{
				continue;
			}
			text.at (j).push_back (char (i + 32 * j));
		}
		text.at (j).push_back ('\0');
	}

	font_texture = new Texture ("fonts/test.tga");
	font_texture->build_font ();
	bind_texture (font_texture);
}

SHR::~SHR()
{
	delete font_texture;
	
	// shut down our window
	glutDestroyWindow (main_window);
}

void
	SHR::
	draw_start
	()
	const
{
	// Clear the window.
	glClearColor (0.03, 0.03, 0.03, 0.0);
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity ();

	render_triangle (
		new D3 (0.0, 0.4, 0.7),				//	color
		new D3 (- 225.0f, 70.0f, 20.0f),	//	a
		new D3 (100.0f, 200.0f, 0.0f),		//	b
		new D3 (10.0f, - 50.0f, 0.0f)		//	c
	);

	//	Draw the strings, according to the current mode and font.
	for (int j = 0; j < 4; j++)
	{
		print_bitmap (new D3 (0.0, 1.0, 0.0), - 400.0, 250.0 - 20.0 * j,
			bitmap_fonts_a [font_index], text [j]);
	}
}

void SHR::draw_stop () const
{

	print_tga (new D3 (1.0f, 0.5f, 0.5f), 60, - 120, false, "Lazy");
	print_tga (new D3 (0.85f, 0.5f, 0.67f), 60, - 100, false, "Bum");
	print_tga (new D3 (0.67f, 0.5f, 0.85f), 60, - 80, false, "Ware");
	print_tga (new D3 (0.5f, 0.5f, 1.0f), 40, 0, 0, "Productions");

	glutSwapBuffers ();
}

//	Enables 2D texturing
void SHR::texturing_2d (bool flag) const
{
	if (flag)
	{
		glEnable (GL_TEXTURE_2D);
	}
	else
	{
		glDisable (GL_TEXTURE_2D);
	}
}

void SHR::use_color (D3 * color) const
{
	assert (color != NULL);
	
	glColor3f (color->x, color->y, color->z);

	delete color;
}

//	Binds a texture with opengl.
void SHR::bind_texture (Texture * texture) const
{
	assert (texture != NULL);
	
	glBindTexture (GL_TEXTURE_2D, texture->get_id ());
}

void
	SHR::
	render_quad
	(D3 * pos, float width, float height,
	float rotation, float tx, float ty) const
{
	glPushMatrix ();
	glTranslatef (pos->x, pos->y, pos->z);
	delete pos;
	
	if (rotation != 0)
	{
		glRotatef (rotation, 0, 0, 1);
	}
	glTranslatef (- width / 2.0f, - width / 2.0f, 0); //makes sure that it rotates around its center
	glBegin (GL_QUADS);
		/*glTexCoord2f (tx+0, -ty-1);*/ glVertex2f (0, 0);
		/*glTexCoord2f (tx+1, -ty-1);*/ glVertex2f (width, 0);
		/*glTexCoord2f (tx+1, -ty-0);*/ glVertex2f (width, height);
		/*glTexCoord2f (tx+0, -ty-0);*/ glVertex2f (0, height);
	glEnd();
	glPopMatrix();
}

void
	SHR::
	render_triangle
	(D3 * color, D3 * a, D3 * b, D3 * c) const
{
	glPushMatrix ();

	use_color (color);
	
	glBegin (GL_POLYGON);					// start drawing a polygon
		glVertex3f (a->x, a->y, a->z);
		glVertex3f (b->x, b->y, b->z);
		glVertex3f (c->x, c->y, c->z);
	glEnd ();								// we're done with the polygon

	glPopMatrix ();

	delete a;
	delete b;
	delete c;
}

/*void
	SHR::
	renderPointSprite
	(float xpos, float ypos, int size) const
{
	
}

void SHR::enable2D (int xscale, int yscale)
{
	glMatrixMode (GL_PROJ	glCallLists (fmt.size (), GL_UNSIGNED_BYTE, fmt.c_str ());	// Write The Text To The Screen
ECTION);
	glPushMatrix ();
	glLoadIdentity ();
	glOrtho (0, xscale, 0, yscale, -1, 0);
	glMatrixMode (GL_MODELVIEW);
	glPushMatrix ();
	glLoadIdentity ();
}

//	Returns the view to normal

void SHR::disable2D ()
{
	glMatrixMode (GL_PROJECTION);
	glPopMatrix ();
	glMatrixMode (GL_MODELVIEW);
	glPopMatrix ();
}*/

void
	SHR::
	print_bitmap
	(D3 * color, float x, float y, void * font, string text)
	const
{
	glPushMatrix ();
	use_color (color);

	glRasterPos2f (x, y);
	for (unsigned int i = 0; i < text.size (); i++)
	{
		glutBitmapCharacter (font, text.at (i));
	}
	glPopMatrix ();
}

void SHR::
	print_tga (D3 * color, float x, float y, bool italic, string text) const
{
	assert (text != "");
	
	glPushMatrix ();

	use_color (color);

	texturing_2d (true);										// Enable Texture Mapping
	glTranslated (-x, -y, 0);									// Position The Text (0,0 - Top Left)
	
	if (italic)
	{
		glListBase (- 31);								// Choose The Font Set (0 or 1)
	}
	else
	{
		glListBase (97);								// Choose The Font Set (0 or 1)
	}
	glCallLists (text.size (), GL_UNSIGNED_BYTE, text.c_str ());	// Write The Text To The Screen
//	glTranslated (x, y, 0);										// Position The Text (0,0 - Top Left)

	texturing_2d (false);										// Disable Texture Mapping

	glPopMatrix ();
}

void
	SHR::resize
	(int new_width, int new_height) const
{
	GLdouble size;
	GLdouble aspect;

	// Use the whole window.
	glViewport (0, 0, new_width, new_height);

	// We are going to do some 2-D orthographic drawing.
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();
	size = (GLdouble)((new_width >= new_height) ? new_width : new_height) / 2.0;
	if (new_width <= new_height)
	{
		aspect = (GLdouble) new_height / (GLdouble) new_width;
		glOrtho (-size, size, -size * aspect, size * aspect, -10000.0, 10000.0);
	}
	else
	{
		aspect = (GLdouble) new_width / (GLdouble) new_height;
		glOrtho (-size * aspect, size * aspect, -size, size, -10000.0, 10000.0);
	}

	// Make the world and window coordinates coincide so that 1.0 in
	// model space equals one pixel in window space.
	glScaled (aspect, aspect, 1.0);

	// Now determine where to draw things.
	glMatrixMode (GL_MODELVIEW);
	glLoadIdentity ();
}
