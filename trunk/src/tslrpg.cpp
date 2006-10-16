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

//	This gives a problem:
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
		debug () << "Main loop - A" << endl;
		active_sector -> update ();
	
		debug () << "Main loop - B" << endl;
		input_engine -> capture ();

		debug () << "Main loop - C" << endl;
		Ogre :: PlatformManager :: getSingletonPtr () -> messagePump (window);

		debug () << "Main loop - D" << endl;
		if (! root -> renderOneFrame ())
		{
			debug () << "Main loop - E" << endl;
			break;
		}

		debug () << "Main loop - F" << endl;
		//	!!!	gui_engine -> render ();

		debug () << "Main loop - G" << endl;
		//	quit
		if (player -> is_dead () || input_engine -> get_key ("q", false)
							//	|| input_engine->is_key_down (OIS :: KC_ESCAPE)
								|| input_engine -> get_key ("Escape", false)
								|| window -> isClosed())
		{
		debug () << "Main loop - H" << endl;
			running = false;
		}

		//	hit
		debug () << "Main loop - I" << endl;
		if (input_engine -> get_key ("h", true))
		{
			debug () << "Main loop - J" << endl;
			Character * npc = active_sector -> get_child <NPC> ();
			debug () << battle_engine . hit (player, npc) << endl;
		}
		
		//	transfer
		debug () << "Main loop - K" << endl;
		if (input_engine -> get_key ("t", true))
		{
			debug () << "Key 't' was pressed." << endl;
			Character * npc = active_sector -> get_child <NPC> ();
			assert (npc != NULL);
			assert (npc -> is_initialized ());
			if (player -> has_weapon ())
			{
				player -> move_to (player -> get_weapon (), npc);
				assert (npc -> has_weapon ());
			}
			else
			{
				npc -> move_to (npc -> get_weapon (), player);
			}
		}
		
		debug () << "Main loop - L" << endl;
	}
}
