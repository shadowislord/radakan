//Collision Engine

#ifndef COLLISION_ENGINE_HPP
#define COLLISION_ENGINE_HPP

#include "object.hpp"
#include "obstacle.hpp"

using namespace std;

class Collision_Engine:
	public Object
{
	public:
		Collision_Engine(string new_name);
		virtual ~Collision_Engine();
		Obstacle* check_Collision(Obstacle* o);
		bool has_Collided(Obstacle* a, Obstacle* b);
		void handle_Collision();
		void handle(Obstacle* hit, Obstacle* hitter);
};

#endif
