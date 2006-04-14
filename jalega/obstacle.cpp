#include "obstacle.hpp"

using namespace std;

//  Constructor
Obstacle::
	Obstacle (string new_name, D3 * new_position, float new_radius):
	D3::
	D3 (new_name, new_position)
{
	assert (D3::is_initialized ());
	assert (new_position != NULL);
	assert (0 < new_radius);

	radius = new_radius;

	assert (is_initialized ());
}

//  Destructor
Obstacle::
	~Obstacle ()
{
	assert (is_initialized ());
}

//	virtual
bool
	Obstacle::
	is_initialized
	()
	const
{
	return Object::is_initialized () && (0 < radius);
}

//	virtual
void
	Obstacle::
	draw
	(SHR * shr, float left, float bottom)
	const
{
	assert (is_initialized ());

	float a = left + get_x ();
	float b = bottom + get_y ();

	//	a tiny house is rendered

	//	left wall
	shr->render_quad (
		new D3 (0.6, 0.6, 0.6),
		new D3 (a, b + 5, 0),
		new D3 (a + 10, b, 0),
		new D3 (a + 10, b + 10, 0),
		new D3 (a, b + 15, 0)
	);
	//	right wall
	shr->render_quad (
		new D3 (0.7, 0.7, 0.7),
		new D3 (a + 20, b + 5, 0),
		new D3 (a + 10, b, 0),
		new D3 (a + 10, b + 10, 0),
		new D3 (a + 20, b + 15, 0)
	);
	//	left roof
	shr->render_triangle (
		new D3 (0.8, 0, 0.1),
		new D3 (a + 10, b + 25, 0),
		new D3 (a, b + 15, 0),
		new D3 (a + 10, b + 10, 0)
	);
	//	right roof
	shr->render_triangle (
		new D3 (0.9, 0, 0.2),
		new D3 (a + 10, b + 25, 0),
		new D3 (a + 20, b + 15, 0),
		new D3 (a + 10, b + 10, 0)
	);
	//	door
	shr->render_quad (
		new D3 (0.6, 0.1, 0.1),
		new D3 (a + 14, b + 2, 0),
		new D3 (a + 16, b + 3, 0),
		new D3 (a + 16, b + 9, 0),
		new D3 (a + 14, b + 8, 0)
	);
}

void
	Obstacle::
	move
	(D3 * new_position)
{
	assert (is_initialized ());

	x = new_position->get_x ();
	y = new_position->get_y ();
	z = new_position->get_z ();

	delete new_position;
}

float
	Obstacle::
	get_radius
	()
	const
{
	assert (is_initialized ());

	return radius;
}

bool
	Obstacle::
	collides
	(Obstacle * other_obstacle)
	const
{
	assert (is_initialized ());

	return (distance (other_obstacle) <	get_radius () +
		other_obstacle->get_radius ());
}
