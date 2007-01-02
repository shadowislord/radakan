#include "GranTurismOgre.h"

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
//------------------------------------------------------------------------------------------------
INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT )
#else
int main(int argc, char *argv[])
#endif
{
    GranTurismOgreApplication app;

    SET_TERM_HANDLER;
    
    try 
	{
		app.go();
    } 
	catch( Ogre::Exception& e )
	{
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
        MessageBox( 0, e.getFullDescription().c_str(), "An exception has occured!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
#else
        std::cerr << "An exception has occured: " << e.getFullDescription().c_str() << std::endl;
#endif
    }

    return 0;
}


using namespace Ogre;
using namespace OgreOde;
using namespace OgreOde_Prefab;
using namespace OgreOde_Loader;

//------------------------------------------------------------------------------------------------
static const String carNames[] = {
	"Jeep",
	"JeepSway",
	"Subaru"
};
static const String carFileNames[] = {
	"jeep.ogreode",
	"jeep.ogreode",
	"subaru.ogreode"
};
static int sSelectedCar = 1;
static int maxNumCar = 3;

//------------------------------------------------------------------------------------------------
GranTurismOgreFrameListener::GranTurismOgreFrameListener(RenderWindow* win, Camera* cam, Real time_step, Root *root, OgreOde::World *world) : 
	ExampleFrameListener(win, cam),
	_vehicle(0),
    _world(world)
{
	// Reduce move speed
    mMoveSpeed = 25;


    dotOgreOdeLoader = new OgreOde_Loader::DotLoader(world);

    const int _stepper_mode_choice = 2;
    const int _stepper_choice = 3;
    const Ogre::Real time_scale = Ogre::Real(1.0);
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
            time_step,
            max_frame_time,
            time_scale);

        break;
    case 1:
        _stepper = new OgreOde::ExactVariableStepHandler(_world, 
                                                            stepModeType, 
                                                                time_step,
                                                                max_frame_time,
                                                                time_scale);

        break;
    case 2:
        _stepper = new OgreOde::ForwardFixedStepHandler(_world, 
                                                    stepModeType, 
                                                    time_step,
                                                    max_frame_time,
                                                    time_scale);

        break;
    case 3:
    default:
        _stepper = new OgreOde::ForwardFixedInterpolatedStepHandler (_world, 
                                                                    stepModeType, 
                                                                    time_step,
                                                                    frame_rate,
                                                                    max_frame_time,
                                                                    time_scale);
        break;
    }

	_stepper->setAutomatic(OgreOde::StepHandler::AutoMode_PostFrame, root);
	//_stepper->setAutomatic(OgreOde::Stepper::AutoMode_PreFrame, root);

	Root::getSingleton().setFrameSmoothingPeriod(5.0f);
	//Root::getSingleton().setFrameSmoothingPeriod(0.0f);
	
	changeCar();

	// Load up our UI and display it
	Overlay* pOver = (Overlay*)OverlayManager::getSingleton().getByName("OgreOdeDemos/Overlay");    
	OverlayManager::getSingleton().getOverlayElement("OgreOdeDemos/Name")->setCaption(String("Name: ") + "GranTurismo (car + trimesh)");
	OverlayManager::getSingleton().getOverlayElement("OgreOdeDemos/Keys")->setCaption(String("Keys: ") + "I/K - Accelerate/Brake, J/L - Turn, X - Change drive mode, N - Change Car");
	OverlayManager::getSingleton().getOverlayElement("OgreOdeDemos/OtherKeys")->setCaption(String("Extra: ") + "E - Debug Object");
	pOver->show();

}

//------------------------------------------------------------------------------------------------
GranTurismOgreFrameListener::~GranTurismOgreFrameListener()
{
	delete _stepper;
}
//------------------------------------------------------------------------------------------------
void GranTurismOgreFrameListener::changeCar()
{
	sSelectedCar = (sSelectedCar + 1) % maxNumCar;

	delete _vehicle;	
    _vehicle = static_cast <OgreOde_Prefab::Vehicle *> (dotOgreOdeLoader->loadObject (carFileNames[sSelectedCar], carNames[sSelectedCar]));



	// Initially (i.e. in the config file) it's rear wheel drive
	_drive = 'R';

	// Move the vehicle
	Vector3 v_pos = mCamera->getPosition() + (mCamera->getDirection() * 15.0);
	//v_pos.y += 10;
	_vehicle->setPosition(v_pos);

	updateInfo();  
}
//------------------------------------------------------------------------------------------------
void GranTurismOgreFrameListener::updateInfo()
{
	Overlay* pOver = (Overlay*)OverlayManager::getSingleton().getByName("OgreOdeDemos/Overlay"); 
	String newInfo (String("Info: ") + carNames[sSelectedCar]);
	switch(_drive)
	{
		// Switch from rear to front
	case 'R':
		newInfo = newInfo + " & Front wheel drive";
		break;

		// Switch from front to all
	case 'F':
		newInfo = newInfo + " & All wheel drive";
		break;

		// Switch from all to rear
	case '4':
		newInfo = newInfo + " & Rear wheel drive";
		break;
	}
	OverlayManager::getSingleton().getOverlayElement("OgreOdeDemos/Info")->setCaption(newInfo);   
}
//------------------------------------------------------------------------------------------------
bool GranTurismOgreFrameListener::frameStarted(const FrameEvent& evt)
{
	Real time = evt.timeSinceLastFrame;

    bool ret = ExampleFrameListener::frameStarted(evt);

	if (mTimeUntilNextToggle <= 0) 
	{
		// Switch debugging objects on or off
		if (mInputDevice->isKeyDown(KC_E))
		{
            _world->setShowDebugGeometries(!_world->getShowDebugGeometries());
			mTimeUntilNextToggle = 0.5;
        }
        // Switch debugging Contacts on or off
        if (mInputDevice->isKeyDown(KC_B))
        {
            _world->setShowDebugContact(!_world->getShowDebugContact());
            mTimeUntilNextToggle = 0.5;
        }

		if(mInputDevice->isKeyDown(KC_N)) 
		{
			changeCar();
			mTimeUntilNextToggle = 0.5;
		}

		if(mInputDevice->isKeyDown(KC_U)) 
		{
			_stepper->pause(false);
			mTimeUntilNextToggle = 0.5;
		}
		if(mInputDevice->isKeyDown(KC_P)) 
		{
			_stepper->pause(true);
			mTimeUntilNextToggle = 0.5;
		}
		// Change the drive mode between front, rear and 4wd
		if ((mInputDevice->isKeyDown(KC_X)))
		{
			switch(_drive)
			{
				// Switch from rear to front
			case 'R':
				_drive = 'F';

				_vehicle->getWheel(0)->setPowerFactor(1);
				_vehicle->getWheel(1)->setPowerFactor(1);
				_vehicle->getWheel(2)->setPowerFactor(0);
				_vehicle->getWheel(3)->setPowerFactor(0);

				updateInfo();
				break;

				// Switch from front to all
			case 'F':
				_drive = '4';

				_vehicle->getWheel(0)->setPowerFactor(0.6);
				_vehicle->getWheel(1)->setPowerFactor(0.6);
				_vehicle->getWheel(2)->setPowerFactor(0.4);
				_vehicle->getWheel(3)->setPowerFactor(0.4);

				updateInfo();
				break;

				// Switch from all to rear
			case '4':
				_drive = 'R';

				_vehicle->getWheel(0)->setPowerFactor(0);
				_vehicle->getWheel(1)->setPowerFactor(0);
				_vehicle->getWheel(2)->setPowerFactor(1);
				_vehicle->getWheel(3)->setPowerFactor(1);

				updateInfo();
				break;
			}
			mTimeUntilNextToggle = 0.5;
		}
	}
	if(!_stepper->isPaused())
	{
		_vehicle->setInputs(mInputDevice->isKeyDown(KC_J),
                            mInputDevice->isKeyDown(KC_L),
                            mInputDevice->isKeyDown(KC_I),
                            mInputDevice->isKeyDown(KC_K));
		_vehicle->update(time);

		// Thanks to Ahmed!
		const Ogre::Real followFactor = 0.2; 
		const Ogre::Real camHeight = 2.0; 
		const Ogre::Real camDistance = 7.0; 
		const Ogre::Real camLookAhead = 8.0;

		Quaternion q = _vehicle->getSceneNode()->getOrientation(); 
		Vector3 toCam = _vehicle->getSceneNode()->getPosition(); 

		toCam.y += camHeight; 
		toCam.z -= camDistance * q.zAxis().z; 
		toCam.x -= camDistance * q.zAxis().x; 
	      
		mCamera->move( (toCam - mCamera->getPosition()) * followFactor ); 
		mCamera->lookAt(_vehicle->getSceneNode()->getPosition() + ((_vehicle->getSceneNode()->getOrientation() * Ogre::Vector3::UNIT_Z) * camLookAhead));
	}
	return ret;
}
//------------------------------------------------------------------------------------------------
GranTurismOgreApplication::GranTurismOgreApplication()
{
	_world = 0;
	_time_step = 0.01;
	_track = 0;
}
//------------------------------------------------------------------------------------------------
void GranTurismOgreApplication::chooseSceneManager(void)
{
    mSceneMgr = mRoot->createSceneManager( ST_GENERIC, "ExampleGrandTurismo" );
}
//------------------------------------------------------------------------------------------------
void GranTurismOgreApplication::setupResources(void)
{
	ExampleApplication::setupResources();
	ResourceGroupManager *rsm = ResourceGroupManager::getSingletonPtr();
	StringVector groups = rsm->getResourceGroups();        
	if (std::find(groups.begin(), groups.end(), String("OgreOde")) == groups.end())
	{
		rsm->createResourceGroup("OgreOde");
		try
		{
			rsm->addResourceLocation("../../../../../ogreaddons/ogreode/demos/Media","FileSystem", "OgreOde");
		}
		catch (Ogre::Exception& e)
		{
			String error = e.getFullDescription();
			rsm->addResourceLocation("../../../ogreode/demos/Media","FileSystem", "OgreOde");
		}
	}
}
//------------------------------------------------------------------------------------------------
void GranTurismOgreApplication::createCamera(void)
{
    // Create the camera
    mCamera = mSceneMgr->createCamera("PlayerCam");

    mCamera->setPosition(Vector3(125,-14,8));
    mCamera->lookAt(mCamera->getPosition() + Ogre::Vector3(0,0,1));
    mCamera->setNearClipDistance( 1 );
    mCamera->setFarClipDistance( 1000 );
}
//------------------------------------------------------------------------------------------------
// Just override the mandatory create scene method
void GranTurismOgreApplication::createScene(void)
{
    // Set ambient light
    mSceneMgr->setAmbientLight(ColourValue(0.5, 0.5, 0.5));
	mSceneMgr->setSkyBox(true,"GranTurismOgre/Skybox");

    // Create a light
    Light* l = mSceneMgr->createLight("MainLight");

	// Accept default settings: point light, white diffuse, just set position
    // NB I could attach the light to a SceneNode if I wanted it to move automatically with
    //  other objects, but I don't
    l->setPosition(20,800,50);
	l->setSpecularColour(1,0.9,0);

	l->setCastShadows(true);

	mSceneMgr->setShadowTechnique(SHADOWTYPE_STENCIL_MODULATIVE);
	mSceneMgr->setShadowColour(ColourValue(0.5,0.5,0.5));

    _world = new OgreOde::World(mSceneMgr);

    _world->setGravity(Vector3(0,-9.80665,0));
    _world->setCFM(10e-5);
    _world->setERP(0.8);
    _world->setAutoSleep(true);
    _world->setContactCorrectionVelocity(1.0);

	// Create something that will step the world automatically



	_world->setCollisionListener(this);

	SceneNode *track_node = mSceneMgr->getRootSceneNode()->createChildSceneNode("track");
	Entity *track_mesh = mSceneMgr->createEntity("track","racingcircuit.mesh");
	track_node->attachObject(track_mesh);

	OgreOde::EntityInformer ei(track_mesh);
	_track = ei.createStaticTriangleMesh(_world, _world->getDefaultSpace());
    track_mesh->setUserObject (_track);
  
}
//------------------------------------------------------------------------------------------------
// Create new frame listener
void GranTurismOgreApplication::createFrameListener(void)
{
    mFrameListener= new GranTurismOgreFrameListener(mWindow, mCamera,_time_step,mRoot, _world);
    mRoot->addFrameListener(mFrameListener);
}
//------------------------------------------------------------------------------------------------
bool GranTurismOgreApplication::collision(OgreOde::Contact* contact)
{
	if(!OgreOde_Prefab::Vehicle::handleTyreCollision(contact))
	{
		contact->setBouncyness(0.0);
		contact->setCoulombFriction(18.0);
	}
	return true;
}
//------------------------------------------------------------------------------------------------
GranTurismOgreApplication::~GranTurismOgreApplication()
{
	delete _track;
	delete _world;
}

