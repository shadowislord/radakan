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

	window = root->initialise (true);

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
//	debug () << "deleting scene_mgr... " << int (scene_mgr) << endl;
//	delete scene_mgr;
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
	Event * event;

	input_engine -> event_queue . push (new Move_Event (Move_Event :: forward));
	input_engine -> event_queue . push (new Hit_Event (player, player));
	input_engine -> event_queue . push (new Exit_Event ());

	while (running)
	{
		root->renderOneFrame ();
		
		event = input_engine -> process (NULL);

		if (event != NULL)
		{
			if (event -> is_type <Exit_Event> ())
			{
				delete event;
				running = false;
			}
			else if (event -> is_type <Hit_Event> ())
			{
				event = battle_engine . process (event);
			}
			else
			{
				delete event;
			}

			//	Sometimes a new event was generated:
			if (event != NULL)
			{
				input_engine -> event_queue . push (event);
			}
		}
	}
}
