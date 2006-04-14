#ifndef WORLD_HPP
#define WORLD_HPP

#include "tile.hpp"
//	#include <boost/multi_array.hpp>
#include "tinyxml/tinyxml.h"

using namespace std;
//	using namespace boost;

//	tiles are numbered like this:
//	0: (0, 0)	3: (1, 0)	6: (2, 0)
//	1: (0, 1)	4: (1, 1)	7: (2, 1)
//	2: (0, 2)	5: (1, 2)	8: (2, 2)

class World:
	public Object
{
	public:
		World (string new_name);								//	constructor
		virtual ~World ();										//	destructor
		virtual bool is_initialized () const;
		unsigned int get_width () const;
		unsigned int get_height () const;
		Tile * get_tile (unsigned int x, unsigned int y) const;	//	zero based
		virtual void draw (SHR * shr, float left, float bottom) const;
		bool add_obstacle (Obstacle * new_obstacle);

	private:
		unsigned int width;
		unsigned int height;
		string image_path;
		vector <Tile *> tiles;
		vector <Obstacle *> obstacles;
		Texture * image;
//		multi_array_ref <Tile, 2>  * tiles;	//	not working tile_vector wrapper
};

#endif

