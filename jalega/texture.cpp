#include "texture.hpp"

using namespace std;

//  Constructor
Texture::
	Texture ():
	Object::
	Object ("annonymous_texture")
{
	assert (is_initialized ());
}

//  Destructor
Texture::
	~Texture ()
{

}

//	virtual
bool
	Texture::
	is_initialized ()
	const
{
	return Object::is_initialized ();
}
