#ifndef OBSTACLE_HPP
#define OBSTACLE_HPP

#include "sl_entity.hpp"

using namespace std;

class Obstacle:
	public Sl_Entity
{
	public:
		Obstacle
			(string new_name,
			Vector3 new_size,
			bool new_movable,
			float new_volume,
			float new_weight);
		virtual ~Obstacle ();
		virtual bool is_initialized () const;
		const bool movable;		//	false means that the object is static
		const float volume;		//	in litres
		const float weight;		//	in kilograms
};

#endif
