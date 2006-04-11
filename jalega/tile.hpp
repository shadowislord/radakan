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
		bool has_obstacle () const;
		void set_obstacle (Obstacle * new_obstacle);	//	has to be empty
		void remove_obstacle ();						//	has to have one
		Obstacle * get_obstacle () const;				//	has to have one
		virtual void draw (unsigned int left, unsigned int top) const;

	private:
		Obstacle * obstacle;
};

#endif
