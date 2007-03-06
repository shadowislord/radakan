#include "observer.hpp"

using namespace std;
using namespace tsl;

//  constructor
template <class T> Observer <T> ::
	Observer () :
	Object ("The name doesn't matter as this class is an abstact class.")
{
	log (TSL_DEBUG) << get_class_name () << " ()" << endl;
	assert (Object :: is_initialized ());
	
	assert (Observer <T> :: is_initialized ());
}

//  destructor
template <class T> Observer <T> ::
	~Observer ()
{
	log (TSL_DEBUG) << "~" << get_class_name () << " ()" << endl;
	assert (Observer <T> :: is_initialized ());
}

//	virtual
template <class T> bool Observer <T> ::
	is_initialized ()
	const
{
	return warn <Observer <T> > (Object :: is_initialized ());
}

//	static
template <class T> string Observer <T> ::
	get_class_name ()
{
	return "Observer <" + T :: get_class_name () + ">";
}

#include "body.hpp"

template class Observer <Body>;
