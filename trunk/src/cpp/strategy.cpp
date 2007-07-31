#include "strategy.hpp"
#include "log.hpp"

using namespace std;
using namespace TSL;
using namespace TSL :: Strategies;

//	static
const string Strategy ::
	get_class_name ()
{
	return "Strategy";
}

//  constructor
Strategy ::
	Strategy () :
	Object ("The name doesn't matter as this class is an abstact class.")
{
	Engines :: Log :: trace (me, Strategy :: get_class_name (), "");
	
	//	Do nothing.

	assert (Strategy :: is_initialized ());
}

//  destructor
Strategy ::
	~Strategy ()
{
	Engines :: Log :: trace (me, Strategy :: get_class_name (), "~");
	assert (Strategy :: is_initialized ());

	//	Do nothing.
}

//	virtual
bool Strategy ::
	is_initialized ()
	const
{
	assert (Object :: is_initialized ());

	return true;
}
