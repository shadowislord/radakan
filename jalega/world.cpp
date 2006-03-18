#include "world.hpp"

using namespace std;
//	using namespace boost;

//  Constructor
World::
	World (string new_name):
	Object::
	Object (new_name)
{
	TiXmlDocument doc ("data/" + * this + ".xml");
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
	string result_width = size->Attribute ("width", & width);
	assert (result_width != "");
	string result_height = size->Attribute ("height", & height);
	assert (result_height != "");

	debug () << "Size: " << width << " " << height << endl;

	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < height; j++)
		{
			tile_vector.push_back
				(new Tile (* this + "_" + to_string (i) + "_" + to_string (j)));
		}
	}
	
//	tiles = new multi_array_ref <Tile, 2>
//						(tile_vector.at (0), extents[new_width][new_height]);

	assert (is_initialized ());
}

//  Destructor
World::
	~World ()
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
	is_initialized ()
	const
{
	return Object::is_initialized ();
}

Tile *
	World::
	get_tile (int x, int y)
{
	assert (is_initialized ());

	return tile_vector.at (x * height + y);
//	return & tiles[x][y][0];	//	I've no idea why the [0] needs to be added.
}
