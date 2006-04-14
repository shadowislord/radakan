#include "character.hpp"

using namespace std;

//  Constructor
Character::
	Character (string new_name, D3 * new_position, float new_radius):
	Obstacle::
	Obstacle (new_name, new_position, new_radius)
{
	assert (Obstacle::is_initialized ());
	//	empty
	assert (is_initialized ());
}

//  Destructor
Character::
	~Character ()
{
	assert (is_initialized ());
}

//	virtual
void
	Character::
	draw
	(SHR * shr, float left, float bottom)
	const
{
	assert (is_initialized ());
	
	float a = left + get_x ();
	float b = bottom + get_y ();
	
	//	a tiny grey character is rendered

	//	head
	shr->render_quad (
		new D3 (0.9, 0.5, 0.5),
		new D3 (a + 10, b + 16, 0),
		new D3 (a + 8, b + 14, 0),
		new D3 (a + 10, b + 11, 0),
		new D3 (a + 12, b + 14, 0)
	);
	//	arms
	shr->render_quad (
		new D3 (0.6, 0.6, 0.6),
		new D3 (a + 2, b + 10, 0),
		new D3 (a + 2, b + 12, 0),
		new D3 (a + 18, b + 12, 0),
		new D3 (a + 18, b + 10, 0)
	);
	//	belly and left leg
	shr->render_quad (
		new D3 (0.6, 0.6, 0.6),
		new D3 (a + 7, b + 10, 0),
		new D3 (a + 13, b + 10, 0),
		new D3 (a + 6, b, 0),
		new D3 (a + 8, b, 0)
	);
	//	right leg
	shr->render_quad (
		new D3 (0.5, 0.5, 0.5),
		new D3 (a + 10, b + 5, 0),
		new D3 (a + 13, b + 10, 0),
		new D3 (a + 14, b, 0),
		new D3 (a + 12, b, 0)
	);
}

