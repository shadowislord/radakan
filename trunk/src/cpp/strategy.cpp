#include "strategy.hpp"
#include "log.hpp"

using namespace std;
using namespace Radakan;
using namespace Radakan :: Strategies;

//	static
string Strategy ::
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
	assert (Strategy :: is_initialized ());
	
	prepare_for_destruction ();
}

//  destructor
Strategy ::
	~Strategy ()
{
	Engines :: Log :: trace (me, Strategy :: get_class_name (), "~");
	assert (Strategy :: is_initialized ());

	//	Do nothing.
	//	'prepare_for_destruction ();' hasn't to be called,
	//	because this is an abstract base class.
}

//	virtual
bool Strategy ::
	is_initialized ()
	const
{
	assert (Object :: is_initialized ());

	return true;
}
