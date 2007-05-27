#include "log.hpp"
#include "singleton.hpp"

using namespace std;
using namespace TSL;

//	static
template <class T> T * Ogre :: Singleton <T> ::
	ms_Singleton (NULL);

//	static
template <class T> const string Singleton <T> ::
	get_class_name ()
{
	return "Singleton <" + T :: get_class_name () + ">";
}

template <class T> Singleton <T> ::
	Singleton () :
	Object ("The name doesn't matter as this class is an abstact class.")
{
	Engines :: Log :: trace <Singleton <T> > (me);

	//	Do nothing.

	assert (Singleton <T> :: is_initialized ());
}

template <class T> Singleton <T> ::
	~Singleton ()
{
	Engines :: Log :: trace <Singleton <T> > (me, "~");
	assert (Singleton <T> :: is_initialized ());

	//	Do nothing.
}

//	virtual
template <class T> bool Singleton <T> ::
	is_initialized ()
	const
{
	assert (Object :: is_initialized ());

	return true;
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

//	static
template <class T> void Singleton <T> ::
	destruct ()
{
	assert (is_instantiated ());
	
	delete & get ();
}

#include "alive_state.hpp"
#include "audio_engine.hpp"
#include "battle_plugin.hpp"
#include "chat_state.hpp"
#include "conversation_plugin.hpp"
#include "dead_state.hpp"
#include "fight_state.hpp"
#include "game.hpp"
#include "gui_engine.hpp"
#include "input_engine.hpp"
#include "menu_state.hpp"
#include "play_state.hpp"
#include "plugin_manager.hpp"
#include "quit_state.hpp"
#include "settings.hpp"
#include "tracker.hpp"

template class Singleton <Algorithms :: Alive_State>;
template class Singleton <Algorithms :: Chat_State>;
template class Singleton <Algorithms :: Dead_State>;
template class Singleton <Algorithms :: Fight_State>;
template class Singleton <Algorithms :: Menu_State>;
template class Singleton <Algorithms :: Play_State>;
template class Singleton <Algorithms :: Quit_State>;
template class Singleton <Engines :: Audio_Engine>;
template class Singleton <Engines :: Game>;
template class Singleton <Engines :: GUI_Engine>;
template class Singleton <Engines :: Input_Engine>;
template class Singleton <Engines :: Log>;
template class Singleton <Engines :: Plugin_Manager>;
template class Singleton <Engines :: Settings>;
template class Singleton <Engines :: Tracker>;
template class Singleton <Items :: Player_Character>;
template class Singleton <Plugins :: Battle_Plugin>;
template class Singleton <Plugins :: Conversation_Plugin>;
template class Singleton <World>;
