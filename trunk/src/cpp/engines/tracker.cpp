#include "engines/log.hpp"
#include "engines/tracker.hpp"
#include "pointer.hpp"

using namespace std;
using namespace Radakan;
using namespace Radakan :: Engines;

//	static
string Tracker ::
	get_class_name ()
{
	return "Tracker";
}

//  constructor
Tracker ::
	Tracker () :
	Object ("tracker")
{
	Log :: trace (me, Tracker :: get_class_name ());
	assert (Tracker :: is_initialized ());
}

//  destructor
Tracker ::
	~Tracker ()
{
	Log :: trace (me, Tracker :: get_class_name (), "~");
	assert (Tracker :: is_initialized ());

	prepare_for_destruction ();

	#ifdef RADAKAN_DEBUG
		for (Pointer <Object> i = Tracker :: get () -> get_child (); i . points_to_object ();
			i = Tracker :: get () -> get_another_child ())
		{
			Log :: log (me) << "Warning: " << i << " was not destructed." << endl;
		}
		assert (Tracker :: get () -> is_empty ());
	#endif
}

//	virtual
bool Tracker ::
	is_initialized ()
	const
{
	return Object :: is_initialized ();
}
