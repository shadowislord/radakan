#ifndef STATE_HPP
#define STATE_HPP

#include "set.hpp"

using namespace std;

namespace tsl
{

	///	State is the abstract base class for all 'finite state machine'-states.

	template <class T> class State :
		public virtual Object
	{
		public :
			//	protected constructor, see below
			virtual ~State ();
			virtual bool is_initialized () const;
			static string get_class_name ();

			//	pure virtual method
			virtual string run () = 0;
			
			static string nothing;

		protected:
			State (T & new_owner);
			T & owner;
	};
}

#endif	//	STATE_HPP
