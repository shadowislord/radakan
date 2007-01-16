#include "singleton.hpp"

using namespace std;
using namespace tsl;

template <typename T> T * Ogre :: Singleton <T> :: ms_Singleton = NULL;

//  constructor
template <typename T> Singleton <T> ::
	Singleton (string new_name) :
	Object (new_name)
{
	trace () << "Singleton <" << T :: get_type_name () << "> (" << new_name << ")" << endl;
	assert (Object :: is_initialized ());

	assert (Singleton :: is_initialized ());
}

//  destructor
template <typename T> Singleton <T> ::
	~Singleton ()
{
	trace () << "~Singleton <" << T :: get_type_name () << "> ()" << endl;
	assert (Singleton <T> :: is_initialized ());
}

//	virtual
template <typename T> bool Singleton <T> ::
	is_initialized ()
	const
{
	return warn <Singleton <T> > (Object :: is_initialized ());
}

//	static
template <typename T> string Singleton <T> ::
	get_type_name ()
{
	return "Singleton <" + T :: get_type_name () + ">";
}

//	static
template <typename T> T & Singleton <T> ::
	get ()
{
	assert (is_instantiated ());

	return Ogre :: Singleton <T> :: getSingleton ();
}

//	static
template <typename T> bool Singleton <T> ::
	is_instantiated ()
{
	return Ogre :: Singleton <T> :: getSingletonPtr () != NULL;
}

#include "tsl.hpp"

template class Singleton <Audio_Engine>;
template class Singleton <Battle_Engine>;
template class Singleton <GUI_Engine>;
template class Singleton <Input_Engine>;
template class Singleton <Player>;
template class Singleton <TSL>;
