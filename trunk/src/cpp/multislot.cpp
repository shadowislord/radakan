#include "multislot.hpp"

using namespace std;
using namespace tsl;

//  constructor
template <class T> Multislot <T> ::
	Multislot
	(
		string new_name,
		string mesh_name,
		float new_volume,
		float new_weight,
		unsigned int new_max
	) :
	Object (new_name),
	Container
	(
		new_name,
		mesh_name,
		new_volume,
		new_weight,
		true,
		true,
		true
	),
	max (new_max)
{
	trace () << "Multislot (" << new_name << ")" << endl;
	assert (Container :: is_initialized ());

	assert (is_initialized ());
}

//  destructor
template <class T> Multislot <T> ::
	~Multislot ()
{
	trace () << "~" << get_class_name () << " ()" << endl;
	assert (Container :: is_initialized ());
}

//	virtual
template <class T> bool Multislot <T> ::
	is_initialized ()
	const
{
	return warn <Multislot <T> > (Container :: is_initialized ());
}

//	static
template <class T> string Multislot <T> ::
	get_class_name ()
{
	return "Multislot <" + T :: get_class_name () + ">";
}

//	virtual
template <class T> bool Multislot <T> ::
	add (Item & item)
{
	assert (is_initialized ());
	assert (item . is_initialized ());
	assert (! item . has_representation ());

	if (item . is_type <T> ())
	{
		bool check = Container :: add (item . to_type <T> ());
		assert (check);
		return true;
	}

	return false;
}

//	static
template <class T> Multislot <T> & Multislot <T> ::
	create
	(
		string new_name,
		string new_mesh_name,
		float new_volume,
		float new_weight,
		unsigned int new_max
	)
{
	Multislot <T> * temp =
		new Multislot <T>
		(
			new_name,
			new_mesh_name,
			new_volume,
			new_weight,
			new_max
		);

	return * temp;
}

template class Multislot <Container>;
template class Multislot <Item>;
