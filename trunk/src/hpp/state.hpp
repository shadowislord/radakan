#ifndef STATE_HPP
#define STATE_HPP

#include "set.hpp"

using namespace std;

namespace tsl
{

	///	State is the abstract base class for all 'finite state machine'-states.

	template <typename T> class State :
		public virtual Object
	{
		public :
			//	protected constructor, see below
			virtual ~State ();
			virtual bool is_initialized () const;
			static string get_type_name ();

			//	pure virtual method
			virtual string think () = 0;	//	change your current state

		protected:
			State (T & new_owner);
			T & owner;
	};
}

#endif	//	STATE_HPP
