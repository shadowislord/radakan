#include "battle_message.hpp"
#include "character.hpp"
#include "conversation_message.hpp"
#include "log.hpp"
#include "movable_model.hpp"

using namespace std;
using namespace TSL;
using namespace TSL :: Items;

//	static
const string Character ::
	get_class_name ()
{
	return "Character";
}

//	static
Set <Character> Character ::
	characters ("characters (static)");

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
	//	head (* new Static_Item (my + "head", "bar.mesh", 1, 1)),
	//	head (* new Multislot <Hat> (1)),
	//	body (* new Multislot <Shirt> (1)),
	back (* new Multislot <Container> (my + "back", "bar.mesh", Ogre :: Vector3 (0.5, 0.5, 0.3), 0, 1)),
	//	arms (* new Multislot <Bracer> (2)),
	hands (* new Multislot <Item> (my + "hands", "bar.mesh", Ogre :: Vector3 (1, 0.3, 0.3), 0, 2)),
	//	legs (* new Multislot <Pants> (1)),
	//	feet (* new Multislot <Shoe> (2)),
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

	for (Character * character = characters . get_child ();
		character != NULL; character = characters . get_another_child ())
	{
		register_observer (* character);
		character -> register_observer (* this);
	}

	check = characters . add (* this);
	assert (check);

	register_observer (* this);

	assert (is_initialized ());
}

//  destructor
Character ::
	~Character ()
{
	Engines :: Log :: trace (me, Character :: get_class_name (), "~");
	assert (Character :: is_initialized ());

	//	Do nothing.
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

Movable_Model & Character ::
	get_movable_model ()
	const
{
	if (movable_model == NULL)
	{
		const_cast <Character *> (this) -> movable_model
			= & get_model () . to_type <Movable_Model> ();
	}

	return * movable_model;
}

void Character ::
	chat (TiXmlElement & option, Character & target)
{
	assert (is_initialized ());

	Object * message = new Messages :: Conversation_Message (option, * this, target);
	call_observers (* message);
	delete message;
}

void Character ::
	hit (string fight_mode, Character & target)
{
	assert (is_initialized ());

	Engines :: Log :: show (me + " hits " + target + "!");

	Object * message = new Messages :: Battle_Message (fight_mode, * this, target);
	call_observers (* message);
	delete message;
}
