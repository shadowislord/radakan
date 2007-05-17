#include "audio_engine.hpp"
#include "battle_engine.hpp"
#include "chat_state.hpp"
#include "dead_state.hpp"
#include "game.hpp"
#include "fight_state.hpp"
#include "input_engine.hpp"
#include "log.hpp"
#include "menu_state.hpp"
#include "quit_state.hpp"
#include "world.hpp"

#include <OgreColourValue.h>

using namespace std;
using namespace TSL;

//	static
const string Game ::
	get_class_name ()
{
	return "Game";
}

Game ::
	Game (string tsl_path, string ogre_media_path) :
	Object ("Game"),
	last_turn_lenght (0)
{
	Log :: trace <Game> (me, "", tsl_path, ogre_media_path);

	new Log ();

	new Audio_Engine ();
	Audio_Engine :: get () . load (tsl_path + "/data/sound/prelude_11.ogg");
	Audio_Engine :: get () . play ();

	//	Don't copy the log to the console. Store the log to a file, if debugging.
	(new Ogre :: LogManager ()) -> createLog (tsl_path + "/log/ogre.txt", true, false, ! debugging);
	root = new Ogre :: Root (tsl_path + "/data/plugins.cfg", tsl_path + "/data/ogre.cfg");
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

		// Initialise our resources
		Ogre :: ResourceGroupManager :: getSingleton () . initialiseAllResourceGroups ();

		window = root -> initialise (true, "The Scattered Lands");

		//	set default mipmap level (NB some APIs ignore this)
		Ogre :: TextureManager :: getSingleton() . setDefaultNumMipmaps (5);	

		new Input_Engine (* window);

		Ogre :: SceneManager & scene_manager = * root -> createSceneManager (Ogre :: ST_GENERIC);
		new GUI_Engine (* window, scene_manager, tsl_path + "/log/cegui.txt");

		new World (scene_manager, tsl_path);
		new Menu_State ();
		new Quit_State ();

		Algorithm_State_Machine <Game> :: set_active_state (World :: get ());
		assert (get_active_state () . is_type <World> ());

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
	Log :: trace <Game> (me, "~");
	assert (is_initialized ());
	Log :: log (me) << "active state: " << get_active_state () << endl;
	assert (get_active_state () == Quit_State :: get ());

	unset_active_state ();

	Menu_State :: destruct ();
	Quit_State :: destruct ();
	World :: destruct ();

	Chat_State :: destruct ();
	Fight_State :: destruct ();
	Alive_State :: destruct ();
	Dead_State :: destruct ();
	
	GUI_Engine :: destruct ();
	Input_Engine :: destruct ();
	Audio_Engine :: destruct ();

	Log :: destruct ();
}

//	virtual
bool Game ::
	is_initialized ()
	const
{
	assert (Singleton <Game> :: is_initialized ());
	assert (Algorithm_State_Machine <Game> :: is_initialized ());

	return true;
}

void Game ::
	run ()
{
	while (get_active_state () != Quit_State :: get ())
	{
		#ifdef TSL_DEBUG
			turn ++;
		#endif

		bool check = root -> renderOneFrame ();
		assert (check);
		GUI_Engine :: get () . render ();

		Input_Engine :: get () . capture ();
		Ogre :: WindowEventUtilities :: messagePump ();

		Algorithm_State_Machine <Game> :: run ();

		last_turn_lenght = float (turn_lenght_timer . getMilliseconds ()) / 1000;

		Log :: log (me) << "Turn lenght: " << last_turn_lenght << endl;

//		if (maximal_turn_lenght < last_turn_lenght)
//		{
//			last_turn_lenght = maximal_turn_lenght;
//		}
		turn_lenght_timer . reset ();

		if (window -> isClosed ())
		{
			Algorithm_State_Machine <Game> :: set_active_state (Quit_State :: get ());
		}
	}
	
	assert (get_active_state () == Quit_State :: get ());
}

string Game ::
	get_FPS () const
{
	assert (is_initialized ());

	return "FPS: " + to_string (window -> getAverageFPS ());
}

void Game ::
	set_camera (Ogre :: Camera & new_camera)
{
	assert (is_initialized ());

	root -> getRenderSystem () -> _getViewport () -> setCamera (& new_camera);
}

const float & Game ::
	get_last_turn_lenght () const
{
	return last_turn_lenght;
}
