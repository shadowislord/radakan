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
		Vector3 position;	//	the position of the obstacle
		float weight; //in kilograms preferably
		bool moving; //true = object is moving, false = object is static
};

#endif
