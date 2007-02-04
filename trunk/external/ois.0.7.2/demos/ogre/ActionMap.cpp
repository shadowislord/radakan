#include "ActionMap.h"
#include "OISMouse.h"
#include "OISKeyboard.h"
#include "OISJoyStick.h"

using namespace OIS;

//---------------------------------------------------------------------------//
ActionMap::~ActionMap()
{
	setEnabled(false);
}

//---------------------------------------------------------------------------//
void ActionMap::setCallbacks( KeyListener* kl, MouseListener* ml )
{
	keyCallback = kl;
	mouseCallback = ml;
}

//---------------------------------------------------------------------------//
void ActionMap::setEnabled( bool enabled )
{
	if( enabled )
	{
		if( mKey ) mKey->setEventCallback( this );
		if( mMouse ) mMouse->setEventCallback( this );
		if( mJoy ) mJoy->setEventCallback( this );
	}
	else
	{
		if( mKey ) mKey->setEventCallback(0);
		if( mMouse ) mMouse->setEventCallback(0);
		if( mJoy ) mJoy->setEventCallback(0);
	}
}

//---------------------------------------------------------------------------//
void ActionMap::reset()
{
	mouseButtonEvents.clear();
	mouseXY.setProperties(-1, -1, OIS_Button, 0);
	mouseZ.setProperties(-1, -1, OIS_Button, 0);

	joyButtonEvents.clear();
	joyAxisEvents.clear();
}

//---------------------------------------------------------------------------//
void ActionMap::addKeyboardAction(int iID, int iTrigger, ActionBind *pBind)
{
	keyEvents[iTrigger] = ActionEventInfo(iID, iTrigger, OIS_Button, pBind);
}

//---------------------------------------------------------------------------//
void ActionMap::addMouseAction(int iID, int iTrigger, ComponentType type, ActionBind *pBind)
{
	if( type == OIS_Axis )
	{
		if( iTrigger == 0 )	//XY Axis
			mouseXY.setProperties( iID, iTrigger, type, pBind );
		else			//Z Axis
			mouseZ.setProperties( iID, iTrigger, type, pBind );
	}
	else
		mouseButtonEvents[iTrigger] = ActionEventInfo(iID, iTrigger, type, pBind);
}

//---------------------------------------------------------------------------//
void ActionMap::addJoyStickAction(int iID, int iTrigger, ComponentType type, ActionBind *pBind)
{
	if( type == OIS_Button )
		joyButtonEvents[iTrigger] = ActionEventInfo(iID, iTrigger, type, pBind);
	else if( type == OIS_Axis )
		joyAxisEvents[iTrigger] = ActionEventInfo(iID, iTrigger, type, pBind);
	else if( type == OIS_POV )
		joyPovEvents[iTrigger] = ActionEventInfo(iID, iTrigger, type, pBind);
}

//---------------------------------------------------------------------------//
bool ActionMap::keyPressed(const KeyEvent& arg)
{
	ActionList::iterator i = keyEvents.find(arg.key);
	if( i != keyEvents.end() )
		i->second.bound->callMethod( ActionMapArg(i->second.id,&Button(true)) );

	if( keyCallback )
		return keyCallback->keyPressed(arg);
	else
		return true;
}

//---------------------------------------------------------------------------//
bool ActionMap::keyReleased(const KeyEvent& arg)
{
	ActionList::iterator i = keyEvents.find(arg.key);
	if( i != keyEvents.end() )
		i->second.bound->callMethod(ActionMapArg(i->second.id,&Button(false)));

	if( keyCallback )
		return keyCallback->keyReleased(arg);
	else
		return true;
}

//---------------------------------------------------------------------------//
bool ActionMap::mouseMoved(const MouseEvent& arg)
{
	if( arg.state.relZ == 0 )	//XY Axis
	{
		if( mouseXY.trigger != -1 )
		{
			Axis axis(arg.state);
			axis.abZ = axis.relZ = 0; //Zero out Z axis, as that is seperate
			mouseXY.bound->callMethod(ActionMapArg(mouseXY.id,&axis));
		}
	}
	else				//Z Axis
	{
		if( mouseZ.trigger != -1 )
		{
			Axis axis;
			axis.abZ = arg.state.abZ;
			axis.relZ = arg.state.relZ;
			mouseZ.bound->callMethod(ActionMapArg(mouseZ.id,&axis));
		}
	}

	if( mouseCallback )
		return mouseCallback->mouseMoved(arg);
	else
		return true;
}

//---------------------------------------------------------------------------//
bool ActionMap::mousePressed(const MouseEvent& arg, MouseButtonID button)
{
	ActionList::iterator i = mouseButtonEvents.find(button);
	if( i != mouseButtonEvents.end() )
		i->second.bound->callMethod(ActionMapArg(i->second.id,&Button(true)));

	if( mouseCallback )
		return mouseCallback->mousePressed(arg, button);
	else
		return true;
}

//---------------------------------------------------------------------------//
bool ActionMap::mouseReleased(const MouseEvent& arg, MouseButtonID button)
{
	ActionList::iterator i = mouseButtonEvents.find(button);
	if( i != mouseButtonEvents.end() )
		i->second.bound->callMethod(ActionMapArg(i->second.id,&Button(false)));

	if( mouseCallback )
		return mouseCallback->mouseReleased(arg, button);
	else
		return true;
}

//---------------------------------------------------------------------------//
bool ActionMap::axisMoved(const JoyStickEvent &arg, int axis)
{
	ActionList::iterator i = joyAxisEvents.find(axis);
	if( i != joyAxisEvents.end() )
		i->second.bound->callMethod(ActionMapArg(i->second.id,
			&arg.state.mAxes[axis]));

	return true;
}

//---------------------------------------------------------------------------//
bool ActionMap::povMoved(const JoyStickEvent &arg, int pov)
{
	ActionList::iterator i = joyPovEvents.find(pov);
	if( i != joyPovEvents.end() )
		i->second.bound->callMethod(ActionMapArg(i->second.id,&Pov(arg.state.mPOV[pov])));

	return true;
}

//---------------------------------------------------------------------------//
bool ActionMap::buttonPressed(const JoyStickEvent &arg, int button)
{
	ActionList::iterator i = joyButtonEvents.find(button);
	if( i != joyButtonEvents.end() )
		i->second.bound->callMethod(ActionMapArg(i->second.id,&Button(true)));

	return true;
}

//---------------------------------------------------------------------------//
bool ActionMap::buttonReleased(const JoyStickEvent& arg, int button)
{
	ActionList::iterator i = joyButtonEvents.find(button);
	if( i != joyButtonEvents.end() )
		i->second.bound->callMethod(ActionMapArg(i->second.id,&Button(false)));

	return true;
}
