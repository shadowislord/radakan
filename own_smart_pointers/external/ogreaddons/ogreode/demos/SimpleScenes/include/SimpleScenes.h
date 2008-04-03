/*
SimpleScenes.h
-------------
A basic framework class off which the specific scenes will be built.
Does some housekeeping of objects and so on, eventually functionality
similar to this will be built into OgreOde.
*/
#ifndef _SIMPLESCENES_H_
#define _SIMPLESCENES_H_

// We'll need the OgreOde definitions
#include "OgreOde_Core.h"
#include "OgreOde_Prefab.h"
#include "OgreOde_Loader.h"

#if !(OGRE_VERSION <  ((1 << 16) | (3 << 8) | 0))
#include "OIS/ois.h"
namespace OIS
{
	class Keyboard;
	class Mouse;
};
#endif //OGRE_VERSION not heihort

enum QueryFlags
{
	ANY_QUERY_MASK					= 1<<0,
	ZOMBIE_QUERY_MASK				= 1<<1,
	GEOMETRY_QUERY_MASK				= 1<<2,
	VEHICLE_QUERY_MASK				= 1<<3,
	STATIC_GEOMETRY_QUERY_MASK		= 1<<4
};
/*
The base Test class, is also able to listen for collisions and thus change the contact properties
*/
class SimpleScenes:public OgreOde::CollisionListener,public OgreOde::StepListener
{
public:
	static const Ogre::Real KEY_DELAY;
	static const Ogre::Real STEP_RATE;

	// Constructor/destructor
	SimpleScenes(OgreOde::World *world);
	virtual ~SimpleScenes();

	// Useful methods that will be handy for all tests
    void createRagDoll();
    OgreOde::Body* createRandomObject(OgreOde::Geometry::Class objectClass);
	void updateScene();
    const Ogre::SceneNode* getLastNode(){return _last_node;}
    void setInfoText(const Ogre::String& text);

	// The things we'll have to override in derived classes
	// Actual tests must provide a name but everything else can default
	virtual bool collision(OgreOde::Contact* contact);
    virtual const Ogre::String& getKeys(){return Ogre::StringUtil::BLANK;}
	virtual const Ogre::String& getName() = 0;
	virtual void addForcesAndTorques(){}

#if (OGRE_VERSION <  ((1 << 16) | (3 << 8) | 0))
    virtual void frameStarted(Ogre::Real time, Ogre::InputReader* input){};
    virtual void frameEnded(Ogre::Real time, Ogre::InputReader* input);

#else
    virtual void frameStarted(Ogre::Real time, OIS::Keyboard* input, OIS::Mouse* mouse){};
    virtual void frameEnded(Ogre::Real time, OIS::Keyboard* input, OIS::Mouse* mouse);

#endif //OGRE_VERSION not heihort


	// If we register this with a stepper it'll get told every time the world's about to be stepped
	bool preStep(Ogre::Real time)
	{
		addForcesAndTorques();
		return true;
	}

protected:
	// Keep track of what created us
	Ogre::SceneManager*	_mgr;
	OgreOde::World*		_world;
	OgreOde::Space*		_space;
	OgreOde_Prefab::RagdollFactory*   _ragdollFactory; 

	// Whatever the derived class sets this to will be what's looked at by the camera
	Ogre::SceneNode*			_last_node;

	// Keep track of the things we create so we can 
	// delete them automatically when we switch scenes
	std::vector<OgreOde::Body*>		_bodies;
	std::vector<OgreOde::Geometry*> _geoms;
    std::vector<OgreOde::Joint*>	_joints;

    std::vector<Ogre::MovableObject*> clearList;
    std::vector<OgreOde_Prefab::Ragdoll *> RagdollList;

    Ogre::Real _key_delay;
    OgreOde_Loader::DotLoader *dotOgreOdeLoader;
};

#endif
