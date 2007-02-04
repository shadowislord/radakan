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
#include <CEGUI/CEGUIImageset.h>
#include <CEGUI/CEGUISystem.h>
#include <CEGUI/CEGUILogger.h>
#include <CEGUI/CEGUISchemeManager.h>
#include <CEGUI/CEGUIWindowManager.h>
#include <CEGUI/CEGUIWindow.h>
#include <CEGUI/elements/CEGUIMultiColumnList.h>
#include <CEGUI/elements/CEGUIListboxTextItem.h>

#include "OgreSceneManager.h"
#include "OgreCamera.h"
#include "OgreRoot.h"

#include "ActionConfig.h"
#include "OISMouse.h"
#include "OISKeyboard.h"
#include "OISJoyStick.h"

//Some Simple Configurable Action IDs
const int A_JUMP			= 1;	//All
const int A_FIRE			= 2;	//All
const int A_MOVE			= 3;	//Only used for mouse and joy Move
const int A_MOVE_FORWARD	= 4;	//Keyboard only setting
const int A_MOVE_BACKWARD	= 5;	//Keyboard only setting
const int A_MOVE_LEFT		= 6;	//Keyboard only setting
const int A_MOVE_RIGHT		= 7;	//Keyboard only setting
const int A_LOOK			= 8;	//Only used for mouse and joy move
const int A_LOOK_UP			= 9;	//Keyboard only setting
const int A_LOOK_DOWN		= 10;	//Keyboard only setting
const int A_LOOK_LEFT		= 11;	//Keyboard only setting
const int A_LOOK_RIGHT		= 12;	//Keyboard only setting
const int A_CENTER_VIEW		= 13;	//All

const int A_TEST_DONE		= 14;	//Action that happens when Escape Pushed


//--------------------------------------------------------------------------------//
ActionConfig::ActionConfig( OIS::Mouse *mouse, OIS::Keyboard *key, OIS::JoyStick *joy,
			    Ogre::SceneManager* scnMgr ) :
	mMouse(mouse), mKey(key), mJoy(joy), mSceneMgr(scnMgr)
{
	//Create an ActionMap instance
	actionMap = new OIS::ActionMap(mKey, mMouse, mJoy);

	using namespace CEGUI;
	WindowManager& wmgr = WindowManager::getSingleton();

	oldKeyListener = mKey->getEventCallback();
	oldMouseListener = mMouse->getEventCallback();

	//One time MultiList Setup
	MultiColumnList *listBox;

	//Keyboard list
	listBox = (MultiColumnList*)wmgr.getWindow((utf8*)"Keyboard");
	listBox->setSelectionMode(MultiColumnList::RowSingle);
	listBox->addColumn("Action", 0, 0.5f);
	listBox->addColumn("Trigger", 1, 0.5f);
	listBox->subscribeEvent(MultiColumnList::EventSelectionChanged, 
		Event::Subscriber(&ActionConfig::handleKeyboardSetup, this));
	//Mouse list
	listBox = (MultiColumnList*)wmgr.getWindow((utf8*)"Mouse");
	listBox->setSelectionMode(MultiColumnList::RowSingle);
	listBox->addColumn("Action", 0, 0.5f);
	listBox->addColumn("Trigger", 1, 0.5f);
	listBox->subscribeEvent(MultiColumnList::EventSelectionChanged, 
		Event::Subscriber(&ActionConfig::handleMouseSetup, this));
	//Joystick list
	listBox = (MultiColumnList*)wmgr.getWindow((utf8*)"Joystick");
	listBox->setSelectionMode(MultiColumnList::RowSingle);
	listBox->addColumn("Action", 0, 0.5f);
	listBox->addColumn("Trigger", 1, 0.5f);
	listBox->subscribeEvent(MultiColumnList::EventSelectionChanged, 
		Event::Subscriber(&ActionConfig::handleJoystickSetup, this));

	//Create some functors for registering with ActionMap
	fireBind = new OIS::ActionBind(&ActionConfig::handleFire,this);
	jumpBind = new OIS::ActionBind(&ActionConfig::handleJump,this);
	moveBind = new OIS::ActionBind(&ActionConfig::handleMove,this);
	lookBind = new OIS::ActionBind(&ActionConfig::handleLook,this);
	doneBind = new OIS::ActionBind(&ActionConfig::handleDone,this);

	//Reset everything
	createScene();
	reset();
}

//--------------------------------------------------------------------------------//
ActionConfig::~ActionConfig()
{
	delete fireBind;
	delete jumpBind;
	delete moveBind;
	delete lookBind;
	delete doneBind;

	delete actionMap;
}

//--------------------------------------------------------------------------------//
void ActionConfig::createScene()
{
	//Situate Camera, place guy, etc etc
}

//--------------------------------------------------------------------------------//
void ActionConfig::reset()
{
	using namespace OIS;

	keyEvents.clear();
	mouseEvents.clear();
	joyEvents.clear();

	currentAction = "";

	//This should be saved/loaded from a file.. but, we will just set it up in code

	//Jumping
	keyEvents.push_back(DemoActionEventInfo(A_JUMP,KC_SPACE,OIS_Button,jumpBind,"Jump"));
	mouseEvents.push_back(DemoActionEventInfo(A_JUMP,MB_Right,OIS_Button,jumpBind,"Jump"));
	joyEvents.push_back(DemoActionEventInfo(A_JUMP,1,OIS_Button,jumpBind,"Jump"));

	//Firing
	keyEvents.push_back(DemoActionEventInfo(A_FIRE,KC_LCONTROL,OIS_Button,fireBind,"Fire"));
	mouseEvents.push_back(DemoActionEventInfo(A_FIRE,MB_Left,OIS_Button,fireBind,"Fire"));
	joyEvents.push_back(DemoActionEventInfo(A_FIRE,0,OIS_Button,fireBind,"Fire"));

	//Center View
	keyEvents.push_back(DemoActionEventInfo(A_CENTER_VIEW,KC_INSERT,OIS_Button,lookBind,"Center View"));
	mouseEvents.push_back(DemoActionEventInfo(A_CENTER_VIEW,MB_Middle,OIS_Button,lookBind,"Center View"));
	joyEvents.push_back(DemoActionEventInfo(A_CENTER_VIEW,2,OIS_Button,lookBind,"Center View"));

	//Move - joyStick & Mouse Only
	mouseEvents.push_back(DemoActionEventInfo(A_MOVE,-1,OIS_Axis,moveBind,"Move"));
	joyEvents.push_back(DemoActionEventInfo(A_MOVE,0,OIS_Axis,moveBind,"Move"));

	//Look - joyStick & Mouse Only
	mouseEvents.push_back(DemoActionEventInfo(A_LOOK,0,OIS_Axis,lookBind,"Look"));
	joyEvents.push_back(DemoActionEventInfo(A_LOOK,1,OIS_Axis,lookBind,"Look"));

	//Keyboard only movement actions
	keyEvents.push_back(DemoActionEventInfo(A_MOVE_FORWARD,KC_W,OIS_Button,moveBind,"Move Forward"));
	keyEvents.push_back(DemoActionEventInfo(A_MOVE_BACKWARD,KC_S,OIS_Button,moveBind,"Move Backward"));
	keyEvents.push_back(DemoActionEventInfo(A_MOVE_LEFT,KC_A,OIS_Button,moveBind,"Move Left"));
	keyEvents.push_back(DemoActionEventInfo(A_MOVE_RIGHT,KC_D,OIS_Button,moveBind,"Move Right"));

	//Keyboard only looking actions
	keyEvents.push_back(DemoActionEventInfo(A_LOOK_UP,KC_UP,OIS_Button,lookBind,"Look Up"));
	keyEvents.push_back(DemoActionEventInfo(A_LOOK_DOWN,KC_DOWN,OIS_Button,lookBind,"Look Down"));
	keyEvents.push_back(DemoActionEventInfo(A_LOOK_LEFT,KC_LEFT,OIS_Button,lookBind,"Look Left"));
	keyEvents.push_back(DemoActionEventInfo(A_LOOK_RIGHT,KC_RIGHT,OIS_Button,lookBind,"Look Right"));

	//Setup CEGUI stuff
	fillList();
}

//--------------------------------------------------------------------------------//
void ActionConfig::fillList()
{
	using namespace CEGUI;
	WindowManager& wmgr = WindowManager::getSingleton();

	//--------- KEYBOARD ACTION LIST --------------//
	MultiColumnList *listBox = (MultiColumnList*)wmgr.getWindow((utf8*)"Keyboard");
	listBox->resetList();

	std::vector<DemoActionEventInfo>::iterator i = keyEvents.begin(), e = keyEvents.end();
	ListboxTextItem* item = 0;

	for( ; i != e; ++i )
	{
		int row = listBox->addRow();
		//Action ID Name
		item = new ListboxTextItem( i->name.c_str(), 0);
		item->setTextColours(ColourRect(colour(.5, 0, .3)));
		listBox->setItem(item, MCLGridRef( row, 0 ));

		//Mapped Column
		if( i->trigger != -1 )
			i->li = new ListboxTextItem(mKey->getAsString((OIS::KeyCode)i->trigger), 0);
		else
			i->li = new ListboxTextItem(" ", 0);
		i->li->setTextColours(ColourRect(colour(.5, 0, .3)));
		listBox->setItem(i->li, MCLGridRef( row, 1 ));
	}

	//--------- MOUSE ACTION LIST --------------//
	listBox = (MultiColumnList*)wmgr.getWindow((utf8*)"Mouse");
	listBox->resetList();

	i = mouseEvents.begin(); e = mouseEvents.end();
	for( ; i != e; ++i )
	{
		int row = listBox->addRow();
		//Action ID Name
		item = new ListboxTextItem( i->name.c_str(), 0);
		item->setTextColours(ColourRect(colour(.5, 0, .3)));
		listBox->setItem(item, MCLGridRef( row, 0 ));

		//Command Column
		i->li = new ListboxTextItem(getAxisButtonName(i->cType, i->trigger), 0);
		i->li->setTextColours(ColourRect(colour(.5, 0, .3)));
		listBox->setItem(i->li, MCLGridRef( row, 1 ));
	}

	//--------- JOYSTICK ACTION LIST --------------//
	listBox = (MultiColumnList *)wmgr.getWindow((utf8*)"Joystick");
	listBox->resetList();	//Clear any old values

	i = joyEvents.begin(); e = joyEvents.end();
	for( ; i != e; ++i )
	{
		int row = listBox->addRow();
		//Action ID Name
		item = new ListboxTextItem( i->name.c_str(), 0);
		item->setTextColours(ColourRect(colour(.5, 0, .3)));
		listBox->setItem(item, MCLGridRef( row, 0 ));

		//Command Column
		i->li = new ListboxTextItem(getAxisButtonName(i->cType, i->trigger), 0);
		i->li->setTextColours(ColourRect(colour(.5, 0, .3)));
		listBox->setItem(i->li, MCLGridRef( row, 1 ));
	}

	//Hide for controls that do not exist
	if( !mKey ) wmgr.getWindow((utf8*)"Keyboard")->setVisible(false);
	if( !mMouse ) wmgr.getWindow((utf8*)"Mouse")->setVisible(false);
	if( !mJoy ) wmgr.getWindow((utf8*)"Joystick")->setVisible(false);
}

//----------------------------------------------------------------//
bool ActionConfig::handleKeyboardSetup(const CEGUI::EventArgs& e)
{
	
	using namespace CEGUI;

	MultiColumnList *listBox = (MultiColumnList*)WindowManager::getSingleton().getWindow((utf8*)"Keyboard");
	ListboxItem* item = listBox->getFirstSelectedItem();
	if( !item ) return false;

	//Disable main window.. Start handling Key Events.. Display window prompt
	WindowManager::getSingleton().getWindow((utf8*)"Main")->setEnabled(false);
	mKey->setEventCallback(this);
	WindowManager::getSingleton().getWindow((utf8*)"ActionPrompt")->setVisible(true);

	std::ostringstream ss;
	ss << "Push key for for \"";
	//CEGUI::String op << is unresolved ??? oh well, this works for now
	ss << "Push key for for \"" << item->getText().c_str() << "\" Action..."
	   << "\n\n Push ESCAPE to cancel";

	WindowManager::getSingleton().getWindow((utf8*)"ActionPromptLable")->setText(ss.str());

	currentAction = item->getText().c_str();

	//Then, in our KeyListener.. the next keyevent we have, we assign that as the
	//input. Close the window prompt. And update the MultiColumnList
	return true;
}

//----------------------------------------------------------------//
bool ActionConfig::handleMouseSetup(const CEGUI::EventArgs& e)
{
	using namespace CEGUI;

	MultiColumnList *listBox = (MultiColumnList*)WindowManager::getSingleton().getWindow((utf8*)"Mouse");
	ListboxItem* item = listBox->getFirstSelectedItem();
	if( !item ) return false;

	//Disable main window.. Start handling mouse & key Events.. Display window prompt
	WindowManager::getSingleton().getWindow((utf8*)"Main")->setEnabled(false);
	mMouse->setEventCallback(this);
	WindowManager::getSingleton().getWindow((utf8*)"ActionPrompt")->setVisible(true);

	std::ostringstream ss;
	ss << "Click or move mouse for \"" << item->getText().c_str() << "\" Action...";
	WindowManager::getSingleton().getWindow((utf8*)"ActionPromptLable")->setText(ss.str());

	currentAction = item->getText().c_str();
	
	return true;
}

//----------------------------------------------------------------//
bool ActionConfig::handleJoystickSetup(const CEGUI::EventArgs& e)
{
	using namespace CEGUI;

	MultiColumnList *listBox = (MultiColumnList*)WindowManager::getSingleton().getWindow((utf8*)"Joystick");
	ListboxItem* item = listBox->getFirstSelectedItem();
	if( !item ) return false;

	//Disable main window.. Start handling mouse & key Events.. Display window prompt
	WindowManager::getSingleton().getWindow((utf8*)"Main")->setEnabled(false);
	mJoy->setEventCallback(this);
	WindowManager::getSingleton().getWindow((utf8*)"ActionPrompt")->setVisible(true);

	std::ostringstream ss;
	ss << "Click or move joystick for \"" << item->getText().c_str() << "\" Action...";
	WindowManager::getSingleton().getWindow((utf8*)"ActionPromptLable")->setText(ss.str());

	currentAction = item->getText().c_str();

	return true;
}

//--------------------------------------------------------------------------------//
bool ActionConfig::keyPressed(const OIS::KeyEvent& arg)
{
	using namespace CEGUI;
	// check for ESCAPE - cancel
	if( arg.key != OIS::KC_ESCAPE )
	{
		//Store Key press and update listbox
		DemoActionEventInfo *act = getAction(keyEvents, currentAction);
		if(act)
		{
			act->trigger = arg.key;
			act->li->setText(mKey->getAsString((OIS::KeyCode)arg.key));
			ensureNoDuplicate(keyEvents, act);
		}
	}

	WindowManager::getSingleton().getWindow((utf8*)"Main")->setEnabled(true);
	WindowManager::getSingleton().getWindow((utf8*)"ActionPrompt")->setVisible(false);

	//Set back old listener
	mKey->setEventCallback(oldKeyListener);

	//We return false, because we care not for any more events
	return false;
}

//--------------------------------------------------------------------------------//
bool ActionConfig::mouseMoved(const OIS::MouseEvent& arg)
{
	using namespace CEGUI;
	//Store Mouse Axis and update listbox
	DemoActionEventInfo *act = getAction( mouseEvents, currentAction );
	if( act )
	{
		act->cType = OIS::OIS_Axis;
		if( arg.state.relZ != 0 )
			act->trigger = 1; //This is the mouse wheel
		else
			act->trigger = 0; //Normal XY mouse axis

		act->li->setText(getAxisButtonName(act->cType, act->trigger));
		ensureNoDuplicate(mouseEvents, act);
	}

	WindowManager::getSingleton().getWindow((utf8*)"Main")->setEnabled(true);
	WindowManager::getSingleton().getWindow((utf8*)"ActionPrompt")->setVisible(false);

	//Set back old listener
	mMouse->setEventCallback(oldMouseListener);

	//We return false, because we care not for any more events
	return false;
}

//--------------------------------------------------------------------------------//
bool ActionConfig::mousePressed(const OIS::MouseEvent& arg, OIS::MouseButtonID id)
{
	using namespace CEGUI;
	//Store Mouse Button and update listbox
	DemoActionEventInfo *act = getAction( mouseEvents, currentAction );
	if( act )
	{
		act->trigger = id;
		act->cType = OIS::OIS_Button;
		act->li->setText(getAxisButtonName(act->cType, act->trigger));
		ensureNoDuplicate(mouseEvents, act);
	}

	WindowManager::getSingleton().getWindow((utf8*)"Main")->setEnabled(true);
	WindowManager::getSingleton().getWindow((utf8*)"ActionPrompt")->setVisible(false);

	//Set back old listener
	mMouse->setEventCallback(oldMouseListener);

	//We return false, because we care not for any more events
	return false;
}

//--------------------------------------------------------------------------------//
bool ActionConfig::buttonPressed(const OIS::JoyStickEvent &arg, int button)
{
	using namespace CEGUI;
	//Store Mouse Button and update listbox
	DemoActionEventInfo *act = getAction( joyEvents, currentAction );
	if( act )
	{
		act->trigger = button;
		act->cType = OIS::OIS_Button;
		act->li->setText(getAxisButtonName(act->cType, act->trigger));
		ensureNoDuplicate(joyEvents, act);
	}

	WindowManager::getSingleton().getWindow((utf8*)"Main")->setEnabled(true);
	WindowManager::getSingleton().getWindow((utf8*)"ActionPrompt")->setVisible(false);

	//We return false, because we care not for any more events
	return false;
}

//--------------------------------------------------------------------------------//
bool ActionConfig::axisMoved(const OIS::JoyStickEvent &arg, int axis)
{
	using namespace CEGUI;
	
	DemoActionEventInfo *act = getAction( joyEvents, currentAction );
	if( act )
	{
		act->cType = OIS::OIS_Axis;
		act->trigger = axis;
		act->li->setText(getAxisButtonName(act->cType, act->trigger));
		ensureNoDuplicate(joyEvents, act);
	}

	WindowManager::getSingleton().getWindow((utf8*)"Main")->setEnabled(true);
	WindowManager::getSingleton().getWindow((utf8*)"ActionPrompt")->setVisible(false);

	//We return false, because we care not for any more events
	return false;
}

//--------------------------------------------------------------------------------//
bool ActionConfig::povMoved(const OIS::JoyStickEvent &arg, int pov)
{
	using namespace CEGUI;
	//Store Mouse Button and update listbox
	DemoActionEventInfo *act = getAction( joyEvents, currentAction );
	if( act )
	{
		act->cType = OIS::OIS_POV;
		act->trigger = pov;
		act->li->setText(getAxisButtonName(act->cType, act->trigger));
		ensureNoDuplicate(joyEvents, act);
	}

	WindowManager::getSingleton().getWindow((utf8*)"Main")->setEnabled(true);
	WindowManager::getSingleton().getWindow((utf8*)"ActionPrompt")->setVisible(false);

	//We return false, because we care not for any more events
	return false;
}

//--------------------------------------------------------------------------------//
DemoActionEventInfo* ActionConfig::getAction( std::vector<DemoActionEventInfo>& list, std::string name )
{
	std::vector<DemoActionEventInfo>::iterator i = list.begin(), e = list.end();
	for(; i != e; ++i) 
		if( i->name == name ) return &(*i);
	return 0;
}

//--------------------------------------------------------------------------------//
void ActionConfig::ensureNoDuplicate( std::vector<DemoActionEventInfo>& list, DemoActionEventInfo* act )
{
	std::vector<DemoActionEventInfo>::iterator i = list.begin(), e = list.end();
	for(; i != e; ++i)
	{
		if( i->trigger == act->trigger && 
		    i->cType == act->cType &&
		    i->name != act->name )
		{
			//Opps.. one dup existed.. clear it
			i->trigger = -1;
			i->li->setText("");
		}
	}
}

//--------------------------------------------------------------------------------//
std::string ActionConfig::getAxisButtonName( OIS::ComponentType type, int trigger )
{
	std::ostringstream ss;

	if( trigger != -1 )
	{
		if( type == OIS::OIS_Button )
			ss << "Button ";
		else if( type == OIS::OIS_Axis )
			ss << "Axis ";
		else if( type == OIS::OIS_POV )
			ss << "POV ";
		ss << trigger;
		return ss.str();
	}

	return " ";
}

//--------------------------------------------------------------------------------//
void ActionConfig::startTest()
{
	actionMap->reset();

	//Configure the action map for each device
	std::vector<DemoActionEventInfo>::iterator i = keyEvents.begin(), e = keyEvents.end();
	for( ; i != e; ++i )
		actionMap->addKeyboardAction( i->id, i->trigger, i->bound );

	i = mouseEvents.begin(); e = mouseEvents.end();
	for( ; i != e; ++i )
		actionMap->addMouseAction( i->id, i->trigger, i->cType, i->bound );

	i = joyEvents.begin(); e = joyEvents.end();
	for( ; i != e; ++i )
		actionMap->addJoyStickAction( i->id, i->trigger, i->cType, i->bound);

	//Setup a Key Event for exiting test - Escape Key
	actionMap->addKeyboardAction( A_TEST_DONE, OIS::KC_ESCAPE, doneBind );

	//Enable it.. this disables any registered buffered callbacks
	actionMap->setEnabled(true);

	//Hide CEGUI Window
	CEGUI::WindowManager::getSingleton().getWindow((CEGUI::utf8*)"Main")->setVisible(false);
	CEGUI::System::getSingleton().setDefaultMouseCursor(0);

	Ogre::Root::getSingleton().addFrameListener(this);

	mMoveScale = 1.0f;
	mRotXScale = 0.0f;
	mRotYScale = 0.0f;
}

//--------------------------------------------------------------------------------//
bool ActionConfig::frameStarted(const Ogre::FrameEvent& evt)
{
	Ogre::Camera *cam = mSceneMgr->getCamera("PlayerCam");

	// Move about 100 units per second,
	mMoveScale = 100 * evt.timeSinceLastFrame;

	cam->yaw(Ogre::Degree(mRotXScale));
	cam->pitch(Ogre::Degree(mRotYScale));
	cam->moveRelative(mTranslateVector);
	return true;
}

//--------------------------------------------------------------------------------//
void ActionConfig::handleJump( const OIS::ActionMapArg& arg )
{
	//Don't really care if axis or not. we only want to do this on keydown & not on keyup
	//if( arg.flags & OIS::OIS::ActionMapArg::AM_Button )
	//{
	//	if( arg.flags & OIS::OIS::ActionMapArg::AM_ButtonDown )
	//		do fire
	//	return;
	//}

	//do Fire
}

//--------------------------------------------------------------------------------//
void ActionConfig::handleFire( const OIS::ActionMapArg& arg )
{
	//Don't really care if axis or not.. but, we only want to do this once
	//if( arg.flags & OIS::OIS::ActionMapArg::AM_Button )
	//{
	//	if( arg.flags & OIS::ActionMapArg::AM_ButtonDown )
	//		do fire
	//	return;
	//}

	//do Fire
}

//--------------------------------------------------------------------------------//
void ActionConfig::handleMove( const OIS::ActionMapArg& arg )
{
	switch( arg.ActionID )
	{
		//Axis Event
		case A_MOVE:
		{
			//User set button as move event - just ignore
			if( arg.component->cType == OIS::OIS_Button )
			{
				break;
			}
			else if( arg.component->cType == OIS::OIS_Axis )
			{
				const OIS::Axis* axis = static_cast<const OIS::Axis*>(arg.component);
				if( axis->absOnly == false )
				{
					//Not an absolute only axis
					mSceneMgr->getCamera("PlayerCam")->moveRelative(
						Ogre::Vector3( axis->relX * 0.13f, //Move left/right
									   axis->relZ * 0.1f,  //Up/Down
									   axis->relY * 0.13f ) ); //F/B
				}
				else
				{
					//Is an absolute only axis
					//Up/Down
					mTranslateVector.y = mMoveScale * axis->abZ/(float)OIS::JoyStick::MAX_AXIS;
					//Move left/right
					mTranslateVector.x = mMoveScale * axis->abX/(float)OIS::JoyStick::MAX_AXIS;
					//Move forward/back
					mTranslateVector.z = -mMoveScale * axis->abY/(float)OIS::JoyStick::MAX_AXIS;
				}
			}
			else if( arg.component->cType == OIS::OIS_POV )
			{
				using namespace OIS;
				const Pov* pov = static_cast<const Pov*>(arg.component);

				if( pov->direction & Pov::North ) //Going up
					mTranslateVector.z = -mMoveScale;
				else if( pov->direction & Pov::South ) //Going down
					mTranslateVector.z = mMoveScale;

				if( pov->direction & Pov::East ) //Going right
					mTranslateVector.x = mMoveScale;
				else if( pov->direction & Pov::West ) //Going left
					mTranslateVector.x = -mMoveScale;

				if( pov->direction == Pov::Centered ) //stopped/centered out
					mTranslateVector.x = mTranslateVector.z = 0;
			}
			break;
		}
		//Moves forward - Key event only
		case A_MOVE_FORWARD:
			if( arg.component->cType == OIS::OIS_Button )
			{
				if( static_cast<const OIS::Button*>(arg.component)->pushed )
					mTranslateVector.z = -mMoveScale;
				else
					mTranslateVector.z = 0;		
			}
			break;
		//Moves Backwards - Key event only
		case A_MOVE_BACKWARD:
			if( arg.component->cType == OIS::OIS_Button )
			{
				if( static_cast<const OIS::Button*>(arg.component)->pushed )
					mTranslateVector.z = mMoveScale;
				else
					mTranslateVector.z = 0;
			}
			break;
		//Strafe Left - Key event only
		case A_MOVE_LEFT:
			if( arg.component->cType == OIS::OIS_Button )
			{
				if( static_cast<const OIS::Button*>(arg.component)->pushed )
					mTranslateVector.x = -mMoveScale;
				else
					mTranslateVector.x = 0;
			}
			break;
		//Strafe Right - Key event only
		case A_MOVE_RIGHT:
			if( arg.component->cType == OIS::OIS_Button )
			{
				if( static_cast<const OIS::Button*>(arg.component)->pushed )
					mTranslateVector.x = mMoveScale;
				else
					mTranslateVector.x = 0;
			}
			break;
		default:
			break;
	}
}

//--------------------------------------------------------------------------------//
void ActionConfig::handleLook( const OIS::ActionMapArg& arg )
{
	//Look - handles the camera looking.. Could just as well control
	//the players view instead
	switch( arg.ActionID )
	{
		//Axis Event - hopefully
		case A_LOOK:
		{
			//User set button as look axis - just ignore
			if( arg.component->cType == OIS::OIS_Button )
			{
				break;
			}
			else if( arg.component->cType == OIS::OIS_Axis )
			{
				const OIS::Axis* axis = static_cast<const OIS::Axis*>(arg.component);
				if( axis->absOnly == false )
				{
					//Move using relative Axis
					Ogre::Camera *cam = mSceneMgr->getCamera("PlayerCam");

					cam->yaw(Ogre::Radian(Ogre::Degree(-axis->relX * 0.13f)));
					cam->pitch(Ogre::Radian(Ogre::Degree(-axis->relY * 0.13f)));
				}
				else
				{
					//absolute only axis
					mRotXScale = -axis->abX/OIS::JoyStick::MAX_AXIS * 0.05;
					mRotYScale = -axis->abY/OIS::JoyStick::MAX_AXIS * 0.01;
				}
			}
			break;
		}
		//Tilt View Up - Key Event only
		case A_LOOK_UP:
			break;
		//Tilt view Down - Key Event only
		case A_LOOK_DOWN:
			break;
		//Turn Left - Key Event only
		case A_LOOK_LEFT:
			break;
		//Turn Right - Key Event only
		case A_LOOK_RIGHT:
			break;
		//Centers/levels out - Don't care if button or axes
		case A_CENTER_VIEW:
			break;
		default: break;
	}
}

//--------------------------------------------------------------------------------//
void ActionConfig::handleDone( const OIS::ActionMapArg& arg )
{
	//ESCAPE Key event - only do once, though, they keyup should never reach us
	//anyway
	if( static_cast<const OIS::Button*>(arg.component)->pushed )
	{
		//Stop the test, and reset listeners
		actionMap->setEnabled(false);

		//Restore the old listeners.. which were replaced with the ActionMap
		mKey->setEventCallback(oldKeyListener);
		mMouse->setEventCallback(oldMouseListener);

		//Show CEGUI Window
		CEGUI::WindowManager::getSingleton().getWindow((CEGUI::utf8*)"Main")->setVisible(true);
		CEGUI::System::getSingleton().setDefaultMouseCursor((CEGUI::utf8*)"WindowsLook",
			(CEGUI::utf8*)"MouseArrow");
		
		Ogre::Root::getSingleton().removeFrameListener(this);
	}
}
