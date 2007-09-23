#include "log.hpp"
#include "tracker.hpp"

using namespace std;
using namespace Radakan;
using namespace Radakan :: Engines;

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
	Log :: trace (me, Tracker :: get_class_name ());
	assert (Tracker :: is_initialized ());

	prepare_for_destruction ();
}

//  destructor
Tracker ::
	~Tracker ()
{
	Log :: trace (me, Tracker :: get_class_name (), "~");
	assert (Tracker :: is_initialized ());

	#ifdef RADAKAN_DEBUG
		for (Reference <Object> i = Tracker :: get () -> get_child (); i . points_to_object ();
			i = Tracker :: get () -> get_another_child ())
		{
			Log :: log (me) << "Warning: " << i -> name << " was not destructed." << endl;
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
