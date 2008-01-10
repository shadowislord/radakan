#include "engines/log.hpp"
#include "items/character.hpp"
#include "messages/battle_message.hpp"
#include "messages/conversation_message.hpp"
#include "map.hpp"
#include "movable_model.hpp"
#include "pair.hpp"
#include "set.hpp"
#include "skill.hpp"
#include "slot.hpp"

using namespace std;
using namespace Radakan;
using namespace Radakan :: Items;

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
		float new_mass,
		Ogre :: Vector3 new_size,
		const Reference <Mesh_Data> new_mesh_data
	) :
	Object ("The name doesn't matter as this class is an abstact class."),
	Container_Item <Item>
	(
		"The name doesn't matter as this class is an abstact class.",
		new_mass,
		new_size,
		new_mesh_data
	),
	//	head (new Static_Item (name + "'s head", "bar.mesh", 1, 1)),
	//	head (new Container_Item <Hat> ()),
	//	body (new Container_Item <Shirt> ()),
	back
	(
		new Container_Item <Container_Item <Item> >
		(
			name + "'s back",
			1,
			Ogre :: Vector3 (0.5, 0.5, 0.3),
			Reference <Mesh_Data> ()
		)
	),
	//	arms (new Container_Item <Bracer> ()),
	right_hand
	(
		new Container_Item <Item>
		(
			name + "'s right hand",
			1,
			Ogre :: Vector3 (1, 0.3, 0.3),
			Reference <Mesh_Data> ()
		)
	),
	//	legs (new Container_Item <Pants> ()),
	//	feet (new Container_Item <Shoe> ()),
	skills (new Map <string, Skill> ("skills"))
{
	Engines :: Log :: trace (me, Character :: get_class_name (), "", to_string (new_mass), to_string (new_size), new_mesh_data . get_name ());

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

	for (Pointer <Character> character = characters -> get_child ();
		character . points_to_object (); character = characters -> get_another_child ())
	{
		register_observer (character);
		character -> register_observer (this_character);
	}

	check = characters -> add (this_character);
	assert (check);

	register_observer (this_character);

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

	Reference <Messages :: Message <Character> > temp
		(new Messages :: Battle_Message (fight_mode, Reference <Character> (this), target));
	call_observers (temp);
}

float Character ::
	get_skill (const string & skill_name)
	const
{
	assert (is_initialized ());
	assert (skill_name != "fear");
	assert (skill_name != "like");
	
	if (! skills -> look_up (skill_name) . points_to_object ())
	{
		skills -> add
		(
			Reference <Pair <string, Skill> >
				(new Pair <string, Skill> (skill_name, Reference <Skill> (new Skill (skill_name))))
		);
	}

	return skills -> look_up (skill_name) -> get_value ();
}
