#include "container.hpp"
#include "engines/log.hpp"
#include "pointer.hpp"

using namespace std;
using namespace Radakan;


//	static
template <class T> string Container <T> ::
	get_class_name ()
{
	return "Container <" + T :: get_class_name () + ">";
}

template <class T> unsigned int Container <T> :: unlimited()
{
	return numeric_limits<unsigned int>::max ();
}

//  constructor
template <class T> Container <T> ::
	Container (unsigned int new_maximal_size) :
	Object ("Doesn't matter."),
	maximal_size (new_maximal_size),
	sealed (false)
{
	Engines :: Log :: trace (me, Container <T> :: get_class_name (), "", name, to_string (new_maximal_size));
	assert (0 < maximal_size);

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

	return true;
}

template <class T> void Container <T> ::
	clear ()
{
	for (Pointer <T> child (get_child ()); child . points_to_object (); child = get_child ())
	{
		Engines :: Log :: log (this -> me) << "Dropping child " << child . get_name () << "..." << endl;
		
		drop (child);
	}
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
	Engines :: Log :: trace (me, Container <T> :: get_class_name (), "move", moved . get_name (), destination . get_name ());
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
#if RADAKAN_GUI_MODE == RADAKAN_CEGUI_MODE
	#include <elements/CEGUIListboxItem.h>
#endif
#include "engines/audio_engine.hpp"
#include "engines/game.hpp"
#include "gui.hpp"
#include "items/character.hpp"
#include "items/container_item.hpp"
#include "messages/message.hpp"
#include "model.hpp"
#include "movable_model.hpp"
#include "opinion.hpp"
#include "pair.hpp"
#include "play_gui.hpp"
#include "skill.hpp"
#include "strategies/actions/action.hpp"
#include "strategies/game_modes/play.hpp"
#include "tile.hpp"

template class Container <GUI>;
template class Container <Items :: Character>;
template class Container <Items :: Container_Item <Items :: Container_Item <Items :: Item> > >;
template class Container <Items :: Container_Item <Items :: Item> >;
template class Container <Items :: Item>;
template class Container <Messages :: Message <Items :: Character> >;
template class Container <Model>;
template class Container <Object>;
template class Container <Observer <Messages :: Message <Items :: Character> > >;
template class Container <Observer <Messages :: Message <Object> > >;
template class Container <Observer <Object> >;
template class Container <Opinion>;
#if RADAKAN_GUI_MODE == RADAKAN_CEGUI_MODE
	template class Container <Pair <CEGUI :: ListboxItem *, Messages :: Message <Items :: Character> > >;
#endif
template class Container <Pair <pair <int, int>, Tile> >;
template class Container <Pair <string, Skill> >;
template class Container <Skill>;
#if RADAKAN_AUDIO_MODE == RADAKAN_AUDIERE_MODE
	template class Container <Sound_Sample>;
#endif
template class Container <Strategies :: Actions :: Action>;
template class Container <Strategies :: Behaviors :: Behavior>;
template class Container <Strategies :: Game_Modes :: Game_Mode>;
template class Container <Tile>;
