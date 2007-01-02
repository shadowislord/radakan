
#include "OgreOdePrecompiledHeaders.h"

#include "OgreOdeBody.h"
#include "OgreOdeSpace.h"
#include "OgreOdeGeometry.h"
#include "OgreOdeWorld.h"

using namespace OgreOde;
using namespace Ogre;

//------------------------------------------------------------------------------------------------
Space::Space(World *world, const Space* space) :
    _world(world),
    _internal_collisions (true)
{
}
//------------------------------------------------------------------------------------------------
dSpaceID Space::getSpaceID() const
{
	return _space;
}
//------------------------------------------------------------------------------------------------
dSpaceID Space::getSpaceID(const Space* space) const
{
	if(space) 
        return space->getSpaceID();
	return 0;
}
//------------------------------------------------------------------------------------------------
void Space::setAutoCleanup(bool on)
{
	dSpaceSetCleanup(_space,(on)?1:0);
}
//------------------------------------------------------------------------------------------------
bool Space::getAutoCleanup()
{
	return ((dSpaceGetCleanup(_space))?true:false); 
}
//------------------------------------------------------------------------------------------------
void Space::addGeometry(const Geometry& geometry)
{
	dSpaceAdd(_space,geometry.getGeometryID()); 
}
//------------------------------------------------------------------------------------------------
void Space::removeGeometry(const Geometry& geometry)
{
	dSpaceRemove(_space,geometry.getGeometryID()); 
}
//------------------------------------------------------------------------------------------------
bool Space::containsGeometry(const Geometry& geometry)
{
	return ((dSpaceQuery(_space,geometry.getGeometryID()))?true:false); 
}
//------------------------------------------------------------------------------------------------
int Space::getGeometryCount()
{
	return dSpaceGetNumGeoms(_space); 
}
//------------------------------------------------------------------------------------------------
Geometry* Space::getGeometry(int index)
{
	return (Geometry*) _world->getGeometryList().findItem((unsigned int)dSpaceGetGeom(_space,index)); 
}
//------------------------------------------------------------------------------------------------
void Space::registerSpace()
{
	_world->getSpaceList().registerItem(this);
	dGeomSetData((dGeomID)_space,(void*)this);
}
//------------------------------------------------------------------------------------------------
void Space::collide(void* data)
{
	if(_internal_collisions)
	{
		dSpaceCollide(_space,data,World::collisionCallback);
	}
}
//------------------------------------------------------------------------------------------------
void Space::collide(Space* space,void* data)
{
	dSpaceCollide2((dGeomID)_space,(dGeomID)(space->getSpaceID()),data,World::collisionCallback);
}
//------------------------------------------------------------------------------------------------
void Space::collide(Geometry* geometry,void* data)
{
	dSpaceCollide2((dGeomID)_space,geometry->getGeometryID(),data,World::collisionCallback);
}

void Space::setInternalCollisions(bool collide)
{
	_internal_collisions = collide;
}
//------------------------------------------------------------------------------------------------
bool Space::getInternalCollisions()
{
	return _internal_collisions;
}
//------------------------------------------------------------------------------------------------
const AxisAlignedBox& Space::getAxisAlignedBox()
{
	dReal aabb[6];
	dGeomGetAABB((dGeomID)_space,aabb);
	_bounding_box.setExtents((Real)aabb[0],(Real)aabb[2],(Real)aabb[4],(Real)aabb[1],(Real)aabb[3],(Real)aabb[5]);
	return _bounding_box;
}
//------------------------------------------------------------------------------------------------
unsigned long Space::getID()
{
	return (unsigned long)_space;
}
//------------------------------------------------------------------------------------------------
Space::~Space()
{
	_world->getSpaceList().unregisterItem((unsigned long)_space);
	dSpaceDestroy(_space); 
}
//------------------------------------------------------------------------------------------------
SimpleSpace::SimpleSpace(World *world, const Space* space) : Space (world, space)
{
	_space = dSimpleSpaceCreate(getSpaceID(space)); 
	registerSpace();
}
//------------------------------------------------------------------------------------------------
SimpleSpace::~SimpleSpace()
{
}
//------------------------------------------------------------------------------------------------
HashTableSpace::HashTableSpace(World *world, const Space* space) : Space (world, space)
{
	_space = dHashSpaceCreate(getSpaceID(space)); 
	registerSpace();
}
//------------------------------------------------------------------------------------------------
void HashTableSpace::setLevels(int min_level,int max_level)
{
	dHashSpaceSetLevels(_space,min_level,max_level); 
}
//------------------------------------------------------------------------------------------------
int HashTableSpace::getMinimumLevel()
{
	int min_level,max_level;
	dHashSpaceGetLevels(_space,&min_level,&max_level);
	return min_level;
}
//------------------------------------------------------------------------------------------------
int HashTableSpace::getMaximumLevel()
{
	int min_level,max_level;
	dHashSpaceGetLevels(_space,&min_level,&max_level);
	return max_level;
}
//------------------------------------------------------------------------------------------------
HashTableSpace::~HashTableSpace()
{
}
//------------------------------------------------------------------------------------------------
QuadTreeSpace::QuadTreeSpace(const Ogre::Vector3& center,const Ogre::Vector3& extents,int depth,World *world, const Space* space) : Space (world, space)
{
	dVector3 c,e;
	
	c[0] = (dReal)center.x;
	c[1] = (dReal)center.y;
	c[2] = (dReal)center.z;

	e[0] = (dReal)extents.x;
	e[1] = (dReal)extents.y;
	e[2] = (dReal)extents.z;

	_space = dQuadTreeSpaceCreate(getSpaceID(space),c,e,depth); 
	registerSpace();
}
//------------------------------------------------------------------------------------------------
QuadTreeSpace::~QuadTreeSpace()
{
}
//------------------------------------------------------------------------------------------------
OgreSceneManagerSpace::OgreSceneManagerSpace(const Ogre::Vector3& center,
                                             const Ogre::Vector3& extents,
                                             int depth,
                                             Ogre::SceneManager *_scn_mgr,
                                             World *world, const Space* space) : 
        Space (world, space),
        _scn_mgr(_scn_mgr)
{
    _intersection_query = _scn_mgr->createIntersectionQuery();

    const std::set<Ogre::SceneQuery::WorldFragmentType> *supportedQueryTypes = _intersection_query->getSupportedWorldFragmentTypes();
    std::set<Ogre::SceneQuery::WorldFragmentType>::const_iterator it = 
        supportedQueryTypes->find (Ogre::SceneQuery::WFT_PLANE_BOUNDED_REGION);
    if (it != supportedQueryTypes->end())
    {
        _intersection_query->setWorldFragmentType(Ogre::SceneQuery::WFT_PLANE_BOUNDED_REGION);
        _scene_geometry = true;
    }
    else
    {
        _intersection_query->setWorldFragmentType(Ogre::SceneQuery::WFT_NONE);
        _scene_geometry = false;
    }

    // for now register a dummy space in ode.
    // perhaps this step can be avoided.dVector3 c,e;

    dVector3 c;
    c[0] = (dReal)center.x;
    c[1] = (dReal)center.y;
    c[2] = (dReal)center.z;

    dVector3 e;
    e[0] = (dReal)extents.x;
    e[1] = (dReal)extents.y;
    e[2] = (dReal)extents.z;

    _space = dQuadTreeSpaceCreate(getSpaceID(space), c, e,depth); 
    registerSpace();
}
//------------------------------------------------------------------------------------------------
OgreSceneManagerSpace::~OgreSceneManagerSpace()
{
    delete _intersection_query;
}
//------------------------------------------------------------------------------------------------
void OgreSceneManagerSpace::collide(void* data)
{
    if(_internal_collisions)
    {
        // Collision detection
        Ogre::IntersectionSceneQueryResult& results = _intersection_query->execute();

        Body *body1, *body2;
        Geometry *geom;
        Ogre::UserDefinedObject *uo1, *uo2;

        // Movables to Movables
        Ogre::SceneQueryMovableIntersectionList::iterator it, itend;
        itend = results.movables2movables.end();
        for (it = results.movables2movables.begin(); it != itend; ++it)
        {
            /* debugging
            MovableObject *mo1, *mo2;
            mo1 = it->first;
            mo2 = it->second;
            */

            // Get user defined objects (generic in OGRE)
            uo1 = it->first->getUserObject();
            uo2 = it->second->getUserObject();

            // Only perform collision if we have UserDefinedObject links
            if (uo1 && uo2)
            {
                bool isBody1 = false;
                if (uo1->getTypeName () == "Body")
                    isBody1 = true;

                bool isBody2 = false;
                if (uo2->getTypeName () == "Body")
                    isBody2 = true;
                if (isBody1 || isBody2)
                {
                    if (isBody2 && isBody1)
                    {
                        // Both are dynamic object
                       body1 = static_cast<Body*>(uo1);
                       body2 = static_cast<Body*>(uo2);

                        // Do detailed collision test
                        body1->collide (data, body2);
                    }
                    else
                    {
                        // Both are dynamic object
                        if (isBody1)
                        {
                            body1 = static_cast<Body*>     (uo1);
                            geom  = static_cast<Geometry*> (uo2);
                        }
                        else
                        {
                            geom  = static_cast<Geometry*> (uo1);
                            body1 = static_cast<Body*>     (uo2);
                        }

                        // Do detailed collision test
                        body1->collide(data, geom);
                    }
                }
            }
        }

        // Movables to World
        if (_scene_geometry)
        {
            Ogre::MovableObject *mo;
            Ogre::SceneQuery::WorldFragment *wf;

            Ogre::SceneQueryMovableWorldFragmentIntersectionList::iterator wit, witend;
            witend = results.movables2world.end();
            for (wit = results.movables2world.begin(); wit != witend; ++wit)
            {
                mo = wit->first;
                wf = wit->second;

                // Get user defined objects (generic in OGRE)
                uo1 = mo->getUserObject();
                // Only perform collision if we have UserDefinedObject link
                // otherwise ...
                if (uo1)
                {
                    // Cast to ApplicationObject
                    if (uo1->getTypeName () == "Body")
                    {
                        body1 = static_cast<Body*>(uo1);
                        body1->collidePlaneBounds(data, wf);
                    }
//                     else // static objects don't collide against Scene Geometry
//                     {
//                         geom = static_cast<Geometry*>(uo);
//                         // Do detailed collision test
//                     }
                }
            }
        }
        // no need to use that one.
        // dSpaceCollide(_space,data, World::collisionCallback);
    }
}
