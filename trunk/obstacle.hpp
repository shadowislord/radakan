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
		
		//	this is useful for container system
		virtual bool is_initialized () const;
		
		virtual float get_total_weight () const;
		const bool movable;		//	false means that the object is static
		const float volume;		//	in litres
		const float weight;		//	in kilograms
};

#endif
