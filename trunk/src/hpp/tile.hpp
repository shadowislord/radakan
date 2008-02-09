#ifndef RADAKAN_TILE_HPP
#define RADAKAN_TILE_HPP

#include "location.hpp"

using namespace std;

class TiXmlDocument;
class TiXmlElement;

#if RADAKAN_PHYSICS_MODE == RADAKAN_OGREODE_MODE
	namespace OgreOde
	{
		class SimpleSpace;
	}
#endif

namespace Radakan
{
	template <class T> class Container;
	class Model;

	namespace Items
	{
		class Item;
		class Character;
	}
	
	///	A Tile is a square piece (64 m x 64 m) of the world.
	///	Tile data is stored under 'trunk/data/world/tile/'.
	class Tile :
		public Location <Model>
	{
		public :
			static string get_class_name ();
			
			static const int side_length;

			Tile
			(
				Mathematics :: Vector_3D new_coordinates
#if RADAKAN_PHYSICS_MODE == RADAKAN_OGREODE_MODE
				,
				boost :: shared_ptr <OgreOde :: SimpleSpace> new_space
#endif
			);
			virtual ~Tile ();
			virtual bool is_initialized () const;
			
			virtual bool add (Reference <Model> model);
			virtual bool move (Reference <Model> model, Reference <Container <Model> > destination);

			const Mathematics :: Vector_3D coordinates;
			const Mathematics :: Vector_3D position;

			Reference <Container <Items :: Character> > characters;

#if RADAKAN_PHYSICS_MODE == RADAKAN_OGREODE_MODE
			boost :: shared_ptr <OgreOde :: SimpleSpace> space;
#endif

		private :
			void load_model (TiXmlElement & element);
			Reference <Items :: Item> load_item (TiXmlDocument & document, string item_name);

			TiXmlDocument * doc;
	};
}

#endif	//	RADAKAN_TILE_HPP
