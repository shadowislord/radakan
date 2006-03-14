#ifndef WORLD_HPP
#define WORLD_HPP

#include <boost/multi_array.hpp>
#include "tile.hpp"

using namespace std;
using namespace boost;

class World:
	public Object	//	should be changed to GameObject once it's ready
{
	public:
		World (string new_name, int new_width, int new_height);	//	Constructor
		virtual ~World ();										//	Destructor
		virtual bool is_initialized () const;
		Tile get_tile (int x, int y);							//	zero based

	private:
		multi_array <Tile, 2>  * tiles;
};

#endif

