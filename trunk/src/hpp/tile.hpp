#ifndef RADAKAN_TILE_HPP
#define RADAKAN_TILE_HPP

#include "location.hpp"

using namespace std;

class TiXmlDocument;
class TiXmlElement;

namespace OgreOde
{
	class SimpleSpace;
}

namespace Radakan
{
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
				pair <int, int> new_coordinates,
				boost :: shared_ptr <OgreOde :: SimpleSpace> new_space
			);
			virtual ~Tile ();
			virtual bool is_initialized () const;
			
			virtual bool add (Reference <Model> model);
			virtual bool move (Reference <Model> model, Reference <Container <Model> > destination);

			const pair <int, int> coordinates;
			const Ogre :: Vector3 position;

			Reference <Set <Items :: Character> > characters;

			boost :: shared_ptr <OgreOde :: SimpleSpace> space;

		private :
			void load_model (TiXmlElement & element);
			Reference <Items :: Item> load_item (TiXmlDocument & document, string item_name);

			TiXmlDocument * doc;
	};
}

#endif	//	RADAKAN_TILE_HPP
