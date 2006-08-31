#include "tslrpg.hpp"

using namespace std;
using namespace Ogre;

Tslrpg::
	Tslrpg ():
	Object ("Tslrpg")
{
	root = new Root();

	if (! root->showConfigDialog ())
	{
		 abort ();
	}

    // I just don't like the resource cfg file :/
    // Catch any errors
    try
    {
        // Add textures directory
        ResourceGroupManager::getSingleton().addResourceLocation("data/texture",
                                                                 "FileSystem",
                                                                 "Textures",
                                                                 true);

        // Add 3D models directory
        ResourceGroupManager::getSingleton().addResourceLocation("data/model",
                                                                 "FileSystem",
                                                                 "Models",
                                                                 true);

        // Add materials directory
        ResourceGroupManager::getSingleton().addResourceLocation("data/material",
                                                                 "FileSystem",
                                                                 "material",
                                                                 true);

        // Initialise our resources
        ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

    // End of try statement
    }
    catch(Ogre::Exception &e)
    {
    }


	window = root->initialise (true);

	scene_mgr = root->createSceneManager (ST_GENERIC);

	//	Create the camera
	camera = scene_mgr->createCamera ("PlayerCam");
	camera->setPosition (Vector3 (0,0,0));
	camera->lookAt (Vector3 (0,0,-300));
	camera->setNearClipDistance (5);
	camera->setFarClipDistance (2000);

	// Create one viewport, entire window
	Viewport * vp = window->addViewport (camera);
	vp->setBackgroundColour (ColourValue (0,0,0));

	// Alter the camera aspect ratio to match the viewport
	camera->setAspectRatio (
		Real (vp->getActualWidth ()) / Real (vp->getActualHeight ()));

	// Set default mipmap level (NB some APIs ignore this)
	TextureManager::getSingleton().setDefaultNumMipmaps(5);

	ResourceGroupManager::getSingleton ().initialiseAllResourceGroups ();

	frame_listener = new Sl_Frame_Listener (scene_mgr, window, camera, false, false);

	root->addFrameListener (frame_listener);

	input_device = PlatformManager::getSingleton ().createInputReader ();
	input_device->initialise (window);

	player = new Character ("player");
	debug () << * player << "'s weight: " << player->get_total_weight () << endl;
	Weapon * sword = new Weapon
		("sword", Vector3 (1, 0.1, 0.1), 1, 2, 3, 4, 5, 6, 7, 8);
	debug () << * sword << "'s weight: " << sword->get_total_weight () << endl;
	assert (! player->backpack->contains (sword));
	assert (player->backpack->add (sword));
	assert (player->backpack->contains (sword));
	debug () << * player << "'s weight with sword: "
		<< player->get_total_weight () << endl;
	assert (player->backpack->remove (sword));
	assert (! player->backpack->contains (sword));
	debug () << * player << "'s weight: " << player->get_total_weight () << endl;

	battle_engine.hit (player, player);

	create_scene ();
}

Tslrpg::
	~Tslrpg ()
{
//	this doesn't get called somehow

	debug () << ("deleting player...") << int (player) << endl;
	delete player;
	debug () << ("deleting input_device...") << int (input_device) << endl;
	delete input_device;
	debug () << ("deleting frame_listener...") << int (frame_listener) << endl;
	delete frame_listener;
	debug () << ("deleting camera...") << int (camera) << endl;
	delete camera;
//	debug () << ("deleting window...") << int (window) << endl;
//	delete window;
	debug () << ("deleting scene_mgr...") << int (scene_mgr) << endl;
	delete scene_mgr;
	debug () << ("deleting root...") << int (root) << endl;
	delete root;
	debug () << ("all deleted") << endl;
}

void
	Tslrpg::
	run ()
{
	root->startRendering ();
}

//	virtual
void Tslrpg::create_scene ()
{
	Entity * ent = scene_mgr->createEntity("ogre", "fort.mesh");
	//ent->setMaterialName("metal_plate");
	SceneNode* node = scene_mgr->getRootSceneNode()->createChildSceneNode();
	node->attachObject(ent);
	node->setPosition(0,0,0);

	for (int i = 0; i < 100; i++)
	{
		char name[50];
		sprintf (name, "cluster_%d", i);
		BillboardSet * bbs2 = scene_mgr->createBillboardSet (name, 1);
		bbs2->setBillboardRotationType (BBR_VERTEX);
		SceneNode* bbsNode2 = scene_mgr->getRootSceneNode ()->createChildSceneNode ();

		char type[50];
		int ran = ((int)(Math::RangeRandom(0, 1)*100))%8+1;
		sprintf(type, "spacebillboard/cluster_%d", ran);
		bbs2->setMaterialName(type);
		bbs2->getMaterial()->setTextureFiltering(TFO_TRILINEAR);
		bbs2->getMaterial()->setDepthCheckEnabled(true);
		int px = int (Math::RangeRandom (-2500, 2500));
		int py = int (Math::RangeRandom (-2500, 2500));
		int pz = int (Math::RangeRandom (-2500, 2500));
		px = 0;
		Billboard* bb = bbs2->createBillboard (0, 0, 0, ColourValue());
		int dim = int (Math::RangeRandom (1.5f, 2));
		bb->setDimensions (100*dim, 100*dim);
		bb->setRotation (Radian (Math::RangeRandom (0, 2 * Math::PI)));
		bbsNode2->attachObject (bbs2);
		bbsNode2->setPosition (px, py, pz);
	}
	for(int i = 0; i < 200; i++)
	{
		char name[50];
		sprintf(name, "star_%d", i);
		BillboardSet* bbs2 = scene_mgr->createBillboardSet(name, 1);
		bbs2->setBillboardRotationType(BBR_VERTEX);
		SceneNode* bbsNode2 = scene_mgr->getRootSceneNode()->createChildSceneNode();

		char type[50];
		int ran = ((int)(Math::RangeRandom (0, 1) * 100)) % 2 + 1;
		sprintf (type, "spacebillboard/star_%d", ran);
		bbs2->setMaterialName (type);
		bbs2->getMaterial()->setTextureFiltering (TFO_TRILINEAR);
		bbs2->getMaterial()->setDepthCheckEnabled (true);
		int px = int (Math::RangeRandom (-2500, 2500));
		int py = int (Math::RangeRandom (-2500, 2500));
		int pz = int (Math::RangeRandom (-2500, 2500));
		px = 0;
		Billboard* bb = bbs2->createBillboard
		(
			0, 0, 0,
			ColourValue
			(
				Math::RangeRandom (0.6f,1),
				Math::RangeRandom (0.6f,1),
				Math::RangeRandom (0.6f,1)
			)
		);
		int dim = int (Math::RangeRandom(1.5f, 2));
		bb->setDimensions (5 * dim, 5 * dim);
		bb->setRotation (Radian (Math::RangeRandom (0, 2 * Math::PI)));
		bbsNode2->attachObject (bbs2);
		bbsNode2->setPosition (px, py, pz);
	}
}
