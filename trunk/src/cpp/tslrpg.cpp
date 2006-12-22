#include "tslrpg.hpp"
#include <algorithm>

using namespace std;
using namespace sl;

template <> Tslrpg * Ogre :: Singleton <Tslrpg> :: ms_Singleton = NULL;

Tslrpg ::
	Tslrpg (string sl_path, string ogre_path) :
	Object ("Tslrpg"),
//	State_Machine (NULL),
	Set <Sector> ("Tslrpg")
{
	trace () << "Tslrpg (" << sl_path << ", " << ogre_path << ")" << endl;

	root = new Ogre :: Root ("data/plugins.cfg", "data/ogre.cfg", "log/ogre.txt");
	if (! root -> showConfigDialog ())
	{
		abort ();
	}

	try	// Catch any errors
	{
//		new Ogre :: GLGpuProgramManager ();
	
		// Add textures directory
		Ogre :: ResourceGroupManager :: getSingleton () . addResourceLocation
					(sl_path + "/data/texture", "FileSystem", "textures", true);

		// Add 3D models directory
		Ogre :: ResourceGroupManager :: getSingleton () . addResourceLocation
					(sl_path + "/data/model", "FileSystem", "models", true);

		// Add materials directory
		Ogre :: ResourceGroupManager :: getSingleton () . addResourceLocation
					(sl_path + "/data/material", "FileSystem", "materials", true);
					
		// Add gui config directory
		Ogre :: ResourceGroupManager :: getSingleton () . addResourceLocation
					(sl_path + "/data/gui/config", "FileSystem", "gui", true);
					
		// Add gui font directory
		Ogre :: ResourceGroupManager :: getSingleton () . addResourceLocation
					(sl_path + "/data/gui/font", "FileSystem", "gui", true);
					
		// Add gui imageset directory
		Ogre :: ResourceGroupManager :: getSingleton () . addResourceLocation
					(sl_path + "/data/gui/imageset", "FileSystem", "gui", true);

		// Add gui looknfeel directory
		Ogre :: ResourceGroupManager :: getSingleton () . addResourceLocation
					(sl_path + "/data/gui/looknfeel", "FileSystem", "gui", true);

		// Add gui scheme directory
		Ogre :: ResourceGroupManager :: getSingleton () . addResourceLocation
					(sl_path + "/data/gui/scheme", "FileSystem", "gui", true);

		Ogre :: ResourceGroupManager :: getSingleton () . addResourceLocation
					(ogre_path + "/Samples/Media/gui", "FileSystem", "gui", true);
					
		Ogre :: ResourceGroupManager :: getSingleton () . addResourceLocation
					(ogre_path + "/Samples/Media/fonts", "FileSystem", "gui", true);

		Ogre :: ResourceGroupManager :: getSingleton () . addResourceLocation
					(ogre_path + "/Samples/Media/models", "FileSystem", "models", true);

		// Initialise our resources
		Ogre :: ResourceGroupManager :: getSingleton () . initialiseAllResourceGroups ();
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
	
	window = root -> initialise (true, "Scattered Lands");

	Set <Sector> :: add (* new Sector ("Sector 1",
				* root -> createSceneManager (Ogre :: ST_GENERIC)));

	Set <Sector> :: add (* new Sector ("Sector 2",
				* root -> createSceneManager (Ogre :: ST_GENERIC)));
	active_sector = Set <Sector> :: get_one_child ();

	root -> getRenderSystem () -> _setViewport
					(window -> addViewport (& active_sector -> get_camera ()));

	gui_engine = new GUI_Engine (* window, sl_path + "/log/cegui.txt");
	gui_engine -> set_scene_manager (active_sector -> get_scene_manager ());

	//	Set default mipmap level (NB some APIs ignore this)
	Ogre :: TextureManager :: getSingleton() . setDefaultNumMipmaps (5);

	//	This is the new input mechanism that is taking advantage of the
	//	new engine handler.
	input_engine = new Input_Engine (* window);

	timer = Ogre :: PlatformManager :: getSingleton () . createTimer ();
}

Tslrpg ::
	~Tslrpg ()
{
	trace () << "~Tslrpg ()" << endl;
	delete gui_engine;
	delete input_engine;
}

//	virtual
bool Tslrpg ::
	is_initialized ()
	const
{
	return warn <Tslrpg> (Set <Sector> :: is_initialized ());
}

//	static
string Tslrpg ::
	get_type_name ()
{
	return "tslrpg";
}

void Tslrpg ::
	run ()
{
	assert (is_initialized ());

	int time = 0;
	while (true)
	{
		timer -> reset ();

		active_sector -> update (time);
	
		input_engine -> capture ();
		Ogre :: PlatformManager :: getSingletonPtr () -> messagePump (window);
		
		//	Of course, the program should not quit when you die, but it should do *something*. To make sure the program does not crash later, it currently does shut down when you die.
		if (! root -> renderOneFrame ()
				|| ! gui_engine -> render ()
				|| Player :: getSingleton () . is_dead ()
				|| input_engine -> get_key ("Escape", false)
				|| window -> isClosed())
		{
			break;
		}

		//	Handle movement
		//	Normal WASD keys don't work on all keyboard layouts, so we'll use ESDF for now.
		if (input_engine -> get_key ("e", false))
		{
			Player :: getSingleton () . run (0.2 * time);
		}
		if (input_engine -> get_key ("d", false))
		{
			Player :: getSingleton () . run (- 0.1 * time);
		}
		if (input_engine -> get_key ("s", false))
		{
			Player :: getSingleton () . turn (0.005 * time);
		}
		if (input_engine -> get_key ("f", false))
		{
			Player :: getSingleton () . turn (- 0.005 * time);
		}

		//	hit
		if (input_engine -> get_key ("h", true))
		{
			NPC * npc = active_sector -> get_child <NPC> ();
			assert (npc != NULL);
			if (! npc -> is_dead ())
			{
				debug () << battle_engine . hit (Player :: getSingleton (), * npc) << endl;
			}
		}
		
		//	move the weapon
		if (input_engine -> get_key ("m", true))
		{
			//	Memo to self (Tinus):
			//	NPC npc = * active_sector -> get_child <NPC> ();
			//	that *copies* the NPC.
			NPC & npc = * active_sector -> get_child <NPC> ();
			assert (npc . is_initialized ());
			if (Player :: getSingleton () . has_weapon ())
			{
				Player :: getSingleton () . move_to (* Player :: getSingleton () . get_weapon (), npc);
				assert (! Player :: getSingleton () . has_weapon ());
				assert (npc . has_weapon ());
			}
			else
			{
				npc . Character :: move_to (* npc . get_weapon (), Player :: getSingleton ());
				assert (Player :: getSingleton () . has_weapon ());
				assert (! npc . has_weapon ());
			}
		}

		if (input_engine -> get_mouse_button (left_mouse_button, true))
		{
			debug () << left_mouse_button << endl;
		}
		
		if (input_engine -> get_mouse_button (middle_mouse_button, false))
		{
			float raw_x_offset = - 0.007 * input_engine -> get_mouse_width (true);

			if (raw_x_offset != 0)
			{
				//	apply treshold
				float x_offset = raw_x_offset / abs (raw_x_offset) * max (abs (raw_x_offset) - 0.030f, 0.f);
			
				debug () << middle_mouse_button << " " <<  raw_x_offset << " " << x_offset << endl;

				Player :: getSingleton () . turn (x_offset);
			}
		}
		
		if (input_engine -> get_key ("KP_End", true))
		{
			switch_to (Set <Sector> :: get_child <Sector> ("Sector 1"));
		}
		
		if (input_engine -> get_key ("KP_Down", true))
		{
			switch_to (Set <Sector> :: get_child <Sector> ("Sector 2"));
		}

		active_sector -> get_camera () . setPosition (Player :: getSingleton () . node -> getPosition () + Ogre :: Vector3 (0, 18, 0));
		active_sector -> get_camera () . setOrientation (Player :: getSingleton () . node -> getOrientation ());

		time = timer -> getMilliseconds ();
	}
}

void Tslrpg ::
	switch_to (Sector * new_active_sector)
{
	if (new_active_sector != active_sector)
	{
		//	Update player position:
		active_sector -> removeRigidBody (Player :: getSingletonPtr ());
		active_sector -> move_to (Player :: getSingleton (), * new_active_sector);
		Player :: getSingleton () . node = & new_active_sector -> copy_node (* Player :: getSingleton () . node);
		active_sector = new_active_sector;
		active_sector -> addRigidBody (Player :: getSingletonPtr ());

		//	Update camera & scene manager:
		root -> getRenderSystem () -> _getViewport () -> setCamera (& active_sector -> get_camera ());
		gui_engine -> set_scene_manager (active_sector -> get_scene_manager ());
	}
}
