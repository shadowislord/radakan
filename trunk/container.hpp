#ifndef CONTAINER_HPP
#define CONTAINER_HPP

#include "weapon.hpp"

using namespace std;

class Container:
	public Entity
{
	public:
		Container
			(string new_name,
			bool new_movable,
			float new_volume,
			float new_weight,
			Ogre::Vector3 new_position);
		virtual ~Container ();
		virtual bool is_initialized () const;
		virtual float get_total_weight () const;
		virtual bool add (Entity * item);		//	true iff succesfull
		virtual bool remove (Entity * item);	//	true iff succesfull
		virtual bool contains (Entity * item) const;

	private:
		set <Entity *> items;
};

#endif
