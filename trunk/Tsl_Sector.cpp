#include "Tsl_Sector.hpp"

Tsl_Sector::
	Tsl_Sector(string new_name): 
	Object(new_name)
{
	assert (Object::is_initialized ());
	
	assert (is_initialized ());

}

Tsl_Sector::~Tsl_Sector(){

}

//vector<Obstacle*> getObstacles(){
//	return obstacles;
//}