#include "engines/log.hpp"
#include "engines/tracker.hpp"
#include "pointer.hpp"

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

	//  constructor
	Tracker ::
		Tracker () :
		Object ("tracker", true)	//	Here 'true' means 'prevent automatic destruction'.
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

		for (Pointer <Object> i = get_child (); i . points_to_object ();
			i = get_another_child ())
		{
			Log :: log (me) << "Warning: " << i << " was not destructed." << endl;
		}
		
		assert (Tracker :: get () -> is_empty ());
	}

	//	virtual
	bool Tracker ::
		is_initialized ()
		const
	{
		return Object :: is_initialized ();
	}
#endif
