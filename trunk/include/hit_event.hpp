#ifndef HIT_EVENT_HPP
#define HIT_EVENT_HPP

#include "character.hpp"
#include "event.hpp"

using namespace std;

class Hit_Event:
	public virtual Event
{
	public:
		Hit_Event (Character * new_attacker, Character * new_defender);
		virtual ~Hit_Event ();
		virtual bool is_initialized () const;
		Character * attacker;
		Character * defender;
};

#endif	//	EVENT_HPP
