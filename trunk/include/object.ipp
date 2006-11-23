#ifndef OBJECT_IPP
#define OBJECT_IPP

#include "object.hpp"

using namespace std;
using namespace sl;

template <typename T> bool Object ::
	is_type ()
	const
{
	trace () << "is_type ()" << endl;
	assert (is_initialized ());
	
	return (dynamic_cast <T *> (const_cast <Object *> (this)) != NULL);
}

template <typename T> T * Object ::
	to_type ()
	const
{
	trace () << "to_type ()" << endl;
	assert (is_initialized ());
	assert (is_type <T> ());
	
	return dynamic_cast <T *> (const_cast <Object *> (this));
}

#endif	//	OBJECT_IPP
