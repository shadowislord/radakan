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
#include "OISInputManager.h"
#include "OISException.h"

//Bring in correct Header / InputManager for current build platform
#if defined OIS_SDL_PLATFORM
#  include "SDL/SDLInputManager.h"
#elif defined OIS_WIN32_PLATFORM
#  include "win32/Win32InputManager.h"
#elif defined OIS_LINUX_PLATFORM
#  include "linux/LinuxInputManager.h"
#elif defined OIS_APPLE_PLATFORM
#  include "mac/MacInputManager.h"
#elif defined OIS_XBOX_PLATFORM
#  include "xbox/XBoxInputManager.h"
#endif

using namespace OIS;

//quick hack for singleton
InputManager* g_Singleton = 0;

const char* gVersionName = OIS_VERSION_NAME;

//----------------------------------------------------------------------------//
unsigned int InputManager::getVersionNumber()
{
	return OIS_VERSION;
}

//----------------------------------------------------------------------------//
const char* InputManager::getVersionName()
{
	return gVersionName;
}

//----------------------------------------------------------------------------//
InputManager* InputManager::createInputSystem( ParamList &paramList )
{
	if( g_Singleton ) 
		OIS_EXCEPT( E_Duplicate, "An Input Manager Already Exists" );

	InputManager* im = 0;

#if defined OIS_SDL_PLATFORM
	im = new SDLInputManager();
#elif defined OIS_WIN32_PLATFORM
	im = new Win32InputManager();
#elif defined OIS_XBOX_PLATFORM
	im = new XBoxInputManager();
#elif defined OIS_LINUX_PLATFORM
	im = new LinuxInputManager();
#elif defined OIS_APPLE_PLATFORM
	im = new MacInputManager();
#else
	OIS_EXCEPT(E_General, "No platform library.. check build platform defines!");
#endif 

	try
	{
		im->_initialize(paramList);
	}
	catch(...)
	{
		delete im;
		throw; //rethrow
	}

	g_Singleton = im;
	return im;
}

//----------------------------------------------------------------------------//
void InputManager::destroyInputSystem()
{
	delete g_Singleton;
	g_Singleton = 0;
}

//----------------------------------------------------------------------------//
InputManager* InputManager::getSingletonPtr()
{
	return g_Singleton;
}
