#include <fstream>
#include <sstream>
#include "log.hpp"
#ifdef TSL_DEBUG
	#include "world.hpp"
#endif

using namespace std;
using namespace TSL;
using namespace TSL :: Engines;

//	static
const string Log ::
	get_class_name ()
{
	return "Log";
}

//  constructor
Log ::
	Log () :
	Object ("Log")
{
	trace <Log> (me, "");
	
 //	Do nothing.
	
	assert (is_initialized ());
}

//  destructor
Log ::
	~Log ()
{
	trace <Log> (me, "~");
	assert (is_initialized ());

	//	Do nothing.
}

//	virtual
bool Log ::
	is_initialized ()
	const
{
	return Object :: is_initialized ();
}

//	static
ostream & Log ::
	error (const Object & logger)
{
	log (logger) << "An error occurred:" << endl;
	return cout << "\t";
}

//	static
ostream & Log ::
	log (const Object & logger)
{
	#ifdef TSL_DEBUG
		cout << endl;
		if (World :: is_instantiated ())
		{
			cout << "= turn " << World :: get () . get_turn () << " =" << endl;
		}
		cout << "'" << logger << "' reports:" << endl;
		return cout << "\t";
	#else
		return cout;	//	completely ignored when not in TSL_DEBUG
	#endif
}

//	static
void Log ::
	show (string message_contents)
{
	trace <Log> (get (), "show", message_contents);
	
	if (is_instantiated ())
	{
		Object * message = new Object (message_contents);
		get () . call_observers (* message);
		delete message;
	}
}

//	static
template <class T> void Log ::
	trace
	(
		const Object & logger,
		string method,
		string argument_1,
		string argument_2,
		string argument_3,
		string argument_4,
		string argument_5,
		string argument_6
	)
{
	//	assert (logger . Object :: is_type <T>);
	
	if (method . empty ())
	{
		method = T :: get_class_name ();
	}
	else if (method . at (0) == '~')
	{
		method = "~" + T :: get_class_name ();
	}

	log (logger) << T :: get_class_name () << " :: " << method << " ("
		<< argument_1
		<< (argument_2 = "" ? "" : ", ") << argument_2
		<< (argument_3 = "" ? "" : ", ") << argument_3
		<< (argument_4 = "" ? "" : ", ") << argument_4
		<< (argument_5 = "" ? "" : ", ") << argument_5
		<< (argument_6 = "" ? "" : ", ") << argument_6
		<< ")" << endl;
}

#include "alive_state.hpp"
#include "audio_engine.hpp"
#include "battle_plugin.hpp"
#include "chat_state.hpp"
#include "conversation_message.hpp"
#include "conversation_plugin.hpp"
#include "dead_state.hpp"
#include "fight_state.hpp"
#include "game.hpp"
#include "gui_engine.hpp"
#include "input_engine.hpp"
#include "menu_state.hpp"
#include "player_character.hpp"
#include "plugin_manager.hpp"
#include "settings.hpp"
#include "quit_state.hpp"
#include "tracker.hpp"
#include "world.hpp"

template void Log :: trace <Algorithms :: Algorithm <Engines :: Game> >
(
	const Object & logger,
	string method,
	string argument_1,
	string argument_2,
	string argument_3,
	string argument_4,
	string argument_5,
	string argument_6
);
template void Log :: trace <Algorithms :: Algorithm <Items :: NPC> >
(
	const Object & logger,
	string method,
	string argument_1,
	string argument_2,
	string argument_3,
	string argument_4,
	string argument_5,
	string argument_6
);
template void Log :: trace <State_Machines :: Algorithm_State_Machine <Engines :: Game> >
(
	const Object & logger,
	string method,
	string argument_1,
	string argument_2,
	string argument_3,
	string argument_4,
	string argument_5,
	string argument_6
);
template void Log :: trace <State_Machines :: Algorithm_State_Machine <Items :: NPC> >
(
	const Object & logger,
	string method,
	string argument_1,
	string argument_2,
	string argument_3,
	string argument_4,
	string argument_5,
	string argument_6
);
template void Log :: trace <Algorithms :: Alive_State>
(
	const Object & logger,
	string method,
	string argument_1,
	string argument_2,
	string argument_3,
	string argument_4,
	string argument_5,
	string argument_6
);
template void Log :: trace <Engines :: Audio_Engine>
(
	const Object & logger,
	string method,
	string argument_1,
	string argument_2,
	string argument_3,
	string argument_4,
	string argument_5,
	string argument_6
);
template void Log :: trace <Plugins :: Battle_Plugin>
(
	const Object & logger,
	string method,
	string argument_1,
	string argument_2,
	string argument_3,
	string argument_4,
	string argument_5,
	string argument_6
);
template void Log :: trace <Items :: Character>
(
	const Object & logger,
	string method,
	string argument_1,
	string argument_2,
	string argument_3,
	string argument_4,
	string argument_5,
	string argument_6
);
template void Log :: trace <Algorithms :: Chat_State>
(
	const Object & logger,
	string method,
	string argument_1,
	string argument_2,
	string argument_3,
	string argument_4,
	string argument_5,
	string argument_6
);
template void Log :: trace <Items :: Container>
(
	const Object & logger,
	string method,
	string argument_1,
	string argument_2,
	string argument_3,
	string argument_4,
	string argument_5,
	string argument_6
);
template void Log :: trace <Messages :: Conversation_Message>
(
	const Object & logger,
	string method,
	string argument_1,
	string argument_2,
	string argument_3,
	string argument_4,
	string argument_5,
	string argument_6
);
template void Log :: trace <Plugins :: Conversation_Plugin>
(
	const Object & logger,
	string method,
	string argument_1,
	string argument_2,
	string argument_3,
	string argument_4,
	string argument_5,
	string argument_6
);
template void Log :: trace <Algorithms :: Dead_State>
(
	const Object & logger,
	string method,
	string argument_1,
	string argument_2,
	string argument_3,
	string argument_4,
	string argument_5,
	string argument_6
);
template void Log :: trace <State_Machines :: Data_State_Machine <GUI> >
(
	const Object & logger,
	string method,
	string argument_1,
	string argument_2,
	string argument_3,
	string argument_4,
	string argument_5,
	string argument_6
);
template void Log :: trace <State_Machines :: Data_State_Machine <Tile> >
(
	const Object & logger,
	string method,
	string argument_1,
	string argument_2,
	string argument_3,
	string argument_4,
	string argument_5,
	string argument_6
);
template void Log :: trace <Algorithms :: Fight_State>
(
	const Object & logger,
	string method,
	string argument_1,
	string argument_2,
	string argument_3,
	string argument_4,
	string argument_5,
	string argument_6
);
template void Log :: trace <Engines :: Game>
(
	const Object & logger,
	string method,
	string argument_1,
	string argument_2,
	string argument_3,
	string argument_4,
	string argument_5,
	string argument_6
);
template void Log :: trace <GUI>
(
	const Object & logger,
	string method,
	string argument_1,
	string argument_2,
	string argument_3,
	string argument_4,
	string argument_5,
	string argument_6
);
template void Log :: trace <Engines :: GUI_Engine>
(
	const Object & logger,
	string method,
	string argument_1,
	string argument_2,
	string argument_3,
	string argument_4,
	string argument_5,
	string argument_6
);
template void Log :: trace <Engines :: Input_Engine>
(
	const Object & logger,
	string method,
	string argument_1,
	string argument_2,
	string argument_3,
	string argument_4,
	string argument_5,
	string argument_6
);
template void Log :: trace <Items :: Item>
(
	const Object & logger,
	string method,
	string argument_1,
	string argument_2,
	string argument_3,
	string argument_4,
	string argument_5,
	string argument_6
);
template void Log :: trace <Items :: NPC>
(
	const Object & logger,
	string method,
	string argument_1,
	string argument_2,
	string argument_3,
	string argument_4,
	string argument_5,
	string argument_6
);
template void Log :: trace <Algorithms :: Menu_State>
(
	const Object & logger,
	string method,
	string argument_1,
	string argument_2,
	string argument_3,
	string argument_4,
	string argument_5,
	string argument_6
);
/*template void Log :: trace <Message>
(
	const Object & logger,
	string method,
	string argument_1,
	string argument_2,
	string argument_3,
	string argument_4,
	string argument_5,
	string argument_6
);*/
template void Log :: trace <Model>
(
	const Object & logger,
	string method,
	string argument_1,
	string argument_2,
	string argument_3,
	string argument_4,
	string argument_5,
	string argument_6
);
template void Log :: trace <Movable_Model>
(
	const Object & logger,
	string method,
	string argument_1,
	string argument_2,
	string argument_3,
	string argument_4,
	string argument_5,
	string argument_6
);
template void Log :: trace <Items :: Multislot <Items :: Container> >
(
	const Object & logger,
	string method,
	string argument_1,
	string argument_2,
	string argument_3,
	string argument_4,
	string argument_5,
	string argument_6
);
template void Log :: trace <Items :: Multislot <Items :: Item> >
(
	const Object & logger,
	string method,
	string argument_1,
	string argument_2,
	string argument_3,
	string argument_4,
	string argument_5,
	string argument_6
);
template void Log :: trace <Object>
(
	const Object & logger,
	string method,
	string argument_1,
	string argument_2,
	string argument_3,
	string argument_4,
	string argument_5,
	string argument_6
);
template void Log :: trace <Observable <Items :: Character> >
(
	const Object & logger,
	string method,
	string argument_1,
	string argument_2,
	string argument_3,
	string argument_4,
	string argument_5,
	string argument_6
);
template void Log :: trace <Observable <GUI> >
(
	const Object & logger,
	string method,
	string argument_1,
	string argument_2,
	string argument_3,
	string argument_4,
	string argument_5,
	string argument_6
);
template void Log :: trace <Observable <Engines :: Log> >
(
	const Object & logger,
	string method,
	string argument_1,
	string argument_2,
	string argument_3,
	string argument_4,
	string argument_5,
	string argument_6
);
template void Log :: trace <Observable <Algorithms :: Play_State> >
(
	const Object & logger,
	string method,
	string argument_1,
	string argument_2,
	string argument_3,
	string argument_4,
	string argument_5,
	string argument_6
);
template void Log :: trace <Observer <Items :: Character> >
(
	const Object & logger,
	string method,
	string argument_1,
	string argument_2,
	string argument_3,
	string argument_4,
	string argument_5,
	string argument_6
);
template void Log :: trace <Observer <GUI> >
(
	const Object & logger,
	string method,
	string argument_1,
	string argument_2,
	string argument_3,
	string argument_4,
	string argument_5,
	string argument_6
);
template void Log :: trace <Observer <Engines :: Log> >
(
	const Object & logger,
	string method,
	string argument_1,
	string argument_2,
	string argument_3,
	string argument_4,
	string argument_5,
	string argument_6
);
template void Log :: trace <Observer <Algorithms :: Play_State> >
(
	const Object & logger,
	string method,
	string argument_1,
	string argument_2,
	string argument_3,
	string argument_4,
	string argument_5,
	string argument_6
);
template void Log :: trace <Algorithms :: Play_State>
(
	const Object & logger,
	string method,
	string argument_1,
	string argument_2,
	string argument_3,
	string argument_4,
	string argument_5,
	string argument_6
);
template void Log :: trace <Items :: Player_Character>
(
	const Object & logger,
	string method,
	string argument_1,
	string argument_2,
	string argument_3,
	string argument_4,
	string argument_5,
	string argument_6
);
template void Log :: trace <Algorithms :: Quit_State>
(
	const Object & logger,
	string method,
	string argument_1,
	string argument_2,
	string argument_3,
	string argument_4,
	string argument_5,
	string argument_6
);
template void Log :: trace <Set <Items :: Character> >
(
	const Object & logger,
	string method,
	string argument_1,
	string argument_2,
	string argument_3,
	string argument_4,
	string argument_5,
	string argument_6
);
template void Log :: trace <Set <GUI> >
(
	const Object & logger,
	string method,
	string argument_1,
	string argument_2,
	string argument_3,
	string argument_4,
	string argument_5,
	string argument_6
);
template void Log :: trace <Set <Items :: Item> >
(
	const Object & logger,
	string method,
	string argument_1,
	string argument_2,
	string argument_3,
	string argument_4,
	string argument_5,
	string argument_6
);
template void Log :: trace <Set <Model> >
(
	const Object & logger,
	string method,
	string argument_1,
	string argument_2,
	string argument_3,
	string argument_4,
	string argument_5,
	string argument_6
);
template void Log :: trace <Set <Items :: NPC> >
(
	const Object & logger,
	string method,
	string argument_1,
	string argument_2,
	string argument_3,
	string argument_4,
	string argument_5,
	string argument_6
);
template void Log :: trace <Set <Object> >
(
	const Object & logger,
	string method,
	string argument_1,
	string argument_2,
	string argument_3,
	string argument_4,
	string argument_5,
	string argument_6
);
template void Log :: trace <Set <Observer <Items :: Character> > >
(
	const Object & logger,
	string method,
	string argument_1,
	string argument_2,
	string argument_3,
	string argument_4,
	string argument_5,
	string argument_6
);
template void Log :: trace <Set <Observer <GUI> > >
(
	const Object & logger,
	string method,
	string argument_1,
	string argument_2,
	string argument_3,
	string argument_4,
	string argument_5,
	string argument_6
);
template void Log :: trace <Set <Observer <Engines :: Log> > >
(
	const Object & logger,
	string method,
	string argument_1,
	string argument_2,
	string argument_3,
	string argument_4,
	string argument_5,
	string argument_6
);
template void Log :: trace <Set <Observer <Algorithms :: Play_State> > >
(
	const Object & logger,
	string method,
	string argument_1,
	string argument_2,
	string argument_3,
	string argument_4,
	string argument_5,
	string argument_6
);
template void Log :: trace <Set <Tile> >
(
	const Object & logger,
	string method,
	string argument_1,
	string argument_2,
	string argument_3,
	string argument_4,
	string argument_5,
	string argument_6
);
template void Log :: trace <Set <Sound> >
(
	const Object & logger,
	string method,
	string argument_1,
	string argument_2,
	string argument_3,
	string argument_4,
	string argument_5,
	string argument_6
);
template void Log :: trace <Engines :: Settings>
(
	const Object & logger,
	string method,
	string argument_1,
	string argument_2,
	string argument_3,
	string argument_4,
	string argument_5,
	string argument_6
);
template void Log :: trace <Singleton <Algorithms :: Alive_State> >
(
	const Object & logger,
	string method,
	string argument_1,
	string argument_2,
	string argument_3,
	string argument_4,
	string argument_5,
	string argument_6
);
template void Log :: trace <Singleton <Engines :: Audio_Engine> >
(
	const Object & logger,
	string method,
	string argument_1,
	string argument_2,
	string argument_3,
	string argument_4,
	string argument_5,
	string argument_6
);
template void Log :: trace <Singleton <Plugins :: Battle_Plugin> >
(
	const Object & logger,
	string method,
	string argument_1,
	string argument_2,
	string argument_3,
	string argument_4,
	string argument_5,
	string argument_6
);
template void Log :: trace <Singleton <Algorithms :: Chat_State> >
(
	const Object & logger,
	string method,
	string argument_1,
	string argument_2,
	string argument_3,
	string argument_4,
	string argument_5,
	string argument_6
);
template void Log :: trace <Singleton <Plugins :: Conversation_Plugin> >
(
	const Object & logger,
	string method,
	string argument_1,
	string argument_2,
	string argument_3,
	string argument_4,
	string argument_5,
	string argument_6
);
template void Log :: trace <Singleton <Algorithms :: Dead_State> >
(
	const Object & logger,
	string method,
	string argument_1,
	string argument_2,
	string argument_3,
	string argument_4,
	string argument_5,
	string argument_6
);
template void Log :: trace <Singleton <Algorithms :: Fight_State> >
(
	const Object & logger,
	string method,
	string argument_1,
	string argument_2,
	string argument_3,
	string argument_4,
	string argument_5,
	string argument_6
);
template void Log :: trace <Singleton <Engines :: Game> >
(
	const Object & logger,
	string method,
	string argument_1,
	string argument_2,
	string argument_3,
	string argument_4,
	string argument_5,
	string argument_6
);
template void Log :: trace <Singleton <Engines :: GUI_Engine> >
(
	const Object & logger,
	string method,
	string argument_1,
	string argument_2,
	string argument_3,
	string argument_4,
	string argument_5,
	string argument_6
);
template void Log :: trace <Singleton <Engines :: Input_Engine> >
(
	const Object & logger,
	string method,
	string argument_1,
	string argument_2,
	string argument_3,
	string argument_4,
	string argument_5,
	string argument_6
);
template void Log :: trace <Singleton <Engines :: Log> >
(
	const Object & logger,
	string method,
	string argument_1,
	string argument_2,
	string argument_3,
	string argument_4,
	string argument_5,
	string argument_6
);
template void Log :: trace <Singleton <Algorithms :: Menu_State> >
(
	const Object & logger,
	string method,
	string argument_1,
	string argument_2,
	string argument_3,
	string argument_4,
	string argument_5,
	string argument_6
);
template void Log :: trace <Singleton <Algorithms :: Play_State> >
(
	const Object & logger,
	string method,
	string argument_1,
	string argument_2,
	string argument_3,
	string argument_4,
	string argument_5,
	string argument_6
);
template void Log :: trace <Singleton <Items :: Player_Character> >
(
	const Object & logger,
	string method,
	string argument_1,
	string argument_2,
	string argument_3,
	string argument_4,
	string argument_5,
	string argument_6
);
template void Log :: trace <Singleton <Plugin_Manager> >
(
	const Object & logger,
	string method,
	string argument_1,
	string argument_2,
	string argument_3,
	string argument_4,
	string argument_5,
	string argument_6
);
template void Log :: trace <Singleton <Algorithms :: Quit_State> >
(
	const Object & logger,
	string method,
	string argument_1,
	string argument_2,
	string argument_3,
	string argument_4,
	string argument_5,
	string argument_6
);
template void Log :: trace <Singleton <Engines :: Settings> >
(
	const Object & logger,
	string method,
	string argument_1,
	string argument_2,
	string argument_3,
	string argument_4,
	string argument_5,
	string argument_6
);
template void Log :: trace <Singleton <Engines :: Tracker> >
(
	const Object & logger,
	string method,
	string argument_1,
	string argument_2,
	string argument_3,
	string argument_4,
	string argument_5,
	string argument_6
);
template void Log :: trace <Singleton <World> >
(
	const Object & logger,
	string method,
	string argument_1,
	string argument_2,
	string argument_3,
	string argument_4,
	string argument_5,
	string argument_6
);
template void Log :: trace <Sound>
(
	const Object & logger,
	string method,
	string argument_1,
	string argument_2,
	string argument_3,
	string argument_4,
	string argument_5,
	string argument_6
);
template void Log :: trace <State_Machines :: State_Machine <Algorithms :: Algorithm <Engines :: Game> > >
(
	const Object & logger,
	string method,
	string argument_1,
	string argument_2,
	string argument_3,
	string argument_4,
	string argument_5,
	string argument_6
);
template void Log :: trace <State_Machines :: State_Machine <Algorithms :: Algorithm <Items :: NPC> > >
(
	const Object & logger,
	string method,
	string argument_1,
	string argument_2,
	string argument_3,
	string argument_4,
	string argument_5,
	string argument_6
);
template void Log :: trace <State_Machines :: State_Machine <GUI> >
(
	const Object & logger,
	string method,
	string argument_1,
	string argument_2,
	string argument_3,
	string argument_4,
	string argument_5,
	string argument_6
);
template void Log :: trace <State_Machines :: State_Machine <Tile> >
(
	const Object & logger,
	string method,
	string argument_1,
	string argument_2,
	string argument_3,
	string argument_4,
	string argument_5,
	string argument_6
);
template void Log :: trace <Items :: Static_Item>
(
	const Object & logger,
	string method,
	string argument_1,
	string argument_2,
	string argument_3,
	string argument_4,
	string argument_5,
	string argument_6
);
template void Log :: trace <Tile>
(
	const Object & logger,
	string method,
	string argument_1,
	string argument_2,
	string argument_3,
	string argument_4,
	string argument_5,
	string argument_6
);
template void Log :: trace <Engines :: Tracker>
(
	const Object & logger,
	string method,
	string argument_1,
	string argument_2,
	string argument_3,
	string argument_4,
	string argument_5,
	string argument_6
);
template void Log :: trace <Items :: Weapon>
(
	const Object & logger,
	string method,
	string argument_1,
	string argument_2,
	string argument_3,
	string argument_4,
	string argument_5,
	string argument_6
);
template void Log :: trace <World>
(
	const Object & logger,
	string method,
	string argument_1,
	string argument_2,
	string argument_3,
	string argument_4,
	string argument_5,
	string argument_6
);
