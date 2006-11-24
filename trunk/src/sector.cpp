#include "sector.hpp"

using namespace std;
using namespace sl;

Sector ::
	Sector
		(string new_name,
		Ogre :: SceneManager * new_scene_manager) :
	Object (new_name),
	Set <Entity> (new_name)
{
	assert (Object :: is_initialized ());

	scene_manager = new_scene_manager;
	camera = scene_manager -> createCamera ("Eyes");
	camera -> setNearClipDistance (5);
	camera -> setFarClipDistance (2000);

	scene_manager -> setSkyDome (true, "Peaceful", 10, 5);	//	Doesn't work.

	add (new Entity (false, true, true, 0, 0, Ogre :: Vector3 (0, 0, 0),
		create_entity_node ("Tavern", "tavern.mesh")));

	add (new Entity (false, true, true, 0, 0, Ogre :: Vector3 (0, 0, 0),
		create_entity_node ("Bar", "bar.mesh")));

	add (new Entity (false, true, true, 0, 0, Ogre :: Vector3 (116, 0, 17),
		create_entity_node ("Table 1", "table.mesh")));

	add (new Entity (false, true, true, 0, 0, Ogre :: Vector3 (116, 0, 57),
		create_entity_node ("Table 2", "table.mesh")));

	add (new Entity (false, true, true, 0, 0, Ogre :: Vector3 (26, 0, 97),
		create_entity_node ("Table 3", "table.mesh")));

	add
		(new Entity
			(false,
			true,
			true,
			0,
			0,
			Ogre :: Vector3 (1000, 0, 500),
			create_entity_node ("Fort", "fort.mesh")));
			

	if (Player :: getSingletonPtr () == NULL)
	{
		Player * player = new Player (create_entity_node ("Player", "bar.mesh"));
			player -> node -> setScale (Ogre :: Vector3 (0.2, 2, 0.2));

		player -> add
		(
			new Container
			(
				true,
				true,
				true,
				30,
				3,
				player -> node -> getPosition (),
				create_entity_node ("Backpack", "bar.mesh")
			)
		);

		debug () << * player << "'s weight: "
										<< player -> get_total_weight () << endl;
		Weapon * sword = new Weapon
				(1, 2, Ogre :: Vector3 (1, 0, 4), 3, 4, 5, 6, 7, 8,
				create_entity_node ("Sword", "bar.mesh"));
		debug () << * sword << "'s weight: " << sword -> get_total_weight () << endl;
		player -> add (sword);
		
		add (player);
	}

	NPC * npc = new NPC (create_entity_node ("NPC", "bar.mesh"));
	add (npc);
	npc -> State_Machine :: add <Peace_State> ();
	npc -> State_Machine :: add <Fight_State> ();

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

	assert (is_initialized ());
}

Sector ::
	~Sector ()
{
	assert (is_initialized ());

	debug () << "deleting camera... " << int (camera) << endl;
	delete camera;
}

//	virtual
bool Sector ::
	is_initialized ()
	const
{
	return Object :: is_initialized ();
}

//	virtual
bool Sector ::
	add (Entity * entity)
{
	assert (is_initialized ());
	assert (entity != NULL);
	assert (entity -> is_initialized ());

	if (entity -> is_type <NPC> ())
	{
		bool success = npcs . insert (entity -> to_type <NPC> ()) . second;
		assert (success);
	}
	return Set <Entity> :: add (entity);
}

void Sector ::
	update ()
{
	assert (is_initialized ());

	string think;
	string act;
	for (set <NPC *> :: const_iterator i = npcs . begin ();
													i != npcs . end (); i ++)
	{
		think = (* i) -> think ();
		if (! think . empty ())
		{
			trace () << think << endl;
		}
		act = (* i) -> act ();
		if (! act . empty ())
		{
			trace () << act << endl;
		}
	}
}

Ogre :: SceneManager * Sector ::
	get_scene_manager ()
	const
{
	assert (is_initialized ());

	return scene_manager;
}

Ogre :: Camera * Sector ::
	get_camera ()
	const
{
	assert (is_initialized ());

	return camera;
}

Ogre :: SceneNode * Sector ::
	copy_node (Ogre :: SceneNode * example)
{
	assert (example -> numAttachedObjects () == 1);
	assert (example -> getAttachedObject (0) != NULL);
	
	Ogre :: SceneNode * node = scene_manager -> getRootSceneNode ()
													-> createChildSceneNode ();
	
	node -> attachObject (example -> detachObject
										(static_cast <unsigned short int> (0)));
	node -> setPosition (example -> getPosition ());
	node -> setOrientation (example -> getOrientation ());
//	node -> setVisibility (example -> getVisibility ());
	node -> setScale (example -> getScale ());
	
	return node;
}

Ogre :: SceneNode * Sector ::
	create_entity_node (string name, string mesh_name)
{
	Ogre :: SceneNode * node = scene_manager -> getRootSceneNode () ->
													createChildSceneNode ();
	
	node -> attachObject (scene_manager -> createEntity (name, mesh_name));
	
	assert (node -> numAttachedObjects () == 1);
	assert (node -> getAttachedObject (0) != NULL);
	return node;
}
