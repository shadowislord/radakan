#ifndef TSL_LOCATION_HPP
#define TSL_LOCATION_HPP

#include "set.hpp"

using namespace std;

namespace TSL
{

	///	Location can hold subclasses of Resident.
	template <class T> class Location :
		public Set <T>
	{
		public :
			//	protected constructor, see below
			virtual ~Location ();
			virtual bool is_initialized () const;
			
			static const string get_class_name ();

			///	'add' returns true on success.
			virtual bool add (T & t);

			virtual void drop (Object & t, bool stay = false);
			
		protected :
			Location (int new_maximal_size = Set <T> :: unlimited);
	};
}

#endif	// TSL_LOCATION_HPP
