#ifndef TSL_OBSERVER_HPP
#define TSL_OBSERVER_HPP

#include "object.hpp"

using namespace std;

namespace tsl
{
	template <class T> class Observer :
		public virtual Object
	{
		public :
			//	protected constructor, see below
			virtual ~Observer ();
			virtual bool is_initialized () const;
			static string get_class_name ();

		protected:
			Observer ();
	};
}

#endif	//	TSL_OBSERVER_HPP
