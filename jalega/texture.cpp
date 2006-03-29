#include "texture.hpp"

using namespace std;

//  Constructor
Texture::
	Texture (string filename):
	Object::
	Object (filename)
{
	assert (is_initialized ());
	load_TGA (filename);
}

//  Destructor
Texture::
	~Texture ()
{
	assert (is_initialized ());
}

//	virtual
bool
	Texture::
	is_initialized ()
	const
{
	return Object::is_initialized ();
}

void
	Texture::
	load_TGA
	(string filename)		// loads a TGA file into graphic card memory
{
	assert (is_initialized ());
	GLubyte		TGAheader[12]={0,0,2,0,0,0,0,0,0,0,0,0};	// Uncompressed TGA Header
	GLubyte		TGAcompare[12];								// Used To Compare TGA Header
	GLubyte		header[6];									// First 6 Useful Bytes From The Header
	GLuint		bytesPerPixel;								// Holds Number Of Bytes Per Pixel Used In The TGA File
	GLuint		imageSize;									// Used To Store The Image Size When Setting Aside Ram
	GLuint		temp;										// Temporary Variable
	GLuint		type=GL_RGBA;								// Set The Default GL Mode To RBGA (32 BPP)

	FILE *file = fopen(filename.c_str (), "rb");						// Open The TGA File

	assert (file != NULL);					// Does File Even Exist?
	assert (fread(TGAcompare,1,sizeof(TGAcompare),file)==sizeof(TGAcompare));	// Are There 12 Bytes To Read?
	assert (memcmp(TGAheader,TGAcompare,sizeof(TGAheader)) == 0);	// Does The Header Match What We Want?
	assert (fread(header,1,sizeof(header),file)==sizeof(header));	// If So Read Next 6 Header Bytes

	width  = header[1] * 256 + header[0];			// Determine The TGA Width	(highbyte*256+lowbyte)
	height = header[3] * 256 + header[2];			// Determine The TGA Height	(highbyte*256+lowbyte)

 	assert (width > 0);									// Is The Width Less Than Or Equal To Zero
	assert (height > 0);								// Is The Height Less Than Or Equal To Zero
	assert ((header[4] == 24) || (header[4] == 32));	// Is The TGA 24 or 32 Bit?

	bpp	= header[4];							// Grab The TGA's Bits Per Pixel (24 or 32)
	bytesPerPixel	= bpp / 8;						// Divide By 8 To Get The Bytes Per Pixel
	imageSize		= width * height * bytesPerPixel;	// Calculate The Memory Required For The TGA Data

	imageData = (GLubyte *) malloc (imageSize);		// Reserve Memory To Hold The TGA Data

	assert (imageData != NULL);									// Does The Storage Memory Exist?
	assert (fread(imageData, 1, imageSize, file) == imageSize);	// Does The Image Size Match The Memory Reserved?


	for(GLuint i=0; i<(unsigned int)(imageSize); i+=bytesPerPixel)		// Loop Through The Image Data
	{											// Swaps The 1st And 3rd Bytes ('R'ed and 'B'lue)
		temp = imageData[i];					// Temporarily Store The Value At Image Data 'i'
		imageData[i] = imageData[i + 2];		// Set The 1st Byte To The Value Of The 3rd Byte
		imageData[i + 2] = temp;				// Set The 3rd Byte To The Value In 'temp' (1st Byte Value)
	}

	fclose (file);								// Close The File

	// Build A Texture From The Data
	glGenTextures (1, & texID);					// Generate OpenGL texture IDs

	glBindTexture (GL_TEXTURE_2D, texID);		// Bind Our Texture
	glTexParameterf (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	// Linear Filtered
	glTexParameterf (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);	// Linear Filtered
	
	if (bpp == 24)								// Was The TGA 24 Bits
	{
		type = GL_RGBA;							// If So Set The 'type' To GL_RGB
	}

	glTexImage2D (GL_TEXTURE_2D, 0, type, width, height, 0, type, GL_UNSIGNED_BYTE, imageData);

	return;											// Texture Building Went Ok, Return
}
