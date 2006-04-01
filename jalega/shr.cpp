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

void
	print_bitmap//_string
	(void * font, string s)
{
	for (unsigned int i = 0; i < s.size (); i++)
	{
		glutBitmapCharacter (font, s.at (i));
	}
}

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

	font_texture = new Texture ("fonts/test.tga", 1);
	font_texture->build_font ();
	bind_texture (font_texture);
}

SHR::~SHR()
{
	delete font_texture;
	
	// shut down our window
	glutDestroyWindow (main_window);
}

void SHR::draw () const
{
	// Clear the window.
	glClearColor (0.0, 0.0, 0.0, 0.0);
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity ();

	glColor4f (0.0, 0.5, 1.0, 0.0);
	render_triangle (
		new Vector_3D (- 225.0f, 70.0f, 20.0f),
		new Vector_3D (100.0f, 200.0f, 0.0f),
		new Vector_3D (10.0f, - 50.0f, 0.0f)
	);

	// Draw the strings, according to the current mode and font.
	glColor4f (0.0, 1.0, 0.0, 0.0);
	for (int j = 0; j < 4; j++)
	{
		glRasterPos2f (- 225.0, 70.0 - 20.0 * j);
		print_bitmap (bitmap_fonts_a [font_index], text [j]);
	}

	for (unsigned int i = 0; i < tsl->get_width (); i++)
	{
		for (unsigned int j = 0; j < tsl->get_height (); j++)
		{
			render_quad	(font_texture, 50 + 25 * i, 100 + 25 * j, 20, 20, 0, 0, 0, 0);
		}
	}
		
	use_color (new Vector_3D (1.0f, 0.5f, 0.5f));	// Set Color To Bright Red
	glPrint (60, - 120, 1, "Lazy");					// Display Renderer
	glPrint (60, - 100, 1, "Bum");					// Display Vendor Name
	glPrint (60, - 80, 1, "Ware");					// Display Version

	use_color (new Vector_3D (0.5f, 0.5f, 1.0f));	// Set Color To Bright Blue
	glPrint (40, 0, 0, "Productions");

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

void SHR::use_color (Vector_3D * new_color) const
{
	glColor3f (new_color->x, new_color->y, new_color->z);

	delete new_color;
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
	(Texture * texture, float xpos, float ypos, float width, float height,
	float zdepth, float rotation, float tx, float ty) const
{
	glPushMatrix ();
	bind_texture (texture);
	glTranslatef (xpos, ypos, zdepth);
	if (rotation != 0)
	{
		glRotatef (rotation, 0, 0, 1);
	}
	glTranslatef (-width/2.0f, -width/2.0f, 0); //makes sure that it rotates around its center
	glBegin (GL_QUADS);
		glTexCoord2f (tx+0, -ty-1); glVertex2f (0, 0);
		glTexCoord2f (tx+1, -ty-1); glVertex2f (width, 0);
		glTexCoord2f (tx+1, -ty-0); glVertex2f (width, height);
		glTexCoord2f (tx+0, -ty-0); glVertex2f (0, height);
	glEnd();
	glPopMatrix();
}

void
	SHR::
	render_triangle
	(Vector_3D * a, Vector_3D * b, Vector_3D * c) const
{
	glBegin (GL_POLYGON);					// start drawing a polygon
		glVertex3f (a->x, a->y, a->z);
		glVertex3f (b->x, b->y, b->z);
		glVertex3f (c->x, c->y, c->z);
	glEnd ();								// we're done with the polygon

	delete a;
	delete b;
	delete c;
}

void
	SHR::
	renderPointSprite
	(Texture * texture, float xpos, float ypos, int size) const
{
	
}

/*void SHR::enable2D (int xscale, int yscale)
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

void SHR::
	glPrint (GLint x, GLint y, unsigned int set, string fmt) const
{
	assert (fmt != "");
	assert (set <= 1);

	texturing_2d (true);										// Enable Texture Mapping
	glLoadIdentity ();											// Reset The Modelview Matrix
	glTranslated (-x, -y, 0);									// Position The Text (0,0 - Top Left)
	glListBase (128 * set - 31);								// Choose The Font Set (0 or 1)
	glCallLists (fmt.size (), GL_UNSIGNED_BYTE, fmt.c_str ());	// Write The Text To The Screen
	glTranslated (x, y, 0);										// Position The Text (0,0 - Top Left)

	texturing_2d (false);										// Disable Texture Mapping
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
