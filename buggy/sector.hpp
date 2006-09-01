#ifndef SECTOR_HPP
#define SECTOR_HPP

#include "entity.hpp"

using namespace std;

class Sector:
	public Object
{
	public:
		Sector (string new_name);
		virtual ~Sector ();

		set <Entity *> entities;	//	the items that are currently in this sector
};

#endif
