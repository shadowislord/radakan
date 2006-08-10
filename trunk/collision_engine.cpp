#include "collision_engine.hpp"

using namespace Ogre;

Collision_Engine::
	Collision_Engine(string new_name):
	Object (new_name)
{
	assert (Object::is_initialized ());
	
	assert (is_initialized ());
}

Obstacle* Collision_Engine::check_Collision(Obstacle* e)
{
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

void Collision_Engine::handle(Obstacle* hit, Obstacle* hitter)
{
	//alert the AI that the obstacle was hit, and handle any physical movement as a result of colliding.
}

void Collision_Engine::handle_Collision()
{
	/*
	for(int i = 0; i < allobstacles.size(); i++){
		if(allobstacles[index].isMoving());
		handle(hasCollided(allobstacles[i], allobstacles[i]), allobstacles[i]);
	}
	*/
}

bool Collision_Engine::has_Collided(Obstacle* a, Obstacle* b)
{
	//	a quite accurate guess
	return ((a->position - b->position).squaredLength () <=
		a->size.squaredLength () + b->size.squaredLength ());

//	Believe it or not, but the ASCII art below represents a cube.
//	Notice the order of the vertices, don't change it.
//
//		   ___,-1_
//		7='     | \
//		| \_   _|_,=6
//		|   5-' |   |
//		|  _|_,-4_  |
//		3=' |     \ |
//		  \_|  ___,=2
//		    0-'

	vector <Vector3> a_v;	//	a_vertices
	a_v.push_back (a->position + a->size);
	a_v.push_back (a->position - a->size);
	a_v.push_back (Vector3 (a_v.at (0).x, a_v.at (0).y, a_v.at (1).z));
	a_v.push_back (Vector3 (a_v.at (0).x, a_v.at (1).y, a_v.at (0).z));
	a_v.push_back (Vector3 (a_v.at (0).x, a_v.at (1).y, a_v.at (1).z));
	a_v.push_back (Vector3 (a_v.at (1).x, a_v.at (0).y, a_v.at (0).z));
	a_v.push_back (Vector3 (a_v.at (1).x, a_v.at (0).y, a_v.at (1).z));
	a_v.push_back (Vector3 (a_v.at (1).x, a_v.at (1).y, a_v.at (0).z));

	for (int i = 0; i < 6; i++)	//	face of a
	{
		for (int j = 0; j < 6; j++)	//	face of b
		{
			for (int k = 0; k < 4; k++)	//	edge of face of b
			{
				Vector3 b1;
				Vector3 b2;
				if (j < 2)
				{
					if (k < 2)
					{
						b1 = Vector3 (2 * k - 3, 2 * j - 3, 1);
						b2 = Vector3 (2 * k - 3, 2 * j - 3, - 1);
					}
					else
					{
						b1 = Vector3 (1, 2 * j - 3, 2 * k - 3);
						b2 = Vector3 (- 1, 2 * j - 3, 2 * k - 3);
					}
				}
				else if (j < 4)
				{
					if (k < 2)
					{
						b1 = Vector3 (2 * k - 3, 1, 2 * j - 3);
						b2 = Vector3 (2 * k - 3, - 1, 2 * j - 3);
					}
					else
					{
						b1 = Vector3 (1, 2 * k - 3, 2 * j - 3);
						b2 = Vector3 (- 1, 2 * k - 3, 2 * j - 3);
					}
				}
				else
				{
					if (k < 2)
					{
						b1 = Vector3 (2 * j - 3, 2 * k - 3, 1);
						b2 = Vector3 (2 * j - 3, 2 * k - 3, - 1);
					}
					else
					{
						b1 = Vector3 (2 * j - 3, 1, 2 * k - 3);
						b2 = Vector3 (2 * j - 3, - 1, 2 * k - 3);
					}
				}

				map <int, Real> w;
				
				for (int l = 0; l < 8; l++)
				{
					for (int m = 0; m < 8; m++)
					{
						int w_[l + 8 * m] =
							(a_v.at (l) - a_v.at (m)).dotProduct
								(b1.crossProduct (b2))
							+ (b1 - b2).dotProduct
								(a_v.at (l).crossProduct (a_v.at (m)));
					}
				}
			}
		}
	}
	
	return false;
}

Collision_Engine::~Collision_Engine()
{

}
