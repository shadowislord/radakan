#ifndef TSL_TILE_HPP
#define TSL_TILE_HPP

#include <Ogre.h>
#if OGRE_VERSION_MINOR < 3
	#include <OgreKeyEvent.h>
	#include <OgreEventListeners.h>
#endif
#include <OgreStringConverter.h>
#include <OgreException.h>

#include "npc.hpp"
#include "player.hpp"

#include <tinyxml.h>

using namespace std;

namespace tsl
{
	//	'typename' added to assure that const_iterator is a type.
	//	'class' would give MSV problems.
	typedef set <NPC *> :: const_iterator NPC_iterator;

	///	A Tile is a square piece of a sector.
	class Tile :
		public Disjoint_Set <Item>
	{
		public :
			Tile (OgreOde :: World & new_world, pair <int, int> new_coordinates, string tsl_path);
			virtual ~Tile ();
			virtual bool is_initialized () const;
			static string get_class_name ();

			///	Don't use 'add' directely, use 'represent' instead.
			virtual bool add (Item & item);
			virtual bool move (Item & item, Disjoint_Set <Item> & destination);

			const pair <int, int> coordinates;
			const Ogre :: Vector3 position;

			static const int side_length;

			//	This can't be a Disjoint_Set,
			//	as the NPCs would be part of diffrent Disjoint_Sets.
			set <NPC *> npcs;

		private :
			Representation & represent
					(Item & item, float x, float y, float z, float scale = 1);
			void add_xml (TiXmlElement & element);

			OgreOde :: World & world;

			TiXmlDocument * doc;
	};
}

#endif	//	TSL_TILE_HPP
