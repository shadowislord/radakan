#include "tsl.hpp"
#include "play_state.hpp"
#include <algorithm>

using namespace std;
using namespace tsl;

TSL ::
	TSL (string tsl_path, string ogre_path) :
	Object ("TSL"),
	Singleton <TSL> ("TSL"),
	State_Machine <TSL> (* this),
	go_on ("go on"),
	quit ("quit")
{
	trace () << "TSL (" << tsl_path << ", " << ogre_path << ")" << endl;

	audio_engine = new Audio_Engine ();
	audio_engine -> load (tsl_path + "/data/sound/prelude_11.ogg");
	audio_engine -> play ();

	//	Don't copy the log to the console. Store the log to a file, if debugging.
	(new Ogre :: LogManager ()) -> createLog (tsl_path + "/log/ogre.txt", true, false,
		#ifdef TSL_DEBUG
			false
		#else
			true
		#endif
		);
	root = new Ogre :: Root (tsl_path + "/data/plugins.cfg",tsl_path + "/data/ogre.cfg");
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
					(ogre_path + "/Samples/Media/gui", "FileSystem", "gui", true);
					
		Ogre :: ResourceGroupManager :: getSingleton () . addResourceLocation
					(ogre_path + "/Samples/Media/fonts", "FileSystem", "gui", true);

		Ogre :: ResourceGroupManager :: getSingleton () . addResourceLocation
					(ogre_path + "/Samples/Media/models", "FileSystem", "models", true);

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
	Ogre :: MeshManager :: getSingleton (). createPlane ("plane.mesh", "custom", Ogre :: Plane (Ogre :: Vector3 :: UNIT_Z, Ogre :: Vector3 :: ZERO), 20000, 20000, 20, 20);

	//	set default mipmap level (NB some APIs ignore this)
	Ogre :: TextureManager :: getSingleton() . setDefaultNumMipmaps (5);	

	input_engine = new Input_Engine (* window);

	gui_engine = new GUI_Engine (* window, tsl_path + "/log/cegui.txt", * input_engine);

	State_Machine <TSL> :: add <Play_State> ();

	assert (get_active_state () . is_type <Sector> ());
	Sector & active_sector = get_active_state () . to_type <Sector> ();

	root -> getRenderSystem () -> _setViewport
			(window -> addViewport (& active_sector . get_camera ()));
	gui_engine -> set_scene_manager (active_sector . get_scene_manager ());

	turn_lenght_timer = Ogre :: PlatformManager :: getSingleton () . createTimer ();
	last_turn_lenght = 0;

	assert (is_initialized ());
}

TSL ::
	~TSL ()
{
	trace () << "~TSL ()" << endl;
	assert (is_initialized ());
	
	delete input_engine;
	delete audio_engine;
	delete gui_engine;
}

//	virtual
bool TSL ::
	is_initialized ()
	const
{
	return warn <TSL> (State_Machine <TSL> :: is_initialized ());
}

//	static
string TSL ::
	get_class_name ()
{
	return "TSL";
}

string TSL ::
	run ()
{
	//	Of course, the program should not quit when you die, but it should do *something*. To make sure the program does not crash later, it currently does shut down when you die.
	while
	(
		(State_Machine <TSL> :: run () == go_on)
		&& root -> renderOneFrame ()
		&& gui_engine -> render ()
		&& ! Player :: get () . is_dead ()
		&& ! window -> isClosed ()
	)
	{
		input_engine -> capture ();
		Ogre :: PlatformManager :: getSingleton () . messagePump (window);

		last_turn_lenght = turn_lenght_timer -> getMilliseconds ();
		turn_lenght_timer -> reset ();
	}

	//	The return value doesn't really matter here.
	return quit;
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

int TSL ::
	get_last_turn_lenght () const
{
	return last_turn_lenght;
}
