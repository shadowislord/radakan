#ifndef TSL_TILE_HPP
#define TSL_TILE_HPP

#include <Ogre.h>
#if OGRE_VERSION_MINOR < 3
	#include <OgreKeyEvent.h>
	#include <OgreEventListeners.h>
#endif
#include <OgreStringConverter.h>
#include <OgreException.h>

#include "body.hpp"
#include "npc.hpp"
#include "player.hpp"
#include "static_item.hpp"

#include <tinyxml.h>

using namespace std;

namespace tsl
{
	//	'typename' added to assure that const_iterator is a type.
	//	'class' would give MSV problems.
	typedef set <NPC *> :: const_iterator NPC_iterator;

	///	A Tile is a square piece of a sector.
	class Tile :
		public Disjoint_Set <Body>,
		public OgreOde :: SimpleSpace
	{
		public :
			Tile (pair <int, int> new_coordinates, string new_tsl_path);
			virtual ~Tile ();
			virtual bool is_initialized () const;
			static string get_class_name ();

			virtual bool add (Body & body);
			virtual bool move (Body & body, Disjoint_Set <Body> & destination);

			const pair <int, int> coordinates;
			const Ogre :: Vector3 position;

			static const int side_length;

			//	This can't be a Disjoint_Set,
			//	as the NPCs would be part of diffrent Disjoint_Sets.
			set <NPC *> npcs;

		private :
			void load_xml (TiXmlElement & element);
			void load_xml_file (TiXmlDocument & document);
			Body & create_body (Item & item, Ogre :: Vector3 position, float scale);

			const string tsl_path;

			TiXmlDocument doc;

	};
}

#endif	//	TSL_TILE_HPP
