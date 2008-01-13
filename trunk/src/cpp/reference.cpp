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

template <class T> bool Reference <T> ::
	operator== (const Reference <T> & other)
	const
{
	return pointee == other . pointee;
}

template <class T> template <class U> bool Reference <T> ::
	operator== (const Reference <U> & other)
	const
{
	return pointee == other . pointee;
}

template <class T> bool Reference <T> ::
	operator!= (const Reference <T> & other) const
{
	return pointee != other . pointee;
}

template <class T> bool Reference <T> ::
	operator< (const Reference <T> & other) const
{
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

	//	Engines :: Log :: trace (* this, get_class_name (), "", get_name (), "A", "(end)");
}

template <class T> Reference <T> ::
	Reference (const Reference <T> & other) :
	Reference_Base (),	//	GCC (using '-Wall -Wextra') gives a warning without this line.
	pointee (other . pointee),
	parent (NULL)
{
	//	Engines :: Log :: trace (* this, get_class_name (), "", other . get_name (), "B");

	if (pointee != NULL)
	{
		pointee -> register_reference (* this);
	}

	//	Engines :: Log :: trace (* this, get_class_name (), "", other . get_name (), "B", "(end)");
}

template <class T> template <class U> Reference <T> ::
	Reference (const Reference <U> & other) :
	pointee (other . pointee),
	parent (NULL)
{
	//	Engines :: Log :: trace (* this, get_class_name (), "", other . get_name (), "C");

	if (pointee != NULL)
	{
		pointee -> register_reference (* this);
	}

	//	Engines :: Log :: trace (* this, get_class_name (), "", other . get_name (), "C", "(end)");
}

//	virtual
template <class T> Reference <T> ::
	~Reference ()
{
	//	Engines :: Log :: trace (* this, get_class_name (), "~");

	if (pointee != NULL)
	{
		pointee -> unregister_reference (* this);
	}
}

//	virtual
template <class T> void Reference <T> ::
	destruct ()
	const
{
	assert (pointee != NULL);

	if (parent == NULL)
	{
		pointee -> unregister_reference (* this);

		const_cast <Reference <T> &> (* this) . pointee = NULL;
	}
	else
	{
		parent -> drop (* this);	//	Calls my destructor.
	}
}

//	virtual
template <class T> string Reference <T> ::
	get_name ()
	const
{
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
				result += " [P: " + parent -> name + "]";
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
	assert (pointee != NULL);

	return pointee;
}

template <class T> const T * Reference <T> ::
	operator-> ()
	const
{
	assert (pointee != NULL);

	return pointee;
}

template <class T> bool Reference <T> ::
	points_to_object ()
	const
{
	return pointee != NULL;
}

template <class T> void Reference <T> ::
	set_parent (Container <T> & new_parent)
{
	parent = & new_parent;
}

template <class T> template <class U> bool Reference <T> ::
	is_castable ()
	const
{
//	Engines :: Log :: trace (* this, Reference <T> :: get_class_name (), "is_castable", "<" + U :: get_class_name () + ">");

	return (dynamic_cast <const U *> (pointee) != NULL);
}

template <class T> template <class U> Reference <U> Reference <T> ::
	cast ()
{
//	Engines :: Log :: trace (* this, Reference <T> :: get_class_name (), "cast", "<" + U :: get_class_name () + ">");
	assert (is_castable <U> ());

	return Reference <U> (dynamic_cast <U *> (pointee));
}

template <class T> template <class U> const Reference <U> Reference <T> ::
	cast_const ()
	const
{
//	Engines :: Log :: trace (* this, Reference <T> :: get_class_name (), "cast_const", "<" + U :: get_class_name () + ">");
	assert (is_castable <U> ());

	return (const_cast <Reference <T> *> (this)) -> cast <U> ();
}

//	to avert linking errors:
#if RADAKAN_GUI_MODE == RADAKAN_CEGUI_MODE
	#include <elements/CEGUIListboxItem.h>
#endif
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
#include "items/static_item.hpp"
#include "items/weapon.hpp"
#include "map.hpp"
#include "mesh_data.hpp"
#include "messages/battle_message.hpp"
#include "messages/conversation_message.hpp"
#include "model.hpp"
#include "movable_model.hpp"
#include "opinion.hpp"
#include "play_state_gui.hpp"
#include "skill.hpp"
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
#if RADAKAN_GUI_MODE == RADAKAN_CEGUI_MODE
	template class Reference <Container <Pair <CEGUI :: ListboxItem *, Messages :: Message <Items :: Character> > > >;
#endif
template class Reference <Container <Pair <pair <int, int>, Tile> > >;
template class Reference <Container <Pair <string, Skill> > >;
template class Reference <Container <Play_State_GUI> >;
template class Reference <Container <Skill> >;
#if RADAKAN_AUDIO_MODE == RADAKAN_AUDIERE_MODE
	template class Reference <Container <Sound_Sample> >;
#endif
template class Reference <Container <Strategies :: Strategy <Engines :: Game> > >;
template class Reference <Container <Strategies :: Strategy <Items :: Character> > >;
template class Reference <Container <Tile> >;
#if RADAKAN_AUDIO_MODE == RADAKAN_AUDIERE_MODE
	template class Reference <Engines :: Audio_Engine>;
#endif
//	template class Reference <Engines :: Battle_Engine>;
template class Reference <Engines :: Conversation_Engine>;
template class Reference <Engines :: Game>;
template class Reference <Engines :: GUI_Engine>;
template class Reference <Engines :: Input_Engine>;
template class Reference <Engines :: Log>;
template class Reference <Engines :: Settings>;

#ifdef RADAKAN_DEBUG
	template class Reference <Engines :: Tracker>;
#endif

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
#if RADAKAN_GUI_MODE == RADAKAN_CEGUI_MODE
	template class Reference <Map <CEGUI :: ListboxItem *, Messages :: Message <Items :: Character> > >;
#endif
template class Reference <Map <pair <int, int>, Tile> >;
template class Reference <Map <string, Skill> >;
template class Reference <Mesh_Data>;
template class Reference <Messages :: Battle_Message>;
template class Reference <Messages :: Conversation_Message>;
template class Reference <Messages :: Message <Engines :: Game> >;
template class Reference <Messages :: Message <Items :: Character> >;
template class Reference <Model>;
template class Reference <Movable_Model>;
template class Reference <Object>;
template class Reference <Observer <Messages :: Message <Items :: Character> > >;
template class Reference <Observer <Object> >;
template class Reference <Opinion>;
#if RADAKAN_GUI_MODE == RADAKAN_CEGUI_MODE
	template class Reference <Pair <CEGUI :: ListboxItem *, Messages :: Message <Items :: Character> > >;
#endif
template class Reference <Pair <pair <int, int>, Tile> >;
template class Reference <Pair <string, Skill> >;
template class Reference <Play_State_GUI>;
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
#if RADAKAN_AUDIO_MODE == RADAKAN_AUDIERE_MODE
	template class Reference <Set <Sound_Sample> >;
#endif
template class Reference <Set <Strategies :: Strategy <Engines :: Game> > >;
template class Reference <Set <Strategies :: Strategy <Items :: Character> > >;
template class Reference <Set <Tile> >;
template class Reference <Skill>;
template class Reference <Slot <Strategies :: Strategy <Engines :: Game> > >;
template class Reference <Slot <Strategies :: Strategy <Items :: Character> > >;
template class Reference <Slot <Tile> >;
#if RADAKAN_AUDIO_MODE == RADAKAN_AUDIERE_MODE
	template class Reference <Sound_Sample>;
#endif
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

template bool Reference <Items :: Character> ::
	operator== (const Reference <Items :: NPC> & other) const;
template bool Reference <Items :: Character> ::
	operator== (const Reference <Items :: Player_Character> & other) const;

template <class FROM, class TO> void convert (const FROM & from)
{
	return (void) TO (from);
}

template void convert
	<
		Reference <Engines :: Input_Engine>,
		Reference <Observer <Messages :: Message <Items :: Character> > >
	>
	(const Reference <Engines :: Input_Engine> & from);
template void convert
	<
		Reference <Engines :: Input_Engine>,
		Reference <Observer <Object> >
	>
	(const Reference <Engines :: Input_Engine> & from);
template void convert
	<
		Reference <Items :: Character>,
		Reference <Observer <Messages :: Message <Items :: Character> > >
	>
	(const Reference <Items :: Character> & from);
template void convert
	<
		Reference <Items :: Container_Item <Items :: Container_Item <Items :: Item> > >,
		Reference <Items :: Item>
	>
	(const Reference <Items :: Container_Item <Items :: Container_Item <Items :: Item> > > & from);
template void convert
	<
		Reference <Items :: Container_Item <Items :: Item> >,
		Reference <Container <Items :: Item> >
	>
	(const Reference <Items :: Container_Item <Items :: Item> > & from);
template void convert
	<
		Reference <Items :: Container_Item <Items :: Item> >,
		Reference <Items :: Item>
	>
	(const Reference <Items :: Container_Item <Items :: Item> > & from);
template void convert
	<
		Reference <Items :: NPC>,
		Reference <Items :: Character>
	>
	(const Reference <Items :: NPC> & from);
template void convert
	<
		Reference <Items :: Player_Character>,
		Reference <Items :: Character>
	>
	(const Reference <Items :: Player_Character> & from);
template void convert
	<
		Reference <Messages :: Conversation_Message>,
		Reference <Messages :: Message <Items :: Character> >
	>
	(const Reference <Messages :: Conversation_Message> & from);
template void convert
	<
		Reference <Movable_Model>,
		Reference <Model>
	>
	(const Reference <Movable_Model> & from);
template void convert
	<
		Reference <Play_State_GUI>,
		Reference <GUI>
	>
	(const Reference <Play_State_GUI> & from);
template void convert
	<
		Reference <Play_State_GUI>,
		Reference <Observer <Messages :: Message <Items :: Character> > >
	>
	(const Reference <Play_State_GUI> & from);
template void convert
	<
		Reference <Strategies :: Fight_State>,
		Reference <Strategies :: Strategy <Items :: Character> >
	>
	(const Reference <Strategies :: Fight_State> & from);
template void convert
	<
		Reference <Strategies :: Menu_State>,
		Reference <Strategies :: Strategy <Engines :: Game> >
	>
	(const Reference <Strategies :: Menu_State> & from);
template void convert
	<
		Reference <Strategies :: Play_State>,
		Reference <Strategies :: Strategy <Engines :: Game> >
	>
	(const Reference <Strategies :: Play_State> & from);
template void convert
	<
		Reference <Tile>,
		Reference <Container <Model> >
	>
	(const Reference <Tile> & from);

template bool Reference <Items :: Character> ::
	is_castable <Items :: NPC> () const;
template bool Reference <Items :: Item> ::
	is_castable <Items :: Character> () const;
template bool Reference <Items :: Item> ::
	is_castable <Items :: NPC> () const;
template bool Reference <Items :: Item> ::
	is_castable <Items :: Static_Item> () const;
template bool Reference <Messages :: Message <Items :: Character> > ::
	is_castable <Messages :: Battle_Message> () const;
template bool Reference <Messages :: Message <Items :: Character> > ::
	is_castable <Messages :: Conversation_Message> () const;
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
template Reference <Items :: NPC> Reference <Items :: Item> ::
	cast <Items :: NPC> ();
template Reference <Movable_Model> Reference <Model> ::
	cast <Movable_Model> ();
template Reference <Strategies :: Alive_State>
	Reference <Strategies :: Strategy <Items :: Character> > ::
	cast <Strategies :: Alive_State> ();

template const Reference <Messages :: Battle_Message>
	Reference <Messages :: Message <Items :: Character> > ::
	cast_const <Messages :: Battle_Message> () const;
template const Reference <Messages :: Conversation_Message>
	Reference <Messages :: Message <Items :: Character> > ::
	cast_const <Messages :: Conversation_Message> () const;
