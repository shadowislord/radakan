#include "item.hpp"

using namespace std;
using namespace tsl;

//  constructor
Item ::
 Item
	(
		string new_name,
		string new_mesh_name,
		float new_volume,
		float new_weight,
		bool new_mobile,
		bool new_solid,
		bool new_visible
	) :
	Object (new_name),
	mesh_name (new_mesh_name),
	volume (new_volume),
	weight (new_weight),
	mobile (new_mobile),
	solid (new_solid),
	visible (new_visible),
	representation (NULL)
{
	trace () << get_class_name () << " (" << * this << ", " << mesh_name
		<< ", " << volume << ", " << weight << ", " << bool_to_string (mobile)
		<< ", " << bool_to_string (solid) << ", " << bool_to_string (visible)
		<< ")" << endl;
	assert (Object :: is_initialized ());

	assert (Item :: is_initialized ());
}

//  destructor
Item ::
	~Item ()
{
	trace () << "~" << get_class_name () << " ()" << endl;
	assert (Item :: is_initialized ());
	
	assert (Object :: is_initialized ());
}

//	virtual
bool Item ::
	is_initialized ()
	const
{
	return warn <Item> (Object :: is_initialized () && (0 <= volume) && (0 <= weight));
}

//	static
string Item ::
	get_class_name ()
{
	return "Item";
}

//	virtual
float Item ::
	get_total_weight ()
	const
{
	assert (Item :: is_initialized ());
	
	return weight;
}

void Item ::
	add_representation (Ogre :: SceneNode & node)
{
	assert (Item :: is_initialized ());
	assert (visible);
	assert (! has_representation ());

	representation = new Representation (* this, node);
}

bool Item ::
	has_representation () const
{
	assert (Item :: is_initialized ());
	
	return (representation != NULL);
}

void Item ::
	remove_representation ()
{
	assert (Item :: is_initialized ());
	assert (has_representation ());
	
	representation = NULL;
}

Representation & Item ::
	get_representation () const
{
	assert (Item :: is_initialized ());
	assert (has_representation ());
	
	return * representation;
}

//	static
Item & Item ::
	create
	(
		string new_name,
		string new_mesh_name,
		float new_volume,
		float new_weight,
		bool new_mobile,
		bool new_solid,
		bool new_visible
	)
{
	return *
		(
			new Item
			(
				new_name,
				new_mesh_name,
				new_volume,
				new_weight,
				new_mobile,
				new_solid,
				new_visible
			)
		);
}
