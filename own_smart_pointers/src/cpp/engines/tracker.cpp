#include "engines/log.hpp"
#include "engines/tracker.hpp"
#include "pointer.hpp"
#include "set.hpp"

#ifdef RADAKAN_DEBUG
	using namespace std;
	using namespace Radakan;
	using namespace Radakan :: Engines;

	//	static
	string Tracker ::
		get_class_name ()
	{
		return "Tracker";
	}
	
	//	static
	bool Tracker ::
		is_tracking ()
	{
		return tracking;
	}

	//	static
	bool Tracker ::
		tracking (false);

	//  constructor
	Tracker ::
		Tracker () :
		Object ("tracker", "singleton"),
		objects
			(new Set <Object>
				("tracker's objects", Container <Object> :: unlimited (), true))
	{
		Log :: trace (me, Tracker :: get_class_name ());
		
		tracking = true;
		
		assert (Tracker :: is_initialized ());
	}

	//  destructor
	Tracker ::
		~Tracker ()
	{
		Log :: trace (me, Tracker :: get_class_name (), "~");
		assert (Tracker :: is_initialized ());

		prepare_for_destruction ();

		for (Pointer <Object> i = objects -> get_child (); i . points_to_object ();
			i = objects -> get_another_child ())
		{
			Log :: log (me) << "Warning: " << i << " was not destructed." << endl;

			i -> list_references ();
		}
		
		assert (objects -> is_empty ());
	}

	//	virtual
	bool Tracker ::
		is_initialized ()
		const
	{
		return Object :: is_initialized ();
	}
#endif
