#include "tsl.hpp"
#include "menu_state.hpp"
#include "world.hpp"
#include "quit_state.hpp"
#include "dead_state.hpp"
#include "fight_state.hpp"
#include "alive_state.hpp"
#include "audio_engine.hpp"
#include "battle_engine.hpp"
#include "input_engine.hpp"

#include <OgreColourValue.h>

using namespace std;
using namespace tsl;

const float maximal_turn_lenght = 0.1;

TSL ::
	TSL (string tsl_path, string ogre_media_path) :
	Object ("TSL")
{
	log (debugging) << "TSL (" << tsl_path << ", " << ogre_media_path << ")" << endl;

	new Audio_Engine ();
	Audio_Engine :: get () . load (tsl_path + "/data/sound/prelude_11.ogg");
	Audio_Engine :: get () . play ();

	//	Don't copy the log to the console. Store the log to a file, if debugging.
	(new Ogre :: LogManager ()) -> createLog (tsl_path + "/log/ogre.txt", true, false, ! debugging);
	root = new Ogre :: Root (tsl_path + "/data/plugins.cfg", tsl_path + "/data/ogre.cfg");
	if (! root -> showConfigDialog ())
	{
		error () << " detected an Ogre configuration dialog problem." << endl;
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

	}	// End of try statement
	catch (Ogre :: Exception & exception)
	{
		#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
			MessageBox
				(NULL, exception . getFullDescription () . c_str (),
				"An exception has occured!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
		#else
			error () << "detected an exception: " << exception . getFullDescription () << endl;
		#endif
		abort ();
	}

	window = root -> initialise (true, "The Scattered Lands");

	//	set default mipmap level (NB some APIs ignore this)
	Ogre :: TextureManager :: getSingleton() . setDefaultNumMipmaps (5);	

	new Input_Engine (* window);

	Ogre :: SceneManager * scene_manager = root -> createSceneManager (Ogre :: ST_GENERIC);
	new GUI_Engine
	(
		* window,
		* scene_manager,
		tsl_path + "/log/cegui.txt"
	);

	new World
	(
		* scene_manager,
		tsl_path
	);
	new Menu_State ();
	new Quit_State ();

	Ogre :: Camera * camera = scene_manager -> getCameraIterator () . getNext ();
	assert (camera != NULL);

	Algorithm_State_Machine <TSL> :: set_active_state (World :: get ());
	assert (get_active_state () . is_type <World> ());

	root -> getRenderSystem () -> _setViewport (window -> addViewport (camera));
	root -> getRenderSystem () -> _getViewport () -> setBackgroundColour (Ogre :: ColourValue :: Blue);

	last_turn_lenght = 0;

	assert (is_initialized ());
}

TSL ::
	~TSL ()
{
	log (debugging) << "~" << get_class_name () << " ()" << endl;
	assert (is_initialized ());
	log (debugging) << "active state: " << get_active_state () << endl;
	assert (get_active_state () == Quit_State :: get ());

	delete & Input_Engine :: get ();
	delete & Audio_Engine :: get ();
	delete & GUI_Engine :: get ();

	unset_active_state ();

	delete & Menu_State :: get ();
	delete & World :: get ();
	delete & Quit_State :: get ();

	delete & Alive_State :: get ();
	delete & Dead_State :: get ();
	delete & Fight_State :: get ();
}

//	virtual
bool TSL ::
	is_initialized ()
	const
{
	assert (Singleton <TSL> :: is_initialized ());
	assert (Algorithm_State_Machine <TSL> :: is_initialized ());

	return true;
}

//	static
string TSL ::
	get_class_name ()
{
	return "TSL";
}

void TSL ::
	run ()
{
	while (get_active_state () != Quit_State :: get ())
	{
		#ifdef TSL_DEBUG
			turn ++;
		#endif

		bool check = root -> renderOneFrame ();
		assert (check);
		check = GUI_Engine :: get () . render ();
		assert (check);
	
		Input_Engine :: get () . capture ();
		Ogre :: WindowEventUtilities :: messagePump ();

		log (debugging) << "Turn lenth (part one): " << float (turn_lenght_timer . getMilliseconds ()) / 1000 << endl;

		Algorithm_State_Machine <TSL> :: run ();

		last_turn_lenght = float (turn_lenght_timer . getMilliseconds ()) / 1000;

		log () << "Turn lenght: " << last_turn_lenght;

		if (maximal_turn_lenght < last_turn_lenght)
		{
			log (debugging) << "Reducing the turn lenght from " << last_turn_lenght << " to " << maximal_turn_lenght << "..." << endl;
			last_turn_lenght = maximal_turn_lenght;
		}
		turn_lenght_timer . reset ();

		if (window -> isClosed ())
		{
			Algorithm_State_Machine <TSL> :: set_active_state (Quit_State :: get ());
		}
	}
	
	assert (get_active_state () == Quit_State :: get ());
}

string TSL ::
	get_FPS () const
{
	assert (is_initialized ());

	return "FPS: " + to_string (window -> getAverageFPS ());
}

void TSL ::
	set_camera (Ogre :: Camera & new_camera)
{
	assert (is_initialized ());

	root -> getRenderSystem () -> _getViewport () -> setCamera (& new_camera);
}

const float & TSL ::
	get_last_turn_lenght () const
{
	return last_turn_lenght;
}
