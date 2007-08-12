#include "audio_engine.hpp"
#include "game.hpp"
#include "input_engine.hpp"
#include "log.hpp"
#include "menu_state.hpp"
#include "play_state.hpp"
#include "settings.hpp"
#include "tracker.hpp"

#include <OgreColourValue.h>
#include <OgreDefaultHardwareBufferManager.h>

using namespace std;
using namespace TSL;
using namespace TSL :: Engines;

//	static
const string Game ::
	get_class_name ()
{
	return "Game";
}

Game ::
	Game (string tsl_path, string ogre_media_path) :
	Object ("game")
{
	Log :: trace (me, Game :: get_class_name (), "", tsl_path, ogre_media_path);

	#ifdef TSL_DEBUG
		Log :: log (me) << "Debug mode is enabled." << endl;
		Log :: log (me) << "Further logs will be written to '" << tsl_path << "/log/log.txt'." << endl;

		//	'Log :: log (me)' is redirected to a log file.
		cout . rdbuf ((new ofstream ((tsl_path + "/log/log.txt") . c_str ())) -> rdbuf ());
		cerr . rdbuf (Log :: log (me) . rdbuf ());
		clog . rdbuf (Log :: log (me) . rdbuf ());
	#else
		Log :: log (me) << "Debug mode is disabled." << endl;
		Log :: log (me) << "No further logging will occur." << endl;

		//	From here on, all Log :: log (me) messages are ignored.
		Log :: log (me) . rdbuf ((new ostream (new stringbuf (ios_base :: out))) -> rdbuf ());
	#endif

	new Tracker ();
	new Log ();
	new Settings (tsl_path);

	new Audio_Engine ();

	//	Don't copy the log to the console. Store the log to a file, if debugging.
	// (new Ogre :: LogManager ()) -> createLog (tsl_path + "/log/ogre.txt", true, false, ! debugging);
	root = new Ogre :: Root (tsl_path + "/data/plugins.cfg", tsl_path + "/data/ogre.cfg", tsl_path + "/log/ogre.log");
	if (! root -> showConfigDialog ())
	{
		Log :: error (me) << "An Ogre configuration dialog problem occurred." << endl;
		abort ();
	}

	try	// Catch any errors
	{
		// Add textures directory
		Ogre :: ResourceGroupManager :: getSingleton () . addResourceLocation
					(tsl_path + "/data/texture", "FileSystem", "textures", true);

		// Add 3D models directory
		Ogre :: ResourceGroupManager :: getSingleton () . addResourceLocation
					(tsl_path + "/data/model", "FileSystem", "models", true);

		// Add materials directory
		Ogre :: ResourceGroupManager :: getSingleton () . addResourceLocation
					(tsl_path + "/data/material", "FileSystem", "materials", true);

		// Add gui config directory
		Ogre :: ResourceGroupManager :: getSingleton () . addResourceLocation
					(tsl_path + "/data/gui/config", "FileSystem", "gui", true);

		// Add gui font directory
		Ogre :: ResourceGroupManager :: getSingleton () . addResourceLocation
					(tsl_path + "/data/gui/font", "FileSystem", "gui", true);

		// Add gui imageset directory
		Ogre :: ResourceGroupManager :: getSingleton () . addResourceLocation
					(tsl_path + "/data/gui/imageset", "FileSystem", "gui", true);

		// Add gui looknfeel directory
		Ogre :: ResourceGroupManager :: getSingleton () . addResourceLocation
					(tsl_path + "/data/gui/looknfeel", "FileSystem", "gui", true);

		// Add gui scheme directory
		Ogre :: ResourceGroupManager :: getSingleton () . addResourceLocation
					(tsl_path + "/data/gui/scheme", "FileSystem", "gui", true);

		Ogre :: ResourceGroupManager :: getSingleton () . addResourceLocation
					(ogre_media_path + "/gui", "FileSystem", "gui", true);

		Ogre :: ResourceGroupManager :: getSingleton () . addResourceLocation
					(ogre_media_path + "/fonts", "FileSystem", "gui", true);

		Ogre :: ResourceGroupManager :: getSingleton () . addResourceLocation
					(ogre_media_path + "/models", "FileSystem", "models", true);

		window = root -> initialise (true, "The Scattered Lands");

		Ogre :: MeshManager :: getSingleton () . createPlane
			("ground.mesh", "models", Ogre :: Plane (Ogre :: Vector3 (1, 0, 0), 0), 20, 20);

		// Initialise our resources
		Ogre :: ResourceGroupManager :: getSingleton () . initialiseAllResourceGroups ();

		//	set default mipmap level (NB some APIs ignore this)
		Ogre :: TextureManager :: getSingleton() . setDefaultNumMipmaps (5);

		new Input_Engine (* window);

		Ogre :: SceneManager & scene_manager = * root -> createSceneManager (Ogre :: ST_GENERIC);

		new GUI_Engine (* window, scene_manager);

		new World (scene_manager);

		new Strategies :: Menu_State ();
		new Strategies :: Play_State (scene_manager);
		set_active_state (Strategies :: Play_State :: get ());

		Ogre :: Camera * camera = scene_manager . getCameraIterator () . getNext ();
		assert (camera != NULL);

		root -> getRenderSystem () -> _setViewport (window -> addViewport (camera));
		root -> getRenderSystem () -> _getViewport () -> setBackgroundColour (Ogre :: ColourValue :: Blue);

	}	// try
	catch (Ogre :: Exception & exception)
	{
		Log :: error (me) << "Exception: " << exception . getFullDescription () << endl;
		abort ();
	}

	assert (is_initialized ());
}

Game ::
	~Game ()
{
	Log :: trace (me, Game :: get_class_name (), "~");
	assert (is_initialized ());

	delete & Strategies :: Play_State :: get ();
	delete & World :: get ();
	delete & Settings :: get ();
	delete & GUI_Engine :: get ();
	//	delete & Input_Engine :: get ();	//	Already auto-destructed.
	delete & Audio_Engine :: get ();
	delete & Log :: get ();
	delete & Tracker :: get ();
}

//	virtual
bool Game ::
	is_initialized ()
	const
{
	assert (Singleton <Game> :: is_initialized ());
	assert (Strategies :: Strategy_State_Machine :: is_initialized ());

	return true;
}

void Game ::
	run ()
{
	const Object * message = & Object :: update;
	while (has_active_state ())
	{
		if (window -> isClosed ())
		{
			message = & Object :: terminate;
		}

		Input_Engine :: get () . capture ();
		Ogre :: WindowEventUtilities :: messagePump ();

		Strategies :: Strategy_State_Machine :: run (* message, true);

		bool check = root -> renderOneFrame ();
		assert (check);
		GUI_Engine :: get () . render ();
	}
}
