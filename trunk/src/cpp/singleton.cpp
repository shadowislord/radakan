#include "log.hpp"
#include "singleton.hpp"

using namespace std;
using namespace Radakan;

//	static
template <class T> T * Ogre :: Singleton <T> ::
	ms_Singleton (NULL);

//	static
template <class T> Reference <T> Singleton <T> ::
	myself;

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
	Engines :: Log :: trace (me, Singleton <T> :: get_class_name ());

	assert (Singleton <T> :: is_initialized ());
}

template <class T> Singleton <T> ::
	~Singleton ()
{
	Engines :: Log :: trace (me, Singleton <T> :: get_class_name (), "~");
	assert (Singleton <T> :: is_initialized ());

	//	Do nothing.
	//	'prepare_for_destruction ();' hasn't to be called,
	//	because this is an abstract base class.
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
template <class T> void Singleton <T> ::
	destruct ()
{
	assert (is_instantiated ());

	myself . reset_pointee ();

	assert (! is_instantiated ());
}

//	static
template <class T> Reference <T> Singleton <T> ::
	get ()
{
	assert (is_instantiated ());

	if (! myself . points_to_object ())
	{
		//	This is not 'dangerous' if the Singleton would not be instantiated.
		myself . reset_pointee (Ogre :: Singleton <T> :: getSingletonPtr ());
	}

	return myself;
}

//	static
template <class T> bool Singleton <T> ::
	is_instantiated ()
{
	return Ogre :: Singleton <T> :: getSingletonPtr () != NULL;
}

#include "audio_engine.hpp"
#include "battle_engine.hpp"
#include "conversation_engine.hpp"
#include "fight_state.hpp"
#include "game.hpp"
#include "gui_engine.hpp"
#include "input_engine.hpp"
#include "menu_state.hpp"
#include "play_state.hpp"
#include "player_character.hpp"
//	#include "plugin_manager.hpp"
#include "settings.hpp"
#include "tracker.hpp"
#include "world.hpp"

template class Singleton <Strategies :: Menu_State>;
template class Singleton <Strategies :: Play_State>;
template class Singleton <Engines :: Audio_Engine>;
template class Singleton <Engines :: Battle_Engine>;
template class Singleton <Engines :: Conversation_Engine>;
template class Singleton <Engines :: Game>;
template class Singleton <Engines :: GUI_Engine>;
template class Singleton <Engines :: Input_Engine>;
template class Singleton <Engines :: Log>;
//	template class Singleton <Engines :: Plugin_Manager>;
template class Singleton <Engines :: Settings>;
template class Singleton <Engines :: Tracker>;
template class Singleton <Items :: Player_Character>;
//	template class Singleton <Plugins :: Battle_Plugin>;
//	template class Singleton <Plugins :: Conversation_Plugin>;
template class Singleton <World>;
