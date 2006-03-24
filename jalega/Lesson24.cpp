/*
 *		This Code Was Created By Jeff Molofee 2000
 *		If You've Found This Code Useful, Please Let Me Know.
 *		Visit My Site At nehe.gamedev.net
 */

#include <iostream>										// Header File For Standard Input / Output
#include <cassert>
#include <string>										// Header File For String Management
#include <GL/gl.h>										// Header File For The OpenGL32 Library
#include <GL/glu.h>										// Header File For The GLu32 Library
#include <GL/glut.h>

using namespace std;


int			scroll;											// Used For Scrolling The Screen
int			maxtokens;										// Keeps Track Of The Number Of Extensions Supported
int			swidth;											// Scissor Width
int			sheight;										// Scissor Height

typedef struct												// Create A Structure
{
	GLubyte	*imageData;										// Image Data (Up To 32 Bits)
	GLuint	bpp;											// Image Color Depth In Bits Per Pixel.
	GLuint	width;											// Image Width
	GLuint	height;											// Image Height
	GLuint	texID;											// Texture ID Used To Select A Texture
} TextureImage;												// Structure Name

TextureImage * font_texture;									// Storage For One Texture

void BuildFont()									// Build Our Font Display List
{
	glBindTexture (GL_TEXTURE_2D, font_texture->texID);		// Select Our Font Texture
	for (int i=0; i < 256; i++)					// Loop Through All 256 Lists
	{
		float cx = float (i % 16) / 16.0f;						// X Position Of Current Character
		float cy = float (i / 16) / 16.0f;						// Y Position Of Current Character

		glNewList (1+ i,GL_COMPILE);					// Start Building A List
			glBegin (GL_QUADS);								// Use A Quad For Each Character
				glTexCoord2f (cx,1.0f-cy-0.0625f);			// Texture Coord (Bottom Left)
				glVertex2d (0,16);							// Vertex Coord (Bottom Left)
				glTexCoord2f (cx+0.0625f,1.0f-cy-0.0625f);	// Texture Coord (Bottom Right)
				glVertex2i (16,16);							// Vertex Coord (Bottom Right)
				glTexCoord2f (cx+0.0625f,1.0f-cy-0.001f);	// Texture Coord (Top Right)
				glVertex2i (16,0);							// Vertex Coord (Top Right)
				glTexCoord2f (cx,1.0f-cy-0.001f);			// Texture Coord (Top Left)
				glVertex2i (0,0);							// Vertex Coord (Top Left)
			glEnd ();										// Done Building Our Quad (Character)
			glTranslated (14,0,0);							// Move To The Right Of The Character
		glEndList ();										// Done Building The Display List
	}														// Loop Until All 256 Are Built
}


bool LoadTGA(TextureImage *texture,  string filename)			// Loads A TGA File Into Memory
{    
	GLubyte		TGAheader[12]={0,0,2,0,0,0,0,0,0,0,0,0};	// Uncompressed TGA Header
	GLubyte		TGAcompare[12];								// Used To Compare TGA Header
	GLubyte		header[6];									// First 6 Useful Bytes From The Header
	GLuint		bytesPerPixel;								// Holds Number Of Bytes Per Pixel Used In The TGA File
	GLuint		imageSize;									// Used To Store The Image Size When Setting Aside Ram
	GLuint		temp;										// Temporary Variable
	GLuint		type=GL_RGBA;								// Set The Default GL Mode To RBGA (32 BPP)

	FILE *file = fopen(filename.c_str (), "rb");						// Open The TGA File

	if(	file==NULL ||										// Does File Even Exist?
		fread(TGAcompare,1,sizeof(TGAcompare),file)!=sizeof(TGAcompare) ||	// Are There 12 Bytes To Read?
		memcmp(TGAheader,TGAcompare,sizeof(TGAheader))!=0				||	// Does The Header Match What We Want?
		fread(header,1,sizeof(header),file)!=sizeof(header))				// If So Read Next 6 Header Bytes
	{
		if (file == NULL)									// Did The File Even Exist? *Added Jim Strong*
			return false;									// Return False
		else
		{
			fclose(file);									// If Anything Failed, Close The File
			return false;									// Return False
		}
	}

	texture->width  = header[1] * 256 + header[0];			// Determine The TGA Width	(highbyte*256+lowbyte)
	texture->height = header[3] * 256 + header[2];			// Determine The TGA Height	(highbyte*256+lowbyte)

 	if(	texture->width	<=0	||								// Is The Width Less Than Or Equal To Zero
		texture->height	<=0	||								// Is The Height Less Than Or Equal To Zero
		(header[4]!=24 && header[4]!=32))					// Is The TGA 24 or 32 Bit?
	{
		fclose(file);										// If Anything Failed, Close The File
		return false;										// Return False
	}

	texture->bpp	= header[4];							// Grab The TGA's Bits Per Pixel (24 or 32)
	bytesPerPixel	= texture->bpp/8;						// Divide By 8 To Get The Bytes Per Pixel
	imageSize		= texture->width*texture->height*bytesPerPixel;	// Calculate The Memory Required For The TGA Data

	texture->imageData=(GLubyte *)malloc(imageSize);		// Reserve Memory To Hold The TGA Data

	if(	texture->imageData==NULL ||							// Does The Storage Memory Exist?
		fread(texture->imageData, 1, imageSize, file)!=imageSize)	// Does The Image Size Match The Memory Reserved?
	{
		if(texture->imageData!=NULL)						// Was Image Data Loaded
			free(texture->imageData);						// If So, Release The Image Data

		fclose(file);										// Close The File
		return false;										// Return False
	}

	for(GLuint i=0; i<(unsigned int)(imageSize); i+=bytesPerPixel)		// Loop Through The Image Data
	{														// Swaps The 1st And 3rd Bytes ('R'ed and 'B'lue)
		temp=texture->imageData[i];							// Temporarily Store The Value At Image Data 'i'
		texture->imageData[i] = texture->imageData[i + 2];	// Set The 1st Byte To The Value Of The 3rd Byte
		texture->imageData[i + 2] = temp;					// Set The 3rd Byte To The Value In 'temp' (1st Byte Value)
	}

	fclose (file);											// Close The File

	// Build A Texture From The Data
	glGenTextures(1, &texture[0].texID);					// Generate OpenGL texture IDs

	glBindTexture(GL_TEXTURE_2D, texture[0].texID);			// Bind Our Texture
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	// Linear Filtered
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);	// Linear Filtered
	
	if (texture[0].bpp==24)									// Was The TGA 24 Bits
	{
		type=GL_RGBA;										// If So Set The 'type' To GL_RGB
	}

	glTexImage2D(GL_TEXTURE_2D, 0, type, texture[0].width, texture[0].height, 0, type, GL_UNSIGNED_BYTE, texture[0].imageData);

	return true;											// Texture Building Went Ok, Return True
}

GLvoid glPrint(GLint x, GLint y, int set, string fmt)	// Where The Printing Happens
{
	if (fmt == "")										// If There's No Text
		return;												// Do Nothing
	
	assert (set <= 1);
//	glEnable(GL_TEXTURE_2D);								// Enable Texture Mapping
	glLoadIdentity();										// Reset The Modelview Matrix
	glTranslated(x,y,0);									// Position The Text (0,0 - Top Left)
	glListBase(- 31 + 128 * set);							// Choose The Font Set (0 or 1)

	glCallLists(fmt.size (), GL_UNSIGNED_BYTE, fmt.c_str ());		// Write The Text To The Screen
	glDisable(GL_TEXTURE_2D);								// Disable Texture Mapping
}

void Resize(int width, int height)			// Resize And Initialize The GL Window
{
	glViewport(0,0,width,height);							// Reset The Current Viewport
	glMatrixMode(GL_PROJECTION);							// Select The Projection Matrix
	glLoadIdentity();										// Reset The Projection Matrix
	glOrtho(0.0f,640,480,0.0f,-1.0f,1.0f);					// Create Ortho 640x480 View (0,0 At Top Left)
	glMatrixMode(GL_MODELVIEW);								// Select The Modelview Matrix
	glLoadIdentity();										// Reset The Modelview Matrix
}

int InitGL()											// All Setup For OpenGL Goes Here
{
	if (! LoadTGA(font_texture,"fonts/test.tga"))				// Load The Font Texture
	{
		return false;										// If Loading Failed, Return False
	}

	BuildFont();											// Build The Font

	glShadeModel(GL_SMOOTH);								// Enable Smooth Shading
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);					// Black Background
	glClearDepth(1.0f);										// Depth Buffer Setup
	glBindTexture(GL_TEXTURE_2D, font_texture->texID);		// Select Our Font Texture

	return true;											// Initialization Went OK
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
//		print_bitmap_string (bitmap_fonts [font_index], text [j]);
	}
		
	glColor4f (0.0, 0.5, 1.0, 0.0);
	glBegin (GL_POLYGON);					// start drawing a polygon
		glVertex3f (0.0f, -20.0f, 0.0f);
		glVertex3f (100.0f, 0.0f, 0.0f);
		glVertex3f (10.0f, -50.0f, 0.0f);
	glEnd ();								// we're done with the polygon

/////////////////////////////////////////////////////////

	glColor3f(1.0f,0.5f,0.5f);								// Set Color To Bright Red
	glPrint(50,16,1,"Renderer");							// Display Renderer
	glPrint(80,48,1,"Vendor");								// Display Vendor Name
	glPrint(66,80,1,"Version");								// Display Version

	glColor3f(0.5f,0.5f,1.0f);								// Set Color To Bright Blue
	glPrint(192,432,0,"LazyBumWare Productions");

////////////////////////////////////
	glutSwapBuffers ();
}

void KeyFunc(unsigned char key, int x, int y)
{
    switch(key) {
        case 27:    //escape key
            exit(0);
            break;
        default:
            break;
    }
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitWindowSize(640, 480);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutCreateWindow("NeHe's Token, Extensions, Scissoring & TGA Loading Tutorial");
    glutDisplayFunc(draw);
    glutKeyboardFunc(KeyFunc);
    glutReshapeFunc(Resize);
		font_texture = new TextureImage ();
    if(!InitGL())
	{
        abort ();
    }
    glutMainLoop();
}
    
