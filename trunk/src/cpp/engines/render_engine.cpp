#include "engines/log.hpp"
#include "engines/gui_engine.hpp"
#include "engines/render_engine.hpp"
#include "engines/settings.hpp"
#include "items/character.hpp"
#include "movable_model.hpp"

#include <OgreRenderSystem.h>
#include <OgreRenderWindow.h>
#include <OgreRoot.h>

using namespace std;
using namespace Radakan;
using namespace Radakan :: Engines;

//	static
string Render_Engine ::
	get_class_name ()
{
	return "Render_Engine";
}

Render_Engine ::
	Render_Engine () :
	Object ("render engine", true),	//	Here 'true' means 'prevent automatic destruction'.
	root
	(
		boost :: shared_ptr <Ogre :: Root>
		(
			new Ogre :: Root
			(
				Settings :: get () -> radakan_path + "/data/plugins.cfg",
				Settings :: get () -> radakan_path + "/data/ogre.cfg",
				Settings :: get () -> radakan_path + "/log/ogre.txt"
			)
		)
	)
{
	Engines :: Log :: trace (me, Render_Engine :: get_class_name ());
	
	if (! root -> showConfigDialog ())
	{
		Log :: error (me) << "An Ogre configuration dialog problem occurred." << endl;
		abort ();
	}

	window . reset (root -> initialise (true, "Radakan"));
	scene_manager . reset (root -> createSceneManager (Ogre :: ST_GENERIC));
	camera . reset (scene_manager -> createCamera ("camera"));

	camera -> setNearClipDistance (0.001);
	camera -> setFarClipDistance (150);

	root -> getRenderSystem () -> _setViewport (window -> addViewport (camera . get ()));
}

Render_Engine ::
	~Render_Engine ()
{
	Engines :: Log :: trace (me, Render_Engine :: get_class_name (), "~");
	assert (is_initialized ());

	prepare_for_destruction ();
}

//	virtual
bool Render_Engine ::
	is_initialized ()
	const
{
	assert (Object :: is_initialized ());

	return true;
}

void Render_Engine ::
	render ()
{
	assert (Object :: is_initialized ());

	camera -> setPosition
	(
		Items :: Character :: get_player_character ()
			-> get_movable_model () -> node -> getPosition ()
			+ Items :: Character :: get_player_character ()
				-> get_movable_model () -> get_top_direction ()
			* Engines :: Settings :: get () -> get_camera_distance ()
	);
	camera -> setOrientation
	(
		make_quaternion
		(
			Engines :: Settings :: get () -> get_vertical_camera_angle (),
			Items :: Character :: get_player_character ()
				-> get_movable_model () -> get_side_direction ()
		)
		* Items :: Character :: get_player_character ()
			-> get_movable_model () -> node -> getOrientation ()
	);

	bool check = root -> renderOneFrame ();
	assert (check);
	
	GUI_Engine :: get () -> render ();
}

boost :: shared_ptr <Ogre :: RenderWindow> Render_Engine ::
	get_window ()
{
	assert (Object :: is_initialized ());

	return window;
}

boost :: shared_ptr <Ogre :: SceneManager> Render_Engine ::
	get_scene_manager ()
{
	assert (Object :: is_initialized ());

	return scene_manager;
}
