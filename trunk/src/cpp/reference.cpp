#include "log.hpp"

using namespace std;
using namespace Radakan;

//	static
template <class T, class U> const string Reference <T, U> ::
	get_class_name ()
{
	//	Do not turn this into a static data member,
	//	that'd cause problems for template (sub)classes.

	return "Reference <" + T :: get_class_name () + ", " + U :: get_class_name () + ">";
}

template <class T, class U> Reference <T, U> & Reference <T, U> ::
	operator= (const Reference & other)
{
	reset_pointee (other . pointee);

	return * this;
}

template <class T, class U> template <class V, class W> Reference <T, U> & Reference <T, U> ::
	operator= (const Reference <V, W> & other)
{
	reset_pointee (other . pointee);

	return * this;
}

template <class T, class U> template <class V, class W> bool Reference <T, U> ::
	operator== (const Reference <V, W> & other) const
{
	return counter == other . counter;
}

template <class T, class U> Reference <T, U> ::
	Reference (T * new_pointee) :
	pointee (new_pointee),
	parent (NULL)
{
	Engines :: Log :: trace (* this, get_class_name (), "", (points_to_object () ? pointee -> name : string ("NULL")), "A");
	
	if (points_to_object ())
	{
		pointee -> register_reference (* this);
	}
	
	Engines :: Log :: trace (* this, get_class_name (), "", (points_to_object () ? pointee -> name : string ("NULL")), "A", "(end)");
}

template <class T, class U> Reference <T, U> ::
	Reference (const Reference & other) :
	pointee (other . pointee),
	parent (NULL)
{
	Engines :: Log :: trace (* this, get_class_name (), "", (points_to_object () ? pointee -> name : string ("NULL")), "B");

	if (points_to_object ())
	{
		pointee -> register_reference (* this);
	}
	
	Engines :: Log :: trace (* this, get_class_name (), "", (points_to_object () ? pointee -> name : string ("NULL")), "B", "(end)");
}

template <class T, class U> template <class V, class W> Reference <T, U> ::
	Reference (const Reference <V, W> & other) :
	pointee (other . pointee),
	parent (NULL)
{
	Engines :: Log :: trace (* this, get_class_name (), "", (points_to_object () ? pointee -> name : string ("NULL")), "C");

	if (points_to_object ())
	{
		pointee -> register_reference (* this);
	}
	
	Engines :: Log :: trace (* this, get_class_name (), "", (points_to_object () ? pointee -> name : string ("NULL")), "C", "(end)");
}

template <class T, class U> Reference <T, U> ::
	~Reference ()
{
	Engines :: Log :: trace (* this, get_class_name (), "~", (points_to_object () ? pointee -> name : string ("NULL")));
	
	if (has_parent ())
	{
		parent -> drop (* this);
	}

	if (points_to_object ())
	{
		pointee -> unregister_reference (* this);
	}
}

//	virtual
template <class T, class U> string Reference <T, U> ::
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

template <class T, class U> T * Reference <T, U> ::
	operator-> ()
{
	return pointee;
}

template <class T, class U> const T * Reference <T, U> ::
	operator-> () const
{
	return pointee;
}

template <class T, class U> bool Reference <T, U> ::
	points_to_object () const
{
	return pointee != NULL;
}

template <class T, class U> void Reference <T, U> ::
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

template <class T, class U> bool Reference <T, U> ::
	has_parent () const
{
	return parent != NULL;
}

template <class T, class U> void Reference <T, U> ::
	set_parent (U * new_parent)
{
	assert (! has_parent ());

	parent = new_parent;
}

//	to avert linking errors:
#include "alive_state.hpp"
#include "audio_engine.hpp"
#include "battle_message.hpp"
#include "character.hpp"
#include "chat_state.hpp"
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
#include "tought.hpp"
#include "world.hpp"
#include "weapon.hpp"

template class Reference <Engines :: Audio_Engine>;
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
template class Reference <Set <Messages :: Conversation_Message> >;
template class Reference <Set <GUI> >;
template class Reference <Set <Items :: Item> >;
template class Reference <Set <Object> >;
template class Reference <Set <Observer <GUI> > >;
template class Reference <Set <Tile> >;
template class Reference <Set <Sound_Sample> >;
template class Reference <Sound_Sample>;
template class Reference <State_Machine <Tile> >;
template class Reference <Strategies :: Strategy>;
template class Reference <Strategies :: Alive_State>;
template class Reference <Strategies :: Fight_State>;
template class Reference <Strategies :: Menu_State>;
template class Reference <Strategies :: Play_State>;
template class Reference <Strategies :: Strategy_State_Machine>;
template class Reference <Tile>;
template class Reference <Tought>;

template class Reference <const Engines :: Audio_Engine>;
template class Reference <const Engines :: Game>;
template class Reference <const Engines :: GUI_Engine>;
template class Reference <const Engines :: Input_Engine>;
template class Reference <const Engines :: Log>;
template class Reference <const Engines :: Settings>;
template class Reference <const Engines :: Tracker>;
template class Reference <const GUI>;
template class Reference <const Items :: Character>;
template class Reference <const Items :: Container>;
template class Reference <const Items :: Item>;
template class Reference <const Items :: Multislot <Items :: Item> >;
template class Reference <const Items :: NPC>;
template class Reference <const Items :: Player_Character>;
template class Reference <const Items :: Weapon>;
template class Reference <const Location <GUI> >;
template class Reference <const Location <Tile> >;
template class Reference <const Messages :: Battle_Message>;
template class Reference <const Messages :: Conversation_Message>;
template class Reference <const Model>;
template class Reference <const Movable_Model>;
template class Reference <const Object>;
template class Reference <const Observer <Strategies :: Play_State> >;
template class Reference <const Observer <Engines :: Log> >;
template class Reference <const Observer <GUI> >;
template class Reference <const Observer <Items :: Character> >;
template class Reference <const Set <Messages :: Conversation_Message> >;
template class Reference <const Set <GUI> >;
template class Reference <const Set <Items :: Item> >;
template class Reference <const Set <Object> >;
template class Reference <const Set <Observer <GUI> > >;
template class Reference <const Set <Tile> >;
template class Reference <const Set <Sound_Sample> >;
template class Reference <const Sound_Sample>;
template class Reference <const State_Machine <Tile> >;
template class Reference <const Strategies :: Strategy>;
template class Reference <const Strategies :: Alive_State>;
template class Reference <const Strategies :: Fight_State>;
template class Reference <const Strategies :: Menu_State>;
template class Reference <const Strategies :: Play_State>;
template class Reference <const Strategies :: Strategy_State_Machine>;
template class Reference <const Tile>;
template class Reference <const Tought>;
