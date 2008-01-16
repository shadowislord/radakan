#include <OgreColourValue.h>
#include <OgreDefaultHardwareBufferManager.h>
#include <OgreMeshManager.h>
#include <OgreRoot.h>
#include <OgreTextureManager.h>

#include "engines/audio_engine.hpp"
#include "engines/conversation_engine.hpp"
#include "engines/game.hpp"
#include "engines/gui_engine.hpp"
#include "engines/input_engine.hpp"
#include "engines/log.hpp"
#include "engines/settings.hpp"
#include "engines/tracker.hpp"
#include "messages/message.hpp"
#include "strategies/game_modes/menu.hpp"
#include "strategies/game_modes/play.hpp"
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
	Game (string path_to_config) :
	Object ("game")
{
	Log :: trace (me, Game :: get_class_name (), "", path_to_config);
	
	new Settings (path_to_config);
	
	// These strings are loaded from a configuration file using the Settings class.
	string radakan_path = Settings :: get() -> radakan_path;
	string ogre_media_path = Settings :: get() -> ogre_media_path;

	#ifdef RADAKAN_DEBUG
		Log :: log (me) << "Debug mode is enabled." << endl;

		new Tracker ();
		new Log (radakan_path);
	#else
		Log :: log (me) << "Debug mode is disabled." << endl;

		Log :: no_logs (me);
	#endif

	#if RADAKAN_AUDIO_MODE == RADAKAN_AUDIERE_MODE
		new Audio_Engine ();
	#endif

	root . reset (new Ogre :: Root (radakan_path + "/data/plugins.cfg", radakan_path + "/data/ogre.cfg", radakan_path + "/log/ogre.txt"));
	if (! root -> showConfigDialog ())
	{
		Log :: error (me) << "An Ogre configuration dialog problem occurred." << endl;
		abort ();
	}

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

	new Conversation_Engine ();
	new Strategies :: Game_Modes :: Menu ();
	new Strategies :: Game_Modes :: Play (window, scene_manager);
	set_active_state (Strategies :: Game_Modes :: Play :: get ());

	Log :: show
		("Radakan is a single-player sandbox 3D RPG...");
	Log :: show
		("...in a dark dynamic fantasy world.");
	Log :: show
		("Check the player guide:");
	Log :: show
		("http://radakan.org/index.php?title=Category:Player_guide");

	assert (is_initialized ());
}

Game ::
	~Game ()
{
	Log :: trace (me, Game :: get_class_name (), "~");
	assert (is_initialized ());

	Conversation_Engine :: uninstantiate ();
	Strategies :: Game_Modes :: Menu :: uninstantiate ();
	Strategies :: Game_Modes :: Play :: uninstantiate ();
	World :: uninstantiate ();
	Settings :: uninstantiate ();
	GUI_Engine :: uninstantiate ();
	Input_Engine :: uninstantiate ();

	#if RADAKAN_AUDIO_MODE == RADAKAN_AUDIERE_MODE
		Audio_Engine :: uninstantiate ();
	#endif

	#ifdef RADAKAN_DEBUG
		Log :: uninstantiate ();
		Tracker :: uninstantiate ();
	#endif
}

//	virtual
bool Game ::
	is_initialized ()
	const
{
	assert (Singleton <Game> :: is_initialized ());
	//	assert (Strategy_State_Machine <Strategies :: Game_Modes :: Game_Mode, Object> :: is_initialized ());	//	'assert' can't handle double templates.

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
			Strategy_State_Machine <Strategies :: Game_Modes :: Game_Mode, Object> :: run
				(Messages :: Message <Object> :: terminate);
		}
		else
		{
			Input_Engine :: get () -> capture ();
			Ogre :: WindowEventUtilities :: messagePump ();

			Strategy_State_Machine <Strategies :: Game_Modes :: Game_Mode, Object> :: run
				(Messages :: Message <Object> :: update);

			bool check = root -> renderOneFrame ();
			assert (check);
			GUI_Engine :: get () -> render ();
		}
	}

	Log :: trace (me, Game :: get_class_name (), "end of run");
}
