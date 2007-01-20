/*
The zlib/libpng License

Copyright (c) 2005 Phillip Castaneda (pjcast -- www.wreckedgames.com)

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
#ifndef _OIS_ACTIONMAPHEADERS_
#define _OIS_ACTIONMAPHEADERS_
#include "OISPrereqs.h"
#include "OISJoyStick.h"
#include "OISMouse.h"
#include "OISKeyboard.h"

namespace OIS
{
	//*********** Base binding class *****************
	template <typename returnT, typename args>
	class IBind
	{
	public:
		virtual returnT operator()(args) const = 0;
		virtual ~IBind() {}
	};

	//******** Implement of binding for member funtions
	template <class T, typename returnT, typename Args>
	class memberBind : public IBind<returnT, Args>
	{
	public:
		typedef returnT(T::*F)(Args);
		memberBind(F function, T* object) :
		mFunction(function), mObject(object) {}

		virtual returnT operator()(Args args) const {return (mObject->*mFunction)(args);}
	protected:
		F  mFunction;
		T* mObject;
	};

	//***** Generic Binder (WIP) **************
	template <typename returnT, typename Args>
	class Binder
	{
		Binder() : mObject(0) {}
	public:
		~Binder() { delete mObject; }

		//Member Binding
		template <class T>
		Binder(returnT (T::*function)(Args), T* object) {
			mObject = new memberBind<T, returnT, Args>(function, object);
		}
		//Todo regular function binding

		returnT callMethod(Args args) const { return (*mObject)(args); }

	protected:
		IBind<returnT, Args>* mObject;
	};

	/**
		Class used as parameter for handing out ActionMap Events
	*/
	class ActionMapArg
	{
	public:
		ActionMapArg(int id, const Component* comp) :
			ActionID(id), component(comp) {};

		//! The ActionID
		int ActionID;

		//! The component (button, Axis, POV, etc)
		const Component* component;
	};

	//! function pointer that returns void and takes 'const ActionMapArg&' param
	typedef Binder<void, const ActionMapArg&> ActionBind;

	/**
		Utility structure used for storing event info
	*/
	struct ActionEventInfo
	{
		//! Default constructor
		ActionEventInfo() : id(0), trigger(-1), cType(OIS_Button), bound(0) {};

		/**
		@remarks
			Constructor
		@param iID
			ActionID
		@param iTrigger
			Trigger - KeyCode, Axis num, etc
		@param bButton
			True - button; false - other (axis)
		@param pBind
			Function pointer
		*/
		ActionEventInfo( int iID, int iTrigger, ComponentType type, ActionBind* pBind ) : 
			id(iID), trigger(iTrigger), cType(type), bound(pBind) {};

		//! Utility method for setting up members
		void setProperties( int iID, int iTrigger, ComponentType type, ActionBind* pBind )
		{
			id = iID;
			trigger = iTrigger;
			cType = type;
			bound = pBind;
		}

		//! Action ID Numbercd
		int  id;
		//! KeyCode/Axis number/Button number/etc
		int  trigger;
		//! Type of component
		ComponentType cType;
		//! A function pointer for event callback
		ActionBind* bound;
	};

	/**
		Utility class for dispatching events based on Actions.
		@Note Buttons get two events for pushed/released. 
		@See ActionMapArg for info on whether or not the button is pushed or not
	*/
	class ActionMap : public KeyListener, public MouseListener, public JoyStickListener
	{
		ActionMap() {};
	public:
		/**
		@remarks
			Constructs ActionMap class
		@param kb
			Pointer to a keyboard device for events. Or null for no key device
		@param mouse
			Pointer to a mouse device for events. Or null for no mouse device
		@param joy
			Pointer to a joystick device for events. Or null for no joy device
		*/
		ActionMap( Keyboard* kb, Mouse* mouse, JoyStick* joy ) : 
			mKey(kb), mMouse(mouse), mJoy(joy), 
			keyCallback(0), mouseCallback(0) {};

		virtual ~ActionMap();

		/**
		@remarks
			If you want key and/or mouse events to also 'pass' through back to 
			your listener, pass in your listeners here
		@param kl
			KeyListener
		@param ml
			MouseListener
		*/
		void setCallbacks( KeyListener* kl, MouseListener* ml );

		/**
		@remarks
			If you want key and/or mouse events to also 'pass' through back to 
			your listener, pass in your listeners here. Setting this true
			will replace your listeners (of the valid devices you passed in earlier).
			If you want to continue getting your own events too, refer to setCallbacks
		@param enabled
			true - start recieving/passing events. False - stop
		*/
		void setEnabled( bool enabled );

		/**
		@remarks
			Clears out all event lists
		*/
		void reset();

		/**
		@remarks
			Add an event for keyboard device
		@param iID
			ActionID number you want to pass through
		@param iTrigger
			The actual trigger value (KeyCode, Button number, Axis number). Must be 			unique for each button/axis of the same device (i.e. cannot register
			two events for key '2').
		@param pBind
			A valid function pointer used to call for event
		*/
		void addKeyboardAction(int iID, int iTrigger, ActionBind *pBind);

		/**
		@remarks
			Add an event for mouse device
		@param iID
			ActionID number you want to pass through
		@param iTrigger
			The actual trigger value (KeyCode, Button number, Axis number). Must be 			unique for each button/axis of the same device (i.e. cannot register
			two events for button '0' left mouse).
		@param type
			type of component
		@param pBind
			A valid function pointer used to call for event
		*/
		void addMouseAction(int iID, int iTrigger, ComponentType type, ActionBind *pBind);

		/**
		@remarks
			Add an event for joystick device
		@param iID
			ActionID number you want to pass through
		@param iTrigger
			The actual trigger value (KeyCode, Button number, Axis number). Must be 			unique for each button/axis of the same device (i.e. cannot register
			two events for button '0').
		@param type
			type of component
		@param pBind
			A valid function pointer used to call for event
		*/
		void addJoyStickAction(int iID, int iTrigger, ComponentType type, ActionBind *pBind);

	protected:
		//------ Buffered Device events ------------------------
		bool keyPressed(const KeyEvent& arg);
		bool keyReleased(const KeyEvent&);

		bool mouseMoved(const MouseEvent& arg);
		bool mousePressed(const MouseEvent& arg, MouseButtonID);
		bool mouseReleased(const MouseEvent&, MouseButtonID);

		bool axisMoved(const JoyStickEvent &arg, int axis);
		bool povMoved(const JoyStickEvent &arg, int pov);
		bool buttonPressed(const JoyStickEvent &arg, int button);
		bool buttonReleased(const JoyStickEvent&, int);

		//! Typedef for easy iteration int = Trigger number
		typedef std::map<int, ActionEventInfo> ActionList;

		//! Keyboard events
		ActionList keyEvents;

		//! Mouse button events
		ActionList mouseButtonEvents;
		//! Mouse XY axes events
		ActionEventInfo mouseXY;
		//! Mouse Z (wheel) axis events
		ActionEventInfo mouseZ;

		//! Joy button events
		ActionList joyButtonEvents;
		//! Joy axes events
		ActionList joyAxisEvents;
		//! Joy POV events
		ActionList joyPovEvents;

		//! Keyboard device
		Keyboard* mKey;
		//! Mouse device
		Mouse* mMouse;
		//! JoyStick device
		JoyStick* mJoy;

		//! Keyboard callback pointer used to pass through events
		KeyListener* keyCallback;
		//! Mouse callback pointer used to pass through events
		MouseListener* mouseCallback;
	};
}
#endif
