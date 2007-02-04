/*
-----------------------------------------------------------------------------
This source file is part of OIS - Object Oriented Input System)
For the latest info, see http://www.sourceforge.net/projects/wgois/

You may use this sample code for anything you like. This demo uses the Action Mapping
classes of OIS to setup Action Events
-----------------------------------------------------------------------------
*/
#include <CEGUI/CEGUIImageset.h>
#include <CEGUI/CEGUISystem.h>
#include <CEGUI/CEGUILogger.h>
#include <CEGUI/CEGUISchemeManager.h>
#include <CEGUI/CEGUIWindowManager.h>
#include <CEGUI/CEGUIWindow.h>
#include <CEGUI/elements/CEGUICombobox.h>
#include <CEGUI/elements/CEGUIListbox.h>
#include <CEGUI/elements/CEGUIListboxTextItem.h>
#include <CEGUI/elements/CEGUIPushButton.h>
#include <CEGUI/elements/CEGUIScrollbar.h>
#include <CEGUI/elements/CEGUIStaticImage.h>
#include "OgreCEGUIRenderer.h"
#include "OgreCEGUIResourceProvider.h"

#include "ExampleApplication.h"
#include "OISJoyStick.h"
#include "ActionConfig.h"

//----------------------------------------------------------------//
CEGUI::MouseButton convertOISMouseButtonToCegui(int buttonID)
{
    switch (buttonID)
    {
	case 0: return CEGUI::LeftButton;
	case 1: return CEGUI::RightButton;
	case 2:	return CEGUI::MiddleButton;
	case 3: return CEGUI::X1Button;
	default: return CEGUI::LeftButton;
    }
}

//----------------------------------------------------------------//
class GuiFrameListener : public ExampleFrameListener
{
private:
	CEGUI::Renderer* mGUIRenderer;
	bool mShutdownRequested;
public:
	// NB using buffered input, this is the only change
	GuiFrameListener(RenderWindow* win, Camera* cam, CEGUI::Renderer* renderer) :
		ExampleFrameListener( win, cam, true, true ), mGUIRenderer(renderer), 
			mShutdownRequested(false)
	{
		//Override the ExamleFrameListeners callback with ours instead
		mMouse->setEventCallback( this );
		mKeyboard->setEventCallback( this );
	}

	//----------------------------------------------------------------//
	void requestShutdown(void)
	{
		mShutdownRequested = true;
	}
	//----------------------------------------------------------------//
	OIS::Mouse* getMouse() { return mMouse;}
	OIS::Keyboard* getKeyboard() { return mKeyboard;}
	OIS::JoyStick* getJoyStick() { return mJoy; }

	//----------------------------------------------------------------//
	bool frameStarted(const FrameEvent& evt)
	{
		ExampleFrameListener::frameStarted(evt);
		if( mJoy )
			mJoy->capture();
		return true;
	}
	//----------------------------------------------------------------//
	bool frameEnded(const FrameEvent& evt)
	{
		if (mShutdownRequested)
			return false;
		else
			return ExampleFrameListener::frameEnded(evt);
	}

	//----------------------------------------------------------------//
	bool mouseMoved( const OIS::MouseEvent &arg )
	{
		CEGUI::System::getSingleton().injectMouseMove( arg.state.relX, arg.state.relY );
		return true;
	}

	//----------------------------------------------------------------//
	bool mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
	{
		CEGUI::System::getSingleton().injectMouseButtonDown(convertOISMouseButtonToCegui(id));
		return true;
	}

	//----------------------------------------------------------------//
	bool mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
	{
		CEGUI::System::getSingleton().injectMouseButtonUp(convertOISMouseButtonToCegui(id));
		return true;
	}

	//----------------------------------------------------------------//
	bool keyPressed( const OIS::KeyEvent &arg )
	{
		if( arg.key == OIS::KC_ESCAPE )
			mShutdownRequested = true;
		CEGUI::System::getSingleton().injectKeyDown( arg.key );
		CEGUI::System::getSingleton().injectChar( arg.text );
		return true;
	}

	//----------------------------------------------------------------//
	bool keyReleased( const OIS::KeyEvent &arg )
	{
		CEGUI::System::getSingleton().injectKeyUp( arg.key );
		return true;
	}
};

class GuiApplication : public ExampleApplication
{
	CEGUI::OgreCEGUIRenderer* mGUIRenderer;
	CEGUI::System* mGUISystem;
	ActionConfig *actionConfig;

public:
	//----------------------------------------------------------------//
	GuiApplication() : mGUIRenderer(0), mGUISystem(0), actionConfig(0)
	{
	}

	//----------------------------------------------------------------//
	~GuiApplication()
	{
		delete actionConfig;
		delete mGUISystem;
		delete mGUIRenderer;
	}

protected:
	//----------------------------------------------------------------//
	void createScene(void)
	{
		//--------------- setup OGRE scene ---------------------//
		mSceneMgr->setAmbientLight(ColourValue(0.5, 0.5, 0.5));
		mSceneMgr->setSkyDome(true, "Examples/CloudySky", 5, 8);

		Light* l = mSceneMgr->createLight("MainLight");
		l->setPosition(20,80,50);

		Entity *ent = mSceneMgr->createEntity("Ship", "razor.mesh");
		mSceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(ent);

		//--------------- setup GUI system ---------------------//
		mGUIRenderer = new CEGUI::OgreCEGUIRenderer(mWindow, RENDER_QUEUE_OVERLAY, 
			false, 3000, mSceneMgr);
		mGUISystem = new CEGUI::System(mGUIRenderer);
		CEGUI::Logger::getSingleton().setLoggingLevel(CEGUI::Informative);
		CEGUI::SchemeManager::getSingleton().loadScheme((CEGUI::utf8*)
			"WindowsLook.scheme");
		mGUISystem->setDefaultMouseCursor((CEGUI::utf8*)"WindowsLook",(CEGUI::utf8*)
			"MouseArrow");
		mGUISystem->injectMousePosition(0,0);
		mGUISystem->setDefaultFont((CEGUI::utf8*)"Tahoma-12");

		CEGUI::Window* sheet = CEGUI::WindowManager::getSingleton().loadWindowLayout
			((CEGUI::utf8*)"ActionMapping.layout");
		mGUISystem->setGUISheet(sheet);
		//Register our handlers for the three bottom buttons
		setupEventHandlers();
	}

	//----------------------------------------------------------------//
	void createFrameListener(void)
	{
		mFrameListener= new GuiFrameListener(mWindow, mCamera, mGUIRenderer);
		mRoot->addFrameListener(mFrameListener);

		//Get Mouse & Keyboard, and create joystick if present
		GuiFrameListener* gUI = static_cast<GuiFrameListener*>(mFrameListener);
		OIS::Mouse* mouse = gUI->getMouse();
		OIS::Keyboard* kb = gUI->getKeyboard();
		OIS::JoyStick* joy = gUI->getJoyStick();

		//Utility class for configuring, & testing ActionMap
		actionConfig = new ActionConfig( mouse, kb, joy, mSceneMgr );
		mFrameListener->showDebugOverlay(false);
	}

	//----------------------------------------------------------------//
	void setupEventHandlers(void)
	{
		CEGUI::WindowManager& wmgr = CEGUI::WindowManager::getSingleton();
		wmgr.getWindow((CEGUI::utf8*)"Test")->subscribeEvent(
			CEGUI::PushButton::EventClicked, 
			CEGUI::Event::Subscriber(&GuiApplication::handleTest, this));
		wmgr.getWindow((CEGUI::utf8*)"Reset")->subscribeEvent(
			CEGUI::PushButton::EventClicked, 
			CEGUI::Event::Subscriber(&GuiApplication::handleReset, this));
		wmgr.getWindow((CEGUI::utf8*)"Quit")->subscribeEvent(
			CEGUI::PushButton::EventClicked, 
			CEGUI::Event::Subscriber(&GuiApplication::handleQuit, this));
	}

	//----------------------------------------------------------------//
	bool handleTest(const CEGUI::EventArgs& e)
	{
		actionConfig->startTest();
		return true;
	}

	//----------------------------------------------------------------//
	bool handleReset(const CEGUI::EventArgs& e)
	{
		actionConfig->reset();
		return true;
	}

	//----------------------------------------------------------------//
	bool handleQuit(const CEGUI::EventArgs& e)
	{
		static_cast<GuiFrameListener*>(mFrameListener)->requestShutdown();
		return true;
	}
};

//------------------------------------------------------------------------//
//------------------------- MAIN METHOD - Entry Point --------------------//

#include "OISException.h"

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#  define WIN32_LEAN_AND_MEAN
#  include "windows.h"

//----------------------------------------------------------------//
INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT )
#else
int main(int argc, char *argv[])
#endif
{
	// Create application object
	GuiApplication app;

	try 
	{
		app.go();
	}
	//Catch any Ogre exceptions
	catch( Ogre::Exception& e ) 
	{
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
		MessageBox( NULL, e.getFullDescription().c_str(), "An exception has occured!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
#else
		std::cerr << "An exception has occured: " << e.getFullDescription().c_str() << std::endl;
#endif
    }
	//Catch any OIS exceptions
    catch( OIS::Exception &oe )
    {
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
		MessageBox( NULL, oe.eText, "An exception has occured!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
#else
		std::cerr << "An exception has occured: " << oe.eText << " " << oe.eLine << std::endl;
#endif
	}
	//Catch any thing else that might have thrown... Perhaps CEGUI...
    catch(...)
	{
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
		MessageBox( NULL, "Unknown Exception type!", "An exception has occured!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
#else
		std::cerr << "An exception has occured: Unknown Type! " << std::endl;
#endif
	}

	return 0;
}
