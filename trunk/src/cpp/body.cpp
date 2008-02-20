#include "body.hpp"
#include "engines/log.hpp"
#include "engines/settings.hpp"

#if RADAKAN_PHYSICS_MODE == RADAKAN_BULLET_MODE
	#include "bullet_body.hpp"
#elif RADAKAN_PHYSICS_MODE == RADAKAN_OGREODE_MODE
	#include "ogre_body.hpp"
#elif RADAKAN_PHYSICS_MODE == RADAKAN_PAL_MODE
	#include "pal_body.hpp"
#endif

using namespace std;
using namespace Radakan;

//	static
string Body ::
	get_class_name ()
{
	return "Body";
}

//	static
Reference <Body> Body ::
	create
	(
		float mass,
		Mathematics :: Vector_3D size,
		Mathematics :: Vector_3D position,
		Mathematics :: Quaternion orientation
	)
{
	#if RADAKAN_PHYSICS_MODE == RADAKAN_BULLET_MODE
		return Reference <Body> (new Bullet_Body (mass, size, position, orientation));
	#elif RADAKAN_PHYSICS_MODE == RADAKAN_OGREODE_MODE
		return Reference <Body> (new Ogre_Ode_Body (mass, size, position, orientation));
	#elif RADAKAN_PHYSICS_MODE == RADAKAN_PAL_MODE
		return Reference <Body> (new Pal_Body (mass, size, position, orientation));
	#endif
}

Body ::
	Body () :
	Object ("Doesn't matter")
{
	//	Nothing to do.
}

//	virtual
Body ::
	~Body ()
{
	assert (is_initialized ());

	//	Nothing to do.
}

//	virtual
bool Body ::
	is_initialized ()
	const
{
	assert (Object :: is_initialized ());

	return true;
}

Mathematics :: Vector_3D Body ::
	get_front_direction () const
{
	assert (Body :: is_initialized ());

	return get_orientation () * Mathematics :: Vector_3D :: z_axis;
}

Mathematics :: Vector_3D Body ::
	get_side_direction () const
{
	assert (Body :: is_initialized ());

	return get_orientation () * Mathematics :: Vector_3D :: x_axis;
}

Mathematics :: Vector_3D Body ::
	get_top_direction () const
{
	assert (Body :: is_initialized ());

	return get_orientation () * Mathematics :: Vector_3D :: y_axis;
}

void Body ::
	walk (float relative_destination_movement_speed)
{
	if (relative_destination_movement_speed != 0)
	{
		Engines :: Log :: log (me) << "relative_destination_movement_speed: "
			<< relative_destination_movement_speed << endl;
	}
	assert (Body :: is_initialized ());
	assert (Ogre :: Math :: Abs (relative_destination_movement_speed) <= 1);

	//	I don't know why this needs a '-', but it works for me. --Tinus
	Mathematics :: Vector_3D ideal_speed
	(
		- relative_destination_movement_speed
			* Engines :: Settings :: get () -> maximal_movement_speed
			* get_front_direction ()
	);

	Mathematics :: Vector_3D force = Engines :: Settings :: get () -> movement_reaction
		* (ideal_speed - get_linear_velocity ());

	force = force - (force . dotProduct (Mathematics :: Vector_3D :: y_axis))
		* Mathematics :: Vector_3D :: y_axis;
		
	if (relative_destination_movement_speed != 0)
	{
		Engines :: Log :: log (me) << "force: " << force << endl;
	}

	apply_force (force);
}

void Body ::
	turn (float relative_destination_turn_speed, Mathematics :: Vector_3D axis)
{
	if (relative_destination_turn_speed != 0)
	{
		Engines :: Log :: trace (me, Body :: get_class_name (), "turn",
			to_string (relative_destination_turn_speed), axis . to_string ());
	}
	assert (Body :: is_initialized ());
	assert (Ogre :: Math :: Abs (relative_destination_turn_speed) <= 1);

	if (axis == Mathematics :: Vector_3D :: zero_vector)
	{
		axis = get_top_direction ();
	}

	//	I don't know why this needs a '-', but it works for me. --Tinus
	Mathematics :: Vector_3D ideal_speed
	(
		relative_destination_turn_speed
			* Engines :: Settings :: get () -> maximal_turn_speed
			* axis
	);

	Mathematics :: Vector_3D torque = Engines :: Settings :: get () -> turn_reaction
		* (ideal_speed - get_angular_velocity ());

	if (relative_destination_turn_speed != 0)
	{
		Engines :: Log :: log (me) << "torque: " << torque << endl;
	}

	apply_torque (torque);
}

void Body ::
	jump ()
{
	Engines :: Log :: trace (me, Body :: get_class_name (), "jump");
	
	assert (Body :: is_initialized ());

	if (true)
	{
		apply_force (Mathematics :: Vector_3D (0, 100, 0) . to_bullet ());
	}
}
