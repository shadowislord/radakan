#include "character.hpp"

using namespace std;

//  Constructor
Character::
	Character (string new_name, D3 * new_position):
	Obstacle::
	Obstacle (new_name, new_position)
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
	(SHR * shr, int left, int bottom)
	const
{
	assert (is_initialized ());
	
	float x = left + position->x;
	float y = bottom + position->y;
	
	//	a tiny grey character is rendered

	//	head
	shr->render_quad (
		new D3 (0.9, 0.5, 0.5),
		new D3 (x + 10, y + 16, 0),
		new D3 (x + 8, y + 14, 0),
		new D3 (x + 10, y + 11, 0),
		new D3 (x + 12, y + 14, 0)
	);
	//	arms
	shr->render_quad (
		new D3 (0.6, 0.6, 0.6),
		new D3 (x + 2, y + 10, 0),
		new D3 (x + 2, y + 12, 0),
		new D3 (x + 18, y + 12, 0),
		new D3 (x + 18, y + 10, 0)
	);
	//	left belly and leg
	shr->render_quad (
		new D3 (0.6, 0.6, 0.6),
		new D3 (x + 7, y + 10, 0),
		new D3 (x + 13, y + 10, 0),
		new D3 (x + 6, y, 0),
		new D3 (x + 8, y, 0)
	);
	//	right belly and leg
	shr->render_quad (
		new D3 (0.5, 0.5, 0.5),
		new D3 (x + 10, y + 5, 0),
		new D3 (x + 13, y + 10, 0),
		new D3 (x + 14, y, 0),
		new D3 (x + 12, y, 0)
	);
}

