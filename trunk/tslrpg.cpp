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

	ConfigFile conf;
	conf.load("resources.cfg");

	// Go through all sections & settings in the file
	ConfigFile::SectionIterator sec_it = conf.getSectionIterator ();

	String secName, typeName, archName;
	while (sec_it.hasMoreElements())
	{
		secName = sec_it.peekNextKey ();
		ConfigFile::SettingsMultiMap * settings = sec_it.getNext ();
		ConfigFile::SettingsMultiMap::iterator i;
		for (i = settings->begin (); i != settings->end (); ++i)
		{
			typeName = i->first;
			archName = i->second;
			ResourceGroupManager::getSingleton().addResourceLocation(
				archName, typeName, secName);
		}
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

	ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

	frame_listener = new FrameLstr (scene_mgr, window, camera, false, false);
	root->addFrameListener (frame_listener);
	
	input_device = PlatformManager::getSingleton().createInputReader();
	input_device->initialise (window);

	create_scene ();


	player = new Character ("player", Vector3 (1, 1, 1));
	assert (! player->inventory->contains (player));
	assert (player->inventory->add (player));
	assert (player->inventory->contains (player));
	assert (player->inventory->remove (player));
	assert (! player->inventory->contains (player));
}

Tslrpg::
	~Tslrpg ()
{
//	this doesn't get called somehow

	debug () << ("Z");
	delete player;
	debug () << ("a");
	delete root;
	debug () << ("b");
	delete camera;
	debug () << ("c");
	delete scene_mgr;
	debug () << ("d");
	delete frame_listener;
	debug () << ("e");
	delete window;
	debug () << ("f");
	delete input_device;
	debug () << ("g");
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
		sprintf(name, "cluster_%d", i);
		BillboardSet* bbs2 = scene_mgr->createBillboardSet(name, 1);
		bbs2->setBillboardRotationType(BBR_VERTEX);
		SceneNode* bbsNode2 = scene_mgr->getRootSceneNode()->createChildSceneNode();
		
		char type[50];
		int ran = ((int)(Math::RangeRandom(0, 1)*100))%8+1;
		sprintf(type, "spacebillboard/cluster_%d", ran);
		bbs2->setMaterialName(type);
		bbs2->getMaterial()->setTextureFiltering(TFO_TRILINEAR);
		bbs2->getMaterial()->setDepthCheckEnabled(true);
		int px = int (Math::RangeRandom(-2500, 2500));
		int py = int (Math::RangeRandom(-2500, 2500));
		int pz = int (Math::RangeRandom(-2500, 2500));
		px = 0;
		Billboard* bb = bbs2->createBillboard(0, 0, 0, ColourValue());
		int dim = int (Math::RangeRandom(1.5f, 2));
		bb->setDimensions(100*dim, 100*dim);
		bb->setRotation(Radian(Math::RangeRandom(0, 2*Math::PI)));
		bbsNode2->attachObject(bbs2);
		bbsNode2->setPosition(px, py, pz);
	}
	for(int i = 0; i < 200; i++)
	{
		char name[50];
		sprintf(name, "star_%d", i);
		BillboardSet* bbs2 = scene_mgr->createBillboardSet(name, 1);
		bbs2->setBillboardRotationType(BBR_VERTEX);
		SceneNode* bbsNode2 = scene_mgr->getRootSceneNode()->createChildSceneNode();
		
		char type[50];
		int ran = ((int)(Math::RangeRandom(0, 1)*100))%2+1;
		sprintf(type, "spacebillboard/star_%d", ran);
		bbs2->setMaterialName(type);
		bbs2->getMaterial()->setTextureFiltering(TFO_TRILINEAR);
		bbs2->getMaterial()->setDepthCheckEnabled(true);
		int px = int (Math::RangeRandom(-2500, 2500));
		int py = int (Math::RangeRandom(-2500, 2500));
		int pz = int (Math::RangeRandom(-2500, 2500));
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
		bb->setDimensions(5*dim, 5*dim);
		bb->setRotation(Radian(Math::RangeRandom(0, 2*Math::PI)));
		bbsNode2->attachObject (bbs2);
		bbsNode2->setPosition (px, py, pz);
	}
}
