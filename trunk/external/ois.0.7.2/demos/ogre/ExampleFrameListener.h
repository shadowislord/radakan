/*
-----------------------------------------------------------------------------
This source file is part of OGRE
    (Object-oriented Graphics Rendering Engine)
For the latest info, see http://www.ogre3d.org/

Copyright (c) 2000-2005 The OGRE Team
Also see acknowledgements in Readme.html

You may use this sample code for anything you like, it is not covered by the
LGPL like the rest of the engine.
-----------------------------------------------------------------------------
*/
/*
-----------------------------------------------------------------------------
Filename:    ExampleFrameListener.h
Description: Defines an example frame listener which responds to frame events.
This frame listener just moves a specified camera around based on
keyboard and mouse movements.
Mouse:    Freelook
W or Up:  Forward
S or Down:Backward
A:        Step left
D:        Step right
             PgUp:     Move upwards
             PgDown:   Move downwards
             F:        Toggle frame rate stats on/off
			 R:        Render mode
             T:        Cycle texture filtering
                       Bilinear, Trilinear, Anisotropic(8)
             P:        Toggle on/off display of camera position / orientation
-----------------------------------------------------------------------------
*/

#ifndef __ExampleFrameListener_H__
#define __ExampleFrameListener_H__

#include "Ogre.h"
#include "OgreStringConverter.h"
#include "OgreException.h"

//OIS Includes
#include "OISInputManager.h"
#include "OISEvents.h"
#include "OISMouse.h"
#include "OISKeyboard.h"
#include "OISJoyStick.h"

using namespace Ogre;

class ExampleFrameListener: public FrameListener, public OIS::KeyListener, public OIS::MouseListener
{
protected:
	int mSceneDetailIndex ;
	Real mMoveSpeed;
	Degree mRotateSpeed;
	Overlay* mDebugOverlay;

	void updateStats(void)
	{
		static String currFps = "Current FPS: ";
		static String avgFps = "Average FPS: ";
		static String bestFps = "Best FPS: ";
		static String worstFps = "Worst FPS: ";
		static String tris = "Triangle Count: ";

		// update stats when necessary
		try {
			OverlayElement* guiAvg = OverlayManager::getSingleton().getOverlayElement("Core/AverageFps");
			OverlayElement* guiCurr = OverlayManager::getSingleton().getOverlayElement("Core/CurrFps");
			OverlayElement* guiBest = OverlayManager::getSingleton().getOverlayElement("Core/BestFps");
			OverlayElement* guiWorst = OverlayManager::getSingleton().getOverlayElement("Core/WorstFps");

			const RenderTarget::FrameStats& stats = mWindow->getStatistics();

			guiAvg->setCaption(avgFps + StringConverter::toString(stats.avgFPS));
			guiCurr->setCaption(currFps + StringConverter::toString(stats.lastFPS));
			guiBest->setCaption(bestFps + StringConverter::toString(stats.bestFPS)
				+" "+StringConverter::toString(stats.bestFrameTime)+" ms");
			guiWorst->setCaption(worstFps + StringConverter::toString(stats.worstFPS)
				+" "+StringConverter::toString(stats.worstFrameTime)+" ms");
			OverlayElement* guiTris = OverlayManager::getSingleton().getOverlayElement("Core/NumTris");
			guiTris->setCaption(tris + StringConverter::toString(stats.triangleCount));

			OverlayElement* guiDbg = OverlayManager::getSingleton().getOverlayElement("Core/DebugText");
			guiDbg->setCaption(mWindow->getDebugText());
		}
		catch(...) {}
	}

	OIS::Mouse* mMouse;
	OIS::Keyboard* mKeyboard;
	OIS::JoyStick* mJoy;
public:
	// Constructor takes a RenderWindow because it uses that to determine input context
	ExampleFrameListener(RenderWindow* win, Camera* cam, bool bufferedKeys = false, 
		bool bufferedMouse = false, bool bufferedJoy = true )
	{
		mDebugOverlay = OverlayManager::getSingleton().getByName("Core/DebugOverlay");
        
		// BEGIN INPUT INITIALIZATION
		OIS::ParamList pl;	
		size_t windowHnd = 0;
		std::ostringstream windowHndStr;
#if defined OIS_WIN32_PLATFORM
		win->getCustomAttribute("HWND", &windowHnd);
#elif defined OIS_LINUX_PLATFORM
		win->getCustomAttribute("GLXWINDOW", &windowHnd);
//	pl.insert(std::make_pair(std::string("x11_mouse_grab"), std::string("false")));
//	pl.insert(std::make_pair(std::string("x11_mouse_hide"), std::string("false")));
#endif
		//Both x11 and Win32 use handle of some sort..
		windowHndStr << windowHnd;
		pl.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));

		OIS::InputManager &im = *OIS::InputManager::createInputSystem( pl );
		mKeyboard = static_cast<OIS::Keyboard*>(im.createInputObject( OIS::OISKeyboard, bufferedKeys ));
		mMouse = static_cast<OIS::Mouse*>(im.createInputObject( OIS::OISMouse, bufferedMouse ));
		try
		{
			mJoy = static_cast<OIS::JoyStick*>(im.createInputObject( OIS::OISJoyStick, bufferedJoy ));
		}
		catch(...)
		{
			mJoy = 0;
		}

		unsigned int width, height, depth;
		int left, top;
		win->getMetrics(width, height, depth, left, top);

		//Set Mouse Region.. if window resizes, we should alter this to reflect as well
		const OIS::MouseState &ms = mMouse->getMouseState();
		ms.width = width;
		ms.height = height;

		mKeyboard->setEventCallback(this);
		mMouse->setEventCallback(this);
		// END INPUT INITIALIZATION

		mRotateSpeed = 36;
		mMoveSpeed = 100;

		mCamera = cam;
		mWindow = win;
		mStatsOn = true;
		mNumScreenShots = 0;
		mTimeUntilNextToggle = 0;
		mSceneDetailIndex = 0;
		mMoveScale = 0.0f;
		mRotScale = 0.0f;
		mTranslateVector = Vector3::ZERO;
		mAniso = 1;
		mFiltering = TFO_BILINEAR;

		showDebugOverlay(true);
	}

	virtual ~ExampleFrameListener()
	{
		OIS::InputManager* im = OIS::InputManager::getSingletonPtr();
		if( im )
		{
			im->destroyInputObject( mMouse );
			im->destroyInputObject( mKeyboard );
			im->destroyInputObject( mJoy );
			im->destroyInputSystem();
			im = 0;
		}
	}

	virtual bool processUnbufferedKeyInput(const FrameEvent& evt)
	{
		if(mKeyboard->isKeyDown(OIS::KC_A))
			mTranslateVector.x = -mMoveScale;	// Move camera left

		if(mKeyboard->isKeyDown(OIS::KC_D))
			mTranslateVector.x = mMoveScale;	// Move camera RIGHT

		if(mKeyboard->isKeyDown(OIS::KC_UP) || mKeyboard->isKeyDown(OIS::KC_W) )
			mTranslateVector.z = -mMoveScale;	// Move camera forward

		if(mKeyboard->isKeyDown(OIS::KC_DOWN) || mKeyboard->isKeyDown(OIS::KC_S) )
			mTranslateVector.z = mMoveScale;	// Move camera backward

		if(mKeyboard->isKeyDown(OIS::KC_PGUP))
			mTranslateVector.y = mMoveScale;	// Move camera up

		if(mKeyboard->isKeyDown(OIS::KC_PGDOWN))
			mTranslateVector.y = -mMoveScale;	// Move camera down

		if(mKeyboard->isKeyDown(OIS::KC_RIGHT))
			mCamera->yaw(-mRotScale);
		
		if(mKeyboard->isKeyDown(OIS::KC_LEFT))
			mCamera->yaw(mRotScale);

		if( mKeyboard->isKeyDown(OIS::KC_ESCAPE) || mKeyboard->isKeyDown(OIS::KC_Q) )
			return false;

		// see if user wants to toggle mouse buffered mode
		if( mKeyboard->isKeyDown(OIS::KC_M) && mTimeUntilNextToggle <= 0 )
		{
			mMouse->setBuffered( !mMouse->buffered() );
			mTimeUntilNextToggle = 1;
		}

		// Must be going from immediate keyboard to buffered keyboard
	        if( mKeyboard->isKeyDown(OIS::KC_K) && mTimeUntilNextToggle <= 0 ) 
		{
			mKeyboard->setBuffered(true);
        	    mTimeUntilNextToggle = 1;
	        }

        	if( mKeyboard->isKeyDown(OIS::KC_F) && mTimeUntilNextToggle <= 0 ) 
		{
			mStatsOn = !mStatsOn;
			showDebugOverlay(mStatsOn);
			mTimeUntilNextToggle = 1;
		}

		if( mKeyboard->isKeyDown(OIS::KC_T) && mTimeUntilNextToggle <= 0 ) 
		{
			switch(mFiltering)
			{
			case TFO_BILINEAR:
				mFiltering = TFO_TRILINEAR;
				mAniso = 1;
				break;
			case TFO_TRILINEAR:
				mFiltering = TFO_ANISOTROPIC;
				mAniso = 8;
				break;
			case TFO_ANISOTROPIC:
				mFiltering = TFO_BILINEAR;
				mAniso = 1;
				break;
			default: break;
			}
			MaterialManager::getSingleton().setDefaultTextureFiltering(mFiltering);
			MaterialManager::getSingleton().setDefaultAnisotropy(mAniso);

			showDebugOverlay(mStatsOn);
			mTimeUntilNextToggle = 1;
		}

		if(mKeyboard->isKeyDown(OIS::KC_SYSRQ) && mTimeUntilNextToggle <= 0)
		{
			char tmp[20];
			sprintf( tmp, "screenshot_%d.png", ++mNumScreenShots );
			mWindow->writeContentsToFile(tmp);
			mTimeUntilNextToggle = 0.5;
			mWindow->setDebugText(String("Wrote ") + tmp);
		}
		
		if(mKeyboard->isKeyDown(OIS::KC_R) && mTimeUntilNextToggle <=0)
		{
			//mSceneDetailIndex = (mSceneDetailIndex+1)%3 ;
			//switch(mSceneDetailIndex) {
			//	case 0 : mCamera->setDetailLevel(SDL_SOLID); break;
			//	case 1 : mCamera->setDetailLevel(SDL_WIREFRAME); break;
			//	case 2 : mCamera->setDetailLevel(SDL_POINTS); break;
			//}
			mTimeUntilNextToggle = 0.5;
		}

		static bool displayCameraDetails = false;
		if(mKeyboard->isKeyDown(OIS::KC_P) && mTimeUntilNextToggle <= 0)
		{
			displayCameraDetails = !displayCameraDetails;
			mTimeUntilNextToggle = 0.5;
			if (!displayCameraDetails) mWindow->setDebugText("");
		}
		
		// Print camera details
		if(displayCameraDetails)
			mWindow->setDebugText("P: " + StringConverter::toString(mCamera->getDerivedPosition()) + " " + "O: " + StringConverter::toString(mCamera->getDerivedOrientation()));

		// Return true to continue rendering
		return true;
	}

	bool processUnbufferedMouseInput(const FrameEvent& evt)
	{
		// Rotation factors, may not be used if the second mouse button is pressed
		// 2nd mouse button - slide, otherwise rotate
		const OIS::MouseState &ms = mMouse->getMouseState();
		if( ms.buttonDown( OIS::MB_Right ) )
		{
			mTranslateVector.x += ms.relX * 0.13;
			mTranslateVector.y -= ms.relY * 0.13;
		}
		else
		{
			mRotX = Degree(-ms.relX * 0.13);
			mRotY = Degree(-ms.relY * 0.13);
		}

		return true;
	}

	void moveCamera()
	{
		// Make all the changes to the camera
		// Note that YAW direction is around a fixed axis (freelook style) rather than a natural YAW (e.g. airplane)
		mCamera->yaw(mRotX);
		mCamera->pitch(mRotY);
		mCamera->moveRelative(mTranslateVector);
	}

	void showDebugOverlay(bool show)
	{
		if (mDebugOverlay)
		{
			if(show)
				mDebugOverlay->show();
			else
				mDebugOverlay->hide();
		}
	}

	// Override frameStarted event to process that (don't care about frameEnded)
	virtual bool frameStarted(const FrameEvent& evt)
	{
		if(mWindow->isClosed())	return false;

		//Need to capture/update each device
		mKeyboard->capture();		
		mMouse->capture();

		if( mMouse->buffered() == false || mKeyboard->buffered() == false )
		{
			// one of the input modes is immediate, so setup what is needed for immediate mouse/key movement
			if (mTimeUntilNextToggle >= 0) 
				mTimeUntilNextToggle -= evt.timeSinceLastFrame;

			// If this is the first frame, pick a speed
			if (evt.timeSinceLastFrame == 0)
			{
				mMoveScale = 1;
				mRotScale = 0.1;
			}
			// Otherwise scale movement units by time passed since last frame
			else
			{
				// Move about 100 units per second,
				mMoveScale = mMoveSpeed * evt.timeSinceLastFrame;
				// Take about 10 seconds for full rotation
				mRotScale = mRotateSpeed * evt.timeSinceLastFrame;
			}
			mRotX = 0;
			mRotY = 0;
			mTranslateVector = Vector3::ZERO;
		
			if( mKeyboard->buffered() == false )
				if (processUnbufferedKeyInput(evt) == false)
					return false;
			
			if( mMouse->buffered() == false )
				if (processUnbufferedMouseInput(evt) == false)
					return false;

			moveCamera();
		}

		return true;
	}

	virtual bool frameEnded(const FrameEvent& evt)
	{
		updateStats();
		return true;
	}

	bool keyPressed( const OIS::KeyEvent &arg )
	{
		return true;
	}

	bool keyReleased( const OIS::KeyEvent &arg )
	{
		if( arg.key == OIS::KC_M )
			mMouse->setBuffered( !mMouse->buffered() );	
		else if( arg.key == OIS::KC_K )
			mKeyboard->setBuffered( !mKeyboard->buffered() );

		return true;
	}

	bool mouseMoved( const OIS::MouseEvent &arg )
	{
		return true;
	}

	bool mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
	{
		return true;
	}

	bool mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
	{
		return true;
	}

protected:
	Camera* mCamera;

	Vector3 mTranslateVector;
	RenderWindow* mWindow;
	bool mStatsOn;
	unsigned int mNumScreenShots;
	float mMoveScale;
	Degree mRotScale;

	// just to stop toggles flipping too fast
	Real mTimeUntilNextToggle;
	Radian mRotX, mRotY;
	TextureFilterOptions mFiltering;
	int mAniso;
};
#endif
