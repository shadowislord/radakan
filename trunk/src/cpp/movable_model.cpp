#include "log.hpp"
#include "movable_model.hpp"

using namespace std;
using namespace Radakan;

//	static
const string Movable_Model ::
	get_class_name ()
{
	return "Movable_Model";
}

//  constructor
Movable_Model ::
	Movable_Model (Items :: Item & new_item, Ogre :: Vector3 position, float scale, OgreOde :: Geometry & new_geometry, OgreOde :: Body & new_body) :
	Object (new_item . name + "'s movable model"),
	Model (new_item, position, scale, new_geometry),
	body (new_body)
{
	Engines :: Log :: trace (me, Movable_Model :: get_class_name (), "", new_item . name, to_string (position), to_string (scale), "~new_geometry~", "~new_body~");

	node . attachObject (& body);

	assert (Model :: is_initialized ());
}

//  destructor
Movable_Model ::
	~Movable_Model ()
{
	Engines :: Log :: trace (me, Movable_Model :: get_class_name (), "~");
	assert (Model :: is_initialized ());

	prepare_for_destruction ();

	node . detachObject (& body);
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
	move (float top_speed)
{
	assert (Model :: is_initialized ());
	assert (Ogre :: Math :: Abs (top_speed) <= 1);

	body . setForce (1000 * (top_speed * get_front_direction () - body . getLinearVelocity ()));
}

void Movable_Model ::
	turn (float top_radian_angle_speed, Ogre :: Vector3 ax)
{
	//	Engines :: Log :: trace (me, Movable_Model :: get_class_name (), "turn", to_string (top_radian_angle_speed), to_string (ax));
	assert (Model :: is_initialized ());
	assert (Ogre :: Math :: Abs (top_radian_angle_speed) <= 1);

	if (ax == zero_vector)
	{
		ax = get_top_direction ();
	}

	body . setTorque (30 * (top_radian_angle_speed * ax - body . getAngularVelocity ()));
}

void Movable_Model ::
	reset ()
{
	Engines :: Log :: trace (me, Movable_Model :: get_class_name (), "reset");
	assert (Model :: is_initialized ());

	body . setOrientation (Ogre :: Quaternion (1, 0, 0, 0));
	body . setPosition (body . getPosition () + y_axis * (2 - body . getPosition () . y));
	body . setLinearVelocity (zero_vector);
	body . setForce (zero_vector);
}
