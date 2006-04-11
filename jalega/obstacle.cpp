#include "obstacle.hpp"

using namespace std;

//  Constructor
Obstacle::
	Obstacle (string new_name):
	Object::
	Object (new_name)
{
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
	is_initialized ()
	const
{
	return Object::is_initialized ();
}

//	virtual
void
	Obstacle::
	draw
	(unsigned int left, unsigned int top)
	const
{
	shr->render_quad (
		new D3 (0.7, 0.7, 0.7),
		new D3 (left, top, 0),
		new D3 (left + 10, top, 0),
		new D3 (left + 10, top + 10, 0),
		new D3 (left, top + 10, 0)
	);

}

