#ifndef OBJECT_IPP
#define OBJECT_IPP

#include "object.hpp"

using namespace std;
using namespace sl;

template <typename T> bool Object ::
	is_type ()
	const
{
	assert (is_initialized ("is_type ()"));
	
	return (dynamic_cast <T *> (const_cast <Object *> (this)) != NULL);
}

template <typename T> T * Object ::
	to_type ()
	const
{
	assert (is_initialized ("to_type ()"));
	assert (is_type <T> ());
	
	return dynamic_cast <T *> (const_cast <Object *> (this));
}

#endif	//	OBJECT_IPP
