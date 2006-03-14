#ifndef TILE_HPP
#define TILE_HPP

#include "object.hpp"

using namespace std;

class Tile:
	public Object	//	should be changed to GameObject once it's ready
{
	public:
		//	Constructor
		Tile ();
		//	Constructor
		Tile (string new_name);
		//	Destructor
		virtual ~Tile ();
		virtual bool is_initialized () const;
		bool has_object ();
		void set_object (Object * new_object);	//	remove the old one first
		void remove_object ();
		Object * get_object ();

	private:
		Object * object;	//	should be changed to GameObject once it's ready
};

#endif
