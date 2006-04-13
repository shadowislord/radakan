//	The graphics renderer
//	
//	This should force no opengl commands are needed for use anywhere else.
//	They should all be accessed from this class.
//	This ensures that there are no graphics related dependancies from the game
//	logic side.
//	
//	I'm a bit worried about the texture id problem.
//	If the renderer is switched to another rendering system other than Opengl.
//	Some classes relating to textures may need changing.

#include "shr.hpp"

int font_index = 0;

void * bitmap_fonts[7] =
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
	assert (Object::is_initialized ());

	glutInit (&argc, argv);

	glutInitWindowSize (800, 600);
	glutInitDisplayMode (GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);
	main_window = glutCreateWindow ("tslrpg");
	
	glClearDepth (1);
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

	assert (is_initialized ());
}

SHR::~SHR()
{
	assert (is_initialized ());
	delete font_texture;
	
	// shut down our window
	glutDestroyWindow (main_window);
}

//	virtual
bool
	SHR::
	is_initialized
	()
	const
{
	return Object::is_initialized () && (font_texture != NULL) &&
		font_texture->is_initialized () ;
}

void
	SHR::
	draw_start
	()
	const
{
	assert (is_initialized ());
	
	// Clear the window.
	glClearColor (0.03, 0.03, 0.03, 0.0);
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity ();

	render_triangle (
		new D3 (0, 0.4, 0.7),				//	color
		new D3 (- 225, 70, 20),	//	a
		new D3 (100, 200, 0),		//	b
		new D3 (10, - 50, 0)		//	c
	);

	//	Draw the strings, according to the current mode and font.
	for (int j = 0; j < 4; j++)
	{
		print_bitmap (new D3 (0, 1, 0), - 400, 250 - 20 * j,
			font_index, text [j]);
	}
}

void SHR::draw_stop () const
{
	assert (is_initialized ());
	
	print_tga (new D3 (1, 0.5, 0.5), 60, - 120, false, "Lazy");
	print_tga (new D3 (0.85, 0.5, 0.67), 50, - 80, false, "Bum");
	print_tga (new D3 (0.67, 0.5, 0.85), 40, - 40, false, "Ware");
	print_tga (new D3 (0.5, 0.5, 1), 30, 0, false, "Productions");

	glutSwapBuffers ();
}

//	Enables 2D texturing
void SHR::texturing_2d (bool flag) const
{
	assert (is_initialized ());
	
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
	assert (is_initialized ());
	assert (color != NULL);
	
	glColor3f (color->x, color->y, color->z);

	delete color;
}

//	Binds a texture with opengl.
void SHR::bind_texture (Texture * texture) const
{
	assert (is_initialized ());
	assert (texture != NULL);
	
	glBindTexture (GL_TEXTURE_2D, texture->get_id ());
}

void
	SHR::
	render_quad
	(D3 * color, D3 * a, D3 * b, D3 * c, D3 * d)
	const
{
	assert (is_initialized ());
	
	glPushMatrix ();

	use_color (color);
	glBegin (GL_QUADS);
		glVertex3f (a->x, a->y, a->z);
		glVertex3f (b->x, b->y, b->z);
		glVertex3f (c->x, c->y, c->z);
		glVertex3f (d->x, d->y, d->z);
	glEnd ();

	glPopMatrix ();

	delete a;
	delete b;
	delete c;
	delete d;
}

void
	SHR::
	render_triangle
	(D3 * color, D3 * a, D3 * b, D3 * c) const
{
	assert (is_initialized ());
	
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
	assert (is_initialized ());
	
	
}

void SHR::enable2D (int xscale, int yscale)
{
	assert (is_initialized ());
	
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
	assert (is_initialized ());
	
	glMatrixMode (GL_PROJECTION);
	glPopMatrix ();
	glMatrixMode (GL_MODELVIEW);
	glPopMatrix ();
}*/

void
	SHR::
	print_bitmap
	(D3 * color, float x, float y, int font_number, string text)
	const
{
	assert (is_initialized ());
	assert (0 <= font_number);
	assert (font_number < 7);

	glPushMatrix ();
	use_color (color);

	glRasterPos2f (x, y);
	for (unsigned int i = 0; i < text.size (); i++)
	{
		glutBitmapCharacter (bitmap_fonts [font_number], text.at (i));
	}
	glPopMatrix ();
}

void SHR::
	print_tga (D3 * color, float x, float y, bool italic, string text) const
{
	assert (is_initialized ());
	assert (text != "");
	
	glPushMatrix ();

	use_color (color);

	glTranslated (-x, -y, 0);							// Position The Text (0,0 - Top Left)
	texturing_2d (true);								// Enable Texture Mapping
	
	if (italic)
	{
		glListBase (0 - 31);							// Choose The Font Set (0 or 1)
	}
	else
	{
		glListBase (97);								// Choose The Font Set (0 or 1)
	}
	glCallLists (text.size (), GL_UNSIGNED_BYTE, text.c_str ());	// Write The Text To The Screen

	texturing_2d (false);								// Disable Texture Mapping

	glPopMatrix ();
}

void
	SHR::resize
	(int new_width, int new_height) const
{
	assert (is_initialized ());
	
	GLdouble size;
	GLdouble aspect;

	// Use the whole window.
	glViewport (0, 0, new_width, new_height);

	// We are going to do some 2-D orthographic drawing.
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();
	size = (GLdouble)((new_width >= new_height) ? new_width : new_height) / 2;
	if (new_width <= new_height)
	{
		aspect = (GLdouble) new_height / (GLdouble) new_width;
		glOrtho (-size, size, -size * aspect, size * aspect, -10000, 10000);
	}
	else
	{
		aspect = (GLdouble) new_width / (GLdouble) new_height;
		glOrtho (-size * aspect, size * aspect, -size, size, -10000, 10000);
	}

	// Make the world and window coordinates coincide so that 1 in
	// model space equals one pixel in window space.
	glScaled (aspect, aspect, 1);

	// Now determine where to draw things.
	glMatrixMode (GL_MODELVIEW);
	glLoadIdentity ();
}

void
	SHR::
	post_redisplay
	()
	const
{
	glutPostRedisplay ();
}
