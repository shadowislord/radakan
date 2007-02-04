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
#ifndef OIS_JOYEVENTS

#include "linux/LinuxJoyStick.h"
#include "linux/LinuxInputManager.h"
#include "OISEvents.h"
#include "OISException.h"

#include <fcntl.h>		//Needed to Open A file descriptor
#include <linux/joystick.h>

#include <sstream>

using namespace OIS;

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

	listener = 0;
}

//-------------------------------------------------------------------//
LinuxJoyStick::~LinuxJoyStick()
{
}

//-------------------------------------------------------------------//
void LinuxJoyStick::_initialize()
{
	//Clear old joy state
	mState.clear();

	if( mJoyStick == -1 )
		OIS_EXCEPT(E_InputDeviceNonExistant, "LinuxJoyStick::_initialize() >> JoyStick Not Found!");
}
#include <iostream>
//-------------------------------------------------------------------//
void LinuxJoyStick::capture()
{
	if( mJoyStick == -1 )
		OIS_EXCEPT(E_General, "LinuxJoyStick::capture() >> device has not been created!");

//Current Mapping For my joystick.. may need to be different for other controllers
//xxx Look into making this in a configurable file which will be loaded
//LinuxReports / Name / ActualAxisNumber (As we refer to axis with XY - Axes really)
//0 		 X Axis 0
//1 		 Y Axis 0
//2 		 Z Axis 2
//3 		 X Axis 1
//4 		 Y Axis 1
//5 		 Z Axis 3
//6 		 D PadX 4
//7 		 D PadY 4
	int AxisMask[8] = {0,0,2,1,1,3,4,4};
	int XYZMask[8] = {0,1,2,0,1,2,0,1};
	bool axisMoved[8] = {false,false,false,false,false,false,false,false};

	js_event js;
	while (read(mJoyStick, &js, sizeof(struct js_event)) == sizeof(struct js_event))
	{
		switch(js.type & ~JS_EVENT_INIT)
		{
		case JS_EVENT_BUTTON:
			if(js.value)
			{
				mState.buttons |= (1 << js.number);
				if( mBuffered && listener ) if(!listener->buttonPressed(JoyStickEvent(this,js.time,mState), js.number)) return;
			}
			else
			{
				mState.buttons &= ~(1 << js.number);
				if( mBuffered && listener ) if(!listener->buttonReleased(JoyStickEvent(this,js.time,mState), js.number)) return;
			}
			break;
		case JS_EVENT_AXIS:
			axisMoved[AxisMask[js.number]] = true;

			if( XYZMask[js.number] == 0 )      //X Value
				mState.mAxes[AxisMask[js.number]].abX = js.value;
			else if( XYZMask[js.number] == 1 ) //Y Value
				mState.mAxes[AxisMask[js.number]].abY = js.value;
			else if( XYZMask[js.number] == 2 ) //Z Value
				mState.mAxes[AxisMask[js.number]].abZ = js.value;
			break;
		}
	}

	for( int i = 0; i < 8; ++i )
		if( axisMoved[i] && mBuffered && listener )
			listener->axisMoved( JoyStickEvent(this,js.time,mState),i);
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
#include <errno.h>
#include <iostream>
//-------------------------------------------------------------------//
JoyStickInfoList LinuxJoyStick::_scanJoys()
{
	using namespace std;
	JoyStickInfoList joys;

	//Search for max amount of JoySticks
	for(int i = 0; i < OIS_MAX_DEVICES; ++i )
	{
		stringstream s;
		s << "/dev/input/js" << i;
		int joyStick = open(s.str().c_str(), O_RDONLY | O_NONBLOCK, 0);

		if( joyStick != -1 )
		{
			JoyStickInfo js;
			char nameBuff[OIS_DEVICE_NAME];

			if( ioctl(joyStick, JSIOCGNAME(OIS_DEVICE_NAME), nameBuff) < 0 )
				js.vendor = "Generic joyStick";
			else
				js.vendor = nameBuff;
			js.devId = i;
			js.joyFileD = joyStick;
			ioctl(joyStick, JSIOCGVERSION, &(js.version));
			ioctl(joyStick, JSIOCGAXES, &(js.axes));
			ioctl(joyStick, JSIOCGBUTTONS, &(js.buttons));
			joys.push_back(js);

			//Clear out any old data
			js_event hack;
			while( read(joyStick, &hack, sizeof(struct js_event)) ==
				sizeof(struct js_event)) {}
		}
		else
			break;
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

#endif //#ifndef OIS_JOYEVENTS
