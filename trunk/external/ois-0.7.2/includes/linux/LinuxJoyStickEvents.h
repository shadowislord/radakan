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
#ifndef _LINUX_JOYSTICK_H_EADER_
#define _LINUX_JOYSTICK_H_EADER_

//Check to see which JoyStick support is being compiled in
#include "OISConfig.h"
#ifdef OIS_JOYEVENTS

#include "OISJoyStick.h"
#include "linux/LinuxPrereqs.h"

namespace OIS
{
	/**
		Linux specialization of JoyStick class.. This version is favored over the other.. and has the
		*possibility* of Force Feedback.. notice I say possibility, i make no gaurantees under linux,
		as FF support is sketchy at best AFAIK.
	*/
	class LinuxJoyStick : public JoyStick
	{
	public:
		LinuxJoyStick(bool buffered, const JoyStickInfo& js);
		virtual ~LinuxJoyStick();
		
		/** @copydoc Object::setBuffered */
		virtual void setBuffered(bool buffered);
		
		/** @copydoc Object::capture */
		virtual void capture();

		/** @copydoc Object::queryInterface */
		virtual Interface* queryInterface(Interface::IType type);
		
		/** @copydoc Object::_initialize */
		virtual void _initialize();
 
		/**
		@remarks
			For internal use only... Returns a strucure to the manager, to make the device
			availiable for use again
		*/
		JoyStickInfo _getJoyInfo();

		static JoyStickInfoList _scanJoys();
		static void _clearJoys(JoyStickInfoList &joys);
	protected:

		int mJoyStick;
		LinuxForceFeedback* ff_effect;
	};
}

#endif //defined OIS_JOYEVENTS
#endif //_LINUX_JOYSTICK_H_EADER_
