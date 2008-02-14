#ifndef RADAKAN_WORLD_HPP
#define RADAKAN_WORLD_HPP

#include "singleton.hpp"

namespace Radakan
{
	namespace Items
	{
		class Item;
	}
	
	template <typename T, class U> class Map;
	class Tile;
	
	///	World contains all basic 'environment' data.
	class World :
		public Singleton <World>
	{
		public:
			///	These are the tile position limits, not the item position limits.
			static const int min_x;
			static const int max_x;
			static const int min_z;
			static const int max_z;
			
			static const float time_step;
			
			static string get_class_name ();
			
			static void create ();
			
		protected :
			World ();
			
		public :
			virtual ~World ();
			virtual bool is_initialized () const;
			
			virtual void update () = 0;
			
			///	'get_close_items (...)' returns all close models, but not the argument.
			Reference <Container <Items :: Item> > get_close_items
				(Reference <Items :: Item> by);
			
			boost :: scoped_ptr <Ogre :: SceneNode> root_node;
			
		protected :
			Reference <Map <Mathematics :: Vector_3D, Tile> > tiles;
	};
}

#endif	// RADAKAN_WORLD_HPP
