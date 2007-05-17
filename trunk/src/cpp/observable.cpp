#include "log.hpp"
#include "observable.hpp"

using namespace std;
using namespace TSL;

//	static
template <class T> const string Observable <T> ::
	get_class_name ()
{
	return "Observable <" + T :: get_class_name () + ">";
}

//  constructor
template <class T> Observable <T> ::
	Observable () :
	Object ("The name doesn't matter as this class is an abstact class."),
	observers (my + "observers")
{
	Log :: trace <Observable> (me, "");
	assert (Object :: is_initialized ());

	assert (Observable <T> :: is_initialized ());
}

//  destructor
template <class T> Observable <T> ::
	~Observable ()
{
	Log :: trace <Observable> (me, "~");
	assert (Observable <T> :: is_initialized ());
	
	assert (Object :: is_initialized ());
}

//	virtual
template <class T> bool Observable <T> ::
	is_initialized ()
	const
{
	assert (Object :: is_initialized ());

	return true;
}

//	to avert linking errors:
#include "log.hpp"

template class Observable <Log>;
