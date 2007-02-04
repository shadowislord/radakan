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
//Check to see which JoyStick support is being compiled in
#include "OISConfig.h"
#ifdef OIS_JOYEVENTS

#include "linux/LinuxJoyStickEvents.h"
#include "linux/LinuxInputManager.h"
#include "linux/LinuxForceFeedback.h"
#include "linux/EventHelpers.h"

#include "OISEvents.h"
#include "OISException.h"

#include <fcntl.h>        //Needed to Open a file descriptor
#include <linux/input.h>

#include <sstream>

using namespace OIS;

//#define OIS_LINUX_JOY_DEBUG

#ifdef OIS_LINUX_JOY_DEBUG
# include <iostream>
#endif

//-------------------------------------------------------------------//
LinuxJoyStick::LinuxJoyStick(bool buffered, const JoyStickInfo& js)
{
	mBuffered = buffered;
	mType = OISJoyStick;

	mDevID = js.devId;
	mJoyStick = js.joyFileD;
	mVendor = js.vendor;
	numAxes = js.axes;
	numButtons = js.buttons;

	ff_effect = 0;
}

//-------------------------------------------------------------------//
LinuxJoyStick::~LinuxJoyStick()
{
	EventUtils::removeForceFeedback( &ff_effect );
}

//-------------------------------------------------------------------//
void LinuxJoyStick::_initialize()
{
	//Clear old joy state
	mState.clear();

	//This will create and new us a force feedback structure if it exists
	EventUtils::enumerateForceFeedback( mJoyStick, &ff_effect );

	if( mJoyStick == -1 )
		OIS_EXCEPT(E_InputDeviceNonExistant, "LinuxJoyStick::_initialize() >> JoyStick Not Found!");
}

//Need to verify/text against many joystick types for best mapping
//OIS Axes
//Stick               0,
//Rotational          1,
//Velocity            2,
//AngularVelocity     3,
//Acceleration        4,
//AngularAcceleration 5,
//Force               6
//Torque              7
//LinuxReports | Name   | OIS AxisNumber | Linux Name Define
//==============================================================
//    0x0        X Axis    0                  ABS_X
//    0x1        Y Axis    0                  ABS_Y
//    0x2        Z Axis    2                  ABS_Z
//    0x3        Rx Axis   1                  ABS_RX
//    0x4        Ry Axis   1                  ABS_RY
//    0x5        Rz Axis   3                  ABS_RZ
//    0x6           ?      4                  ABS_THROTTLE
//    0x7           ?      5                  ABS_RUDDER
//    0x8           ?      6                  ABS_WHEEL
//    0x9           ?      7                  ABS_GAS
//    0xa           ?      7                  ABS_BRAKE
//============================================================
// POV Values
//LinuxReports | Name   | OIS PovNumber  | Linux Name Define
//============================================================
//    0x10  (16)           0                  ABS_HAT0X
//    0x11  (17)           0                  ABS_HAT0Y
//    0x12  (18)           1                  ABS_HAT1X
//    0x13  (19)           1                  ABS_HAT1Y
//    0x14  (20)           2                  ABS_HAT2X
//    0x15  (21)           2                  ABS_HAT2Y
//    0x16  (22)           3                  ABS_HAT3X
//    0x17  (23)           3                  ABS_HAT3Y

#define aX 0
#define aY 1
#define aZ 2

//-------------------------------------------------------------------//
void LinuxJoyStick::capture()
{
	//Look at above table for how these are mapped
	static const short AxisMask[ABS_BRAKE+1] = {0,0,2,1,1,3,4,5,6,7,7};
	//Used for determining if X, Y, or Z component of axis
	static const short XYZMask[ABS_BRAKE+1] = {aX,aY,aZ, aX,aY,aZ, aX, aX, aX, aX,aY};

	static const short POV_MASK[8] = {0,0,1,1,2,2,3,3};

	//Used to determine if an axis has been changed and needs an event
	bool axisMoved[8] = {false,false,false,false,false,false,false,false};
	bool POVMoved[8] = {false,false,false,false,false,false,false,false}; //Four dummy bools

	if( mJoyStick == -1 )
		OIS_EXCEPT(E_General, "LinuxJoyStick::capture() >> device has not been created!");

	input_event js;
	//We are in non blocking mode - we just read once, and try to fill up buffer
	while( read(mJoyStick, &js, sizeof(struct input_event)) == sizeof(struct input_event) )
	{
		switch(js.type)
		{
		//Button
		case EV_KEY:
		{
			//Normalise the button value
			int button;
			//mapping Buttons to OIS button number
			if( js.code < BTN_GAMEPAD )
				js.code = BTN_THUMBR + 1;

			button = js.code - BTN_GAMEPAD;

			if(js.value)
			{
				mState.buttons |= (1 << button);
				if( mBuffered && listener )
					if(!listener->buttonPressed(JoyStickEvent(this,0,mState), button)) return;
			}
			else
			{
				mState.buttons &= ~(1 << button);
				if( mBuffered && listener )
					if(!listener->buttonReleased(JoyStickEvent(this,0,mState), button)) return;
			}
			break;
		}
		//Absoulte Axis
		case EV_ABS:
		{
			//A Stick (BrakeDefine is the highest possible Axis)
			if( js.code <= ABS_BRAKE )
			{
				axisMoved[AxisMask[js.code]] = true;

				if( XYZMask[js.code] == aX )
					mState.mAxes[AxisMask[js.code]].abX = js.value;
				else if( XYZMask[js.code] == aY )
					mState.mAxes[AxisMask[js.code]].abY = js.value;
				else if( XYZMask[js.code] == aZ )
					mState.mAxes[AxisMask[js.code]].abZ = js.value;
			}
			//A POV - Max four POVs allowed
			else if( js.code < ABS_HAT3Y )
			{
				//Normalise the POV to between 0-7
				//Even is X Axis, Odd is Y Axis
				unsigned char LinuxPovNumber = js.code - 16;
				short OIS_POVIndex = POV_MASK[LinuxPovNumber];

				POVMoved[OIS_POVIndex] = true;

				//Handle X Axis first (Even) (left right)
				if((LinuxPovNumber & 0x0001) == 0)
				{
					//Why do this? Because, we use a bit field, and when this axis is east,
					//it can't possibly be west too. So clear out the two X axes, then refil
					//it in with the new direction bit.
					//Clear the East/West Bit Flags first
					mState.mPOV[OIS_POVIndex].direction &= 0x11110011;
					if( js.value == -1 )	//Left
						mState.mPOV[OIS_POVIndex].direction |= Pov::West;
					else if( js.value == 1 ) //Right
						mState.mPOV[OIS_POVIndex].direction |= Pov::East;
				}
				//Handle Y Axis (Odd) (up down)
				else
				{
					//Clear the North/South Bit Flags first
					mState.mPOV[OIS_POVIndex].direction &= 0x11111100;
					if( js.value == -1 )	//Up
						mState.mPOV[OIS_POVIndex].direction |= Pov::North;
					else if( js.value == 1 ) //Down
						mState.mPOV[OIS_POVIndex].direction |= Pov::South;
				}
			}

			#ifdef OIS_LINUX_JOY_DEBUG
			  //Print some additional debug information
			  std::cout << "\nABS Linux Axis# " << js.code << std::endl;
			#endif
			break;
		}
		//Relative Axes (Do any joysticks actually have a relative axis?)
		case EV_REL:
		default: break;
		}
	}

	//All axes and POVs are combined into one movement per pair per captured frame
	if( mBuffered && listener )
	{
		for( int i = 0; i < 8; ++i ) //8 = Max Sticks OIS supports
		{
			if( axisMoved[i] )
				listener->axisMoved( JoyStickEvent(this,0,mState),i);
			if( POVMoved[i] )
				listener->povMoved( JoyStickEvent(this,0,mState),i);
		}
	}
}

//-------------------------------------------------------------------//
void LinuxJoyStick::setBuffered(bool buffered)
{
	if( buffered != mBuffered )
	{
		mBuffered = buffered;
		_initialize();
	}
}

//-------------------------------------------------------------------//
JoyStickInfo LinuxJoyStick::_getJoyInfo()
{
	JoyStickInfo js;

	js.devId = mDevID;
	js.joyFileD = mJoyStick;
	js.vendor = mVendor;
	js.axes = numAxes;
	js.buttons = numButtons;

	return js;
}

//-------------------------------------------------------------------//
JoyStickInfoList LinuxJoyStick::_scanJoys()
{
	JoyStickInfoList joys;

	//Search through all of the event devices.. and identify which ones are joysticks
	//xxx move this to InputManager, as it can also scan all other events
	for(int i = 0; i < 32; ++i )
	{
		std::stringstream s;
		s << "/dev/input/event" << i;
		int fd = open( s.str().c_str(), O_RDONLY |O_NONBLOCK );
        #ifdef OIS_LINUX_JOY_DEBUG
          std::cout << "\nOpening " << s.str() << "...";
        #endif
		try
		{
			JoyStickInfo js;
			if( EventUtils::isJoyStick(fd, js) )
			{
				joys.push_back(js);
                #ifdef OIS_LINUX_JOY_DEBUG
                  std::cout << "\n__Joystick added to list";
                #endif
			}
			else
			{
                #ifdef OIS_LINUX_JOY_DEBUG
                  std::cout << "\n__Not a joystick!!";
                #endif
				close(fd);
			}
		}
		catch(...)
		{
            #ifdef OIS_LINUX_JOY_DEBUG
              std::cout << "\nException caught!!";
            #endif
			close(fd);
		}
	}

	return joys;
}

//-------------------------------------------------------------------//
void LinuxJoyStick::_clearJoys(JoyStickInfoList &joys)
{
	for(JoyStickInfoList::iterator i = joys.begin(); i != joys.end(); ++i)
		close(i->joyFileD);
	joys.clear();
}

//-------------------------------------------------------------------//
Interface* LinuxJoyStick::queryInterface(Interface::IType type)
{
	if( ff_effect && type == Interface::ForceFeedback )
		return ff_effect;

	return 0;
}

#endif //#ifdef OIS_JOYEVENTS
