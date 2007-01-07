#ifndef STATE_HPP
#define STATE_HPP

#include "character.hpp"

using namespace std;

namespace sl
{

	///	State is the abstract base class for all 'finite state machine'-states.

	class State :
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
			State (Character & new_owner);
			Character & owner;
	};
}

#endif	//	STATE_HPP
