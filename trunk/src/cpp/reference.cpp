#include "log.hpp"

using namespace std;
using namespace Radakan;

//	static
template <class T> const string Reference <T> ::
	get_class_name ()
{
	//	Do not turn this into a static data member,
	//	that'd cause problems for template (sub)classes.

	return "Reference <" + T :: get_class_name () + ">";
}

template <class T> Reference <T> & Reference <T> ::
	operator= (const Reference & other)
{
	reset_pointee (other . pointee);

	return * this;
}

template <class T> template <class V> Reference <T> & Reference <T> ::
	operator= (const Reference <V> & other)
{
	reset_pointee (other . pointee);

	return * this;
}

template <class T> Reference <T> ::
	Reference (T * new_pointee) :
	pointee (new_pointee)
{
	Engines :: Log :: trace (* this, get_class_name (), "", (points_to_object () ? pointee -> name : string ("NULL")), "A");
	
	if (points_to_object ())
	{
		pointee -> register_reference (* this);
	}
	
	Engines :: Log :: trace (* this, get_class_name (), "", (points_to_object () ? pointee -> name : string ("NULL")), "A", "(end)");
}

template <class T> Reference <T> ::
	Reference (const Reference <T> & other) :
	pointee (other . pointee)
{
	Engines :: Log :: trace (* this, get_class_name (), "", (points_to_object () ? pointee -> name : string ("NULL")), "B");

	if (points_to_object ())
	{
		pointee -> register_reference (* this);
	}
	
	Engines :: Log :: trace (* this, get_class_name (), "", (points_to_object () ? pointee -> name : string ("NULL")), "B", "(end)");
}

template <class T> template <class V> Reference <T> ::
	Reference (const Reference <V> & other) :
	pointee (other . pointee)
{
	Engines :: Log :: trace (* this, get_class_name (), "", (points_to_object () ? pointee -> name : string ("NULL")), "C");

	if (points_to_object ())
	{
		pointee -> register_reference (* this);
	}
	
	Engines :: Log :: trace (* this, get_class_name (), "", (points_to_object () ? pointee -> name : string ("NULL")), "C", "(end)");
}

template <class T> Reference <T> ::
	~Reference ()
{
	Engines :: Log :: trace (* this, get_class_name (), "~", (points_to_object () ? pointee -> name : string ("NULL")));
	
	if (points_to_object ())
	{
		pointee -> unregister_reference (* this);
	}
}

//	virtual
template <class T> const string Reference <T> ::
	get_name ()
	const
{
	string result = "NULL";
	
	if (points_to_object ())
	{
		result = pointee -> name;
	}
	
	return result + " " + Reference_Base :: get_name ();
}

template <class T> T * Reference <T> ::
	operator-> ()
{
	return pointee;
}

template <class T> const T * Reference <T> ::
	operator-> () const
{
	return pointee;
}

template <class T> bool Reference <T> ::
	points_to_object () const
{
	return pointee != NULL;
}

template <class T> void Reference <T> ::
	reset_pointee (T * new_pointee)
{
	if (points_to_object ())
	{
		pointee -> unregister_reference (* this);
	}
	
	pointee = new_pointee;

	if (points_to_object ())
	{
		pointee -> register_reference (* this);
	}
}

//	to avert linking errors:
#include "alive_state.hpp"
#include "audio_engine.hpp"
#include "battle_engine.hpp"
#include "battle_message.hpp"
#include "character.hpp"
#include "chat_state.hpp"
#include "conversation_engine.hpp"
#include "conversation_message.hpp"
#include "container.hpp"
#include "fight_state.hpp"
#include "game.hpp"
#include "gui.hpp"
#include "gui_engine.hpp"
#include "input_engine.hpp"
#include "menu_state.hpp"
#include "model.hpp"
#include "movable_model.hpp"
#include "multislot.hpp"
#include "npc.hpp"
#include "play_state.hpp"
#include "player_character.hpp"
#include "settings.hpp"
#include "tile.hpp"
#include "tracker.hpp"
#include "thought.hpp"
#include "world.hpp"
#include "weapon.hpp"

template class Reference <Engines :: Audio_Engine>;
template class Reference <Engines :: Battle_Engine>;
template class Reference <Engines :: Conversation_Engine>;
template class Reference <Engines :: Game>;
template class Reference <Engines :: GUI_Engine>;
template class Reference <Engines :: Input_Engine>;
template class Reference <Engines :: Log>;
template class Reference <Engines :: Settings>;
template class Reference <Engines :: Tracker>;
template class Reference <GUI>;
template class Reference <Items :: Character>;
template class Reference <Items :: Container>;
template class Reference <Items :: Item>;
template class Reference <Items :: Multislot <Items :: Item> >;
template class Reference <Items :: Multislot <Items :: Container> >;
template class Reference <Items :: NPC>;
template class Reference <Items :: Player_Character>;
template class Reference <Items :: Weapon>;
template class Reference <Location <GUI> >;
template class Reference <Location <Tile> >;
template class Reference <Messages :: Battle_Message>;
template class Reference <Messages :: Conversation_Message>;
template class Reference <Model>;
template class Reference <Movable_Model>;
template class Reference <Object>;
template class Reference <Observer <Engines :: Log> >;
template class Reference <Observer <GUI> >;
template class Reference <Observer <Items :: Character> >;
template class Reference <Observer <Strategies :: Play_State> >;
template class Reference <Set <GUI> >;
template class Reference <Set <Items :: Character> >;
template class Reference <Set <Items :: Item> >;
template class Reference <Set <Items :: NPC> >;
template class Reference <Set <Messages :: Conversation_Message> >;
template class Reference <Set <Model> >;
template class Reference <Set <Movable_Model> >;
template class Reference <Set <Object> >;
template class Reference <Set <Observer <Engines :: Log> > >;
template class Reference <Set <Observer <GUI> > >;
template class Reference <Set <Observer <Items :: Character> > >;
template class Reference <Set <Observer <Strategies :: Play_State> > >;
template class Reference <Set <Sound_Sample> >;
template class Reference <Set <Strategies :: Strategy> >;
template class Reference <Set <Tile> >;
template class Reference <Set <Thought> >;
template class Reference <Sound_Sample>;
template class Reference <State_Machine <Tile> >;
template class Reference <Strategies :: Strategy>;
template class Reference <Strategies :: Alive_State>;
template class Reference <Strategies :: Fight_State>;
template class Reference <Strategies :: Menu_State>;
template class Reference <Strategies :: Play_State>;
template class Reference <Strategies :: Strategy_State_Machine>;
template class Reference <Tile>;
template class Reference <Thought>;
template class Reference <World>;

//	template class Reference <const Engines :: Audio_Engine>;
//	template class Reference <const Engines :: Game>;
//	template class Reference <const Engines :: GUI_Engine>;
//	template class Reference <const Engines :: Input_Engine>;
//	template class Reference <const Engines :: Log>;
//	template class Reference <const Engines :: Settings>;
//	template class Reference <const Engines :: Tracker>;
template class Reference <const GUI>;
template class Reference <const Items :: Character>;
template class Reference <const Items :: Container>;
//	template class Reference <const Items :: Item>;
//	template class Reference <const Items :: Multislot <Items :: Item> >;
//	template class Reference <const Items :: NPC>;
//	template class Reference <const Items :: Player_Character>;
//	template class Reference <const Items :: Weapon>;
template class Reference <const Location <GUI> >;
template class Reference <const Location <Items :: Item> >;
template class Reference <const Location <Model> >;
template class Reference <const Location <Tile> >;
template class Reference <const Location <Strategies :: Strategy> >;
template class Reference <const Messages :: Battle_Message>;
template class Reference <const Messages :: Conversation_Message>;
//	template class Reference <const Model>;
//	template class Reference <const Movable_Model>;
template class Reference <const Object>;
//	template class Reference <const Observer <Strategies :: Play_State> >;
//	template class Reference <const Observer <Engines :: Log> >;
//	template class Reference <const Observer <GUI> >;
//	template class Reference <const Observer <Items :: Character> >;
template class Reference <const Set <Messages :: Conversation_Message> >;
template class Reference <const Set <GUI> >;
//	template class Reference <const Set <Items :: Item> >;
template class Reference <const Set <Model> >;
//	template class Reference <const Set <Object> >;
//	template class Reference <const Set <Observer <GUI> > >;
//	template class Reference <const Set <Tile> >;
//	template class Reference <const Set <Sound_Sample> >;
//	template class Reference <const Set <Strategies :: Strategy> >;
//	template class Reference <const Sound_Sample>;
//	template class Reference <const State_Machine <Tile> >;
template class Reference <const Strategies :: Strategy>;
template class Reference <const Strategies :: Alive_State>;
template class Reference <const Strategies :: Fight_State>;
//	template class Reference <const Strategies :: Menu_State>;
//	template class Reference <const Strategies :: Play_State>;
//	template class Reference <const Strategies :: Strategy_State_Machine>;
//	template class Reference <const Tile>;
//	template class Reference <const Thought>;
