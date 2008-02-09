#include "engines/log.hpp"
#include "engines/mediator.hpp"
#include "items/character.hpp"
#include "map.hpp"
#include "messages/nothing.hpp"
#include "movable_model.hpp"
#include "pair.hpp"
#include "set.hpp"
#include "skill.hpp"
#include "slot.hpp"
#include "strategies/behaviors/ai.hpp"
#include "strategies/behaviors/player.hpp"

using namespace std;
using namespace Radakan;
using namespace Radakan :: Items;

//	static
string Character ::
	get_class_name ()
{
	return "Items :: Character";
}

//	static
Reference <Character> Character ::
	get_player_character ()
{
	return Strategies :: Behaviors :: Player :: get () -> character;
}

//  constructor
Character ::
	Character
	(
		string new_name,
		float new_mass,
		Mathematics :: Vector_3D new_size,
		const Reference <Mesh_Data> new_mesh_data,
		string type
	) :
	Object (new_name),
	Container_Item <Item>
	(
		new_name,
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
			Mathematics :: Vector_3D (0.5, 0.5, 0.3),
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
			Mathematics :: Vector_3D (1, 0.3, 0.3),
			Reference <Mesh_Data> ()
		)
	),
	//	legs (new Container_Item <Pants> ()),
	//	feet (new Container_Item <Shoe> ()),
	skills (new Map <string, Skill> ("skills"))
{
	Engines :: Log :: trace (me, Character :: get_class_name (), "", to_string (new_mass),
		new_size . to_string (), new_mesh_data . get_name ());
	add_automatic_destruction_prevention ("construction of " + get_class_name ());
	
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
	Container <Items :: Item> :: seal ();

	if (type == "player character")
	{
		behave <Strategies :: Behaviors :: Player> ();
	}
	else
	{
		assert (type == "npc");

		behave <Strategies :: Behaviors :: AI> ();
	}

	Engines :: Mediator :: get () -> register_observer <Messages :: Nothing>
		(Reference <Observer <Messages :: Nothing> > (this));

	remove_automatic_destruction_prevention ("construction of " + get_class_name ());
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
	assert (Observer <Messages :: Nothing> :: is_initialized ());
	assert (Container_Item <Item> :: is_initialized ());

	return true;
}

void Character ::
	drop (Reference <Strategies :: Behaviors :: Behavior> behavior)
{
	get_movable_model () -> turn (1, get_model () -> get_side_direction ());
	
	Engines :: Log :: show (me . get_name (true) + " died.");

	Slot <Strategies :: Behaviors :: Behavior> :: drop (behavior);
}

bool Character ::
	is_alive () const
{
	return has_active_state ();
}

Reference <Movable_Model> Character ::
	get_movable_model ()
	const
{
	if (! movable_model . points_to_object ())
	{
		movable_model . reset_pointee (get_model () . cast <Movable_Model> (), true);
	}

	return movable_model;
}

void Character ::
	call (Reference <Messages :: Nothing>)
{
	assert (is_initialized ());

	run (Messages :: Nothing :: get ());
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
			Pair <string, Skill> :: create
				(skill_name, Reference <Skill> (new Skill (skill_name)))
		);
	}

	return skills -> look_up (skill_name) -> get_value ();
}

template <class T> void Character ::
	behave ()
{
	assert (! State_Machine <Strategies :: Behaviors :: Behavior> :: has_active_state ());
	
	set_active_state (Reference <Strategies :: Behaviors :: Behavior>
		(new T (Reference <Character> (this))));
}
