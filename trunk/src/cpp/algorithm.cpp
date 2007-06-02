#include "algorithm.hpp"
#include "log.hpp"

using namespace std;
using namespace TSL;
using namespace TSL :: Algorithms;

//	static
const string Algorithm ::
	get_class_name ()
{
	return "Algorithm";
}

//  constructor
Algorithm ::
	Algorithm () :
	Object ("The name doesn't matter as this class is an abstact class.")
{
	Engines :: Log :: trace (me, Algorithm :: get_class_name (), "");
	
	//	Do nothing.

	assert (Algorithm :: is_initialized ());
}

//  destructor
Algorithm ::
	~Algorithm ()
{
	Engines :: Log :: trace (me, Algorithm :: get_class_name (), "~");
	assert (Algorithm :: is_initialized ());

	//	Do nothing.
}

//	virtual
bool Algorithm ::
	is_initialized ()
	const
{
	assert (Object :: is_initialized ());

	return true;
}
