#include "character.hpp"

using namespace std;
using namespace tsl;

//  constructor
Character ::
	Character
	(
		string mesh_name,
		float new_volume,
		float new_weight
	) :
	Object ("The name doesn't matter as this class is an abstact class."),
	Container
	(
		"The name doesn't matter as this class is an abstact class.",
		mesh_name,
		new_volume,
		new_weight,
		true,
		true,
		true
	),
	head (Item :: create (* this + "'s head", "bar.mesh", 1, 1)),
	//	head (Multislot <Hat> :: create (1)),
	//	body (Multislot <Shirt> :: create (1)),
	back (Multislot <Container> :: create (* this + "'s back", "bar.mesh", 50, 0, 1)),
	//	arms (Multislot <Bracer> :: create (2)),
	hands (Multislot <Item> :: create (* this + "'s hands", "bar.mesh", 20, 0, 2))
	//	legs (Multislot <Pants> :: create (1)),
	//	feet (Multislot <Shoe> :: create (2))
{
	trace () << "Character (...)" << endl;
	assert (Container :: is_initialized ());

	bool check = Container :: add (head);
	assert (check);
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

	assert (is_initialized ());
}

//  destructor
Character ::
	~Character ()
{
	trace () << "~" << get_class_name () << " ()" << endl;
	assert (Character :: is_initialized ());
}

//	virtual
bool Character ::
	is_initialized ()
	const
{
	return warn <Character> (Container :: is_initialized ());
}

//	static
string Character ::
	get_class_name ()
{
	return "Character";
}
