#ifndef CONTAINER_HPP
#define CONTAINER_HPP

#include "weapon.hpp"
#include "set.hpp"

using namespace std;

namespace sl
{
	///	Container holds the data of one in-game container.

	class Container :
		public Entity,
		public Set <Entity>
	{
		public :
			Container
				(Ogre :: SceneNode * new_node);
			Container
				(bool new_movable,
				bool new_solid,
				bool new_visible,
				float new_volume,
				float new_weight,
				Ogre :: Vector3 new_position,
				Ogre :: SceneNode * new_node);
			virtual ~Container ();
			virtual bool is_initialized () const;
			virtual float get_total_weight () const;
			virtual bool add (Entity * entity);
	};
}

#endif
