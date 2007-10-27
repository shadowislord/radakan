#include "engines/log.hpp"
#include "items/character.hpp"
#include "messages/battle_message.hpp"
#include "messages/conversation_message.hpp"
#include "movable_model.hpp"
#include "slot.hpp"

using namespace std;
using namespace Radakan;
using namespace Radakan :: Items;

//	static
const float Character ::
	default_experience (13);

//	static
string Character ::
	get_class_name ()
{
	return "Character";
}

//	static
Reference <Set <Character> > Character ::
	characters (new Set <Character> ("characters (static)"));

//  constructor
Character ::
	Character
	(
		string new_mesh_name,
		Ogre :: Vector3 new_size,
		float new_mass
	) :
	Object ("The name doesn't matter as this class is an abstact class."),
	Container_Item <Item>
	(
		"The name doesn't matter as this class is an abstact class.",
		new_mesh_name,
		new_size,
		new_mass
	),
	//	head (new Static_Item (name + "'s head", "bar.mesh", 1, 1)),
	//	head (new Container_Item <Hat> ()),
	//	body (new Container_Item <Shirt> ()),
	back
	(
		new Container_Item <Container_Item <Item> >
		(
			name + "'s back",
			"bar.mesh",
			Ogre :: Vector3 (0.5, 0.5, 0.3),
			1,
			true,
			true,
			true,
			1
		)
	),
	//	arms (new Container_Item <Bracer> ()),
	right_hand
	(
		new Container_Item <Item>
		(
			name + "'s right hand",
			"bar.mesh",
			Ogre :: Vector3 (1, 0.3, 0.3),
			1,
			true,
			true,
			true,
			1
		)
	)
	//	legs (new Container_Item <Pants> ()),
	//	feet (new Container_Item <Shoe> ())
{
	Engines :: Log :: trace (me, Character :: get_class_name (), "", new_mesh_name, to_string (new_size), to_string (new_mass));

	bool check/* = Container_Item <Item> :: add (head)*/;
	/*assert (check);*/
	//	Container_Item <Item> :: add (body);
	check = Container_Item <Item> :: add (back);
	assert (check);
	//	Container_Item <Item> :: add (arms);
	check = Container_Item <Item> :: add (right_hand);
	assert (check);
	//	Container_Item <Item> :: add (legs);
	//	Container_Item <Item> :: add (feet);

	//	Make sure no body parts are added anymore.
	seal ();

	Reference <Character> this_character (this);

	for (Reference <Character> character = characters -> get_child ();
		character . points_to_object (); character = characters -> get_another_child ())
	{
		register_observer (character);
		character -> register_observer (this_character);
	}

	check = characters -> add (this_character);
	assert (check);

	register_observer (this_character);

	experiences ["charisma"] = default_experience;

	assert (is_initialized ());
}

//  destructor
Character ::
	~Character ()
{
	Engines :: Log :: trace (me, Character :: get_class_name (), "~");
	assert (Character :: is_initialized ());

	//	Do nothing.
	//	'prepare_for_destruction ();' hasn't to be called,
	//	because this is an abstract base class.
}

//	virtual
bool Character ::
	is_initialized ()
	const
{
	assert (Observable <Messages :: Message <Character> > :: is_initialized ());
	assert (Observer <Messages :: Message <Character> > :: is_initialized ());
	assert (Container_Item <Item> :: is_initialized ());

	return true;
}

Reference <Movable_Model> Character ::
	get_movable_model ()
	const
{
	if (! movable_model . points_to_object ())
	{
		movable_model = get_model () . cast <Movable_Model> ();
	}

	return movable_model;
}

void Character ::
	hit (string fight_mode, Reference <Character> target)
{
	assert (is_initialized ());

	Engines :: Log :: show (name + " hits " + target . get_name () + "!");

	Reference <Messages :: Message <Character> > temp (new Messages :: Battle_Message (fight_mode, Reference <Character> (this), target));
	call_observers (temp);
}

float Character ::
	get_skill (const string & skill_name)
	const
{
	assert (is_initialized ());
	assert (skill_name != "fear");
	assert (skill_name != "like");
	assert (0 < experiences . count (skill_name));

	return 4 / Ogre :: Math :: PI * Ogre :: Math :: ATan (experiences . find (skill_name) -> second / default_experience) . valueRadians ();
}

void Character ::
	add_experience (const string & skill_name, float amount)
{
	assert (0 < experiences . count (skill_name));
	
	experiences [skill_name] += amount;
}
