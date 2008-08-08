#include "input.hpp"
#include "tslrpg.hpp"

bool Tslrpg_Prototype::setup_ogre (void)
{

	// Get the OGRE root object
	root = new Root ();

	// Load a previously saved configuration file
	if (! root->restoreConfig ()) { return false; }

	// Create the render window
	window = root->initialise (true, "Scattered Lands");

	// Create the scene manager
	scene_mgr = root->createSceneManager (ST_EXTERIOR_REAL_FAR, "TslrpgSM");

	// Create a default camera
	camera = scene_mgr->createCamera ("DefaultCam");
	camera->setPosition (Vector3 (0, 0, 400));
	camera->lookAt (Vector3 (0, 0, -400));
	camera->setNearClipDistance (5);

	// Add a Viewport to the render window
	Viewport* vp = window->addViewport (camera);
	vp->setBackgroundColour (ColourValue (0,0,0));

	// Alter the camera aspect ratio to match the viewport
	camera->setAspectRatio (Real (vp->getActualWidth ()) / Real (vp->getActualHeight ()));

	// Set default mipmap level
	TextureManager::getSingleton ().setDefaultNumMipmaps (5);

	// Load resources
	setup_resources ();

	// Create and add our input frame listener
	input = new Input (window, camera);
	root->addFrameListener (input);

	return true;
}

bool Tslrpg_Prototype::setup_resources (void)
{

	// Add models directory
	ResourceGroupManager::getSingleton ().addResourceLocation
		("./data/model", "FileSystem", "Models", true);

	// Add textures directory
	ResourceGroupManager::getSingleton ().addResourceLocation
		("./data/texture", "FileSystem", "Textures", true);

	// Add materials directory
	ResourceGroupManager::getSingleton ().addResourceLocation
		("./data/material", "FileSystem", "Materials", true);

	// --------------------------------------------------------------------------------------

	// Add paginglandscape2 directories

	ResourceGroupManager::getSingleton ().addResourceLocation
		("./data/paginglandscape2/datasrcs", "FileSystem", "PLSM2", true);

	ResourceGroupManager::getSingleton ().addResourceLocation
		("./data/paginglandscape2/materials/programs", "FileSystem", "PLSM2", true);

	ResourceGroupManager::getSingleton ().addResourceLocation
		("./data/paginglandscape2/materials/scripts", "FileSystem", "PLSM2", true);

	ResourceGroupManager::getSingleton ().addResourceLocation
		("./data/paginglandscape2/materials/textures", "FileSystem", "PLSM2", true);

	ResourceGroupManager::getSingleton ().addResourceLocation
		("./data/paginglandscape2/models", "FileSystem", "PLSM2", true);

	/*
	ResourceGroupManager::getSingleton ().addResourceLocation
		("./data/paginglandscape2/overlays", "FileSystem", "PLSM2", true);
		*/

	ResourceGroupManager::getSingleton ().addResourceLocation
		("./data/paginglandscape2/terrains", "FileSystem", "PLSM2", true);

	ResourceGroupManager::getSingleton ().addResourceLocation
		("./data/paginglandscape2/terrains/alpes", "FileSystem", "PLSM2", true);

	ResourceGroupManager::getSingleton ().addResourceLocation
		("./data/paginglandscape2/terrains/hf129_3", "FileSystem", "PLSM2", true);

	ResourceGroupManager::getSingleton ().addResourceLocation
		("./data/paginglandscape2/terrains/TsmTerrain", "FileSystem", "PLSM2", true);

	// --------------------------------------------------------------------------------------

	// Load resources
	ResourceGroupManager::getSingleton ().initialiseAllResourceGroups ();

	return true;
}

bool Tslrpg_Prototype::setup_scene (void)
{

	// Set the sky dome
	scene_mgr->setSkyDome (true, "Peaceful", 10, 5);

	// Set the world geometry
	scene_mgr->setWorldGeometry ("paginglandscape2.cfg");

	return true;
}

void Tslrpg_Prototype::run (void)
{
	setup_ogre ();
	setup_scene ();

	while (! window->isClosed ())
	{
		PlatformManager::getSingletonPtr ()->messagePump (window);

		if (! root->renderOneFrame ()) break;
	}
}