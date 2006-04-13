#include "tile.hpp"

using namespace std;

//  constructor
Tile::
	Tile (string new_name):
	Object::
	Object (new_name)
{
	assert (Object::is_initialized ());
	
	assert (is_initialized ());
}

//  destructor
Tile::
	~Tile ()
{
	assert (is_initialized ());
}

//	virtual
void
	Tile::
	draw
	(SHR * shr, int left, int bottom)
	const
{
	shr->render_quad (
		new D3 (0.2, 0.5, 0.2),
		new D3 (left, bottom, 0),
		new D3 (left + 24, bottom, 0),
		new D3 (left + 24, bottom + 24, 0),
		new D3 (left, bottom + 24, 0)
	);
}
