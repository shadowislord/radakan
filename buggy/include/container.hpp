#ifndef CONTAINER_HPP
#define CONTAINER_HPP

#include "weapon.hpp"
#include "tree.hpp"

using namespace std;

///	Container holds the data of one in-game container.

class Container:
	public Tree <Entity>
{
	public:
		Container (string new_name);
		Container (Entity * new_entity);
		virtual ~Container ();
		virtual bool is_initialized () const;
		virtual float get_total_weight () const;
		virtual Container * add (Entity * entity);	//	returns subtree, iff succes
};

#endif
