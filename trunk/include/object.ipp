#ifndef OBJECT_IPP
#define OBJECT_IPP

#include "object.hpp"

using namespace std;

template <typename T> T * Object ::
	copy ()
	const
{
	assert (is_initialized ());
	
	T * result = new T (* to_type <T> ());
	result -> assign (* this);
	
	#ifdef SL_DEBUG
		objects.insert (result);
	#endif
	
	debug () << * this << " was copied to " << * result << ". " << endl;
	return result;
}

template <typename T> bool Object ::
	is_type ()
	const
{
	assert (is_initialized ());
	
	return (dynamic_cast <T *> (const_cast <Object *> (this)) != NULL);
}

template <typename T> T * Object ::
	to_type ()
	const
{
	assert (is_initialized ());
	assert (is_type <T> ());
	
	return dynamic_cast <T *> (const_cast <Object *> (this));
}

#endif	//	OBJECT_IPP
