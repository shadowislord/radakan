#include <Ogre.h>
#include <OIS.h>

#include "character.hpp"
#include "camera.hpp"

#include "input.hpp"
#include "tslrpg.hpp"

bool Tslrpg_Prototype::setup_ogre (void)
{

	// Get the OGRE root object
	ogre_root = new Ogre::Root ();

	// Load a previously saved configuration file
	// if (! ogre_root->restoreConfig ()) { return false; }
	if (! ogre_root->showConfigDialog ()) { return false; }

	// Create the render window
	ogre_window = ogre_root->initialise (true, "Scattered Lands");

	// Create the scene manager
	ogre_scene_mgr = ogre_root->createSceneManager (ST_EXTERIOR_CLOSE, "Tslrpg_Scene_Mgr");

	// Create a default camera
	ogre_camera = ogre_scene_mgr->createCamera ("DefaultCam");
	ogre_camera->setPosition (Vector3 (0, 0, 0));
	ogre_camera->lookAt (Vector3 (0, 0, -1));
	ogre_camera->setNearClipDistance (5);

	// Add a Viewport to the render window
	Viewport* vp = ogre_window->addViewport (ogre_camera);
	vp->setBackgroundColour (ColourValue (0,0,0));

	// Alter the camera aspect ratio to match the viewport
	ogre_camera->setAspectRatio (Real (vp->getActualWidth ()) / Real (vp->getActualHeight ()));

	// Set default mipmap level
	TextureManager::getSingleton ().setDefaultNumMipmaps (5);

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

	// Load resources
	ResourceGroupManager::getSingleton ().initialiseAllResourceGroups ();

	return true;
}

bool Tslrpg_Prototype::setup_scene (void)
{

	// Set the sky dome
	ogre_scene_mgr->setSkyDome (true, "Peaceful", 10, 5);

	// Setup lighting
	ogre_scene_mgr->setAmbientLight (ColourValue (0.5, 0.5, 0.5));
	ogre_scene_mgr->setShadowTechnique (SHADOWTYPE_STENCIL_ADDITIVE);

	// Create a light
	Light * myLight = ogre_scene_mgr -> createLight ("DayLight");
	myLight -> setCastShadows (true);
	myLight -> setDirection (-1.0, -1.0, 0.0);
	myLight -> setType (Light::LT_DIRECTIONAL);

	// Create a player character
	player_char = new Player_Character ("Player1", ogre_scene_mgr->createEntity ("Ogre_Head", "ogrehead.mesh"), ogre_scene_mgr);

	// Create an extended camera
	ex_camera = new Extended_Camera ("Chase_Cam", ogre_scene_mgr, ogre_camera);

	// Create a plane to use as the ground
	Plane plane (Vector3::UNIT_Y, 0);
	MeshManager::getSingleton().createPlane("Ground",
		ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, plane,
		10000,10000, 10,10, true, 1, 120,120, Vector3::UNIT_Z);

	// Create the ground
	Entity* ground = ogre_scene_mgr->createEntity ("Ground_Entity", "Ground");
	ground->setMaterialName ("TavernWalls");
	ground->setCastShadows (false);
	SceneNode* ground_node = ogre_scene_mgr->getRootSceneNode ()->createChildSceneNode ();
	ground_node->attachObject (ground);
	ground_node->setPosition (Vector3 (0.0f, -50.0f, 0.0f));

	// Create and add our input frame listener
	frame_listener = new Input (ogre_window, ogre_camera, player_char, ex_camera);
	ogre_root->addFrameListener (frame_listener);

	return true;
}

void Tslrpg_Prototype::run (void)
{
	setup_ogre ();
	setup_resources ();
	setup_scene ();

	while (! ogre_window->isClosed ())
	{
		PlatformManager::getSingletonPtr ()->messagePump (ogre_window);

		if (! ogre_root->renderOneFrame ()) break;
	}
}