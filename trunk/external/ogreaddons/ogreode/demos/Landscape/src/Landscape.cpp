#include "Landscape.h"




//enable this to test it against PLSM2 instead of TSM.
//#define _PLSM2
//
#ifndef _PLSM2
    //static String config_file  ("landscape.cfg");
    //static String config_file("landscape1.cfg");
    //static String config_file("landscape2.cfg");
    //static String config_file("landscape3.cfg");
    //static String config_file("landscape4.cfg");
    static String config_file("landscape5.cfg");
#else  //_PLSM2
    static String config_file ("paginglandscape2.cfg");
#endif //_PLSM2


#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include "windows.h"

INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT )
#else
int main(int argc, char *argv[])
#endif
{
    LandscapeApplication app;

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
static int sSelectedCar = 0;
static int maxNumCar = 3;


//------------------------------------------------------------------------------------------------
LandscapeFrameListener::LandscapeFrameListener(RenderWindow* win, Camera* cam,
                                               Root *root) : 
        ExampleFrameListener(win, cam),
        mSceneMgr (mCamera->getSceneManager()),
        _world (new OgreOde::World(mSceneMgr)),
        _terrain(0),
	    _vehicle(0),
        _time_step (0.01)
{

    _world->setGravity(Vector3(0,-9.80665,0));
    _world->setCFM(10e-5);
    _world->setERP(0.8);
    _world->setAutoSleep(true);
    _world->setContactCorrectionVelocity(1.0);

    dotOgreOdeLoader = new OgreOde_Loader::DotLoader (_world);

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
	
	_stepper->setAutomatic(OgreOde::StepHandler::AutoMode_PostFrame,root);
 

    bool does_center_is_corner = false;
    Ogre::Vector3 scale = Ogre::Vector3::ZERO;
    int nodes_per_side = 0;
    int nodes_per_side_all_pagesW = 0; 
    int nodes_per_side_all_pagesH = 0;
    bool center;
    // Terrain Scene Manager should support those... 
    mSceneMgr->getOption("Scale", &scale);
    mSceneMgr->getOption("PageSize", &nodes_per_side);

   Ogre::Real worldWidth, worldHeight; 
#ifndef _PLSM2
    if (nodes_per_side == 0)
    {
        Ogre::ConfigFile config;
        Ogre::String val;

        config.load(config_file, "OgreOde", "=", true);

        val = config.getSetting( "PageSize" );
        assert( !val.empty() );
        nodes_per_side = atoi( val.c_str() );

        val = config.getSetting( "PageWorldX" );
        assert( !val.empty() );
        scale.x = atof( val.c_str() ) / nodes_per_side;

        val = config.getSetting( "MaxHeight" );
        assert( !val.empty() );
        scale.y = atof( val.c_str() );

        val = config.getSetting( "PageWorldZ" );
        assert( !val.empty() );
        scale.z = atof( val.c_str() ) / nodes_per_side;

    }
    center = false;
    worldWidth  = scale.x * (nodes_per_side - 1);
    worldHeight = scale.z * (nodes_per_side - 1); 
    nodes_per_side_all_pagesW = nodes_per_side; 
    nodes_per_side_all_pagesH = nodes_per_side;
#else    
    center = true;

    int w=0;
    int h=0;

    mSceneMgr->getOption("Width", &w);
    mSceneMgr->getOption("Height", &h);


    worldWidth  = w * (nodes_per_side - 1) * scale.x;
    worldHeight = h * (nodes_per_side - 1) * scale.z;

    nodes_per_side_all_pagesW = w * (nodes_per_side);
    nodes_per_side_all_pagesH = h * (nodes_per_side);

    bool noInterpolation = true;
    mSceneMgr->setOption("queryNoInterpolation", &noInterpolation);
    mSceneMgr->setOption("LoadNow", 0);
#endif 

    _terrain = new OgreOde::TerrainGeometry(_world,
        _world->getDefaultSpace(), 
        scale,
        nodes_per_side_all_pagesW,
        nodes_per_side_all_pagesH,
        worldWidth, 
        worldHeight,
        center);
    //_terrain->setHeightListener(this);
    _world->setCollisionListener(this);

    // Create some boxes to play with
#ifdef _DEBUG
    const Ogre::Real boxspace = 24.0;
#else
    const Ogre::Real boxspace = 12.0;
#endif
    for(Real z = -12.0;z <= 12.0;z += boxspace)
    {
        for(Real x = -12.0;x <= 12.0;x += boxspace)
        {   
            // Create the Ogre box
            String name = String("Box_") + StringConverter::toString(x) + "_" + StringConverter::toString(z);
            Entity* box = mSceneMgr->createEntity(name,"crate.mesh");
            box->setCastShadows(true);

            SceneNode* node = mSceneMgr->getRootSceneNode()->createChildSceneNode(name);
            node->attachObject(box);
            node->setScale(0.15,0.15,0.15);

            // Set the position
            Ogre::Vector3 pos(50 + x,130,50 + z);
            node->setPosition(pos);

            // Create a box for ODE and attach it to the Ogre version
            OgreOde::Body* body = new OgreOde::Body(_world);
            node->attachObject(body);
            body->setMass(OgreOde::BoxMass(0.5,Vector3(1.5,1.5,1.5)));

            OgreOde::BoxGeometry* geom = new OgreOde::BoxGeometry(Vector3(1.5,1.5,1.5), _world, _world->getDefaultSpace());
            geom->setBody(body);

        }
    }


	// Reduce move speed
    mMoveSpeed = 25;


    _ray_query = mCamera->getSceneManager()->createRayQuery(ray);
    _ray_query->setQueryTypeMask(Ogre::SceneManager::WORLD_GEOMETRY_TYPE_MASK );
    _ray_query->setWorldFragmentType(Ogre::SceneQuery::WFT_SINGLE_INTERSECTION); 
    ray.setDirection(Vector3::UNIT_Y);


	Root::getSingleton().setFrameSmoothingPeriod(5.0f);
	
    changeCar();

	// Load up our UI and display it
	Overlay* pOver = (Overlay*)OverlayManager::getSingleton().getByName("OgreOdeDemos/Overlay"); 
	OverlayManager::getSingleton().getOverlayElement("OgreOdeDemos/Name")->setCaption(String("Name: ") + "Landscape (Terrain + vehicle + box)");
	OverlayManager::getSingleton().getOverlayElement("OgreOdeDemos/Keys")->setCaption(String("Keys: ") + "I/K - Accelerate/Brake, J/L - Turn, X - Change drive mode, N - Change Car");
	OverlayManager::getSingleton().getOverlayElement("OgreOdeDemos/OtherKeys")->setCaption(String("Extra: ") + "E - Debug Object");
	pOver->show();


}

LandscapeFrameListener::~LandscapeFrameListener()
{
	delete _stepper;
	mCamera->getSceneManager()->destroyQuery(_ray_query);
    delete _terrain;
    delete _world;
}

//------------------------------------------------------------------------------------------------
void LandscapeFrameListener::changeCar()
{
	sSelectedCar = (sSelectedCar + 1) % maxNumCar;

	delete _vehicle;
	//_vehicle = new OgreOde_Prefab::Vehicle(carNames[sSelectedCar]);
	//_vehicle->load(carFileNames[sSelectedCar]);
	_vehicle = static_cast <OgreOde_Prefab::Vehicle *> (dotOgreOdeLoader->loadObject (carFileNames[sSelectedCar], carNames[sSelectedCar]));
	// Initially (i.e. in the config file) it's rear wheel drive
	_drive = 'R';

	// Move the vehicle
	Vector3 v_pos = mCamera->getPosition() + (mCamera->getDirection() * 15.0);
	
    v_pos.y += 50.0;
    {
        ray.setOrigin(v_pos);
        _ray_query->setRay(ray);
        RaySceneQueryResult& result = _ray_query->execute();
        RaySceneQueryResult::iterator i = result.begin();
        if (i != result.end() && i->worldFragment)
        {
            const Ogre::Real height = i->worldFragment->singleIntersection.y;
            if (v_pos.y < height)
                v_pos.y = height + 7.0;
               
        }
    }

	_vehicle->setPosition(v_pos);

	updateInfo();
}
//------------------------------------------------------------------------------------------------
void LandscapeFrameListener::updateInfo()
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
bool LandscapeFrameListener::frameStarted(const FrameEvent& evt)
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
		if ((mInputDevice->isKeyDown(KC_X))&& _vehicle)
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

	if(!_stepper->isPaused() && _vehicle)
	{
		_vehicle->setInputs(mInputDevice->isKeyDown(KC_J),mInputDevice->isKeyDown(KC_L),mInputDevice->isKeyDown(KC_I),mInputDevice->isKeyDown(KC_K));
		_vehicle->update(time);
	}


	// Thanks to Ahmed!
    if (_vehicle)
    {
	    const Ogre::Real followFactor = 0.05; 
	    const Ogre::Real camHeight = 5.0; 
	    const Ogre::Real camDistance = 10.0; 
	    const Ogre::Real camLookAhead = 6.0;

	    Quaternion q = _vehicle->getSceneNode()->getOrientation(); 
	    Vector3 toCam = _vehicle->getSceneNode()->getPosition(); 

	    toCam.y += camHeight; 
	    toCam.z -= camDistance * q.zAxis().z; 
	    toCam.x -= camDistance * q.zAxis().x; 
          
	    mCamera->move( (toCam - mCamera->getPosition()) * followFactor ); 
	    mCamera->lookAt(_vehicle->getSceneNode()->getPosition() + ((_vehicle->getSceneNode()->getOrientation() * Ogre::Vector3::UNIT_Z) * camLookAhead));
    }
    if (_average_num_query == 0)
        _average_num_query = _terrain->getNumQueries();
    _average_num_query = (_average_num_query*99 + _terrain->getNumQueries()) / 100;
   
    mWindow->setDebugText(
        (_vehicle?"Vehicle Pos: " + StringConverter::toString(_vehicle->getSceneNode()->getPosition()):String("")) +
        " nQueries: " + StringConverter::toString(_average_num_query));

    _terrain->resetNumQueries();
	return ret;
}

//------------------------------------------------------------------------------------------------
Real LandscapeFrameListener::heightAt(const Ogre::Vector3& position)
{
    return _terrain->getHeightAt(position); //this is the get height function 
}


//------------------------------------------------------------------------------------------------
bool LandscapeFrameListener::collision(OgreOde::Contact* contact)
{
    Geometry * const g1 = contact->getFirstGeometry ();
    Geometry * const g2 = contact->getSecondGeometry ();
    const bool isg1Sphere = g1 && g1->getClass () != Geometry::Class_Sphere;
    const bool isg2Sphere = g2 && g2->getClass () != Geometry::Class_Sphere;
   
    if((isg1Sphere || isg2Sphere) && 
        OgreOde_Prefab::Vehicle::handleTyreCollision(contact))
    {
            //handled by Vehicle.
    }
    else
    {
        contact->setBouncyness(0.0);
        contact->setCoulombFriction(18.0);
    }

    return true;
}

//------------------------------------------------------------------------------------------------
LandscapeApplication::LandscapeApplication()
{
}

//------------------------------------------------------------------------------------------------
bool LandscapeApplication::setup(void)
{
#ifndef _PLSM2
	mRoot = new Root("plugins.cfg", "ogre.cfg", "ogreode_landscape_tsm.log");
#else //_PLSM2
	mRoot = new Root("plugins_plsm2.cfg", "plsm2_display.cfg", "ogreode_landscape_plsm2.log");
#endif //_PLSM2

	setupResources();

	bool carryOn = configure();
	if (!carryOn) return false;

	chooseSceneManager();
	createCamera();
	createViewports();

	// Set default mipmap level (NB some APIs ignore this)
	TextureManager::getSingleton().setDefaultNumMipmaps(20);

	// Create any resource listeners (for loading screens)
	createResourceListener();
	// Load resources
	loadResources();

	// Create the scene
	createScene();

	createFrameListener();

	return true;

}

//------------------------------------------------------------------------------------------------
void LandscapeApplication::chooseSceneManager(void)
{
#ifndef _PLSM2
    mSceneMgr = mRoot->createSceneManager( ST_EXTERIOR_CLOSE, "basicsm" );
#else //_PLSM2
	mSceneMgr = mRoot->createSceneManager( "PagingLandScapeSceneManager", "basicsm" );
	assert(mSceneMgr);
#endif //_PLSM2
}

//------------------------------------------------------------------------------------------------
void LandscapeApplication::setupResources(void)
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
#ifdef _PLSM2
	if (std::find(groups.begin(), groups.end(), String("PLSM2")) == groups.end())
	{
		rsm->createResourceGroup("PLSM2");
		try
		{
			rsm->addResourceLocation("../../../../../ogreaddons/paginglandscape/Samples/Media/paginglandscape2","FileSystem", "PLSM2");

			rsm->addResourceLocation("../../../../../ogreaddons/paginglandscape/Samples/Media/paginglandscape2/models","FileSystem", "PLSM2");
			rsm->addResourceLocation("../../../../../ogreaddons/paginglandscape/Samples/Media/paginglandscape2/overlays","FileSystem", "PLSM2");
			rsm->addResourceLocation("../../../../../ogreaddons/paginglandscape/Samples/Media/paginglandscape2/materials","FileSystem", "PLSM2");

			rsm->addResourceLocation("../../../../../ogreaddons/paginglandscape/Samples/Media/paginglandscape2/materials/scripts","FileSystem", "PLSM2");
			rsm->addResourceLocation("../../../../../ogreaddons/paginglandscape/Samples/Media/paginglandscape2/materials/textures","FileSystem", "PLSM2");
			rsm->addResourceLocation("../../../../../ogreaddons/paginglandscape/Samples/Media/paginglandscape2/materials/programs","FileSystem", "PLSM2");

			rsm->addResourceLocation("../../../../../ogreaddons/paginglandscape/Samples/Media/paginglandscape2/datasrcs","FileSystem", "PLSM2");
			rsm->addResourceLocation("../../../../../ogreaddons/paginglandscape/Samples/Media/paginglandscape2/terrains","FileSystem", "PLSM2");
		}
		catch (Ogre::Exception& e)
		{
			String error = e.getFullDescription();
			rsm->addResourceLocation("../../../paginglandscape/Samples/Media/paginglandscape2","FileSystem", "PLSM2");

			rsm->addResourceLocation("../../../paginglandscape/Samples/Media/paginglandscape2/models","FileSystem", "PLSM2");
			rsm->addResourceLocation("../../../paginglandscape/Samples/Media/paginglandscape2/overlays","FileSystem", "PLSM2");
			rsm->addResourceLocation("../../../paginglandscape/Samples/Media/paginglandscape2/materials","FileSystem", "PLSM2");

			rsm->addResourceLocation("../../../paginglandscape/Samples/Media/paginglandscape2/materials/scripts","FileSystem", "PLSM2");
			rsm->addResourceLocation("../../../paginglandscape/Samples/Media/paginglandscape2/materials/textures","FileSystem", "PLSM2");
			rsm->addResourceLocation("../../../paginglandscape/Samples/Media/paginglandscape2/materials/programs","FileSystem", "PLSM2");

			rsm->addResourceLocation("../../../paginglandscape/Samples/Media/paginglandscape2/datasrcs","FileSystem", "PLSM2");
			rsm->addResourceLocation("../../../paginglandscape/Samples/Media/paginglandscape2/terrains","FileSystem", "PLSM2");
		}
	}
#endif //_PLSM2
}

//------------------------------------------------------------------------------------------------
void LandscapeApplication::createCamera(void)
{
    // Create the camera
    mCamera = mSceneMgr->createCamera("PlayerCam");

	// Set a nice viewpoint
	mCamera->setPosition(25,15,25);
	mCamera->setOrientation(Quaternion(-0.3486, 0.0122, 0.9365, 0.0329));

    mCamera->setNearClipDistance( 1 );
    mCamera->setFarClipDistance( 1000 );
}

//------------------------------------------------------------------------------------------------
// Just override the mandatory create scene method
void LandscapeApplication::createScene(void)
{
    // Set ambient light
    mSceneMgr->setAmbientLight(ColourValue(0.5, 0.5, 0.5));

    // Create a light
    Light* l = mSceneMgr->createLight("MainLight");

	// Accept default settings: point light, white diffuse, just set position
    // NB I could attach the light to a SceneNode if I wanted it to move automatically with
    //  other objects, but I don't
    l->setPosition(20,800,50);
	l->setSpecularColour(1,0.9,0);
	l->setCastShadows(true);

#ifndef _DEBUG
	mSceneMgr->setShadowTechnique(SHADOWTYPE_STENCIL_MODULATIVE);
	mSceneMgr->setShadowColour(ColourValue(0.5,0.5,0.5));
#else
	mSceneMgr->setShadowTechnique(SHADOWTYPE_NONE);
#endif

    // Fog
    // NB it's VERY important to set this before calling setWorldGeometry 
    // because the vertex program picked will be different
    ColourValue fadeColour(0.93, 0.86, 0.76);
    mSceneMgr->setFog( FOG_LINEAR, fadeColour, .001, 500, 1000);
    mWindow->getViewport(0)->setBackgroundColour(fadeColour);

	// Set up the terrain according to our own settings	

	DataStreamPtr cfgFile = ResourceGroupManager::getSingleton().openResource (config_file, "OgreOde");
    mSceneMgr -> setWorldGeometry( cfgFile );


	//String blank("");
	//mSceneMgr->setOption("CustomMaterialName",&blank);

    // Infinite far plane?
    if (mRoot->getRenderSystem()->getCapabilities()->hasCapability(RSC_INFINITE_FAR_PLANE))
    {
		mCamera->setFarClipDistance(0);
    }

    // Define the required skyplane
    Plane plane;
    
	plane.d = 5000;
    plane.normal = -Vector3::UNIT_Y;


}

//------------------------------------------------------------------------------------------------
// Create new frame listener
void LandscapeApplication::createFrameListener(void)
{
    mFrameListener= new LandscapeFrameListener(mWindow, mCamera,mRoot);
    mRoot->addFrameListener(mFrameListener);
}

//------------------------------------------------------------------------------------------------
LandscapeApplication::~LandscapeApplication()
{
}
