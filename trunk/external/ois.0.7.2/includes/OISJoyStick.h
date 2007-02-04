/*
The zlib/libpng License

Copyright (c) 2006 Phillip Castaneda (pjcast -- www.wreckedgames.com)

This software is provided 'as-is', without any express or implied warranty. In no event will
the authors be held liable for any damages arising from the use of this software.

Permission is granted to anyone to use this software for any purpose, including commercial
applications, and to alter it and redistribute it freely, subject to the following
restrictions:

    1. The origin of this software must not be misrepresented; you must not claim that
		you wrote the original software. If you use this software in a product,
		an acknowledgment in the product documentation would be appreciated but is
		not required.

    2. Altered source versions must be plainly marked as such, and must not be
		misrepresented as being the original software.

    3. This notice may not be removed or altered from any source distribution.
*/
#ifndef OIS_Joystick_H
#define OIS_Joystick_H
#include "OISObject.h"
#include "OISEvents.h"

namespace OIS
{
	//! POV / HAT Joystick component
	struct _OISExport Pov : Component
	{
		Pov() : Component(OIS_POV), direction(0) {}

		static const int Centered  = 0x00000000;
		static const int North     = 0x00000001;
		static const int South     = 0x00000010;
		static const int East      = 0x00000100;
		static const int West      = 0x00001000;
		static const int NorthEast = 0x00000101;
		static const int SouthEast = 0x00000110;
		static const int NorthWest = 0x00001001;
		static const int SouthWest = 0x00001010;

		int direction;
	};

	//! A sliding axis - only used in Win32 Right Now
	struct _OISExport Slider : Component
	{
		Slider() : Component(OIS_Slider), abX(0), abY(0) {};
		//! true if pushed, false otherwise
		int abX, abY;
	};

	/**
		Represents the state of the joystick
		All members are valid for both buffered and non buffered mode
		Sticks with zero values are not present on the device
	*/
	struct _OISExport JoyStickState
	{
		JoyStickState() : buttons(0) { clear(); }

		enum AxisName
		{
			//! Typical Stick Axis
			Stick = 0,
			//! Rotational Axis
			Rotational,
			//! Velocity Axis
			Velocity,
			//! Angular Velocity Axis
			AngularVelocity,
			//! Acceleration Axis
			Acceleration,
			//! AngularAcceleration Axis
			AngularAcceleration,
			//! Force Axis
			Force,
			//! Torque Axis
			Torque
		};

		//! Represents the maximum number of axes
		Axis mAxes[8];

		//! Represents the value of a POV. Maximum of 4
		Pov mPOV[4];

		//! Represent the max sliders
		Slider mSliders[4];

		//! Represents All Buttons By Bit (max buttons = 32)
		int buttons;

		//! Nice Button Bit Test
		inline int buttonDown( int button ) const {
			return buttons & ( 1 << button );
		}

		//! internal method
		void clear() {
			buttons = 0;
			for( int i = 0; i < 8; ++i )
			{
				mAxes[i].absOnly = true;	//Currently, joysticks only report Abs
				mAxes[i].clear();
			}
		}
	};

	/**
		Specialised for joystick events
	*/
	class _OISExport JoyStickEvent : public EventArg
	{
	public:
		JoyStickEvent( Object* obj, unsigned int ts, const JoyStickState &st ) :
		  EventArg(obj,ts), state(st) {}
		virtual ~JoyStickEvent() {}

		const JoyStickState &state;
	};

	/**
		To recieve buffered joystick input, derive a class from this, and implement the
		methods here. Then set the call back to your JoyStick instance with JoyStick::setEventCallback
		Each JoyStick instance can use the same callback class, as a devID number will be provided
		to differentiate between connected joysticks. Of course, each can have a seperate
		callback instead.
	*/
	class _OISExport JoyStickListener
	{
	public:
		virtual ~JoyStickListener() {}

		virtual bool buttonPressed( const JoyStickEvent &arg, int button ) = 0;
		virtual bool buttonReleased( const JoyStickEvent &arg, int button ) = 0;

		virtual bool axisMoved( const JoyStickEvent &arg, int axis ) = 0;

        //Joystick Event, amd sliderID
		virtual bool sliderMoved( const JoyStickEvent &, int ) {return true;}
		//Joystick Event, amd povID
		virtual bool povMoved( const JoyStickEvent &, int ) {return true;}
	};

	/**
		Joystick base class. To be implemented by specific system (ie. DirectX joystick)
		This class is useful as you remain OS independent using this common interface.
	*/
	class _OISExport JoyStick : public Object
	{
	public:
		virtual ~JoyStick() {}

		/**
		@remarks
			Returns the number of buttons
		*/
		short buttons() {return numButtons;}

		/**
		@remarks
			Returns the number of axes
		*/
		short axes() {return numAxes;}

		/**
		@remarks
			Register/unregister a JoyStick Listener - Only one allowed for simplicity. If broadcasting
			is neccessary, just broadcast from the callback you registered.
		@param joyListener
			Send a pointer to a class derived from JoyStickListener or 0 to clear the callback
		*/
		virtual void setEventCallback( JoyStickListener *joyListener ) {listener = joyListener;}

		/**
		@remarks
			Returns currently set callback.. or 0
		*/
		JoyStickListener* getEventCallback() {return listener;}

		/**
		@remarks
			Returns the state of the joystick - is valid for both buffered and non buffered mode
		*/
		const JoyStickState& getJoyStickState() const { return mState; }

		//! The minimal axis value
		static const int MIN_AXIS = -32767;
		//! The maximum axis value
		static const int MAX_AXIS = 32768;

	protected:
		JoyStick() : numButtons(0), numAxes(0), listener(0) {}

		short numButtons;
		short numAxes;

		JoyStickState mState;

		JoyStickListener *listener;
	};
}
#endif
