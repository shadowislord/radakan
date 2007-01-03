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
#include "Win32/Win32Mouse.h"
#include "Win32/Win32InputManager.h"
#include "OISException.h"
#include "OISEvents.h"

using namespace OIS;

//-------------------------------------------------------------------//
Win32Mouse::Win32Mouse( IDirectInput8* pDI, bool buffered, DWORD coopSettings )
{
	mBuffered = buffered;
	mMouse = 0;
	mDirectInput = pDI;
	coopSetting = coopSettings;
	mType = OISMouse;
	listener = 0;
}

//-------------------------------------------------------------------//
void Win32Mouse::_initialize()
{
	DIPROPDWORD dipdw;

	//Clear old state
	mState.clear();

    dipdw.diph.dwSize       = sizeof(DIPROPDWORD);
    dipdw.diph.dwHeaderSize = sizeof(DIPROPHEADER);
    dipdw.diph.dwObj        = 0;
    dipdw.diph.dwHow        = DIPH_DEVICE;
	dipdw.dwData            = MOUSE_DX_BUFFERSIZE;
	
	if( FAILED(mDirectInput->CreateDevice(GUID_SysMouse, &mMouse, NULL)) )
		OIS_EXCEPT( E_General, "Win32Mouse::Win32Mouse >> Failed to create device" );

	if( FAILED(mMouse->SetDataFormat(&c_dfDIMouse2)) )
		OIS_EXCEPT( E_General, "Win32Mouse::Win32Mouse >> Failed to set format" );
	
	HWND hwin = ((Win32InputManager*)(InputManager::getSingletonPtr()))->getWindowHandle();

	if( FAILED(mMouse->SetCooperativeLevel(hwin, coopSetting)) )
		OIS_EXCEPT( E_General, "Win32Mouse::Win32Mouse >> Failed to set coop level" );
	
	if( FAILED(mMouse->SetProperty(DIPROP_BUFFERSIZE, &dipdw.diph )) )
		OIS_EXCEPT( E_General, "Win32Mouse::Win32Mouse >> Failed to set property" );

	HRESULT hr = mMouse->Acquire();
	if (FAILED(hr) && hr != DIERR_OTHERAPPHASPRIO)
		OIS_EXCEPT( E_General, "Win32Mouse::Win32Mouse >> Failed to aquire mouse!" );
}

//-------------------------------------------------------------------//
Win32Mouse::~Win32Mouse()
{
	if (mMouse)
	{
		mMouse->Unacquire();
		mMouse->Release();
		mMouse = 0;
	}
}

//-------------------------------------------------------------------//
void Win32Mouse::capture()
{
	//Clear old relative values
	mState.relX = mState.relY = mState.relZ = 0;

	DIDEVICEOBJECTDATA diBuff[MOUSE_DX_BUFFERSIZE];
	DWORD entries = MOUSE_DX_BUFFERSIZE;

	HRESULT hr = mMouse->GetDeviceData( sizeof(DIDEVICEOBJECTDATA), diBuff, &entries, 0 );
	if( hr != DI_OK )
	{
		hr = mMouse->Acquire();
		while( hr == DIERR_INPUTLOST ) 
			hr = mMouse->Acquire();

		hr = mMouse->GetDeviceData( sizeof(DIDEVICEOBJECTDATA), diBuff, &entries, 0 );
		
		//Perhaps the user just tabbed away, and coop settings
		//are nonexclusive..so just ignore
		if( FAILED(hr) )
			return;
	}

	bool axesMoved = false;
	//Accumulate all axis movements for one axesMove message..
	//Buttons are fired off as they are found
	for(unsigned int i = 0; i < entries; ++i )
	{
		switch( diBuff[i].dwOfs )
		{
			case DIMOFS_BUTTON0: if(!_doMouseClick(0, diBuff[i])) return; break;
			case DIMOFS_BUTTON1: if(!_doMouseClick(1, diBuff[i])) return; break;
			case DIMOFS_BUTTON2: if(!_doMouseClick(2, diBuff[i])) return; break;
			case DIMOFS_BUTTON3: if(!_doMouseClick(3, diBuff[i])) return; break;
			case DIMOFS_BUTTON4: if(!_doMouseClick(4, diBuff[i])) return; break;	
			case DIMOFS_BUTTON5: if(!_doMouseClick(5, diBuff[i])) return; break;
			case DIMOFS_BUTTON6: if(!_doMouseClick(6, diBuff[i])) return; break;	
			case DIMOFS_BUTTON7: if(!_doMouseClick(7, diBuff[i])) return; break;	
			case DIMOFS_X:
				mState.relX += diBuff[i].dwData;
				axesMoved = true;
				break;
			case DIMOFS_Y:
				mState.relY += diBuff[i].dwData;
				axesMoved = true;
				break;
			case DIMOFS_Z:
				mState.relZ += diBuff[i].dwData;
				axesMoved = true;
				break;
			default: break;
		} //end switch
	}//end for

	if( axesMoved )
	{
		if( coopSetting & DISCL_NONEXCLUSIVE )
		{
			//DirectInput provides us with meaningless values, so correct that
			HWND hwin = ((Win32InputManager*)(InputManager::getSingletonPtr()))->getWindowHandle();
			POINT point;
			GetCursorPos(&point);
			ScreenToClient(hwin, &point);
			mState.abX = point.x;
			mState.abY = point.y;
		}
		else
		{
			mState.abX +=  mState.relX;
			mState.abY +=  mState.relY;
		}
		mState.abZ +=  mState.relZ;

		//Clip values to window
		if( mState.abX < 0 )
			mState.abX = 0;
		else if( mState.abX > mState.width )
			mState.abX = mState.width;
		if( mState.abY < 0 )
			mState.abY = 0;
		else if( mState.abY > mState.height )
			mState.abY = mState.height;

		_doMouseMove(diBuff[entries-1].dwTimeStamp);
	}
}

//-------------------------------------------------------------------//
bool Win32Mouse::_doMouseClick( int mouseButton, DIDEVICEOBJECTDATA& di )
{
	if( di.dwData & 0x80 )
	{
		mState.buttons |= 1 << mouseButton; //turn the bit flag on
		if( listener && mBuffered )
			return listener->mousePressed( MouseEvent( this, di.dwTimeStamp, mState ), (MouseButtonID)mouseButton );
	}
	else
	{
		mState.buttons &= ~(1 << mouseButton); //turn the bit flag off
		if( listener && mBuffered )
			return listener->mouseReleased( MouseEvent( this, di.dwTimeStamp, mState ), (MouseButtonID)mouseButton );
	}

	return true;
}

//-------------------------------------------------------------------//
bool Win32Mouse::_doMouseMove( unsigned int tStamp )
{
	
	if( listener && mBuffered )
		return listener->mouseMoved( MouseEvent( this, tStamp, mState ) );

	return true;
}

//-------------------------------------------------------------------//
void Win32Mouse::setBuffered(bool buffered)
{
	mBuffered = buffered;
}
