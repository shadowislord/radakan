//	#include "log.hpp"
#include "pointer.hpp"

using namespace std;
using namespace Radakan;

//	static
template <class T> string Pointer <T> ::
	get_class_name ()
{
	//	Do not turn this into a static data member,
	//	that'd cause problems for template (sub)classes.

	return "Pointer <" + T :: get_class_name () + ">";
}

template <class T> Pointer <T> & Pointer <T> ::
	operator= (const Reference <T> & other)
{
	reset_pointee (other . Reference <T> :: pointee);

	return * this;
}

template <class T> template <class U> Pointer <T> & Pointer <T> ::
	operator= (const Reference <U> & other)
{
	reset_pointee (other . pointee);

	return * this;
}

template <class T> Pointer <T> ::
	Pointer (T * new_pointee) :
	Reference <T> (new_pointee)
{

}

template <class T> Pointer <T> ::
	Pointer (const Pointer <T> & other) :
	Reference <T> (other . pointee)
{

}

template <class T> Pointer <T> ::
	Pointer (const Reference <T> & other) :
	Reference <T> (other . pointee)
{

}

template <class T> template <class U> Pointer <T> ::
	Pointer (const Reference <U> & other) :
	Reference <T> (other)
{

}

//	virtual
template <class T> Pointer <T> ::
	~Pointer ()
{

}

template <class T> void Pointer <T> ::
	reset_pointee (T * new_pointee)
{
	if (Reference <T> :: pointee != NULL)
	{
		Reference <T> :: pointee -> unregister_reference (* this);
	}

	Reference <T> :: pointee = new_pointee;

	if (Reference <T> :: pointee != NULL)
	{
		Reference <T> :: pointee -> register_reference (* this);
	}
}

//	to avert linking errors:
#if RADAKAN_GUI_MODE == RADAKAN_CEGUI_MODE
	#include <elements/CEGUIListboxItem.h>
#endif
#include "container.hpp"
#include "engines/audio_engine.hpp"
#include "engines/game.hpp"
#include "gui.hpp"
#include "items/character.hpp"
#include "messages/message.hpp"
#include "movable_model.hpp"
#include "opinion.hpp"
#include "pair.hpp"
#include "play_gui.hpp"
#include "skill.hpp"
#include "strategies/actions/action.hpp"
#include "strategies/behaviors/behavior.hpp"
#include "strategies/game_modes/game_mode.hpp"
#include "tile.hpp"

#if RADAKAN_AUDIO_MODE == RADAKAN_AUDIERE_MODE
	template class Pointer <Sound_Sample>;
#endif
template class Pointer <Container <GUI> >;
template class Pointer <Container <Items :: Character> >;
template class Pointer <Container <Items :: Container_Item <Items :: Container_Item <Items :: Item> > > >;
template class Pointer <Container <Items :: Container_Item <Items :: Item> > >;
template class Pointer <Container <Items :: Item> >;
template class Pointer <Container <Model> >;
template class Pointer <Container <Strategies :: Actions :: Action> >;
template class Pointer <Container <Strategies :: Behaviors :: Behavior> >;
template class Pointer <Container <Strategies :: Game_Modes :: Game_Mode> >;
template class Pointer <Container <Tile> >;
template class Pointer <GUI>;
template class Pointer <Items :: Character>;
template class Pointer <Items :: Container_Item <Items :: Container_Item <Items :: Item> > >;
template class Pointer <Items :: Container_Item <Items :: Item> >;
template class Pointer <Items :: Item>;
template class Pointer <Location <GUI> >;
template class Pointer <Location <Items :: Item> >;
template class Pointer <Location <Items :: Container_Item <Items :: Container_Item <Items :: Item> > > >;
template class Pointer <Location <Items :: Container_Item <Items :: Item> > >;
template class Pointer <Location <Model> >;
template class Pointer <Location <Strategies :: Actions :: Action> >;
template class Pointer <Location <Strategies :: Behaviors :: Behavior> >;
template class Pointer <Location <Strategies :: Game_Modes :: Game_Mode> >;
template class Pointer <Messages :: Message <Items :: Character> >;
template class Pointer <Model>;
template class Pointer <Movable_Model>;
template class Pointer <Object>;
template class Pointer <Observer <Messages :: Message <Items :: Character> > >;
template class Pointer <Observer <Messages :: Message <Object> > >;
template class Pointer <Observer <Object> >;
template class Pointer <Opinion>;
#if RADAKAN_GUI_MODE == RADAKAN_CEGUI_MODE
	template class Pointer <Pair <CEGUI :: ListboxItem *, Messages :: Message <Items :: Character> > >;
#endif
template class Pointer <Pair <pair <int, int>, Tile> >;
template class Pointer <Pair <string, Skill> >;
template class Pointer <Skill>;
template class Pointer <Strategies :: Actions :: Action>;
template class Pointer <Strategies :: Behaviors :: Behavior>;
template class Pointer <Strategies :: Game_Modes :: Game_Mode>;
template class Pointer <Tile>;

template Pointer <Items :: Item> & Pointer <Items :: Item> ::
	operator= (const Reference <Items :: Character> & other);
template Pointer <GUI> & Pointer <GUI> ::
	operator= (const Reference <Play_GUI> & other);
