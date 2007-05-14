#include "observer.hpp"

using namespace std;
using namespace TSL;

//	static
template <class T> const string Observer <T> ::
	class_name ("Observer <" + T :: class_name + ">");

//  constructor
template <class T> Observer <T> ::
	Observer () :
	Object ("The name doesn't matter as this class is an abstact class.")
{
	log (debugging) << class_name << " ()" << endl;
	assert (Object :: is_initialized ());
	
	assert (Observer <T> :: is_initialized ());
}

//  destructor
template <class T> Observer <T> ::
	~Observer ()
{
	log (debugging) << "~" << class_name << " ()" << endl;
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
