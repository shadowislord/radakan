#include "engines/log.hpp"
#include "engines/render_engine.hpp"
#include "engines/settings.hpp"
#include "items/character.hpp"
#include "strategies/behaviors/player.hpp"

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
	Object ("render engine", "singleton"),
	root
	(
		new Ogre :: Root
		(
			Settings :: get () -> radakan_path + "/data/plugins.cfg",
			Settings :: get () -> radakan_path + "/data/ogre.cfg",
			Settings :: get () -> radakan_path + "/log/ogre.txt"
		)
	),
	turn (0),
	timer (new Ogre :: Timer ())
{
	Engines :: Log :: trace (me, Render_Engine :: get_class_name ());

	if (root -> restoreConfig ())
	{
		Log :: log (me) << "Ogre's configuration is successfully loaded." << endl;
	}
	else if (root -> showConfigDialog ())
	{
		Log :: log (me) << "Ogre's configuration is saved." << endl;
	}
	else
	{
		Log :: error (me) << "An Ogre configuration problem occurred." << endl;
		abort ();
	}

	Engines :: Log :: trace (me, Render_Engine :: get_class_name (), "", "A");
	window . reset (root -> initialise (true, "Radakan"));
	Engines :: Log :: trace (me, Render_Engine :: get_class_name (), "", "B");
	scene_manager . reset (root -> createSceneManager (Ogre :: ST_GENERIC));
	Engines :: Log :: trace (me, Render_Engine :: get_class_name (), "", "C");
	camera . reset (scene_manager -> createCamera ("camera"));
	Engines :: Log :: trace (me, Render_Engine :: get_class_name (), "", "D");

	camera -> setNearClipDistance (0.001);
	camera -> setFarClipDistance (150);

	Engines :: Log :: trace (me, Render_Engine :: get_class_name (), "", "E");
	root -> getRenderSystem () -> _setViewport (window -> addViewport (camera . get ()));

	Engines :: Log :: trace (me, Render_Engine :: get_class_name (), "", "(end)");
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

	turn ++;

	camera -> setPosition
		(Strategies :: Behaviors :: Player :: get () -> get_camera_position ());
	camera -> setOrientation
		(Strategies :: Behaviors :: Player :: get () -> get_camera_orientation ());

	bool check = root -> renderOneFrame ();
	assert (check);

	Log :: log (me) << "FPS: " << get_FPS () << endl;
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

void Render_Engine ::
	set_skydome (string skydome_name)
{
	assert (Object :: is_initialized ());

	scene_manager -> setSkyDome (true, skydome_name, 5, 8, 100);
}


float Render_Engine ::
	get_FPS () const
{
	assert (is_initialized ());

	return turn / (0.001 * timer -> getMilliseconds ());
}
