#ifndef AI_STATE_HPP
#define AI_STATE_HPP

#include "object.hpp"

using namespace std;

class AI_State:
	public virtual Object
{
	public:
		AI_State (string new_name);
		virtual ~AI_State ();
		virtual bool is_initialized () const;
};

#endif	//	AI_STATE_HPP
