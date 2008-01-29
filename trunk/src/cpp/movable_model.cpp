#include <OgreSceneNode.h>

#include <OgreOdeGeometry.h>
#include <OgreOdeBody.h>
#include <OgreOdeMass.h>

#include "engines/log.hpp"
#include "items/item.hpp"
#include "movable_model.hpp"
#include "world.hpp"
#include "engines/settings.hpp"


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
	Movable_Model (Reference <Items :: Item> new_item, Ogre :: Vector3 position) :
	Object (new_item . get_name () + "'s movable model"),
	Model (new_item, position)
{
	Engines :: Log :: trace (me, Movable_Model :: get_class_name (), "", new_item . get_name (), to_string (position));

	body . reset (new OgreOde :: Body (World :: get () -> ogre_ode_world . get (), name));
	body -> setMass (OgreOde :: BoxMass (item -> mass, item -> size));
	geometry -> setBody (body . get ());
	node -> attachObject (body . get ());

	assert (Model :: is_initialized ());
}

//  destructor
Movable_Model ::
	~Movable_Model ()
{
	Engines :: Log :: trace (me, Movable_Model :: get_class_name (), "~");
	assert (Model :: is_initialized ());

	prepare_for_destruction ();

	node -> detachObject (body . get ());
}

//	virtual
bool Movable_Model ::
	is_initialized ()
	const
{
	assert (Model :: is_initialized ());

	//	TODO re-enable assert ((body . getPosition () - node . getPosition ()) . length () < 0.01);
	//	TODO re-enable assert ((body . getPosition () - geometry . getPosition ()) . length () < 0.01);

	return true;
}

void Movable_Model ::
	move (float relative_destination_movement_speed)
{
	assert (Model :: is_initialized ());
	assert (Ogre :: Math :: Abs (relative_destination_movement_speed) <= 1);

	if (relative_destination_movement_speed < 0)
	{
		Engines :: Log :: log (me) << "relative_destination_movement_speed: " << relative_destination_movement_speed << endl;
	}

	//	I don't know why 'get_front_direction ()' needs a '-',
	//	but it works for me. --Tinus
	body -> setForce
	(
		Engines :: Settings :: get () -> movement_reaction
		*
		(
			relative_destination_movement_speed
				* Engines :: Settings :: get () -> maximal_movement_speed
				* (- get_front_direction ())
			-
			body -> getLinearVelocity ()
		)
	);
}

void Movable_Model ::
	turn (float relative_destination_turn_speed, Ogre :: Vector3 ax)
{
	//	Engines :: Log :: trace (me, Movable_Model :: get_class_name (), "turn", to_string (relative_destination_turn_speed), to_string (ax));
	assert (Model :: is_initialized ());
	assert (Ogre :: Math :: Abs (relative_destination_turn_speed) <= 1);

	if (ax == zero_vector)
	{
		ax = get_top_direction ();
	}

	body -> setTorque
	(
		Engines :: Settings :: get () -> turn_reaction
		*
		(
			relative_destination_turn_speed
				* Engines :: Settings :: get () -> maximal_turn_speed
				* ax
			-
			body -> getAngularVelocity ()
		)
	);
}

void Movable_Model ::
	reset ()
{
	Engines :: Log :: trace (me, Movable_Model :: get_class_name (), "reset");
	assert (Model :: is_initialized ());

	body -> setOrientation (Ogre :: Quaternion (1, 0, 0, 0));
	body -> setPosition (body -> getPosition () + y_axis * (2 - body -> getPosition () . y));
	body -> setLinearVelocity (zero_vector);
	body -> setForce (zero_vector);
}
