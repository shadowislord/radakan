#include "log.hpp"
#include "movable_model.hpp"

using namespace std;
using namespace TSL;

//	static
const string Movable_Model ::
	get_class_name ()
{
	return "Movable_Model";
}

//  constructor
Movable_Model ::
	Movable_Model (Item & new_item, Ogre :: Vector3 position, float scale, OgreOde :: Geometry & new_geometry, OgreOde :: Body & new_body) :
	Object (new_item + "'s movable model"),
	Model (new_item, position, scale, new_geometry),
	body (new_body)
{
	Log :: trace <Movable_Model> (me, "", new_item, to_string (position), to_string (scale), "~new_geometry~", "~new_body~");
	assert (Model :: is_initialized ());

	node . attachObject (& body);

	assert (Model :: is_initialized ());
}

//  destructor
Movable_Model ::
	~Movable_Model ()
{
	Log :: trace <Movable_Model> (me, "~");
	assert (Model :: is_initialized ());
	
	node . detachObject (& body);

	assert (Model :: is_initialized ());
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
	move (float top_speed, float turn_length)
{
	assert (Model :: is_initialized ());
	assert (Ogre :: Math :: Abs (top_speed) <= 1);
	assert (0 <= turn_length);

//	Log :: log (me) << "Position: " << to_string (body . getPosition ()) << endl;
//	Log :: log (me) << "Speed: " << to_string (body . getPointWorldVelocity (body . getPosition ())) << endl;
	
//	Log :: log (me) << "current force: " << to_string (body . getForce ()) << endl;
	
	body . setForce (200 * (top_speed * get_front_direction () - body . getLinearVelocity ()));
	
//	Log :: log (me) << "new force: " << to_string (body . getForce ()) << endl;
}

void Movable_Model ::
	turn (float top_radian_angle_speed, float turn_length, Ogre :: Vector3 ax)
{
	assert (Model :: is_initialized ());
	assert (Ogre :: Math :: Abs (top_radian_angle_speed) <= 1);
	assert (0 <= turn_length);

	if (ax == zero_vector)
	{
		ax = get_top_direction ();
	}

	body . setTorque (10 * (top_radian_angle_speed * ax - body . getAngularVelocity ()));
}

void Movable_Model ::
	reset ()
{
	Log :: trace <Movable_Model> (me, "reset");
	assert (Model :: is_initialized ());

	body . setOrientation (Ogre :: Quaternion (1, 0, 0, 0));
	body . setPosition (body . getPosition () + y_axis * (2 - body . getPosition () . y));
	body . setLinearVelocity (zero_vector);
	body . setForce (zero_vector);
}
