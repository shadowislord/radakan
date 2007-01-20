#include "singleton.hpp"

using namespace std;
using namespace tsl;

template <class T> T * Ogre :: Singleton <T> :: ms_Singleton = NULL;

//  constructor
template <class T> Singleton <T> ::
	Singleton (string new_name) :
	Object (new_name)
{
	trace () << get_class_name () << " (" << new_name << ")" << endl;
	assert (Object :: is_initialized ());

	assert (Singleton :: is_initialized ());
}

//  destructor
template <class T> Singleton <T> ::
	~Singleton ()
{
	trace () << "~" << get_class_name () << " ()" << endl;
	assert (Singleton <T> :: is_initialized ());
}

//	virtual
template <class T> bool Singleton <T> ::
	is_initialized ()
	const
{
	return warn <Singleton <T> > (Object :: is_initialized ());
}

//	static
template <class T> string Singleton <T> ::
	get_class_name ()
{
	return "Singleton <" + T :: get_class_name () + ">";
}

//	static
template <class T> T & Singleton <T> ::
	get ()
{
	assert (is_instantiated ());

	return Ogre :: Singleton <T> :: getSingleton ();
}

//	static
template <class T> bool Singleton <T> ::
	is_instantiated ()
{
	return Ogre :: Singleton <T> :: getSingletonPtr () != NULL;
}

#include "menu_state.hpp"
#include "play_state.hpp"

template class Singleton <Audio_Engine>;
template class Singleton <Battle_Engine>;
template class Singleton <GUI_Engine>;
template class Singleton <Input_Engine>;
template class Singleton <Menu_State>;
template class Singleton <Play_State>;
template class Singleton <Player>;
template class Singleton <TSL>;
