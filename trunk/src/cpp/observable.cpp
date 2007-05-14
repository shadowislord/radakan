#include "observable.hpp"

using namespace std;
using namespace TSL;

//	static
template <class T> const string Observable <T> ::
	class_name ("Observable <" + T :: class_name + ">");

//  constructor
template <class T> Observable <T> ::
	Observable () :
	Object ("The name doesn't matter as this class is an abstact class.")
{
	Object :: log (debugging) << class_name << " ()" << endl;
	assert (Object :: is_initialized ());

	assert (Observable <T> :: is_initialized ());
}

//  destructor
template <class T> Observable <T> ::
	~Observable ()
{
	Object :: log (debugging) << "~" << class_name << " ()" << endl;
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
