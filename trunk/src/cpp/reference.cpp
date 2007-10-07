//	#include "log.hpp"
#include "reference.hpp"

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

template <class T> bool Reference <T> ::
	operator== (const Reference <T> & other)
	const
{
	assert (is_initialized ());
	assert (other . is_initialized ());
	
	return pointee == other . pointee;
}

template <class T> template <class V> bool Reference <T> ::
	operator== (const Reference <V> & other)
	const
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
	//	Engines :: Log :: trace (* this, get_class_name (), "", get_name (), "A");
	
	if (pointee != NULL)
	{
		pointee -> register_reference (* this);
	}
	
	assert (is_initialized ());
	//	Engines :: Log :: trace (* this, get_class_name (), "", get_name (), "A", "(end)");
}

template <class T> Reference <T> ::
	Reference (const Reference <T> & other) :
	pointee (other . pointee),
	parent (NULL)
{
	//	Engines :: Log :: trace (* this, get_class_name (), "", other . get_name (), "B");
	assert (other . is_initialized ());

	if (pointee != NULL)
	{
		pointee -> register_reference (* this);
	}
	
	assert (is_initialized ());
	//	Engines :: Log :: trace (* this, get_class_name (), "", other . get_name (), "B", "(end)");
}

template <class T> template <class V> Reference <T> ::
	Reference (const Reference <V> & other) :
	pointee (other . pointee),
	parent (NULL)
{
	//	Engines :: Log :: trace (* this, get_class_name (), "", other . get_name (), "C");
	assert (other . is_initialized ());

	if (pointee != NULL)
	{
		pointee -> register_reference (* this);
	}
	
	assert (is_initialized ());
	//	Engines :: Log :: trace (* this, get_class_name (), "", other . get_name (), "C", "(end)");
}

//	virtual
template <class T> Reference <T> ::
	~Reference ()
{
	//	Engines :: Log :: trace (* this, get_class_name (), "~");
	assert (is_initialized ());
	assert (parent == NULL);
	
	if (pointee != NULL)
	{
		pointee -> unregister_reference (* this);
	}
}

//	virtual
template <class T> void Reference <T> ::
	destruct_from_parent () const
{
	//	Engines :: Log :: trace (* this, get_class_name (), "destruct_from_parent");
	assert (is_initialized ());
	assert (parent != NULL);
	
	Set <T> * temp_parent = parent;
	const_cast <Reference <T> *> (this) -> parent = NULL;
		
	//	My parent will delete me.
	temp_parent -> drop (* this);
}

//	virtual
template <class T> bool Reference <T> ::
	is_initialized ()
	const
{
	assert ((pointee != NULL) || (parent == NULL));

	return true;
}

//	virtual
template <class T> string Reference <T> ::
	get_name ()
	const
{
	assert (is_initialized ());

	string result = Reference_Base :: get_name () + " [" + T :: get_class_name () + "] ";
	
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

//	virtual
template <class T> void Reference <T> ::
	reset_pointee ()
{
	//	Engines :: Log :: trace (* this, get_class_name (), "reset_pointee");
	
	assert (is_initialized ());
	assert (parent == NULL);
	
	if (pointee != NULL)
	{
		pointee -> unregister_reference (* this);
	
		pointee = NULL;
	}
}

template <class T> void Reference <T> ::
	reset_pointee (T * new_pointee)
{
	//	Engines :: Log :: trace (* this, get_class_name (), "reset_pointee", "~new_pointee~");
	
	assert (is_initialized ());
	assert (parent == NULL);
	
	reset_pointee ();
	
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

	parent = & new_parent;
}

template <class T> bool Reference <T> ::
	has_parent ()
	const
{
	assert (is_initialized ());

	return parent != NULL;
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
#include "log.hpp"
#include "menu_state.hpp"
#include "model.hpp"
#include "movable_model.hpp"
#include "npc.hpp"
#include "play_state.hpp"
#include "player_character.hpp"
#include "settings.hpp"
#include "slot.hpp"
#include "tile.hpp"
#include "tracker.hpp"
#include "thought.hpp"
#include "world.hpp"
#include "weapon.hpp"

template class Reference <Container <GUI> >;
template class Reference <Container <Items :: Character> >;
template class Reference <Container <Items :: Container_Item <Items :: Container_Item <Items :: Item> > > >;
template class Reference <Container <Items :: Container_Item <Items :: Item> > >;
template class Reference <Container <Items :: Item> >;
template class Reference <Container <Items :: NPC> >;
template class Reference <Container <Messages :: Conversation_Message> >;
template class Reference <Container <Model> >;
template class Reference <Container <Object> >;
template class Reference <Container <Observer <Engines :: Log> > >;
template class Reference <Container <Observer <GUI> > >;
template class Reference <Container <Observer <Items :: Character> > >;
template class Reference <Container <Observer <Strategies :: Play_State> > >;
template class Reference <Container <Sound_Sample> >;
template class Reference <Container <Strategies :: Strategy> >;
template class Reference <Container <Tile> >;
template class Reference <Container <Thought> >;
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
template class Reference <Items :: Container_Item <Items :: Container_Item <Items :: Item> > >;
template class Reference <Items :: Container_Item <Items :: Item> >;
template class Reference <Items :: Item>;
template class Reference <Items :: NPC>;
template class Reference <Items :: Player_Character>;
template class Reference <Items :: Weapon>;
template class Reference <Location <GUI> >;
template class Reference <Location <Items :: Item> >;
template class Reference <Location <Items :: Container_Item <Items :: Container_Item <Items :: Item> > > >;
template class Reference <Location <Items :: Container_Item <Items :: Item> > >;
template class Reference <Location <Model> >;
template class Reference <Location <Strategies :: Strategy> >;
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
template class Reference <Set <Object> >;
template class Reference <Set <Observer <Engines :: Log> > >;
template class Reference <Set <Observer <GUI> > >;
template class Reference <Set <Observer <Items :: Character> > >;
template class Reference <Set <Observer <Strategies :: Play_State> > >;
template class Reference <Set <Sound_Sample> >;
template class Reference <Set <Strategies :: Strategy> >;
template class Reference <Set <Tile> >;
template class Reference <Set <Thought> >;
template class Reference <Slot <Strategies :: Strategy> >;
template class Reference <Slot <Tile> >;
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

template Reference <Container <Items :: Item> > ::
	Reference (const Reference <Items :: Container_Item <Items :: Item> > & other);
template Reference <Items :: Item> ::
	Reference (const Reference <Items :: Container_Item <Items :: Item> > & other);
template Reference <Items :: Item> ::
	Reference (const Reference <Items :: Container_Item <Items :: Container_Item <Items :: Item> > > & other);
template Reference <Items :: Character> ::
	Reference (const Reference <Items :: NPC> & other);
template Reference <Items :: Character> ::
	Reference (const Reference <Items :: Player_Character> & other);
template Reference <Model> ::
	Reference (const Reference <Movable_Model> & other);
template Reference <Object> ::
	Reference (const Reference <Messages :: Battle_Message> & other);
template Reference <Object> ::
	Reference (const Reference <Messages :: Conversation_Message> & other);
template Reference <Object> ::
	Reference (const Reference <Set <Messages :: Conversation_Message> > & other);
template Reference <Observer <Items :: Character> > ::
	Reference (const Reference <Items :: Character> & other);
template Reference <Observer <GUI> > ::
	Reference (const Reference <Engines :: Input_Engine> & other);
template Reference <Container <Model> > ::
	Reference (const Reference <Tile> & other);
template Reference <Strategies :: Strategy> ::
	Reference (const Reference <Strategies :: Fight_State> & other);
template Reference <Strategies :: Strategy> ::
	Reference (const Reference <Strategies :: Play_State> & other);
template Reference <Strategies :: Strategy> ::
	Reference (const Reference <Strategies :: Menu_State> & other);

template bool Reference <Items :: Character> ::
	operator== (const Reference <Items :: NPC> & other) const;
template bool Reference <Items :: Character> ::
	operator== (const Reference <Items :: Player_Character> & other) const;
