#include "obstacle.hpp"

using namespace std;

//  Constructor
Obstacle::
	Obstacle (string new_name, D3 * new_position):
	Object::
	Object (new_name)
{
	assert (Object::is_initialized ());
	assert (new_position != NULL);

	position = new_position;

	assert (is_initialized ());
}

//  Destructor
Obstacle::
	~Obstacle ()
{
	assert (is_initialized ());

	delete position;
}

//	virtual
bool
	Obstacle::
	is_initialized
	()
	const
{
	return Object::is_initialized () && (position != NULL) &&
		position->is_initialized ();
}

//	virtual
void
	Obstacle::
	draw
	(SHR * shr, int left, int bottom)
	const
{
	assert (is_initialized ());

	float x = left + position->x;
	float y = bottom + position->y;

	//	a tiny house is rendered

	//	left wall
	shr->render_quad (
		new D3 (0.6, 0.6, 0.6),
		new D3 (x, y + 5, 0),
		new D3 (x + 10, y, 0),
		new D3 (x + 10, y + 10, 0),
		new D3 (x, y + 15, 0)
	);
	//	right wall
	shr->render_quad (
		new D3 (0.7, 0.7, 0.7),
		new D3 (x + 20, y + 5, 0),
		new D3 (x + 10, y, 0),
		new D3 (x + 10, y + 10, 0),
		new D3 (x + 20, y + 15, 0)
	);
	//	left roof
	shr->render_triangle (
		new D3 (0.8, 0, 0.1),
		new D3 (x + 10, y + 25, 0),
		new D3 (x, y + 15, 0),
		new D3 (x + 10, y + 10, 0)
	);
	//	right roof
	shr->render_triangle (
		new D3 (0.9, 0, 0.2),
		new D3 (x + 10, y + 25, 0),
		new D3 (x + 20, y + 15, 0),
		new D3 (x + 10, y + 10, 0)
	);
	//	door
	shr->render_quad (
		new D3 (0.6, 0.1, 0.1),
		new D3 (x + 14, y + 2, 0),
		new D3 (x + 16, y + 3, 0),
		new D3 (x + 16, y + 9, 0),
		new D3 (x + 14, y + 8, 0)
	);
}

void
	Obstacle::
	move
	(D3 * new_position)
{
	assert (is_initialized ());

	delete position;
	position = new_position;
}
