#include "container.hpp"
#include "engines/log.hpp"

using namespace std;
using namespace Radakan;

//	static
template <class T> string Container <T> ::
	get_class_name ()
{
	return "Container <" + T :: get_class_name () + ">";
}

//	This could have been any number (strictly) below 0.
template <class T> const int Container <T> ::
	unlimited = - 10;

//  constructor
template <class T> Container <T> ::
	Container (string name, int new_maximal_size) :
	Object (name),
	maximal_size (new_maximal_size),
	sealed (false)
{
	Engines :: Log :: trace (me, Container <T> :: get_class_name (), "", name, to_string (new_maximal_size));
	assert ((new_maximal_size == unlimited) || (0 <= new_maximal_size));

	//	Do nothing.

	assert (Container <T> :: is_initialized ());
}

//  destructor
template <class T> Container <T> ::
	~Container ()
{
	Engines :: Log :: trace (me, Container <T> :: get_class_name (), "~");
	assert (Container <T> :: is_initialized ());

	prepare_for_destruction ();
}

//	virtual
template <class T> bool Container <T> ::
	is_initialized ()
	const
{
	assert (Object :: is_initialized ());
	assert (unlimited < 0);
	assert ((maximal_size == unlimited) || (0 <= maximal_size));

	return true;
}

template <class T> void Container <T> ::
	seal ()
{
	sealed = true;
}

template <class T> bool Container <T> ::
	is_sealed () const
{
	return sealed;
}

//	virtual
template <class T> bool Container <T> ::
	move (Reference <T> moved, Reference <Container <T> > destination)
{
	Engines :: Log :: trace (this -> me, Container <T> :: get_class_name (), "move", moved . get_name (), destination . get_name ());
	assert (Container <T> :: is_initialized ());
	assert (moved -> is_initialized ());
	assert (destination -> is_initialized ());
	assert (! Container <T> :: is_sealed ());
	assert (contains (moved));

	drop (moved);
	
	if (destination -> add (moved))
	{
		return true;
	}
	bool check = add (moved);
	assert (check);
	
	return false;
}

//	to avert linking errors:
#include "engines/audio_engine.hpp"
#include "engines/game.hpp"
#include "gui.hpp"
#include "items/character.hpp"
#include "items/container_item.hpp"
#include "items/npc.hpp"
#include "messages/conversation_message.hpp"
#include "model.hpp"
#include "movable_model.hpp"
#include "opinion.hpp"
#include "strategies/play_state.hpp"
#include "strategies/strategy.hpp"
#include "tile.hpp"

template class Container <GUI>;
template class Container <Items :: Character>;
template class Container <Items :: Container_Item <Items :: Container_Item <Items :: Item> > >;
template class Container <Items :: Container_Item <Items :: Item> >;
template class Container <Items :: Item>;
template class Container <Items :: NPC>;
template class Container <Messages :: Message <Items :: Character> >;
template class Container <Model>;
template class Container <Object>;
template class Container <Observer <Messages :: Message <Items :: Character> > >;
template class Container <Observer <Object> >;
template class Container <Opinion>;
template class Container <Sound_Sample>;
template class Container <Strategies :: Strategy <Engines :: Game> >;
template class Container <Strategies :: Strategy <Items :: Character> >;
template class Container <Tile>;
