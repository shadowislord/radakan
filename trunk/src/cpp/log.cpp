#include <fstream>
#include <sstream>
#include "log.hpp"

using namespace std;
using namespace TSL;

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
	assert (Object :: is_initialized ());
	
	assert (is_initialized ());
}

//  destructor
Log ::
	~Log ()
{
	trace <Log> (me, "~");
	assert (is_initialized ());
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
		cout << "= turn " << turn << " =" << endl;
		cout << "'" << logger << "' reports:" << endl;
		return cout << "\t";
	#else
		return cout;	//	completely ignored when not in TSL_DEBUG
	#endif
}

//	static
void Log ::
	show (string message)
{
	assert (is_instantiated ());

	trace <Log> (get (), "show", message);
	
	Set <Observer <Log> > & observers = get () . observers;
	for (Observer <Log> * i = observers . get_child (); i != NULL;
		i = observers . get_another_child ())
	{
		i -> call ("show", message);
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

#include "audio_engine.hpp"
#include "battle_engine.hpp"
#include "chat_state.hpp"
#include "dead_state.hpp"
#include "fight_state.hpp"
#include "game.hpp"
#include "gui_engine.hpp"
#include "input_engine.hpp"
#include "menu_state.hpp"
#include "alive_state.hpp"
#include "quit_state.hpp"
#include "speech_state.hpp"
#include "world.hpp"

template void Log :: trace <Algorithm <Game> >
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
template void Log :: trace <Algorithm <NPC> >
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
template void Log :: trace <Algorithm_State_Machine <Game> >
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
template void Log :: trace <Algorithm_State_Machine <NPC> >
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
template void Log :: trace <Alive_State>
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
template void Log :: trace <Audio_Engine>
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
template void Log :: trace <Battle_Engine>
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
template void Log :: trace <Character>
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
template void Log :: trace <Chat_State>
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
template void Log :: trace <Container>
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
template void Log :: trace <Dead_State>
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
template void Log :: trace <Data_State_Machine <GUI> >
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
template void Log :: trace <Data_State_Machine <Tile> >
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
template void Log :: trace <Environment>
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
template void Log :: trace <Fight_State>
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
template void Log :: trace <Game>
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
template void Log :: trace <GUI_Engine>
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
template void Log :: trace <GUI_Listener>
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
template void Log :: trace <Input_Engine>
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
template void Log :: trace <Item>
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
template void Log :: trace <NPC>
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
template void Log :: trace <Menu_State>
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
template void Log :: trace <Multislot <Container> >
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
template void Log :: trace <Multislot <Item> >
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
template void Log :: trace <Observable <Log> >
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
template void Log :: trace <Observer <Log> >
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
template void Log :: trace <Player>
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
template void Log :: trace <Quit_State>
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
template void Log :: trace <Set <Item> >
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
template void Log :: trace <Set <NPC> >
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
template void Log :: trace <Set <Observer <Log> > >
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
template void Log :: trace <Singleton <Alive_State> >
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
template void Log :: trace <Singleton <Audio_Engine> >
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
template void Log :: trace <Singleton <Battle_Engine> >
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
template void Log :: trace <Singleton <Chat_State> >
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
template void Log :: trace <Singleton <Dead_State> >
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
template void Log :: trace <Singleton <Environment> >
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
template void Log :: trace <Singleton <Fight_State> >
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
template void Log :: trace <Singleton <Game> >
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
template void Log :: trace <Singleton <GUI_Engine> >
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
template void Log :: trace <Singleton <GUI_Listener> >
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
template void Log :: trace <Singleton <Input_Engine> >
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
template void Log :: trace <Singleton <Log> >
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
template void Log :: trace <Singleton <Menu_State> >
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
template void Log :: trace <Singleton <Player> >
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
template void Log :: trace <Singleton <Quit_State> >
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
template void Log :: trace <Speech_State>
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
template void Log :: trace <State_Machine <Algorithm <Game> > >
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
template void Log :: trace <State_Machine <Algorithm <NPC> > >
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
template void Log :: trace <State_Machine <GUI> >
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
template void Log :: trace <State_Machine <Tile> >
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
template void Log :: trace <Static_Item>
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
template void Log :: trace <Weapon>
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


