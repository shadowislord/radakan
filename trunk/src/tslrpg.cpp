#include "tslrpg.hpp"

using namespace std;

extern string path;

Tslrpg::
	Tslrpg (string path) :
	Object ("Tslrpg")
{
	root = new Ogre :: Root();

	if (! root->showConfigDialog ())
	{
		abort ();
	}

	// I just don't like the resource cfg file :-/ --Tariqwalji

	try	// Catch any errors
	{
		// Add textures directory
		Ogre :: ResourceGroupManager :: getSingleton ().addResourceLocation
					(path + "/data/texture", "FileSystem", "Textures", true);

		// Add 3D models directory
		Ogre :: ResourceGroupManager :: getSingleton ().addResourceLocation
					(path + "/data/model", "FileSystem", "Models", true);

		// Add materials directory
		Ogre :: ResourceGroupManager :: getSingleton ().addResourceLocation
					(path + "/data/material", "FileSystem", "material", true);

		// Initialise our resources
		Ogre :: ResourceGroupManager :: getSingleton ().initialiseAllResourceGroups ();
	}	// End of try statement
	catch (Ogre :: Exception & e)
	{
		#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
			MessageBox (NULL, e.getFullDescription ().c_str (), "An exception has occured!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
		#else
			cerr << "An exception has occured: " << e.getFullDescription () << endl;
		#endif
	}

	window = root->initialise (true, "Scattered Lands");

	//	This is the new input mechanism that is taking advantage of the
	//	new engine handler.
	input_engine = new Input_Engine (window);

	active_sector = new Sector
		("Sector 1", root->createSceneManager (Ogre :: ST_GENERIC), window);
	sectors.insert (active_sector);

	//	Set default mipmap level (NB some APIs ignore this)
	Ogre :: TextureManager :: getSingleton().setDefaultNumMipmaps (5);

	Ogre :: ResourceGroupManager :: getSingleton ().initialiseAllResourceGroups ();

	player = active_sector->get_player ();
}

Tslrpg ::
	~Tslrpg ()
{
	debug () << "deleting sectors..." << endl;
	for (set <Sector *> :: const_iterator i = sectors.begin (); i != sectors.end (); i ++)
	{
		debug () << "deleting " << * * i << "... " << int (* i) << endl;
		delete (* i);
	}

	delete input_engine;


//	These give problems:

//	debug () << "deleting window... " << int (window) << endl;
//	delete window;
//	debug () << "deleting root... " << int (root) << endl;
//	delete root;
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
	
	bool running = true;

	while (running)
	{
		input_engine -> capture ();

		//	quit
		/*if (player -> is_dead () || input_engine -> get_key ("q", false)
								|| input_engine -> get_key ("Escape", false)
								|| window -> isClosed())
		{
			running = false;
		}*/

		if (player->is_dead() || input_engine->is_key_down (OIS::KC_ESCAPE)
			|| window->isClosed ())
		{
			running = false;
		}

		Ogre::PlatformManager::getSingletonPtr ()->messagePump (window);

		if (! root->renderOneFrame ()) break;

		// root->renderOneFrame ();

		gui_engine -> render ();

		//	hit
		if (input_engine -> get_key ("h", true))
		{
			Character * npc = active_sector -> get_child <NPC> ();
			debug () << battle_engine . hit (player, npc) << endl;
		}
		
		//	transfer
		if (input_engine -> get_key ("t", true))
		{
			Character * npc = active_sector -> get_child <NPC> ();
			if (player -> has_weapon ())
			{
				player -> move_to (player -> get_weapon (), npc);
			}
			else
			{
				npc -> move_to (npc -> get_weapon (), player);
			}
		}
	}

	// root->startRendering ();
}
