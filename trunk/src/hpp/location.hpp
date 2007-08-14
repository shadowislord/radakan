#ifndef RADAKAN_LOCATION_HPP
#define RADAKAN_LOCATION_HPP

#include "set.hpp"

using namespace std;

namespace Radakan
{

	///	Only one instances of me can hold a specifc Resident.
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

#endif	// RADAKAN_LOCATION_HPP
