#include "movable_body.hpp"

using namespace std;
using namespace TSL;

//	static
const string Movable_Body ::
	class_name ("Movable_Body");

//  constructor
Movable_Body ::
	Movable_Body (Item & new_item, Ogre :: Vector3 position, float scale, OgreOde :: Geometry & new_geometry, OgreOde :: Body & new_body) :
	Object (new_item + "'s movable body"),
	Body (new_item, position, scale, new_geometry),
	body (new_body)
{
	log (debugging) << class_name << " (" << new_item << ", " << to_string (position) << ", " << scale << ")" << endl;
	assert (Body :: is_initialized ());

	node . attachObject (& body);

	assert (Body :: is_initialized ());
}

//  destructor
Movable_Body ::
	~Movable_Body ()
{
	log (debugging) << "~" << class_name << " ()" << endl;
	assert (Body :: is_initialized ());
	
	node . detachObject (& body);

	assert (Body :: is_initialized ());
}

//	virtual
bool Movable_Body ::
	is_initialized ()
	const
{
	assert (Body :: is_initialized ());

	//	TODO re-enable assert ((body . getPosition () - node . getPosition ()) . length () < 0.01);
	//	TODO re-enable assert ((body . getPosition () - geometry . getPosition ()) . length () < 0.01);

	return true;
}

void Movable_Body ::
	move (float top_speed, float turn_length)
{
	assert (Body :: is_initialized ());
	assert (Ogre :: Math :: Abs (top_speed) <= 1);
	assert (0 <= turn_length);

//	log (debugging) << "Position: " << to_string (body . getPosition ()) << endl;
//	log (debugging) << "Speed: " << to_string (body . getPointWorldVelocity (body . getPosition ())) << endl;
	
//	log (debugging) << "current force: " << to_string (body . getForce ()) << endl;
	
	body . setForce (200 * (top_speed * get_front_direction () - body . getLinearVelocity ()));
	
//	log (debugging) << "new force: " << to_string (body . getForce ()) << endl;
}

void Movable_Body ::
	turn (float top_radian_angle_speed, float turn_length, Ogre :: Vector3 ax)
{
	assert (Body :: is_initialized ());
	assert (Ogre :: Math :: Abs (top_radian_angle_speed) <= 1);
	assert (0 <= turn_length);

	if (ax == zero_vector)
	{
		ax = get_top_direction ();
	}

	body . setTorque (10 * (top_radian_angle_speed * ax - body . getAngularVelocity ()));
}

void Movable_Body ::
	reset ()
{
	log (debugging) << "reset ()" << endl;
	assert (Body :: is_initialized ());

	body . setOrientation (Ogre :: Quaternion (1, 0, 0, 0));
	body . setPosition (body . getPosition () + y_axis * (2 - body . getPosition () . y));
	body . setLinearVelocity (zero_vector);
	body . setForce (zero_vector);
}
