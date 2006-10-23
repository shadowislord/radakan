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
					(path + "/data/texture", "FileSystem", "textures", true);

		// Add 3D models directory
		Ogre :: ResourceGroupManager :: getSingleton ().addResourceLocation
					(path + "/data/model", "FileSystem", "models", true);

		// Add materials directory
		Ogre :: ResourceGroupManager :: getSingleton ().addResourceLocation
					(path + "/data/material", "FileSystem", "material", true);
					
		// Add gui directory
		Ogre :: ResourceGroupManager :: getSingleton ().addResourceLocation
					(path + "/data/gui", "FileSystem", "gui", true);

		// Add fonts directory
		Ogre :: ResourceGroupManager :: getSingleton ().addResourceLocation
					(path + "/data/fonts", "FileSystem", "fonts", true);

		// Initialise our resources
		Ogre :: ResourceGroupManager :: getSingleton ().initialiseAllResourceGroups ();
	}	// End of try statement
	catch (Ogre :: Exception & e)
	{
		#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
			MessageBox (NULL, e . getFullDescription () . c_str (), "An exception has occured!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
		#else
			cerr << "An exception has occured: " << e . getFullDescription () << endl;
		#endif
		abort ();
	}

	window = root->initialise (true, "Scattered Lands");


	Ogre :: SceneManager * scene_manager = root->createSceneManager
														(Ogre :: ST_GENERIC);
	active_sector = new Sector ("Sector 1", scene_manager, window);
	sectors.insert (active_sector);

	gui_engine = new GUI_Engine (window, scene_manager);
	
	//	Set default mipmap level (NB some APIs ignore this)
	Ogre :: TextureManager :: getSingleton() . setDefaultNumMipmaps (5);

	Ogre :: ResourceGroupManager :: getSingleton () . initialiseAllResourceGroups ();

	//	This is the new input mechanism that is taking advantage of the
	//	new engine handler.
	input_engine = new Input_Engine (window);	

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

	delete gui_engine;
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
	
	while (true)
	{
		active_sector -> update ();
	
		input_engine -> capture ();

		Ogre :: PlatformManager :: getSingletonPtr () -> messagePump (window);

		if (! root -> renderOneFrame ())
		{
			break;
		}

		//	!!!
		gui_engine -> render ();

		//	quit
		//	Of course, the program should not quit when you die, but it should do *something*. To make sure the program does not crash later, it currently does shut down when you die.
		if (player -> is_dead () || input_engine -> get_key ("q", false)
								|| input_engine -> get_key ("Escape", false)
								|| window -> isClosed())
		{
			break;
		}

		//	hit
		if (input_engine -> get_key ("h", true))
		{
			Character * npc = active_sector -> get_child <NPC> ();
			if (! npc -> is_dead ())
			{
				debug () << battle_engine . hit (player, npc) << endl;
			}
		}
		
		//	transfer
		if (input_engine -> get_key ("t", true))
		{
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
	}
}
