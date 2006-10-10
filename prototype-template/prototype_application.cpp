
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

	assert (ogre_window != NULL);

	ogre_renderer = ogre_root->getRenderSystem();

	// Platform specific stuff (used in rendering loop - see run()).
	ogre_platform_manager = & PlatformManager::getSingleton();

	ogre_scene_manager = ogre_root->createSceneManager(ST_GENERIC, "PrototypeApplication");

	// Create our camera
	ogre_camera = ogre_scene_manager->createCamera("ThePlayer");
	ogre_camera->setPosition(Vector3(300,300,1000));
	ogre_camera->lookAt(Vector3(100,50,300));
	ogre_camera->setNearClipDistance(5);

	// Create a new view port from our active window
	// Apply our viewport to the camera.
	Viewport *vp = ogre_window->addViewport(ogre_camera);
	vp->setBackgroundColour(ColourValue(0,0,0));

	// Aspect ratio corrections
	ogre_camera->setAspectRatio(Real(vp->getActualWidth())/Real(vp->getActualHeight()));
	// Create and add our input frame listener
	input = new Input(ogre_window, ogre_camera);
	ogre_root->addFrameListener(input);

       // Set up GUI system
        cegui_renderer = new CEGUI::OgreCEGUIRenderer(ogre_window, Ogre::RENDER_QUEUE_OVERLAY, false, 3000, ogre_scene_manager);
        cegui_system = new CEGUI::System(cegui_renderer);

}

void PrototypeApplication::Load_Resources()
{
	// Actually all this line of code does is delegate the parameters to
	// addResourceLocation().
	ogre_root->addResourceLocation("./Models/", "FileSystem", "Models", true);
	ogre_root->addResourceLocation("./Sky/", "FileSystem", "Skies", true);
	ogre_root->addResourceLocation("./Textures/", "FileSystem", "Textures", true);

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

		// Interestingly, if you return a false within your frameStarted
		// in your frame listener, this will return false.
		if(!ogre_root->renderOneFrame()) break;
	}

	return true;
}

void PrototypeApplication::Create_Scene(void)
{
	    ogre_scene_manager->setSkyDome( true, "Peaceful" );

        // Create a light
        Light* 	light_source_1 = ogre_scene_manager->createLight("light_source_1");
				light_source_1->setType(Light::LT_POINT);
				light_source_1->setPosition(-800, 1000, 400);
				light_source_1->setDiffuseColour(1, 1, 1);
				light_source_1->setSpecularColour(1, 1, 1);

		ogre_scene_manager->setAmbientLight( ColourValue( 0.1, 0.1, 0.1 ) );
		ogre_scene_manager->setShadowTechnique( SHADOWTYPE_STENCIL_MODULATIVE );

		// World interactive objects
		Entity *wagon = ogre_scene_manager->createEntity("Wagon", "OgreWagon.mesh");

		// World objects
		Entity *table = ogre_scene_manager->createEntity("Table", "Table.mesh");
		Entity *small_pot = ogre_scene_manager->createEntity("SmallPot", "Pot.mesh");
		Entity *large_pot = ogre_scene_manager->createEntity("LargePot", "Pot2.mesh");



		// World elements
		Entity *house_style1 = ogre_scene_manager->createEntity("HouseStyle1", "House.mesh");

		// Fences
		Entity *fence_1 = ogre_scene_manager->createEntity("Fence1", "fences1.mesh");
		Entity *fence_2 = ogre_scene_manager->createEntity("Fence2", "fences2.mesh");
		Entity *fence_3 = ogre_scene_manager->createEntity("Fence3", "fences3.mesh");
		Entity *fence_4 = ogre_scene_manager->createEntity("Fence4", "fences4.mesh");
		Entity *fence_5 = ogre_scene_manager->createEntity("Fence5", "fences5.mesh");

		// Enable shadows
		wagon			->setCastShadows( true );
		table			->setCastShadows( true );
		small_pot		->setCastShadows( true );
		large_pot		->setCastShadows( true );
		house_style1	->setCastShadows( true );


		fence_1			->setCastShadows( true );
		fence_2			->setCastShadows( true );
		fence_3			->setCastShadows( true );
		fence_4			->setCastShadows( true );
		fence_5			->setCastShadows( true );


        // Assign entities to nodes
        // Usable entities
	    SceneNode *wagon_node = ogre_scene_manager->getRootSceneNode()->createChildSceneNode( "WagonNode" );
        wagon_node->attachObject( wagon );
        wagon_node->scale( .1, .1, .1 );

        // World objects
	    SceneNode *table_node = ogre_scene_manager->getRootSceneNode()->createChildSceneNode( "TableNode" );
        table_node->attachObject( table );
        table_node->scale( 2, 2, 2 );
		table_node->setPosition(300, 0, 0);

	    SceneNode *small_pot_node = ogre_scene_manager->getRootSceneNode()->createChildSceneNode( "SmallPotNode" );
        small_pot_node->attachObject( small_pot );
		small_pot_node->setPosition(300, 0, 250);

	    SceneNode *large_pot_node = ogre_scene_manager->getRootSceneNode()->createChildSceneNode( "LargePotNode" );
        large_pot_node->attachObject( large_pot );
		large_pot_node->setPosition(120, 0, 0);

		// World elements
	    SceneNode *house_style1_node = ogre_scene_manager->getRootSceneNode()->createChildSceneNode( "HouseStyle1Node" );
        house_style1_node->attachObject( house_style1 );
		house_style1_node->setPosition(300, 0, 200);

		SceneNode *fence1_node = ogre_scene_manager->getRootSceneNode()->createChildSceneNode( "Fence1Node" );
		fence1_node->attachObject(fence_1);
		fence1_node->setPosition(-300, 0, 350);

		SceneNode *fence2_node = ogre_scene_manager->getRootSceneNode()->createChildSceneNode( "Fence2Node" );
		fence2_node->attachObject(fence_2);
		fence2_node->setPosition(-200, 0, 350);

		SceneNode *fence3_node = ogre_scene_manager->getRootSceneNode()->createChildSceneNode( "Fence3Node" );
		fence3_node->attachObject(fence_3);
		fence3_node->setPosition(-100, 0, 350);


		SceneNode *fence4_node = ogre_scene_manager->getRootSceneNode()->createChildSceneNode( "Fence4Node" );
		fence4_node->attachObject(fence_4);
		fence4_node->setPosition(0, 0, 350);


		SceneNode *fence5_node = ogre_scene_manager->getRootSceneNode()->createChildSceneNode( "Fence5Node" );
		fence5_node->attachObject(fence_5);
		fence5_node->setPosition(100, 0, 350);


		// Generate ground
		Plane plane( Vector3::UNIT_Y, 0 );
		MeshManager::getSingleton().createPlane("ground",
		   ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, plane,
		   10000,10000,10,10,true,1,5,5,Vector3::UNIT_Z);

	   // Create an attach the ground entity
       Entity *ground = ogre_scene_manager->createEntity( "GroundEntity", "ground" );
       ogre_scene_manager->getRootSceneNode()->createChildSceneNode()->attachObject(ground);

       ground->setMaterialName("Grass");
       ground->setCastShadows(false);

      // Create the billboard chain
      /*BillboardChain *lightning = new BillboardChain("lightning", 1000, 2, true, true, true);
      lightning->setMaterialName("Peaceful");
      for (int i = 0; i < 1000; i++)
         lightning->addChainElement(1, BillboardChain::Element(Vector3(Math::Sin(i / 100.0f * 2 * Math::PI),
                                                            Math::Cos(i / 100.0f * 2 * Math::PI) ,
                                                            i / 100.0f),
															0.1f,
															i / 10.0f,
															ColourValue(1.0f, 1.0, 1.0)));

      // Add it to the scene
      SceneNode *lightning_node = ogre_scene_manager->getRootSceneNode()->createChildSceneNode( "LightningNode" );

      lightning_node->attachObject(lightning);
      lightning_node->setPosition(100, 100, 100);
      lightning_node->scale(10, 10, 10);*/

}




