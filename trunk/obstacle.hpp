#ifndef OBSTACLE_HPP
#define OBSTACLE_HPP

#include <OgreVector3.h>
#include "entity.hpp"

using namespace std;
using namespace Ogre;

class Obstacle:
	public Tsl_Entity
{
	public:
		Obstacle (string new_name, Vector3 new_size);
		virtual ~Obstacle ();
		virtual bool is_initialized () const;
		Vector3 size;	//	the bounding box
};

#endif
