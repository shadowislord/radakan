#include "sector.hpp"

Sector::
	Sector (string new_name):
	Object (new_name)
{
	assert (Object :: is_initialized ());
	
	assert (is_initialized ());

}

Sector ::
	~Sector ()
{

}

//vector<Obstacle*> getObstacles(){
//	return obstacles;
//}
