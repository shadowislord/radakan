#ifndef SECTOR_HPP
#define SECTOR_HPP

#include "obstacle.hpp"

using namespace std;

class Sector:
	public Object
{
	public:
		Sector(string new_name);
		virtual ~Sector();
		//vector<Obstacle*> getObstacles();

		vector<Obstacle*> obstacles; //the obstacles that are currently in this sector
};

#endif
