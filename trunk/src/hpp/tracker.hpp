#ifndef RADAKAN_TRACKER_HPP
#define RADAKAN_TRACKER_HPP

#include "set.hpp"
#include "singleton.hpp"

using namespace std;

namespace Radakan
{
	namespace Engines
	{

		///	Tracker is used to keep track of all objects.
		///	For tracking method calls, see Log :: trace.
		class Tracker :
			public Singleton <Tracker>,
			public Set <Object>
		{
			public :
				Tracker ();
				virtual ~Tracker ();
				virtual bool is_initialized () const;
				
				static  string get_class_name ();
		};
	}
}

#endif	// RADAKAN_TRACKER_HPP
