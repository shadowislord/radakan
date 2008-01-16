#include "engines/log.hpp"
#include "singleton.hpp"

using namespace std;
using namespace Radakan;

//	static
template <class T> T * Ogre :: Singleton <T> ::
	ms_Singleton (NULL);

//	static
template <class T> string Singleton <T> ::
	get_class_name ()
{
	return "Singleton <" + T :: get_class_name () + ">";
}

//	static
template <class T> bool Singleton <T> ::
	is_instantiated ()
{
	return Ogre :: Singleton <T> :: getSingletonPtr () != NULL;
}

//	static
template <class T> Reference <T> Singleton <T> ::
	get ()
{
	assert (is_instantiated ());

	return Reference <T> (Ogre :: Singleton <T> :: getSingletonPtr ());
}

//	static
template <class T> void Singleton <T> ::
	uninstantiate ()
{
	assert (is_instantiated ());
	assert (Ogre :: Singleton <T> :: getSingletonPtr () -> is_initialized ());

	delete Ogre :: Singleton <T> :: getSingletonPtr ();

	assert (! is_instantiated ());
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

#include "engines/audio_engine.hpp"
#include "engines/battle_engine.hpp"
#include "engines/conversation_engine.hpp"
#include "engines/game.hpp"
#include "engines/gui_engine.hpp"
//	#include "engines/plugin_manager.hpp"
#include "engines/settings.hpp"
#include "engines/tracker.hpp"
#include "engines/input_engine.hpp"
#include "items/characters/player_character.hpp"
#include "play_gui.hpp"
#include "strategies/game_modes/menu.hpp"
#include "strategies/game_modes/play.hpp"
#include "world.hpp"

#if RADAKAN_AUDIO_MODE == RADAKAN_AUDIERE_MODE
	template class Singleton <Engines :: Audio_Engine>;
#endif
//	template class Singleton <Engines :: Battle_Engine>;
template class Singleton <Engines :: Conversation_Engine>;
template class Singleton <Engines :: Game>;
template class Singleton <Engines :: GUI_Engine>;
template class Singleton <Engines :: Input_Engine>;
template class Singleton <Engines :: Log>;
//	template class Singleton <Engines :: Plugin_Manager>;
template class Singleton <Engines :: Settings>;
template class Singleton <Play_GUI>;
template class Singleton <Items :: Characters :: Player_Character>;
//	template class Singleton <Plugins :: Battle_Plugin>;
//	template class Singleton <Plugins :: Conversation_Plugin>;
template class Singleton <Strategies :: Game_Modes :: Menu>;
template class Singleton <Strategies :: Game_Modes :: Play>;
template class Singleton <World>;
#ifdef RADAKAN_DEBUG
	template class Singleton <Engines :: Tracker>;
#endif
