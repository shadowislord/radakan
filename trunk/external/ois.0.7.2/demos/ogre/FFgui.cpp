#include <CEGUI/CEGUIImageset.h>
#include <CEGUI/CEGUISystem.h>
#include <CEGUI/CEGUILogger.h>
#include <CEGUI/CEGUISchemeManager.h>
#include <CEGUI/CEGUIWindowManager.h>
#include <CEGUI/CEGUIWindow.h>
#include <CEGUI/elements/CEGUICombobox.h>
#include <CEGUI/elements/CEGUIListbox.h>
#include <CEGUI/elements/CEGUIListboxTextItem.h>
#include <CEGUI/elements/CEGUIRadioButton.h>
#include <CEGUI/elements/CEGUISpinner.h>
#include <CEGUI/elements/CEGUIPushButton.h>
#include <CEGUI/elements/CEGUIScrollbar.h>
#include <CEGUI/elements/CEGUIStaticImage.h>
#include "OgreCEGUIRenderer.h"
#include "OgreCEGUIResourceProvider.h"
#include "ExampleApplication.h"

#include "OISException.h"
#include "OISForceFeedback.h"

//Some helper vars
const char *effect_types[] = {"UnKnown", "Constant", "Ramp", "Spring", 
	"Friction", "Square", "Triangle", "Sine", "SawToothUp", "SawToothDown", 
	"Damper", "Inertia", "Custom"};

const char *effect_force[] = {"UnknownForce","ConstantForce","RampForce",
	"PeriodicForce","ConditionalForce","CustomForce"};

//----------------------------------------------------------------//
class GuiFrameListener : public ExampleFrameListener
{
private:
	CEGUI::Renderer* mGUIRenderer;
	bool mShutdownRequested;

public:
	//----------------------------------------------------------------//
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
		mJoy->capture();

		if (mShutdownRequested)
			return false;

		return true;
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
		CEGUI::System::getSingleton().injectMouseButtonDown((CEGUI::MouseButton)id);
		return true;
	}

	//----------------------------------------------------------------//
	bool mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
	{
		CEGUI::System::getSingleton().injectMouseButtonUp((CEGUI::MouseButton)id);
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

//----------------------------------------------------------------//
class GuiApplication : public ExampleApplication
{
	CEGUI::OgreCEGUIRenderer* mGUIRenderer;
	CEGUI::System* mGUISystem;

	std::vector<OIS::Effect*> mEffects;
	OIS::Effect* SelectedEffect;

public:
	//----------------------------------------------------------------//
	GuiApplication() : mGUIRenderer(0), mGUISystem(0)
	{
		SelectedEffect = 0;
	}

	//----------------------------------------------------------------//
	~GuiApplication()
	{
		std::vector<OIS::Effect*>::iterator i = mEffects.begin(), e = mEffects.end();
		for(; i != e; ++i )
			delete *i;
		mEffects.clear();

		delete mGUISystem;
		delete mGUIRenderer;
	}

protected:
	//----------------------------------------------------------------//
	void createScene(void)
	{
		using namespace CEGUI;

		//--------------- setup OGRE scene ---------------------//
		mSceneMgr->setAmbientLight(ColourValue(0.5, 0.5, 0.5));
		mSceneMgr->setSkyDome(true, "Examples/CloudySky", 5, 8);

		//--------------- setup GUI system ---------------------//
		mGUIRenderer = new OgreCEGUIRenderer(mWindow, RENDER_QUEUE_OVERLAY, false, 
						     3000, mSceneMgr);
		mGUISystem = new System(mGUIRenderer);
		Logger::getSingleton().setLoggingLevel(Informative);
		SchemeManager::getSingleton().loadScheme((utf8*)"WindowsLook.scheme");
		mGUISystem->setDefaultMouseCursor((utf8*)"WindowsLook",(utf8*)"MouseArrow");
		mGUISystem->injectMousePosition(0,0);
		mGUISystem->setDefaultFont((utf8*)"Tahoma-12");

		Window* sheet = WindowManager::getSingleton().loadWindowLayout((utf8*)"ForceFeedback.layout");
		mGUISystem->setGUISheet(sheet);

		//Register our handlers for the three bottom buttons
		setupEventHandlers();
	}

	//----------------------------------------------------------------//
	void createFrameListener(void)
	{
		using namespace OIS;

		mFrameListener= new GuiFrameListener(mWindow, mCamera, mGUIRenderer);
		mRoot->addFrameListener(mFrameListener);

		//Create the Joystick
		JoyStick* joy = static_cast<GuiFrameListener*>(mFrameListener)->getJoyStick();
		if( joy == 0 )
			OIS_EXCEPT( OIS::E_InputDisconnected, "FF Device should be first device found for this demo.");

		//Fill in supported FF effects
		ForceFeedback* ff = (OIS::ForceFeedback*)joy->queryInterface(Interface::ForceFeedback);
		if( !ff )
			OIS_EXCEPT(E_NotSupported, "No Force Feedback capable device found!");

		//This should be turned off prior to uploading any FF effects
		ff->setAutoCenterMode( false );

		const ForceFeedback::SupportedEffectList &list = ff->getSupportedEffects();
		ForceFeedback::SupportedEffectList::const_iterator i = list.begin(), e = list.end();

		//Populate the CEGUI ComboBox
		CEGUI::Listbox *listBox = static_cast<CEGUI::Listbox*>(CEGUI::WindowManager::
				getSingletonPtr()->getWindow("SupportedEffectsList"));

		for( ; i != e; ++i)
		{
			std::ostringstream ss;
			ss << effect_types[i->second] << " (" << effect_force[i->first] << ")";

			CEGUI::ListboxTextItem* li = new CEGUI::ListboxTextItem(ss.str());
			li->setTextColours(CEGUI::ColourRect(CEGUI::colour(.5, 0, .3)));
			li->setSelectionColours(CEGUI::ColourRect(CEGUI::colour(0.2, 0.7, 0.7)));
			li->setSelectionBrushImage("WindowsLook", "TabButtonMiddleSelected" );
			listBox->addItem(li);
		}

		mFrameListener->showDebugOverlay(false);
	}

	//----------------------------------------------------------------//
	void setupEventHandlers(void)
	{
		using namespace CEGUI;
		WindowManager& wmgr = WindowManager::getSingleton();

		//-------- Main Window Events ---------------------------------------------//
		//Quit
		wmgr.getWindow((utf8*)"Quit")->subscribeEvent(
			PushButton::EventClicked, Event::Subscriber(&GuiApplication::handleQuit, this));
		//Gain Level
		wmgr.getWindow((utf8*)"GainLevel")->subscribeEvent(
			Spinner::EventValueChanged, Event::Subscriber(&GuiApplication::handleGainChanged, this));
		//Create Effect
		wmgr.getWindow((utf8*)"Create")->subscribeEvent(
			PushButton::EventClicked, Event::Subscriber(&GuiApplication::handleCreate, this));
		//Remove Effect
		wmgr.getWindow((utf8*)"Remove")->subscribeEvent(
			PushButton::EventClicked, Event::Subscriber(&GuiApplication::handleRemove, this));
		//Current Effect selection
		wmgr.getWindow((utf8*)"EffectsList")->subscribeEvent(
			Listbox::EventSelectionChanged, Event::Subscriber(&GuiApplication::handleEffectChanged, this));

		//-------- General Properties Events ------------------------------------//
		wmgr.getWindow((utf8*)"ReplayLength")->subscribeEvent(
			Spinner::EventValueChanged, Event::Subscriber(&GuiApplication::handleReplayLength, this));

		//-------- Direction Events ---------------------------------------------//
		//-------- Constant Events ------------------------------------------------//
		wmgr.getWindow((utf8*)"ConstantLevel")->subscribeEvent(
			Spinner::EventValueChanged, Event::Subscriber(&GuiApplication::handleConstantLevel, this));

		//-------- Ramp Events ----------------------------------------------------//
		//-------- Periodic Events ------------------------------------------------//
		//-------- Conditional Events ---------------------------------------------//

	}

	//----------------------------------------------------------------//
	bool handleConstantLevel(const CEGUI::EventArgs& e)
	{
		OIS::JoyStick* joy = static_cast<GuiFrameListener*>(mFrameListener)->getJoyStick();
		OIS::ForceFeedback* ff = (OIS::ForceFeedback*)joy->queryInterface(OIS::Interface::ForceFeedback);
		CEGUI::WindowManager& wmgr = CEGUI::WindowManager::getSingleton();

		static_cast<OIS::ConstantEffect*>(SelectedEffect->getForceEffect())->level = 
			((CEGUI::Spinner*)wmgr.getWindow((CEGUI::utf8*)"ConstantLevel"))->getCurrentValue();
		
		ff->modify( SelectedEffect );
		return true;
	}

	//----------------------------------------------------------------//
	bool handleReplayLength(const CEGUI::EventArgs& e)
	{
		OIS::JoyStick* joy = static_cast<GuiFrameListener*>(mFrameListener)->getJoyStick();
		OIS::ForceFeedback* ff = (OIS::ForceFeedback*)joy->queryInterface(OIS::Interface::ForceFeedback);
		CEGUI::WindowManager& wmgr = CEGUI::WindowManager::getSingleton();
		unsigned int length = ((CEGUI::Spinner*)wmgr.getWindow((CEGUI::utf8*)"ReplayLength"))->getCurrentValue();
		if( length == 0 )
			SelectedEffect->replay_length = OIS::Effect::OIS_INFINITE;
		else
			SelectedEffect->replay_length = length * 1000000; //Seconds to Microseconds

		ff->modify( SelectedEffect );
		return true;
	}

	//----------------------------------------------------------------//
	bool handleEffectChanged(const CEGUI::EventArgs& e)
	{
		using namespace CEGUI;
		WindowManager& wmgr = WindowManager::getSingleton();
		Listbox* list = (Listbox*)wmgr.getWindow((utf8*)"EffectsList");
		ListboxTextItem *lti = (ListboxTextItem*)list->getFirstSelectedItem();

		//Make everything not visible
		wmgr.getWindow((utf8*)"Direction")->setVisible(false);
		wmgr.getWindow((utf8*)"General")->setVisible(false);
		wmgr.getWindow((utf8*)"ConstantForm")->setVisible(false);
		wmgr.getWindow((utf8*)"RampForm")->setVisible(false);
		wmgr.getWindow((utf8*)"PeriodicForm")->setVisible(false);
		wmgr.getWindow((utf8*)"ConditionalForm")->setVisible(false);
		wmgr.getWindow((utf8*)"EnvelopeForm")->setVisible(false);

		if( lti == 0 )
		{
			SelectedEffect = 0;
			return true;
		}

		//Every Force has possibility of direction
		wmgr.getWindow((utf8*)"Direction")->setVisible(true);
		//Every Force has general properties
		wmgr.getWindow((utf8*)"General")->setVisible(true);

		int handle = atoi(lti->getText().c_str());
		std::vector<OIS::Effect*>::iterator i = mEffects.begin(), end = mEffects.end();
		for( ; i != end; ++i )
		{
			if( handle == (*i)->_handle )
			{
				SelectedEffect = *i;

				switch( SelectedEffect->force )
				{
				case OIS::Effect::ConstantForce: updateConstantForm(); break;
				case OIS::Effect::RampForce: updateRampForm(); break;
				case OIS::Effect::PeriodicForce: updatePeriodicForm(); break;
				case OIS::Effect::ConditionalForce: updateConditionalForm(); break;
				default: break;
				}
			}
		}

		return true;
	}

	//-----------------------------------------------------------------------//
	void updateConstantForm()
	{
		using namespace CEGUI;
		WindowManager& wmgr = WindowManager::getSingleton();
		wmgr.getWindow((utf8*)"ConstantForm")->setVisible(true);
		wmgr.getWindow((utf8*)"EnvelopeForm")->setVisible(true);

		OIS::ConstantEffect* fe = (OIS::ConstantEffect*)SelectedEffect->getForceEffect();
	}
	void updateRampForm()
	{
		using namespace CEGUI;
		WindowManager& wmgr = WindowManager::getSingleton();
		wmgr.getWindow((utf8*)"RampForm")->setVisible(true);
		wmgr.getWindow((utf8*)"EnvelopeForm")->setVisible(true);

		OIS::RampEffect* fe = (OIS::RampEffect*)SelectedEffect->getForceEffect();
	}
	void updatePeriodicForm()
	{
		using namespace CEGUI;
		WindowManager& wmgr = WindowManager::getSingleton();
		wmgr.getWindow((utf8*)"PeriodicForm")->setVisible(true);
		wmgr.getWindow((utf8*)"EnvelopeForm")->setVisible(true);

		OIS::PeriodicEffect* fe = (OIS::PeriodicEffect*)SelectedEffect->getForceEffect();
	}
	void updateConditionalForm()
	{
		using namespace CEGUI;
		WindowManager& wmgr = WindowManager::getSingleton();
		wmgr.getWindow((utf8*)"ConditionalForm")->setVisible(true);

		OIS::ConditionalEffect* fe = (OIS::ConditionalEffect*)SelectedEffect->getForceEffect();
	}

	//----------------------------------------------------------------//
	bool handleRemove(const CEGUI::EventArgs& e)
	{
		using namespace CEGUI;
		WindowManager& wmgr = WindowManager::getSingleton();
		Listbox* list = (Listbox*)wmgr.getWindow((utf8*)"EffectsList");
		ListboxTextItem *lti = (ListboxTextItem*)list->getFirstSelectedItem();
		
		if( lti == 0 ) return true;

		int handle = atoi(lti->getText().c_str());
		std::vector<OIS::Effect*>::iterator i = mEffects.begin(), end = mEffects.end();
		for( ; i != end; ++i )
		{
			if( handle == (*i)->_handle )
			{
				OIS::JoyStick* joy = static_cast<GuiFrameListener*>(mFrameListener)->getJoyStick();
				OIS::ForceFeedback* ff = (OIS::ForceFeedback*)joy->queryInterface(OIS::Interface::ForceFeedback);
				ff->remove(*i);
				delete (*i);
				mEffects.erase(i);
				break;
			}
		}
		list->removeItem( lti );
		//Update displayed panels
		handleEffectChanged(e);

		return true;
	}

	//----------------------------------------------------------------//
	bool handleCreate(const CEGUI::EventArgs& e)
	{
		using namespace CEGUI;
		OIS::JoyStick* joy = static_cast<GuiFrameListener*>(mFrameListener)->getJoyStick();
		OIS::ForceFeedback* ff = (OIS::ForceFeedback*)joy->queryInterface(OIS::Interface::ForceFeedback);
		WindowManager& wmgr = WindowManager::getSingleton();
		Listbox* list = (Listbox*)wmgr.getWindow((utf8*)"SupportedEffectsList");

		if( list->getFirstSelectedItem() == 0 )
			return true;

		size_t index = list->getItemIndex( list->getFirstSelectedItem() );
		const OIS::ForceFeedback::SupportedEffectList &eff = ff->getSupportedEffects();
		OIS::ForceFeedback::SupportedEffectList::const_iterator it = eff.begin(), end = eff.end();
		for(int ind = 0; it != end; ++it, ++ind )
		{
			//Find the type of Effect to create based off of the selection index
			if( ind == index )
			{
				OIS::Effect* effect = new OIS::Effect(it->first, it->second);
				//Upload Effect with default settings
				ff->upload(effect);
				mEffects.push_back(effect);
				
				//Add to loaded effect list
				Listbox *listBox = static_cast<Listbox*>(WindowManager::getSingletonPtr()->getWindow("EffectsList"));
				std::ostringstream ss;
				ss << effect->_handle;
				ListboxTextItem* li = new ListboxTextItem(ss.str());
				li->setTextColours(CEGUI::ColourRect(colour(.5, 0, .3)));
				li->setSelectionColours(CEGUI::ColourRect(colour(0.2, 0.7, 0.7)));
				li->setSelectionBrushImage("WindowsLook", "TabButtonMiddleSelected" );
				listBox->addItem(li);
				break;
			}
		}
		return true;
	}

	//----------------------------------------------------------------//
	bool handleGainChanged(const CEGUI::EventArgs& e)
	{
		OIS::JoyStick* joy = static_cast<GuiFrameListener*>(mFrameListener)->getJoyStick();
		OIS::ForceFeedback* ff = (OIS::ForceFeedback*)joy->queryInterface(OIS::Interface::ForceFeedback);
		CEGUI::WindowManager& wmgr = CEGUI::WindowManager::getSingleton();
		float gain = ((CEGUI::Spinner*)wmgr.getWindow((CEGUI::utf8*)"GainLevel"))->getCurrentValue();

		ff->setMasterGain( gain );
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
		 MessageBox( NULL, e.getFullDescription().c_str(), "Exception!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
		#else
		 std::cerr << "Exception occured: " << e.getFullDescription().c_str() << std::endl;
		#endif
    }
	//Catch any OIS exceptions
    catch( OIS::Exception &oe )
    {
		#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
		 MessageBox( NULL, oe.eText, "Exception!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
		#else
		 std::cerr << "Exception occured: " << oe.eText << " " << oe.eLine << std::endl;
		#endif
	}
	//Catch any thing else that might have thrown... Perhaps CEGUI...
    catch(...)
	{
		#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
		 MessageBox( NULL, "Unknown", "Exception!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
		#else
		 std::cerr << "Exception ccured: Unknown Type! " << std::endl;
		#endif
	}

	return 0;
}
