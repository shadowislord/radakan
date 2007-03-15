/*#ifndef TSL_OBSERVER_HPP
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

			virtual void call (string message) = 0;
			
		protected:
			Observer ();
	};
}

#endif	//	TSL_OBSERVER_HPP*/
