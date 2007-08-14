#ifndef RADAKAN_TILE_HPP
#define RADAKAN_TILE_HPP

#include <Ogre.h>
#if OGRE_VERSION_MINOR < 3
	#include <OgreKeyEvent.h>
	#include <OgreEventListeners.h>
#endif
#include <OgreStringConverter.h>
#include <OgreException.h>

#include "model.hpp"
#include "npc.hpp"

#include <tinyxml.h>

using namespace std;

namespace Radakan
{
	///	A Tile is a square piece of the world.
	///	Tile data is stored under trunk/data/tile/.
	class Tile :
		public Resident <Tile>,
		public Location <Model>,
		public OgreOde :: SimpleSpace
	{
		public :
			Tile (pair <int, int> new_coordinates);
			virtual ~Tile ();
			virtual bool is_initialized () const;
			
			static const string get_class_name ();
			
			virtual bool add (Model & model);
			virtual bool move (Model & model, Set <Model> & destination);

			const pair <int, int> coordinates;
			const Ogre :: Vector3 position;

			static const int side_length;

			Set <Items :: NPC> npcs;

		private :
			void load_xml (TiXmlElement & element);
			void load_xml_file (TiXmlDocument & document);
			Model & create_model (Items :: Item & item, Ogre :: Vector3 position, float scale);

			TiXmlDocument doc;
	};
}

#endif	//	RADAKAN_TILE_HPP
