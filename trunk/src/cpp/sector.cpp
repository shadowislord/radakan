#include "sector.hpp"

using namespace std;
using namespace tsl;

Sector ::
	Sector
	(
		string new_name,
		Ogre :: SceneManager & scene_manager
	) :
	Object (new_name),
	OgreOde :: World (& scene_manager),
	camera (* getSceneManager () -> createCamera (new_name + "'s camera"))
{
	trace () << "Sector (" << new_name << ", ~scene_manager~)" << endl;
	assert (Object :: is_initialized ());

	camera . setNearClipDistance (5);
	camera . setFarClipDistance (2000);

	setShowDebugGeometries (true);

	setGravity (Ogre :: Vector3 (0, - 9.81, 0));

	//	TODO make the next line work.
	getSceneManager () -> setSkyDome (true, "Peaceful", 10, 5);

	//	the ground
	Item & ground = represent (Item :: create ("Ground", "test3-ground.mesh", 0, 0, false), 1000, - 50, 0);
	ground . get_representation () . get_entity () . setMaterialName ("TavernWalls");
	//	to cancel out gravity...
	ground . get_representation () . addForce (Ogre :: Vector3 (0, 9.81, 0));
	
	//	the player
	if (! Player :: is_instantiated ())
	{
		represent (Player :: create ("Player", "ninja.mesh", 80, 65), 100, 0, 200, 0.1);
		assert (Player :: get () . has_representation ());
	}

	represent (NPC :: create ("Ninja (" + * this + ")", "ninja.mesh", 80, 65), 120, 0, 30, 0.1);

	//	the tavern
	represent (Item :: create ("Tavern", "tavern.mesh", 0, 0, false), 0, 0, 0);
	represent (Item :: create ("Bar", "bar.mesh", 0, 0, false), 0, 0, 0);
	represent (Item :: create ("Table 1", "table.mesh", 0, 0), 116, 0, 17);
	represent (Item :: create ("Table 2", "table.mesh", 0, 0), 116, 0, 57);
	represent (Item :: create ("Table 3", "table.mesh", 0, 0), 26, 0, 97);

	//	'table2.mesh' is not textured yet.

	//	These fences are not textured yet.
	represent (Item :: create ("Fence 1", "fences1.mesh", 0, 0, false), 126, 0, - 197, 0.3);
	represent (Item :: create ("Fence 2", "fences2.mesh", 0, 0, false), 226, 0, - 297, 0.3);
	represent (Item :: create ("Fence 3", "fences3.mesh", 0, 0, false), 326, 0, - 397, 0.3);
	represent (Item :: create ("Fence 4", "fences4.mesh", 0, 0, false), 426, 0, - 497, 0.3);
	represent (Item :: create ("Fence 5", "fences5.mesh", 0, 0, false), 526, 0, - 597, 0.3);

	#ifndef TSL_TRACE
		//	not textured
		represent (Item :: create ("House", "house.mesh", 0, 0, false), 800, 0, 200, 0.4);
		represent (Item :: create ("Wagon", "wagon.mesh", 0, 0), - 500, 0, - 500, 0.02);
		represent (Item :: create ("Pot", "pot.mesh", 0, 0), - 550, 0, - 500, 0.2);
		represent (Item :: create ("Pot 2", "pot_2.mesh", 0, 0), - 600, 0, - 500, 0.2);
		represent (Item :: create ("Pine tree", "pine_tree_2.mesh", 0, 0, false), - 700, 35, - 700, 1.5);

		//	forest of 1961 trees
		//	(30000 trees takes to long to load)
		Item * temp_tree = NULL;
		for (int i = - 25; i <= 25; i++)
		{
			for (int j = int (- sqrt (625 - i * i)); j <= sqrt (625 - i * i); j++)
			{
				if (0.4 < Ogre :: Math :: RangeRandom (0, 1))
				{
					temp_tree = & represent
					(
						Item :: create
						(
							"Tree no. " + to_string (50 * i + j),
							"tree.mesh",
							0,
							0,
							false
						),
						- 1500 + 100 * i + Ogre :: Math :: RangeRandom (- 130, 130),
						0,
						3000 + 100 * j + Ogre :: Math :: RangeRandom (- 130, 130),
						1.5
					);
					temp_tree -> get_representation () . turn (- Ogre :: Math :: HALF_PI, Ogre :: Vector3 (1, 0, 0));
				}
				else
				{
					represent
					(
						Item :: create
						(
							"Tree no. " + to_string (50 * i + j),
							"pine_tree.mesh",
							0,
							0,
							false
						),
						- 1500 + 100 * i + Ogre :: Math :: RangeRandom (- 130, 130),
						96,
						3000 + 100 * j + Ogre :: Math :: RangeRandom (- 130, 130),
						1.5
					);
				}
			}
		}
	#endif

	assert (is_initialized ());
}

Sector ::
	~Sector ()
{
	trace () << "~" << get_class_name () << " ()" << endl;
	assert (is_initialized ());
}

//	virtual
bool Sector ::
	is_initialized ()
	const
{
	assert (warn <Sector> (Object :: is_initialized ()));

	#ifdef TSL_TRACE
		for (Item * i = get_child (); i != NULL; i = get_another_child ())
		{
			assert (i -> has_representation ());
		}
	#endif

	return true;
}

//	static
string Sector ::
	get_class_name ()
{
	return "Sector";
}

//	virtual
bool Sector ::
	add (Item & item)
{
	assert (is_initialized ());
	assert (item . is_initialized ());
	assert (! item . has_representation ());

	bool check = Disjoint_Set <Item> :: add (item);
	assert (check);

	Ogre :: SceneNode & node = * getSceneManager () -> getRootSceneNode () ->
													createChildSceneNode ();

	node . attachObject
		(getSceneManager () -> createEntity (item + "'s entity", item . mesh_name));
	
	assert (node . numAttachedObjects () == 1);
	assert (node . getAttachedObject (0) != NULL);
	item . add_representation (node, this);
	
	if (item . is_type <NPC> ())
	{
		trace () << item << " will be added to the list of NPCs..." << endl;
		bool check = npcs . insert (& item . to_type <NPC> ()) . second;
		assert (check);
		
		trace () << item << " was added to the list of NPCs." << endl;
	}
	
	return true;
}

//	virtual
bool Sector ::
	move (Item & item, Disjoint_Set <Item> & destination)
{
	trace () << "move (" << item << ", " << destination << ")" << endl;
	assert (is_initialized ());
	assert (item . is_initialized ());
	assert (item . has_representation ());
	assert (destination . is_initialized ());
	
	item . remove_representation ();

	assert (! getSceneManager () -> hasEntity (Player :: get ()));
	
	if (item . is_type <NPC> ())
	{
		npcs . erase (& item . to_type <NPC> ());
	}

	bool check = Disjoint_Set <Item> :: move (item, destination);
	assert (check);

	return true;
}

Ogre :: Camera & Sector ::
	get_camera ()
	const
{
	assert (is_initialized ());

	return camera;
}

Item & Sector ::
	represent (Item & item, float x, float y, float z, float scale)
{
	assert (is_initialized ());
	
	add (item);

	item . get_representation () . setPosition (Ogre :: Vector3 (x, y, z));
	item . get_representation () . set_scale (scale);

	return item;
}

//	static
Sector & Sector ::
	create 
	(
		string new_name,
		Ogre :: SceneManager & new_scene_manager
	)
{
	return * (new Sector (new_name, new_scene_manager));
}
