#include "engines/log.hpp"
#include "items/item.hpp"
#include "map.hpp"
#include "tile.hpp"
#include "bullet_world.hpp"

#include <btBulletDynamicsCommon.h>

using namespace std;
using namespace Radakan;

//	static
string Bullet_World ::
	get_class_name ()
{
	return "Bullet_World";
}

Bullet_World ::
	Bullet_World () :
	Object (get_class_name (), "singleton"),
	collision_configuration (new btDefaultCollisionConfiguration),
	dispatcher (new btCollisionDispatcher (collision_configuration . get ())),
	broadphase (new btSimpleBroadphase),
	constraint_solver (new btSequentialImpulseConstraintSolver),
	bullet_world (new btDiscreteDynamicsWorld
	(
		dispatcher . get (),
		broadphase . get (),
		constraint_solver . get (),
		collision_configuration . get ()
	))
{
	bullet_world -> setGravity (btVector3 (0, - 9.81, 0));
	
	for (int x = min_x; x <= max_x; x ++)
	{
		for (int z = min_z; z <= max_z; z ++)
		{
			Mathematics :: Vector_3D coordinates (x, 0, z);

			Engines :: Log :: log (me) << "coordinates: " << coordinates << endl;

			Reference <Tile> tile (new Tile (coordinates));

			tiles -> add
				(Pair <Mathematics :: Vector_3D, Tile> :: create (coordinates, tile));
		}
	}
}

Bullet_World ::
	~Bullet_World ()
{
	Engines :: Log :: trace (me, Bullet_World :: get_class_name (), "~");
	assert (Bullet_World :: is_initialized ());

	prepare_for_destruction ();
}

//	virtual
bool Bullet_World ::
	is_initialized ()
	const
{
	return Object :: is_initialized ();
}

//	virtual
void Bullet_World ::
	update ()
{
	assert (is_initialized ());

	bullet_world -> stepSimulation (World :: time_step);

	Pointer <Tile> tile;
	for (int x = min_x; x <= max_x; x ++)
	{
		for (int z = min_z; z <= max_z; z ++)
		{
			tile = tiles -> look_up (Mathematics :: Vector_3D (x, 0, z));
			for (Pointer <Items :: Item> item = tile -> get_child ();
				item . points_to_object (); item = tile -> get_another_child ())
			{
				item -> sync_model ();
			}
		}
	}
}

void Bullet_World ::
	add_body (boost :: shared_ptr <btRigidBody> body)
{
	assert (is_initialized ());

	bullet_world -> addRigidBody (body . get ());
}
