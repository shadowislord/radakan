
#include "prototype_application.hpp"

using namespace Ogre;

PrototypeApplication::PrototypeApplication()
{
	// Define where the plugins and settings cfg files are from
	ogre_root = new Root("plugins.cfg","settings.cfg");

	// Load configuration from settings.cfg
	ogre_root->restoreConfig();

	// Create a new window based on the settings
	ogre_window = ogre_root->initialise(true, "Scattered Lands Prototype Application");

	ogre_renderer = ogre_root->getRenderSystem();

	// Platform specific stuff (used in rendering loop - see run()).
	ogre_platform_manager = new PlatformManager();

	ogre_scene_manager = ogre_root->createSceneManager(ST_GENERIC, "PrototypeApplication");

	// Create our camera
	ogre_camera = ogre_scene_manager->createCamera("ThePlayer");
	ogre_camera->setPosition(Vector3(0,0,0));
	ogre_camera->lookAt(Vector3(0,0,-300));
	ogre_camera->setNearClipDistance(5);

	// Create a new view port from our active window
	// Apply our viewport to the camera.
	Viewport *vp = ogre_window->addViewport(ogre_camera);
	vp->setBackgroundColour(ColourValue(0,0,255));

	// Aspect ratio corrections
	ogre_camera->setAspectRatio(Real(vp->getActualWidth())/Real(vp->getActualHeight()));

}

void PrototypeApplication::Load_Resources()
{
	// Actually all this line of code does is delegate the parameters to
	// addResourceLocation().
	ogre_root->addResourceLocation("Models/", "FileSystem", "Models", true);

	ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
}

bool PrototypeApplication::Run()
{
	Load_Resources();
	Create_Scene();

	// Initialise renderer
	ogre_renderer->_initRenderTargets();

	while(!ogre_window->isClosed())
	{
		// Quick info: Interestingly, this actually just delegates the messagePump
		// depending on the OS.
		ogre_platform_manager->messagePump(ogre_window);

		if(!ogre_root->renderOneFrame()) break;
	}

	return true;
}

void PrototypeApplication::Create_Scene(void)
    {

        // Create a light
        Light* myLight = ogre_scene_manager->createLight("Light0");
        myLight->setType(Light::LT_POINT);
        myLight->setPosition(0, 1000, 100);
        myLight->setDiffuseColour(1, 1, 1);
        myLight->setSpecularColour(1, 1, 1);

		ogre_scene_manager->setAmbientLight( ColourValue( 0.1, 0.1, 0.1 ) );
		ogre_scene_manager->setShadowTechnique( SHADOWTYPE_STENCIL_MODULATIVE );

        //OgreWagon.mesh
        Entity *ent1 = ogre_scene_manager->createEntity( "Wagon", "OgreWagon.mesh" );
		ent1->setCastShadows( true );

        Entity *ent2 = ogre_scene_manager->createEntity( "Table", "Table.mesh" );
		ent2->setCastShadows( true );

        Entity *ent3 = ogre_scene_manager->createEntity( "Pot", "Pot.mesh" );
		ent3->setCastShadows( true );

        Entity *ent4 = ogre_scene_manager->createEntity( "Pot2", "Pot2.mesh" );
		ent3->setCastShadows( true );


        SceneNode *node1 = ogre_scene_manager->getRootSceneNode()->createChildSceneNode( "WagonNode" );
        node1->attachObject( ent1 );
        node1->scale( .2, .2, .2 );

        SceneNode *node2 = ogre_scene_manager->getRootSceneNode()->createChildSceneNode( "TableNode" );
        node2->attachObject( ent2 );
        node2->scale( 2, 2, 2 );
		node2->setPosition(300, 0, 0);

        SceneNode *node3 = ogre_scene_manager->getRootSceneNode()->createChildSceneNode( "PotNode" );
        node3->attachObject( ent3 );
		node3->setPosition(120, 0, 0);

        SceneNode *node4 = ogre_scene_manager->getRootSceneNode()->createChildSceneNode( "Pot2Node" );
        node4->attachObject( ent4 );
		node4->setPosition(300, 0, 200);


		Plane plane( Vector3::UNIT_Y, 0 );
		MeshManager::getSingleton().createPlane("ground",
		   ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, plane,
		   1000,1000,20,20,true,1,5,5,Vector3::UNIT_Z);


       Entity *ent = ogre_scene_manager->createEntity( "GroundEntity", "ground" );
       ogre_scene_manager->getRootSceneNode()->createChildSceneNode()->attachObject(ent);

       ent->setMaterialName("Examples/Rockwall");
       ent->setCastShadows(false);

    }




