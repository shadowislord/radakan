#include "sector.hpp"

using namespace std;
using namespace sl;

Sector ::
	Sector
		(string new_name,
		Ogre :: SceneManager & new_scene_manager) :
	Object (new_name),
	Set <Entity> (new_name),
	btDiscreteDynamicsWorld
	(
		new btCollisionDispatcher (),
		new btSimpleBroadphase (),
		new btSequentialImpulseConstraintSolver ()
	),
	scene_manager (& new_scene_manager)
{
	trace () << "Sector (" << new_name << ", ~new_scene_manager~)" << endl;
	assert (Object :: is_initialized ());

	camera = scene_manager -> createCamera ("Eyes");
	camera -> setNearClipDistance (5);
	camera -> setFarClipDistance (2000);

	//	!!!	This doesn't work somehow.
	scene_manager -> setSkyDome (true, "Peaceful", 10, 5);

	add (* (new Entity (false, true, true, 0, 0, btVector3 (0, 0, 0),
		create_entity_node ("Tavern", "tavern.mesh"))));

	add (* (new Entity (false, true, true, 0, 0, btVector3 (0, 0, 0),
		create_entity_node ("Bar", "bar.mesh"))));

	add (* (new Entity (false, true, true, 0, 0, btVector3 (116, 0, 17),
		create_entity_node ("Table 1", "table.mesh"))));

	add (* (new Entity (false, true, true, 0, 0, btVector3 (116, 0, 57),
		create_entity_node ("Table 2", "table.mesh"))));

	add (* (new Entity (false, true, true, 0, 0, btVector3 (26, 0, 97),
		create_entity_node ("Table 3", "table.mesh"))));

	add (* (new Entity (false, true, true, 0, 0, btVector3 (126, 0, - 197),
		create_entity_node ("Fence 1", "fences1.mesh"))));

	add (* (new Entity (false, true, true, 0, 0, btVector3 (226, 0, - 297),
		create_entity_node ("Fence 2", "fences2.mesh"))));

	add (* (new Entity (false, true, true, 0, 0, btVector3 (326, 0, - 397),
		create_entity_node ("Fence 3", "fences3.mesh"))));

	add (* (new Entity (false, true, true, 0, 0, btVector3 (426, 0, - 497),
		create_entity_node ("Fence 4", "fences4.mesh"))));

	add (* (new Entity (false, true, true, 0, 0, btVector3 (526, 0, - 597),
		create_entity_node ("Fence 5", "fences5.mesh"))));

	add (* (new Entity (false, true, true, 0, 0, btVector3 (1000, 0, 500),
		create_entity_node ("House", "house.mesh"))));

	add (* (new Entity (false, true, true, 0, 0, btVector3 (- 500, 0, - 500),
		create_entity_node ("Wagon", "wagon.mesh"))));

	if (Player :: getSingletonPtr () == NULL)
	{
		Player * player = new Player (create_entity_node ("Player", "ninja.mesh"));
		player -> node -> setScale (Ogre :: Vector3 (0.1, 0.1, 0.1));

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
				create_entity_node ("Backpack", "bar.mesh")
			))
		);

		trace () << to_string (player) << "'s weight: "
										<< player -> get_total_weight () << endl;
		Weapon * sword = new Weapon
				(1, 2, btVector3 (1, 0, 4), 3, 4, 5, 6, 7, 8,
				create_entity_node ("Sword", "bar.mesh"));
		trace () << to_string (sword) << "'s weight: " << sword -> get_total_weight () << endl;
		player -> add (* sword);
		
		add (* player);
	}

	NPC * ninja = new NPC (create_entity_node ("Ninja (" + * this + ")", "ninja.mesh"));
	ninja -> node -> setScale (Ogre :: Vector3 (0.1, 0.1, 0.1));
	ninja -> set_position (btVector3 (120, 0, 30));
	add (* ninja);

	for (int i = 0; i < 100; i++)
	{
		char name [50];
		sprintf (name, "cluster_%d", i);
		Ogre :: BillboardSet * bbs2 = scene_manager -> createBillboardSet (name, 1);
		bbs2 -> setBillboardRotationType (Ogre :: BBR_VERTEX);
		Ogre :: SceneNode * bbsNode2 = scene_manager -> getRootSceneNode () -> createChildSceneNode ();

		char type [50];
		int ran = (int (Ogre :: Math :: RangeRandom (0, 1) * 100)) % 8 + 1;
		sprintf (type, "spacebillboard/cluster_%d", ran);
		bbs2 -> setMaterialName (type);
		bbs2 -> getMaterial () -> setTextureFiltering (Ogre :: TFO_TRILINEAR);
		bbs2 -> getMaterial () -> setDepthCheckEnabled (true);
		int px = int (Ogre :: Math :: RangeRandom (- 2500, 2500));
		int py = int (Ogre :: Math :: RangeRandom (- 2500, 2500));
		int pz = int (Ogre :: Math :: RangeRandom (- 2500, 2500));
		Ogre :: Billboard * bb = bbs2 -> createBillboard
											(0, 0, 0, Ogre :: ColourValue());
		int dim = int (Ogre :: Math :: RangeRandom (1.5f, 2));
		bb -> setDimensions (100 * dim, 100 * dim);
		bb -> setRotation (Ogre :: Radian (Ogre :: Math :: RangeRandom (0, 2 * Ogre :: Math :: PI)));
		bbsNode2 -> attachObject (bbs2);
		bbsNode2 -> setPosition (px, py, pz);
	}
	for (int i = 0; i < 100; i++)
	{
		char name [50];
		sprintf (name, "star_%d", i);
		Ogre :: BillboardSet * bbs2 = scene_manager -> createBillboardSet (name, 1);
		bbs2 -> setBillboardRotationType (Ogre :: BBR_VERTEX);
		Ogre :: SceneNode * bbsNode2 = scene_manager -> getRootSceneNode () -> createChildSceneNode ();

		char type [50];
		int ran = (int (Ogre :: Math :: RangeRandom (0, 1) * 100)) % 2 + 1;
		sprintf (type, "spacebillboard/star_%d", ran);
		bbs2 -> setMaterialName (type);
		bbs2 -> getMaterial () -> setTextureFiltering (Ogre :: TFO_TRILINEAR);
		bbs2 -> getMaterial () -> setDepthCheckEnabled (true);
		int px = int (Ogre :: Math :: RangeRandom (-2500, 2500));
		int py = int (Ogre :: Math :: RangeRandom (-2500, 2500));
		int pz = int (Ogre :: Math :: RangeRandom (-2500, 2500));
		Ogre :: Billboard * bb = bbs2 -> createBillboard
		(
			0, 0, 0,
			Ogre :: ColourValue
			(
				Ogre :: Math :: RangeRandom (0.6f, 1),
				Ogre :: Math :: RangeRandom (0.6f, 1),
				Ogre :: Math :: RangeRandom (0.6f, 1)
			)
		);
		int dim = int (Ogre :: Math :: RangeRandom(1.5f, 2));
		bb -> setDimensions (5 * dim, 5 * dim);
		bb -> setRotation (Ogre :: Radian (Ogre :: Math :: RangeRandom (0, 2 * Ogre :: Math :: PI)));
		bbsNode2 -> attachObject (bbs2);
		bbsNode2 -> setPosition (px, py, pz);
	}

	Ogre :: SceneNode & groundNode = create_entity_node ("Ground","plane.mesh");
	dynamic_cast <Ogre :: Entity *> (groundNode . getAttachedObject (0)) -> setMaterialName ("TavernWalls");
	groundNode . setOrientation (Ogre :: Quaternion (Ogre :: Radian (- Ogre :: Math :: HALF_PI), Ogre :: Vector3 :: UNIT_X));

	assert (is_initialized ());
}

Sector ::
	~Sector ()
{
	trace () << "~Sector (" << endl;
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

	#ifdef SL_DEBUG
		if (entity . is_type <NPC> ())
		{
			bool success = npcs . insert (& entity . to_type <NPC> ()) . second;
			assert (success);
		}
		bool result = Set <Entity> :: add (entity);
		assert (result);
	#endif
	
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

	return Set <Entity> :: move_to (entity, other_set);
}

void Sector ::
	update (int milliseconds_passed)
{
	assert (is_initialized ());

//	stepSimulation (milliseconds_passed / 1000000.f);
	string think;

	NPC * npc = NULL;
	for (set <NPC *> :: const_iterator i = npcs . begin ();
													i != npcs . end (); i ++)
	{
		npc = * i;
		if (! npc -> is_dead ())
		{
			think = npc -> think ();
			if (! think . empty ())
			{
				trace () << think << endl;
			}
		}
	}
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

	return * scene_manager;
}

Ogre :: SceneNode & Sector ::
	copy_node (Ogre :: SceneNode & example)
{
	assert (example . numAttachedObjects () == 1);
	assert (example . getAttachedObject (0) != NULL);
	
	Ogre :: SceneNode * node = scene_manager -> getRootSceneNode ()
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
	create_entity_node (string name, string mesh_name)
{
	Ogre :: SceneNode * node = scene_manager -> getRootSceneNode () ->
													createChildSceneNode ();
	
	node -> attachObject (scene_manager -> createEntity (name, mesh_name));
	
	assert (node -> numAttachedObjects () == 1);
	assert (node -> getAttachedObject (0) != NULL);
	return * node;
}
