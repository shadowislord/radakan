
#include "OgreOdeWorld.h"
#include "OgreOdeBody.h"
#include "OgreOdeJoint.h"
#include "OgreOdeGeometry.h"
#include "OgreOdeSpace.h"
#include "OgreOdeEntityInformer.h"
#include "OgreOdeMass.h"
#include "OgreOdeUtility.h"

#include "OgreOde_Prefab.h"
#include "OgreOdeRagdoll.h"

using namespace Ogre;
using namespace OgreOde;
using namespace OgreOde_Prefab;


//------------------------------------------------------------------------------------------------
Ragdoll::PhysicalBone::PhysicalBone()
{
	_body = 0;
	_geometry = 0;
	_original_position = Ogre::Vector3::ZERO;
	_original_orientation = Ogre::Quaternion::IDENTITY;
	_bone = 0;
	_parent_bone = 0;
	_joint = 0;
	_motor = 0;
}
//------------------------------------------------------------------------------------------------
Ragdoll::PhysicalBone::~PhysicalBone()
{
	delete _joint;
	delete _geometry;
	delete _body;
	delete _motor;

	for (RagdollPhysicalBoneList::iterator i = _child_bones.begin();
        i != _child_bones.end(); ++i) 
        delete (*i);
}
//------------------------------------------------------------------------------------------------
Ragdoll::BoneSettings::BoneSettings()
{
	_geometry_class		= Geometry::Class_Capsule;
	_joint_type			= Joint::Type_BallJoint;
	_primary_axis		= Ogre::Vector3::UNIT_X;
	_secondary_axis		= Ogre::Vector3::UNIT_Y;
	_collapse			= Ragdoll::BoneSettings::Collapse_None;
	_mass				= 1.0;
	_radius				= 1.0;
	_primary_histop		= OgreOde::Utility::Infinity;
	_primary_lostop		= -OgreOde::Utility::Infinity;
	_secondary_histop	= OgreOde::Utility::Infinity;
	_secondary_lostop	= -OgreOde::Utility::Infinity;
	_linear_damping     = 0.0;
	_angular_damping    = 0.0;
}
//------------------------------------------------------------------------------------------------
Ragdoll::BoneSettings::~BoneSettings()
{
}
//------------------------------------------------------------------------------------------------
Ragdoll::Ragdoll(const String& name, MeshPtr& ptr):
    Entity(name, ptr),
    Object(OgreOde_Prefab::ObjectType_Ragdoll, 0)
{ 
	setDefaultBoneSettings(Ragdoll::BoneSettings());
	_ei = 0;
	_joint_group = 0;
	_is_static = true;
	_space = 0;
	_is_physical = false;

	_node_trans = Matrix4::IDENTITY;
	_node_trans_inv = _node_trans.inverse();
	_node_orient = Ogre::Quaternion::IDENTITY;
	_node_posn = Ogre::Vector3::ZERO;
    _is_hardwareAnimationRemovedByMe = false;

    //mDisplaySkeleton = true;
}

//------------------------------------------------------------------------------------------------
void Ragdoll::sleep(Ragdoll::PhysicalBone* bone)
{
	bone->_body->sleep();
	for (RagdollPhysicalBoneList::iterator i = bone->_child_bones.begin();
		i != bone->_child_bones.end();++i)
	{
		sleep(*i);
	}
}
//------------------------------------------------------------------------------------------------
void Ragdoll::sleep()
{
	if (!_is_static)
	{
		for (RagdollPhysicalBoneList::iterator i = _root_bones.begin();
			i != _root_bones.end();++i)
		{
			sleep(*i);
		}
	}
}
//------------------------------------------------------------------------------------------------
bool Ragdoll::isAwake(Ragdoll::PhysicalBone* bone)
{
	if (bone->_body->isAwake())
		return true;
	for (RagdollPhysicalBoneList::iterator i = bone->_child_bones.begin();
		i != bone->_child_bones.end();++i)
	{
		if (isAwake(*i) == true)
			return true;
	}
	return false;
}
//------------------------------------------------------------------------------------------------
bool Ragdoll::isAwake()
{
	if (!_is_static)
	{
		for (RagdollPhysicalBoneList::iterator i = _root_bones.begin();
			i != _root_bones.end();++i)
		{
			if (isAwake(*i) == true)
				return true;
		}
	}
	return false;
}
//------------------------------------------------------------------------------------------------
void Ragdoll::wake(Ragdoll::PhysicalBone* bone)
{
	bone->_body->wake();
	for (RagdollPhysicalBoneList::iterator i = bone->_child_bones.begin();
		i != bone->_child_bones.end();++i)
	{
		wake(*i);
	}
}
//------------------------------------------------------------------------------------------------
void Ragdoll::wake()
{
	if (!_is_static)
	{
		for (RagdollPhysicalBoneList::iterator i = _root_bones.begin();
			i != _root_bones.end();++i)
		{
			wake(*i);
		}
	}
}
//------------------------------------------------------------------------------------------------
void Ragdoll::setDefaultBoneSettings(const Ragdoll::BoneSettings &settings)
{
	_default_bone_settings = settings;
}
//------------------------------------------------------------------------------------------------
void Ragdoll::setBoneSettings(const String& bone_name,const Ragdoll::BoneSettings &settings)
{
	_bone_settings.insert(std::pair<String,Ragdoll::BoneSettings>(bone_name,settings));
}
//------------------------------------------------------------------------------------------------
void Ragdoll::setSelfCollisions(bool collide)
{
	_space->setInternalCollisions(collide);
}
//------------------------------------------------------------------------------------------------
void Ragdoll::takePhysicalControl(World *world, Space* space,bool static_geometry)
{
    _world = world;
    // disable Hardware Animation
    assert (hasSkeleton()) ;
    if (isHardwareAnimationEnabled())
    {    
        const unsigned int numSubEntities  = getNumSubEntities ();
        for (unsigned int k = 0; k < numSubEntities; k++)
        {
            SubEntity *subEntity = getSubEntity (k);

            MaterialPtr mHWAnimated = MaterialManager::getSingleton().getByName(subEntity->getMaterialName ());
            // check if we didn't modify that once before
            MaterialPtr mSWAnimated = MaterialManager::getSingleton().getByName(subEntity->getMaterialName ()  + "ClonedSW");
            if (mSWAnimated.isNull ())
            {
                mSWAnimated = mHWAnimated->clone (subEntity->getMaterialName () + "ClonedSW");
                mSWAnimated->load ();

                bool notEmptiedOfHWTechnique = true;
                bool removedAtLeastOne = false;
                Technique *t;
                while (notEmptiedOfHWTechnique)
                {              
                    unsigned short i = 0  ;     
                    const unsigned short numTechniques = mSWAnimated->getNumSupportedTechniques ();
                    while (i < numTechniques)
                    {
                        t = mSWAnimated->getSupportedTechnique (i);
                        if (t->getPass(0) &&
                            t->getPass(0)->hasVertexProgram() &&
                            t->getPass(0)->getVertexProgram()->isSkeletalAnimationIncluded()
                            //t->getPass(0)->getVertexProgram()->isPoseAnimationIncluded() ??
                            )
                        {
                            mSWAnimated->removeTechnique (i);
                            removedAtLeastOne = true;
                            break;
                        }
                        i++;
                    }
                    if (i == numTechniques)
                    {
                        notEmptiedOfHWTechnique = false;
                        mSWAnimated->reload ();
                    }
                    // re-loop to ensure there is no HW animation left...                   
                }
            }
            subEntity->setMaterialName (mSWAnimated->getName());                
        }
        _is_hardwareAnimationRemovedByMe = true;
    }

	// Cache node transformations
	_node_posn = mParentNode->_getDerivedPosition();
	_node_orient = mParentNode->_getDerivedOrientation();
	_node_trans = mParentNode->_getFullTransform();
	_node_trans_inv = _node_trans.inverse();

	_space = new SimpleSpace(world, space);
	_space->setInternalCollisions(false);
	_space->setAutoCleanup(false);

	_is_physical = true;
	_is_static = static_geometry;

	_joint_group = new JointGroup(_world);

	_ei = new EntityInformer(this, _node_trans);


    addSoftwareAnimationRequest(false);

    Skeleton::BoneIterator rbi = mSkeletonInstance->getRootBoneIterator();

	while(rbi.hasMoreElements())
	{
		createBoneBody(rbi.getNext(), 0, static_geometry);
	}

}
//------------------------------------------------------------------------------------------------
void Ragdoll::createBoneBody(Bone *bone,Ragdoll::PhysicalBone *parent,bool static_geometry)
{
	// We're controlling the bone now
	bone->setManuallyControlled (true);
    //bone->setInheritOrientation (false);

	// Make up a name
	String body_name = mName + bone->getName();

	// Cache bone transformations
	Matrix4 bone_trans = bone->_getFullTransform();
	Vector3 bone_posn = bone->_getDerivedPosition();
	Quaternion bone_orient = bone->_getDerivedOrientation();

	// Find the settings for this bone, or use defaults
	std::map<String,Ragdoll::BoneSettings>::iterator i = _bone_settings.find(bone->getName());
	Ragdoll::BoneSettings settings = (i != _bone_settings.end())? i->second : _default_bone_settings;	
	Ragdoll::PhysicalBone *physical_bone = 0;

	// Create an oriented capsule
	if (settings._geometry_class == Geometry::Class_Capsule)
	{
		// Create the geometry
		CapsuleGeometry *geom = _ei->createOrientedCapsule(bone->getHandle(), _world, _space);
		if (geom)
		{
			// Create a bone
			physical_bone = new Ragdoll::PhysicalBone();
			physical_bone->_body = (static_geometry)?0:(new Body(_world, body_name));
			physical_bone->_geometry = geom;

			// Don't collapse, set capsule inertia tensor
			if (settings._collapse == Ragdoll::BoneSettings::Collapse_None)
			{
				if (physical_bone->_body) 
                    physical_bone->_body->setMass(
                            CapsuleMass(settings._mass,
                                        geom->getRadius(),
                                        Ogre::Vector3::UNIT_Z,
                                        geom->getLength()));
			}
			else
			{
				// Get the details of the capsule we created and work out where it is in relation to the bone (joint)
				const Ogre::Real radius = geom->getRadius();
				const Ogre::Real length = geom->getLength();
				const Ogre::Quaternion orient = geom->getOrientation(); 
				
				const Ogre::Vector3 jpos = _node_trans * bone_posn;
				const Ogre::Vector3 posn = geom->getPosition();
				const Ogre::Vector3 offs = jpos - posn;

				// Don't need the capsule geometry anymore, create a new sphere one
				delete geom;
                SphereGeometry *newGeom = new SphereGeometry(radius, _world, _space);
				physical_bone->_geometry = newGeom;
				if (physical_bone->_body) 
                    physical_bone->_body->setMass(SphereMass(settings._mass,radius));

				// Move the geometry up away from the joint, or down to the joint
				if (settings._collapse == Ragdoll::BoneSettings::Collapse_Up)
				{
					newGeom->setPosition(posn - offs);
					newGeom->setOrientation(orient);
				}
				else
				{
					newGeom->setPosition(posn + offs);
					newGeom->setOrientation(orient);
				}
			}
		}
	}
	// Create an oriented box
	else if (settings._geometry_class == Geometry::Class_Box)
	{
		BoxGeometry *geom = _ei->createOrientedBox(bone->getHandle(), _world, _space);
		if (geom)
		{
			physical_bone = new Ragdoll::PhysicalBone();
			physical_bone->_body = (static_geometry)?0:(new Body(_world, body_name));
			physical_bone->_geometry = geom;

			if (physical_bone->_body) 
                physical_bone->_body->setMass(
                        BoxMass(settings._mass, geom->getSize()));
		}
	}
	// Don't create any geometry
	else if (settings._geometry_class == Geometry::Class_NoGeometry)
	{
		physical_bone = new Ragdoll::PhysicalBone();
		physical_bone->_body = (static_geometry)?0:(new Body(_world, body_name));

		if (physical_bone->_body)
		{
			physical_bone->_body->setMass(SphereMass(settings._mass,settings._radius));
			physical_bone->_body->setPosition(_node_trans * bone_posn);
		}
	}
	
	if (physical_bone)
    {
        // set what does control the bone
        physical_bone->_parent_bone = parent;
        physical_bone->_bone = bone;

		// Sync the body and geometry
		if ((physical_bone->_body) && (physical_bone->_geometry))
		{
			physical_bone->_body->setPosition(physical_bone->_geometry->getPosition());
			physical_bone->_body->setOrientation(physical_bone->_geometry->getOrientation());
		}

		if (physical_bone->_geometry)
		{
			physical_bone->_geometry->setBody(physical_bone->_body);
			physical_bone->_geometry->setUserObject(this);
		}

		if (physical_bone->_body)
		{
			physical_bone->_original_orientation = bone_orient.UnitInverse() * (_node_orient.UnitInverse() * physical_bone->_body->getOrientation());
			physical_bone->_original_position = physical_bone->_body->getPointBodyPosition(_node_trans * bone_posn);

			physical_bone->_body->setDamping(settings._linear_damping,settings._angular_damping);
		}

        // update and record in parent
		if ((physical_bone->_parent_bone) 
            && (physical_bone->_parent_bone->_body) 
            && (physical_bone->_body))
		{
			if (settings._joint_type == Joint::Type_BallJoint)
			{
				physical_bone->_joint = new BallJoint(_world, _joint_group);
				physical_bone->_joint->attach(physical_bone->_parent_bone->_body,physical_bone->_body);
				physical_bone->_joint->setAnchor(_node_trans * bone_posn);

				physical_bone->_motor = new AngularMotorJoint(_world, _joint_group);
				physical_bone->_motor->attach(physical_bone->_parent_bone->_body,physical_bone->_body);
				physical_bone->_motor->setMode(AngularMotorJoint::Mode_UserAngularMotor);
				physical_bone->_motor->setAnchor(_node_trans * bone_posn);
				physical_bone->_motor->setAxisCount(2);
				
				physical_bone->_motor->setAxis(1,AngularMotorJoint::RelativeOrientation_FirstBody,(_node_orient * physical_bone->_parent_bone->_bone->_getDerivedOrientation()) * settings._primary_axis);
				physical_bone->_motor->setAngle(1,0);
				physical_bone->_motor->setParameter(Joint::Parameter_LowStop,settings._primary_lostop,1);
				physical_bone->_motor->setParameter(Joint::Parameter_HighStop,settings._primary_histop,1);

				physical_bone->_motor->setAxis(2,AngularMotorJoint::RelativeOrientation_FirstBody,(_node_orient * physical_bone->_parent_bone->_bone->_getDerivedOrientation()) * settings._secondary_axis);
				physical_bone->_motor->setAngle(2,0);
				physical_bone->_motor->setParameter(Joint::Parameter_LowStop,settings._secondary_lostop,2);
				physical_bone->_motor->setParameter(Joint::Parameter_HighStop,settings._secondary_histop,2);
			}
			else if (settings._joint_type == Joint::Type_HingeJoint)
			{
				physical_bone->_joint = new HingeJoint(_world, _joint_group);
				physical_bone->_joint->attach(physical_bone->_parent_bone->_body,physical_bone->_body);
				physical_bone->_joint->setAnchor(_node_trans * bone_posn);
				physical_bone->_joint->setAxis((_node_orient * physical_bone->_parent_bone->_bone->_getDerivedOrientation()) * settings._primary_axis);

				physical_bone->_joint->setParameter(Joint::Parameter_LowStop,settings._primary_lostop);
				physical_bone->_joint->setParameter(Joint::Parameter_HighStop,settings._primary_histop);
			}
			else if (settings._joint_type == Joint::Type_UniversalJoint)
			{
				physical_bone->_joint = new UniversalJoint(_world, _joint_group);
				physical_bone->_joint->attach(physical_bone->_parent_bone->_body,physical_bone->_body);
				physical_bone->_joint->setAnchor(_node_trans * bone_posn);
				physical_bone->_joint->setAxis((_node_orient * physical_bone->_parent_bone->_bone->_getDerivedOrientation()) * settings._primary_axis);
				physical_bone->_joint->setAdditionalAxis((_node_orient * physical_bone->_parent_bone->_bone->_getDerivedOrientation()) * settings._secondary_axis);

				physical_bone->_joint->setParameter(Joint::Parameter_LowStop,settings._primary_lostop);
				physical_bone->_joint->setParameter(Joint::Parameter_HighStop,settings._primary_histop);

				physical_bone->_joint->setParameter(Joint::Parameter_LowStop,settings._secondary_lostop,2);
				physical_bone->_joint->setParameter(Joint::Parameter_HighStop,settings._secondary_histop,2);
			}
			else if (settings._joint_type == Joint::Type_FixedJoint)
			{
				physical_bone->_joint = new FixedJoint(_world, _joint_group);
				physical_bone->_joint->attach(physical_bone->_parent_bone->_body,physical_bone->_body);
				physical_bone->_joint->setAnchor(_node_trans * bone_posn);
			}

			physical_bone->_parent_bone->_child_bones.push_back(physical_bone);
		}
		else 
        {
            _root_bones.push_back(physical_bone);
        }
	}

	Node::ChildNodeIterator cni = bone->getChildIterator();
	while(cni.hasMoreElements())
	{
		createBoneBody((Bone*)cni.getNext(),(physical_bone)?physical_bone:parent,static_geometry);
	}
}
//------------------------------------------------------------------------------------------------
void Ragdoll::turnToStone(Ragdoll::PhysicalBone* bone)
{
	bone->_geometry->setBody(0);

	delete bone->_motor;
	delete bone->_joint;
	delete bone->_body;

	bone->_motor = 0;
	bone->_joint = 0;
	bone->_body = 0;

	for (RagdollPhysicalBoneList::iterator i = bone->_child_bones.begin();i != bone->_child_bones.end();++i)
	{
		turnToStone(*i);
	}
}
//------------------------------------------------------------------------------------------------
void Ragdoll::turnToStone()
{
	if (!_is_static)
	{
		for (RagdollPhysicalBoneList::iterator i = _root_bones.begin();
            i != _root_bones.end();++i)
		{
			turnToStone(*i);
		}

		delete _joint_group;
		_joint_group = 0;

		_is_static = true;
	}
}
//------------------------------------------------------------------------------------------------
void Ragdoll::releasePhysicalControl(Ragdoll::PhysicalBone* bone)
{
	bone->_bone->setManuallyControlled(false);

	delete bone->_geometry;
	bone->_geometry = 0;

	for (RagdollPhysicalBoneList::iterator i = bone->_child_bones.begin();i != bone->_child_bones.end();++i)
	{
		releasePhysicalControl(*i);
	}
}
//------------------------------------------------------------------------------------------------
void Ragdoll::releasePhysicalControl()
{
	turnToStone();
    
	if (_is_physical)
    {
       // re-enable hardware Animation if previously disabled
        if (_is_hardwareAnimationRemovedByMe) 
        {
            const unsigned int numSubEntities  = getNumSubEntities ();
            for (unsigned int k = 0; k < numSubEntities; k++)
            {
                SubEntity *subEntity = getSubEntity (k);
                const String hwMatName (subEntity->getMaterialName ());
                MaterialPtr mSWAnimated = MaterialManager::getSingleton().getByName(hwMatName);
                assert (std::string::npos != hwMatName.find("ClonedSW"));
                const String swMatName(hwMatName, 0, hwMatName.find("ClonedSW"));
                MaterialPtr mHWAnimated = MaterialManager::getSingleton().getByName(swMatName);
                assert (!mHWAnimated.isNull ());
                
                subEntity->setMaterialName (mHWAnimated->getName());                
            }
            _is_hardwareAnimationRemovedByMe = false;
        }

		for (RagdollPhysicalBoneList::iterator i = _root_bones.begin();i != _root_bones.end();++i)
		{
			releasePhysicalControl(*i);
			delete (*i);
		}
		_root_bones.clear();

		delete _space;
		_space = 0;

        _is_physical = false;

        removeSoftwareAnimationRequest(false);
	}
}
//------------------------------------------------------------------------------------------------
void Ragdoll::updatePhysicalBone(Ragdoll::PhysicalBone *physical_bone,const Matrix4 &this_trans,const Matrix4 &base_trans)
{

    // USE OF BASE_TRANS ???
	Quaternion body_orient = physical_bone->_body->getOrientation() * physical_bone->_original_orientation.UnitInverse();
	Quaternion bone_orient = Ogre::Quaternion::IDENTITY;

	Vector3 body_posn = physical_bone->_body->getPointWorldPosition(physical_bone->_original_position);
	Vector3 bone_posn = Ogre::Vector3::ZERO;

	Matrix3 bone_rot;
	body_orient.ToRotationMatrix(bone_rot);

	Matrix4 bone_trans = Matrix4::IDENTITY;
	bone_trans = bone_rot;
	bone_trans.setTrans(body_posn);

	bone_trans = this_trans.inverse() * bone_trans;

	bone_posn = Ogre::Vector3(bone_trans[0][3], bone_trans[1][3], bone_trans[2][3]);
	bone_trans.extract3x3Matrix(bone_rot);
	bone_orient.FromRotationMatrix(bone_rot);

	physical_bone->_bone->setOrientation(bone_orient);
	physical_bone->_bone->setPosition(bone_posn);

    //physical_bone->_bone->needUpdate (true);

	for (RagdollPhysicalBoneList::iterator i = physical_bone->_child_bones.begin();
        i != physical_bone->_child_bones.end();++i)
	{
		updatePhysicalBone(*i,
            base_trans * (*i)->_bone->getParent()->_getFullTransform(),
            base_trans);
	}
}
//------------------------------------------------------------------------------------------------
void Ragdoll::update()
{
	if ((_is_physical) && (!_is_static))
	{		
		for (RagdollPhysicalBoneList::iterator i = _root_bones.begin();
            i != _root_bones.end();++i)
		{
			updatePhysicalBone(*i, _node_trans, _node_trans);
		}
		mParentNode->needUpdate();        
	}
}
//------------------------------------------------------------------------------------------------
const AxisAlignedBox& Ragdoll::getBoundingBox(void) const
{
	if (_space)
	{
		// Get from space and convert to local 
		mFullBoundingBox = _space->getAxisAlignedBox();
		mFullBoundingBox.transform(_node_trans_inv);
	}
	else
	{
		// Get from Mesh
		mFullBoundingBox = mMesh->getBounds();
		mFullBoundingBox.merge(getChildObjectsBoundingBox());
	}
	return mFullBoundingBox;
}
//------------------------------------------------------------------------------------------------
bool Ragdoll::collision(Contact* contact)
{
	Body *body = contact->getFirstGeometry()->getBody();
	if (!body) 
        body = contact->getSecondGeometry()->getBody();
	_hit_list.push_back(std::pair<Body*,Vector3>(body,contact->getPosition()));
	return false;
}
//------------------------------------------------------------------------------------------------
void Ragdoll::pick(Ragdoll::PhysicalBone *bone,RayGeometry *ray)
{
	if (bone->_geometry) 
        ray->collide(bone->_geometry,this);
	
	for (RagdollPhysicalBoneList::iterator i = bone->_child_bones.begin();
        i != bone->_child_bones.end();++i)
	{
		pick(*i,ray);
	}
}
//------------------------------------------------------------------------------------------------
bool Ragdoll::pick(RayGeometry *ray,Body* &body,Vector3 &position)
{
	_hit_list.clear();
	for (RagdollPhysicalBoneList::iterator i = _root_bones.begin();
        i != _root_bones.end();++i)
	{
		pick(*i, ray);
	}

	const bool rc = (_hit_list.empty())? false : true;

	if (rc)
	{
	    const Ogre::Real dist = ray->getLength();
	    for (std::vector<std::pair<Body*,Vector3> >::iterator i = _hit_list.begin();
            i != _hit_list.end();++i)
	    {
		    const Ogre::Real this_dist = (i->second - ray->getPosition()).length();
		    if (this_dist < dist)
		    {
			    body = i->first;
			    position = i->second;
		    }
	    }
	}

	return rc;
}
//------------------------------------------------------------------------------------------------
Ragdoll::~Ragdoll()
{
    releasePhysicalControl ();
	for (RagdollPhysicalBoneList::iterator i = _root_bones.begin();i != _root_bones.end();++i) 
        delete (*i);
	delete _space;
	delete _ei;
	delete _joint_group;
}

//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
String RagdollFactory::FACTORY_TYPE_NAME = "ODERagdoll";
//-----------------------------------------------------------------------
const String& RagdollFactory::getType(void) const
{
	return FACTORY_TYPE_NAME;
}
//-----------------------------------------------------------------------
MovableObject* RagdollFactory::createInstanceImpl( const String& name,
												  const NameValuePairList* params)
{
	// must have mesh parameter
	MeshPtr pMesh;
	if (params != 0)
	{
		NameValuePairList::const_iterator ni = params->find("mesh");
		if (ni != params->end())
		{
			// Get mesh (load if required)
			pMesh = MeshManager::getSingleton().load(
				ni->second,
				// note that you can change the group by pre-loading the mesh
				ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME );
		}
	}
	if (pMesh.isNull())
	{
		OGRE_EXCEPT(Exception::ERR_INVALIDPARAMS,
			"'mesh' parameter required when constructing an Ragdoll.",
			"RagdollFactory::createInstance");
	}
	return new Ragdoll(name, pMesh);

}
//-----------------------------------------------------------------------
void RagdollFactory::destroyInstance( MovableObject* obj)
{
	delete obj;
} 