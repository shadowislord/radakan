/*
SimpleScenes_Chain.h
----------------------
A reimplementation of the ODE spherical chain demo 
using Ogre and the OgreOde wrapper.
*/
#ifndef _SIMPLESCENESCHAIN_H_
#define _SIMPLESCENES_CHAIN_H_

/*
The chain test extends the base test class
*/
class SimpleScenes_Chain:public SimpleScenes
{
public:
	// Standard constructor, creates everything in the demo
    SimpleScenes_Chain(OgreOde::World *world) :
      SimpleScenes(world)
	{
		// Set up the sizes of stuff
		Real link_size = 0.4,geom_size = 0.4,adjust = 0.1;
		OgreOde::Body* last_body = 0;
		int num_links = 10;

		_first_body = 0;
		_force = 100;

		// Create all the links in the chain
		for (int i = num_links - 1;i >= 0;i--)
		{
			Real np = (Real)i * (link_size + adjust);
			String name = String("Sphere_") + StringConverter::toString(i);

			// Visuals
			Entity* entity = _mgr->createEntity(name,"ball.mesh");
			entity->setNormaliseNormals(true);
			entity->setCastShadows(true);

			SceneNode* node = _mgr->getRootSceneNode()->createChildSceneNode(entity->getName());
			node->attachObject(entity);
			node->setScale(link_size * 0.2,link_size * 0.2,link_size * 0.2);
			node->setPosition(Vector3(np,np + 0.4,np));

			// Physicals
			OgreOde::EntityInformer ei(entity,Matrix4::getScale(node->getScale()));
			OgreOde::Body* body = ei.createSingleDynamicSphere(1.0,_world, _space);
			_bodies.push_back(body);
			_geoms.push_back(body->getGeometry(0));

			// Join the current body to the last one (if there was a last one)
			if (!_first_body)
			{
				_last_node = node;
				_first_body = body;
			}

			if (last_body)
			{
				OgreOde::BallJoint* joint = new OgreOde::BallJoint(_world);
				joint->attach(body,last_body);
				Real ja = ((Real)i + 0.5) * (link_size + adjust);
				joint->setAnchor(Vector3(ja,ja + 0.4,ja));

				_joints.push_back(joint);
			}

			last_body = body;
		}

		// Create some static boxes to bump into
		createBox(0,2,Vector3(4,1,4));
		createBox(1,2,Vector3(-4,1,4));
		createBox(2,2,Vector3(-4,1,-4));
		createBox(3,2,Vector3(4,1,-4));
	}

      // Let the user throw the chain around
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
		_force_to_apply = Ogre::Vector3::ZERO;

		Vector3 right = _mgr->getCamera("PlayerCam")->getRight();
		Vector3 forward = right.crossProduct(Vector3::UNIT_Y);

		// Up
		if (input->isKeyDown(KC_X)) _force_to_apply += Ogre::Vector3::UNIT_Y * _force;

		// Left/right
		if (input->isKeyDown(KC_J)) _force_to_apply -= right * _force;
		if (input->isKeyDown(KC_L)) _force_to_apply += right * _force;

		// Forward/back
		if (input->isKeyDown(KC_K)) _force_to_apply += forward * _force;
		if (input->isKeyDown(KC_I)) _force_to_apply -= forward * _force;

		_mgr->getCamera("PlayerCam");
	}

	// Apply the forces before every time step
	void addForcesAndTorques()
	{
		// Apply the force we calculated in the key handler
		_first_body->addForce(_force_to_apply);
	}

	// Return our name for the test application to display
	virtual const String& getName()
	{
		static String name = "Test Chain";
		return name;
	}

	// Tell the user what keys they can use
	virtual const String& getKeys()
	{
		static String keys = "J/L, I/K, X - Throw the chain around";
		return keys;
	}

	// Override the collision callback to set our own parameters
	bool collision(OgreOde::Contact* contact)
	{
		// Set the floor to be a bit slippy
		contact->setCoulombFriction(10.0);
		return true;
	}

	// Use the destructor to delete the crate scene nodes
	// everything else gets deleted automatically
	~SimpleScenes_Chain()
	{
		for (int i = 0;i < 4;i++)
		{
			if (i != 2)
			{
				String name = String("Crate_") + StringConverter::toString(i);
				_mgr->destroySceneNode(name);
				_mgr->destroyEntity(name);
			}
		}
	}

protected:
	// Utility method to create a static box
	void createBox(int id,Real size,const Ogre::Vector3& position)
	{
		// Visual
		String name = String("Crate_") + StringConverter::toString(id);

		Entity* entity = _mgr->createEntity(name,"crate.mesh");
		entity->setNormaliseNormals(true);
		entity->setCastShadows(true);

		SceneNode* node = _mgr->getRootSceneNode()->createChildSceneNode(entity->getName());
		node->attachObject(entity);
		node->setScale(size * 0.1,size * 0.1,size * 0.1);
		node->setPosition(position);

		// Make one of them dynamic, the others are static
		OgreOde::EntityInformer ei(entity,Matrix4::getScale(node->getScale()));
		if ((position.x < 0)&&(position.z < 0))
		{
			_box_body = ei.createSingleDynamicBox(2.0, _world, _space); 
			_bodies.push_back(_box_body);
			_geoms.push_back(_box_body->getGeometry(0));
		}
		else
		{
			// Collision geometry
			_geoms.push_back(ei.createSingleStaticBox(_world, _space));
		}
	}  

protected:
	// Keep track of the top of the chain so we can throw it around
	OgreOde::Body *_first_body,*_box_body;
	Real _force;
	Vector3 _force_to_apply;
};

#endif
