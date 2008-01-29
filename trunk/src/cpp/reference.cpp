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
	get_name (bool suppress_debug_info)
	const
{
	#ifdef RADAKAN_DEBUG
		suppress_debug_info = true;
	#endif

	if (suppress_debug_info)
	{
		if (pointee == NULL)
		{
			return "NULL";
		}
		else
		{
			return pointee -> name;
		}
	}
	else
	{
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
	}
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

	return Reference <U> (boost :: polymorphic_downcast <U *, T> (pointee));
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
#include "container.hpp"
#include "engines/audio_engine.hpp"
#include "engines/battle_engine.hpp"
#include "engines/game.hpp"
#include "engines/gui_engine.hpp"
#include "engines/input/command_data.hpp"
#include "engines/input/command_reader.hpp"
#include "engines/input/mouse_data.hpp"
#include "engines/input/mouse_reader.hpp"
#include "engines/input/registrator.hpp"
#include "engines/log.hpp"
#include "engines/mediator.hpp"
#include "engines/mediator_implementation.hpp"
#include "engines/render_engine.hpp"
#include "engines/settings.hpp"
#include "engines/tracker.hpp"
#include "gui.hpp"
#include "items/character.hpp"
#include "items/static_item.hpp"
#include "items/weapon.hpp"
#include "map.hpp"
#include "mesh_data.hpp"
#include "messages/button_event.hpp"
#include "messages/communications/converse.hpp"
#include "messages/communications/fight.hpp"
#include "messages/list_event.hpp"
#include "messages/list_update.hpp"
#include "messages/nothing.hpp"
#include "model.hpp"
#include "movable_model.hpp"
#include "opinion.hpp"
#include "skill.hpp"
#include "slot.hpp"
#include "strategies/actions/conversing.hpp"
#include "strategies/actions/fighting.hpp"
#include "strategies/behaviors/ai.hpp"
#include "strategies/behaviors/player.hpp"
#include "strategies/game_modes/menu.hpp"
#include "strategies/game_modes/play.hpp"
#include "tile.hpp"
#include "world.hpp"

template class Reference <Container <GUI> >;
template class Reference <Container <Items :: Character> >;
template class Reference
	<Container <Items :: Container_Item <Items :: Container_Item <Items :: Item> > > >;
template class Reference <Container <Items :: Container_Item <Items :: Item> > >;
template class Reference <Container <Items :: Item> >;
template class Reference <Container <Messages :: Communications :: Communication> >;
template class Reference <Container <Model> >;
template class Reference <Container <Object> >;
template class Reference <Container <Observer <Messages :: Button_Event> > >;
template class Reference
	<Container <Observer <Messages :: Communications :: Communication> > >;
template class Reference <Container <Observer <Messages :: List_Event> > >;
template class Reference <Container <Observer <Messages :: List_Update> > >;
template class Reference <Container <Observer <Messages :: Nothing> > >;
template class Reference <Container <Opinion> >;
template class Reference
	<Container <Pair <string, Messages :: Communications :: Communication> > >;
template class Reference <Container <Pair <pair <int, int>, Tile> > >;
template class Reference <Container <Pair <string, Skill> > >;
template class Reference <Container <Skill> >;
#if RADAKAN_AUDIO_MODE == RADAKAN_AUDIERE_MODE
	template class Reference <Container <Sound_Sample> >;
#endif	//	RADAKAN_AUDIO_ENGINE_HPP
template class Reference <Container <Strategies :: Actions :: Action> >;
template class Reference <Container <Strategies :: Behaviors :: Behavior> >;
template class Reference <Container <Strategies :: Game_Modes :: Game_Mode> >;
template class Reference <Container <Tile> >;
#if RADAKAN_AUDIO_MODE == RADAKAN_AUDIERE_MODE
	template class Reference <Engines :: Audio_Engine>;
#endif	//	RADAKAN_AUDIO_ENGINE_HPP
//	template class Reference <Engines :: Battle_Engine>;
template class Reference <Engines :: Game>;
template class Reference <Engines :: GUI_Engine>;
template class Reference <Engines :: Input :: Command_Data>;
template class Reference <Engines :: Input :: Command_Reader>;
template class Reference <Engines :: Input :: Mouse_Data>;
template class Reference <Engines :: Input :: Mouse_Reader>;
template class Reference <Engines :: Input :: Registrator>;
template class Reference <Engines :: Log>;
template class Reference <Engines :: Mediator>;
template class Reference <Engines :: Mediator_Implementation>;
template class Reference <Engines :: Render_Engine>;
template class Reference <Engines :: Settings>;
#ifdef RADAKAN_DEBUG
	template class Reference <Engines :: Tracker>;
#endif
template class Reference <GUI>;
template class Reference <Items :: Character>;
template class Reference
	<Items :: Container_Item <Items :: Container_Item <Items :: Item> > >;
template class Reference <Items :: Container_Item <Items :: Item> >;
template class Reference <Items :: Item>;
template class Reference <Items :: Weapon>;
template class Reference <Location <Items :: Item> >;
template class Reference
	<Location <Items :: Container_Item <Items :: Container_Item <Items :: Item> > > >;
template class Reference <Location <Items :: Container_Item <Items :: Item> > >;
template class Reference <Location <Model> >;
template class Reference <Location <Strategies :: Actions :: Action> >;
template class Reference <Location <Strategies :: Behaviors :: Behavior> >;
template class Reference <Location <Strategies :: Game_Modes :: Game_Mode> >;
template class Reference <Map <string, Messages :: Communications :: Communication> >;
template class Reference <Map <pair <int, int>, Tile> >;
template class Reference <Map <string, Skill> >;
template class Reference <Mesh_Data>;
template class Reference <Messages :: Button_Event>;
template class Reference <Messages :: Communications :: Communication>;
template class Reference <Messages :: Communications :: Converse>;
template class Reference <Messages :: Communications :: Fight>;
template class Reference <Messages :: List_Event>;
template class Reference <Messages :: List_Update>;
template class Reference <Messages :: Nothing>;
template class Reference <Model>;
template class Reference <Movable_Model>;
template class Reference <Object>;
template class Reference <Observer <Messages :: Button_Event> >;
template class Reference <Observer <Messages :: Communications :: Communication> >;
template class Reference <Observer <Messages :: List_Event> >;
template class Reference <Observer <Messages :: List_Update> >;
template class Reference <Observer <Messages :: Nothing> >;
template class Reference <Opinion>;
template class Reference <Pair <string, Messages :: Communications :: Communication> >;
template class Reference <Pair <pair <int, int>, Tile> >;
template class Reference <Pair <string, Skill> >;
template class Reference <Set <GUI> >;
template class Reference <Set <Items :: Character> >;
template class Reference <Set <Items :: Item> >;
template class Reference <Set <Messages :: Communications :: Communication> >;
template class Reference <Set <Model> >;
template class Reference <Set <Object> >;
template class Reference <Set <Observer <Messages :: Communications :: Communication> > >;
template class Reference <Set <Opinion> >;
#if RADAKAN_AUDIO_MODE == RADAKAN_AUDIERE_MODE
	template class Reference <Set <Sound_Sample> >;
#endif	//	RADAKAN_AUDIO_ENGINE_HPP
template class Reference <Set <Strategies :: Game_Modes :: Game_Mode> >;
//	template class Reference <Set <Strategies :: Strategy <Items :: Character> > >;
template class Reference <Set <Tile> >;
template class Reference <Skill>;
#if RADAKAN_AUDIO_MODE == RADAKAN_AUDIERE_MODE
	template class Reference <Slot <Strategies :: Game_Modes :: Game_Mode> >;
#endif	//	RADAKAN_AUDIO_ENGINE_HPP
//	template class Reference <Slot <Strategies :: Strategy <Items :: Character> > >;
template class Reference <Slot <Tile> >;
#if RADAKAN_AUDIO_MODE == RADAKAN_AUDIERE_MODE
	template class Reference <Sound_Sample>;
#endif	//	RADAKAN_AUDIO_ENGINE_HPP
template class Reference <State_Machine <Tile> >;
template class Reference <Strategies :: Game_Modes :: Game_Mode>;
//	template class Reference <Strategies :: Strategy <Items :: Character> >;
template class Reference <Strategies :: Actions :: Action>;
template class Reference <Strategies :: Actions :: Fighting>;
template class Reference <Strategies :: Behaviors :: Behavior>;
template class Reference <Strategies :: Behaviors :: AI>;
template class Reference <Strategies :: Behaviors :: Player>;
template class Reference <Strategies :: Game_Modes :: Menu>;
template class Reference <Strategies :: Game_Modes :: Play>;
//	template class Reference <Strategies :: Strategy_State_Machine <Engines :: Game> >;
//	template class Reference <Strategies :: Strategy_State_Machine <Items :: Character> >;
template class Reference <Tile>;
template class Reference <World>;

template <class FROM, class TO> void convert (const FROM & from)
{
	return (void) TO (from);
}

/*template void convert
	<
		Reference <Items :: Character>,
		Reference <Observer <Messages :: Communications :: Communication> >
	>
	(const Reference <Items :: Character> & from);*/
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
		Reference <Items :: Character>,
		Reference <Items :: Item>
	>
	(const Reference <Items :: Character> & from);
template void convert
	<
		Reference <Messages :: Communications :: Converse>,
		Reference <Messages :: Communications :: Communication>
	>
	(const Reference <Messages :: Communications :: Converse> & from);
template void convert
	<
		Reference <Movable_Model>,
		Reference <Model>
	>
	(const Reference <Movable_Model> & from);
template void convert
	<
		Reference <Strategies :: Game_Modes :: Menu>,
		Reference <Strategies :: Game_Modes :: Game_Mode>
	>
	(const Reference <Strategies :: Game_Modes :: Menu> & from);
template void convert
	<
		Reference <Strategies :: Game_Modes :: Play>,
		Reference <Strategies :: Game_Modes :: Game_Mode>
	>
	(const Reference <Strategies :: Game_Modes :: Play> & from);
template void convert
	<
		Reference <Tile>,
		Reference <Container <Model> >
	>
	(const Reference <Tile> & from);

template bool Reference <Items :: Item> ::
	is_castable <Items :: Character> () const;
template bool Reference <Items :: Item> ::
	is_castable <Items :: Static_Item> () const;

template Reference <Items :: Character> Reference <Items :: Item> ::
	cast <Items :: Character> ();
template Reference <Movable_Model> Reference <Model> ::
	cast <Movable_Model> ();
template Reference <Strategies :: Behaviors :: AI> Reference <Strategies :: Behaviors :: Behavior> ::
	cast <Strategies :: Behaviors :: AI> ();

template const Reference <Messages :: Communications :: Fight>
	Reference <Messages :: Communications :: Communication> ::
	cast_const <Messages :: Communications :: Fight> () const;
template const Reference <Messages :: Communications :: Converse>
	Reference <Messages :: Communications :: Communication> ::
	cast_const <Messages :: Communications :: Converse> () const;
