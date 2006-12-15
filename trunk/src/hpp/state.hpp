#ifndef STATE_HPP
#define STATE_HPP

#include "character.hpp"

using namespace std;

namespace sl
{
	class State :
		public virtual Object
	{
		public :
			State (Character & new_owner);
			virtual ~State ();
			virtual bool is_initialized () const;
			static string get_type_name ();
			
			virtual string think ();	//	change your current state

		protected:
			Character & owner;
	};
}

#endif	//	STATE_HPP
