#include "collision_engine.hpp"

Collision_Engine::
	Collision_Engine(string new_name):
	Object (new_name)
{
	assert (Object::is_initialized ());
	
	assert (is_initialized ());
}

Obstacle* Collision_Engine::check_Collision(Obstacle* e){
	/*
	Sector* s = getSector(o->position);
	Obstacle* obstacles s->getObstacles();
	for(int i = 0; i < Obstacles.size(); i++){
		if(has_Collided(o, obstacles[i]));
			return obstacles[i];
	}
	*/
	return 0;
}

void Collision_Engine::handle(Obstacle* hit, Obstacle* hitter){
	//alert the AI that the obstacle was hit, and handle any physical movement as a result of colliding.
}

void Collision_Engine::handle_Collision(){
	/*
	for(int i = 0; i < allobstacles.size(); i++){
		if(allobstacles[index].isMoving());
		handle(hasCollided(allobstacles[i], allobstacles[i]), allobstacles[i]);
	}
	*/
}

bool Collision_Engine::has_Collided(Obstacle* a, Obstacle* b){
	//calculate if bounding box from a intersects with bounding box from b
	return false;
}

Collision_Engine::~Collision_Engine(){

}