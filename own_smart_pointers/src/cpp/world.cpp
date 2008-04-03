#include "engines/log.hpp"
#include "engines/render_engine.hpp"
#include "items/item.hpp"
#include "map.hpp"
//	#include "messages/message.hpp"
#include "tile.hpp"
#include "world.hpp"

#if RADAKAN_PHYSICS_MODE == RADAKAN_BULLET_MODE
	#include "bullet_world.hpp"
#elif RADAKAN_PHYSICS_MODE == RADAKAN_OGREODE_MODE
	#include "ogre_ode_world.hpp"
#elif RADAKAN_PHYSICS_MODE == RADAKAN_PAL_MODE
	#include "pal_world.hpp"
#endif

#include <OgreSceneManager.h>

using namespace std;
using namespace Radakan;

//	static
const int World ::
	min_x (- 1);

//	static
const int World ::
	max_x (2);

//	static
const int World ::
	min_z (- 1);

//	static
const int World ::
	max_z (1);

//	static
const float  World ::
	time_step (0.01);

//	static
string World ::
	get_class_name ()
{
	return "World";
}

//	static
void World ::
	create ()
{
	#if RADAKAN_PHYSICS_MODE == RADAKAN_BULLET_MODE
		new Bullet_World ();
	#elif RADAKAN_PHYSICS_MODE == RADAKAN_OGREODE_MODE
		new Ogre_Ode_World ();
	#elif RADAKAN_PHYSICS_MODE == RADAKAN_PAL_MODE
		new Pal_World ();
	#endif
}

World ::
	World () :
	Object ("world", "singleton"),
	root_node (Engines :: Render_Engine :: get () -> get_scene_manager ()
		-> getRootSceneNode ()),
	tiles (new Map <Mathematics :: Vector_3D, Tile> ("tiles"))
{

}

World ::
	~World ()
{
	Engines :: Log :: trace (me, World :: get_class_name (), "~");
	assert (World :: is_initialized ());

	prepare_for_destruction ();
}

//	virtual
bool World ::
	is_initialized ()
	const
{
	return Object :: is_initialized ();
}
