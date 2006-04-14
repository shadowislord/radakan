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
	assert (Object::is_initialized ());
	
	TiXmlDocument doc;
	bool success = doc.LoadFile ("data/" + * this + ".xml");
	assert (success);
	TiXmlHandle docHandle (& doc);
	TiXmlElement * image_xml =
				docHandle.FirstChild ("world").FirstChild ("image").Element ();
	assert (image_xml != NULL);
	image_path = image_xml->Attribute ("path");
	assert (image_path != "");

	image = new Texture (image_path);
//	image = new Texture ("fonts/test.tga");
	
	debug () << "Image path: " << image_path << endl;

	TiXmlElement * size =
				docHandle.FirstChild ("world").FirstChild ("size").Element ();
	assert (size != NULL);
	int temp_width = - 1;
	size->Attribute ("width", & temp_width);
	assert (0 < temp_width);
	width = (unsigned int) (temp_width);
	int temp_height = - 1;
	size->Attribute ("height", & temp_height);
	assert (0 < temp_height);
	height = (unsigned int) (temp_height);

	debug () << "Size: " << width << " " << height << endl;

	for (unsigned int i = 0; i < width; i++)
	{
		for (unsigned int j = 0; j < height; j++)
		{
			tiles.push_back
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

	return tiles.at (x * height + y);
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

//	virtual
void
	World::
	draw
	(SHR * shr, float left, float bottom)
	const
{
	assert (is_initialized ());

	for (unsigned int i = 0; i < get_width (); i++)
	{
//		make sure tiles in the back get draw first:
		for (unsigned int j = 0; j < get_height (); j++)
		{
			get_tile (i, j)->draw (shr, left + 25 * i, bottom + 25 * j);
		}
	}
	
	for (unsigned int i = 0; i < obstacles.size (); i++)
	{
		obstacles.at (i)->draw (shr, left, bottom);
	}
}

bool
	World::
	add_obstacle
	(Obstacle * new_obstacle)
{
	assert (is_initialized ());
	assert (new_obstacle->is_initialized ());
//	assert (new_obstacle->get_x () + new_obstacle->get_radius () <= width);
//	assert (new_obstacle->get_y () + new_obstacle->get_radius () <= height);


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
