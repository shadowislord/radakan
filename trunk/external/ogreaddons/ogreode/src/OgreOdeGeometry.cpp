
#include "OgreOdePrecompiledHeaders.h"

#include "OgreOdeGeometry.h"
#include "OgreOdeWorld.h"
#include "OgreOdeSpace.h"
#include "OgreOdeBody.h"
#include "OgreOdeCollision.h"
#include "OgreOdeDebugObject.h"
#include "OgreOdeDebugContact.h"

using namespace OgreOde;
using namespace Ogre;

//------------------------------------------------------------------------------------------------
int Geometry::_geometry_count = 0;

//------------------------------------------------------------------------------------------------
Geometry::Geometry(World *world, Space* space):
    UserDefinedObject(),
	_contact_high_water_mark (0),
    _last_contact_num(0),
	_max_contacts (32),
    _debug_contacts (0),
	_contacts (0),
	_encapsulator (0),
	_debug_obj (0),
	_debug_node (0),
	_user_data (0),
	_user_object (0),
	_geom (0),
    _world(world)
{
	
}
//------------------------------------------------------------------------------------------------
Geometry::~Geometry()
{
    destroyDebugObject();

    delete[] _contacts;
    if (_debug_contacts)
    {         
        for (unsigned int i = 0; i < _max_contacts; i++)
            delete _debug_contacts[i];
        delete [] _debug_contacts;
    }
    if (_geom)
    {
        _world->getGeometryList().unregisterItem((unsigned long)_geom);
        dGeomDestroy(_geom); 
    }
}
//------------------------------------------------------------------------------------------------
dGeomID Geometry::getGeometryID() const
{
	return _geom;
}

//------------------------------------------------------------------------------------------------
void Geometry::setEncapsulator(Geometry* encapsulator)
{
	_encapsulator = encapsulator;

	if (_world->getShowDebugGeometries())
	{
		reparentDebugObject(encapsulator->_debug_node);
	}
}

//------------------------------------------------------------------------------------------------
void Geometry::reparentDebugObject(Ogre::Node* node)
{
	if ((node)&&(_debug_node)&&(_debug_node->getParent() != node))
	{
		Ogre::SceneNode* parent = static_cast<Ogre::SceneNode*>(_debug_node->getParent());
		parent->removeChild(_debug_node->getName());
		node->addChild(_debug_node);

		_debug_node->setPosition(getPosition());
		_debug_node->setOrientation(getOrientation());
	}
}

//------------------------------------------------------------------------------------------------
void Geometry::createDebugObject()
{
	if (!_debug_node)
	{
		Ogre::SceneNode* parent = _world->_manager->getRootSceneNode();
		_debug_node = static_cast<Ogre::Node*>(parent->createChildSceneNode(Ogre::String("OgreOde::Geometry_") + Ogre::StringConverter::toString(_geometry_count) + Ogre::String("_Debug"))); 

        if (_debug_obj)
        {
            static_cast<Ogre::SceneNode*>(_debug_node)->attachObject(_debug_obj);
        }

        if (getBody())
        {
            static_cast<Ogre::SceneNode*>(_debug_node->getParent())->removeChild(_debug_node->getName());
            getBody()->addDebugNode(_debug_node);
            if (_debug_obj) 
                _debug_obj->setMode(DebugObject::Mode_Enabled);
        }
		else if (_encapsulator)
		{
			reparentDebugObject(_encapsulator->_debug_node);
		}
		else
		{
			if (_debug_obj) 
                _debug_obj->setMode(DebugObject::Mode_Static);
			
			_debug_node->setPosition(getPosition());
			_debug_node->setOrientation(getOrientation());
		}
		_geometry_count++;
	}
}

//------------------------------------------------------------------------------------------------
void Geometry::destroyDebugObject()
{
	if (_debug_node)
    {

        Ogre::SceneNode* sn = static_cast<Ogre::SceneNode*>(_debug_node);
        sn->removeAndDestroyAllChildren ();
        sn = static_cast<Ogre::SceneNode*>(_debug_node->getParent());
        sn->removeAndDestroyChild(_debug_node->getName());
		_debug_node = 0;
	}

	if (_debug_obj)
	{
		delete _debug_obj;
		_debug_obj = 0;
	}
}

//------------------------------------------------------------------------------------------------
void Geometry::registerGeometry()
{
	dGeomSetData(_geom, (void*)this);
    _world->getGeometryList().registerItem(this);

    if (_world->getShowDebugGeometries()) 
        createDebugObject();

    setMaxContacts (_max_contacts);
}
//------------------------------------------------------------------------------------------------
dSpaceID Geometry::getSpaceID(Space* space) const
{
	if (!space) return 0;
	return space->getSpaceID();
}

//------------------------------------------------------------------------------------------------
void Geometry::setBody(Body* body)
{
	destroyDebugObject();
    if (body)
    {
        body->addGeometry (this);
        dGeomSetBody(_geom, body->getBodyID()); 
    }
    else
    {
        dGeomSetBody(_geom, 0); 
    }
	if (_world->getShowDebugGeometries()) 
        createDebugObject();
}

//------------------------------------------------------------------------------------------------
Body* Geometry::getBody()
{
	const dBodyID body = dGeomGetBody(_geom);
	if (!body) return 0;
	else return (Body*)dBodyGetData(body); 
}

//------------------------------------------------------------------------------------------------
void Geometry::notify(Body* body)
{
	if (getBody() == body)
	{
		destroyDebugObject();
	}
}

//------------------------------------------------------------------------------------------------
void Geometry::setDebug(const bool debug)
{
    destroyDebugObject();
    if (debug) 
        createDebugObject();    
}
//------------------------------------------------------------------------------------------------
void Geometry::setDebugContact(const bool debug)
{    
    if (_debug_contacts)
    {
        for (unsigned int i = 0; i < _max_contacts; i++)
            delete _debug_contacts[i];
        delete [] _debug_contacts;
        _debug_contacts = 0;
    }
    if (debug) 
    {
        _debug_contacts = new DebugContact*[_max_contacts];
        for (unsigned int i = 0; i < _max_contacts; i++)
            _debug_contacts[i] = new DebugContact(Ogre::StringConverter::toString((int)_geom) + 
                                                    "_Contact_" + 
                                                    Ogre::StringConverter::toString(i),
                                                    _world);
    }
}
//------------------------------------------------------------------------------------------------
void Geometry::setPosition(const Ogre::Vector3& position)
{
    dGeomSetPosition(_geom,(dReal)position.x,(dReal)position.y,(dReal)position.z);

    if ((_debug_node)&& ((!getBody()) || (_encapsulator)))
        _debug_node->setPosition(position);
}

//------------------------------------------------------------------------------------------------
void Geometry::setOrientation(const Ogre::Quaternion& orientation)
{
	dQuaternion q;
	q[0] = (dReal)orientation.w;
	q[1] = (dReal)orientation.x;
	q[2] = (dReal)orientation.y;
	q[3] = (dReal)orientation.z;
	dGeomSetQuaternion(_geom,q); 

	if ((_debug_node)&& ((!getBody()) || (_encapsulator))) _debug_node->setOrientation(orientation);
}

//------------------------------------------------------------------------------------------------
const Ogre::Vector3& Geometry::getPosition()
{
	const dReal* position = dGeomGetPosition(_geom);
	_position.x = (Ogre::Real)position[0];
	_position.y = (Ogre::Real)position[1];
	_position.z = (Ogre::Real)position[2];
	return _position;
}

//------------------------------------------------------------------------------------------------
const Ogre::Quaternion& Geometry::getOrientation()
{
	dQuaternion orientation;
	dGeomGetQuaternion(_geom,orientation); 
	_orientation.w = (Real)orientation[0];
	_orientation.x = (Real)orientation[1];
	_orientation.y = (Real)orientation[2];
	_orientation.z = (Real)orientation[3];
	return _orientation;
}

//------------------------------------------------------------------------------------------------
const AxisAlignedBox& Geometry::getAxisAlignedBox()
{
	dReal aabb[6];
	dGeomGetAABB(_geom,aabb);
	_bounding_box.setExtents((Real)aabb[0],(Real)aabb[2],(Real)aabb[4],(Real)aabb[1],(Real)aabb[3],(Real)aabb[5]);
	return _bounding_box;
}

//------------------------------------------------------------------------------------------------
Space* Geometry::getSpace()
{
	return (Space*)_world->getSpaceList().findItem((unsigned int)dGeomGetSpace(_geom));
}


//------------------------------------------------------------------------------------------------
void Geometry::enable()
{
	dGeomEnable(_geom);
}

//------------------------------------------------------------------------------------------------
void Geometry::disable()
{
	dGeomDisable(_geom); 
}

//------------------------------------------------------------------------------------------------
bool Geometry::isEnabled()
{
	return dGeomIsEnabled(_geom); 
}

//------------------------------------------------------------------------------------------------
Geometry::Class Geometry::getClass() const
{
	return (Geometry::Class)dGeomGetClass(_geom);
}

//------------------------------------------------------------------------------------------------
void Geometry::setCategoryBitfield(unsigned long bits)
{
	dGeomSetCategoryBits(_geom,bits); 
}

//------------------------------------------------------------------------------------------------
void Geometry::setCollisionBitfield(unsigned long bits)
{
	dGeomSetCollideBits(_geom,bits); 
}

//------------------------------------------------------------------------------------------------
unsigned long Geometry::getCategoryBitfield()
{
	return dGeomGetCategoryBits(_geom); 
}

//------------------------------------------------------------------------------------------------
unsigned long Geometry::getCollisionBitfield()
{
	return dGeomGetCollideBits(_geom); 
}

//------------------------------------------------------------------------------------------------
int Geometry::collide(Geometry* geometry,CollisionListener* listener)
{
	const unsigned int num_contacts = (unsigned int) dCollide(_geom,
										geometry->getGeometryID(),
										_max_contacts,
										&(_contacts[0].geom),
										sizeof(dContact));
	if (num_contacts)
	{
		_contact_high_water_mark = std::max(_contact_high_water_mark,num_contacts);

		Contact contact;
		
		const dWorldID wid = _world->getWorldID();
		const dJointGroupID cid = _world->getContactGroupID();
		const dBodyID b1 = dGeomGetBody(_geom);
		const dBodyID b2 = dGeomGetBody(geometry->getGeometryID());

		if (listener)
		{
			for(unsigned int i = 0;i < num_contacts;i++)
			{
				contact.setContact (&_contacts[i]);

				if (listener->collision(&contact))
				{
					dJointAttach(dJointCreateContact(wid,cid,&_contacts[i]),b1,b2);
                } 
			}
		}
		else
		{
			for(unsigned int i = 0;i < num_contacts;i++)
			{
				contact.setContact (&_contacts[i]);
   
				dJointAttach(dJointCreateContact(wid,cid,&_contacts[i]), b1, b2);
			}
		} 
    }
    _last_contact_num = num_contacts;
	return num_contacts;
}
//------------------------------------------------------------------------------------------------
void Geometry::updateDebugContact()
{
    assert (_world->getShowDebugContact ());
    assert (_debug_contacts);
    {
        unsigned int k = 0;
        while (k < _max_contacts)
        {
            _debug_contacts[k++]->setEnabled(false);
        }

        if (_last_contact_num)
        {
            Contact contact;
            for(unsigned int i = 0;i < _last_contact_num;i++)
            {
                k = 0;
                while (k < _max_contacts)
                {
                    if (_debug_contacts[k]->isEnabled() == false)
                        break;
                    k++;
                }
                assert (k < _max_contacts);

                _debug_contacts[k]->setEnabled(true);
                contact.setContact (&_contacts[i]);
                _debug_contacts[k]->update (&contact);
            }
        }
    }
    _last_contact_num = 0;
 }
//------------------------------------------------------------------------------------------------
unsigned int Geometry::getMaxContacts() const
{
	return _max_contacts;
}

//------------------------------------------------------------------------------------------------
void Geometry::setMaxContacts(unsigned int max_contacts)
{
	delete[] _contacts;
	_contacts = new dContact[max_contacts];

    if (_world->getShowDebugContact ())
    {
        if (_debug_contacts)
        {         
            for (unsigned int i = 0; i < _max_contacts; i++)
                delete _debug_contacts[i];
            delete [] _debug_contacts;
        }
        _debug_contacts = new DebugContact*[max_contacts];
        for (unsigned int i = 0; i < max_contacts; i++)
            _debug_contacts[i] = new DebugContact(Ogre::StringConverter::toString((int)_geom) +  + "_Contact_" + Ogre::StringConverter::toString(i),
            _world);
    }
    _max_contacts = max_contacts;
}

//------------------------------------------------------------------------------------------------
unsigned int Geometry::getContactHighWaterMark() const
{
	return _contact_high_water_mark;
}

//------------------------------------------------------------------------------------------------
unsigned long Geometry::getID() const
{
	return (unsigned long)_geom;
}
//------------------------------------------------------------------------------------------------
void Geometry::clearOffset()
{
	dGeomClearOffset(_geom);
}
//------------------------------------------------------------------------------------------------
int Geometry::isOffset()
{
	return dGeomIsOffset(_geom);
}
//------------------------------------------------------------------------------------------------
void Geometry::setOffsetPosition (const Ogre::Vector3 &pos)
{
	dGeomSetOffsetPosition (_geom, pos.x, pos.y, pos.z);
}
//------------------------------------------------------------------------------------------------
void Geometry::setOffsetQuaternion(const Ogre::Quaternion &quat) 
{
	dQuaternion q;
	q[0] = quat.x; 
	q[1] = quat.y; 
	q[2] = quat.z; 
	q[3] = quat.w;
	dGeomSetOffsetQuaternion (_geom, q);
}
//------------------------------------------------------------------------------------------------
void Geometry::setOffsetWorldPosition(const Ogre::Vector3 &pos) 
{
	dGeomSetOffsetWorldPosition(_geom, pos.x, pos.y, pos.z);
}
//------------------------------------------------------------------------------------------------
void Geometry::setOffsetWorldQuaternion(const Ogre::Quaternion &quat) 
{
	dQuaternion q;
	q[0] = quat.x; 
	q[1] = quat.y; 
	q[2] = quat.z; 
	q[3] = quat.w;
	dGeomSetOffsetWorldQuaternion (_geom, q);
}
//------------------------------------------------------------------------------------------------
Vector3 Geometry::getOffsetPosition() 
{
	const dReal *p =  dGeomGetOffsetPosition (_geom);
	return Ogre::Vector3(p[0], p[1], p[2]);
}
//------------------------------------------------------------------------------------------------
Quaternion Geometry::getOffsetQuaternion() 
{
	dQuaternion q;
	dGeomGetOffsetQuaternion (_geom, q);
	return Ogre::Quaternion(q[0], q[1], q[2], q[3]);
}
//------------------------------------------------------------------------------------------------
SphereGeometry::SphereGeometry(Real radius, World *world, Space* space):Geometry(world, space)
{
	_geom = dCreateSphere(getSpaceID(space),(dReal)radius); 
	registerGeometry();
}

//------------------------------------------------------------------------------------------------
void SphereGeometry::setRadius(Real radius)
{
	dGeomSphereSetRadius(_geom,(dReal)radius);
}

//------------------------------------------------------------------------------------------------
Real SphereGeometry::getRadius()
{
	return (Real)dGeomSphereGetRadius(_geom); 
}

//------------------------------------------------------------------------------------------------
Real SphereGeometry::getPointDepth(const Ogre::Vector3& point)
{
	return (Real)dGeomSpherePointDepth(_geom,(dReal)point.x,(dReal)point.y,(dReal)point.z); 
}

//------------------------------------------------------------------------------------------------
void SphereGeometry::createDebugObject()
{
	_debug_obj = new SphereDebugObject(getRadius());
	Geometry::createDebugObject();
}

//------------------------------------------------------------------------------------------------
SphereGeometry::~SphereGeometry()
{
}

//------------------------------------------------------------------------------------------------
BoxGeometry::BoxGeometry(const Ogre::Vector3& size, World *world, Space* space):Geometry(world, space)
{
	_geom = dCreateBox(getSpaceID(space),(dReal)size.x,(dReal)size.y,(dReal)size.z); 
	registerGeometry();
}

//------------------------------------------------------------------------------------------------
void BoxGeometry::setSize(const Ogre::Vector3& size)
{
	dGeomBoxSetLengths(_geom,(dReal)size.x,(dReal)size.y,(dReal)size.z); 
}

//------------------------------------------------------------------------------------------------
const Ogre::Vector3& BoxGeometry::getSize()
{
	dVector3 result;
	dGeomBoxGetLengths(_geom,result); 
	_size.x = (Real)result[0];
	_size.y = (Real)result[1];
	_size.z = (Real)result[2];
	return _size;
}

//------------------------------------------------------------------------------------------------
Real BoxGeometry::getPointDepth(const Ogre::Vector3& point)
{
	return (Real)dGeomBoxPointDepth(_geom,(dReal)point.x,(dReal)point.y,(dReal)point.z); 
}

//------------------------------------------------------------------------------------------------
void BoxGeometry::createDebugObject()
{
	_debug_obj = new BoxDebugObject(getSize());
	Geometry::createDebugObject();	
}

//------------------------------------------------------------------------------------------------
BoxGeometry::~BoxGeometry()
{
}

//------------------------------------------------------------------------------------------------
InfinitePlaneGeometry::InfinitePlaneGeometry(const Plane& plane,World *world, Space* space):Geometry(world, space)
{
	_geom = dCreatePlane(getSpaceID(space),(dReal)plane.normal.x,(dReal)plane.normal.y,(dReal)plane.normal.z,(dReal)-plane.d); 
	registerGeometry();
}

//------------------------------------------------------------------------------------------------
void InfinitePlaneGeometry::setDefinition(const Plane& plane)
{
	dGeomPlaneSetParams(_geom,(dReal)plane.normal.x,(dReal)plane.normal.y,(dReal)plane.normal.z,(dReal)-plane.d); 
}

//------------------------------------------------------------------------------------------------
const Plane& InfinitePlaneGeometry::getDefinition()
{
	dVector4 result;
	dGeomPlaneGetParams(_geom,result); 
	_plane.normal.x = result[0];
	_plane.normal.y = result[1];
	_plane.normal.z = result[2];
	_plane.d = -result[3];
	return _plane;
}

//------------------------------------------------------------------------------------------------
Real InfinitePlaneGeometry::getPointDepth(const Ogre::Vector3& point)
{
	return (Real)dGeomPlanePointDepth(_geom,(dReal)point.x,(dReal)point.y,(dReal)point.z); 
}

//------------------------------------------------------------------------------------------------
void InfinitePlaneGeometry::setPosition(const Ogre::Vector3& position)
{
}

//------------------------------------------------------------------------------------------------
void InfinitePlaneGeometry::setOrientation(const Ogre::Quaternion& orientation)
{
}

//------------------------------------------------------------------------------------------------
const Ogre::Vector3& InfinitePlaneGeometry::getPosition()
{
	return Ogre::Vector3::ZERO;
}

//------------------------------------------------------------------------------------------------
const Ogre::Quaternion& InfinitePlaneGeometry::getOrientation()
{
	return Ogre::Quaternion::ZERO;
}

//------------------------------------------------------------------------------------------------
const AxisAlignedBox& InfinitePlaneGeometry::getAxisAlignedBox()
{
	return _bounding_box;
}

//------------------------------------------------------------------------------------------------
InfinitePlaneGeometry::~InfinitePlaneGeometry()
{
}

//------------------------------------------------------------------------------------------------
CapsuleGeometry::CapsuleGeometry(Real radius,Real length,World *world, Space* space):Geometry(world, space)
{
	_geom = dCreateCapsule(getSpaceID(space),(dReal)radius,(dReal)length);
	registerGeometry();
}

//------------------------------------------------------------------------------------------------
void CapsuleGeometry::setDefinition(Real radius,Real length)
{
	dGeomCapsuleSetParams(_geom,(dReal)radius,(dReal)length);
}

//------------------------------------------------------------------------------------------------
Real CapsuleGeometry::getRadius()
{
	dReal radius,length;
	dGeomCapsuleGetParams(_geom,&radius,&length); 
	return radius;
}

//------------------------------------------------------------------------------------------------
Real CapsuleGeometry::getLength()
{
	dReal radius,length;
	dGeomCapsuleGetParams(_geom,&radius,&length); 
	return length;
}

//------------------------------------------------------------------------------------------------
Real CapsuleGeometry::getPointDepth(const Ogre::Vector3& point)
{
	return (Real)dGeomCapsulePointDepth(_geom,(dReal)point.x,(dReal)point.y,(dReal)point.z); 
}

//------------------------------------------------------------------------------------------------
void CapsuleGeometry::createDebugObject()
{
	_debug_obj = new CapsuleDebugObject(getRadius(),getLength());
	Geometry::createDebugObject();
}

//------------------------------------------------------------------------------------------------
CapsuleGeometry::~CapsuleGeometry()
{
}

//------------------------------------------------------------------------------------------------
CylinderGeometry::CylinderGeometry(Real radius,Real length,World *world, Space* space):Geometry(world, space)
{
	_geom = dCreateCCylinder(getSpaceID(space),(dReal)radius,(dReal)length);
	registerGeometry();
}

//------------------------------------------------------------------------------------------------
void CylinderGeometry::setDefinition(Real radius,Real length)
{
	dGeomCCylinderSetParams(_geom,(dReal)radius,(dReal)length);
}

//------------------------------------------------------------------------------------------------
Real CylinderGeometry::getRadius()
{
	dReal radius,length;
	dGeomCCylinderGetParams(_geom,&radius,&length); 
	return radius;
}

//------------------------------------------------------------------------------------------------
Real CylinderGeometry::getLength()
{
	dReal radius,length;
	dGeomCCylinderGetParams(_geom,&radius,&length); 
	return length;
}

//------------------------------------------------------------------------------------------------
Real CylinderGeometry::getPointDepth(const Ogre::Vector3& point)
{
	return (Real)dGeomCCylinderPointDepth(_geom,(dReal)point.x,(dReal)point.y,(dReal)point.z); 
}

//------------------------------------------------------------------------------------------------
void CylinderGeometry::createDebugObject()
{
	_debug_obj = new CylinderDebugObject(getRadius(),getLength());
	Geometry::createDebugObject();
}

//------------------------------------------------------------------------------------------------
CylinderGeometry::~CylinderGeometry()
{
}
//------------------------------------------------------------------------------------------------
RayGeometry::RayGeometry(Real length,World *world, Space* space) : Geometry(world, space)
{
	_geom = dCreateRay(getSpaceID(space),(dReal)length);
	registerGeometry();
}

//------------------------------------------------------------------------------------------------
void RayGeometry::createDebugObject()
{
	_debug_obj = new RayDebugObject(getStart(),getDirection(),getLength());
	Geometry::createDebugObject();
	if (_debug_node)
	{
		_debug_node->setPosition(Vector3::ZERO);
		_debug_node->setOrientation(Quaternion::IDENTITY);
	}
}

//------------------------------------------------------------------------------------------------
void RayGeometry::setLength(Real length)
{
	dGeomRaySetLength(_geom,(dReal)length); 
}

//------------------------------------------------------------------------------------------------
Real RayGeometry::getLength()
{
	return (Real)dGeomRayGetLength(_geom); 
}

//------------------------------------------------------------------------------------------------
void RayGeometry::setDefinition(const Ogre::Vector3& start,const Ogre::Vector3& direction)
{
	dGeomRaySet(_geom,
					(dReal)start.x,(dReal)start.y,(dReal)start.z,
					(dReal)direction.x,(dReal)direction.y,(dReal)direction.z);

	if ((_debug_node)&& ((!getBody()) || (_encapsulator))) 
		(static_cast<RayDebugObject *>(_debug_obj))->setDefinition(start, direction, (Real)dGeomRayGetLength(_geom));

}

//------------------------------------------------------------------------------------------------
const Ogre::Vector3& RayGeometry::getStart()
{
	dVector3 start,direction;
	dGeomRayGet(_geom,start,direction); 
	_start.x = (Real)start[0];
	_start.y = (Real)start[1];
	_start.z = (Real)start[2];
	return _start;
}

//------------------------------------------------------------------------------------------------
const Ogre::Vector3& RayGeometry::getDirection()
{
	dVector3 start,direction;
	dGeomRayGet(_geom,start,direction); 
	_direction.x = (Real)direction[0];
	_direction.y = (Real)direction[1];
	_direction.z = (Real)direction[2];
	return _direction;
}

//------------------------------------------------------------------------------------------------
RayGeometry::~RayGeometry()
{
}

//------------------------------------------------------------------------------------------------
TransformGeometry::TransformGeometry(World *world, Space* space) : Geometry(world, space)
{
	_geom = dCreateGeomTransform(getSpaceID(space)); 
	dGeomTransformSetCleanup(_geom,0);
	dGeomTransformSetInfo(_geom,1);
	registerGeometry();
}

//------------------------------------------------------------------------------------------------
void TransformGeometry::setEncapsulatedGeometry(Geometry* geometry)
{
	dGeomTransformSetGeom(_geom,geometry->getGeometryID()); 
	destroyDebugObject();
	if (_world->getShowDebugGeometries()) 
		createDebugObject();	

	geometry->setEncapsulator(this);
}

//------------------------------------------------------------------------------------------------
void TransformGeometry::createDebugObject()
{
	Geometry::createDebugObject();
	if (getEncapsulatedGeometry())
	{
		getEncapsulatedGeometry()->destroyDebugObject();
		getEncapsulatedGeometry()->createDebugObject();
	}
}

//------------------------------------------------------------------------------------------------
void TransformGeometry::destroyDebugObject()
{
	if (getEncapsulatedGeometry()) getEncapsulatedGeometry()->destroyDebugObject();
	Geometry::destroyDebugObject();
}

//------------------------------------------------------------------------------------------------
Geometry* TransformGeometry::getEncapsulatedGeometry() const
{
	dGeomID id = dGeomTransformGetGeom(_geom);
	if (id == 0) return 0;
	else return (Geometry*)dGeomGetData(id); 
}

//------------------------------------------------------------------------------------------------
TransformGeometry::~TransformGeometry()
{
}


//------------------------------------------------------------------------------------------------
TriangleMeshGeometry::TriangleMeshGeometry(const Ogre::Vector3* vertices,
                                           unsigned int vertex_count,
                                           const unsigned int* indices,
                                           unsigned int index_count,
                                           World *world, Space* space) : 
    Geometry(world, space),
    _vertex_count (vertex_count),
    _index_count (index_count)
{
	_vertex_count = vertex_count;
	_index_count = index_count;
	_vertices = new dVector3[vertex_count];
	_indices = new unsigned int[index_count];

	for(unsigned int i = 0;i < vertex_count;i++)
	{
		_vertices[i][0] = (dReal)vertices[i].x;
		_vertices[i][1] = (dReal)vertices[i].y;
		_vertices[i][2] = (dReal)vertices[i].z;
	}

	memcpy(_indices,indices,sizeof(unsigned int) * index_count);

	_data = dGeomTriMeshDataCreate(); 
	dGeomTriMeshDataBuildSimple(_data,(const dReal*)_vertices, (int)vertex_count, (int*)_indices, (int)index_count); 
	_geom = dCreateTriMesh(getSpaceID(space),_data,0,0,0);
	registerGeometry();

	_collision_listener = 0;
}

//------------------------------------------------------------------------------------------------
Vector3 TriangleMeshGeometry::getPoint(unsigned int index,const Ogre::Vector3& uv)
{
	dVector3 out;
	dGeomTriMeshGetPoint(_geom, (int) index, (dReal)uv.x, (dReal)uv.y, out); 
	return Ogre::Vector3((Real)out[0],(Real)out[1],(Real)out[2]);
}

//------------------------------------------------------------------------------------------------
TriangleMeshTriangle TriangleMeshGeometry::getTriangle(int index)
{
	dVector3 v0,v1,v2;
	dGeomTriMeshGetTriangle(_geom,(int)index, &v0, &v1, &v2); 
	
	TriangleMeshTriangle tri;

	tri.v0.x = v0[0]; tri.v0.y = v0[1]; tri.v0.z = v0[2];
	tri.v1.x = v1[0]; tri.v1.y = v1[1]; tri.v1.z = v1[2];
	tri.v2.x = v2[0]; tri.v2.y = v2[1]; tri.v2.z = v2[2];
	
	return tri;
}

//------------------------------------------------------------------------------------------------
void TriangleMeshGeometry::clearTemporalCoherenceCache()
{
	dGeomTriMeshClearTCCache(_geom); 
}

//------------------------------------------------------------------------------------------------
void TriangleMeshGeometry::enableTemporalCoherence(Geometry::Class geometry_class, bool enable)
{
	assert((geometry_class == Geometry::Class_Sphere)||(geometry_class == Geometry::Class_Box));
	dGeomTriMeshEnableTC(_geom,(int)geometry_class,(enable)?1:0); 
}

//------------------------------------------------------------------------------------------------
bool TriangleMeshGeometry::isTemporalCoherenceEnabled(Geometry::Class geometry_class)
{
	return ((dGeomTriMeshIsTCEnabled(_geom,(int)geometry_class))?true:false);
}

//------------------------------------------------------------------------------------------------
int TriangleMeshGeometry::_collisionCallback(dGeomID mesh,dGeomID object,int triangle)
{
	TriangleMeshGeometry* trimesh = (TriangleMeshGeometry*)dGeomGetData(mesh);
	if (trimesh->_collision_listener)
	{
		Geometry* geometry = (object)?((Geometry*)dGeomGetData(object)):0;
		return ((trimesh->_collision_listener->collide(trimesh,geometry,triangle))?1:0);
	}
	return 1;
}

//------------------------------------------------------------------------------------------------
void TriangleMeshGeometry::setCollisionListener(TriangleMeshCollisionListener* collision_listener)
{
	_collision_listener = collision_listener;
	dGeomTriMeshSetCallback(_geom,(_collision_listener)?TriangleMeshGeometry::_collisionCallback:0); 
}

//------------------------------------------------------------------------------------------------
void TriangleMeshGeometry::_intersectionCallback(dGeomID mesh,dGeomID object,const int* triangles,int triangle_count)
{
	TriangleMeshGeometry* trimesh = (TriangleMeshGeometry*)dGeomGetData(mesh);
	if (trimesh->_intersection_listener)
	{
		Geometry* geometry = (object)?((Geometry*)dGeomGetData(object)):0;
		trimesh->_intersection_listener->intersect(trimesh,geometry,triangles,triangle_count);
	}
}

//------------------------------------------------------------------------------------------------
void TriangleMeshGeometry::setIntersectionListener(TriangleMeshIntersectionListener* intersection_listener)
{
	_intersection_listener = intersection_listener;
	dGeomTriMeshSetArrayCallback(_geom,(_intersection_listener)?TriangleMeshGeometry::_intersectionCallback:0); 
}

//------------------------------------------------------------------------------------------------
int TriangleMeshGeometry::_rayCallback(dGeomID mesh,dGeomID ray,int triangle,dReal u,dReal v)
{
	TriangleMeshGeometry* trimesh = (TriangleMeshGeometry*)dGeomGetData(mesh);
	if (trimesh->_ray_listener)
	{
		RayGeometry* ray_geometry = (ray)?((RayGeometry*)dGeomGetData(ray)):0;
		return ((trimesh->_ray_listener->collide(trimesh,ray_geometry,triangle,Vector3((Real)u,(Real)v,0.0)))?1:0);
	}
	return 1;
}

//------------------------------------------------------------------------------------------------
void TriangleMeshGeometry::setRayListener(TriangleMeshRayListener* ray_listener)
{
	_ray_listener = ray_listener;
	dGeomTriMeshSetRayCallback(_geom,(_ray_listener)?TriangleMeshGeometry::_rayCallback:0); 
}

//------------------------------------------------------------------------------------------------
void TriangleMeshGeometry::createDebugObject()
{
	_debug_obj = new TriangleMeshDebugObject((_index_count / 3) * 6);
	TriangleMeshDebugObject* obj = static_cast<TriangleMeshDebugObject*>(_debug_obj);

	obj->beginDefinition();
	for(unsigned int i = 0,j = 0;i < _index_count;i+=3,j+=6)
	{
		obj->setVertex(j,Vector3((Real)_vertices[_indices[i]][0],(Real)_vertices[_indices[i]][1],(Real)_vertices[_indices[i]][2]));
		obj->setVertex(j+1,Vector3((Real)_vertices[_indices[i+1]][0],(Real)_vertices[_indices[i+1]][1],(Real)_vertices[_indices[i+1]][2]));

		obj->setVertex(j+2,Vector3((Real)_vertices[_indices[i+1]][0],(Real)_vertices[_indices[i+1]][1],(Real)_vertices[_indices[i+1]][2]));
		obj->setVertex(j+3,Vector3((Real)_vertices[_indices[i+2]][0],(Real)_vertices[_indices[i+2]][1],(Real)_vertices[_indices[i+2]][2]));

		obj->setVertex(j+4,Vector3((Real)_vertices[_indices[i+2]][0],(Real)_vertices[_indices[i+2]][1],(Real)_vertices[_indices[i+2]][2]));
		obj->setVertex(j+5,Vector3((Real)_vertices[_indices[i]][0],(Real)_vertices[_indices[i]][1],(Real)_vertices[_indices[i]][2]));
	}
	obj->endDefinition();

	Geometry::createDebugObject();
}

//------------------------------------------------------------------------------------------------
TriangleMeshGeometry::~TriangleMeshGeometry()
{
	dGeomTriMeshDataDestroy(_data); 
	delete[] _vertices;
	delete[] _indices;
}


//------------------------------------------------------------------------------------------------
ConvexGeometry::ConvexGeometry(const Ogre::Vector3* vertices,
							   unsigned int vertex_count,
							   const unsigned int* indices,
							   unsigned int index_count,
                               World *world, Space* space) : 
    Geometry(world, space),
    _vertex_count (vertex_count),
    _index_count (index_count)
{
	_vertices = new dReal[vertex_count*3];
	_indices = new unsigned int[index_count];

	for(unsigned int i = 0;i < vertex_count;i++)
	{
		_vertices[i*3 + 0] = (dReal)vertices[i].x;
		_vertices[i*3 + 1] = (dReal)vertices[i].y;
		_vertices[i*3 + 2] = (dReal)vertices[i].z;
	}

	memcpy(_indices,indices,sizeof(unsigned int) * index_count);


	_geom = dCreateConvex (getSpaceID(space),
		0,//dReal *_planes,
		0,//unsigned int _planecount,
		_vertices,
		vertex_count,
		0);//unsigned int *_polygons)

	// 	dGeomSetConvex (dGeomID g,
	// 		dReal *_planes,
	// 		unsigned int _count,
	// 		dReal *_points,
	// 		unsigned int _pointcount,unsigned int *_polygons);

	registerGeometry();
}
//------------------------------------------------------------------------------------------------
void ConvexGeometry::createDebugObject()
{
	_debug_obj = new TriangleMeshDebugObject((_index_count / 3) * 6);
	TriangleMeshDebugObject* obj = static_cast<TriangleMeshDebugObject*>(_debug_obj);

	obj->beginDefinition();
	for(unsigned int i = 0,j = 0;i < _index_count;i+=3,j+=6)
	{
		obj->setVertex(j,Vector3((Real)_vertices[_indices[i]*3 + 0],(Real)_vertices[_indices[i]*3 + 1],(Real)_vertices[_indices[i]*3 + 2]));
		obj->setVertex(j+1,Vector3((Real)_vertices[_indices[i+1]*3 + 0],(Real)_vertices[_indices[i+1]*3 + 1],(Real)_vertices[_indices[i+1]*3 + 2]));

		obj->setVertex(j+2,Vector3((Real)_vertices[_indices[i+1]*3 + 0],(Real)_vertices[_indices[i+1]*3 + 1],(Real)_vertices[_indices[i+1]*3 + 2]));
		obj->setVertex(j+3,Vector3((Real)_vertices[_indices[i+2]*3 + 0],(Real)_vertices[_indices[i+2]*3 + 1],(Real)_vertices[_indices[i+2]*3 + 2]));

		obj->setVertex(j+4,Vector3((Real)_vertices[_indices[i+2]*3 + 0],(Real)_vertices[_indices[i+2]*3 + 1],(Real)_vertices[_indices[i+2]*3 + 2]));
		obj->setVertex(j+5,Vector3((Real)_vertices[_indices[i]*3 + 0],(Real)_vertices[_indices[i]*3 + 1],(Real)_vertices[_indices[i]*3 + 2]));
	}
	obj->endDefinition();

	Geometry::createDebugObject();
}
//------------------------------------------------------------------------------------------------
ConvexGeometry::~ConvexGeometry()
{
	delete[] _vertices;
	delete[] _indices;
}

//------------------------------------------------------------------------------------------------
TerrainGeometry::TerrainGeometry(World *world, Space* space,
								const Ogre::Vector3 &scale,
                                int nodes_per_sideX,
                                int nodes_per_sideY,
                                Ogre::Real worldSizeX,
                                Ogre::Real worldSizeZ,
								bool centered,
                                Ogre::Real thickness) :
                        Geometry(world, space),
                        _sample_width(scale.x),
                        _sample_height(scale.z),
                        _max_height (scale.y),
                        _halfWorldSizeX(worldSizeX * 0.5),
                        _halfWorldSizeZ(worldSizeZ * 0.5),
                        _centered(centered)
{
	dHeightfieldDataID heightid = dGeomHeightfieldDataCreate();
	dGeomHeightfieldDataBuildCallback(	heightid, //getSpaceID(space), 
										this, // pUserData ?
										TerrainGeometry::_heightCallback, 
										(dReal) (worldSizeX), //X
										(dReal) (worldSizeZ), //Z
										nodes_per_sideX, // w // Vertex count along edge >= 2
										nodes_per_sideY, // h // Vertex count along edge >= 2
										REAL( 1.0 ),     //scale
										REAL( 0.0 ),	// vOffset
										thickness,	// vThickness
										 0); // nWrapMode


	// Give some very bounds which, while conservative,
	// makes AABB computation more accurate than +/-INF.
	dGeomHeightfieldDataSetBounds( heightid, REAL( 0.0 ),  _max_height );
	_geom = dCreateHeightfield( getSpaceID(space), heightid, 1 );

	_listener = 0;
	_ray = Ogre::Ray (Ogre::Vector3::ZERO, Ogre::Vector3::NEGATIVE_UNIT_Y);
	_ray_query = _world->getSceneManager()->createRayQuery(_ray);

	_ray_query->setQueryTypeMask(Ogre::SceneManager::WORLD_GEOMETRY_TYPE_MASK );
	_ray_query->setWorldFragmentType(Ogre::SceneQuery::WFT_SINGLE_INTERSECTION); 

	registerGeometry();

	if (_centered)
	{
		// PLSM2 is centered by default.	
        //setPosition (Ogre::Vector3::ZERO);
        ;
	}
	else
	{
        // TSM is not centered by default.	
		setPosition (Ogre::Vector3(_halfWorldSizeX, 
                                    0, 
                                    _halfWorldSizeZ));
	}
	setOrientation(Ogre::Quaternion::ZERO);
}

//------------------------------------------------------------------------------------------------
dReal TerrainGeometry::_heightCallback(void* data,int x,int z)
{
	TerrainGeometry * const terrain = (TerrainGeometry*)data;    
	if (terrain->_listener)
	{
		return static_cast <dReal> (terrain->_listener->heightAt(Vector3((Ogre::Real)x,
                                                                terrain->_max_height,
                                                                (Ogre::Real)z)));
	}
    else
    {
        return static_cast <dReal> (terrain->getHeightAt(Vector3((Ogre::Real)x, 
                                                        terrain->_max_height, 
                                                        (Ogre::Real)z)));
    }
}

//------------------------------------------------------------------------------------------------
Ogre::Real TerrainGeometry::getPointDepth(const Ogre::Vector3& point)
{
	//return (Real)dGeomTerrainCallbackPointDepth(_geom,(dReal)point.x,(dReal)point.y,(dReal)point.z); 
	//return (Real) dGeomHeightfieldPointDepth (_geom,(dReal)point.x,(dReal)point.y,(dReal)point.z);
	//dGetDepthFn(_geom,(dReal)point.x,(dReal)point.y,(dReal)point.z);
	return (Real) 0.0;
}

//------------------------------------------------------------------------------------------------
void TerrainGeometry::setHeightListener(TerrainGeometryHeightListener* listener)
{
	_listener = listener;
}
//------------------------------------------------------------------------------------------------
bool TerrainGeometry::queryResult(Ogre::MovableObject *obj, Ogre::Real distance)
{
	return false;
}

//------------------------------------------------------------------------------------------------
bool TerrainGeometry::queryResult(Ogre::SceneQuery::WorldFragment *fragment,Ogre::Real distance)
{
	_distance_to_terrain = distance;
	return false;
}
//------------------------------------------------------------------------------------------------
const Ogre::Vector3& TerrainGeometry::getPosition()
{
	return Ogre::Vector3::ZERO;
}

//------------------------------------------------------------------------------------------------
const Ogre::Quaternion& TerrainGeometry::getOrientation()
{
	return Ogre::Quaternion::ZERO;
}

//------------------------------------------------------------------------------------------------
TerrainGeometry::~TerrainGeometry()
{
	_world->getSceneManager()->destroyQuery(_ray_query);
}

//------------------------------------------------------------------------------------------------
std::list<Ogre::Plane>* PlaneBoundedRegionGeometry::_planeCallback(void* data,int x,int z)
{
    PlaneBoundedRegionGeometry* terrain = (PlaneBoundedRegionGeometry*)data;

    if (terrain->_listener)
    {
        return terrain->_listener->planesAt(Vector3((Ogre::Real)x,
            terrain->_max_height,
            (Ogre::Real)z));
    }
    else
    {
        return  terrain->planesAt(Vector3((Ogre::Real)x, 
                terrain->_max_height, 
                (Ogre::Real)z));
    }
    return 0;
}
//------------------------------------------------------------------------------------------------
PlaneBoundedRegionGeometry::PlaneBoundedRegionGeometry (World *world, Space* space,
                                                        const Ogre::AxisAlignedBox &Size) : 
    Geometry(world,space)
{

    _ray = Ogre::Ray (Ogre::Vector3::ZERO, Ogre::Vector3::NEGATIVE_UNIT_Y);
    _ray_query = _world->getSceneManager()->createRayQuery(_ray);

    _ray_query->setQueryTypeMask(Ogre::SceneManager::WORLD_GEOMETRY_TYPE_MASK );
    _ray_query->setWorldFragmentType(Ogre::SceneQuery::WFT_SINGLE_INTERSECTION); 

}

//------------------------------------------------------------------------------------------------
Ogre::Real PlaneBoundedRegionGeometry::getPointDepth(const Ogre::Vector3& point)
{
    //dGetDepthFn(_geom,(dReal)point.x,(dReal)point.y,(dReal)point.z);
    return (Real) 0.0;
}

//------------------------------------------------------------------------------------------------
void PlaneBoundedRegionGeometry::setPlaneListener(PlaneBoundedRegionGeometryPlaneListener* listener)
{
    _listener = listener;
}
//------------------------------------------------------------------------------------------------
bool PlaneBoundedRegionGeometry::queryResult(Ogre::MovableObject *obj, Ogre::Real distance)
{
    return false;
}

//------------------------------------------------------------------------------------------------
bool PlaneBoundedRegionGeometry::queryResult(Ogre::SceneQuery::WorldFragment *fragment,Ogre::Real distance)
{
    _distance_to_terrain = distance;
    return false;
}
//------------------------------------------------------------------------------------------------
const Ogre::Vector3& PlaneBoundedRegionGeometry::getPosition()
{
    return Ogre::Vector3::ZERO;
}

//------------------------------------------------------------------------------------------------
const Ogre::Quaternion& PlaneBoundedRegionGeometry::getOrientation()
{
    return Ogre::Quaternion::ZERO;
}

//------------------------------------------------------------------------------------------------
PlaneBoundedRegionGeometry::~PlaneBoundedRegionGeometry()
{
    _world->getSceneManager()->destroyQuery(_ray_query);
}


