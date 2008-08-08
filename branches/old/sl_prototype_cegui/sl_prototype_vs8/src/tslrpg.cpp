#include "input.hpp"
#include "tslrpg.hpp"

bool Tslrpg_Prototype::setup_ogre (void)
{

	// Get the OGRE root object
	ogre_root = new Ogre::Root ();

	// Load a previously saved configuration file
	if (! ogre_root->restoreConfig ()) { return false; }

	// Create the render window
	ogre_window = ogre_root->initialise (true, "Scattered Lands");

	// Create the scene manager
	ogre_scene_mgr = ogre_root->createSceneManager (ST_GENERIC, "TslrpgSM");

	// Create a default camera
	ogre_camera = ogre_scene_mgr->createCamera ("DefaultCam");
	ogre_camera->setPosition (Vector3 (0, 0, 400));
	ogre_camera->lookAt (Vector3 (0, 0, -400));
	ogre_camera->setNearClipDistance (5);

	// Add a Viewport to the render window
	Viewport* vp = ogre_window->addViewport (ogre_camera);
	vp->setBackgroundColour (ColourValue (0,0,0));

	// Alter the camera aspect ratio to match the viewport
	ogre_camera->setAspectRatio (Real (vp->getActualWidth ()) / Real (vp->getActualHeight ()));

	// Set default mipmap level
	TextureManager::getSingleton ().setDefaultNumMipmaps (5);

	// Load resources
	setup_resources ();

	// Set up CEGUI
	cegui_renderer = new CEGUI::OgreCEGUIRenderer (ogre_window, Ogre::RENDER_QUEUE_OVERLAY, false, 3000, ogre_scene_mgr);
	cegui_system = new CEGUI::System (cegui_renderer);

	CEGUI::SchemeManager::getSingleton ().loadScheme ((CEGUI::utf8*)"TaharezLookSkin.scheme");
	cegui_system->setDefaultMouseCursor ((CEGUI::utf8*)"TaharezLook", (CEGUI::utf8*)"MouseArrow");
	CEGUI::MouseCursor::getSingleton ().setImage ("TaharezLook", "MouseMoveCursor");

	// Create a new GUI sheet
	cegui_window = CEGUI::WindowManager::getSingleton ().createWindow
		((CEGUI::utf8*)"DefaultWindow", (CEGUI::utf8*)"Sheet");
	cegui_system->setGUISheet (cegui_window);

	// Create and add our input frame listener
	ois_input = new Input (ogre_window, ogre_camera, cegui_renderer);
	ogre_root->addFrameListener (ois_input);

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

	// Add CEGUI directories
	ResourceGroupManager::getSingleton ().addResourceLocation
		("./data/cegui/configs", "FileSystem", "Gui", true);

	ResourceGroupManager::getSingleton ().addResourceLocation
		("./data/cegui/fonts", "FileSystem", "Gui", true);

	ResourceGroupManager::getSingleton ().addResourceLocation
		("./data/cegui/imagesets", "FileSystem", "Gui", true);

	ResourceGroupManager::getSingleton ().addResourceLocation
		("./data/cegui/layouts", "FileSystem", "Gui", true);

	ResourceGroupManager::getSingleton ().addResourceLocation
		("./data/cegui/looknfeel", "FileSystem", "Gui", true);

	ResourceGroupManager::getSingleton ().addResourceLocation
		("./data/cegui/lua_scripts", "FileSystem", "Gui", true);

	ResourceGroupManager::getSingleton ().addResourceLocation
		("./data/cegui/schemes", "FileSystem", "Gui", true);

	// Load resources
	ResourceGroupManager::getSingleton ().initialiseAllResourceGroups ();

	return true;
}

bool Tslrpg_Prototype::setup_scene (void)
{

	// Set the sky dome
	ogre_scene_mgr->setSkyDome (true, "Peaceful", 10, 5);

	// 

	return true;
}

void Tslrpg_Prototype::run (void)
{
	setup_ogre ();
	setup_scene ();

	while (! ogre_window->isClosed ())
	{
		PlatformManager::getSingletonPtr ()->messagePump (ogre_window);

		if (! ogre_root->renderOneFrame ()) break;
	}
}