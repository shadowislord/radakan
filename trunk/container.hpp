#ifndef CONTAINER_HPP
#define CONTAINER_HPP

#include "weapon.hpp"

using namespace std;

class Container:
	public Obstacle
{
	public:
		Container (string new_name);
		virtual ~Container ();
		virtual float get_total_weight () const;
		bool add (Obstacle * item);
		bool remove (Obstacle * item);
		bool contains (Obstacle * item);

	private:
		set <Obstacle *> items;
};

#endif
