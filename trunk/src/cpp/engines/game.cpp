#include <OgreColourValue.h>
#include <OgreDefaultHardwareBufferManager.h>
#include <OgreMeshManager.h>
#include <OgreRoot.h>
#include <OgreTextureManager.h>

#include "engines/audio_engine.hpp"
#include "engines/game.hpp"
#include "engines/gui_engine.hpp"
#include "engines/input_engine.hpp"
#include "engines/log.hpp"
#include "engines/settings.hpp"
#include "engines/tracker.hpp"
#include "messages/message.hpp"
#include "strategies/menu_state.hpp"
#include "strategies/play_state.hpp"
#include "world.hpp"

using namespace std;
using namespace Radakan;
using namespace Radakan :: Engines;

//	static
string Game ::
	get_class_name ()
{
	return "Game";
}

Game ::
	Game (string radakan_path, string ogre_media_path) :
	Object ("game")
{
	Log :: trace (me, Game :: get_class_name (), "", radakan_path, ogre_media_path);

	#ifdef RADAKAN_DEBUG
		Log :: log (me) << "Debug mode is enabled." << endl;

		new Log (radakan_path);
		new Tracker ();
	#else
		Log :: log (me) << "Debug mode is disabled." << endl;

		Log :: no_logs (me);
	#endif

	new Settings (radakan_path);

	new Audio_Engine ();

	root . reset (new Ogre :: Root (radakan_path + "/data/plugins.cfg", radakan_path + "/data/ogre.cfg", radakan_path + "/log/ogre.txt"));
	if (! root -> showConfigDialog ())
	{
		Log :: error (me) << "An Ogre configuration dialog problem occurred." << endl;
		abort ();
	}

	try	// Catch any errors
	{
		// Add textures directory
		Ogre :: ResourceGroupManager :: getSingleton () . addResourceLocation
					(radakan_path + "/data/texture", "FileSystem", "textures", true);

		// Add 3D models directory
		Ogre :: ResourceGroupManager :: getSingleton () . addResourceLocation
					(radakan_path + "/data/model", "FileSystem", "models", true);

		// Add materials directory
		Ogre :: ResourceGroupManager :: getSingleton () . addResourceLocation
					(radakan_path + "/data/material", "FileSystem", "materials", true);

		// Add gui config directory
		Ogre :: ResourceGroupManager :: getSingleton () . addResourceLocation
					(radakan_path + "/data/gui/config", "FileSystem", "gui", true);

		// Add gui font directory
		Ogre :: ResourceGroupManager :: getSingleton () . addResourceLocation
					(radakan_path + "/data/gui/font", "FileSystem", "gui", true);

		// Add gui imageset directory
		Ogre :: ResourceGroupManager :: getSingleton () . addResourceLocation
					(radakan_path + "/data/gui/imageset", "FileSystem", "gui", true);

		// Add gui looknfeel directory
		Ogre :: ResourceGroupManager :: getSingleton () . addResourceLocation
					(radakan_path + "/data/gui/looknfeel", "FileSystem", "gui", true);

		// Add gui scheme directory
		Ogre :: ResourceGroupManager :: getSingleton () . addResourceLocation
					(radakan_path + "/data/gui/scheme", "FileSystem", "gui", true);

		Ogre :: ResourceGroupManager :: getSingleton () . addResourceLocation
					(ogre_media_path + "/gui", "FileSystem", "gui", true);

		Ogre :: ResourceGroupManager :: getSingleton () . addResourceLocation
					(ogre_media_path + "/fonts", "FileSystem", "gui", true);

		Ogre :: ResourceGroupManager :: getSingleton () . addResourceLocation
					(ogre_media_path + "/models", "FileSystem", "models", true);

		window . reset (root -> initialise (true, "Radakan"));

		Ogre :: MeshManager :: getSingleton () . createPlane
			("ground.mesh", "models", Ogre :: Plane (Ogre :: Vector3 (1, 0, 0), 0), 20, 20);

		// Initialise our resources
		Ogre :: ResourceGroupManager :: getSingleton () . initialiseAllResourceGroups ();

		//	set default mipmap level (NB some APIs ignore this)
		Ogre :: TextureManager :: getSingleton() . setDefaultNumMipmaps (5);

		new Input_Engine (window);

		scene_manager . reset (root -> createSceneManager (Ogre :: ST_GENERIC));
		assert (scene_manager);

		new GUI_Engine (window, scene_manager);

		new World (scene_manager);

		new Strategies :: Menu_State ();
		new Strategies :: Play_State (scene_manager);
		set_active_state (Strategies :: Play_State :: get ());

		Ogre :: Camera * camera = scene_manager -> getCameraIterator () . getNext ();

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

	Strategies :: Play_State :: uninstantiate ();
	World :: uninstantiate ();
	Settings :: uninstantiate ();
	GUI_Engine :: uninstantiate ();
	//	Input_Engine :: uninstantiate ();	//	Already auto-destructed.
	Audio_Engine :: uninstantiate ();

	#ifdef RADAKAN_DEBUG
		Tracker :: uninstantiate ();
	#endif

	Log :: uninstantiate ();
}

//	virtual
bool Game ::
	is_initialized ()
	const
{
	assert (Singleton <Game> :: is_initialized ());
	assert (Strategies :: Strategy_State_Machine <Game> :: is_initialized ());

	return true;
}

void Game ::
	run ()
{
	Log :: trace (me, Game :: get_class_name (), "run");
	
	while (has_active_state ())
	{
		if (window -> isClosed ())
		{
			Strategies :: Strategy_State_Machine <Game> :: run
				(Messages :: Message <Game> :: terminate);
		}
		else
		{
			Input_Engine :: get () -> capture ();
			Ogre :: WindowEventUtilities :: messagePump ();

			Strategies :: Strategy_State_Machine <Game> :: run
				(Messages :: Message <Game> :: update);

			bool check = root -> renderOneFrame ();
			assert (check);
			GUI_Engine :: get () -> render ();
		}
	}

	Log :: trace (me, Game :: get_class_name (), "end of run");
}
