#ifndef SL_ENTITY_HPP
#define SL_ENTITY_HPP

#include "object.hpp"
#include <OgreVector3.h>

using namespace std;
using namespace Ogre;

//	This is the base class for all in-game visual classes

class Sl_Entity:
	public Object
{
	public:
		Sl_Entity (string new_name, Vector3 new_size);
		virtual ~Sl_Entity ();
		virtual bool is_initialized () const;
		const Vector3 size;		//	the bounding box
		Vector3 position;	//	the position of the obstacle
};

#endif
