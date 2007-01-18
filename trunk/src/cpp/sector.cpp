#include "pause_state.hpp"

using namespace std;
using namespace tsl;

Sector ::
	Sector
	(
		string new_name,
		TSL & new_owner
	) :
	Object (new_name),
	Set <Entity> (new_name),
	State <TSL> (new_owner),
	btDiscreteDynamicsWorld
	(
		new btCollisionDispatcher (),
		new btSimpleBroadphase (),
		new btSequentialImpulseConstraintSolver ()
	),
	scene_manager (owner . new_scene_manager ())
{
	trace () << "Sector (" << new_name << ", " << new_owner << ")" << endl;
	assert (Object :: is_initialized ());

	camera = scene_manager . createCamera ("Eyes");
	camera -> setNearClipDistance (5);
	camera -> setFarClipDistance (2000);

	//	!!!	This doesn't work somehow.
	scene_manager . setSkyDome (true, "Peaceful", 10, 5);

	//	the ground
	Ogre :: SceneNode & groundNode = create_entity_node ("Ground","plane.mesh", 1);
	dynamic_cast <Ogre :: Entity *> (groundNode . getAttachedObject (0)) -> setMaterialName ("TavernWalls");
	groundNode . setOrientation (Ogre :: Quaternion (Ogre :: Radian (- Ogre :: Math :: HALF_PI), Ogre :: Vector3 :: UNIT_X));

	if (! Player :: is_instantiated ())
	{
		Player * player = new Player
		(
			80,
			65,
			btVector3 (100, 0, 200),
			create_entity_node ("Player", "ninja.mesh", 0.1)
		);

		player -> add
		(
			* (new Container
			(
				true,
				true,
				true,
				30,
				3,
				player -> get_position (),
				create_entity_node ("Backpack", "bar.mesh", 1)
			))
		);

		trace () << to_string (player) << "'s weight: "
										<< player -> get_total_weight () << endl;
		Weapon * sword = new Weapon
				(1, 2, btVector3 (1, 0, 4), 3, 4, 5, 6, 7, 8,
				create_entity_node ("Sword", "bar.mesh", 1));
		trace () << to_string (sword) << "'s weight: " << sword -> get_total_weight () << endl;
		player -> add (* sword);
		
		add (* player);
	}

	NPC * ninja = new NPC
	(
		80,
		65,
		btVector3 (120, 0, 30),
		create_entity_node ("Ninja (" + * this + ")", "ninja.mesh", 0.1)
	);
	add (* ninja);

	//	the tavern
	add (* (new Entity (false, true, true, 0, 0, btVector3 (0, 0, 0),
		create_entity_node ("Tavern", "tavern.mesh", 1))));
	add (* (new Entity (false, true, true, 0, 0, btVector3 (0, 0, 0),
		create_entity_node ("Bar", "bar.mesh", 1))));
	add (* (new Entity (false, true, true, 0, 0, btVector3 (116, 0, 17),
		create_entity_node ("Table 1", "table.mesh", 1))));
	add (* (new Entity (false, true, true, 0, 0, btVector3 (116, 0, 57),
		create_entity_node ("Table 2", "table.mesh", 1))));
	add (* (new Entity (false, true, true, 0, 0, btVector3 (26, 0, 97),
		create_entity_node ("Table 3", "table.mesh", 1))));

	//	'table2.mesh' is not textured yet.

	//	These fences are not textured yet.
	add (* (new Entity (false, true, true, 0, 0, btVector3 (126, 0, - 197),
		create_entity_node ("Fence 1", "fences1.mesh", 0.3))));
	add (* (new Entity (false, true, true, 0, 0, btVector3 (226, 0, - 297),
		create_entity_node ("Fence 2", "fences2.mesh", 0.3))));
	add (* (new Entity (false, true, true, 0, 0, btVector3 (326, 0, - 397),
		create_entity_node ("Fence 3", "fences3.mesh", 0.3))));
	add (* (new Entity (false, true, true, 0, 0, btVector3 (426, 0, - 497),
		create_entity_node ("Fence 4", "fences4.mesh", 0.3))));
	add (* (new Entity (false, true, true, 0, 0, btVector3 (526, 0, - 597),
		create_entity_node ("Fence 5", "fences5.mesh", 0.3))));

	//	not textured
	add (* (new Entity (false, true, true, 0, 0, btVector3 (800, 0, 200),
		create_entity_node ("House", "house.mesh", 0.4))));
	add (* (new Entity (false, true, true, 0, 0, btVector3 (- 500, 0, - 500),
		create_entity_node ("Wagon", "wagon.mesh", 0.02))));
	add (* (new Entity (false, true, true, 0, 0, btVector3 (- 550, 0, - 500),
		create_entity_node ("Pot", "pot.mesh", 0.2))));
	add (* (new Entity (false, true, true, 0, 0, btVector3 (- 600, 0, - 500),
		create_entity_node ("Pot 2", "pot_2.mesh", 0.2))));
	add (* (new Entity (false, true, true, 0, 0, btVector3 (- 700, 35, - 700),
		create_entity_node ("Pine tree", "pine_tree_2.mesh", 1.5))));

	//	forest of 1961 trees
	//	(30000 trees takes to long to load)
	Entity * temp_tree = NULL;
	for (int i = - 25; i <= 25; i++)
	{
		for (int j = int (- sqrt (625 - i * i)); j <= sqrt (625 - i * i); j++)
		{
			btVector3 pos = btVector3
				(
					- 1500 + 100 * i + Ogre :: Math :: RangeRandom (- 130, 130),
					0,
					3000 + 100 * j + Ogre :: Math :: RangeRandom (- 130, 130)
				);
			if (0.4 < Ogre :: Math :: RangeRandom (0, 1))
			{
				temp_tree = new Entity (false, true, true, 0, 0, pos,
					create_entity_node ("Tree no. " + to_string (50 * i + j),
					"tree.mesh", 1.5));
				temp_tree -> node -> setDirection (0, - 1, 0);
			}
			else
			{
				temp_tree = new Entity (false, true, true, 0, 0, pos + btVector3 (0, 96, 0),
					create_entity_node ("Tree no. " + to_string (50 * i + j),
					"pine_tree.mesh", 1.5));
			}
			add (* temp_tree);
		}
	}

	for (int i = 0; i < 200; i++)
	{
		char name [50];
		sprintf (name, "cluster_%d", i);
		Ogre :: BillboardSet * bbs2 = scene_manager . createBillboardSet (name, 1);
		bbs2 -> setBillboardRotationType (Ogre :: BBR_VERTEX);
		Ogre :: SceneNode * bbsNode2 = scene_manager . getRootSceneNode () -> createChildSceneNode ();

		char type [50];
		int ran = (int (Ogre :: Math :: RangeRandom (0, 1) * 100)) % 8 + 1;
		sprintf (type, "spacebillboard/cluster_%d", ran);
		bbs2 -> setMaterialName (type);
		bbs2 -> getMaterial () -> setTextureFiltering (Ogre :: TFO_TRILINEAR);
		bbs2 -> getMaterial () -> setDepthCheckEnabled (true);
		int px = int (Ogre :: Math :: RangeRandom (- 2500, 2500));
		int pz = int (Ogre :: Math :: RangeRandom (- 2500, 2500));
		Ogre :: Billboard * bb = bbs2 -> createBillboard
											(0, 0, 0, Ogre :: ColourValue());
		int dim = int (Ogre :: Math :: RangeRandom (1.5f, 2));
		bb -> setDimensions (100 * dim, 100 * dim);
		bb -> setRotation (Ogre :: Radian (Ogre :: Math :: RangeRandom (0, 2 * Ogre :: Math :: PI)));
		bbsNode2 -> attachObject (bbs2);
		bbsNode2 -> setPosition (px, 500, pz);
	}

	assert (is_initialized ());
}

Sector ::
	~Sector ()
{
	trace () << "~Sector ()" << endl;
	assert (is_initialized ());

	trace () << "deleting camera... " << int (camera) << endl;
	delete camera;
}

//	virtual
bool Sector ::
	is_initialized ()
	const
{
	return warn <Sector> (Object :: is_initialized ());
}

//	static
string Sector ::
	get_type_name ()
{
	return "sector";
}

//	virtual
bool Sector ::
	add (Entity & entity)
{
	assert (is_initialized ());
	assert (entity . is_initialized ());

	if (entity . is_type <NPC> ())
	{
		bool success = npcs . insert (& entity . to_type <NPC> ()) . second;
		assert (success);
	}
	bool result = Set <Entity> :: add (entity);
	assert (result);

	addRigidBody (& entity);
	
	return true;
}

//	virtual
bool Sector ::
	move_to (Entity & entity, Set <Entity> & other_set)
{
	assert (is_initialized ());
	assert (entity . is_initialized ());
	assert (other_set . is_initialized ());
	
	removeRigidBody (& entity);
	
	if (entity . is_type <NPC> ())
	{
		npcs . erase (& entity . to_type <NPC> ());
	}

	return Set <Entity> :: move_to (entity, other_set);
}

string Sector ::
	run ()
{
	//	stepSimulation (milliseconds_passed / 1000000.f);
	string think;

	for (set <NPC *> :: const_iterator i = npcs . begin ();
												i != npcs . end (); i ++)
	{
		NPC * npc = * i;
		if (! npc -> is_dead ())
		{
			think = npc -> run ();
			if (think != State <NPC> :: nothing)
			{
				trace () << think << endl;
			}
		}
	}

	//	Handle movement
	//	Normal WASD keys don't work on all keyboard layouts, so we'll use ESDF for now.
	if (Input_Engine :: get () . get_key ("e", false))
	{
		Player :: get () . walk (0.2 * owner . get_last_turn_lenght ());
	}
	if (Input_Engine :: get () . get_key ("d", false))
	{
		Player :: get () . walk (- 0.1 * owner . get_last_turn_lenght ());
	}
	if (Input_Engine :: get () . get_key ("s", false))
	{
		Player :: get () . turn (0.005 * owner . get_last_turn_lenght ());
	}
	if (Input_Engine :: get () . get_key ("f", false))
	{
		Player :: get () . turn (- 0.005 * owner . get_last_turn_lenght ());
	}

	//	pause
	if (Input_Engine :: get () . get_key ("p", true))
	{
		owner . change_active_state <Pause_State> ();
		GUI_Engine :: get () . show ("Game paused");
	}

	//	hit
	if (Input_Engine :: get () . get_key ("h", true))
	{
		NPC & npc = * * npcs . begin ();
		if (! npc . is_dead ())
		{
			GUI_Engine :: get () . show
				(Battle_Engine :: get () . hit (Player :: get (), npc));
		}
		else
		{
			GUI_Engine :: get () . show
				("Mutilating a dead body is *not* nice.");
		}
	}

	//	move the weapon
	if (Input_Engine :: get () . get_key ("m", true))
	{
		//	Memo to self (Tinus):
		//	NPC npc = * * npcs . begin ();  ->  that *copies* the NPC.
		NPC & npc = * * npcs . begin ();
		if (Player :: get () . has_weapon ())
		{
			Player :: get () . move_to (* Player :: get () . get_weapon (), npc);
			assert (! Player :: get () . has_weapon ());
			assert (npc . has_weapon ());
			GUI_Engine :: get () . show
									("You gave your weapon to the ninja.");
		}
		else if (npc . has_weapon ())
		{
			npc . Character :: move_to (* npc . get_weapon (), Player :: get ());
			assert (Player :: get () . has_weapon ());
			assert (! npc . has_weapon ());
			GUI_Engine :: get () . show
								("You took your weapon from the ninja.");
		}
		else
		{
			GUI_Engine :: get () . show
							("Both you and the ninja don't have a weapon.");
		}
	}

	if (Input_Engine :: get () . get_mouse_button
					(Input_Engine :: get () . left_mouse_button, true))
	{
		GUI_Engine :: get () . left_mouse_button_click ();
	}

	if (Input_Engine :: get () . get_mouse_button
					(Input_Engine :: get () . middle_mouse_button, false))
	{
		float x_offset = Input_Engine :: get ()
										. get_mouse_position (true) . first;

		if (x_offset != 0)
		{
			x_offset = - 0.005 * owner . get_last_turn_lenght () * x_offset;

			debug () << Input_Engine :: get () . middle_mouse_button
									<< " - x offset: " <<  x_offset << endl;

			Player :: get () . turn (x_offset);
		}
	}

	if (Input_Engine :: get () . get_mouse_button
					(Input_Engine :: get () . right_mouse_button, true))
	{
		if (0.1 < Ogre :: Math :: RangeRandom (0, 1))
		{
			GUI_Engine :: get () . show
				("FPS: " + to_string (owner . get_FPS ()));
		}
		else
		{
			GUI_Engine :: get () . show
				("Trivia: there are 1961 trees in each forest.");
		}
	}

	if (Input_Engine :: get () . get_key ("1", true))
	{
		State <TSL> * sector = owner . get_child ("Sector 1", true);
		assert (sector != NULL);
		owner . change_active_state (* sector);
		GUI_Engine :: get () . show ("Sector 1");
	}

	if (Input_Engine :: get () . get_key ("2", true))
	{
		State <TSL> * sector = owner . get_child ("Sector 2", true);
		assert (sector != NULL);
		owner . change_active_state (* sector);
		GUI_Engine :: get () . show ("Sector 2");
	}

	camera -> setPosition
		(Player :: get () . node -> getPosition () + Ogre :: Vector3 (0, 18, 0));
	camera -> setOrientation
		(Player :: get () . node -> getOrientation ());
	
	return "continue";
}

Ogre :: Camera & Sector ::
	get_camera ()
	const
{
	assert (is_initialized ());

	return * camera;
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

Ogre :: SceneNode & Sector ::
	create_entity_node (string name, string mesh_name, float scale)
{
	Ogre :: SceneNode * node = scene_manager . getRootSceneNode () ->
													createChildSceneNode ();
	
	node -> attachObject (scene_manager . createEntity (name, mesh_name));
	
	assert (node -> numAttachedObjects () == 1);
	assert (node -> getAttachedObject (0) != NULL);
	node -> setScale (Ogre :: Vector3 (scale, scale, scale));
	return * node;
}
