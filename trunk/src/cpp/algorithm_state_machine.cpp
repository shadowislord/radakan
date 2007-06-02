#include "algorithm_state_machine.hpp"
#include "log.hpp"

using namespace std;
using namespace TSL;
using namespace TSL :: Algorithms;

//	static
const string Algorithm_State_Machine ::
	get_class_name ()
{
	return "Algorithm_State_Machine";
}

//  constructor
Algorithm_State_Machine ::
	Algorithm_State_Machine () :
	Object ("The name doesn't matter as this class is an abstact class.")
{
	Engines :: Log :: trace (this -> me, Algorithm_State_Machine :: get_class_name (), "");
	
	//	Do nothing.

	assert (Algorithm_State_Machine :: is_initialized ());
}

//  destructor
Algorithm_State_Machine ::
	~Algorithm_State_Machine ()
{
	Engines :: Log :: trace (this -> me, Algorithm_State_Machine :: get_class_name (), "~");
	assert (Algorithm_State_Machine :: is_initialized ());

	State_Machine <Algorithm > :: unset_active_state ();
}

//	virtual
bool Algorithm_State_Machine ::
	is_initialized ()
	const
{
	assert (State_Machine <Algorithm> :: is_initialized ());

	return true;
}

//	virtual
void Algorithm_State_Machine ::
	drop (Object & t, bool stay)
{
	Engines :: Log :: trace (this -> me, Algorithm_State_Machine :: get_class_name (), "drop", t, bool_to_string (stay));
	assert (is_initialized ());
	
	State_Machine <Algorithm> :: drop (t, stay);
}

//	virtual
void Algorithm_State_Machine ::
	transit (const Object & message)
{
	assert (is_initialized ());

	if (has_active_state ())
	{
		State_Machine <Algorithm> :: get_active_state () . transit (message);
	}
}
