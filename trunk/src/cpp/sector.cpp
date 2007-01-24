#include "sector.hpp"

using namespace std;
using namespace tsl;

Sector ::
	Sector
	(
		string new_name,
		Ogre :: SceneManager & new_scene_manager
	) :
	Object (new_name),
	btDiscreteDynamicsWorld
	(
		new btCollisionDispatcher (),
		new btSimpleBroadphase (),
		new btSequentialImpulseConstraintSolver ()
	),
	scene_manager (new_scene_manager),
	camera (* scene_manager . createCamera (new_name + "'s camera"))
{
	trace () << "Sector (" << new_name << ", ~new_scene_manager~)" << endl;
	assert (Object :: is_initialized ());

	camera . setNearClipDistance (5);
	camera . setFarClipDistance (2000);

	//	TODO: Make the next line work.
	scene_manager . setSkyDome (true, "Peaceful", 10, 5);

	//	the ground
	Item & ground = represent (Item :: create ("Ground", "plane.mesh", 0, 0), 1, 0, 0, 0);
	dynamic_cast <Ogre :: Entity *> (ground . get_representation () . node . getAttachedObject (0)) -> setMaterialName ("TavernWalls");
	ground . get_representation () . node . setOrientation (Ogre :: Quaternion (Ogre :: Radian (- Ogre :: Math :: HALF_PI), Ogre :: Vector3 :: UNIT_X));

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
					temp_tree -> get_representation () . node . setDirection (0, - 1, 0);
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

	Ogre :: SceneNode & node = * scene_manager . getRootSceneNode () ->
													createChildSceneNode ();

	node . attachObject (scene_manager . createEntity (item, item . mesh_name));
	
	assert (node . numAttachedObjects () == 1);
	assert (node . getAttachedObject (0) != NULL);
	item . add_representation (node);
	
	addRigidBody (& item . get_representation ());

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
	
	removeRigidBody (& item . get_representation ());

	item . remove_representation ();
	
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

Ogre :: SceneManager & Sector ::
	get_scene_manager ()
	const
{
	assert (is_initialized ());

	return scene_manager;
}

Ogre :: SceneNode & Sector ::
	copy_node (Ogre :: SceneNode & example)
{
	assert (is_initialized ());

	assert (example . numAttachedObjects () == 1);
	assert (example . getAttachedObject (0) != NULL);
	
	Ogre :: SceneNode * node = scene_manager . getRootSceneNode ()
													-> createChildSceneNode ();
	
	node -> attachObject (example . detachObject
										(static_cast <unsigned short int> (0)));
	node -> setPosition (example . getPosition ());
	node -> setOrientation (example . getOrientation ());
//	node -> setVisibility (example . getVisibility ());
	node -> setScale (example . getScale ());
	
	return * node;
}

Item & Sector ::
	represent (Item & item, float x, float y, float z, float scale)
{
	assert (is_initialized ());
	
	add (item);

	item . get_representation () . node . setScale (scale, scale, scale);
	item . get_representation () . node . setPosition (x, y, z);

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
