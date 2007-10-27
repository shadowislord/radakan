#include "engines/log.hpp"
#include "strategies/strategy.hpp"

using namespace std;
using namespace Radakan;
using namespace Radakan :: Strategies;

//	static
template <class T> string Strategy <T> ::
	get_class_name ()
{
	return "Strategy <" + T:: get_class_name () + ">";
}

//  constructor
template <class T> Strategy <T> ::
	Strategy () :
	Object ("The name doesn't matter as this class is an abstact class.")
{
	Engines :: Log :: trace (this -> me, Strategy <T> :: get_class_name (), "");
	assert (Strategy :: is_initialized ());

	//	Do nothing.
}

//  destructor
template <class T> Strategy <T> ::
	~Strategy ()
{
	Engines :: Log :: trace (this -> me, Strategy <T> :: get_class_name (), "~");
	assert (Strategy :: is_initialized ());

	//	Do nothing.
	//	'prepare_for_destruction ();' hasn't to be called,
	//	because this is an abstract base class.
}

//	virtual
template <class T> bool Strategy <T> ::
	is_initialized ()
	const
{
	assert (Object :: is_initialized ());

	return true;
}

#include "engines/game.hpp"
#include "items/character.hpp"

template class Strategy <Engines :: Game>;
template class Strategy <Items :: Character>;
