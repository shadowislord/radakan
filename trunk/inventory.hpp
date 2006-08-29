#ifndef INVENTRORY_HPP
#define INVENTRORY_HPP

#include "weapon.hpp"

using namespace std;

class Inventory:
	public Object
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
