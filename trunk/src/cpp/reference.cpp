#include "log.hpp"

using namespace std;
using namespace Radakan;

//	static
template <class T> string Reference <T> ::
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
	assert (is_initialized ());
	assert (other . is_initialized ());
	
	reset_pointee (other . pointee);

	return * this;
}

template <class T> template <class V> bool Reference <T> ::
	operator== (const Reference <V> & other) const
{
	assert (is_initialized ());
	assert (other . is_initialized ());
	
	return pointee == other . pointee;
}

template <class T> bool Reference <T> ::
	operator!= (const Reference <T> & other) const
{
	assert (is_initialized ());
	assert (other . is_initialized ());
	
	return pointee != other . pointee;
}

template <class T> bool Reference <T> ::
	operator< (const Reference <T> & other) const
{
	assert (is_initialized ());
	assert (other . is_initialized ());
	
	return pointee < other . pointee;
}

template <class T> Reference <T> ::
	Reference (T * new_pointee) :
	pointee (new_pointee),
	parent (NULL)
{
	Engines :: Log :: trace (* this, get_class_name (), "", get_name (), "A");
	
	if (pointee != NULL)
	{
		pointee -> register_reference (* this);
	}
	
	assert (is_initialized ());
	Engines :: Log :: trace (* this, get_class_name (), "", get_name (), "A", "(end)");
}

template <class T> Reference <T> ::
	Reference (const Reference <T> & other) :
	pointee (other . pointee),
	parent (NULL)
{
	Engines :: Log :: trace (* this, get_class_name (), "", other . get_name (), "B");
	assert (other . is_initialized ());

	if (pointee != NULL)
	{
		pointee -> register_reference (* this);
	}
	
	assert (is_initialized ());
	Engines :: Log :: trace (* this, get_class_name (), "", other . get_name (), "B", "(end)");
}

template <class T> template <class V> Reference <T> ::
	Reference (const Reference <V> & other) :
	pointee (other . pointee),
	parent (NULL)
{
	Engines :: Log :: trace (* this, get_class_name (), "", other . get_name (), "C");
	assert (other . is_initialized ());

	if (pointee != NULL)
	{
		pointee -> register_reference (* this);
	}
	
	assert (is_initialized ());
	Engines :: Log :: trace (* this, get_class_name (), "", other . get_name (), "C", "(end)");
}

template <class T> Reference <T> ::
	~Reference ()
{
	Engines :: Log :: trace (* this, get_class_name (), "~");
	assert (is_initialized ());
	
	if (pointee != NULL)
	{
		pointee -> unregister_reference (* this);
	}

	if (parent != NULL)
	{
		parent -> drop (* this);
	}
}

//	virtual
template <class T> bool Reference <T> ::
	is_initialized ()
	const
{
	if (parent != NULL)
	{
		assert (pointee != NULL);
//		assert (parent -> pointee != NULL);
//		assert (parent -> parent == NULL);
	}

	return true;
}

//	virtual
template <class T> string Reference <T> ::
	get_name ()
	const
{
	assert (is_initialized ());

	string result = Reference_Base :: get_name () + " ";
	
	if (pointee != NULL)
	{
		result += pointee -> name;

		if (parent != NULL)
		{
			result += " [P: " + parent -> name + "]";
		}

		return result;
	}
	else
	{
		return result + "NULL";
	}
}

template <class T> T * Reference <T> ::
	operator-> ()
{
	assert (is_initialized ());
	assert (pointee != NULL);
	
	return pointee;
}

template <class T> const T * Reference <T> ::
	operator-> ()
	const
{
	assert (is_initialized ());
	assert (pointee != NULL);
	
	return pointee;
}

template <class T> bool Reference <T> ::
	points_to_object ()
	const
{
	assert (is_initialized ());
	
	return pointee != NULL;
}

template <class T> void Reference <T> ::
	reset_pointee (T * new_pointee)
{
	assert (is_initialized ());
	assert (parent == NULL);
	
	if (pointee != NULL)
	{
		pointee -> unregister_reference (* this);
	}
	
	pointee = new_pointee;

	if (pointee != NULL)
	{
		pointee -> register_reference (* this);
	}
}

template <class T> void Reference <T> ::
	set_parent (Set <T> & new_parent)
{
	assert (is_initialized ());
	assert (parent == NULL);

	parent = /*new Reference <Set <T> >*/ (& new_parent);
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
template class Reference <Set <const Object> >;
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

template Reference <Items :: Character> ::
	Reference (const Reference <Items :: NPC> & other);
template Reference <Items :: Character> ::
	Reference (const Reference <Items :: Player_Character> & other);
template Reference <Items :: Item> ::
	Reference (const Reference <Items :: Multislot <Items :: Container> > & other);
template Reference <Items :: Item> ::
	Reference (const Reference <Items :: Multislot <Items :: Item> > & other);
template Reference <Model> ::
	Reference (const Reference <Movable_Model> & other);
template Reference <Observer <Items :: Character> > ::
	Reference (const Reference <Items :: Character> & other);
template Reference <Observer <GUI> > ::
	Reference (const Reference <Engines :: Input_Engine> & other);
template Reference <Set <Items :: Item> > ::
	Reference (const Reference <Items :: Multislot <Items :: Item> > & other);
template Reference <Set <Model> > ::
	Reference (const Reference <Tile> & other);
template Reference <Strategies :: Strategy> ::
	Reference (const Reference <Strategies :: Fight_State> & other);
template Reference <Strategies :: Strategy> ::
	Reference (const Reference <Strategies :: Play_State> & other);
template Reference <Strategies :: Strategy> ::
	Reference (const Reference <Strategies :: Menu_State> & other);

template Reference <const Object> ::
	Reference (const Reference <Messages :: Battle_Message> & other);
template Reference <const Object> ::
	Reference (const Reference <Messages :: Conversation_Message> & other);
template Reference <const Object> ::
	Reference (const Reference <const Messages :: Conversation_Message> & other);
template Reference <const Object> ::
	Reference (const Reference <Object> & other);
template Reference <const Object> ::
	Reference (const Reference <Set <Messages :: Conversation_Message> > & other);

template bool Reference <Items :: Character> ::
	operator== (const Reference <Items :: NPC> & other) const;
template bool Reference <GUI> ::
	operator== (const Reference <GUI> & other) const;
template bool Reference <Items :: Character> ::
	operator== (const Reference <Items :: Player_Character> & other) const;
template bool Reference <const Object> ::
	operator== (const Reference <const Object> & other) const;
