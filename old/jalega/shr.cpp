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
	Object ("shr")
{
	assert (Object::is_initialized ());

	font_index = 0;

	TiXmlDocument doc;
	bool success = doc.LoadFile ("data/" + * this + ".xml");
	assert (success);
	TiXmlHandle docHandle (& doc);
	TiXmlElement * size =
				docHandle.FirstChild (* this).FirstChild ("size").Element ();
	assert (size != NULL);
	int temp_width = - 1;
	size->Attribute ("width", & temp_width);
	assert (0 < temp_width);
	width = (unsigned int) (temp_width);
	int temp_height = - 1;
	size->Attribute ("height", & temp_height);
	assert (0 < temp_height);
	height = (unsigned int) (temp_height);

	glutInit (&argc, argv);

	glutInitWindowSize (width, height);
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
		D3 (0, 0.4, 0.7),				//	color
		D3 (- 500, 70, 20),	//	a
		D3 (- 225, 200, 0),	//	b
		D3 (- 315, - 50, 0)	//	c
	);

	//	Draw the strings, according to the current mode and font.
	for (int j = 0; j < 4; j++)
	{
		print_bitmap (D3 (0, 1, 0), - get_width () /2,
			get_height () / 2 - 50 - 20 * j, font_index, text [j]);
	}
}

void SHR::draw_stop () const
{
	assert (is_initialized ());
	
	print_tga (D3 (1, 0.5, 0.5), - 340, 120, false, "Lazy");
	print_tga (D3 (0.85, 0.5, 0.67), - 350, 80, false, "Bum");
	print_tga (D3 (0.67, 0.5, 0.85), - 360, 40, false, "Ware");
	print_tga (D3 (0.5, 0.5, 1), - 370, 0, false, "Productions");

	glutSwapBuffers ();
}

int SHR::get_width () const
{
	return width;
}

int SHR::get_height () const
{
	return height;
}

void draw (Location * location) const
{
	assert (is_initialized ());
	assert (location->is_initialized ());

//	Pay attention to the order of the types. It does matter.
	if (location->is_type <Tile> ())
	{
		Tile * tile = location->to_type <Tile> ();
		shr->render_quad (
			D3 (0.2, 0.5, 0.2),
			* tile + D3 (12, 12, 0),
			* tile + D3 (12, - 12, 0),
			* tile + D3 (- 12, - 12, 0),
			* tile + D3 (- 12, 12, 0),
		);
	}
	else if (location->is_type <World> ())
	{
		World * world = location->to_type <World> ();
		for (unsigned int i = 0; i < world->tile_width; i++)
		{
//			make sure tiles in the back get draw first:
			for (unsigned int j = 0; j < world->tile_height; j++)
			{
				draw (world->get_tile (i, j));
			}
		}
	
		for (unsigned int i = 0; i < world->obstacles.size (); i++)
		{
			draw (world->obstacles.at (i));
		}
	}
	else if (location->is_type <Obstacle> ())
	{
		Obstacle * obstacle = location->to_type <Obstacle> ();

//		a tiny house is rendered

//		left wall
		shr->render_quad (
			D3 (0.6, 0.6, 0.6),
			D3 (a, b + 5, 0),
			D3 (a + 10, b, 0),
			D3 (a + 10, b + 10, 0),
			D3 (a, b + 15, 0)
		);
//		right wall
		shr->render_quad (
			D3 (0.7, 0.7, 0.7),
			D3 (a + 20, b + 5, 0),
			D3 (a + 10, b, 0),
			D3 (a + 10, b + 10, 0),
			D3 (a + 20, b + 15, 0)
		);
//		left roof
		shr->render_triangle (
			D3 (0.8, 0, 0.1),
			D3 (a + 10, b + 25, 0),
			D3 (a, b + 15, 0),
			D3 (a + 10, b + 10, 0)
		);
//		right roof
		shr->render_triangle (
			D3 (0.9, 0, 0.2),
			D3 (a + 10, b + 25, 0),
			D3 (a + 20, b + 15, 0),
			D3 (a + 10, b + 10, 0)
		);
//		door
		shr->render_quad (
			D3 (0.6, 0.1, 0.1),
			D3 (a + 14, b + 2, 0),
			D3 (a + 16, b + 3, 0),
			D3 (a + 16, b + 9, 0),
			D3 (a + 14, b + 8, 0)
		);
	}
	else
	{
		//	left wall
		shr->render_quad (
			D3 (0.8, 0.5, 0.2),
			* location + D3 (1, 5, 0),
			* location + D3 (- 1, 5, 0),
			* location + D3 (- 1, - 5, 0),
			* location + D3 (1, - 5, 0),
		);
		//	right wall
		shr->render_quad (
			D3 (0.5, 0.2, 0.8),
			* location + D3 (5, 1, 0),
			* location + D3 (5, - 1, 0),
			* location + D3 (- 5, - 1, 0),
			* location + D3 (- 5, 1, 0)
		);
	}
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

void SHR::use_color (D3 color) const
{
	assert (is_initialized ());
//	assert (color != NULL);
	
	glColor3f (color.get_x (), color.get_y (), color.get_z ());

//	delete color;
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
	(D3 color, D3 a, D3 b, D3 c, D3 d)
	const
{
	assert (is_initialized ());
	
	glPushMatrix ();

	debug () << a << b << c << d << endl;
	use_color (color);
	glBegin (GL_QUADS);
		glVertex3f (a.get_x (), a.get_y (), a.get_z ());
		glVertex3f (b.get_x (), b.get_y (), b.get_z ());
		glVertex3f (c.get_x (), c.get_y (), c.get_z ());
		glVertex3f (d.get_x (), d.get_y (), d.get_z ());
	glEnd ();

	glPopMatrix ();
}

void
	SHR::
	render_triangle
	(D3 color, D3 a, D3 b, D3 c) const
{
	assert (is_initialized ());
	
	glPushMatrix ();

	use_color (color);
	glBegin (GL_POLYGON);					// start drawing a polygon
		glVertex3f (a.get_x (), a.get_y (), a.get_z ());
		glVertex3f (b.get_x (), b.get_y (), b.get_z ());
		glVertex3f (c.get_x (), c.get_y (), c.get_z ());
	glEnd ();								// we're done with the polygon

	glPopMatrix ();
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
	(D3 color, float x, float y, int font_number, string text)
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
	print_tga (D3 color, float x, float y, bool italic, string text) const
{
	assert (is_initialized ());
	assert (text != "");
	
	glPushMatrix ();

	use_color (color);

	glTranslated (x, y, 0);			// Position The Text (0,0 - Top Left)
	texturing_2d (true);				// Enable Texture Mapping

	if (italic)
	{
		glListBase (0 - 31);			// Choose The Font Set (0 or 1)
	}
	else
	{
		glListBase (97);				// Choose The Font Set (0 or 1)
	}
	glCallLists (text.size (), GL_UNSIGNED_BYTE, text.c_str ());
										// Write The Text To The Screen

	texturing_2d (false);				// Disable Texture Mapping

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
