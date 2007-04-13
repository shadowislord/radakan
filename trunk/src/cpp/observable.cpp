/*#include "observable.hpp"

using namespace std;
using namespace tsl;

//  constructor
template <class T> Observable <T> ::
	Observable (Observer <Body> & new_observer) :
	Object ("The name doesn't matter as this class is an abstact class."),
	observer (NULL)
{
	Object :: log (TSL_DEBUG) << get_class_name () << " ()" << endl;
	assert (Object :: is_initialized ());

	assert (Observable <T> :: is_initialized ());
}

//  destructor
template <class T> Observable <T> ::
	~Observable ()
{
	Object :: log (TSL_DEBUG) << "~" << get_class_name () << " ()" << endl;
	assert (Observable <T> :: is_initialized ());
}

//	virtual
template <class T> bool Observable <T> ::
	is_initialized ()
	const
{
	assert (Object :: is_initialized ());

	return true;
}

//	static
template <class T> string Observable <T> ::
	get_class_name ()
{
	return "Observable <" + T :: get_class_name () + ">";
}

//	to avert linking errors:
#include "item.hpp"

template class Observable <Item>;*/
