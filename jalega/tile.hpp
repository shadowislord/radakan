#ifndef TILE_HPP
#define TILE_HPP

#include "obstacle.hpp"

using namespace std;

class Tile:
	public Object
{
	public:
		Tile ();										//	Constructor
		Tile (string new_name);							//	Constructor
		virtual ~Tile ();								//	Destructor
		virtual bool is_initialized () const;
		bool has_obstacle ();
		void set_obstacle (Obstacle * new_obstacle);	//	has to be empty
		void remove_obstacle ();						//	has to have one
		Obstacle * get_obstacle ();						//	has to have one

	private:
		Obstacle * obstacle;
};

#endif
