#include "character.hpp"

using namespace std;
using namespace sl;

//  constructor
Character ::
	Character (Ogre :: SceneNode & new_node) :
	Object (get_name (* node)),
	Container
		(true,
		true,
		true,
		80,
		65,
		btVector3 (100, 0, 200),
		new_node)
{
	trace () << "Character (" << get_name (* node) << ")" << endl;
	assert (Entity :: is_initialized ());

	dead = false;

	backpack = NULL;
	weapon = NULL;

	assert (is_initialized ());
}

//  destructor
Character ::
	~Character ()
{
	trace () << "~Character ()" << endl;
	assert (Character :: is_initialized ());
}

//	virtual
bool Character ::
	is_initialized ()
	const
{
	return warn <Character> (Entity :: is_initialized () && (backpack == NULL || backpack -> is_initialized ()) && (weapon == NULL || weapon -> is_initialized ()));
}

//	static
string Character ::
	get_type_name ()
{
	return "character";
}

//	virtual
float Character ::
	get_total_weight ()
	const
{
	assert (is_initialized ());

	float weight = Entity :: get_total_weight ();

	if (backpack != NULL)
	{
		weight += backpack -> get_total_weight ();
	}
	
	return weight;
}

//	virtual
bool Character ::
	add (Entity & entity)
{
	assert (is_initialized ());
	assert (entity . is_initialized ());

	if ((weapon == NULL) && entity . is_type <Weapon> ())
	{
		Container :: add (entity);
		weapon = & entity . to_type <Weapon> ();
		return true;
	}
	else if ((backpack == NULL) && entity . is_type <Container> ())
	{
		Container :: add (entity);
		backpack = & entity . to_type <Container> ();
		return true;
	}
	return false;
}

//	virtual
bool Character ::
	move_to (Entity & entity, Set <Entity> & other_set)
{
	assert (Character :: is_initialized ());
	assert (entity . is_initialized ());
	assert (other_set . is_initialized ());
	assert (contains (entity, false));

	bool result = Container :: move_to (entity, other_set);

	if (result)
	{
		if (weapon == & entity)
		{
			weapon = NULL;
		}
		else if (backpack == & entity)
		{
			backpack = NULL;
		}
	}
	
	assert (result == other_set . contains (entity, false));

	return result;
}

void Character ::
	run (float distance)
{
	assert (Character :: is_initialized ());

	trace () << "position: (" << get_position () . getX () << ", " << get_position () . getY () << ", " << get_position () . getZ () << ")" << endl;

	Ogre :: Quaternion r = Ogre :: Quaternion
	(
		get_rotation () . getW (),
		get_rotation () . getX (),
		get_rotation () . getY (),
		get_rotation () . getZ ()
	);
	
	Ogre :: Vector3 v = Ogre :: Vector3 (0, 0, - distance);

	//	This is not some kind of multiplication, but the rotation of r applied to v.
	//	Bullet seems to lack an equivalent.
	Ogre :: Vector3 w = r * v;	
	set_position (get_position () + btVector3 (w . x, w . y, w . z));
	trace () << "new position: (" << get_position () . getX () << ", " << get_position () . getY () << ", " << get_position () . getZ () << ")" << endl;

	update_scene_node ();
}

void Character ::
	turn (float radian_angle)
{
	assert (Character :: is_initialized ());

	trace () << "rotation: (" << get_rotation () . getX () << ", " << get_rotation () . getY () << ", " << get_rotation () . getZ () << ")" << endl;

	set_rotation (get_rotation () * btQuaternion (btVector3 (0, 1, 0), radian_angle));

	trace () << "new rotation: (" << get_rotation () . getX () << ", " << get_rotation () . getY () << ", " << get_rotation () . getZ () << ")" << endl;

}

string Character ::
	die ()
{
	assert (Character :: is_initialized ());

	dead = true;

	return * this + " died!";
}

bool Character ::
	is_dead ()
	const
{
	assert (Character :: is_initialized ());

	return dead;
}

bool Character ::
	has_weapon ()
	const
{
	assert (Character :: is_initialized ());

	return (weapon != NULL);
}

Weapon * Character ::
	get_weapon ()
	const
{
	assert (Character :: is_initialized ());
	assert (has_weapon ());

	return weapon;
}
