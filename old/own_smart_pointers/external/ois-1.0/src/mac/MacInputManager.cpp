/*
 The zlib/libpng License
 
 Copyright (c) 2006 Chris Snyder 
 
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
#include "mac/MacInputManager.h"
#include "mac/MacKeyboard.h"
#include "mac/MacMouse.h"
#include "OISException.h"

#include <Carbon/Carbon.h>
#include <string>

//#include <iostream>
//using namespace std;

using namespace OIS;
const std::string MacInputManager::iName = "Mac OSX Input Manager";

//--------------------------------------------------------------------------------//
MacInputManager::MacInputManager()
{
    mHideMouse = true;
    mUseRepeat = false;
	mEventTargetRef = NULL;
	mWindow = NULL;
}


//--------------------------------------------------------------------------------//
MacInputManager::~MacInputManager()
{

}

//--------------------------------------------------------------------------------//
void MacInputManager::_initialize(ParamList &paramList)
{
	_parseConfigSettings( paramList );
    
	//Enumerate all devices attached
	_enumerateDevices();
}

//--------------------------------------------------------------------------------//
void MacInputManager::_parseConfigSettings( ParamList &paramList )
{
	// Some carbon apps are running in a window, however full screen apps
	// do not have a window, so we need to account for that too.
	if(paramList.find("WINDOW") != paramList.end())
	{
		if(paramList.find("WINDOW")->second == "0")
		{
			mWindow = NULL;
			mEventTargetRef = GetApplicationEventTarget();
		}
		else
		{
			//TODO 64 bit proof this little conversion xxx wip
			mWindow = (WindowRef) strtoul(paramList.find("WINDOW")->second.c_str(), 0, 10);
			//mEventTargetRef = GetWindowEventTarget(mWindow);
			mEventTargetRef = GetApplicationEventTarget();
		}
    }
	else
	{
		// else get the main active window.. user might not have access to it through some
		// graphics libraries, if that fails then try at the application level.
		mWindow = ActiveNonFloatingWindow();
		if(mWindow == NULL)
		{
			mEventTargetRef = GetApplicationEventTarget();
		}
		else
		{
			//mEventTargetRef = GetWindowEventTarget(mWindow);
			mEventTargetRef = GetApplicationEventTarget();
		}
	}
	
	if(mEventTargetRef == NULL)
	{
		OIS_EXCEPT( E_General, "MacInputManager::_parseConfigSettings >> Unable to find a window or event target" );
	}
    
    // Keyboard
    if(paramList.find("MacAutoRepeatOn") != paramList.end())
	{
        if(paramList.find("MacAutoRepeatOn")->second == "true")
		{
            mUseRepeat = true;
        }
    }	
}

//--------------------------------------------------------------------------------//
void MacInputManager::_enumerateDevices()
{
	// If HID is ever supported, this would be needed.
}

//--------------------------------------------------------------------------------//
int MacInputManager::numJoySticks()
{
	return 0;
}

//--------------------------------------------------------------------------------//
int MacInputManager::numMice()
{
	return 1;
}

//--------------------------------------------------------------------------------//
int MacInputManager::numKeyboards()
{
	return 1;
}

//----------------------------------------------------------------------------//
Object* MacInputManager::createInputObject(Type iType, bool bufferMode)
{
	Object* obj = 0;
	
	switch( iType )
	{
		case OISKeyboard: 
			obj = new MacKeyboard(this, bufferMode, mUseRepeat); 
			break;
		case OISMouse:
			obj = new MacMouse(this, bufferMode);
			break;
		case OISJoyStick:
			return obj;
			break;
		default:
			return obj;
			break;
	}
    
	//Seperate initialization from construction.. as bad things happen when
	//throwing exceptions from constructors
	try
	{
		obj->_initialize();
    }
	catch(...) 
	{
		delete obj;
		throw; //rethrow the exception
	}
    
	return obj;
}

//----------------------------------------------------------------------------//
void MacInputManager::destroyInputObject(Object* obj)
{
	if(obj)
    {
		delete obj;
    }
}
