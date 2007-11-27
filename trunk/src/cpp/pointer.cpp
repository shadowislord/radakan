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
#include "items/npc.hpp"
#include "messages/message.hpp"
#include "movable_model.hpp"
#include "opinion.hpp"
#include "pair.hpp"
#include "play_state_gui.hpp"
#include "skill.hpp"
#include "strategies/strategy.hpp"
#include "tile.hpp"

template class Pointer <Sound_Sample>;
template class Pointer <Container <GUI> >;
template class Pointer <Container <Items :: Character> >;
template class Pointer <Container <Items :: Container_Item <Items :: Container_Item <Items :: Item> > > >;
template class Pointer <Container <Items :: Container_Item <Items :: Item> > >;
template class Pointer <Container <Items :: Item> >;
template class Pointer <Container <Model> >;
template class Pointer <Container <Strategies :: Strategy <Engines :: Game> > >;
template class Pointer <Container <Strategies :: Strategy <Items :: Character> > >;
template class Pointer <Container <Tile> >;
template class Pointer <GUI>;
template class Pointer <Items :: Character>;
template class Pointer <Items :: Container_Item <Items :: Container_Item <Items :: Item> > >;
template class Pointer <Items :: Container_Item <Items :: Item> >;
template class Pointer <Items :: Item>;
template class Pointer <Items :: NPC>;
template class Pointer <Location <GUI> >;
template class Pointer <Location <Items :: Item> >;
template class Pointer <Location <Items :: Container_Item <Items :: Container_Item <Items :: Item> > > >;
template class Pointer <Location <Items :: Container_Item <Items :: Item> > >;
template class Pointer <Location <Model> >;
template class Pointer <Location <Strategies :: Strategy <Engines :: Game> > >;
template class Pointer <Location <Strategies :: Strategy <Items :: Character> > >;
template class Pointer <Messages :: Message <Items :: Character> >;
template class Pointer <Model>;
template class Pointer <Movable_Model>;
template class Pointer <Object>;
template class Pointer <Observer <Messages :: Message <Items :: Character> > >;
template class Pointer <Observer <Object> >;
template class Pointer <Opinion>;
#if RADAKAN_GUI_MODE == RADAKAN_CEGUI_MODE
	template class Pointer <Pair <CEGUI :: ListboxItem *, Messages :: Message <Items :: Character> > >;
#endif
template class Pointer <Pair <pair <int, int>, Tile> >;
template class Pointer <Pair <string, Skill> >;
template class Pointer <Play_State_GUI>;
template class Pointer <Skill>;
template class Pointer <Strategies :: Strategy <Engines :: Game> >;
template class Pointer <Strategies :: Strategy <Items :: Character> >;
template class Pointer <Tile>;

template Pointer <Items :: Character> & Pointer <Items :: Character> ::
	operator= (const Reference <Items :: NPC> & other);
template Pointer <Items :: Item> & Pointer <Items :: Item> ::
	operator= (const Reference <Items :: NPC> & other);
