#include "battle_message.hpp"
#include "character.hpp"
#include "conversation_message.hpp"
#include "log.hpp"
#include "movable_model.hpp"
#include "multislot.hpp"

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
	Container
	(
		"The name doesn't matter as this class is an abstact class.",
		new_mesh_name,
		new_size,
		new_mass,
		true,
		true,
		true,
		Set <Item> :: unlimited
	),
	//	head (new Static_Item (name + "'s head", "bar.mesh", 1, 1)),
	//	head (new Multislot <Hat> (1)),
	//	body (new Multislot <Shirt> (1)),
	back (new Multislot <Container> (name + "'s back", "bar.mesh", Ogre :: Vector3 (0.5, 0.5, 0.3), 0, 1)),
	//	arms (new Multislot <Bracer> (2)),
	hands (new Multislot <Item> (name + "'s hands", "bar.mesh", Ogre :: Vector3 (1, 0.3, 0.3), 0, 2)),
	//	legs (new Multislot <Pants> (1)),
	//	feet (new Multislot <Shoe> (2)),
	movable_model (NULL)
{
	Engines :: Log :: trace (me, Character :: get_class_name (), "", new_mesh_name, to_string (new_size), to_string (new_mass));

	bool check/* = Container :: add (head)*/;
	/*assert (check);*/
	//	Container :: add (body);
	check = Container :: add (back);
	assert (check);
	//	Container :: add (arms);
	check = Container :: add (hands);
	assert (check);
	//	Container :: add (legs);
	//	Container :: add (feet);

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
	assert (Observable <Character> :: is_initialized ());
	assert (Observer <Character> :: is_initialized ());
	assert (Container :: is_initialized ());

	return true;
}

Reference <Movable_Model> Character ::
	get_movable_model ()
	const
{
	if (! movable_model . points_to_object ())
	{
		movable_model = get_model () -> to_type <Movable_Model> ();
	}

	return movable_model;
}

void Character ::
	hit (string fight_mode, Reference <Character> target)
{
	assert (is_initialized ());

	Engines :: Log :: show (name + " hits " + target -> name + "!");

	Reference <Messages :: Battle_Message> temp (new Messages :: Battle_Message (fight_mode, Reference <Character> (this), target));
	call_observers (temp);
}

float Character ::
	get_skill (const string & skill_name)
	const
{
	assert (0 < experiences . count (skill_name));

	return 4 / Ogre :: Math :: PI * Ogre :: Math :: ATan (experiences . find (skill_name) -> second / default_experience) . valueRadians ();
}

void Character ::
	add_experience (const string & skill_name, float amount)
{
	assert (0 < experiences . count (skill_name));
	
	experiences [skill_name] += amount;
}
