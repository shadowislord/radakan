#ifndef _OGREODERAGDOLL_H_
#define _OGREODERAGDOLL_H_

#include "OgreOde_Prefab.h"

#include <map>

namespace OgreOde_Prefab 
{

    class _OgreOdeExport_Prefab Ragdoll : public Object, public Ogre::Entity, public OgreOde::CollisionListener
    {

        friend class RagdollFactory;

    public:
        class PhysicalBone;
        typedef std::vector<Ragdoll::PhysicalBone*> RagdollPhysicalBoneList;

    public:
		class _OgreOdeExport_Prefab PhysicalBone 
		{
		public:
			PhysicalBone();
			~PhysicalBone();

		public:
			OgreOde::Body								*_body;
			OgreOde::Geometry							*_geometry;
			Ogre::Vector3						_original_position;
			Ogre::Quaternion					_original_orientation;
			Ogre::Bone							*_bone;
			PhysicalBone						*_parent_bone;
			RagdollPhysicalBoneList	            _child_bones;
			OgreOde::Joint								*_joint;
			OgreOde::AngularMotorJoint					*_motor;
		};

		class _OgreOdeExport_Prefab BoneSettings
		{
		public:
			enum Collapse
			{
				Collapse_None,
				Collapse_Up,
				Collapse_Down
			};

		public:
			BoneSettings();
			~BoneSettings();

		public:
			OgreOde::Geometry::Class		_geometry_class;
			OgreOde::Joint::Type			_joint_type;
			Ogre::Vector3		_primary_axis;
			Ogre::Vector3		_secondary_axis;
			Collapse			_collapse;
			Ogre::Real			_mass;
			Ogre::Real			_radius;
			Ogre::Real			_primary_histop,_primary_lostop,_secondary_histop,_secondary_lostop;
			Ogre::Real			_linear_damping,_angular_damping;
		};

	public:
		void setDefaultBoneSettings(const Ragdoll::BoneSettings &settings);
		const Ragdoll::BoneSettings &getDefaultBoneSettings(){return _default_bone_settings;}
		
		void setBoneSettings(const Ogre::String& bone_name,const Ragdoll::BoneSettings &settings);
		void setSelfCollisions(bool collide);

        void takePhysicalControl(OgreOde::World* world, OgreOde::Space* space, bool static_geometry = false);
		void turnToStone();

		void sleep();
		void sleep(Ragdoll::PhysicalBone* bone);

		void wake();
		void wake(Ragdoll::PhysicalBone* bone);

		bool isAwake();
		bool isAwake(Ragdoll::PhysicalBone* bone);

		void releasePhysicalControl();

		bool isPhysical()const {return _is_physical;}
		bool isStatic()const {return _is_static;}

		~Ragdoll();

		void update();

        virtual const Ogre::AxisAlignedBox& getBoundingBox(void) const;

		bool pick(OgreOde::RayGeometry *ray, OgreOde::Body* &body, Ogre::Vector3 &position);
		bool collision(OgreOde::Contact* contact);


	protected:

		Ragdoll(const Ogre::String& name, Ogre::MeshPtr& ptr); 

		void createBoneBody(Ogre::Bone *bone, Ragdoll::PhysicalBone *parent, bool static_geometry);
		void updatePhysicalBone(Ragdoll::PhysicalBone *physical_bone, const Ogre::Matrix4 &this_trans, const Ogre::Matrix4 &base_trans);

		void turnToStone(Ragdoll::PhysicalBone* bone);
		void releasePhysicalControl(Ragdoll::PhysicalBone* bone);

		void pick(Ragdoll::PhysicalBone *bone, OgreOde::RayGeometry *ray);

		RagdollPhysicalBoneList _root_bones;
		Ragdoll::BoneSettings _default_bone_settings;
		std::map<Ogre::String, Ragdoll::BoneSettings> _bone_settings;
		Ogre::Matrix4 _node_trans,_node_trans_inv;
		Ogre::Quaternion _node_orient;
		Ogre::Vector3 _node_posn;

		std::vector<std::pair<OgreOde::Body*,Ogre::Vector3> > _hit_list;

		OgreOde::EntityInformer *_ei;
		OgreOde::SimpleSpace    *_space;
        OgreOde::JointGroup     *_joint_group;

		bool    _is_static;
		bool    _is_physical;
        bool    _is_hardwareAnimationRemovedByMe;

	};

	/** Factory object for creating Ragdoll instances */
	class _OgreOdeExport_Prefab RagdollFactory : public Ogre::MovableObjectFactory
	{
	protected:
		Ogre::MovableObject* createInstanceImpl( const Ogre::String& name, const Ogre::NameValuePairList* params);
	public:
		RagdollFactory() {}
		~RagdollFactory() {}

		static Ogre::String FACTORY_TYPE_NAME;

		const Ogre::String& getType(void) const;
		void destroyInstance( Ogre::MovableObject* obj); 

	}; 
}

#endif
