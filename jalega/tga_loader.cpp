/********************************************************************************
/Name:		TGA.cpp																*
/Header:	tga.h																*
/Purpose:	Load Compressed and Uncompressed TGA files							*
/Functions:	LoadTGA(Texture * texture, char * filename)							*
/			LoadCompressedTGA(Texture * texture, char * filename, FILE * fTGA)	*
/			LoadUncompressedTGA(Texture * texture, char * filename, FILE * fTGA)*	
********************************************************************************/
#include "tga_loader.hpp"

/********************************************************************************
/name :		LoadTGA(Texture * texture, char * filename)							*
/function:  Open and test the file to make sure it is a valid TGA file			*	
/parems:	texture, pointer to a Texture structure								*
/			filename, string pointing to file to open							*
********************************************************************************/

bool LoadTGA(Texture * texture, char * filename)						// Load a TGA file
{
	FILE * fTGA;														// File pointer to texture file
	fTGA = fopen(filename, "rb");										// Open file for reading

	assert (fTGA != NULL);												// If it didn't open....

	assert (fread(&tgaheader, sizeof (TGAHeader), 1, fTGA) != 0);		// Attempt to read 12 byte header from file

	if (memcmp(uTGAcompare, &tgaheader, sizeof (tgaheader)) == 0)		// See if header matches the predefined header of
	{																	// an Uncompressed TGA image
		LoadUncompressedTGA(texture, filename, fTGA);					// If so, jump to Uncompressed TGA loading code
	}
	else if (memcmp (cTGAcompare, &tgaheader, sizeof (tgaheader)) == 0)	// See if header matches the predefined header of
	{																	// an RLE compressed TGA image
		LoadCompressedTGA (texture, filename, fTGA);					// If so, jump to Compressed TGA loading code
	}
	else																// If header matches neither type
	{
		abort ();														// Exit function
	}
	return true;														// All went well, continue on
}

bool LoadUncompressedTGA (Texture * texture, char * filename, FILE * fTGA)	// Load an uncompressed TGA (note, much of this code is based on NeHe's 
{																		// TGA Loading code nehe.gamedev.net)
	assert (fread(tga.header, sizeof(tga.header), 1, fTGA) != 0);				// Read TGA header


	texture->width  = tga.header[1] * 256 + tga.header[0];					// Determine The TGA Width	(highbyte*256+lowbyte)
	texture->height = tga.header[3] * 256 + tga.header[2];					// Determine The TGA Height	(highbyte*256+lowbyte)
	texture->bpp	= tga.header[4];										// Determine the bits per pixel
	tga.Width		= texture->width;										// Copy width into local structure						
	tga.Height		= texture->height;										// Copy height into local structure
	tga.Bpp			= texture->bpp;											// Copy BPP into local structure

	assert (texture->width > 0);
	assert (texture->height > 0);
	assert ((texture->bpp == 24) || (texture->bpp == 32));	// Make sure all information is valid


	if(texture->bpp == 24)													// If the BPP of the image is 24...
		texture->type	= GL_RGB;											// Set Image type to GL_RGB
	else																	// Else if its 32 BPP
		texture->type	= GL_RGBA;											// Set image type to GL_RGBA

	tga.bytesPerPixel	= (tga.Bpp / 8);									// Compute the number of BYTES per pixel
	tga.imageSize		= (tga.bytesPerPixel * tga.Width * tga.Height);		// Compute the total amout ofmemory needed to store data
	texture->imageData	= (GLubyte *)malloc(tga.imageSize);					// Allocate that much memory

	assert (texture->imageData != NULL);											// If no space was allocated

	assert (fread(texture->imageData, 1, tga.imageSize, fTGA) == tga.imageSize);	// Attempt to read image data

	// Byte Swapping Optimized By Steve Thomas
	for(GLuint cswap = 0; cswap < (int)tga.imageSize; cswap += tga.bytesPerPixel)
	{
		texture->imageData[cswap] ^= texture->imageData[cswap+2] ^=
		texture->imageData[cswap] ^= texture->imageData[cswap+2];
	}

	fclose(fTGA);															// Close file
	return true;															// Return success
}

bool LoadCompressedTGA(Texture * texture, char * filename, FILE * fTGA)		// Load COMPRESSED TGAs
{ 
	assert (fread(tga.header, sizeof(tga.header), 1, fTGA) != 0);			// Attempt to read header

	texture->width  = tga.header[1] * 256 + tga.header[0];					// Determine The TGA Width	(highbyte*256+lowbyte)
	texture->height = tga.header[3] * 256 + tga.header[2];					// Determine The TGA Height	(highbyte*256+lowbyte)
	texture->bpp	= tga.header[4];										// Determine Bits Per Pixel
	tga.Width		= texture->width;										// Copy width to local structure
	tga.Height		= texture->height;										// Copy width to local structure
	tga.Bpp			= texture->bpp;											// Copy width to local structure

	assert (texture->width > 0);
	assert (texture->height > 0);
	assert ((texture->bpp == 24) || (texture->bpp == 32));	// Make sure all information is valid

	if(texture->bpp == 24)													// If the BPP of the image is 24...
		texture->type	= GL_RGB;											// Set Image type to GL_RGB
	else																	// Else if its 32 BPP
		texture->type	= GL_RGBA;											// Set image type to GL_RGBA

	tga.bytesPerPixel	= (tga.Bpp / 8);									// Compute BYTES per pixel
	tga.imageSize		= (tga.bytesPerPixel * tga.Width * tga.Height);		// Compute amout of memory needed to store image
	texture->imageData	= (GLubyte *)malloc(tga.imageSize);					// Allocate that much memory

	assert (texture->imageData != NULL);									// If it wasnt allocated correctly..

	GLuint pixelcount	= tga.Height * tga.Width;							// Nuber of pixels in the image
	GLuint currentpixel	= 0;												// Current pixel being read
	GLuint currentbyte	= 0;												// Current byte 
	GLubyte * colorbuffer = (GLubyte *)malloc(tga.bytesPerPixel);			// Storage for 1 pixel

	do
	{
		GLubyte chunkheader = 0;											// Storage for "chunk" header

		assert (fread(&chunkheader, sizeof(GLubyte), 1, fTGA) != 0);			// Read in the 1 byte header

		if(chunkheader < 128)												// If the ehader is < 128, it means the that is the number of RAW color packets minus 1
		{																	// that follow the header
			chunkheader++;													// add 1 to get number of following color values
			for(short counter = 0; counter < chunkheader; counter++)		// Read RAW color values
			{
				assert (fread(colorbuffer, 1, tga.bytesPerPixel, fTGA) == tga.bytesPerPixel);
				// Try to read 1 pixel
																						// write to memory
				texture->imageData[currentbyte		] = colorbuffer[2];				    // Flip R and B vcolor values around in the process 
				texture->imageData[currentbyte + 1	] = colorbuffer[1];
				texture->imageData[currentbyte + 2	] = colorbuffer[0];

				if(tga.bytesPerPixel == 4)												// if its a 32 bpp image
				{
					texture->imageData[currentbyte + 3] = colorbuffer[3];				// copy the 4th byte
				}

				currentbyte += tga.bytesPerPixel;										// Increase thecurrent byte by the number of bytes per pixel
				currentpixel++;															// Increase current pixel by 1

				assert (currentpixel <= pixelcount);									// Make sure we havent read too many pixels
			}
		}
		else																			// chunkheader > 128 RLE data, next color reapeated chunkheader - 127 times
		{
			chunkheader -= 127;															// Subteact 127 to get rid of the ID bit
			assert (fread(colorbuffer, 1, tga.bytesPerPixel, fTGA) == tga.bytesPerPixel);	// Attempt to read following color values

			for(short counter = 0; counter < chunkheader; counter++)					// copy the color into the image data as many times as dictated 
			{																			// by the header
				texture->imageData[currentbyte		] = colorbuffer[2];					// switch R and B bytes areound while copying
				texture->imageData[currentbyte + 1	] = colorbuffer[1];
				texture->imageData[currentbyte + 2	] = colorbuffer[0];

				if(tga.bytesPerPixel == 4)												// If TGA images is 32 bpp
				{
					texture->imageData[currentbyte + 3] = colorbuffer[3];				// Copy 4th byte
				}

				currentbyte += tga.bytesPerPixel;										// Increase current byte by the number of bytes per pixel
				currentpixel++;															// Increase pixel count by 1

				assert (currentpixel <= pixelcount);									// Make sure we havent written too many pixels
			}
		}
	}

	while(currentpixel < pixelcount);													// Loop while there are still pixels left
	fclose(fTGA);																		// Close the file
	return true;																		// return success
}
