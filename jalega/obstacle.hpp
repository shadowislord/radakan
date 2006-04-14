#ifndef OBSTACLE_HPP
#define OBSTACLE_HPP

#include "shr.hpp"

using namespace std;

class Obstacle:
	public D3
{
	public:
		Obstacle (string new_name, D3 * new_position, float new_radius);
		virtual ~Obstacle ();
		virtual bool is_initialized () const;
		virtual void draw (SHR * hsr, float left, float bottom) const;
		void move (D3 * new_position);
		float get_radius () const;
		bool collides (Obstacle * other_obstacle) const;

	private:
		float radius;
};

#endif
