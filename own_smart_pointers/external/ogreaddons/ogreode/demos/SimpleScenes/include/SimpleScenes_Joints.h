/*
SimpleScenes_Joints.h
---------------------
A demo using Ogre and the OgreOde wrapper to demonstrate joints.
*/
#ifndef _SIMPLESCENES_JOINTS_H_
#define _SIMPLESCENES_JOINTS_H_

/*
The joints test extends the base test class
*/
class SimpleScenes_Joints:public SimpleScenes
{
public:

	// Create the scene, just a load of differently hinged doors
    SimpleScenes_Joints(OgreOde::World *world) :
      SimpleScenes(world)
	{
		left = right = up = false;

		// Create the player's ball
		Entity* entity = _mgr->createEntity("Ball","ball.mesh");
		entity->setNormaliseNormals(true);
		entity->setCastShadows(true);

		SceneNode* node = _mgr->getRootSceneNode()->createChildSceneNode(entity->getName());

		node->attachObject(entity);
		node->setScale(0.2,0.2,0.2);
		node->setPosition(0,1.2,0);

		OgreOde::EntityInformer ei(entity,Matrix4::getScale(node->getScale()));
		ball_body = ei.createSingleDynamicSphere(5.0,_world, _space);
		_last_node = static_cast<SceneNode*>(ball_body->getParentNode());	
		_bodies.push_back(ball_body);
		_geoms.push_back(ball_body->getGeometry(0));

		// Make the ball stay in the X/Y plane
#ifdef OGREODE_PLANARJOINT
		OgreOde::PlanarJoint* joint2d = new OgreOde::PlanarJoint();
		joint2d->attach(ball_body);
		_joints.push_back(joint2d);
#endif

		// Create the free standing door
		entity = _mgr->createEntity("Free_Door","door.mesh");
		entity->setNormaliseNormals(true);
		entity->setCastShadows(true);

		node = _mgr->getRootSceneNode()->createChildSceneNode(entity->getName());

		node->attachObject(entity);
		node->setPosition(15,4,0);
		node->setOrientation(Quaternion(Degree(90),Vector3(0,1,0)));
		node->setScale(2,2,2);

		OgreOde::EntityInformer ei1(entity,Matrix4::getScale(node->getScale()));
		OgreOde::Body* body = ei1.createSingleDynamicBox(20.0, _world, _space);
		_bodies.push_back(body);
		_geoms.push_back(body->getGeometry(0));

		// Create a door hinged at the left hand side
		entity = _mgr->createEntity("Left_Door","door.mesh");
		entity->setNormaliseNormals(true);
		entity->setCastShadows(true);

		node = _mgr->getRootSceneNode()->createChildSceneNode(entity->getName());

		node->attachObject(entity);
		node->setPosition(-10,4.01,1.5);
		node->setOrientation(Quaternion(Degree(-90),Vector3(0,1,0)));
		node->setScale(2,2,2);

		OgreOde::EntityInformer ei2(entity,Matrix4::getScale(node->getScale()));
		body = ei2.createSingleDynamicBox(20.0,_world, _space);
		body->setDamping(0,20);

		OgreOde::HingeJoint* joint = new OgreOde::HingeJoint(_world);
		joint->attach(body);
		joint->setAxis(Vector3::UNIT_Y);
		joint->setAnchor(Vector3(-10,4.01,3.5));

		_bodies.push_back(body);
		_geoms.push_back(body->getGeometry(0));
		_joints.push_back(joint);

		// Create a door hinged at the right hand side
		entity = _mgr->createEntity("Right_Door","door.mesh");
		entity->setNormaliseNormals(true);
		entity->setCastShadows(true);

		node = _mgr->getRootSceneNode()->createChildSceneNode(entity->getName());

		node->attachObject(entity);
		node->setPosition(-15,4.01,-1.5);
		node->setOrientation(Quaternion(Degree(90),Vector3(0,1,0)));
		node->setScale(2,2,2);

		OgreOde::EntityInformer ei3(entity,Matrix4::getScale(node->getScale()));
		body = ei3.createSingleDynamicBox(20.0,_world, _space);
		body->setDamping(0,20);

		joint = new OgreOde::HingeJoint(_world);
		joint->attach(body);
		joint->setAxis(Vector3::UNIT_Y);
		joint->setAnchor(Vector3(-15,4.01,-3.5));

		_bodies.push_back(body);
		_geoms.push_back(body->getGeometry(0));
		_joints.push_back(joint);

		// Create a door hinged at the top
		entity = _mgr->createEntity("Top_Door","door.mesh");
		entity->setNormaliseNormals(true);
		entity->setCastShadows(true);

		node = _mgr->getRootSceneNode()->createChildSceneNode(entity->getName());

		node->attachObject(entity);
		node->setPosition(10,4.1,0);
		node->setOrientation(Quaternion(Degree(90),Vector3(0,1,0)));
		node->setScale(2,2,2);

		OgreOde::EntityInformer ei4(entity,Matrix4::getScale(node->getScale()));
		body = ei4.createSingleDynamicBox(20.0,_world, _space);
		body->setDamping(0,20);

		joint = new OgreOde::HingeJoint(_world);
		joint->attach(body);
		joint->setAxis(Vector3::UNIT_Z);
		joint->setAnchor(Vector3(10,8.1,0));

		_bodies.push_back(body);
		_geoms.push_back(body->getGeometry(0));
		_joints.push_back(joint);
	}
	
	// Standard destructor
	virtual ~SimpleScenes_Joints(){}

	// Return our name for the test application to display
	virtual const String& getName()
	{
		static String name = "Test Joints";
		return name;
	}

	// Return a description of the keys that the user can use in this test
	virtual const String& getKeys()
	{
		static String keys = "I - Bounce, J - Left, L - Right";
		return keys;
	}

	// Gets called every time the world is stepped, so we can apply forces every time step
	virtual void addForcesAndTorques()
	{
		// Apply a torque about the Z axis to roll the ball left 
		if(left)
		{
			ball_body->wake();
			ball_body->addTorque(Vector3(0,0,50));
		}

		// Apply a torque about the Z axis to roll the ball right
		if(right)
		{
			ball_body->wake();
			ball_body->addTorque(Vector3(0,0,-50));
		}

		// Apply an upwards force, or at least opposite to gravity
		if(up)
		{
			ball_body->wake();
			ball_body->addForce(_world->getGravity() * -10.0);
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

		// Set the key flags so we can apply forces at the right time
		left = input->isKeyDown(KC_J);
		right = input->isKeyDown(KC_L);
		up = input->isKeyDown(KC_I);
	}
	
	// Set up the collision properties
	virtual bool collision(OgreOde::Contact* contact)
	{
		contact->setBouncyness(0.8);
		contact->setCoulombFriction(OgreOde::Utility::Infinity);
		contact->setFrictionMode(OgreOde::Contact::Flag_FrictionPyramid);

		return true;
	}

protected:
	OgreOde::Body *ball_body;
	bool left,right,up;
};

#endif
