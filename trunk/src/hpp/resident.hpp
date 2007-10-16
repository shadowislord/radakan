#ifndef RADAKAN_RESIDENT_HPP
#define RADAKAN_RESIDENT_HPP

#include "object.hpp"

using namespace std;

namespace Radakan
{
	template <class T> class Location;
	
	///	I can only be in one Location at once.
	template <class T> class Resident :
		public virtual Object
	{
		public :
			//	protected constructor, see below
			virtual ~Resident ();
			virtual bool is_initialized () const;
			
			static string get_class_name ();

			void enter (const Reference <Location <T> > & new_location);
			void leave ();

		protected :
			Resident ();

		private :
			Reference <Location <T> > location;
	};
}

#endif	// RADAKAN_RESIDENT_HPP
