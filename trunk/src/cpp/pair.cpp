#include "pair.hpp"

using namespace std;
using namespace Radakan;

//	static
template <typename T, class U> string Pair <T, U> ::
	get_class_name ()
{
	//	Do not turn this into a static data member,
	//	that'd cause problems for template (sub)classes.

	return "Pair <@T@, " + U :: get_class_name () + ">";
}

//	static
template <typename T, class U> Reference <Pair <T, U> > Pair <T, U> ::
	create (const T & new_first, const Reference <U> & second)
{
	return Reference <Pair <T, U> > (new Pair <T, U> (new_first, second));
}

template <typename T, class U> Pair <T, U> ::
	Pair (const T & new_first, const Reference <U> & second) :
	Object ("pair of '@first@' and " + second . get_name ()),
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

#include "messages/communications/communication.hpp"
#include "skill.hpp"
#include "tile.hpp"

template class Pair <string, Messages :: Communications :: Communication>;
template class Pair <pair <int, int>, Tile>;
template class Pair <string, Skill>;
