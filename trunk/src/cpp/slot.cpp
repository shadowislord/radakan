#include "engines/log.hpp"
#include "slot.hpp"

using namespace std;
using namespace Radakan;

//	static
template <class T> string Slot <T> ::
	get_class_name ()
{
	return "Slot <" + T :: get_class_name () + ">";
}

//  constructor
template <class T> Slot <T> ::
	Slot (string new_name, bool new_weak_children) :
	Object (new_name),
	Container <T> (1, new_weak_children)
{
	Engines :: Log :: trace (this -> Object :: me, Slot :: get_class_name (), "");
	
	child . set_parent (* this);

	assert (Slot <T> :: is_initialized ());
}

//  destructor
template <class T> Slot <T> ::
	~Slot ()
{
	Engines :: Log :: trace (this -> me, Slot :: get_class_name (), "~");
	assert (Slot <T> :: is_initialized ());

	Object :: prepare_for_destruction ();
}

//	virtual
template <class T> bool Slot <T> ::
	is_initialized ()
	const
{
	assert (Container <T> :: is_initialized ());
	if (child . points_to_object ())
	{
		assert (child -> is_initialized ());
	}

	return true;
}

//	virtual
template <class T> bool Slot <T> ::
	is_empty () const
{
	assert (Slot <T> :: is_initialized ());

	return ! child . points_to_object ();
}

//	virtual
template <class T> bool Slot <T> ::
	contains (const Reference <T> & contained)
	const
{
//	Engines :: Log :: trace (this -> this -> me, Slot <T> :: get_class_name (), "contains", contained . get_name ());
	assert (Slot <T> :: is_initialized ());
	assert (contained . points_to_object ());
	assert (contained -> is_initialized ());

	return child == contained;
}

//	virtual
template <class T> bool Slot <T> ::
	add (Reference <T> added)
{
	assert (Slot <T> :: is_initialized ());
	assert (added . points_to_object ());
	assert (added -> is_initialized ());

	if (child . points_to_object ())
	{
		return false;
	}

	if (Container <T> :: weak_children)
	{
		child . reset_pointee (added, true);
	}
	else
	{
		child = added;
	}

	return true;
}

//	virtual
template <class T> void Slot <T> ::
	drop (Reference <T> dropped)
{
	Engines :: Log :: trace (this -> me, Slot <T> :: get_class_name (), "drop", dropped . get_name ());
	assert (Slot <T> :: is_initialized ());
	assert (! Container <T> :: is_sealed ());
	assert (dropped . points_to_object ());
	assert (dropped -> is_initialized ());
	assert (contains (dropped));

	child . reset_pointee ();
}

template <class T> Reference <T> Slot <T> ::
	get_child ()
	const
{
//	Engines :: Log :: trace (this -> this -> me, Slot <T> :: get_class_name (), get_child);
	assert (Slot <T> :: is_initialized ());

	return child;
}

template <class T> Reference <T> Slot <T> ::
	get_another_child ()
	const
{
//	Engines :: Log :: trace (this -> this -> me, Slot <T> :: get_class_name (), get_another_child);
	assert (Slot <T> :: is_initialized ());

	return Reference <T> ();
}

#if RADAKAN_AUDIO_MODE == RADAKAN_AUDIERE_MODE
	#include "engines/audio_engine.hpp"
#endif
#include "gui.hpp"
#include "items/container_item.hpp"
#include "messages/communications/communication.hpp"
#include "model.hpp"
#include "skill.hpp"
#include "strategies/actions/action.hpp"
#include "strategies/behaviors/behavior.hpp"
#include "strategies/game_modes/game_mode.hpp"
#include "tile.hpp"

template class Slot <GUI>;
template class Slot <Items :: Container_Item <Items :: Container_Item <Items :: Item> > >;
template class Slot <Items :: Container_Item <Items :: Item> >;
template class Slot <Items :: Item>;
template class Slot <Messages :: Communications :: Communication>;
template class Slot <Model>;
template class Slot <Skill>;
#if RADAKAN_AUDIO_MODE == RADAKAN_AUDIERE_MODE
	template class Slot <Sound_Sample>;
#endif
template class Slot <Strategies :: Actions :: Action>;
template class Slot <Strategies :: Behaviors :: Behavior>;
template class Slot <Strategies :: Game_Modes :: Game_Mode>;
template class Slot <Tile>;
