#include "algorithm.hpp"

using namespace std;
using namespace tsl;

//  constructor
template <class T> Algorithm <T> ::
	Algorithm () :
	Object ("The name doesn't matter as this class is an abstact class.")
{
	log (TSL_DEBUG) << get_class_name () << " ()" << endl;
	assert (Object :: is_initialized ());
	
	assert (Algorithm <T> :: is_initialized ());
}

//  destructor
template <class T> Algorithm <T> ::
	~Algorithm ()
{
	log (TSL_DEBUG) << "~" << get_class_name () << " ()" << endl;
	assert (Algorithm <T> :: is_initialized ());
}

//	virtual
template <class T> bool Algorithm <T> ::
	is_initialized ()
	const
{
	return warn <Algorithm <T> > (Object :: is_initialized ());
}

//	static
template <class T> string Algorithm <T> ::
	get_class_name ()
{
	return "Algorithm <" + T :: get_class_name () + ">";
}

#include "tsl.hpp"
#include "npc.hpp"

template class Algorithm <NPC>;
template class Algorithm <TSL>;
