#include "pair.hpp"

using namespace std;
using namespace Radakan;

//	static
template <typename T, class U> string Pair <T, U> ::
	get_class_name ()
{
	//	Do not turn this into a static data member,
	//	that'd cause problems for template (sub)classes.

	return "Pair <~T~, " + U :: get_class_name () + ">";
}

template <typename T, class U> Pair <T, U> ::
	Pair (const T & new_first, const Reference <U> & second) :
	Object ("pair of 'first' and " + second . get_name ()),
	Slot <U> ("Doesn't matter"),
	first (new_first)
{
	add (second);

	Slot <U> :: seal ();
}

template <typename T, class U> Pair <T, U> ::
	Pair (const Pair <T, U> & other) :
	Object ("pair of 'first' and " + other . get_child () . get_name ()),
	Slot <U> ("Doesn't matter"),
	first (other . first)
{
	add (other . get_child ());

	Slot <U> :: seal ();
}

//	virtual
template <typename T, class U> Pair <T, U> ::
	~Pair ()
{

}

#if RADAKAN_GUI_MODE == RADAKAN_CEGUI_MODE
	#include <elements/CEGUIListboxItem.h>
#endif
#include "items/characters/character.hpp"
#include "messages/message.hpp"
#include "skill.hpp"
#include "tile.hpp"

#if RADAKAN_GUI_MODE == RADAKAN_CEGUI_MODE
	template class Pair <CEGUI :: ListboxItem *, Messages :: Message <Items :: Characters :: Character> >;
#endif
template class Pair <pair <int, int>, Tile>;
template class Pair <string, Skill>;
