#ifndef RADAKAN_TILE_HPP
#define RADAKAN_TILE_HPP

#include "location.hpp"
#include "resident.hpp"

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
		class NPC;
	}
	
	///	A Tile is a square piece of the world.
	///	Tile data is stored under trunk/data/tile/.
	class Tile :
		public Resident <Tile>,
		public Location <Model>
	{
		public :
			static string get_class_name ();
			
			static const int side_length;

			Tile (pair <int, int> new_coordinates);
			virtual ~Tile ();
			virtual bool is_initialized () const;
			
			virtual bool add (Reference <Model> model);
			virtual bool move (Reference <Model> model, Reference <Set <Model> > destination);

			const pair <int, int> coordinates;
			const Ogre :: Vector3 position;

			Reference <Set <Items :: NPC> > npcs;

			boost :: shared_ptr <OgreOde :: SimpleSpace> space;

		private :
			void load_xml (boost :: shared_ptr <TiXmlElement> element);
			void load_xml_file (boost :: shared_ptr <TiXmlDocument> document);
			Reference <Model> create_model (Reference <Items :: Item> item, Ogre :: Vector3 position, float scale);

			boost :: shared_ptr <TiXmlDocument> doc;
	};
}

#endif	//	RADAKAN_TILE_HPP
