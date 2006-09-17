
#ifndef KEYBOARD_EVENT_HPP
#define KEYBOARD_EVENT_HPP

#include "event.hpp"

using namespace std;

class Keyboard_Event:
	public virtual Event
{
	public:
		Keyboard_Event ();
		virtual ~Keyboard_Event ();

		virtual bool is_initialized () const;

        void Set_Event(char evt);
        char Get_Event(char evt);

		static const char EVENT_FORWARD_START       =   0;
		static const char EVENT_FORWARD_STOP        =   1;

		static const char EVENT_BACKWARD_START      =   2;
		static const char EVENT_BACKWARD_STOP       =   3;

		static const char EVENT_LEFT_START          =   4;
		static const char EVENT_LEFT_STOP           =   5;

		static const char EVENT_RIGHT_START         =   6;
		static const char EVENT_RIGHT_STOP          =   7;

		static const char EVENT_JUMP_START          =   8;
		static const char EVENT_JUMP_STOP           =   9;

		static const char EVENT_ATTACK_START        =   10;
		static const char EVENT_ATTACK_STOP         =   11;

		static const char EVENT_EXIT                =   12;

    private:
        char event;
};

#endif	//	KEYBOARD_EVENT_HPP
