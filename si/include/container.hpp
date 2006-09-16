#ifndef CONTAINER_HPP
#define CONTAINER_HPP

#include "weapon.hpp"

using namespace std;

///	Container holds the data of one in-game container.

class Container:
	public Entity
{
	public:
		Container
			(bool new_movable,
			bool new_visible,
			float new_volume,
			float new_weight,
			Ogre::Vector3 new_position,
			Ogre :: Entity * new_ogre_entity,
			Ogre :: SceneNode * new_node);
		virtual ~Container ();
		virtual bool is_initialized () const;
		virtual float get_total_weight () const;
		virtual bool add (Entity * item);		//	true iff succesfull

		///	Move item to new_container.
		virtual bool move_to (Entity * item, Container * new_container);
												//	true iff succesfull
		virtual bool contains (Entity * item) const;

	private:
		//	the items that are *directely* in this container
		set <Entity *> items;
};

#endif
