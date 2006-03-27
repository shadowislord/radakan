#include "shr.hpp"
#include "world.hpp"
#include "tga_loader.hpp"

using namespace std;

int font_index = 0;
int main_window;	//	GLUT window number
vector <string> text;
vector <Object *> objects;
World * tsl;
Texture * font_texture;

void BuildFont()									// Build Our Font Display List
{
	for (int i = 0; i < 256; i++)					// Loop Through All 256 Lists
	{
		float cx = float (i % 16) / 16.0f;						// X Position Of Current Character
		float cy = float (i / 16) / 16.0f;						// Y Position Of Current Character

		glNewList (1 + i, GL_COMPILE);					// Start Building A List
			glBegin (GL_QUADS);								// Use A Quad For Each Character
				glTexCoord2f (cx,1.0f-cy-0.0625f);			// Texture Coord (Bottom Left)
				glVertex2d (0, 16);							// Vertex Coord (Bottom Left)
				glTexCoord2f (cx+0.0625f,1.0f-cy-0.0625f);	// Texture Coord (Bottom Right)
				glVertex2i (16, 16);							// Vertex Coord (Bottom Right)
				glTexCoord2f (cx+0.0625f,1.0f-cy-0.001f);	// Texture Coord (Top Right)
				glVertex2i (16, 0);							// Vertex Coord (Top Right)
				glTexCoord2f (cx,1.0f-cy-0.001f);			// Texture Coord (Top Left)
				glVertex2i (0, 0);							// Vertex Coord (Top Left)
			glEnd ();										// Done Building Our Quad (Character)
			glTranslated (14,0,0);							// Move To The Right Of The Character
		glEndList ();										// Done Building The Display List
	}														// Loop Until All 256 Are Built
	glBindTexture(GL_TEXTURE_2D, font_texture->texID);		// Select Our Font Texture

}

GLvoid glPrint (GLint x, GLint y, unsigned int set, string fmt)	// Where The Printing Happens
{
	if (fmt == "")										// If There's No Text
		return;												// Do Nothing
	
	assert (set <= 1);

	glEnable (GL_TEXTURE_2D);								// Enable Texture Mapping
	glLoadIdentity ();										// Reset The Modelview Matrix
	glTranslated (x, y, 0);									// Position The Text (0,0 - Top Left)
	glListBase (128 * set - 31);							// Choose The Font Set (0 or 1)
	glCallLists (fmt.size (), GL_UNSIGNED_BYTE, fmt.c_str ());	// Write The Text To The Screen
	glTranslated (-x, -y, 0);								// Position The Text (0,0 - Top Left)

	glDisable (GL_TEXTURE_2D);								// Disable Texture Mapping
}

void * * bitmap_fonts[7] =
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
	print_bitmap_string
	(void * * font, string s)
{
	for (unsigned int i = 0; i < s.size (); i++)
	{
		glutBitmapCharacter (font, s.at (i));
	}
}

void
	resize
	(int new_width, int new_height)
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

void
	handle_input
	(GLubyte key, GLint x, GLint y)
{
	// avoid thrashing this procedure
	usleep (100);
	switch (key)
	{
		case 27:	// Esc - Quit
		{
			delete tsl;
			delete font_texture;
	
			// shut down our window
			glutDestroyWindow (main_window);

			bool objects_left = false;
			for (unsigned int i = 0; i < objects.size (); i++)
			{
				if (objects.at (i) != NULL)
				{
					if (! objects_left)
					{
						cout << "Objects left: " << endl;
						objects_left = true;
					}
					cout << i << ": " << * objects.at (i) << endl;
				}
			}
			if (! objects_left)
			{
				cout << "tslrpg finished successfully. " << endl;
				exit (0);
			}
			cerr << "ERROR: tslrpg finished with orphaned objects. " << endl;
			exit (1);
		}
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		{
			font_index = key - '1';
			glutPostRedisplay();
			break;
		}
		default:
		{
			break;
		}
	}
}

void
	draw
	()
{
	// Clear the window.
	glClearColor (0.0, 0.0, 0.0, 0.0);
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Draw the strings, according to the current mode and font.
	glColor4f (0.0, 1.0, 0.0, 0.0);
	for (int j = 0; j < 4; j++)
	{
		glRasterPos2f (- 225.0, 70.0 - 20.0 * j);
		print_bitmap_string (bitmap_fonts [font_index], text [j]);
	}
		
	glColor4f (0.0, 0.5, 1.0, 0.0);
	glBegin (GL_POLYGON);					// start drawing a polygon
		glVertex3f (0.0f, -20.0f, 0.0f);
		glVertex3f (100.0f, 0.0f, 0.0f);
		glVertex3f (10.0f, -50.0f, 0.0f);
	glEnd ();								// we're done with the polygon

	glColor3f (1.0f,0.5f,0.5f);								// Set Color To Bright Red
	glPrint (50,16,1,"Renderer");							// Display Renderer
	glPrint (80,48,1,"Vendor");								// Display Vendor Name
	glPrint (66,80,1,"Version");								// Display Version

	glColor3f (0.5f,0.5f,1.0f);								// Set Color To Bright Blue
	glPrint (192, 432, 0, "LazyBumWare Productions");

	glutSwapBuffers ();
}

int
	main
	(int argc, char * * argv)
{
	glutInit (&argc, argv);
	glutInitWindowSize (800, 600);

	glutInitDisplayMode (GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);

	main_window = glutCreateWindow ("tslrpg");

	glutDisplayFunc (draw);
	glutReshapeFunc (resize);
	glutKeyboardFunc (handle_input);
	
	glClearDepth (1.0);
	glDepthFunc (GL_LESS);

	font_texture = new Texture ();
	LoadTGA (font_texture, "fonts/test.tga");
	BuildFont ();
	glShadeModel (GL_SMOOTH);								// Enable Smooth Shading
	
	tsl = new World ("test_world");
	Obstacle * obstacle = new Obstacle ("abc");
	
	tsl->get_tile (6, 4)->set_obstacle (obstacle);

	//	Set up some strings with the characters to draw.
	for (unsigned int j = 0; j < 4; j++)	//	Skip zero - it's the null terminator
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

	glutMainLoop ();

	abort ();
	return 1;
}

