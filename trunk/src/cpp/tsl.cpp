#include <algorithm>
#include "tsl.hpp"
#include "menu_state.hpp"
#include "play_state.hpp"
#include "quit_state.hpp"
#include "dead_state.hpp"
#include "fight_state.hpp"
#include "peace_state.hpp"

using namespace std;
using namespace tsl;

TSL ::
	TSL (string tsl_path, string ogre_media_path) :
	Object ("TSL")
{
	trace () << "TSL (" << tsl_path << ", " << ogre_media_path << ")" << endl;

	new Audio_Engine ();
	Audio_Engine :: get () . load (tsl_path + "/data/sound/prelude_11.ogg");
	Audio_Engine :: get () . play ();

	//	Don't copy the log to the console. Store the log to a file, if debugging.
	(new Ogre :: LogManager ()) -> createLog (tsl_path + "/log/ogre.txt", true, false,
		#ifdef TSL_DEBUG
			false
		#else
			true
		#endif
		);
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

	new GUI_Engine (* window, tsl_path + "/log/cegui.txt", Input_Engine :: get ());

	new Menu_State ();
	new Play_State ();
	new Quit_State ();

	Algorithm_State_Machine <TSL> :: set_active_state <Play_State> ();

	assert (get_active_state () . is_type <Play_State> ());
	assert (get_active_state () . is_type <Data_State_Machine <Sector> > ());
	Sector & active_sector = get_active_state () . to_type <Data_State_Machine <Sector> > () . get_active_state ();

	root -> getRenderSystem () -> _setViewport
			(window -> addViewport (& active_sector . get_camera ()));
	GUI_Engine :: get () . set_scene_manager (* active_sector . getSceneManager ());

	turn_lenght_timer = Ogre :: PlatformManager :: getSingleton () . createTimer ();
	last_turn_lenght = 0;

	new Battle_Engine (); 

	assert (is_initialized ());
}

TSL ::
	~TSL ()
{
	trace () << "~" << get_class_name () << " ()" << endl;
	assert (is_initialized ());
	trace () << "active state: " << get_active_state () << endl;
	assert (get_active_state () == Quit_State :: get ());

	delete & Input_Engine :: get ();
	delete & Audio_Engine :: get ();
	delete & GUI_Engine :: get ();
	delete & Battle_Engine :: get ();

	unset_active_state ();

	delete & Menu_State :: get ();
	delete & Play_State :: get ();
	delete & Quit_State :: get ();

	delete & Dead_State :: get ();
	delete & Fight_State :: get ();
	delete & Peace_State :: get ();
}

//	virtual
bool TSL ::
	is_initialized ()
	const
{
	assert (warn <TSL> (Singleton <TSL> :: is_initialized ()));
	assert (warn <TSL> (Algorithm_State_Machine <TSL> :: is_initialized ()));

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
		bool check = root -> renderOneFrame ();
		assert (check);
		check = GUI_Engine :: get () . render ();
		assert (check);
	
		Input_Engine :: get () . capture ();
		Ogre :: PlatformManager :: getSingleton () . messagePump (window);

		Algorithm_State_Machine <TSL> :: run ();
		
		last_turn_lenght = turn_lenght_timer -> getMilliseconds ();
		turn_lenght_timer -> reset ();

		turn ++;

		if (window -> isClosed ())
		{
			Algorithm_State_Machine <TSL> :: set_active_state <Quit_State> ();
		}
	}
	
	assert (get_active_state () == Quit_State :: get ());
}

Ogre :: SceneManager & TSL ::
	new_scene_manager () const
{
	assert (is_initialized ());

	return * root -> createSceneManager (Ogre :: ST_GENERIC);
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

unsigned long TSL ::
	get_last_turn_lenght () const
{
	return last_turn_lenght;
}
