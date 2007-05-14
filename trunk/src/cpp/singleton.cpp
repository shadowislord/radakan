#include "singleton.hpp"

using namespace std;
using namespace TSL;

//	static
template <class T> T * Ogre :: Singleton <T> ::
	ms_Singleton (NULL);

//	static
template <class T> const string Singleton <T> ::
	class_name ("Singleton <" + T :: class_name + ">");

template <class T> Singleton <T> ::
	Singleton () :
	Object ("The name doesn't matter as this class is an abstact class.")
{
//	log (debugging) << class_name << " ()" << endl;
	assert (Object :: is_initialized ());

	assert (Singleton :: is_initialized ());
}

template <class T> Singleton <T> ::
	~Singleton ()
{
	log (debugging) << "~" << class_name << " ()" << endl;
	assert (Singleton <T> :: is_initialized ());

	assert (Object :: is_initialized ());
}

//	virtual
template <class T> bool Singleton <T> ::
	is_initialized ()
	const
{
	return Object :: is_initialized ();
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

template <class T> void Singleton <T> ::
	destruct ()
{
	assert (is_instantiated ());
	
	delete & get ();
}

#include "audio_engine.hpp"
#include "battle_engine.hpp"
#include "chat_state.hpp"
#include "dead_state.hpp"
#include "fight_state.hpp"
#include "gui_engine.hpp"
#include "input_engine.hpp"
#include "menu_state.hpp"
#include "alive_state.hpp"
#include "world.hpp"
#include "quit_state.hpp"

template class Singleton <Alive_State>;
template class Singleton <Audio_Engine>;
template class Singleton <Battle_Engine>;
template class Singleton <Chat_State>;
template class Singleton <Dead_State>;
template class Singleton <Environment>;
template class Singleton <Game>;
template class Singleton <GUI_Engine>;
template class Singleton <GUI_Listener>;
template class Singleton <Fight_State>;
template class Singleton <Input_Engine>;
template class Singleton <Log>;
template class Singleton <Menu_State>;
template class Singleton <Player>;
template class Singleton <Quit_State>;
template class Singleton <World>;
