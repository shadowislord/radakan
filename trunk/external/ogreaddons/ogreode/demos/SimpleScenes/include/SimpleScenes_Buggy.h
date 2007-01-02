/*
SimpleScenes_Buggy.h
-------------------
A reimplementation of the ODE test Buggy application using
the OgreOde wrapper for Ogre
*/
#ifndef _SIMPLESCENES_BUGGY_H_
#define _SIMPLESCENES_BUGGY_H_

#include "OgreOde_Prefab.h"

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

/*
Buggy test extends from the base test class
*/
class SimpleScenes_Buggy:public SimpleScenes
{
public:
	//------------------------------------------------------------------------------------------------
	// Constructor also needs to create the scene
    SimpleScenes_Buggy(OgreOde::World *world) : 
      SimpleScenes(world),
	  vehicle(0)
	{
		// Create the vehicle from the config file
		changeCar();
	
		// Move the vehicle
		vehicle->setPosition(Vector3(0,0.82898,0));

		// Initially (i.e. in the config file) it's rear wheel drive
		_drive = 'R';
		setInfoText(carNames[sSelectedCar] + "Rear wheel drive");

		// Create a box to jump over, the visual version
		Entity* entity = _mgr->createEntity("Jump", "crate.mesh");
		entity->setNormaliseNormals(true);
		entity->setCastShadows(true);

		SceneNode* node = _mgr->getRootSceneNode()->createChildSceneNode(entity->getName());
		node->attachObject(entity);
		node->setPosition(Vector3(0,0.3,-5));
		node->setOrientation(Quaternion(Radian(0.4),Vector3(1,0,0)));
		node->setScale(0.3,0.1,0.4);

		// Create the physical version (just static geometry, it can't move so
		// it doesn't need a body) and keep track of it
		OgreOde::EntityInformer ei(entity,Matrix4::getScale(node->getScale()));
		OgreOde::Geometry *geom = ei.createSingleStaticBox(_world, _space);
        entity->setUserObject(geom);
        _geoms.push_back(geom);

		// The car is what we'll want to look at
		_last_node = vehicle->getSceneNode();
	}

	//------------------------------------------------------------------------------------------------
	// Override the destructor since there's some special deleting to do
	virtual ~SimpleScenes_Buggy()
	{
		delete vehicle;

		// Destroy the jump manually since it's not associated with 
		// any body it won't get deleted automatically
		_mgr->destroySceneNode("Jump");
		_mgr->destroyEntity("Jump");

		// Geometries and Joints will get deleted by the base class
	}

	//------------------------------------------------------------------------------------------------
	// Return our name
	virtual const String& getName()
	{
		static String name = "Test Buggy";
		return name;
	}

	//------------------------------------------------------------------------------------------------
	// Tell the user what keys they can use
	virtual const String& getKeys()
	{
		static String keys = "I/K - Accelerate/Brake, J/L - Turn, X - Change drive mode, N - Change Car";
		return keys;
	}


	//------------------------------------------------------------------------------------------------
	void changeCar()
	{
		sSelectedCar = (sSelectedCar + 1) % maxNumCar;

		delete vehicle;
		vehicle = static_cast <OgreOde_Prefab::Vehicle *> (dotOgreOdeLoader->loadObject (carFileNames[sSelectedCar], carNames[sSelectedCar]));


		// Move the vehicle
		vehicle->setPosition(Vector3(0,0.82898,0));
	}
	//------------------------------------------------------------------------------------------------
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
		// Tell the vehicle what digital inputs are being pressed; left, right, power and brake
		// There are equivalent methods for analogue controls, current you can't change gear so
		// you can't reverse!
		vehicle->setInputs(input->isKeyDown(KC_J),input->isKeyDown(KC_L),input->isKeyDown(KC_I),input->isKeyDown(KC_K));

		// Update the vehicle, you need to do this every time step
		vehicle->update(time);
		if (_key_delay > SimpleScenes::KEY_DELAY)
		{

			if ((input->isKeyDown(KC_N)))
			{
				changeCar();
				_key_delay = 0.0;
			}
			// Change the drive mode between front, rear and 4wd
			if ((input->isKeyDown(KC_X)))
			{
				switch(_drive)
				{
					// Switch from rear to front
				case 'R':
					_drive = 'F';

					vehicle->getWheel(0)->setPowerFactor(1);
					vehicle->getWheel(1)->setPowerFactor(1);
					vehicle->getWheel(2)->setPowerFactor(0);
					vehicle->getWheel(3)->setPowerFactor(0);

					setInfoText(carNames[sSelectedCar] + "Front wheel drive");
					break;

					// Switch from front to all
				case 'F':
					_drive = '4';

					vehicle->getWheel(0)->setPowerFactor(0.6);
					vehicle->getWheel(1)->setPowerFactor(0.6);
					vehicle->getWheel(2)->setPowerFactor(0.4);
					vehicle->getWheel(3)->setPowerFactor(0.4);

					setInfoText(carNames[sSelectedCar] + "All wheel drive");
					break;

					// Switch from all to rear
				case '4':
					_drive = 'R';

					vehicle->getWheel(0)->setPowerFactor(0);
					vehicle->getWheel(1)->setPowerFactor(0);
					vehicle->getWheel(2)->setPowerFactor(1);
					vehicle->getWheel(3)->setPowerFactor(1);

					setInfoText(carNames[sSelectedCar] + "Rear wheel drive");
					break;
				}
				_key_delay = 0.0;
			}
		}
	}

	//------------------------------------------------------------------------------------------------
	// Override the collision callback to set our own parameters
	bool collision(OgreOde::Contact* contact)
	{
		// If the base class doesn't think the collision needs to 
		// happen then we won't do anything either
		if (!SimpleScenes::collision(contact)) 
			return false;

		if (!OgreOde_Prefab::Vehicle::handleTyreCollision(contact))
		{
			// Set the floor to be a bit slippy
			contact->setCoulombFriction(12.0);
		}
		return true;
	}

protected:
	// Keep track of the things we need to delete manually or change according to user input
	OgreOde_Prefab::Vehicle *vehicle;
	char _drive;
};

#endif
