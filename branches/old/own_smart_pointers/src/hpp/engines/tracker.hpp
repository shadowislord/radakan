#ifndef RADAKAN_TRACKER_HPP
	#define RADAKAN_TRACKER_HPP

	#ifdef RADAKAN_DEBUG

		#include "container.hpp"
		#include "singleton.hpp"

		using namespace std;

		namespace Radakan
		{
			namespace Engines
			{

				///	Tracker is used to keep track of all objects.
				///	For tracking method calls, see Log :: trace.
				class Tracker :
					public Singleton <Tracker>
				{
					public :
						static  string get_class_name ();
						
						static bool is_tracking ();
						
					private :
						static bool tracking;
						
					public :
						Tracker ();
						virtual ~Tracker ();
						virtual bool is_initialized () const;
						
						Reference <Container <Object> > objects;
				};
			}
		}

	#endif	// RADAKAN_DEBUG
#endif	// RADAKAN_TRACKER_HPP
