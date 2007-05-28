#include "log.hpp"
#include "multislot.hpp"

using namespace std;
using namespace TSL;
using namespace TSL :: Items;

//	static
template <class T> const string Multislot <T> ::
	get_class_name ()
{
	return "Multislot <" + T :: get_class_name () + ">";
}

//  constructor
template <class T> Multislot <T> ::
	Multislot
	(
		string new_name,
		string new_mesh_name,
		Ogre :: Vector3 new_size,
		float new_mass,
		int new_max_size
	) :
	Object (new_name),
	Container
	(
		new_name,
		new_mesh_name,
		new_size,
		new_mass,
		true,
		true,
		true,
		new_max_size
	)
{
	Engines :: Log :: trace (me, Multislot :: get_class_name (), "");
	
	//	Do nothing.

	assert (is_initialized ());
}

//  destructor
template <class T> Multislot <T> ::
	~Multislot ()
{
	Engines :: Log :: trace (me, Multislot :: get_class_name (), "~");
	assert (Container :: is_initialized ());

	forget_dependencies ();
}

//	virtual
template <class T> bool Multislot <T> ::
	is_initialized ()
	const
{
	return Container :: is_initialized ();
}

//	virtual
template <class T> bool Multislot <T> ::
	add (Item & item)
{
	assert (is_initialized ());
	assert (item . is_initialized ());
	assert (! item . has_model ());

	if (item . is_type <T> ())
	{
		return Container :: add (item . to_type <T> ());
	}

	return false;
}

//	static
template <class T> Multislot <T> & Multislot <T> ::
	create
	(
		string new_name,
		string new_mesh_name,
		Ogre :: Vector3 new_size,
		float new_mass,
		int new_max_size
	)
{
	Multislot <T> * temp =
		new Multislot <T>
		(
			new_name,
			new_mesh_name,
			new_size,
			new_mass,
			new_max_size
		);

	return * temp;
}

template class Multislot <Container>;
template class Multislot <Item>;
