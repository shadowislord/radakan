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
	reset_pointee (T * new_pointee, bool weak)
{
	if (Reference <T> :: pointee != NULL)
	{
		Reference <T> :: pointee -> unregister_reference (* this);
	}

	Reference <T> :: pointee = new_pointee;

	if (Reference <T> :: pointee != NULL)
	{
		Reference <T> :: pointee -> register_reference (* this, weak);
	}
}

template <class T> void Pointer <T> ::
	reset_pointee (const Reference <T> & other, bool weak)
{
	reset_pointee (other . pointee, weak);
}

//	to avert linking errors:
#include "container.hpp"
#if RADAKAN_AUDIO_MODE == RADAKAN_AUDIERE_MODE
	#include "engines/audio_engine.hpp"
#endif
#include "body.hpp"
#include "gui.hpp"
#include "items/character.hpp"
#include "messages/button_event.hpp"
#include "messages/communications/communication.hpp"
#include "messages/list_event.hpp"
#include "messages/list_update.hpp"
#include "messages/nothing.hpp"
#include "model.hpp"
#include "opinion.hpp"
#include "pair.hpp"
#include "skill.hpp"
#include "strategies/actions/action.hpp"
#include "strategies/behaviors/behavior.hpp"
#include "strategies/game_modes/game_mode.hpp"
#include "tile.hpp"

template class Pointer <Body>;
template class Pointer <GUI>;
template class Pointer <Items :: Character>;
template class Pointer <Items :: Container_Item <Items :: Container_Item <Items :: Item> > >;
template class Pointer <Items :: Container_Item <Items :: Item> >;
template class Pointer <Items :: Item>;
template class Pointer <Location <Items :: Item> >;
template class Pointer <Location <Items :: Container_Item <Items :: Container_Item <Items :: Item> > > >;
template class Pointer <Location <Items :: Container_Item <Items :: Item> > >;
template class Pointer <Messages :: Communications :: Communication>;
template class Pointer <Model>;
template class Pointer <Object>;
template class Pointer <Observer <Messages :: Button_Event> >;
template class Pointer <Observer <Messages :: Communications :: Communication> >;
template class Pointer <Observer <Messages :: List_Event> >;
template class Pointer <Observer <Messages :: List_Update> >;
template class Pointer <Observer <Messages :: Nothing> >;
template class Pointer <Opinion>;
template class Pointer <Pair <Mathematics :: Vector_3D, Tile> >;
template class Pointer <Pair <string, Messages :: Communications :: Communication> >;
template class Pointer <Pair <string, Skill> >;
#if RADAKAN_AUDIO_MODE == RADAKAN_AUDIERE_MODE
	template class Pointer <Pair <string, Sound_Sample> >;
#endif
template class Pointer <Skill>;
#if RADAKAN_AUDIO_MODE == RADAKAN_AUDIERE_MODE
	template class Pointer <Sound_Sample>;
#endif
template class Pointer <Strategies :: Actions :: Action>;
template class Pointer <Strategies :: Behaviors :: Behavior>;
template class Pointer <Strategies :: Game_Modes :: Game_Mode>;
template class Pointer <Tile>;

template Pointer <Items :: Item> & Pointer <Items :: Item> ::
	operator= (const Reference <Items :: Character> & other);
