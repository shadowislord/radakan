
#include "OgreOdePrecompiledHeaders.h"

#include "OgreOdeWorld.h"
#include "OgreOdeBody.h"
#include "OgreOdeJoint.h"
#include "OgreOdeGeometry.h"
#include "OgreOdeSpace.h"
#include "OgreOdeDebugObject.h"

using namespace OgreOde;
using namespace Ogre;

CollisionListener* World::_collision_listener = 0;
//------------------------------------------------------------------------------------------------
void World::setCollisionListener(CollisionListener* collision_listener)
{
	_collision_listener = collision_listener;
}
//------------------------------------------------------------------------------------------------
CollisionListener* World::getCollisionListener()
{
	return _collision_listener;
}
//------------------------------------------------------------------------------------------------
World::World(SceneManager* manager) :
    _show_debug_geoms (false),
    _show_debug_contact (false),
    _manager (manager)
{
	_world = dWorldCreate();
	_contacts = dJointGroupCreate(0);

	_default_space = new HashTableSpace(this);
	_default_space->setAutoCleanup(false);
	_default_space->setInternalCollisions(true);

	setDamping(0.0,0.0);
	setHistorySize (1);
}
//------------------------------------------------------------------------------------------------
void World::setHistorySize(size_t historySize)
{
	_history_size = historySize;

	MaintainedItemIterator<Body> it = _body_list.getIterator();
	while(!it.end ())
	{
		Body * const b = (Body *) (it.getNext ());
		b->_historyResize(historySize);
	}
}
//------------------------------------------------------------------------------------------------
size_t World::getHistorySize() const 
{
	return _history_size;
}
//------------------------------------------------------------------------------------------------
void World::setGravity(const Ogre::Vector3& gravity)
{
	dWorldSetGravity(_world,(dReal)gravity.x,(dReal)gravity.y,(dReal)gravity.z);
}
//------------------------------------------------------------------------------------------------
const Ogre::Vector3& World::getGravity()
{
	dVector3 g;
	dWorldGetGravity(_world,g);
	_gravity.x = (Real)g[0];
	_gravity.y = (Real)g[1];
	_gravity.z = (Real)g[2];
	return _gravity;
}
//------------------------------------------------------------------------------------------------
void World::setERP(Real erp)
{
	dWorldSetERP(_world,(dReal)erp);
}
//------------------------------------------------------------------------------------------------
Real World::getERP()
{
	return (Real)dWorldGetERP(_world);
}
//------------------------------------------------------------------------------------------------
void World::setCFM(Real cfm)
{
	dWorldSetCFM(_world,(dReal)cfm);
}
//------------------------------------------------------------------------------------------------
Real World::getCFM()
{
	return (Real)dWorldGetCFM(_world);
}
//------------------------------------------------------------------------------------------------
void World::setAutoSleep(bool auto_sleep)
{
	dWorldSetAutoDisableFlag(_world,(auto_sleep)?1:0);
}
//------------------------------------------------------------------------------------------------
bool World::getAutoSleep()
{
	return (dWorldGetAutoDisableFlag(_world))?true:false;
}
//------------------------------------------------------------------------------------------------
void World::setAutoSleepLinearThreshold(Real linear_threshold)
{
	dWorldSetAutoDisableLinearThreshold(_world,(dReal)linear_threshold);
}
//------------------------------------------------------------------------------------------------
Real World::getAutoSleepLinearThreshold()
{
	return (Real)dWorldGetAutoDisableLinearThreshold(_world);
}
//------------------------------------------------------------------------------------------------
void World::setAutoSleepAngularThreshold(Real angular_threshold)
{
	dWorldSetAutoDisableAngularThreshold(_world,(dReal)angular_threshold);
}
//------------------------------------------------------------------------------------------------
Real World::getAutoSleepAngularThreshold()
{
	return (Real)dWorldGetAutoDisableAngularThreshold(_world);
}
//------------------------------------------------------------------------------------------------
void World::setAutoSleepSteps(int steps)
{
	dWorldSetAutoDisableSteps(_world,steps);
}
//------------------------------------------------------------------------------------------------
int World::getAutoSleepSteps()
{
	return dWorldGetAutoDisableSteps(_world);
}
//------------------------------------------------------------------------------------------------
void World::setAutoSleepTime(Real time)
{
	dWorldSetAutoDisableTime(_world,(dReal)time);
}
//------------------------------------------------------------------------------------------------
Real World::getAutoSleepTime()
{
	return (Real)dWorldGetAutoDisableTime(_world);
}
//------------------------------------------------------------------------------------------------
void World::setAutoSleepAverageSamplesCount(size_t time)
{
    dWorldSetAutoDisableAverageSamplesCount(_world, time);
}
//------------------------------------------------------------------------------------------------
size_t World::getAutoSleepAverageSamplesCount()
{
    return (Real)dWorldGetAutoDisableAverageSamplesCount(_world);
}
//------------------------------------------------------------------------------------------------
void World::setContactCorrectionVelocity(Real velocity)
{
	dWorldSetContactMaxCorrectingVel(_world,(dReal)velocity);
}
//------------------------------------------------------------------------------------------------
void World::setContactSurfaceLayer(Real layer)
{
	dWorldSetContactSurfaceLayer(_world,(dReal)layer);
}

//------------------------------------------------------------------------------------------------
void World::collisionCallback(void *data,dGeomID geom_a,dGeomID geom_b)
{
	const bool a_space = (dGeomIsSpace(geom_a))?true:false;
	const bool b_space = (dGeomIsSpace(geom_b))?true:false;
	
	void* const ptr_a = dGeomGetData(geom_a);
	void* const ptr_b = dGeomGetData(geom_b);

	if(a_space  || b_space )
	{
		// Collide a space with a space
		if(a_space && b_space) 
			static_cast<Space*>(ptr_a)->collide(static_cast<Space*>(ptr_b),data);
		else if(a_space) 
			static_cast<Space*>(ptr_a)->collide(static_cast<Geometry*>(ptr_b),data);
		else 
			static_cast<Space*>(ptr_b)->collide(static_cast<Geometry*>(ptr_a),data);

		// Collide geometries internal to the spaces
		if(a_space) 
            static_cast<Space*>(ptr_a)->collide(data);

		if(b_space) 
            static_cast<Space*>(ptr_b)->collide(data);
	}
    else
	{
		// Collide a geom with a geom, i.e. generate contacts
		static_cast<Geometry*>(ptr_a)->collide(static_cast<Geometry*>(ptr_b),_collision_listener);
	}
}
//------------------------------------------------------------------------------------------------
Body* World::findBody(SceneNode* node)
{
	Body* body = 0;
	for(int i = 0;i < node->numAttachedObjects();i++)
	{
		MovableObject* obj = node->getAttachedObject(i);
		if(obj)
		{
			if(obj->getMovableType() == Ogre::String("OgreOde::Body"))
			{
				body = static_cast<Body*>(obj);
				break;
			}
		}
	}
	return body;
}
//------------------------------------------------------------------------------------------------
Body* World::findBody(const Ogre::String& name)
{
	Body *b = 0;
	MaintainedItemIterator<Body> it = _body_list.getIterator();
	while(!it.end ())
	{
		b = (Body*) (it.getNext ());
		if(b->getName() == name) 
			return b;
	}
	return 0;
}
//------------------------------------------------------------------------------------------------
void World::setDefaultSpace(Space* space)
{
	delete _default_space;
	_default_space = space;
}
//------------------------------------------------------------------------------------------------
void World::setQuickStepIterations(int iterations)
{
	dWorldSetQuickStepNumIterations(_world,iterations);
}
//------------------------------------------------------------------------------------------------
int World::getQuickStepIterations()
{
	return dWorldGetQuickStepNumIterations(_world);
}
//------------------------------------------------------------------------------------------------
void World::setShowDebugGeometries(bool show)
{
	_body_list.setDebug(show);
	_geometry_list.setDebug(show);
	_show_debug_geoms = show;
}
//------------------------------------------------------------------------------------------------
void World::setShowDebugContact(bool show)
{
    _geometry_list.setDebugContact(show);
    _show_debug_contact = show;
}
//------------------------------------------------------------------------------------------------
void World::setDamping(Real linear_damping,Real angular_damping)
{
	_linear_damping = -(dReal)linear_damping;
	_angular_damping = -(dReal)angular_damping;
}
//------------------------------------------------------------------------------------------------
Real World::getLinearDamping()
{
	return -(Real)_linear_damping;
}
//------------------------------------------------------------------------------------------------
Real World::getAngularDamping()
{
	return -(Real)_angular_damping;
}
//------------------------------------------------------------------------------------------------
World::~World()
{
	delete _default_space;
	dJointGroupDestroy(_contacts);
    dWorldDestroy(_world);
    dCloseODE();
}
