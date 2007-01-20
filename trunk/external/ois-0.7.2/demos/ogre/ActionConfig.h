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
#ifndef ActionConfig_Header
#define ActionConfig_Header
#include <CEGUI/CEGUIForwardRefs.h>

#include "OISPrereqs.h"
#include "OISEvents.h"

#include "OgrePrerequisites.h"
#include "OgreFrameListener.h"
#include "OgreVector3.h"

#include "ActionMap.h"

//Simple utility structure that extends the one from ActionMap.. As we want to store
//some additional info for configuring things
struct DemoActionEventInfo : public OIS::ActionEventInfo
{
	//! Constructor
	DemoActionEventInfo(int iID,int iTrigger,OIS::ComponentType type,OIS::ActionBind* pBind,const std::string& sName ) :
		ActionEventInfo(iID,iTrigger,type,pBind), name(sName),
		li(0) {};

	//! Friendly text name of the Action
	std::string name;
	//! The listboxitem that the Mapped Action will be displayed in
	CEGUI::ListboxTextItem* li;
};

/**
	This class is kinda a one stop class right now. This class handles setup, config,
	and testing of ActionMap. Ideally, the actionmap should be loaded from a file. But,
	for clarity, and less parsing code, it is setup via code. And also, the ActionMap class
	could be configured from here, but should be passed back to wherever you want to
	implement it at.

	This class inheirits from The three listeners for setting up the ActionMap
	action triggers.

	And the framelistener is used for actionmap testing.. for movements
*/
class ActionConfig : public OIS::KeyListener, public OIS::MouseListener,
		     public OIS::JoyStickListener, public Ogre::FrameListener
{
public:
	ActionConfig( OIS::Mouse *mouse, OIS::Keyboard *key, OIS::JoyStick *joy,
		      Ogre::SceneManager* scnMgr );
	~ActionConfig();

	//Resets everything back to default - lists, actions, scene
	void reset();

	//Go into test mode
	void startTest();

	//cegui event handlers
	bool handleKeyboardSetup(const CEGUI::EventArgs& e);
	bool handleMouseSetup(const CEGUI::EventArgs& e);
	bool handleJoystickSetup(const CEGUI::EventArgs& e);

	//Example Methods used for recieveing Action Events
	void handleJump( const OIS::ActionMapArg& );
	void handleFire( const OIS::ActionMapArg& );
	void handleMove( const OIS::ActionMapArg& );
	void handleLook( const OIS::ActionMapArg& ); //Handle Look and Center Actions
	void handleDone( const OIS::ActionMapArg& );

	//We only listen when we are configuring an action, and restore old listener
	bool keyPressed(const OIS::KeyEvent& arg);
	bool mouseMoved(const OIS::MouseEvent& arg);
	bool mousePressed(const OIS::MouseEvent& arg, OIS::MouseButtonID);
	bool buttonPressed(const OIS::JoyStickEvent &arg, int button);
	bool axisMoved(const OIS::JoyStickEvent &arg, int axis );
	bool povMoved( const OIS::JoyStickEvent &arg, int pov );

	//We ignore these
	bool keyReleased(const OIS::KeyEvent&) {return false;}
	bool mouseReleased(const OIS::MouseEvent&, OIS::MouseButtonID) {return false;}
	bool buttonReleased(const OIS::JoyStickEvent &, int) {return false;}

	//Process frame events
	bool frameStarted(const Ogre::FrameEvent& evt);

protected:
	//Internal helper methods
	void fillList();
	void createScene();

	//Returns a pointer to an actionevent based on the sent name
	DemoActionEventInfo* getAction( std::vector<DemoActionEventInfo>& list,
					std::string name );

	//If an input event exists than remove that one
	void ensureNoDuplicate( std::vector<DemoActionEventInfo>& list,
				DemoActionEventInfo* act );

	//Convert params into a string representing a button/axes number or " "
	std::string getAxisButtonName( OIS::ComponentType type, int trigger );

	//Keep a hold of some devices
	OIS::Mouse *mMouse;
	OIS::Keyboard *mKey;
	OIS::JoyStick *mJoy;

	//Keep old listeners stored when we start messing with the current one
	OIS::KeyListener* oldKeyListener;
	OIS::MouseListener* oldMouseListener;

	//We keep seperate lists for each tyoe of device we want to support..
	//We then assign a default button, or -1 if it will be blank - to allow
	//for assignment later, for each action
	std::vector<DemoActionEventInfo> keyEvents;
	std::vector<DemoActionEventInfo> mouseEvents;
	std::vector<DemoActionEventInfo> joyEvents;

	//Use this to ensure that the same joystick type that was used previously,
	//is still the one connected here
	std::string joyName;

	//Represents the row (Action) that the user clicked.. and is configuring
	std::string currentAction;

	//Our ActionMap instance
	OIS::ActionMap* actionMap;

	//Functors for registering with ActionMap
	OIS::ActionBind* fireBind;
	OIS::ActionBind* jumpBind;
	OIS::ActionBind* moveBind;
	OIS::ActionBind* lookBind;
	OIS::ActionBind* doneBind;

	//Ogre Scene Stuff - For testing the controls
	Ogre::SceneManager* mSceneMgr;
	Ogre::Vector3 mTranslateVector;
	float mMoveScale;
	float mRotXScale;
	float mRotYScale;
};

#endif
