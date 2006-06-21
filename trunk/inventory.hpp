#ifndef INVENTRORY_HPP
#define INVENTRORY_HPP

#include "obstacle.hpp"

using namespace std;

class Inventory:
	public Object
{
	public:
		Inventory (string new_name);
		virtual ~Inventory ();
		bool add (Obstacle * item);
		bool fetch (Obstacle * item);
		bool contains (Obstacle * item);

	private:
		vector <Obstacle *> items;
};

#endif
