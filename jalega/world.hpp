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
		Tile * get_tile (unsigned int x, unsigned int y);		//	zero based

	private:
		unsigned int width;
		unsigned int height;
		string image_path;
		vector <Tile *> tile_vector;
//		multi_array_ref <Tile, 2>  * tiles;	//	not working tile_vector wrapper
};

#endif

