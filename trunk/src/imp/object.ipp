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

//	This method is only used for debugging.
//	Example usage: assert (warn <Class> (object -> Class :: is_initialized ()));
//	In case of an assertion failure, you get an informative warning in the log.
template <typename T> bool Object ::
	warn (bool initialization)
	const
{
	if (! initialization)
	{
		error () << " is not fully initialized as " << T :: get_type_name () << "!" << endl;
	}
	return initialization;
}

#endif	//	OBJECT_IPP
