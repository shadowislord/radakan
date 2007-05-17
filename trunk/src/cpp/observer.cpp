#include "log.hpp"
#include "observer.hpp"

using namespace std;
using namespace TSL;

//	static
template <class T> const string Observer <T> ::
	get_class_name ()
{
	return "Observer <" + T :: get_class_name () + ">";
}

//  constructor
template <class T> Observer <T> ::
	Observer () :
	Object ("The name doesn't matter as this class is an abstact class.")
{
	Log :: trace <Observer <T> > (me);
	assert (Object :: is_initialized ());
	
	assert (Observer <T> :: is_initialized ());
}

//  destructor
template <class T> Observer <T> ::
	~Observer ()
{
	Log :: trace <Observer <T> > (me, "~");
	assert (Observer <T> :: is_initialized ());
}

//	virtual
template <class T> bool Observer <T> ::
	is_initialized ()
	const
{
	return Object :: is_initialized ();
}

#include "log.hpp"

template class Observer <Log>;
