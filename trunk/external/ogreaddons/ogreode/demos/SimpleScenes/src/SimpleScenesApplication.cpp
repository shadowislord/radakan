/*
SimpleScenesApplication.cpp
--------------------------
The main applicatin that handles switching between the
different scenes in this demo, as well as any common 
setup and input handling.
*/
#include "SimpleScenesApplication.h"

// The tests we can display
#include "SimpleScenes_BoxStack.h"
#include "SimpleScenes_Buggy.h"
#include "SimpleScenes_Chain.h"
#include "SimpleScenes_TriMesh.h"
#include "SimpleScenes_Crash.h"
#include "SimpleScenes_Joints.h"
#include "SimpleScenes_Zombie.h"

// Windows stuff
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include "windows.h"

/*
Windows entry point
*/
INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT nCmdShow)
#else
int main(int argc,char* argv[])
#endif
{
	// Create the application and try to run it
    SimpleScenesApplication app;

    try
	{
		app.go();
    } 
	catch(Ogre::Exception& e)
	{
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
		MessageBox( 0, e.getFullDescription().c_str(), "OgreOde SimpleScenes: Exception", MB_OK | MB_ICONERROR | MB_TASKMODAL);
#else
        std::cerr << "An exception has occured: " << e.getFullDescription().c_str() << std::endl;
#endif
    }
    return 0;
}

using namespace Ogre;

#if !(OGRE_VERSION <  ((1 << 16) | (3 << 8) | 0))
using namespace OIS;
#endif //OGRE_VERSION not heihort

using namespace OgreOde;
using namespace OgreOde_Prefab;
using namespace OgreOde_Loader;

/*
The frame listener is informed before every frame
*/
bool SimpleScenesFrameListener::frameStarted(const FrameEvent& evt)
{

	// Do the default demo input handling and keep our UI display
	// in sync with the other stuff (frame rate, logo, etc)
	bool show = mStatsOn;
	bool bOK = ExampleFrameListener::frameStarted(evt);
	if (mStatsOn != show)
	{
	    Overlay* pOver = (Overlay*)OverlayManager::getSingleton().getByName("OgreOdeDemos/Overlay");    
		if (pOver)
		{
			if (mStatsOn) 
                pOver->show();
			else 
                pOver->hide();
		}
	}
    // Tell the demo application that it needs to handle input	
#if (OGRE_VERSION <  ((1 << 16) | (3 << 8) | 0))
    _demo->frameStarted(evt,mInputDevice);
#else
    _demo->frameStarted(evt, mKeyboard, mMouse);
#endif //OGRE_VERSION not heihort

	// Quit or carry on according to the normal demo input
	return bOK;
}

/*
The frame listener is informed after every frame
*/
bool SimpleScenesFrameListener::frameEnded(const FrameEvent& evt)
{
	// Tell our demo that the frame's ended before doing default processing

#if (OGRE_VERSION <  ((1 << 16) | (3 << 8) | 0))
    _demo->frameEnded(evt,mInputDevice);
#else
    _demo->frameEnded(evt, mKeyboard, mMouse);
#endif //OGRE_VERSION not heihort
    return ExampleFrameListener::frameEnded(evt);
}

/*
Create the scene from an Ogre point of view
and create the common OgreOde things we'll need
*/
void SimpleScenesApplication::createScene(void)
{
	MovableObject::setDefaultQueryFlags (ANY_QUERY_MASK);

#ifndef _DEBUG
	// Set up shadowing
	mSceneMgr->setShadowTechnique(SHADOWTYPE_TEXTURE_MODULATIVE);
    mSceneMgr->setShadowColour(ColourValue(0.5, 0.5, 0.5));
	mSceneMgr->setShadowFarDistance(30);

	if (StringUtil::startsWith(mRoot->getRenderSystem()->getName(),"direct")) 
        mSceneMgr->setShadowTextureSettings(1024, 2);
    else 
        mSceneMgr->setShadowTextureSettings(512, 2);
#endif

	// Add some default lighting to the scene
	mSceneMgr->setAmbientLight(ColourValue(0.25, 0.25, 0.25));

	// Create a directional light to shadow and light the bodies
	_spot = mSceneMgr->createLight("Spot");
	_spot->setType(Light::LT_DIRECTIONAL);
	_spot->setDirection(-0.40824828,-0.81649655,-0.40824828);
	_spot->setCastShadows(true);
    _spot->setSpecularColour(1,1,0.8);

	// Give us some sky
	mSceneMgr->setSkyBox(true,"kk3d/DesertVII",5000,true);

	// Position and orient the camera
	mCamera->setPosition(13,4.5,0);
	mCamera->lookAt(0,0.5,0);
	mCamera->setNearClipDistance(0.5);

	// Create the ODE world
	_world = new OgreOde::World(mSceneMgr);

	_world->setGravity(Vector3(0,-9.80665,0));
	_world->setCFM(10e-5);
	_world->setERP(0.8);
	_world->setAutoSleep(true);
    _world->setAutoSleepAverageSamplesCount(10);
	_world->setContactCorrectionVelocity(1.0);

	// Create something that will step the world, but don't do it automatically

    const int _stepper_mode_choice = 2;
    const int _stepper_choice = 3;
    const Ogre::Real time_scale = Ogre::Real(1.7);
    const Ogre::Real max_frame_time = Ogre::Real(1.0 / 4);
    const Ogre::Real frame_rate = Ogre::Real(1.0 / 60);


    StepHandler::StepModeType stepModeType;

    switch(_stepper_mode_choice)
    {
    case 0: stepModeType = StepHandler::BasicStep; break;
    case 1: stepModeType = StepHandler::FastStep; break;
    case 2: stepModeType = StepHandler::QuickStep; break;

    default: stepModeType = StepHandler::QuickStep; break;
    }

    switch (_stepper_choice)
    {
    case 0:
        _stepper = new OgreOde::StepHandler(_world,
            StepHandler::QuickStep, 
            _time_step,
            max_frame_time,
            time_scale);

        break;
    case 1:
        _stepper = new OgreOde::ExactVariableStepHandler(_world, 
            stepModeType, 
            _time_step,
            max_frame_time,
            time_scale);

        break;
    case 2:
        _stepper = new OgreOde::ForwardFixedStepHandler(_world, 
            stepModeType, 
            _time_step,
            max_frame_time,
            time_scale);

        break;
    case 3:
    default:
        _stepper = new OgreOde::ForwardFixedInterpolatedStepHandler (_world, 
            stepModeType, 
            _time_step,
            frame_rate,
            max_frame_time,
            time_scale);
        break;
    }

    //_stepper->setAutomatic(OgreOde::StepHandler::AutoMode_PostFrame, mRoot);
    //_stepper->setAutomatic(OgreOde::Stepper::AutoMode_PreFrame, mRoot);

    Root::getSingleton().setFrameSmoothingPeriod(5.0f);
    //Root::getSingleton().setFrameSmoothingPeriod(0.0f);

	// Create a default plane to act as the ground
	_plane = new OgreOde::InfinitePlaneGeometry(Plane(Vector3(0,1,0),0), _world, _world->getDefaultSpace());

	// Use a load of meshes to represent the floor
    int i = 0;
    StaticGeometry* s;
    s = mSceneMgr->createStaticGeometry("StaticFloor");
    s->setRegionDimensions(Vector3(160.0, 100.0, 160.0));
    // Set the region origin so the center is at 0 world
    s->setOrigin(Vector3::ZERO);
	for (Real z = -80.0;z <= 80.0;z += 20.0)
	{
		for (Real x = -80.0;x <= 80.0;x += 20.0)
		{
			String name = String("Plane_") + StringConverter::toString(i++);
			
			Entity* entity = mSceneMgr->createEntity(name, "plane.mesh");
            entity->setQueryFlags (STATIC_GEOMETRY_QUERY_MASK);
            entity->setUserObject(_plane);
			entity->setCastShadows(false);
            s->addEntity(entity, Vector3(x,0,z));
		}
    }
    s->build();
    //SceneNode* mPlaneNode = mSceneMgr->getRootSceneNode()->createChildSceneNode(name);

	// Load up our UI and display it
    Overlay* pOver = (Overlay*)OverlayManager::getSingleton().getByName("OgreOdeDemos/Overlay");    
    pOver->show();

	// Initialise stuff
	_test = 0;
    _delay = 1.0;
}

/*
The frame listener will notify us when a frame's 
about to be started, so we can update stuff
*/

#if (OGRE_VERSION <  ((1 << 16) | (3 << 8) | 0))
void SimpleScenesApplication::frameStarted(const FrameEvent& evt,InputReader* mInputDevice)
#else
void SimpleScenesApplication::frameStarted(const FrameEvent& evt, OIS::Keyboard* mKeyboard, OIS::Mouse* mMouse)
#endif //OGRE_VERSION not heihort
{
	// If we're looking at something then adjust the camera
	if (_test && _test->getLastNode())
	{
		if (_looking)
		{
			mCamera->lookAt(_test->getLastNode()->getPosition());

			if (_chasing)
			{
				// Thanks to Ahmed!
				const Ogre::Real followFactor = 0.1; 
				const Ogre::Real camHeight = 5.0; 
				const Ogre::Real camDistance = 7.0; 
				
				Quaternion q = _test->getLastNode()->getOrientation(); 
				Vector3 toCam = _test->getLastNode()->getPosition(); 

				toCam.y += camHeight; 
				toCam.z -= camDistance * q.zAxis().z; 
				toCam.x -= camDistance * q.zAxis().x; 
      
				mCamera->move( (toCam - mCamera->getPosition()) * followFactor ); 
			}
		}
	}

	// Set the shadow distance according to how far we are from the plane that receives them
	mSceneMgr->setShadowFarDistance((fabs(mCamera->getPosition().y) + 1.0) * 3.0);

	// If we're running a test, tell it that a frame's ended
    if ((_test)&&(!_paused))
    {
#if (OGRE_VERSION <  ((1 << 16) | (3 << 8) | 0))
        _test->frameStarted(evt.timeSinceLastFrame, mInputDevice);
#else
        _test->frameStarted(evt.timeSinceLastFrame, mKeyboard, mMouse);
#endif //OGRE_VERSION not heihort
    }
}

/*
The frame listener will let us know when a frame's ended. So we
can do stuff that we can't do in a frame started event
e.g. delete things that we can't delete at the start of a frame,
presumably because some processing has already been done, leaving
things dangling, like particle systems.
*/
#if (OGRE_VERSION <  ((1 << 16) | (3 << 8) | 0))
void SimpleScenesApplication::frameEnded(const FrameEvent& evt,InputReader* myInput)
#else
void SimpleScenesApplication::frameEnded(const FrameEvent& evt, OIS::Keyboard* myInput, OIS::Mouse* mMouse)
#endif //OGRE_VERSION not heihort
{
	Real time = evt.timeSinceLastFrame;

	// If we're running a test, tell it that a frame's ended
    // If we're running a test, tell it that a frame's ended
    if ((_test)&&(!_paused))
    {
#if (OGRE_VERSION <  ((1 << 16) | (3 << 8) | 0))
        _test->frameEnded(time, myInput);
#else
        _test->frameEnded(time, myInput, mMouse);
#endif //OGRE_VERSION not heihort
    }

	
	// Step the world and then synchronise the scene nodes with it, 
	// we could get this to do this automatically, but we 
	// can't be sure of what order the frame listeners will fire in
	if (_stepper->step(time))
    {
		_world->synchronise(); 
    }

	_delay += time;
	if (_delay > 1.0)
	{
		bool changed = false;

		// Switch the test we're displaying
		if (myInput->isKeyDown(KC_F1))
		{
			delete _test;
			_test = new SimpleScenes_BoxStack(_world);
			changed = true;
		}
		else if (myInput->isKeyDown(KC_F2))
		{
			delete _test;
			_test = new SimpleScenes_Chain(_world);
			changed = true;
		}
		else if (myInput->isKeyDown(KC_F3))
		{
			delete _test;
			_test = new SimpleScenes_Buggy(_world);
			changed = true;
		}
		else if (myInput->isKeyDown(KC_F4))
		{
			delete _test;
			_test = new SimpleScenes_TriMesh(_world);
			changed = true;
		}
		else if (myInput->isKeyDown(KC_F5))
		{
			delete _test;
			_test = new SimpleScenes_Crash(_world);
			changed = true;
		}
		else if (myInput->isKeyDown(KC_F6))
		{
			delete _test;
			_test = new SimpleScenes_Joints(_world);

			if (mCamera->getPosition().z < 10.0)
			{
				Vector3 pos = mCamera->getPosition();
				mCamera->setPosition(pos.x,pos.y,10.0);
				mCamera->lookAt(0,0,0);
			}
			changed = true;
		}
		else if (myInput->isKeyDown(KC_F7))
		{
			delete _test;
			_test = new SimpleScenes_Zombie(_world);

			changed = true;
		}

		// If we changed the test...
		if ((_test)&&(changed))
		{
			// Register it with the stepper, so we can (for example) add forces before each step
			_stepper->setStepListener(_test);

			// Set the UI to show the test's details
			OverlayManager::getSingleton().getOverlayElement("OgreOdeDemos/Name")->setCaption("Name: " + _test->getName());
			OverlayManager::getSingleton().getOverlayElement("OgreOdeDemos/Keys")->setCaption("Keys: " + _test->getKeys());

			_delay = 0.0;
		}

		// Switch shadows
		if (myInput->isKeyDown(KC_SPACE))
		{
            static Ogre::uint shadowtype = 0;
            shadowtype += 1;
            if (shadowtype > 5)
                shadowtype = 0;
            switch (shadowtype)
            {
            case 0:
                mSceneMgr->setShadowTechnique (SHADOWTYPE_NONE);	
                break;
            case 1:
                mSceneMgr->setShadowTechnique(SHADOWTYPE_TEXTURE_MODULATIVE);
                mSceneMgr->setShadowColour(ColourValue(0.5, 0.5, 0.5));
                mSceneMgr->setShadowFarDistance(30);
                if (StringUtil::startsWith(mRoot->getRenderSystem()->getName(),"direct")) 
                    mSceneMgr->setShadowTextureSettings(1024, 2);
                else 
                    mSceneMgr->setShadowTextureSettings(512, 2);
                break;
            case 2:
                mSceneMgr->setShadowTechnique (SHADOWTYPE_STENCIL_ADDITIVE);	
                break;
            case 3:
                mSceneMgr->setShadowTechnique (SHADOWTYPE_STENCIL_MODULATIVE );	
                break;
            case 4:
                mSceneMgr->setShadowTechnique (SHADOWTYPE_TEXTURE_ADDITIVE );	 
                mSceneMgr->setShadowColour(ColourValue(0.5, 0.5, 0.5));
                mSceneMgr->setShadowFarDistance(30);
                if (StringUtil::startsWith(mRoot->getRenderSystem()->getName(),"direct")) 
                    mSceneMgr->setShadowTextureSettings(1024, 2);
                else 
                    mSceneMgr->setShadowTextureSettings(512, 2);
                break;
            default:
                mSceneMgr->setShadowTechnique (SHADOWTYPE_NONE);	
                break;
            }
			_delay = 0.0;
		}

		// Look at the last object, chase it, or not
		if (myInput->isKeyDown(KC_M))
		{
			if (_looking)
            {
                if (_chasing) 
                {
                    _looking = _chasing = false;
                }
                else
                {
                    _chasing = true;
                }
            }
			else 
            {
                _looking = true;
            }
			_delay = 0.0;
		}

		// Switch debugging objects on or off
		if (myInput->isKeyDown(KC_E))
		{
			_world->setShowDebugGeometries(!_world->getShowDebugGeometries());
			_delay = 0.0;
		}

        // Switch debugging Contacts on or off
        if (myInput->isKeyDown(KC_B))
        {
            _world->setShowDebugContact(!_world->getShowDebugContact());
            _delay = 0.5;
        }

		// Pause or Run the simulation
		if (myInput->isKeyDown(KC_P))
		{
			_paused = !_paused;
			_delay = 0.0;

			_stepper->pause(_paused);

            const Ogre::Real timeSet = (_paused)? 0.0 : 1.0;

            Ogre::SceneManager::MovableObjectIterator it = 
                mSceneMgr->getMovableObjectIterator(ParticleSystemFactory::FACTORY_TYPE_NAME);
            while(it.hasMoreElements())
            {
                ParticleSystem* p = static_cast<ParticleSystem*>(it.getNext());
                p->setSpeedFactor(timeSet);
            }
		}
	}
}

/*
Destructor, delete stuff we created
*/
SimpleScenesApplication::~SimpleScenesApplication(void)
{
	delete _test;
	delete _plane;
	delete _stepper;	
	delete _world;
}
