#ifndef OBSTACLE_HPP
#define OBSTACLE_HPP

#include "object.hpp"
#include <OgreSceneNode.h>
#include <OgreEntity.h>

using namespace std;
//	Don't use namespace Ogre here!

///	Entity is the generic class that holds all data about one in-game entity.

class Entity:
	public Object
{
	public:
		Entity
			(bool new_movable,
			bool new_solid,
			bool new_visible,
			float new_volume,
			float new_weight,
			Ogre :: Vector3 new_position,
			Ogre :: Entity * new_ogre_entity,
			Ogre :: SceneNode * new_node);
		virtual ~Entity ();
		virtual bool is_initialized () const;
		virtual float get_total_weight () const;
		
		const bool movable;		//	false means that the object is static
		const bool solid;		//	false means that you can pass through it
		const bool visible;
		const float volume;		//	in litres
		const float weight;		//	in kilograms
		Ogre :: Vector3 position;	//	the position of the obstacle
		Ogre :: SceneNode * node;	//	create through Scene_Manager
		Ogre :: Entity * ogre_entity;	//	create through Scene_Manager
};

#endif
