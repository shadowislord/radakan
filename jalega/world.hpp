#ifndef WORLD_HPP
#define WORLD_HPP

#include <boost/multi_array.hpp>
#include "tile.hpp"

using namespace std;
using namespace boost;

//	tiles are numbered like this:
//	0: (0, 0)	3: (1, 0)	6: (2, 0)
//	1: (0, 1)	4: (1, 1)	7: (2, 1)
//	2: (0, 2)	5: (1, 2)	8: (2, 2)

class World:
	public Object	//	should be changed to GameObject once it's ready
{
	public:
		World (string new_name, int new_width, int new_height);	//	Constructor
		virtual ~World ();										//	Destructor
		virtual bool is_initialized () const;
		Tile * get_tile (int x, int y);							//	zero based

	private:
		int width;
		int height;
		vector <Tile *> tile_vector;
//		multi_array_ref <Tile, 2>  * tiles;	//	not working tile_vector wrapper
};

#endif

