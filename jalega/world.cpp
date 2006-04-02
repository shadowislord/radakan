#include "world.hpp"

using namespace std;
//	using namespace boost;

//  constructor
World::
	World
	(string new_name):
	Object::
	Object
	(new_name)
{
	TiXmlDocument doc;
	bool success = doc.LoadFile ("data/" + * this + ".xml");
	assert (success);
	TiXmlHandle docHandle (& doc);
	TiXmlElement * image =
				docHandle.FirstChild ("world").FirstChild ("image").Element ();
	assert (image != NULL);
	image_path = image->Attribute ("path");
	assert (image_path != "");
	
	debug () << "Image path: " << image_path << endl;

	TiXmlElement * size =
				docHandle.FirstChild ("world").FirstChild ("size").Element ();
	assert (size != NULL);
	int temp_width = - 1;
	size->Attribute ("width", & temp_width);
	assert (0 < temp_width );
	width = (unsigned int) (temp_width);
	int temp_height = - 1;
	size->Attribute ("height", & temp_height);
	assert (0 < temp_height );
	height = (unsigned int) (temp_height);

	debug () << "Size: " << width << " " << height << endl;

	for (unsigned int i = 0; i < width; i++)
	{
		for (unsigned int j = 0; j < height; j++)
		{
			tile_vector.push_back
				(new Tile (* this + "_" + to_string (i) + "_" + to_string (j)));
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
//	delete tiles;
	for (unsigned int i = 0; i < tile_vector.size (); i++)
	{
		delete tile_vector.at (i);
	}
}

//	virtual
bool
	World::
	is_initialized
	()
	const
{
	return Object::is_initialized ();
}

Tile *
	World::
	get_tile
	(unsigned int x, unsigned int y)
	const
{
	assert (is_initialized ());

	return tile_vector.at (x * height + y);
//	return & tiles[x][y][0];	//	I've no idea why the [0] needs to be added.
}

unsigned int
	World::
	get_width
	()
	const
{
	assert (is_initialized ());

	return width;
}

unsigned int
	World::
	get_height
	()
	const
{
	assert (is_initialized ());

	return height;
}
