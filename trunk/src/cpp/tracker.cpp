#include "log.hpp"
#include "tracker.hpp"

using namespace std;
using namespace TSL;
using namespace TSL :: Engines;

//	static
const string Tracker ::
	get_class_name ()
{
	return "Tracker";
}

//  constructor
Tracker ::
	Tracker () :
	Object ("tracker")
{
	Log :: trace <Tracker> (me);

	//	Do nothing.
	
	assert (Tracker :: is_initialized ());
}

//  destructor
Tracker ::
	~Tracker ()
{
	Log :: trace <Tracker> (me, "~");
	assert (Tracker :: is_initialized ());

	#ifdef TSL_DEBUG
		for (Object * i = Tracker :: get () . get_child (); i != NULL;
			i = Tracker :: get () . get_another_child ())
		{
			cout << "Warning: " << * i << " (" << i << ") was not deleted." << endl;
		}
		assert (Tracker :: get () . is_empty ());
	#endif
}

//	virtual
bool Tracker ::
	is_initialized ()
	const
{
	return Object :: is_initialized ();
}
