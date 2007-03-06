#include "multislot.hpp"

using namespace std;
using namespace tsl;

//  constructor
template <class T> Multislot <T> ::
	Multislot
	(
		string new_name,
		string new_mesh_name,
		float new_volume,
		float new_mass,
		unsigned int new_max_size
	) :
	Object (new_name),
	Container
	(
		new_name,
		new_mesh_name,
		new_volume,
		new_mass,
		true,
		true,
		true
	),
	max_size (new_max_size)
{
	log (TSL_DEBUG) << "Multislot (" << new_name << ", ...)" << endl;
	assert (Container :: is_initialized ());

	assert (is_initialized ());
}

//  destructor
template <class T> Multislot <T> ::
	~Multislot ()
{
	log (TSL_DEBUG) << "~" << get_class_name () << " ()" << endl;
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
	assert (! item . has_body ());

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
		float new_mass,
		unsigned int new_max_size
	)
{
	Multislot <T> * temp =
		new Multislot <T>
		(
			new_name,
			new_mesh_name,
			new_volume,
			new_mass,
			new_max_size
		);

	return * temp;
}

template class Multislot <Container>;
template class Multislot <Item>;
