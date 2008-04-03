#include "engines/log.hpp"
#include "messages/nothing.hpp"

using namespace std;
using namespace Radakan;
using namespace Radakan :: Messages;

//	static
string Nothing ::
	get_class_name ()
{
	return "Messages :: Nothing";
}

//	static
Reference <Nothing> Nothing ::
	get ()
{
	return Reference <Nothing> ();
}

//  private constructor
Nothing ::
	Nothing () :
	Object ("Doesn't matter.")
{
	Engines :: Log :: trace (me, Nothing :: get_class_name ());
	assert (Object :: is_initialized ());
	
	abort ();
}

//  destructor
Nothing ::
	~Nothing ()
{
	Engines :: Log :: trace (me, Nothing :: get_class_name (), "~");
	assert (is_initialized ());

	abort ();
}

//	virtual
bool Nothing ::
	is_initialized ()
	const
{
	abort ();
	
	return false;
}
