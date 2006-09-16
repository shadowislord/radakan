#include "sector.hpp"

Sector::
	Sector
		(string new_name,
		Ogre :: SceneManager * new_scene_manager,
		Ogre :: RenderWindow * window) :
	Object (new_name),
	Container (new_scene_manager->createEntity (new_name, "fort.mesh"),
		new_scene_manager->getRootSceneNode ()->createChildSceneNode ())
{
	assert (Object :: is_initialized ());

	scene_manager = new_scene_manager;
	camera = scene_manager->createCamera ("Eyes");
	camera->setPosition (Ogre :: Vector3 (500, 0, 0));
	camera->lookAt (Ogre :: Vector3 (0, 0, - 300));
	camera->setNearClipDistance (5);
	camera->setFarClipDistance (2000);

	// Create one viewport, entire window
	Ogre :: Viewport * view_port = window->addViewport (camera);
	view_port->setBackgroundColour (Ogre :: ColourValue (0, 0, 0));

	// Alter the camera aspect ratio to match the viewport
	camera->setAspectRatio
		(Ogre :: Real (view_port->getActualWidth ())
		/ Ogre :: Real (view_port->getActualHeight ()));

	add
		(new Entity
			(false,
			true,
			true,
			0,
			0,
			Ogre :: Vector3 (0, 0, 0),
			scene_manager->createEntity ("Fort", "fort.mesh"), scene_manager->getRootSceneNode ()->createChildSceneNode ()));

	player = new Character
		(scene_manager->createEntity ("Player", "fort.mesh"),
		scene_manager->getRootSceneNode ()->createChildSceneNode ());

	add (player);
	player->node->setScale (Ogre :: Vector3 (0.2, 2, 0.2));

	player->add
	(
		new Container
		(
			true,
			true,
			true,
			30,
			3,
			player->node->getPosition (),
			scene_manager->createEntity ("Backpack", "fort.mesh"),
			scene_manager->getRootSceneNode ()->createChildSceneNode ()
		)
	);

	debug () << * player << "'s weight: "
									<< player->get_total_weight () << endl;
	Weapon * sword = new Weapon
			(1, 2, Ogre :: Vector3 (1, 4, 4), 3, 4, 5, 6, 7, 8,
			scene_manager->createEntity ("Sword", "fort.mesh"),
			scene_manager->getRootSceneNode ()->createChildSceneNode ());
	debug () << * sword << "'s weight: " << sword->get_total_weight () << endl;
	assert (! player->contains (sword, true));
	player->add (sword);
	assert (sword != NULL);
	assert (player->contains (sword, false));
	debug () << * player << "'s weight with sword: "
									<< player->get_total_weight () << endl;

	NPC * npc = new NPC
		(scene_manager->createEntity ("NPC", "fort.mesh"),
		scene_manager->getRootSceneNode ()->createChildSceneNode ());
	npc->create_ai ();
	add (npc);
	npc->ai->add <Peace_State> ();
	npc->ai->add <Fight_State> ();
	npc->ai->change_active_state <Fight_State> ();
	
	for (int i = 0; i < 100; i++)
	{
		char name [50];
		sprintf (name, "cluster_%d", i);
		Ogre :: BillboardSet * bbs2 = scene_manager->createBillboardSet (name, 1);
		bbs2->setBillboardRotationType (Ogre :: BBR_VERTEX);
		Ogre :: SceneNode * bbsNode2 = scene_manager->getRootSceneNode ()->createChildSceneNode ();

		char type [50];
		int ran = ((int)(Ogre :: Math :: RangeRandom (0, 1) * 100)) % 8 + 1;
		sprintf (type, "spacebillboard/cluster_%d", ran);
		bbs2->setMaterialName (type);
		bbs2->getMaterial ()->setTextureFiltering (Ogre :: TFO_TRILINEAR);
		bbs2->getMaterial ()->setDepthCheckEnabled (true);
		int px = int (Ogre :: Math :: RangeRandom (-2500, 2500));
		int py = int (Ogre :: Math :: RangeRandom (-2500, 2500));
		int pz = int (Ogre :: Math :: RangeRandom (-2500, 2500));
//		px = 0;
		Ogre :: Billboard * bb = bbs2->createBillboard
											(0, 0, 0, Ogre :: ColourValue());
		int dim = int (Ogre :: Math::RangeRandom (1.5f, 2));
		bb->setDimensions (100 * dim, 100 * dim);
		bb->setRotation (Ogre :: Radian (Ogre :: Math :: RangeRandom (0, 2 * Ogre :: Math :: PI)));
		bbsNode2->attachObject (bbs2);
		bbsNode2->setPosition (px, py, pz);
	}
	for(int i = 0; i < 100; i++)
	{
		char name[50];
		sprintf(name, "star_%d", i);
		Ogre :: BillboardSet * bbs2 = scene_manager->createBillboardSet (name, 1);
		bbs2->setBillboardRotationType (Ogre :: BBR_VERTEX);
		Ogre :: SceneNode * bbsNode2 = scene_manager->getRootSceneNode ()->createChildSceneNode ();

		char type[50];
		int ran = ((int)(Ogre :: Math :: RangeRandom (0, 1) * 100)) % 2 + 1;
		sprintf (type, "spacebillboard/star_%d", ran);
		bbs2->setMaterialName (type);
		bbs2->getMaterial ()->setTextureFiltering (Ogre :: TFO_TRILINEAR);
		bbs2->getMaterial ()->setDepthCheckEnabled (true);
		int px = int (Ogre :: Math :: RangeRandom (-2500, 2500));
		int py = int (Ogre :: Math :: RangeRandom (-2500, 2500));
		int pz = int (Ogre :: Math :: RangeRandom (-2500, 2500));
//		px = 0;
		Ogre :: Billboard * bb = bbs2->createBillboard
		(
			0, 0, 0,
			Ogre :: ColourValue
			(
				Ogre :: Math :: RangeRandom (0.6f,1),
				Ogre :: Math :: RangeRandom (0.6f,1),
				Ogre :: Math :: RangeRandom (0.6f,1)
			)
		);
		int dim = int (Ogre :: Math :: RangeRandom(1.5f, 2));
		bb->setDimensions (5 * dim, 5 * dim);
		bb->setRotation (Ogre :: Radian (Ogre :: Math :: RangeRandom (0, 2 * Ogre :: Math :: PI)));
		bbsNode2->attachObject (bbs2);
		bbsNode2->setPosition (px, py, pz);
	}

	assert (player->get_weapon () == sword);
	assert (player->move_to (sword, this));
	assert (! player->contains (sword, true));
	assert (! player->has_weapon ());
	debug () << * player << "'s weight: "
										<< player->get_total_weight () << endl;

	assert (move_to (sword, npc));

	npc->ai->think ();
	npc->ai->act ();
	
	assert (npc->move_to (sword, player));
	
	npc->ai->think ();
	npc->ai->act ();
	
	npc->ai->think ();
	npc->ai->act ();

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

Character * Sector ::
	get_player ()
	const
{
	assert (is_initialized ());

	return player;
}
