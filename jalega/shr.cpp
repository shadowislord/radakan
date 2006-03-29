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

void * * bitmap_fonts_a[7] =
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
	(void * * font, string s)
{
	for (unsigned int i = 0; i < s.size (); i++)
	{
		glutBitmapCharacter (font, s.at (i));
	}
}

SHR::
	SHR ():
	Object::
	Object ("SHR")
{
/*	glClearColor (0, 0, 0, 0);
	glClearDepth (1.0f);
	glEnable (GL_BLEND);
	//glDepthMask(GL_TRUE);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);*/
	//glEnable(GL_DEPTH_TEST);
	//glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do
	//glEnable(GL_ALPHA_TEST);
	//glEnable(GL_CULL_FACE);
	//glShadeModel(GL_SMOOTH);
	//glEnable(GL_POLYGON_SMOOTH);
	//glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations

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
	build_font (font_texture);
}

SHR::~SHR()
{
	delete font_texture;
}

void SHR::draw ()
{
	// Clear the window.
	glClearColor (0.0, 0.0, 0.0, 0.0);
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity ();

	glColor4f (0.0, 0.5, 1.0, 0.0);
	glBegin (GL_POLYGON);					// start drawing a polygon
		glVertex3f (- 20.0f, - 20.0f, 0.0f);
		glVertex3f (100.0f, 200.0f, 0.0f);
		glVertex3f (10.0f, - 50.0f, 0.0f);
	glEnd ();								// we're done with the polygon

	// Draw the strings, according to the current mode and font.
	glColor4f (0.0, 1.0, 0.0, 0.0);
	for (int j = 0; j < 4; j++)
	{
		glRasterPos2f (- 225.0, 70.0 - 20.0 * j);
		print_bitmap/*_string*/ (bitmap_fonts_a [font_index], text [j]);
	}
		
	glColor3f (1.0f,0.5f,0.5f);				// Set Color To Bright Red
	glPrint (60, - 128, 1, "Renderer");		// Display Renderer
	glPrint (60, - 96, 1, "Vendor");		// Display Vendor Name
	glPrint (60, - 64, 1, "Version");		// Display Version

	glColor3f (0.5f,0.5f,1.0f);				// Set Color To Bright Blue
	glPrint (200, 100, 0, "LazyBumWare Productions");

	glutSwapBuffers ();
}

//	Enables 2D texturing
void SHR::texturing2D (bool flag)
{
	if (flag)
		glEnable (GL_TEXTURE_2D);
	else
		glDisable (GL_TEXTURE_2D);
}


Texture SHR::loadTexture (string location)
{
	//glTexParameteri (GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);

	Texture texture ("error");
//	LoadTGA (&texture, location);						//	gives linking error
	glGenTextures (1, &texture.texID);

	//GLint saveTextureBind = 0;
	//glEnable (GL_TEXTURE_2D);
	//glGetIntegerv (GL_TEXTURE_BINDING_2D, &saveTextureBind);
	
	glBindTexture (GL_TEXTURE_2D, texture.texID);

	float maximumAnistropy = 2; //just set it to two, anything higher shows little effect
	glGetFloatv (GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maximumAnistropy); //only use when full anisotropy is needed
	glTexParameterf (GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, maximumAnistropy);

	//glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_DECAL);

	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

//	-	'class Texture' has no member named 'type'	-	gluBuild2DMipmaps (GL_TEXTURE_2D, GL_COMPRESSED_RGBA_S3TC_DXT5_EXT, texture.width, texture.height, texture.type, GL_UNSIGNED_BYTE, texture.imageData);

	//gluBuild2DMipmaps(GL_TEXTURE_2D, texture.type, texture.width, texture.height, texture.type, GL_UNSIGNED_BYTE, texture.imageData);

	//glTexImage2D(GL_TEXTURE_2D, 0, 4, texture.width, texture.height, 0, texture.type, GL_UNSIGNED_BYTE, texture.imageData);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 3);
//glTexImage2D(GL_TEXTURE_2D, 0, 4, texture.width, texture.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture.imageData);

	//glBindTexture(GL_TEXTURE_2D, saveTextureBind);

	if (texture.imageData)						// If Texture Image Exists ( CHANGE )
	{
		free(texture.imageData);					// Free The Texture Image Memory ( CHANGE )
	}
	return texture;
}


//	Binds a texture with opengl. If the id is -1 then no texture is bound.
void SHR::bindTexture (int textureid)
{
	if(textureid != -1)
	{
		glBindTexture (GL_TEXTURE_2D, textureid);
	}
}

void SHR::renderQuad (int textureid, float xpos, float ypos, float width, float height, float zdepth, float rotation, float tx, float ty)
{
	glPushMatrix ();
	bindTexture (textureid);
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

void SHR::renderPointSprite (int textureid, float xpos, float ypos, int size)
{
	
}

void SHR::enable2D (int xscale, int yscale)
{
	glMatrixMode (GL_PROJECTION);
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
}

void SHR::build_font (Texture * texture)				// Build Our Font Display List
{
	assert (texture != NULL);
	for (int i = 0; i < 256; i++)						// Loop Through All 256 Lists
	{
		float cx = float (i % 16) / 16.0f;				// X Position Of Current Character
		float cy = 1.0f - float (i / 16) / 16.0f;		// Y Position Of Current Character
		float a = 0.0625f;
		float b = 0.001f;

		glNewList (1 + i, GL_COMPILE);					// Start Building A List
			glBegin (GL_QUADS);							// Use A Quad For Each Character
				glTexCoord2f (cx, cy - a);				// Texture Coord (Bottom Left)
				glVertex2d (0, 0);						// Vertex Coord (Bottom Left)
				glTexCoord2f (cx + a, cy - a);			// Texture Coord (Bottom Right)
				glVertex2i (16, 0);						// Vertex Coord (Bottom Right)
				glTexCoord2f (cx + a, cy - b);			// Texture Coord (Top Right)
				glVertex2i (16, 16);					// Vertex Coord (Top Right)
				glTexCoord2f (cx, cy - b);				// Texture Coord (Top Left)
				glVertex2i (0, 16);						// Vertex Coord (Top Left)
			glEnd ();									// Done Building Our Quad (Character)
			glTranslated (14, 0, 0);					// Move To The Right Of The Character
		glEndList ();									// Done Building The Display List
	}													// Loop Until All 256 Are Built
	glBindTexture (GL_TEXTURE_2D, texture->texID);	// Select Our Font Texture
}

void SHR::glPrint (GLint x, GLint y, unsigned int set, string fmt)	// Where The Printing Happens
{
	assert (fmt != "");
	assert (set <= 1);

	glEnable (GL_TEXTURE_2D);									// Enable Texture Mapping
	glLoadIdentity ();											// Reset The Modelview Matrix
	glTranslated (-x, -y, 0);									// Position The Text (0,0 - Top Left)
	glListBase (128 * set - 31);								// Choose The Font Set (0 or 1)
	glCallLists (fmt.size (), GL_UNSIGNED_BYTE, fmt.c_str ());	// Write The Text To The Screen
	glTranslated (x, y, 0);										// Position The Text (0,0 - Top Left)

	glDisable (GL_TEXTURE_2D);									// Disable Texture Mapping
}

