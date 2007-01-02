/*
SimpleScenes.cpp
---------------
Code for the base scene class from which the specific scenes are derived
*/
#include "SimpleScenes.h"

using namespace Ogre;
using namespace OgreOde;
using namespace OgreOde_Prefab;
using namespace OgreOde_Loader;

const Ogre::Real SimpleScenes::KEY_DELAY = 1.0;
const Ogre::Real SimpleScenes::STEP_RATE = 0.01;

/*
Constructor; initailise stuff
*/
SimpleScenes::SimpleScenes(OgreOde::World *world)
{
	_world = world;
	_mgr = _world->getSceneManager();
	_world->setCollisionListener(this);
	_space = _world->getDefaultSpace();
	_key_delay = SimpleScenes::KEY_DELAY;
    _last_node = 0;

    dotOgreOdeLoader = new  OgreOde_Loader::DotLoader (world);

    _ragdollFactory = new OgreOde_Prefab::RagdollFactory();
    Ogre::Root::getSingletonPtr()->addMovableObjectFactory(_ragdollFactory); 
    setInfoText(StringUtil::BLANK);
}

/*
Called by OgreOde whenever a collision occurs, so 
that we can modify the contact parameters
*/
bool SimpleScenes::collision(OgreOde::Contact* contact) 
{
	// Check for collisions between things that are connected and ignore them
	OgreOde::Geometry * const g1 = contact->getFirstGeometry();
	OgreOde::Geometry * const g2 = contact->getSecondGeometry();

	if (g1 && g2)
	{
		const OgreOde::Body * const b1 = g1->getBody();
		const OgreOde::Body * const  b2 = g2->getBody();
		if (b1 && b2 && OgreOde::Joint::areConnected(b1, b2)) 
           return false; 
	}

	// Set the friction at the contact
	contact->setCoulombFriction(OgreOde::Utility::Infinity);

    contact->setBouncyness(0.1);

    //contact->setCoulombFriction(5.0);
    //contact->setFrictionMode(OgreOde::Contact::Flag_SurfaceIsBouncy);

    //contact->setCoulombFriction(OgreOde::Utility::Infinity);

	// Yes, this collision is valid
	return true;
}

/*
Handle key presses
*/
#if (OGRE_VERSION <  ((1 << 16) | (3 << 8) | 0))
void SimpleScenes::frameEnded(Real time, InputReader* input)
#else
void SimpleScenes::frameEnded(Real time, OIS::Keyboard* input, OIS::Mouse* mouse)
#endif //OGRE_VERSION not heihort
{
	_key_delay += time;

    std::for_each (RagdollList.begin(), RagdollList.end(), 
        std::mem_fun (&OgreOde_Prefab::Ragdoll::update)); 
}

/*
Utility method to set the information string in the UI
*/
void SimpleScenes::setInfoText(const String& text)
{
	OverlayManager::getSingleton().getOverlayElement("OgreOdeDemos/Info")->setCaption(String("Info: ") + text);
}

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
    Ogre::Vector3(1.0, 1.0, 1.0),
    Ogre::Vector3(1.0, 1.0, 1.0)
};
static const String ragdollFile [] = {
    "zombie.ogreode",
    "zombie_auto.ogreode",
    "ninja.ogreode",
    "robot.ogreode"
};
static int sSelectedMesh = 0;
static int _ragdoll_count = 0;

/*
Create a ragdoll
*/
void SimpleScenes::createRagDoll()
{
    if (_key_delay < SimpleScenes::KEY_DELAY) 
        return;
 
    _key_delay = 0.0;


    OgreOde_Prefab::Ragdoll *_ragdoll = static_cast <OgreOde_Prefab::Ragdoll *> (dotOgreOdeLoader->loadObject (ragdollFile[sSelectedMesh], 
                                                                           xmlNames[sSelectedMesh], 
                                                                            "zombie" + StringConverter::toString(_ragdoll_count++)));


    _ragdoll->setCastShadows(true);

    SceneNode *_ragdoll_node = _mgr->getRootSceneNode()->createChildSceneNode(_ragdoll->getName() + "Node");
    _ragdoll_node->attachObject(_ragdoll);

    //_ragdoll_node->yaw(Degree(rand() % 360));
    //_ragdoll_node->pitch(Degree(rand() % 360));


    _ragdoll_node->setOrientation(Quaternion(Radian(OgreOde::Utility::randomReal() * 10.0 - 5.0),
                                    Ogre::Vector3(OgreOde::Utility::randomReal() * 2.0 - 1.0,
                                    OgreOde::Utility::randomReal() * 2.0 - 1.0,
                                    OgreOde::Utility::randomReal() * 2.0 - 1.0)));

    _ragdoll_node->setPosition(Vector3((OgreOde::Utility::randomReal() * 10.0) - 5.0,
                                OgreOde::Utility::randomReal() + 5,
                                (OgreOde::Utility::randomReal() * 10.0) - 5.0));


    _ragdoll_node->setScale( meshScale[sSelectedMesh]);


    _ragdoll->getAnimationState(meshAnimation[sSelectedMesh])->setEnabled(false);


    // Create the ragdoll
    _ragdoll->takePhysicalControl(_world, _space, false);
    _ragdoll->setSelfCollisions(false);

    RagdollList.push_back(_ragdoll);

}
/*
Create a randomly sized box, sphere or capsule for dropping on things
*/ 
OgreOde::Body* SimpleScenes::createRandomObject(OgreOde::Geometry::Class objectClass)
{
	if (_key_delay < SimpleScenes::KEY_DELAY) 
        return 0;

	String typeName = (objectClass == OgreOde::Geometry::Class_Box)?"crate":
				      (objectClass == OgreOde::Geometry::Class_Sphere)?"ball":
					  (objectClass == OgreOde::Geometry::Class_Capsule)?"capsule":
                      (objectClass == OgreOde::Geometry::Class_TriangleMesh)?"gun":
                      "unknown";

	// Create the visual representation (the Ogre entity and scene node)
	String name = typeName + StringConverter::toString((unsigned int)_bodies.size());
	Entity* entity = _mgr->createEntity(name, typeName + ".mesh");
    entity->setQueryFlags (GEOMETRY_QUERY_MASK);

	SceneNode* node = _mgr->getRootSceneNode()->createChildSceneNode(name);
	node->attachObject(entity);
	entity->setNormaliseNormals(true);
	entity->setCastShadows(true);

	// Pick a size
	Vector3 size((OgreOde::Utility::randomReal() * 0.5 + 0.1) * 2.0,
                (OgreOde::Utility::randomReal() * 0.5 + 0.1) * 2.0,
                (OgreOde::Utility::randomReal() * 0.5 + 0.1) * 2.0);

	// Create a body associated with the node we created
    OgreOde::Body* body = new OgreOde::Body(_world);
	node->attachObject(body);

	// Set the mass and geometry to match the visual representation
	OgreOde::Geometry* geom = 0;
	switch (objectClass)
	{
		case OgreOde::Geometry::Class_Box:
		{
			size *= 1.75;

			OgreOde::BoxMass mass(1.0,size);
			mass.setDensity(5.0,size);
			
            geom = (OgreOde::Geometry*)new OgreOde::BoxGeometry(size, _world, _space); 
			node->setScale(size.x * 0.1,size.y * 0.1,size.z * 0.1);
			body->setMass(mass);
		}
		break;

		case OgreOde::Geometry::Class_Sphere:
		{
			OgreOde::SphereMass mass(1.0,size.x);
			mass.setDensity(5.0,size.x);

            geom = (OgreOde::Geometry*)new OgreOde::SphereGeometry(size.x, _world, _space);
			node->setScale(size.x * 0.2,size.x * 0.2,size.x * 0.2);
			body->setMass(mass);
		}
		break;

		case OgreOde::Geometry::Class_Capsule:
		{
			size.x *= 0.5;

			OgreOde::CapsuleMass mass(1.0,size.x,Vector3::UNIT_Z,size.y);
			mass.setDensity(5.0,size.x,Vector3::UNIT_Z,size.y);
			
			geom = (OgreOde::Geometry*)new OgreOde::CapsuleGeometry(size.x,size.y, _world, _space);
			node->setScale(size.x,size.x,(size.y + (size.x * 2.0)) * 0.25);
			body->setMass(mass);
		}
        break;

        case OgreOde::Geometry::Class_TriangleMesh:
        {
                OgreOde::EntityInformer ei(entity, node->_getFullTransform());
                geom = ei.createStaticTriangleMesh( _world, _space);

            OgreOde::BoxMass mass(1.0, ei.getSize());
            mass.setDensity(5.0, ei.getSize());

            //node->setScale(size.x,size.x,(size.y + (size.x * 2.0)) * 0.25);
            body->setMass(mass);
        }
        break;
	}

	// Tie the collision geometry to the physical body
    geom->setBody(body);
    entity->setUserObject(geom);

	// Keep track of the body
	_bodies.push_back(body);
	_geoms.push_back(geom);
	
	_key_delay = 0.0;

	// If we created something position and orient it randomly
	if (body)
	{
		body->setOrientation(Quaternion(Radian(OgreOde::Utility::randomReal() * 10.0 - 5.0),
                                Ogre::Vector3(OgreOde::Utility::randomReal() * 2.0 - 1.0,
                                        OgreOde::Utility::randomReal() * 2.0 - 1.0,
                                        OgreOde::Utility::randomReal() * 2.0 - 1.0)));
		body->setPosition(Vector3((OgreOde::Utility::randomReal() * 10.0) - 5.0,
                                    OgreOde::Utility::randomReal() + 5,
                                    (OgreOde::Utility::randomReal() * 10.0) - 5.0));

		// Set the last body we created to be looked at
		_last_node = static_cast<SceneNode*>(body->getParentNode());
	}

	return body;
}

/*
Destructor, delete everything we're keeping track of
*/
SimpleScenes::~SimpleScenes(void)
{
	// Stop listening for collisions
	if (_world->getCollisionListener() == this) 
        _world->setCollisionListener(0);

	// Delete all the joints
	for (std::vector<OgreOde::Joint*>::iterator i = _joints.begin();i != _joints.end();++i)
	{
		delete (*i);
	}

	// Run through the list of bodies we're monitoring
	for (std::vector<OgreOde::Body*>::iterator i = _bodies.begin();i != _bodies.end();++i)
	{
		// Get the node this body controls
		SceneNode* node = static_cast<SceneNode*>((*i)->getParentNode());
		if (node)
		{
			// Get its name and remember all the things attached to it
			String name = node->getName();
			int num = node->numAttachedObjects();
			for (int cur = 0;cur < num; cur++)
			{
				MovableObject* obj = node->getAttachedObject(cur);
				if (obj->getMovableType() != OgreOde::Body::MovableType) 
                    clearList.push_back(obj);
			}

			// Destroy the node by name
			_mgr->getRootSceneNode()->removeAndDestroyChild(name);
		}

		// Delete the body
		delete (*i);
	}

	// Remove all the entities we found attached to scene nodes we're controlling
	for (std::vector<MovableObject*>::iterator i = clearList.begin();i != clearList.end();++i)
	{
        if ((*i)->getMovableType() == "Entity") 
        {
            MovableObject* m = static_cast<MovableObject*>(*i);
            //_mgr->destroyMovableObject(m->getName(), m->);
            _mgr->destroyMovableObject(m);
        }
		else  if ((*i)->getMovableType() == "ParticleSystem") 
        {
            _mgr->destroyParticleSystem(static_cast<ParticleSystem*>(*i));
        }
	}

	// Delete all the collision geometries
	for (std::vector<OgreOde::Geometry*>::iterator i = _geoms.begin();i != _geoms.end();++i)
	{
		delete (*i);
	}
    // Remove all the entities we found attached to scene nodes we're controlling
    for (std::vector<OgreOde_Prefab::Ragdoll *>::iterator i = RagdollList.begin();i != RagdollList.end();++i)
    {

        assert ((*i)->getParentNode ());
        assert ((*i)->getParentNode ()->getParent());
        (static_cast<SceneNode*> ((*i)->getParentNode ()->getParent()))->removeAndDestroyChild(
            (*i)->getParentNode ()->getName ());

        _mgr->destroyMovableObject((*i)->getName(), OgreOde_Prefab::RagdollFactory::FACTORY_TYPE_NAME);
        //_mgr->destroyMovableObject(m);
    }
	Ogre::Root::getSingletonPtr()->removeMovableObjectFactory(_ragdollFactory);
	delete _ragdollFactory; 
}
