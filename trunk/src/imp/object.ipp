#ifndef OBJECT_IPP
#define OBJECT_IPP

#include "object.hpp"

using namespace std;
using namespace sl;

template <typename T> bool Object ::
	is_type ()
	const
{
	trace () << "is_type <" << T :: get_type_name () << "> ()" << endl;
	assert (is_initialized ());
	
	return (dynamic_cast <T *> (const_cast <Object *> (this)) != NULL);
}

template <typename T> T & Object ::
	to_type ()
	const
{
	trace () << "to_type <" << T :: get_type_name () << "> ()" << endl;
	assert (is_initialized ());
	assert (is_type <T> ());
	
	return * dynamic_cast <T *> (const_cast <Object *> (this));
}

//	Example usage: assert (warn <Class> (object -> Class :: is_initialized ()));
//	This doesn't only give you the assert, but also an informative warning.
template <typename T> bool Object ::
	warn (bool initialization)
	const
{
	if (! initialization)
	{
		error () << * this << " (" << this << ") is not fully initialized as " << T :: get_type_name () << "!" << endl;
	}
	return initialization;
}

#endif	//	OBJECT_IPP
