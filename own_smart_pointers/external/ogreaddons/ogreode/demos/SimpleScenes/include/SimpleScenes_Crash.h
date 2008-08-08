/*
SimpleScenes_Crash.h
----------------------
A reimplementation of the ODE "crashing into a wall" demo 
using Ogre and the OgreOde wrapper.
*/
#ifndef _SIMPLESCENES_CRASH_H_
#define _SIMPLESCENES_CRASH_H_

/*
The crash test extends the base test class
*/
class SimpleScenes_Crash:public SimpleScenes
{
public:
	// Standard constructor/destructor
    SimpleScenes_Crash(OgreOde::World *world) :
      SimpleScenes(world)
      {
          _rocket_node_explosion = _mgr->getRootSceneNode ()->createChildSceneNode ("rocket_node_explosion_node");

          _rocketParticles_explosion = _mgr->createParticleSystem ("rocket_node_explosion", "myExplosionTemplate");
          _rocketParticles_smoke_white = _mgr->createParticleSystem ("rocketParticles_smoke_white", "myExplosionSmokewhiteTemplate");
          _rocketParticles_smoke = _mgr->createParticleSystem ("rocketParticles_smoke", "myExplosionSmokeTemplate");

          _rocket_node_explosion->attachObject(_rocketParticles_explosion);
          _rocket_node_explosion->attachObject(_rocketParticles_smoke_white);
          _rocket_node_explosion->attachObject(_rocketParticles_smoke);

          _rocketParticles_explosion->setSpeedFactor(0.0);
          _rocketParticles_smoke_white->setSpeedFactor(0.0);
          _rocketParticles_smoke->setSpeedFactor(0.0);



          _rocket_node_explosion->setVisible(false);

        // create helicopter
        {
		    // Main bit of the helicopter
		    _apache_body = _mgr->createEntity("Apache Body", "apache_body.mesh");

            _apache_body->setQueryFlags(VEHICLE_QUERY_MASK);

		    _apache_body->setCastShadows(true);

		    _apache_body_node = _mgr->getRootSceneNode()->createChildSceneNode("Apache Body");
		    _apache_body_node->attachObject(_apache_body);
		    _apache_body_node->setPosition(0,1.2,0);

		    // Main rotor
		    _main_rotor = _mgr->createEntity("Main Rotor", "main_rotor.mesh");
            _main_rotor->setCastShadows(true);

		    _main_rotor_node = _apache_body_node->createChildSceneNode("Main Rotor");
		    _main_rotor_node->attachObject(_main_rotor);
		    _main_rotor_node->setPosition(0,0.987322,0.573885);

		    // Tail rotor
		    _tail_rotor = _mgr->createEntity("Tail Rotor", "tail_rotor.mesh");
            _tail_rotor->setCastShadows(true);

		    _tail_rotor_node = _apache_body_node->createChildSceneNode("Tail Rotor");
		    _tail_rotor_node->attachObject(_tail_rotor);
		    _tail_rotor_node->setPosition(0.174927,0.173132,-3.50708);

		    // Set up the parameters for the main rotor
		    _main_min = 2.0;
		    _main_max = 90.0;
		    _main_spd = _main_min;
		    _main_response = 40.0;
		    _main_scale = 1.0;

		    // Set up the parameters for the tail rotor
		    _tail_min = -80.0;
		    _tail_max = 80.0;
		    _tail_response = 90.0;
		    _tail_scale = 0.5;
		    _tail_spd = 0.0;

		    // Other simulation parameters
		    _rotate_scale = 20.0;
		    _thrust = 50.0;
		    _linear_damp = 12.5;
		    _angular_damp = 25.0;
		    _thrust_offs = Ogre::Vector3(0,0.1,0);
		    _fire_rate = 0.5;
		    _fire_time = _fire_rate;
		    _fire_force = 250.0;
		    _fire_mass = 0.1;
		    _fire_size = 0.25;
		    _rockets = 0;


		    // Create a space for the helicopter to live in
		    _apache_space = new OgreOde::SimpleSpace(_world, _space);
		    _apache_space->setInternalCollisions(false);
		    _apache_space->setAutoCleanup(false);	

		    // Create the physical body of the helicopter
		    OgreOde::BoxMass apache_mass(1.5,Vector3(1.4,1.46,4.24));
		    _apache = new OgreOde::Body(_world);
		    _apache_body_node->attachObject(_apache);
		    _apache->setMass(apache_mass);

            _apache->setDamping(_linear_damp, _angular_damp);

		    _bodies.push_back(_apache);
		    _last_node = _apache_body_node;

		    // Create the geometry of the helicopter from several bits

		    // Main body
		    OgreOde::TransformGeometry* trans = new OgreOde::TransformGeometry(_world, _apache_space);
            trans->setBody(_apache);

            _apache_body->setUserObject(trans);

            OgreOde::BoxGeometry* geom = new OgreOde::BoxGeometry(Vector3(1.4,1.46,4.24), world, 0);		
		    trans->setEncapsulatedGeometry(geom);
		    geom->setPosition(Vector3(0.013,0.014,1.052));
		    _geoms.push_back(geom);
		    _geoms.push_back(trans);



		    // Left wing
		    trans = new OgreOde::TransformGeometry(_world, _apache_space);
		    geom = new OgreOde::BoxGeometry(Vector3(0.75,0.55,1.05), world, 0);		
		    trans->setBody(_apache);
		    trans->setEncapsulatedGeometry(geom);
		    geom->setPosition(Vector3(-1.089,-0.486,0.446));		
		    _geoms.push_back(geom);
		    _geoms.push_back(trans);

		    // Right wing
		    trans = new OgreOde::TransformGeometry(_world, _apache_space);
		    geom = new OgreOde::BoxGeometry(Vector3(0.75,0.55,1.05), world, 0);		
		    trans->setBody(_apache);
		    trans->setEncapsulatedGeometry(geom);
		    geom->setPosition(Vector3(1.089,-0.505,0.446));		
		    _geoms.push_back(geom);
		    _geoms.push_back(trans);

		    // Tail boom
		    trans = new OgreOde::TransformGeometry(_world, _apache_space);
		    geom = new OgreOde::BoxGeometry(Vector3(0.73,0.66,3.28), world, 0);		
		    trans->setBody(_apache);
		    trans->setEncapsulatedGeometry(geom);
		    geom->setPosition(Vector3(0,-0.533,-2.104));		
		    _geoms.push_back(geom);
		    _geoms.push_back(trans);

		    // Tail flipper (probably not the technically correct name!)
		    trans = new OgreOde::TransformGeometry(_world, _apache_space);
		    geom = new OgreOde::BoxGeometry(Vector3(1.61,0.30,0.45), world, 0);		
		    trans->setBody(_apache);
		    trans->setEncapsulatedGeometry(geom);
		    geom->setPosition(Vector3(-0.014,-0.622,-3.778));		
		    _geoms.push_back(geom);
		    _geoms.push_back(trans);

		    // Tail fin
		    trans = new OgreOde::TransformGeometry(_world, _apache_space);
		    geom = new OgreOde::BoxGeometry(Vector3(0.30,1.27,0.96), world, 0);		
		    trans->setBody(_apache);
		    trans->setEncapsulatedGeometry(geom);
		    geom->setPosition(Vector3(0,-0.042,-3.491));	
		    _geoms.push_back(geom);
		    _geoms.push_back(trans);

		    // Rear wheel - a separate body connected by a ball joint, doesn't have a SceneNode
		    OgreOde::SphereMass wheel_mass(0.02,0.06);
		    _rear_wheel = new OgreOde::Body(_world);
		    _rear_wheel->setMass(wheel_mass);
		    _rear_wheel->setPosition(Vector3(0.0,0.068,-3.937));
		    OgreOde::BallJoint* ball_joint = new OgreOde::BallJoint(_world);
		    ball_joint->attach(_apache,_rear_wheel);
		    ball_joint->setAnchor(_rear_wheel->getPosition());
		    OgreOde::SphereGeometry* sphere = new OgreOde::SphereGeometry(0.06, _world, _apache_space);
		    sphere->setBody(_rear_wheel);
		    _geoms.push_back(sphere);
		    _bodies.push_back(_rear_wheel);
		    _joints.push_back(ball_joint);

		    // Left front wheel - connected by a hinge joint so it can only roll forward
		    OgreOde::SphereMass front_wheel_mass(0.03,0.17);
		    _left_front = new OgreOde::Body(_world);
		    _left_front->setMass(front_wheel_mass);
		    _left_front->setPosition(Vector3(-0.699,0.199,1.163));
		    OgreOde::HingeJoint* hinge_joint = new OgreOde::HingeJoint(_world);
		    hinge_joint->attach(_apache,_left_front);
		    hinge_joint->setAnchor(_left_front->getPosition());
		    hinge_joint->setAxis(Vector3::UNIT_X);
		    sphere = new OgreOde::SphereGeometry(0.17, _world, _apache_space);
		    sphere->setBody(_left_front);
		    _geoms.push_back(sphere);
		    _bodies.push_back(_left_front);
		    _joints.push_back(hinge_joint);

		    // Right front wheel - connected by a hinge joint, doesn't have a SceneNode
		    _right_front = new OgreOde::Body(_world);
		    _right_front->setMass(front_wheel_mass);
		    _right_front->setPosition(Vector3(0.699,0.185,1.163));
		    hinge_joint = new OgreOde::HingeJoint(_world);
		    hinge_joint->attach(_apache,_right_front);
		    hinge_joint->setAnchor(_right_front->getPosition());
		    hinge_joint->setAxis(Vector3::UNIT_X);
		    sphere = new OgreOde::SphereGeometry(0.17, _world, _apache_space);
		    sphere->setBody(_right_front);

		    _geoms.push_back(sphere);
		    _bodies.push_back(_right_front);
		    _joints.push_back(hinge_joint);
        }


		// Create the wall of boxes to crash into, make it 
		// smaller in Debug mode for performance reasons
		int i = 0;
#ifndef _DEBUG
		for (Real y = 0.0;y <= 5.0;y += 1.0)
		{
			for (Real x = -6.0 + (y * 0.5);x <= 6.0 - (y * 0.5);x += 1.0)
			{
#else
		for (Real y = 0.0;y <= 1.0;y += 1.0)
		{
			for (Real x = -2.0 + (y * 0.5);x <= 2.0 - (y * 0.5);x += 1.0)
			{
#endif
				// Create the Ogre box
				String name = String("Box_") + StringConverter::toString(i);
                Entity* box = _mgr->createEntity(name,"crate.mesh");
                box->setQueryFlags (GEOMETRY_QUERY_MASK);
				box->setCastShadows(true);

				SceneNode* node = _mgr->getRootSceneNode()->createChildSceneNode(name);
				node->attachObject(box);
				node->setScale(0.1,0.1,0.1);

				// Set the position and rotate that, then rotate the box by the same amount
				Quaternion q;
				q.FromAngleAxis(Degree(45),Vector3::UNIT_Y);
				Vector3 pos(x,y + 0.5,-10.0);
				node->setPosition(q * pos);
				node->setOrientation(q);

				// Create a box for ODE and attach it to the Ogre version
				OgreOde::Body* body = new OgreOde::Body(_world, name + "Body");
				node->attachObject(body);
				body->setMass(OgreOde::BoxMass(0.01,Vector3(1,1,1)));
                body->setDamping (0.005, 0.005);

				OgreOde::BoxGeometry* geom = new OgreOde::BoxGeometry(Vector3(1.0,1.0,1.0),_world, _space);
                // Tie the collision geometry to the physical body
                geom->setBody(body);
                box->setUserObject(geom);

				// Keep track of the ODE objects for house keeping
				_bodies.push_back(body);
				_geoms.push_back(geom);

				i++;
			}
		}
	}
	virtual ~SimpleScenes_Crash()
	{
		// Destroy the non-standard scene nodes that make up the helicopter
		_apache_body_node->removeAndDestroyChild("Main Rotor");
		_apache_body_node->removeAndDestroyChild("Tail Rotor");
		
		// Remove the entities that represent the scene nodes we destroyed
		_mgr->destroyEntity("Main Rotor");
		_mgr->destroyEntity("Tail Rotor");
        
		// Delete the space in which the helicopter lived
		delete _apache_space;
	}

	// Return our name for the test application to display
	virtual const String& getName()
	{
		static String name = "Test Crash";
		return name;
	}

	// Return a description of the keys that the user can use in this test
	virtual const String& getKeys()
	{
		static String keys = "I/K - Throttle, J/L - Turn, G/B/V/N - Thrust, X - Fire";
		return keys;
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

		// Throttle up or down
		if (input->isKeyDown(KC_I)) _main_spd += (_main_response * time);
		else if (input->isKeyDown(KC_K)) _main_spd -= (_main_response * time);

		// Thrust left, right, forward, or back
		_thrust_force = Ogre::Vector3::ZERO;
		if (input->isKeyDown(KC_G)) _thrust_force.z += _thrust;
		if (input->isKeyDown(KC_B)) _thrust_force.z -= _thrust;
		if (input->isKeyDown(KC_V)) _thrust_force.x += _thrust;
		if (input->isKeyDown(KC_N)) _thrust_force.x -= _thrust;

		// Clamp the main rotor speed
		_main_spd = std::max(_main_min,std::min(_main_max,_main_spd));

		// Turn left or right or automatically stop the tail rotor
		if (input->isKeyDown(KC_J)) _tail_spd += (_tail_response * time);
		else if (input->isKeyDown(KC_L)) _tail_spd -= (_tail_response * time);
		else
		{
			if (_tail_spd < 0.0)
			{
				_tail_spd += (_tail_response * time);
				if (_tail_spd > 0.0) _tail_spd = 0.0;
			}
			else if (_tail_spd > 0.0)
			{
				_tail_spd -= (_tail_response * time);
				if (_tail_spd < 0.0) _tail_spd = 0.0;
			}
		}
		
		// Clamp the tail rotor speed
		_tail_spd = std::max(_tail_min,std::min(_tail_max,_tail_spd));

		// Rotate the tail rotor scene node
		_tail_rotor_node->rotate(Vector3::UNIT_X,Radian(_tail_spd * time * _rotate_scale));

		// If there's some power being applied to the main rotor then...
		if ((_main_spd > _main_min)||(!input->isKeyDown(KC_K)))
		{
			// Rotate the main rotor scene node
			_main_rotor_node->rotate(Vector3::UNIT_Y,Radian(_main_spd * time * _rotate_scale));
		}

		// Fire rockets. Woo!
		_fire_time += time;
		if ((_fire_time > _fire_rate)&&(input->isKeyDown(KC_X)))
		{
			_fire_time = 0.0;
			_rockets++;

			// Create a rocket node and attach a particle system to it
			const String name (String("Rocket_") + StringConverter::toString(_rockets));
			ParticleSystem * rocketParticles = _mgr->createParticleSystem (name, "OgreOdeDemos/Rocket");
			SceneNode * rocket_node = _mgr->getRootSceneNode ()->createChildSceneNode (name);
			rocket_node->attachObject(rocketParticles);
			
			// Alternate firing between the left and right pods and 
			// convert it from body coordinates to world
			// Do the same with the firing "force" vector
			const Vector3 pos (_apache->getPointWorldPosition (Vector3 (1.35 * ((_rockets & 1)?-1.0:1.0),-0.55,0.95)));
			const Vector3 force (_apache->getVectorToWorld (Vector3 (0,0,_fire_force)));

			rocket_node->setPosition (pos);

			// Create a sphere for the physical body
			OgreOde::Body* rocket_body = new OgreOde::Body (_world);
			OgreOde::SphereGeometry* rocket_geom = 
                new OgreOde::SphereGeometry (_fire_size, _world, _space);

			rocket_body->setMass (OgreOde::SphereMass (_fire_mass,_fire_size));
			rocket_body->setAffectedByGravity (false);
			rocket_geom->setBody (rocket_body);

			rocket_node->attachObject (rocket_body);
			
			_bodies.push_back (rocket_body);
			_geoms.push_back (rocket_geom);

			// Fire it off by applying an initial force
			rocket_body->addForce (force);

			// Initialise the rocket's life span to zero
			_rocket_list[rocket_geom] = 0.0f;
		}

		// Check all the rockets
		for (std::map<OgreOde::SphereGeometry*,Real>::iterator i = _rocket_list.begin();
            i != _rocket_list.end();)
		{
            // Increase the time it's lived
            float *rocketTime = &(i->second);
			*rocketTime += time;

			// If it had it's emitter disabled (see below) more than 2 seconds ago then kill it
			if ((*rocketTime < 0.0))
			{
				// Get the geometry's body and kill any particle system attached to the same node
				killParticleSystem (i->first);

                OgreOde::Body * const body = i->first->getBody ();
				// Manually remove the body from the list of managed bodies
				for (std::vector<OgreOde::Body*>::iterator bi = _bodies.begin();
                    bi != _bodies.end();)
				{
					if ((*bi) == body) 
                        bi = _bodies.erase(bi);
					else 
                        ++bi;
				}		
					
				// Manually delete the geometry from the list of managed geometries
				for (std::vector<OgreOde::Geometry*>::iterator gi = _geoms.begin();
                    gi != _geoms.end();)
				{
					if ((*gi) == i->first)
                        gi = _geoms.erase(gi);
					else 
                        ++gi;
				}			
					
				// Delete the actual body and geometry
				delete body;						
				delete i->first;

				// Erase the rocket from the hash map
				_rocket_list.erase(i++);
			}
			else 
			{
				// If the rocket has been alive for more than 2 seconds then kill its emitters (so it fades away)
				if (*rocketTime > 3.0)
                {
                    OgreOde::SphereGeometry* sphereGeom = i->first;
					killEmitters(sphereGeom, rocketTime);
				}
				++i;
			}
		}
	}
	
	// Override the base collision callback, 'cos we don't want the "if connected" check doing
	bool collision(OgreOde::Contact* contact)
    {
        OgreOde::Geometry* firstGeom = contact->getFirstGeometry();
        OgreOde::Geometry* secondGeom = contact->getSecondGeometry();

        OgreOde::SphereGeometry* rocketGeom = 0;
        OgreOde::Geometry* otherGeom = 0;
        float *time = 0;
        std::map<OgreOde::SphereGeometry*,Real>::iterator li;
        if (firstGeom->getClass () == OgreOde::Geometry::Class_Sphere || 
            secondGeom->getClass () == OgreOde::Geometry::Class_Sphere)
        {

            // If a rocket's hit something then stop emitting particles
            // check if it's a sphere (rocket proxy Geom...)
            if (firstGeom->getClass () == OgreOde::Geometry::Class_Sphere)
            {
                li = _rocket_list.find(static_cast<OgreOde::SphereGeometry*>(firstGeom));
                if (li != _rocket_list.end())
                {   
                    rocketGeom = li->first;
                    time = &(li->second);
                    otherGeom = secondGeom;
                }
            }

            if (secondGeom->getClass () == OgreOde::Geometry::Class_Sphere)
            {
                std::map<OgreOde::SphereGeometry*,Real>::iterator li = 
                    _rocket_list.find(static_cast<OgreOde::SphereGeometry*>(secondGeom));
                if (li != _rocket_list.end())
                {
                    rocketGeom = li->first;
                    time = &(li->second);
                    otherGeom = firstGeom;
                }
            }
            if (rocketGeom && *time > 0)
            {
                // prevent collision against Apache
                if (otherGeom->getSpace() == _apache_space)
                {
                    return  false;
                }

                // there, Add an Explosion Effect (could make two sphere, one that destroy, one for the wave effect only.)
                const Vector3 explosionCenter (contact->getPosition ());
                Ogre::SphereSceneQuery* explosionAreaQuery =  _mgr->createSphereQuery(Sphere(explosionCenter, 20), 0xFFFFFFFF);

                // asking for movable physic object (no particles system or static geometry (here the plane))
                explosionAreaQuery->setQueryMask (GEOMETRY_QUERY_MASK | VEHICLE_QUERY_MASK);
                explosionAreaQuery->setQueryTypeMask(SceneManager::ENTITY_TYPE_MASK);

                const SceneQueryResult& result = explosionAreaQuery->execute ();
                if (!result.movables.empty())
                {
                    SceneQueryResultMovableList::const_iterator i = result.movables.begin();
                    while(i != result.movables.end())
                    {
                        //GEOMETRY_QUERY_MASK
                        OgreOde::Geometry *affectedGeom = static_cast <OgreOde::Geometry *> ((*i)->getUserObject ());
                        assert (affectedGeom != rocketGeom);
                        assert (affectedGeom);
                        OgreOde::Body *affectedBody = affectedGeom->getBody ();                        
                        assert (affectedBody);
                        Vector3 forceAreaDirection = (*i)->getParentNode()->getPosition() - explosionCenter;
                        const Real forcePower = forceAreaDirection.squaredLength () / 5;
                        forceAreaDirection.normalise ();

                        // Apply the main rotor force
                        affectedBody->addForce(forceAreaDirection * forcePower);
                        
                        ++i;
                    }
                }

                // remove particle and rocket geom
                killEmitters(rocketGeom, time);

                // add explosion particles
                _rocket_node_explosion->setPosition(explosionCenter);
                _rocket_node_explosion->setVisible(true);

                resetParticleSystem(_rocketParticles_explosion, true, 0.1);
                resetParticleSystem(_rocketParticles_smoke_white, true, 0.3);
                resetParticleSystem(_rocketParticles_smoke, true, 0.6);
            }
            
        }
		// Set the friction at the contact
		contact->setCoulombFriction(5.0);

		// Yes, this collision is valid
		return true;
	}

	// Will get called just before each time step, since a timestep zeros the force accumulators
	void addForcesAndTorques()
	{
		// Apply the main rotor force
		_apache->addForce(Vector3(0,_main_spd * _main_scale,0));

		// Apply the tail rotor torque
		_apache->addRelativeTorque(Vector3(0,_tail_spd * _tail_scale,0));

		// Apply the thrust force
		_apache->addRelativeForceAtRelative(_thrust_force,_thrust_offs);

       
	}

protected:

	// Kill the emitters for any particle systems attached to the same node
	// as the body with which the specified geometry is associated (if that makes sense!)
	void killEmitters(OgreOde::SphereGeometry* geom, Real* time = 0)
	{
		// Find the body
		OgreOde::Body* body = geom->getBody();
		if (body)
		{
			// Find the associated scene node
			SceneNode* node = static_cast<SceneNode*>(body->getParentNode());
			if (node)
			{
				// Kill the emitters of any attached particle systems
				for (int i = 0;i < node->numAttachedObjects();i++)
				{
					MovableObject* obj = node->getAttachedObject(i);
					if (obj->getMovableType() == "ParticleSystem")
					{ 
						static_cast<ParticleSystem*>(obj)->removeAllEmitters();
					}
				}
			}

			// Set the life span value so that we can easily tell 
			// that its had its emitters deleted
			if (time) 
                *time = -100.0;
		}
	}


    void resetParticleSystem(Ogre::ParticleSystem *ps, bool enable, const Ogre::Real delay)
    {
       const unsigned short numEmitters = ps->getNumEmitters();
       for (unsigned short i = 0; i < numEmitters; i++)
       {
           Ogre::ParticleEmitter *pe = ps->getEmitter(i);
           pe->setEnabled(enable);
           pe->setStartTime (delay);
       }

       ps->setSpeedFactor(enable ? 1.0 : 0.0);
    }
	// Kill the particle system and scene node associated with this geometry
	void killParticleSystem(OgreOde::SphereGeometry* geom)
	{
		// Find the body
		OgreOde::Body* body = geom->getBody();
		if (body)
		{
			// Find the scene node
			SceneNode* node = static_cast<SceneNode*>(body->getParentNode());
			if (node)
			{							
				MovableObject* obj = 0;				
				for (int i = 0;i < node->numAttachedObjects();i++)
				{
					// If this object isn't an OgreOde::Body then it must be
					// (in our demo) a particle system that we need to delete
					MovableObject* can_obj = node->getAttachedObject(i);
					if (can_obj->getMovableType() != "OgreOde::Body") 
                        obj = can_obj;
				}

				// Delete the node
				_mgr->getRootSceneNode()->removeAndDestroyChild(node->getName());

				// If we found a particle system then delete it
				if (obj) 
                    _mgr->destroyParticleSystem(obj->getName());
			}
		}
	}

protected:
	// Things we'll control and delete manually
	Entity *_apache_body,*_main_rotor,*_tail_rotor;
	SceneNode *_apache_body_node,*_main_rotor_node,*_tail_rotor_node;

	Real _rotate_scale;
	
	// Physical things we'll need to apply forces to and delete manually
	OgreOde::Body *_apache,*_rear_wheel,*_left_front,*_right_front;
	OgreOde::SimpleSpace* _apache_space;


	// The worst helicopter flight model in the world!
	Real _main_spd,_main_response,_main_scale,_main_min,_main_max;
	Real _tail_spd,_tail_response,_tail_scale,_tail_min,_tail_max;
	Real _thrust,_linear_damp,_angular_damp;
	Vector3 _thrust_offs,_thrust_force;

	// Stuff to help us shoot things
	Real _fire_rate,_fire_time,_fire_force,_fire_mass,_fire_size;
	std::map<OgreOde::SphereGeometry*,Real> _rocket_list;
	int _rockets;  


    SceneNode * _rocket_node_explosion;
    ParticleSystem * _rocketParticles_explosion;
    ParticleSystem * _rocketParticles_smoke_white;
    ParticleSystem * _rocketParticles_smoke;
};

#endif
