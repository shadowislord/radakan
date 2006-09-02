#include "sector.hpp"

Sector::
	Sector
		(string new_name,
		Ogre :: SceneManager * new_scene_manager,
		Ogre :: RenderWindow * window) :
	Object (new_name)
{
	assert (Object :: is_initialized ());

	scene_manager = new_scene_manager;
	camera = scene_manager->createCamera ("PlayerCam");
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
	
	frame_listener = new Sl_Frame_Listener (scene_manager, window, camera, false, false);

	Ogre :: Entity * fort = scene_manager->createEntity("Fort", "fort.mesh");
	//	fort->setMaterialName("metal_plate");
	Ogre :: SceneNode * fort_node = scene_manager->getRootSceneNode ()->createChildSceneNode ();
	fort_node->attachObject(fort);
	fort_node->setPosition(0,0,0);

	player = new Character
		("player",
		scene_manager->createEntity ("player", "fort.mesh"),
		scene_manager->getRootSceneNode ()->createChildSceneNode ());
	entities.insert (player);
	debug () << * player << "'s weight: " << player->get_total_weight () << endl;
	Weapon * sword = new Weapon
					("sword", 1, 2, Ogre :: Vector3 (1, 4, 4), 3, 4, 5, 6, 7, 8);
	debug () << * sword << "'s weight: " << sword->get_total_weight () << endl;
	assert (! player->contains (sword));
	assert (player->add (sword));
	assert (player->contains (sword));
	debug () << * player << "'s weight with sword: "
										<< player->get_total_weight () << endl;

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
	for(int i = 0; i < 200; i++)
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

	assert (is_initialized ());
}

Sector ::
	~Sector ()
{
	assert (is_initialized ());

	for (set <Entity *> :: const_iterator i = entities.begin (); i != entities.end (); i ++)
	{
		delete (* i);
	}
	
	debug () << "deleting frame_listener..." << int (frame_listener) << endl;
	delete frame_listener;
	debug () << "deleting camera..." << int (camera) << endl;
	delete camera;

}

//	virtual
bool Sector ::
	is_initialized ()
	const
{
	return Object :: is_initialized ();
}

void Sector ::
	run ()
{
	assert (is_initialized ());

	Weapon * sword = player->get_weapon ();
	
	assert (player->add (sword));
	assert (player->contains (sword));
	debug () << * player << "'s weight with two times the sword: "
										<< player->get_total_weight () << endl;

	assert (player->remove (sword));
	assert (player->contains (sword));
	assert (player->remove (sword));
	assert (! player->contains (sword));
	assert (! player->has_weapon ());
	debug () << * player << "'s weight: " << player->get_total_weight () << endl;

	player->add (sword);
}

Sl_Frame_Listener * Sector ::
	get_frame_listener ()
	const
{
	assert (is_initialized ());

	return frame_listener;
}

Character * Sector ::
	get_player ()
	const
{
	assert (is_initialized ());

	return player;
}

//vector<Obstacle*> getObstacles(){
//	return obstacles;
//}
