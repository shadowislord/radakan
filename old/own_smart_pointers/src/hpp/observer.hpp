#ifndef RADAKAN_OBSERVER_HPP
#define RADAKAN_OBSERVER_HPP

#include "object.hpp"

using namespace std;

namespace Radakan
{
	namespace Messages
	{
		class Nothing;
	}
	
	///	Observer is an abstract base class for the observer pattern.
	template <class T> class Observer :
		public virtual Object
	{
		public :
			//	protected constructor, see below
			virtual ~Observer ();
			virtual bool is_initialized () const;
			
			static string get_class_name ();
			
			virtual void call (Reference <T> message) = 0;
			
		protected :
			Observer ();
	};
}

#endif	//	RADAKAN_OBSERVER_HPP