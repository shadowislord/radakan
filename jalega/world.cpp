#include "world.hpp"

using namespace std;
//	using namespace boost;

//  constructor
World::
	World
	(string new_name, D3 new_position, D3 new_size):
	Location::
	Location
	(new_name, new_position, new_size)
{
	assert (Object::is_initialized ());

	cout << * this << endl;
	
	TiXmlDocument doc;
	bool success = doc.LoadFile ("data/" + * this + ".xml");
	assert (success);
	TiXmlHandle docHandle (& doc);
	TiXmlElement * image_xml =
				docHandle.FirstChild (* this).FirstChild ("image").Element ();
	assert (image_xml != NULL);
	image_path = image_xml->Attribute ("path");
	assert (image_path != "");

	image = new Texture (image_path);
//	image = new Texture ("fonts/test.tga");
	
	debug () << "Image path: " << image_path << endl;

	TiXmlElement * size =
				docHandle.FirstChild (* this).FirstChild ("size").Element ();
	assert (size != NULL);
	int temp_tile_width = - 1;
	size->Attribute ("tile_width", & temp_tile_width);
	assert (0 < temp_tile_width);
	tile_width = (unsigned int) (temp_tile_width);
	int temp_tile_height = - 1;
	size->Attribute ("tile_height", & temp_tile_height);
	assert (0 < temp_tile_height);
	tile_height = (unsigned int) (temp_tile_height);
	int temp_tile_size = - 1;
	size->Attribute ("tile_size", & temp_tile_size);
	assert (0 < temp_tile_size);
	tile_size = (unsigned int) (temp_tile_size);

	debug () << "Size: " << tile_width << " " << tile_height << endl;

	for (unsigned int i = 0; i < tile_width; i++)
	{
		for (unsigned int j = 0; j < tile_height; j++)
		{
			tiles.push_back
				(new Tile (* this + "_" + to_string (i) + "_" + to_string (j), *this + D3 (i * 25, j * 25, 0), D3(25, 25, 0)));
		}
	}
	
//	tiles = new multi_array_ref <Tile, 2>
//						(tile_vector.at (0), extents[new_width][new_height]);

	assert (is_initialized ());
}

//  destructor
World::
	~World
	()
{
	assert (is_initialized ());

//	delete tiles:
	for (unsigned int i = 0; i < tiles.size (); i++)
	{
		delete tiles.at (i);
	}
	
//	delete obstacles:
	for (unsigned int i = 0; i < obstacles.size (); i++)
	{
		delete obstacles.at (i);
	}
	
	delete image;
}

//	virtual
bool
	World::
	is_initialized
	()
	const
{
	return Object::is_initialized () && (image != NULL) &&
		image->is_initialized ();
}

Tile *
	World::
	get_tile
	(unsigned int x, unsigned int y)
	const
{
	assert (is_initialized ());

	return tiles.at (x * tile_height + y);
}

unsigned int
	World::
	get_width
	()
	const
{
	assert (is_initialized ());

	return tile_size * tile_width;
}

unsigned int
	World::
	get_height
	()
	const
{
	assert (is_initialized ());

	return tile_size * tile_height;
}
unsigned int
	World::
	get_tile_width
	()
	const
{
	assert (is_initialized ());

	return tile_width;
}

unsigned int
	World::
	get_tile_height
	()
	const
{
	assert (is_initialized ());

	return tile_height;
}

bool
	World::
	add_obstacle
	(Obstacle * new_obstacle)
{
	assert (is_initialized ());
	assert (new_obstacle->is_initialized ());
//	assert (D3 (0, 0, 0) <= * new_obstacle);
//	assert (* new_obstacle + new_obstacle->get_size () < get_size ());

	for (unsigned int i = 0; i < obstacles.size (); i++)
	{
		if (obstacles.at (i)->collides (new_obstacle))
		{
			return true;
		}
	}

	obstacles.push_back (new_obstacle);

	return false;
}
