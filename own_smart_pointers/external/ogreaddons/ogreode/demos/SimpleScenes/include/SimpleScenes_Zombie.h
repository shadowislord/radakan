/*
SimpleScenes_RagDoll.h
---------------------
A demo RagDoll shooting demo, showing ragdolls and ray intersections.
*/
#ifndef _SIMPLESCENES_ZOMBIE_H_
#define _SIMPLESCENES_ZOMBIE_H_

static const String xmlNames[] = {
    "zombie",
    "zombie",
    "ninja",
    "robot"
};
static const String meshNames[] = {
    "zombie_small.mesh",
    "zombie_small.mesh",
    "ninja.mesh",
    "robot.mesh"
};
static const String meshAnimation [] = {
    "Walk1",
    "Walk1",
    "Walk",
    "Walk"
};
static const Ogre::Vector3 meshScale [] = {
    Ogre::Vector3(1.0, 1.0, 1.0),
    Ogre::Vector3(1.0, 1.0, 1.0),
    Ogre::Vector3(0.02, 0.02, 0.02),
    Ogre::Vector3(0.05, 0.05, 0.05)
};
static const String ragdollFile [] = {
    "zombie.ogreode",
    "zombie_auto.ogreode",
    "ninja.ogreode",
    "robot.ogreode"
};
static size_t sSelectedMesh = 0;

#ifdef _DEBUG
	static size_t sMaxRagdoll = 10;
#else //_DEBUG
	static size_t sMaxRagdoll = 30;
#endif //_DEBUG

/*
The zombie demo extends the base test class
*/
class SimpleScenes_Zombie:public SimpleScenes
{
private:
    typedef std::list<OgreOde_Prefab::Ragdoll *> RagDollList;

    struct ragDollData
    {
       int  sSelectedMesh;
      Ogre::Real timeSinceBirth;
	  Ogre::Real timeSincePhysical;
    };

public:
	void createRagDoll()
    {

		_last_ragdoll = 0.0;
	
        OgreOde_Prefab::Ragdoll *ragdoll = static_cast <OgreOde_Prefab::Ragdoll *> (dotOgreOdeLoader->loadObject(ragdollFile[sSelectedMesh],
                                                                             xmlNames[sSelectedMesh],
                                                                             "zombie" + StringConverter::toString(_ragdoll_count++)));



		ragdoll->setCastShadows(true);		
		ragdoll->setQueryFlags (ZOMBIE_QUERY_MASK);

        Any a = ragdoll->getUserAny ();
        assert (a.isEmpty ());
        {
            ragDollData *b  = new ragDollData ();

            b->timeSinceBirth = 0.0f;
            b->timeSincePhysical = 0.0f;
            b->sSelectedMesh = sSelectedMesh;
			

            Any *newA = new Any(b);                        
            ragdoll->setUserAny (*newA);
        }


		// Add entity to the scene node
		SceneNode *ragdoll_node = _mgr->getRootSceneNode()->createChildSceneNode(ragdoll->getName() + "Node");
		ragdoll_node->attachObject(ragdoll);
		ragdoll_node->yaw(Degree(rand() % 360));
		ragdoll_node->setPosition(0.0,0.0,0);
        ragdoll_node->setScale( meshScale[sSelectedMesh]);

		_last_node = ragdoll_node;

        
		ragdoll->getAnimationState(meshAnimation[sSelectedMesh])->setEnabled(true);

        myRagDolls.push_back (ragdoll);


        //sSelectedMesh = (sSelectedMesh + 1) % 4;
	}

	// Constructor
    SimpleScenes_Zombie(OgreOde::World *world) :
        SimpleScenes(world)
	{
		_over = (Overlay*)OverlayManager::getSingleton().getByName("OgreOdeDemos/Target");    
		_over->show();

		_gun = _mgr->createEntity("gun","gun.mesh");
		_gun->setCastShadows(false);

		_gun_node = _mgr->getRootSceneNode()->createChildSceneNode("gun");
		_gun_node->attachObject(_gun);

		_camera = _mgr->getCamera("PlayerCam");

		_animation_speed = 1.0;
		_ragdoll_count = 0;
		createRagDoll();

		_shot_time = 0.0;

		myOdeRay = new OgreOde::RayGeometry(1000.0, _world, _space);
		mRayQuery = _mgr->createRayQuery(Ray());
		mRayQuery->setQueryMask (ZOMBIE_QUERY_MASK);
		mRayQuery->setQueryTypeMask(SceneManager::ENTITY_TYPE_MASK);
	}
	
	// Destructor
	virtual ~SimpleScenes_Zombie()
    {    
        for ( RagDollList::iterator zE = myRagDolls.begin();
            zE != myRagDolls.end(); ++zE)
        {
            assert ((*zE)->getParentNode ());
            assert ((*zE)->getParentNode ()->getParent());
            (static_cast<SceneNode*> ((*zE)->getParentNode ()->getParent()))->removeAndDestroyChild(
                (*zE)->getParentNode ()->getName ());
            _mgr->destroyMovableObject((*zE)->getName(), OgreOde_Prefab::RagdollFactory::FACTORY_TYPE_NAME);
                
		}   
		delete myOdeRay;

		_over->hide();
		_mgr->destroySceneNode("gun");
		_mgr->destroyEntity("gun");
		_mgr->destroyQuery (mRayQuery);
	}

	// Return our name for the test application to display
	virtual const String& getName()
	{
		static String name = "RagDoll Shooting Gallery";
		return name;
	}

	// Return a description of the keys that the user can use in this test
	virtual const String& getKeys()
	{
		static String keys = "X - Shoot";
		return keys;
	}

   
#if (OGRE_VERSION <  ((1 << 16) | (3 << 8) | 0))
    virtual void frameStarted(Real time, InputReader* input)
#else
    virtual void frameStarted(Real time, OIS::Keyboard* input, OIS::Mouse* mouse)
#endif //OGRE_VERSION not heihort
	{
		_last_ragdoll += time;

        // Do default key handling
#if (OGRE_VERSION <  ((1 << 16) | (3 << 8) | 0))
        SimpleScenes::frameEnded(time, input);
#else
        SimpleScenes::frameEnded(time, input, mouse);
#endif //OGRE_VERSION not heihort

		_gun_node->setOrientation(_camera->getOrientation());
		_gun_node->setPosition(_camera->getPosition() + (_camera->getOrientation() * Ogre::Vector3(0.3,-0.15,-1.1)));

        for ( RagDollList::iterator zE = myRagDolls.begin();
            zE != myRagDolls.end(); 
            ++zE)
        {   
            ragDollData *b = any_cast<ragDollData *> ((*zE)->getUserAny());
            if ((*zE)->isPhysical())
            {
                if (!(*zE)->isStatic())
                {

                    b->timeSincePhysical += time;
                    (*zE)->update();// RagDoll Activated
                }
            }
            else
            {
                (*zE)->getAnimationState(meshAnimation[b->sSelectedMesh])->addTime(time * _animation_speed);

                (*zE)->getParentNode ()->translate(
                    (*zE)->getParentNode ()->getOrientation() * (Vector3::UNIT_Z * time * 2.5));
                
            }
        }

	}


#if (OGRE_VERSION <  ((1 << 16) | (3 << 8) | 0))
    // Handle the user's key presses	
    virtual void frameEnded(Real time,InputReader* input)
    {
        // Do default key handling
        SimpleScenes::frameEnded(time,input);
#else

    virtual void frameEnded(Real time, OIS::Keyboard* input, OIS::Mouse* mouse)
    {
        // Do default processing
        SimpleScenes::frameEnded(time, input, mouse);
#endif
		_shot_time -= time;

		if (_key_delay > 0 &&
            input->isKeyDown(KC_C))
        {  
            for ( RagDollList::iterator zE = myRagDolls.begin();
                zE != myRagDolls.end(); 
                ++zE)
            {
                if ((*zE)->isPhysical())
                {
                    (*zE)->releasePhysicalControl();
                    ragDollData *b = any_cast<ragDollData *> ((*zE)->getUserAny());
                    (*zE)->getAnimationState(meshAnimation[b->sSelectedMesh])->setEnabled(true);
                }
            }

            _key_delay = SimpleScenes::KEY_DELAY;
		}



        if (_shot_time <= 0.0 &&             
#if (OGRE_VERSION <  ((1 << 16) | (3 << 8) | 0))
             (input->isKeyDown(KC_X) || input->getMouseButton (0)))
#else
            ( input->isKeyDown(KC_X) || mouse->getMouseState().buttonDown(OIS::MB_Left)))
#endif
		{

			Ray pickRay = _camera->getCameraToViewportRay(0.5, 0.5);
			myOdeRay->setDefinition(pickRay.getOrigin(), pickRay.getDirection());

			_shot_time = 0.2;

			mRayQuery->setRay (pickRay);
			const RaySceneQueryResult& result = mRayQuery->execute();
			if (!result.empty())
			{
				RaySceneQueryResult::const_iterator i = result.begin();
		
				//mRayQuery->setSortByDistance (true, 1);//only one hit
				myOdeRay->enable ();
				while((i != result.end()))
				{

					_last_ragdoll = 5.0;

					OgreOde_Prefab::Ragdoll * const radgoll =
						static_cast<OgreOde_Prefab::Ragdoll *> (i->movable);

				   // uncomment to see nice debug mesh of animated mesh
//#define _TESTTRIMESH
#ifdef _TESTTRIMESH 

					// ray cast could be tested against that instead of ragdoll.
					// depending on complexity of mesh could be simpler
					OgreOde::EntityInformer ei((*zE), (*zE)->getParentNode ()->_getFullTransform());
					_geoms.push_back(ei.createStaticTriangleMesh(_space));
#else //_TESTTRIMESH
					bool wasPhysical = radgoll->isPhysical ();
					if (!wasPhysical)
					{
						ragDollData * const b = any_cast<ragDollData *> (radgoll->getUserAny());
						radgoll->getAnimationState(meshAnimation[b->sSelectedMesh])->setEnabled(false);

						
						// Create the ragdoll
						radgoll->takePhysicalControl(_world, _space, false);
						radgoll->setSelfCollisions(false);

					}


                    OgreOde::Body *hit_body;
                    Ogre::Vector3 hit_point;

					bool is_hit = false;
					if (radgoll->pick(myOdeRay, hit_body, hit_point))
					{
						if (hit_body)
						{
							hit_body->addForceAt(pickRay.getDirection() * 250000, hit_point);
							is_hit = true;
							ragDollData *b = any_cast<ragDollData *> (radgoll->getUserAny());
							b->timeSincePhysical = 0.0f;
						}
					}

					if (!is_hit&& !wasPhysical)
					{
						radgoll->releasePhysicalControl();
						ragDollData *b = any_cast<ragDollData *> (radgoll->getUserAny());
						radgoll->getAnimationState(meshAnimation[b->sSelectedMesh])->setEnabled(true);
					}
#endif //_TESTTRIMESH	

					++i;
				}	// if results.	
			}
			myOdeRay->disable ();
        }
			
	           
		for ( RagDollList::iterator zE = myRagDolls.begin();
			zE != myRagDolls.end(); )
		{

			OgreOde_Prefab::Ragdoll * const radgoll = (*zE);
			ragDollData *b = any_cast<ragDollData *> (radgoll->getUserAny());
			b->timeSinceBirth += time;

			// turn to stone to improve fps,
			// better way to do that would be to tweak
			// simulation parameters to be less jitty.
			// better auto sleep
			if (radgoll->isPhysical() 
				&& !radgoll->isStatic() 
				&& radgoll->isAwake()
				&& b->timeSincePhysical > 5.0)
			{
				//radgoll->turnToStone();
				radgoll->sleep();
			}
			
            
			if ( b->timeSinceBirth > sMaxRagdoll)
			{
				(static_cast<SceneNode*> (radgoll->getParentNode ()->getParent()))->removeAndDestroyChild(
					radgoll->getParentNode ()->getName ());
				_mgr->destroyMovableObject(radgoll->getName(), OgreOde_Prefab::RagdollFactory::FACTORY_TYPE_NAME);
				zE = myRagDolls.erase(zE);
			}
			else
			{
				++zE;
			}
		}
        if (_last_ragdoll > 5.0 && 
            myRagDolls.size() < static_cast<size_t>(sMaxRagdoll))
            createRagDoll();
		
	}

protected:
	RagDollList myRagDolls;
	RaySceneQuery *mRayQuery;

	Real _animation_speed;

	Entity *_gun;
	SceneNode *_gun_node;

	OgreOde::RayGeometry *myOdeRay;

	Camera* _camera;
	Overlay* _over;

	Real _shot_time;

	Real _last_ragdoll;
	int _ragdoll_count;
};

#endif
