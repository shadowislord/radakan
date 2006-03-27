/*Comments:
The graphics renderer

This should force no opengl commands are needed for use anywhere else.
They should all be accessed from this class.
This ensures that there are no graphics related dependancies from the game logic side.

I'm a bit worried about the texture id problem. If the renderer is switched to another rendering system other than Opengl.
Some classes relating to textures may need changing.
*/

#include "shr.hpp"
/*
bool LoadTGA (Texture * texture, string path);	//	loads tga textures

SHR::
	SHR ():
	Object::
	Object ("SHR")
{
	glClearColor (0, 0, 0, 0);
	glClearDepth (1.0f);
	glEnable (GL_BLEND);
	//glDepthMask(GL_TRUE);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glEnable(GL_DEPTH_TEST);
	//glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do
	//glEnable(GL_ALPHA_TEST);
	//glEnable(GL_CULL_FACE);
	//glShadeModel(GL_SMOOTH);
	//glEnable(GL_POLYGON_SMOOTH);
	//glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations

}

SHR::~SHR()
{

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

	Texture texture;
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

	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_COMPRESSED_RGBA_S3TC_DXT5_EXT, texture.width, texture.height, texture.type, GL_UNSIGNED_BYTE, texture.imageData);

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
*/
