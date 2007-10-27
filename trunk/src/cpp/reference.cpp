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
	operator= (const Reference <T> & other)
{
	reset_pointee (other . pointee);

	return * this;
}

template <class T> template <class U> Reference <T> & Reference <T> ::
	operator= (const Reference <U> & other)
{
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

template <class T> template <class U> bool Reference <T> ::
	operator== (const Reference <U> & other)
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

template <class T> template <class U> Reference <T> ::
	Reference (const Reference <U> & other) :
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
	
	//	My parent deletes me.
	parent -> drop (* this);
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

	#ifdef RADAKAN_DEBUG
		string result = "'" + Reference_Base :: get_name () + " [" + T :: get_class_name () + "] ";
		
		if (pointee == NULL)
		{
			result += "NULL";
		}
		else
		{
			result += pointee -> name;

			if (parent != NULL)
			{
				result += " [P: " + parent . get_name () + "]";
			}
		}

		return result + "'";
	#else
		if (pointee == NULL)
		{
			return "NULL";
		}
		else
		{
			return pointee -> name;
		}
	#endif
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

template <class T> template <class U> bool Reference <T> ::
	is_castable ()
	const
{
//	Engines :: Log :: trace (* this, Reference <T> :: get_class_name (), "is_castable", "<" + U :: get_class_name () + ">");
	assert (is_initialized ());

	return (dynamic_cast <const U *> (pointee) != NULL);
}

template <class T> template <class U> Reference <U> Reference <T> ::
	cast ()
{
//	Engines :: Log :: trace (* this, Reference <T> :: get_class_name (), "cast", "<" + U :: get_class_name () + ">");
	assert (is_initialized ());
	assert (is_castable <U> ());

	return Reference <U> (dynamic_cast <U *> (pointee));
}

template <class T> template <class U> const Reference <U> Reference <T> ::
	cast_const ()
	const
{
//	Engines :: Log :: trace (* this, Reference <T> :: get_class_name (), "cast_const", "<" + U :: get_class_name () + ">");
	assert (is_initialized ());
	assert (is_castable <U> ());

	return (const_cast <Reference <T> *> (this)) -> cast <U> ();
}

//	to avert linking errors:
#include "container.hpp"
#include "engines/audio_engine.hpp"
#include "engines/battle_engine.hpp"
#include "engines/conversation_engine.hpp"
#include "engines/game.hpp"
#include "engines/gui_engine.hpp"
#include "engines/input_engine.hpp"
#include "engines/log.hpp"
#include "engines/settings.hpp"
#include "engines/tracker.hpp"
#include "gui.hpp"
#include "items/npc.hpp"
#include "items/player_character.hpp"
#include "items/weapon.hpp"
#include "messages/battle_message.hpp"
#include "messages/conversation_message.hpp"
#include "model.hpp"
#include "movable_model.hpp"
#include "opinion.hpp"
#include "play_state_gui.hpp"
#include "slot.hpp"
#include "strategies/alive_state.hpp"
#include "strategies/chat_state.hpp"
#include "strategies/fight_state.hpp"
#include "strategies/menu_state.hpp"
#include "strategies/play_state.hpp"
#include "tile.hpp"
#include "world.hpp"

template class Reference <Container <GUI> >;
template class Reference <Container <Items :: Character> >;
template class Reference <Container <Items :: Container_Item <Items :: Container_Item <Items :: Item> > > >;
template class Reference <Container <Items :: Container_Item <Items :: Item> > >;
template class Reference <Container <Items :: Item> >;
template class Reference <Container <Items :: NPC> >;
template class Reference <Container <Messages :: Message <Items :: Character> > >;
template class Reference <Container <Model> >;
template class Reference <Container <Object> >;
template class Reference <Container <Observer <Messages :: Message <Items :: Character> > > >;
template class Reference <Container <Observer <Object> > >;
template class Reference <Container <Opinion> >;
template class Reference <Container <Sound_Sample> >;
template class Reference <Container <Strategies :: Strategy <Engines :: Game> > >;
template class Reference <Container <Strategies :: Strategy <Items :: Character> > >;
template class Reference <Container <Tile> >;
template class Reference <Engines :: Audio_Engine>;
//	template class Reference <Engines :: Battle_Engine>;
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
template class Reference <Location <Strategies :: Strategy <Engines :: Game> > >;
template class Reference <Location <Strategies :: Strategy <Items :: Character> > >;
template class Reference <Location <Tile> >;
template class Reference <Messages :: Battle_Message>;
template class Reference <Messages :: Conversation_Message>;
template class Reference <Messages :: Message <Engines :: Game> >;
template class Reference <Messages :: Message <Items :: Character> >;
template class Reference <Model>;
template class Reference <Movable_Model>;
template class Reference <Object>;
template class Reference <Play_State_GUI>;
template class Reference <Observer <Messages :: Message <Items :: Character> > >;
template class Reference <Observer <Object> >;
template class Reference <Opinion>;
template class Reference <Set <GUI> >;
template class Reference <Set <Items :: Character> >;
template class Reference <Set <Items :: Item> >;
template class Reference <Set <Items :: NPC> >;
template class Reference <Set <Messages :: Message <Items :: Character> > >;
template class Reference <Set <Model> >;
template class Reference <Set <Object> >;
template class Reference <Set <Observer <Messages :: Message <Items :: Character> > > >;
template class Reference <Set <Observer <Object> > >;
template class Reference <Set <Opinion> >;
template class Reference <Set <Sound_Sample> >;
template class Reference <Set <Strategies :: Strategy <Engines :: Game> > >;
template class Reference <Set <Strategies :: Strategy <Items :: Character> > >;
template class Reference <Set <Tile> >;
template class Reference <Slot <Strategies :: Strategy <Engines :: Game> > >;
template class Reference <Slot <Strategies :: Strategy <Items :: Character> > >;
template class Reference <Slot <Tile> >;
template class Reference <Sound_Sample>;
template class Reference <State_Machine <Tile> >;
template class Reference <Strategies :: Strategy <Engines :: Game> >;
template class Reference <Strategies :: Strategy <Items :: Character> >;
template class Reference <Strategies :: Alive_State>;
template class Reference <Strategies :: Fight_State>;
template class Reference <Strategies :: Menu_State>;
template class Reference <Strategies :: Play_State>;
template class Reference <Strategies :: Strategy_State_Machine <Engines :: Game> >;
template class Reference <Strategies :: Strategy_State_Machine <Items :: Character> >;
template class Reference <Tile>;
template class Reference <World>;

template Reference <Items :: Character> & Reference <Items :: Character> ::
	operator= (const Reference <Items :: NPC> & other);
template Reference <Items :: Item> & Reference <Items :: Item> ::
	operator= (const Reference <Items :: NPC> & other);
	
template bool Reference <Items :: Character> ::
	operator== (const Reference <Items :: NPC> & other) const;
template bool Reference <Items :: Character> ::
	operator== (const Reference <Items :: Player_Character> & other) const;

template Reference <Container <Items :: Item> > ::
	Reference (const Reference <Items :: Container_Item <Items :: Item> > & other);
template Reference <GUI> ::
	Reference (const Reference <Play_State_GUI> & other);
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
//	template Reference <Object> ::
//		Reference (const Reference <Messages :: Battle_Message> & other);
template Reference <Messages :: Message <Items :: Character> > ::
	Reference (const Reference <Messages :: Conversation_Message> & other);
template Reference <Observer <Messages :: Message <Items :: Character> > > ::
	Reference (const Reference <Items :: Character> & other);
template Reference <Observer <Object> > ::
	Reference (const Reference <Engines :: Input_Engine> & other);
template Reference <Observer <Messages :: Message <Items :: Character> > > ::
	Reference (const Reference <Engines :: Input_Engine> & other);
template Reference <Observer <Messages :: Message <Items :: Character> > > ::
	Reference (const Reference <Play_State_GUI> & other);
template Reference <Container <Model> > ::
	Reference (const Reference <Tile> & other);
template Reference <Strategies :: Strategy <Items :: Character> > ::
	Reference (const Reference <Strategies :: Fight_State> & other);
template Reference <Strategies :: Strategy <Engines :: Game> > ::
	Reference (const Reference <Strategies :: Play_State> & other);
template Reference <Strategies :: Strategy <Engines :: Game> > ::
	Reference (const Reference <Strategies :: Menu_State> & other);

template bool Reference <Items :: Character> ::
	is_castable <Items :: NPC> () const;
template bool Reference <Items :: Item> ::
	is_castable <Items :: Character> () const;
template bool Reference <Items :: Item> ::
	is_castable <Items :: NPC> () const;
template bool Reference <Messages :: Message <Items :: Character> > ::
	is_castable <Messages :: Battle_Message> () const;
template bool Reference <Messages :: Message <Items :: Character> > ::
	is_castable <Messages :: Conversation_Message> () const;
//	template bool Reference <Object> ::
//		is_castable <Set <Messages :: Conversation_Message> > () const;
template bool Reference <Strategies :: Strategy <Items :: Character> > ::
	is_castable <Strategies :: Alive_State> () const;
template bool Reference <Strategies :: Strategy <Items :: Character> > ::
	is_castable <Strategies :: Chat_State> () const;
template bool Reference <Strategies :: Strategy <Items :: Character> > ::
	is_castable <Strategies :: Fight_State> () const;

template Reference <Items :: NPC> Reference <Items :: Character> ::
	cast <Items :: NPC> ();
template Reference <Items :: Character> Reference <Items :: Item> ::
	cast <Items :: Character> ();
//	template Reference <Items :: Container_Item <Items :: Item> > Reference <Object> ::
//		cast <Items :: Container_Item <Items :: Item> > ();
template Reference <Items :: NPC> Reference <Items :: Item> ::
	cast <Items :: NPC> ();
template Reference <Movable_Model> Reference <Model> ::
	cast <Movable_Model> ();
//	template Reference <Set <Messages :: Conversation_Message> >
//		Reference <Messages :: Message <Items :: Character> > ::
//		cast <Set <Messages :: Conversation_Message> > ();
template Reference <Strategies :: Alive_State>
	Reference <Strategies :: Strategy <Items :: Character> > ::
	cast <Strategies :: Alive_State> ();

template const Reference <Messages :: Battle_Message>
	Reference <Messages :: Message <Items :: Character> > ::
	cast_const <Messages :: Battle_Message> () const;
template const Reference <Messages :: Conversation_Message>
	Reference <Messages :: Message <Items :: Character> > ::
	cast_const <Messages :: Conversation_Message> () const;
