#include "destructible_reference.hpp"
#include "log.hpp"

using namespace std;
using namespace Radakan;

//	static
template <class T> const string Destructible_Reference <T> ::
	get_class_name ()
{
	//	Do not turn this into a static data member,
	//	that'd cause problems for template (sub)classes.

	return "Destructible_Reference <" + T :: get_class_name () + ">";
}

template <class T> Destructible_Reference <T> ::
	Destructible_Reference (T * new_pointee) :
	Reference <T> (new_pointee)
{
	Engines :: Log :: trace (* this, get_class_name (), "", Reference <T> :: get_name (), "A");
}

template <class T> Destructible_Reference <T> ::
	Destructible_Reference (const Destructible_Reference <T> & other) :
	Reference <T> (other)
{
	Engines :: Log :: trace (* this, get_class_name (), "", Reference <T> :: get_name (), "B");
}

template <class T> Destructible_Reference <T> ::
	Destructible_Reference (const Reference <T> & other) :
	Reference <T> (other)
{
	Engines :: Log :: trace (* this, get_class_name (), "", Reference <T> :: get_name (), "C");
}

template <class T> template <class V> Destructible_Reference <T> ::
	Destructible_Reference (const Destructible_Reference <V> & other) :
	Reference <T> (other)
{
	Engines :: Log :: trace (* this, get_class_name (), "", Reference <T> :: get_name (), "D");
}

template <class T> template <class V> Destructible_Reference <T> ::
	Destructible_Reference (const Reference <V> & other) :
	Reference <T> (other)
{
	Engines :: Log :: trace (* this, get_class_name (), "", Reference <T> :: get_name (), "E");
}

template <class T> Destructible_Reference <T> ::
	~Destructible_Reference ()
{
	Engines :: Log :: trace (* this, get_class_name (), "~", Reference <T> :: get_name ());
	
	if (has_parent ())
	{
		parent -> drop (* this);
	}
}

template <class T> bool Destructible_Reference <T> ::
	has_parent () const
{
	return parent . points_to_object ();
}

template <class T> void Destructible_Reference <T> ::
	set_parent (Reference <Set <T> > new_parent)
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
#include "thought.hpp"
#include "world.hpp"
#include "weapon.hpp"

//	template class Destructible_Reference <Engines :: Audio_Engine>;
//	template class Destructible_Reference <Engines :: Game>;
//	template class Destructible_Reference <Engines :: GUI_Engine>;
//	template class Destructible_Reference <Engines :: Input_Engine>;
//	template class Destructible_Reference <Engines :: Log>;
//	template class Destructible_Reference <Engines :: Settings>;
//	template class Destructible_Reference <Engines :: Tracker>;
template class Destructible_Reference <GUI>;
template class Destructible_Reference <Items :: Character>;
//	template class Destructible_Reference <Items :: Container>;
template class Destructible_Reference <Items :: Item>;
//	template class Destructible_Reference <Items :: Multislot <Items :: Item> >;
template class Destructible_Reference <Items :: NPC>;
//	template class Destructible_Reference <Items :: Player_Character>;
//	template class Destructible_Reference <Items :: Weapon>;
//	template class Destructible_Reference <Location <GUI> >;
//	template class Destructible_Reference <Location <Tile> >;
//	template class Destructible_Reference <Messages :: Battle_Message>;
template class Destructible_Reference <Messages :: Conversation_Message>;
template class Destructible_Reference <Model>;
template class Destructible_Reference <Movable_Model>;
//	template class Destructible_Reference <Object>;
template class Destructible_Reference <Observer <Engines :: Log> >;
template class Destructible_Reference <Observer <GUI> >;
template class Destructible_Reference <Observer <Items :: Character> >;
template class Destructible_Reference <Observer <Strategies :: Play_State> >;
//	template class Destructible_Reference <Set <Messages :: Conversation_Message> >;
//	template class Destructible_Reference <Set <GUI> >;
//	template class Destructible_Reference <Set <Items :: Item> >;
//	template class Destructible_Reference <Set <Object> >;
//	template class Destructible_Reference <Set <Observer <GUI> > >;
//	template class Destructible_Reference <Set <Tile> >;
//	template class Destructible_Reference <Set <Sound_Sample> >;
template class Destructible_Reference <Sound_Sample>;
//	template class Destructible_Reference <State_Machine <Tile> >;
template class Destructible_Reference <Strategies :: Strategy>;
//	template class Destructible_Reference <Strategies :: Alive_State>;
//	template class Destructible_Reference <Strategies :: Fight_State>;
//	template class Destructible_Reference <Strategies :: Menu_State>;
//	template class Destructible_Reference <Strategies :: Play_State>;
//	template class Destructible_Reference <Strategies :: Strategy_State_Machine>;
template class Destructible_Reference <Tile>;
template class Destructible_Reference <Thought>;

//	template class Destructible_Reference <const Engines :: Audio_Engine>;
//	template class Destructible_Reference <const Engines :: Game>;
//	template class Destructible_Reference <const Engines :: GUI_Engine>;
//	template class Destructible_Reference <const Engines :: Input_Engine>;
//	template class Destructible_Reference <const Engines :: Log>;
//	template class Destructible_Reference <const Engines :: Settings>;
//	template class Destructible_Reference <const Engines :: Tracker>;
//	template class Destructible_Reference <const GUI>;
//	template class Destructible_Reference <const Items :: Character>;
//	template class Destructible_Reference <const Items :: Container>;
//	template class Destructible_Reference <const Items :: Item>;
//	template class Destructible_Reference <const Items :: Multislot <Items :: Item> >;
//	template class Destructible_Reference <const Items :: NPC>;
//	template class Destructible_Reference <const Items :: Player_Character>;
//	template class Destructible_Reference <const Items :: Weapon>;
//	template class Destructible_Reference <const Location <GUI> >;
//	template class Destructible_Reference <const Location <Tile> >;
//	template class Destructible_Reference <const Messages :: Battle_Message>;
//	template class Destructible_Reference <const Messages :: Conversation_Message>;
//	template class Destructible_Reference <const Model>;
//	template class Destructible_Reference <const Movable_Model>;
template class Destructible_Reference <const Object>;
//	template class Destructible_Reference <const Observer <Strategies :: Play_State> >;
//	template class Destructible_Reference <const Observer <Engines :: Log> >;
//	template class Destructible_Reference <const Observer <GUI> >;
//	template class Destructible_Reference <const Observer <Items :: Character> >;
//	template class Destructible_Reference <const Set <Messages :: Conversation_Message> >;
//	template class Destructible_Reference <const Set <GUI> >;
//	template class Destructible_Reference <const Set <Items :: Item> >;
//	template class Destructible_Reference <const Set <Object> >;
//	template class Destructible_Reference <const Set <Observer <GUI> > >;
//	template class Destructible_Reference <const Set <Tile> >;
//	template class Destructible_Reference <const Set <Sound_Sample> >;
//	template class Destructible_Reference <const Sound_Sample>;
//	template class Destructible_Reference <const State_Machine <Tile> >;
//	template class Destructible_Reference <const Strategies :: Strategy>;
//	template class Destructible_Reference <const Strategies :: Alive_State>;
//	template class Destructible_Reference <const Strategies :: Fight_State>;
//	template class Destructible_Reference <const Strategies :: Menu_State>;
//	template class Destructible_Reference <const Strategies :: Play_State>;
//	template class Destructible_Reference <const Strategies :: Strategy_State_Machine>;
//	template class Destructible_Reference <const Tile>;
//	template class Destructible_Reference <const Thought>;
