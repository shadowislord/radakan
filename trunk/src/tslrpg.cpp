#include "tslrpg.hpp"
#include <algorithm>
//#include </home/t/opt/ogre/RenderSystems/GL/include/OgreGLGpuProgramManager.h>


using namespace std;
using namespace sl;

extern string path;

Tslrpg ::
	Tslrpg (string path) :
	Object ("Tslrpg")
{
	root = new Ogre :: Root();

	if (! root -> showConfigDialog ())
	{
		abort ();
	}

	// I just don't like the resource cfg file :-/ --Tariqwalji

	try	// Catch any errors
	{
//		new Ogre :: GLGpuProgramManager ();
	
		// Add textures directory
		Ogre :: ResourceGroupManager :: getSingleton () . addResourceLocation
					(path + "/data/texture", "FileSystem", "textures", true);

		// Add 3D models directory
		Ogre :: ResourceGroupManager :: getSingleton () . addResourceLocation
					(path + "/data/model", "FileSystem", "models", true);

		// Add materials directory
		Ogre :: ResourceGroupManager :: getSingleton () . addResourceLocation
					(path + "/data/material", "FileSystem", "materials", true);
					
		// Add gui font directory
		Ogre :: ResourceGroupManager :: getSingleton () . addResourceLocation
					(path + "/data/gui/font", "FileSystem", "gui", true);
					
		// Add gui imageset directory
		Ogre :: ResourceGroupManager :: getSingleton () . addResourceLocation
					(path + "/data/gui/imageset", "FileSystem", "gui", true);

		// Add gui looknfeel directory
		Ogre :: ResourceGroupManager :: getSingleton () . addResourceLocation
					(path + "/data/gui/looknfeel", "FileSystem", "gui", true);

		// Add gui scheme directory
		Ogre :: ResourceGroupManager :: getSingleton () . addResourceLocation
					(path + "/data/gui/scheme", "FileSystem", "gui", true);

		debug () << "A" << endl;
		// Initialise our resources
		Ogre :: ResourceGroupManager :: getSingleton () . initialiseAllResourceGroups ();
		debug () << "B" << endl;
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
	
	debug () << "C" << endl;
	window = root -> initialise (true, "Scattered Lands");
	debug () << "D" << endl;

	Ogre :: SceneManager * scene_manager = root -> createSceneManager
														(Ogre :: ST_GENERIC);
	active_sector = new Sector ("Sector 1", scene_manager, window);
	sectors . insert (active_sector);

	gui_engine = new GUI_Engine (window, scene_manager);
	
	//	Set default mipmap level (NB some APIs ignore this)
	Ogre :: TextureManager :: getSingleton() . setDefaultNumMipmaps (5);

	Ogre :: ResourceGroupManager :: getSingleton () . initialiseAllResourceGroups ();

	//	This is the new input mechanism that is taking advantage of the
	//	new engine handler.
	input_engine = new Input_Engine (window);	

	player = active_sector -> get_player ();

	timer = Ogre :: PlatformManager :: getSingleton () . createTimer ();
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
		timer -> reset ();
		
		active_sector -> update ();
	
		input_engine -> capture ();

		Ogre :: PlatformManager :: getSingletonPtr () -> messagePump (window);

		//	Of course, the program should not quit when you die, but it should do *something*. To make sure the program does not crash later, it currently does shut down when you die.
		if (! root -> renderOneFrame ()
				|| ! gui_engine -> render ()
				|| player -> is_dead ()
				|| input_engine -> get_key ("Escape", false)
				|| window -> isClosed())
		{
			break;
		}

		//	Handle movement
		//	Normal WASD keys don't work on all keyboard layouts, so we'll use ESDF for now.
		int time = timer -> getMilliseconds ();
		if (input_engine -> get_key ("e", false))
		{
			player -> node -> translate (player -> node -> getOrientation () * Ogre :: Vector3 (0, 0, - 0.2 * time));
		}
		if (input_engine -> get_key ("d", false))
		{
			player -> node -> translate (player -> node -> getOrientation () * Ogre :: Vector3 (0, 0, 0.1 * time));
		}
		if (input_engine -> get_key ("s", false))
		{
			player -> node -> yaw (Ogre :: Radian (0.005 * time));
		}
		if (input_engine -> get_key ("f", false))
		{
			player -> node -> yaw (Ogre :: Radian (- 0.005 * time));
		}
		active_sector -> get_camera () -> setPosition (player -> node -> getPosition () + Ogre :: Vector3 (0, 23, 0));
		active_sector -> get_camera () -> setOrientation (player -> node -> getOrientation ());

		//	hit
		if (input_engine -> get_key ("h", true))
		{
			NPC * npc = active_sector -> get_child <NPC> ();
			assert (npc != NULL);
			assert (npc -> is_initialized ());
			if (! npc -> is_dead ())
			{
				debug () << battle_engine . hit (player, npc) << endl;
			}
		}
		
		//	move the weapon
		if (input_engine -> get_key ("m", true))
		{
			NPC * npc = active_sector -> get_child <NPC> ();
			assert (npc != NULL);
			assert (npc -> is_initialized ());
			if (player -> has_weapon ())
			{
				player -> move_to (player -> get_weapon (), npc);
				assert (npc -> has_weapon ());
			}
			else
			{
				npc -> Character :: move_to (npc -> get_weapon (), player);
			}
		}

		if (input_engine -> get_mouse_button (left_mouse_button, true))
		{
			debug () << left_mouse_button << endl;
		}
		
		if (input_engine -> get_mouse_button (middle_mouse_button, false))
		{
			float raw_x_offset = input_engine -> get_mouse_width (true);

			if (raw_x_offset != 0)
			{
				//	apply treshold
				float x_offset = raw_x_offset / abs (raw_x_offset) * max (abs (raw_x_offset) - 3, 0.f);
			
				debug () << middle_mouse_button << " " << - 0.15 * raw_x_offset << " " << - 0.15 * x_offset << endl;

				player -> node -> yaw (Ogre :: Degree (- 0.15 * x_offset));
			}
		}
	}
}
