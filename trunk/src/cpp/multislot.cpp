#include "multislot.hpp"

using namespace std;
using namespace TSL;

//	static
template <class T> const string Multislot <T> ::
	class_name ("Multislot <" + T :: class_name + ">");

//  constructor
template <class T> Multislot <T> ::
	Multislot
	(
		string new_name,
		string new_mesh_name,
		Ogre :: Vector3 new_size,
		float new_mass,
		unsigned int new_max_size
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
		true
	),
	max_size (new_max_size)
{
	log (debugging) << "Multislot (" << new_name << ", ...)" << endl;
	assert (Container :: is_initialized ());

	assert (is_initialized ());
}

//  destructor
template <class T> Multislot <T> ::
	~Multislot ()
{
	log (debugging) << "~" << class_name << " ()" << endl;
	assert (Container :: is_initialized ());
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
	assert (! item . has_body ());

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
		unsigned int new_max_size
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
