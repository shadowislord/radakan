#include "mac/MacMouse.h"
#include "mac/MacInputManager.h"
#include "mac/MacHelpers.h"
#include "OISException.h"
#include "OISEvents.h"

#include <Carbon/Carbon.h>

#include <list>
#include <string>

using namespace OIS;

//-------------------------------------------------------------------//
MacMouse::MacMouse( InputManager* creator, bool buffered ) : mRegainFocus( false )
{
	mCreator = creator;
	mouseEventRef = NULL;
		
	mBuffered = buffered;
	mType = OISMouse;
	
	// Get a "Univeral procedure pointer" for our callback
	mouseUPP = NewEventHandlerUPP( MouseWrapper );
		
	mTempState.X.rel = 0;
	mTempState.Y.rel = 0;
	mTempState.Z.rel = 0;
}

MacMouse::~MacMouse()
{
	if( mouseEventRef != NULL ) RemoveEventHandler( mouseEventRef );
	
	DisposeEventHandlerUPP( mouseUPP );
	
	// Restore Mouse
	CGAssociateMouseAndMouseCursorPosition(TRUE);
	CGDisplayShowCursor(kCGDirectMainDisplay);
}

void MacMouse::_initialize()
{
	mState.clear();
	mRegainFocus = false;

	OSStatus status;
	EventTargetRef event = (( MacInputManager* )mCreator)->_getEventTarget();
		
	if( mouseEventRef != NULL ) RemoveEventHandler( mouseEventRef );
	mouseEventRef = NULL;

	const EventTypeSpec mouseEvents[] =
	{
		{ kEventClassMouse, kEventMouseDown },
		{ kEventClassMouse, kEventMouseUp },
		{ kEventClassMouse, kEventMouseMoved },
		{ kEventClassMouse, kEventMouseDragged },
		{ kEventClassMouse, kEventMouseWheelMoved }		
	};
		
	status = InstallEventHandler( event, mouseUPP, GetEventTypeCount( mouseEvents ), mouseEvents, this, &mouseEventRef );
	if( status != noErr )
	{
		OIS_EXCEPT( E_General, "MacMouse::_initialize >> Error loading Mouse event handler" );
	}
	
	// Hide Mouse
	CGDisplayHideCursor(kCGDirectMainDisplay); //Hide cursor
	//CGDisplayMoveCursorToPoint(kCGDirectMainDisplay, CGPointZero); //Place at display origin
	CGAssociateMouseAndMouseCursorPosition(FALSE);  
}

void MacMouse::setBuffered( bool buffered )
{
	mBuffered = buffered;
}

void MacMouse::capture()
{
	// Check for updates
	if(mState.X.rel == mTempState.X.rel)
	{
		mState.X.rel = 0;
		mTempState.X.rel = 0;
	}
	
	if(mState.Y.rel == mTempState.Y.rel)
	{
		mState.Y.rel = 0;
		mTempState.Y.rel = 0;
	}
	
	if(mState.Z.rel == mTempState.Z.rel)
	{
		mState.Z.rel = 0;
		mTempState.Z.rel = 0;
	}

	// Set the state
	mState.X.rel = mTempState.X.rel;
	mState.Y.rel = mTempState.Y.rel;
		
	if( listener && mBuffered )
		listener->mouseMoved( MouseEvent( this, mState ) );
}

void MacMouse::_mouseCallback( EventRef theEvent )
{
	OSStatus			result = eventNotHandledErr;
    UInt32 				kind = GetEventKind (theEvent);
	// Buttons
	EventMouseButton	button = 0;
	int					mouseButton = 3;
	UInt32				modifiers = 0;
	// Movement
	HIPoint				location = {0.0f, 0.0f};
	HIPoint				delta = {0.0f, 0.0f};
	Rect				clipRect = {0.0f, 0.0f, 0.0f, 0.0f};
	// Wheel
	SInt32				wheelDelta = 0;
	EventMouseWheelAxis	wheelAxis = 0; 

	switch(kind)
	{
		case kEventMouseDragged:
		case kEventMouseMoved:
			// Capture the parameters
			// TODO: Look into HIViewNewTrackingArea
			GetEventParameter(theEvent, kEventParamMouseLocation, typeHIPoint, NULL, sizeof(HIPoint), NULL, &location);
			GetEventParameter(theEvent, kEventParamMouseDelta, typeHIPoint, NULL, sizeof(HIPoint), NULL, &delta);
			
			// Mouse X and Y are the position on the screen, 
			// startng from top-left at 0,0 caps at full monitor resolution
			
			// If we have a window we need to return adjusted coordinates
			// If not, just use raw coordinates
			MacInputManager* im = static_cast<MacInputManager*>(mCreator);
			WindowRef win = im->_getWindow();
			
			if(win != NULL)
			{
				//GetWindowRegion(win, kWindowContentRgn, &contentRgn);
				GetWindowBounds(win, kWindowContentRgn, &clipRect);
			}
			else
			{
				clipRect.right = mState.width;
				clipRect.bottom = mState.height;
			}
			
			// clip the mouse, absolute positioning
			if     (location.x <= clipRect.left)	mState.X.abs = 0;
			else if(location.x >= clipRect.right)	mState.X.abs = clipRect.right - clipRect.left;
			else									mState.X.abs = location.x - clipRect.left;
			
			if     (location.y <= clipRect.top)		mState.Y.abs = 0;
			else if(location.y >= clipRect.bottom)	mState.Y.abs = clipRect.bottom - clipRect.top;
			else									mState.Y.abs = location.y - clipRect.top;
			
			// relative positioning
			mTempState.X.rel = delta.x;
			mTempState.Y.rel = delta.y;
			
			// absolute
			//mState.X.abs = mState.X.abs;
			//mState.Y.abs = mState.Y.abs;
			break;
		case kEventMouseDown:
			// Capture parameters
			GetEventParameter(theEvent, kEventParamMouseButton, typeMouseButton, NULL, sizeof(EventMouseButton), NULL, &button);
			GetEventParameter(theEvent, kEventParamKeyModifiers, typeUInt32, NULL, sizeof(UInt32), NULL, &modifiers);
			
			if((button == kEventMouseButtonTertiary) || ((button == kEventMouseButtonPrimary) && (modifiers & optionKey)))
			{
				mouseButton = 2;
				mState.buttons |= 1 << mouseButton;
			}
			else if((button == kEventMouseButtonSecondary) || ((button == kEventMouseButtonPrimary) && (modifiers & controlKey)))
			{	
				mouseButton = 1;
				mState.buttons |= 1 << mouseButton;
			}
			else if(button == kEventMouseButtonPrimary)
			{
				mouseButton = 0;
				mState.buttons |= 1 << mouseButton;
			}
			if( listener && mBuffered )
				listener->mousePressed( MouseEvent( this, mState ), (MouseButtonID)mouseButton );
			break;
		case kEventMouseUp:
			// Capture parameters
			GetEventParameter(theEvent, kEventParamMouseButton, typeMouseButton, NULL, sizeof(EventMouseButton), NULL, &button);
			GetEventParameter(theEvent, kEventParamKeyModifiers, typeUInt32, NULL, sizeof(UInt32), NULL, &modifiers);
			
			if ((button == kEventMouseButtonTertiary) || ((button == kEventMouseButtonPrimary) && (modifiers & optionKey)))
			{
				mouseButton = 2;
				mState.buttons &= ~(1 << mouseButton);
			}
			else if ((button == kEventMouseButtonSecondary) || ((button == kEventMouseButtonPrimary) && (modifiers & controlKey)))
			{	
				mouseButton = 1;
				mState.buttons &= ~(1 << mouseButton);
			}
			else if (button == kEventMouseButtonPrimary)
			{
				mouseButton = 0;
				mState.buttons &= ~(1 << mouseButton);
			}
			if( listener && mBuffered )
				listener->mouseReleased( MouseEvent( this, mState ), (MouseButtonID)mouseButton );
			break;
		case kEventMouseWheelMoved:
			// Capture parameters
			GetEventParameter(theEvent, kEventParamMouseWheelAxis, typeMouseWheelAxis, NULL, sizeof(EventMouseWheelAxis), NULL, &wheelAxis);
			GetEventParameter(theEvent, kEventParamMouseWheelDelta, typeSInt32, NULL, sizeof(SInt32), NULL, &wheelDelta);
			
			// If the Y axis of the wheel changed, then update the Z
			// Does OIS care about the X wheel axis?
			if(wheelAxis == kEventMouseWheelAxisY)
			{
				mTempState.Z.rel = wheelDelta;
				mState.Z.abs += wheelDelta;
			}
			break;
		default:
			break;
	}
}

void MacMouse::injectEvent( MouseState state, MacEventType type, MouseButtonID button )
{
	 if ( mBuffered && listener ) {
        pendingEvents.push_back( MacMouseStackEvent( MouseEvent(this, state), type, button) );
    }
}
