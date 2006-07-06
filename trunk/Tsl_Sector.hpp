

#ifndef TSL_SECTOR_HPP
#define TSL_SECTOR_HPP

#include "object.hpp"
#include "obstacle.hpp"

using namespace std;

class Tsl_Sector:
	public Object
{
	public:
		Tsl_Sector(string new_name);
		virtual ~Tsl_Sector();
		//vector<Obstacle*> getObstacles();

		vector<Obstacle*> obstacles; //the obstacles that are currently in this sector
};

#endif