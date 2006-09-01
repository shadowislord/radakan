#ifndef OBSTACLE_HPP
#define OBSTACLE_HPP

#include "object.hpp"
#include <OgreEntity.h>

using namespace std;
//	Don't use namespace Ogre here!

class Entity:
	public Object,
	public Ogre :: Entity
{
	public:
		Entity
			(string new_name,	//	for Obect
			bool new_movable,
			bool new_solid,
			float new_volume,
			float new_weight,
			Ogre::Vector3 new_position);
		virtual ~Entity ();
		virtual bool is_initialized () const;
		virtual float get_total_weight () const;
		const bool movable;		//	false means that the object is static
		const bool solid;		//	false means that you can pass through it
		const float volume;		//	in litres
		const float weight;		//	in kilograms
		Ogre::Vector3 position;	//	the position of the obstacle
};

#endif
