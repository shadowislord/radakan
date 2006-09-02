#include "tslrpg.hpp"

using namespace std;

Tslrpg::
	Tslrpg () :
	Object ("Tslrpg")
{
	root = new Ogre :: Root();

	if (! root->showConfigDialog ())
	{
		abort ();
	}

	// I just don't like the resource cfg file :/

	try	// Catch any errors
	{
		// Add textures directory
		Ogre :: ResourceGroupManager :: getSingleton ().addResourceLocation
							("data/texture", "FileSystem", "Textures", true);

		// Add 3D models directory
		Ogre :: ResourceGroupManager :: getSingleton ().addResourceLocation
								("data/model", "FileSystem", "Models", true);

		// Add materials directory
		Ogre :: ResourceGroupManager :: getSingleton ().addResourceLocation
							 ("data/material", "FileSystem", "material", true);

		// Initialise our resources
		Ogre :: ResourceGroupManager :: getSingleton ().initialiseAllResourceGroups ();
	}	// End of try statement
	catch (Ogre :: Exception & e)
	{
	}

	window = root->initialise (true);

	input_device = Ogre :: PlatformManager::getSingleton ().createInputReader ();
	input_device->initialise (window);

	active_sector = new Sector
		("Sector 1", root->createSceneManager (Ogre :: ST_GENERIC), window);
	sectors.insert (active_sector);

	// Set default mipmap level (NB some APIs ignore this)
	Ogre :: TextureManager :: getSingleton().setDefaultNumMipmaps (5);

	Ogre :: ResourceGroupManager :: getSingleton ().initialiseAllResourceGroups ();

	player = active_sector->get_player ();
	root->addFrameListener (active_sector->get_frame_listener ());
}

Tslrpg ::
	~Tslrpg ()
{
	debug () << "Deleting sectors..." << endl;
	for (set <Sector *> :: const_iterator i = sectors.begin (); i != sectors.end (); i ++)
	{
		delete (* i);
	}
	debug () << "Deleting input_device..." << int (input_device) << endl;
	delete input_device;

//	These give problems:

//	debug () << "deleting window..." << int (window) << endl;
//	delete window;
//	debug () << "deleting scene_mgr..." << int (scene_mgr) << endl;
//	delete scene_mgr;
//	debug () << "deleting root..." << int (root) << endl;
//	delete root;

	debug () << "all deleted" << endl;
}

//	virtual
bool Tslrpg ::
	is_initialized ()
	const
{
	return Object :: is_initialized ();
}

void Tslrpg ::
	run ()
{
	assert (is_initialized ());

	active_sector->run ();
	battle_engine.hit (player, player);

	root->startRendering ();
}
