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
	//	head (Static_Item :: create (my + "head", "bar.mesh", 1, 1)),
	//	head (Multislot <Hat> :: create (1)),
	//	body (Multislot <Shirt> :: create (1)),
	back (Multislot <Container> :: create (my + "back", "bar.mesh", Ogre :: Vector3 (0.5, 0.5, 0.3), 0, 1)),
	//	arms (Multislot <Bracer> :: create (2)),
	hands (Multislot <Item> :: create (my + "hands", "bar.mesh", Ogre :: Vector3 (1, 0.3, 0.3), 0, 2)),
	//	legs (Multislot <Pants> :: create (1)),
	//	feet (Multislot <Shoe> :: create (2)),
	movable_model (NULL)
{
	Engines :: Log :: trace <Character> (me, "", new_mesh_name, to_string (new_size), to_string (new_mass));

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

	characters . add (* this);

	assert (is_initialized ());
}

//  destructor
Character ::
	~Character ()
{
	Engines :: Log :: trace <Character> (me, "~");
	assert (Character :: is_initialized ());

	characters . drop (* this);

	for (Character * character = characters . get_child ();
		character != NULL; character = characters . get_another_child ())
	{
		drop_observer (* character);
		character -> drop_observer (* this);
	}
}

//	virtual
bool Character ::
	is_initialized ()
	const
{
	return Container :: is_initialized ();
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
	chat (string message_contents, Character & target)
{
	assert (is_initialized ());

	Object * message = new Messages :: Conversation_Message (message_contents, * this, target);

	call_observers (* message);

	delete message;
}
