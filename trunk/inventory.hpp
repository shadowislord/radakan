#ifndef INVENTRORY_HPP
#define INVENTRORY_HPP

// Dude where is this file!!
//#include "weapon.hpp"

#include "object.hpp"
#include "obstacle.hpp"

using namespace std;

class Inventory:public Object
{
	public:
		Inventory (string new_name);
		virtual ~Inventory ();
		bool add (Obstacle * item);
		bool remove (Obstacle * item);
		bool contains (Obstacle * item);

	private:
		int find (Obstacle * item);
		vector <Obstacle *> items;
};

#endif
