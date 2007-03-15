#include "weapon.hpp"

using namespace std;
using namespace tsl;

//  constructor
Weapon ::
	Weapon
	(
		string new_name,
		string new_mesh_name,
		float new_volume,
		float new_mass,
		float new_sharpness,
		float new_break_chance,
		float new_speed,
		float new_attack_rate,
		float new_defense_rate,
		float new_damage
	) :
	Object (new_name),
	Item
	(
		new_mesh_name,
		new_volume,
		new_mass,
		true,
		true,
		true
	),
	break_chance (new_break_chance),
	speed (new_speed),
	attack_rate (new_attack_rate),
	defense_rate (new_defense_rate),
	damage (new_damage)
{
	assert (Item :: is_initialized ());

	broken = false;
	sharpness = new_sharpness;

	assert (is_initialized ());
}

//  destructor
Weapon ::
	~Weapon ()
{
	log (debugging) << "~" << get_class_name () << " ()" << endl;
	assert (is_initialized ());
}

//	virtual
bool Weapon ::
	is_initialized ()
	const
{
	return warn <Weapon> (Item :: is_initialized () && (0 < break_chance)
	&& (break_chance < 100)  && (0 < sharpness) && (sharpness < 100)
	&& (0 < speed) && (0 < attack_rate) && (0 < defense_rate) && (0 < damage));
}

//	static
string Weapon ::
	get_class_name ()
{
	return "Weapon";
}

//	virtual
OgreOde :: Geometry & Weapon ::
	create_geometry ()
{
	assert (is_initialized ());
	assert (! has_body ());
	
	OgreOde :: Geometry * geometry;

	OgreOde :: Body * body = new OgreOde :: Body (& Environment :: get (), string :: data ());

	geometry = new OgreOde :: SphereGeometry (Ogre :: Math :: RangeRandom (0.5, 1.5), & Environment :: get (), Environment :: get () . getDefaultSpace ());
	log (debugging) << "A default sphere mesh was created for " << string :: data () << "." << endl;

	geometry -> setBody (body);

	body -> setMass (OgreOde :: SphereMass (mass, 1 /*TODO set the right radius*/));

	return * geometry;
}

//	static
Item & Weapon ::
	create
	(
				string new_name,
				string new_mesh_name,
				float new_volume,
				float new_mass,
				float new_sharpness,
				float new_break_chance,
				float new_speed,
				float new_attack_rate,
				float new_defense_rate,
				float new_damage
	)
{
	Item * temp =
		new Weapon
		(
			new_name,
			new_mesh_name,
			new_volume,
			new_mass,
			new_sharpness,
			new_break_chance,
			new_speed,
			new_attack_rate,
			new_defense_rate,
			new_damage
		);

	return * temp;
}
