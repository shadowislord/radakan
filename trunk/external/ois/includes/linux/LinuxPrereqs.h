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
#ifndef _LINUX_INPUTSYSTEM_PREREQS_H
#define _LINUX_INPUTSYSTEM_PREREQS_H


//! Max number of elements to collect from buffered input
#define LI_BUFFERSIZE 6

namespace OIS
{
	class LinuxInputManager;
	class LinuxKeyboard;
	class LinuxJoyStick;
	class LinuxMouse;
	
	class LinuxForceFeedback;

	struct JoyStickInfo
	{
		JoyStickInfo(): devId(-1),joyFileD(-1),version(0),axes(0),buttons(0),hats(0) {}
		//! Device number (/dev/input/j#) or /dev/input/event#
		int devId;
		//! File descriptor
		int joyFileD;
		//! Driver version
		int version;
		//! Joy vendor
		std::string vendor;
		unsigned char axes;
		unsigned char buttons;
		unsigned char hats;
	};

	typedef std::vector< JoyStickInfo > JoyStickInfoList;
}

#endif //_LINUX_INPUTSYSTEM_PREREQS_H
