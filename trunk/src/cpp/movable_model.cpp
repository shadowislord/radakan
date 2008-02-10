#include "engines/log.hpp"
#include "items/item.hpp"
#include "movable_model.hpp"
#include "world.hpp"
#include "engines/settings.hpp"

#include <OgreSceneNode.h>

#if RADAKAN_PHYSICS_MODE == RADAKAN_OGREODE_MODE
	#include <OgreOdeGeometry.h>
	#include <OgreOdeBody.h>
	#include <OgreOdeMass.h>
#elif RADAKAN_PHYSICS_MODE == RADAKAN_BULLET_MODE
	#include <btBulletDynamicsCommon.h>
#else
#endif

using namespace std;
using namespace Radakan;

//	static
string Movable_Model ::
	get_class_name ()
{
	return "Movable_Model";
}

//  constructor
Movable_Model ::
	Movable_Model (Reference <Items :: Item> new_item, Mathematics :: Vector_3D position) :
	Object (new_item . get_name () + "'s movable model"),
	Model (new_item, position)
{
	Engines :: Log :: trace (me, Movable_Model :: get_class_name (), "",
		new_item . get_name (), position . to_string ());

#if RADAKAN_PHYSICS_MODE == RADAKAN_OGREODE_MODE
	body . reset (new OgreOde :: Body (World :: get () -> ogre_ode_world . get (), name));
	body -> setMass (OgreOde :: BoxMass (item -> mass, item -> size));
	geometry -> setBody (body . get ());
	node -> attachObject (body . get ());
#elif RADAKAN_PHYSICS_MODE == RADAKAN_BULLET_MODE
#else
#endif

	assert (Model :: is_initialized ());
}

//  destructor
Movable_Model ::
	~Movable_Model ()
{
	Engines :: Log :: trace (me, Movable_Model :: get_class_name (), "~");
	assert (Model :: is_initialized ());

	prepare_for_destruction ();

#if RADAKAN_PHYSICS_MODE == RADAKAN_OGREODE_MODE
	node -> detachObject (body . get ());
#elif RADAKAN_PHYSICS_MODE == RADAKAN_BULLET_MODE
#else
#endif
}

//	virtual
bool Movable_Model ::
	is_initialized ()
	const
{
	assert (Model :: is_initialized ());

	return true;
}

void Movable_Model ::
	move (float relative_destination_movement_speed)
{
	assert (Model :: is_initialized ());
	assert (Ogre :: Math :: Abs (relative_destination_movement_speed) <= 1);

	if (relative_destination_movement_speed != 0)
	{
		Engines :: Log :: log (me) << "relative_destination_movement_speed: "
			<< relative_destination_movement_speed << endl;
	}

	//	This line works for both Bullet and OgreOde:
	Mathematics :: Vector_3D current_speed (body -> getLinearVelocity ());

	//	I don't know why 'get_front_direction ()' needs a '-',
	//	but it works for me. --Tinus
	Mathematics :: Vector_3D force =  Engines :: Settings :: get () -> movement_reaction
		* (
			relative_destination_movement_speed
				* Engines :: Settings :: get () -> maximal_movement_speed
				* (- get_front_direction ())
			- current_speed
		);

#if RADAKAN_PHYSICS_MODE == RADAKAN_OGREODE_MODE
	body -> setForce (force);
#elif RADAKAN_PHYSICS_MODE == RADAKAN_BULLET_MODE
	body -> applyCentralForce (force . to_bullet ());
#else
#endif
}

void Movable_Model ::
	turn (float relative_destination_turn_speed, Mathematics :: Vector_3D axis)
{
	Engines :: Log :: trace (me, Movable_Model :: get_class_name (), "turn",
		to_string (relative_destination_turn_speed), axis . to_string ());
	assert (Model :: is_initialized ());
	assert (Ogre :: Math :: Abs (relative_destination_turn_speed) <= 1);

	if (axis == Mathematics :: Vector_3D :: zero_vector)
	{
		axis = get_top_direction ();
	}

	if (relative_destination_turn_speed != 0)
	{
		Engines :: Log :: log (me) << "relative_destination_turn_speed: "
			<< relative_destination_turn_speed << endl;
	}

	//	This line works for both Bullet and OgreOde:
	Mathematics :: Vector_3D current_angular_momentum (body -> getAngularVelocity ());
	
	Mathematics :: Vector_3D torque = Engines :: Settings :: get () -> turn_reaction
		* (
			relative_destination_turn_speed
				* Engines :: Settings :: get () -> maximal_turn_speed
				* axis
			- current_angular_momentum
		);
#if RADAKAN_PHYSICS_MODE == RADAKAN_OGREODE_MODE
	body -> setTorque (torque);
#elif RADAKAN_PHYSICS_MODE == RADAKAN_BULLET_MODE
	body -> applyTorque (torque . to_bullet ());
#else
#endif
}

void Movable_Model ::
	reset ()
{
	Engines :: Log :: trace (me, Movable_Model :: get_class_name (), "reset");
	assert (Model :: is_initialized ());

#if RADAKAN_PHYSICS_MODE == RADAKAN_OGREODE_MODE
	body -> setOrientation (Mathematics :: Quaternion :: identity);
	body -> setPosition (body -> getPosition ()
		+ (2 - body -> getPosition () . y) * Mathematics :: Vector_3D :: y_axis);
	body -> setLinearVelocity (Mathematics :: Vector_3D :: zero_vector);
	body -> setForce (Mathematics :: Vector_3D :: zero_vector);
#elif RADAKAN_PHYSICS_MODE == RADAKAN_BULLET_MODE
	body -> clearForces ();

	btTransform transformation
	(
		Mathematics :: Quaternion :: identity . to_bullet (),
		(Mathematics :: Vector_3D
		(
			get_position () + (2 - get_position () . y) * Mathematics :: Vector_3D :: y_axis
		)) . to_bullet ()
	);
	//	motion_state -> getWorldTransform (transformation);
	body -> getMotionState () -> getWorldTransform (transformation);	//	Does this do the same?
#else
#endif
}
