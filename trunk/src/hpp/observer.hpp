#ifndef TSL_OBSERVER_HPP
#define TSL_OBSERVER_HPP

#include "object.hpp"

using namespace std;

namespace TSL
{
	template <class T> class Observer :
		public virtual Object
	{
		public :
			//	protected constructor, see below
			virtual ~Observer ();
			virtual bool is_initialized () const;
			
			static const string get_class_name ();

			virtual void call (const string & type, const string & message) = 0;
			
		protected :
			Observer ();
	};
}

#endif	//	TSL_OBSERVER_HPP
