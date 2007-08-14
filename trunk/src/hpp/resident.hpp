#ifndef RADAKAN_RESIDENT_HPP
#define RADAKAN_RESIDENT_HPP

#include "location.hpp"

using namespace std;

namespace Radakan
{
	///	I can only be in one Location at once.
	template <class T> class Resident :
		public virtual Object
	{
		public :
			//	protected constructor, see below
			virtual ~Resident ();
			virtual bool is_initialized () const;
			
			static const string get_class_name ();

			void enter (const Location <T> & new_location);
			void leave (const Location <T> & old_location);

		protected :
			Resident ();

		private :
			const Location <T> * location;
	};
}

#endif	// RADAKAN_RESIDENT_HPP
