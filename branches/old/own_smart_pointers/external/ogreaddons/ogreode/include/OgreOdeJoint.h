#ifndef _OGREODEJOINT_H_
#define _OGREODEJOINT_H_

#include "OgreOdePreReqs.h"
#include "OgreOdeMaintainedList.h"

namespace OgreOde 
{
	class _OgreOdeExport JointGroup
	{
		friend class World;
		friend class Joint;

	public:
        JointGroup(World *world);
		virtual ~JointGroup();

		void empty();

		virtual unsigned long getID();

	protected:
		dJointGroupID getJointGroupID() const;

	protected:
        dJointGroupID _joint_group;
        World *_world;
	};

	class _OgreOdeExport Joint
    {
		friend class World;

	public:
		enum Type
		{
			Type_BallJoint					= dJointTypeBall,
			Type_HingeJoint					= dJointTypeHinge,
			Type_SliderJoint				= dJointTypeSlider,
			Type_UniversalJoint				= dJointTypeUniversal,
			Type_Suspension_Joint			= dJointTypeHinge2,
			Type_FixedJoint					= dJointTypeFixed,
			Type_AngularMotorJoint			= dJointTypeAMotor,
			Type_PlanarJoint				= dJointTypePlane2D
		};

		enum Parameter
		{
			Parameter_LowStop				= dParamLoStop,
			Parameter_HighStop				= dParamHiStop,
			Parameter_MotorVelocity			= dParamVel,
			Parameter_MaximumForce			= dParamFMax,
			Parameter_FudgeFactor			= dParamFudgeFactor,
			Parameter_Bounceyness			= dParamBounce,
			Parameter_CFM					= dParamCFM,
			Parameter_StopERP				= dParamStopERP,
			Parameter_StopCFM				= dParamStopCFM,
			Parameter_SuspensionERP			= dParamSuspensionERP,
			Parameter_SuspensionCFM			= dParamSuspensionCFM 
		};

	public:
		Joint(World *world, const JointGroup* group = 0);
		virtual ~Joint();

		Joint::Type getType(); 

		void registerJoint();

		void detach();
		virtual void attach(const Body* body);
		virtual void attach(const Body* body_a,const Body* body_b); 

		Body* getFirstBody(); 
		Body* getSecondBody(); 
		
		static bool areConnected(const Body* body_a,const Body* body_b); 
		static bool areConnectedExcluding(const Body* body_a,const Body* body_b,Joint::Type joint_type); 
	
		void enableFeedback(); 
		void disableFeedback(); 
		const Ogre::Vector3& getFirstForce();
		const Ogre::Vector3& getFirstTorque();
		const Ogre::Vector3& getSecondForce();
		const Ogre::Vector3& getSecondTorque();

		virtual void setAnchor(const Ogre::Vector3& position);
		virtual const Ogre::Vector3& getAnchor();
		virtual const Ogre::Vector3& getAnchorError();
		virtual void setAxis(const Ogre::Vector3& axis);
		virtual const Ogre::Vector3& getAxis();
		virtual void setAdditionalAxis(const Ogre::Vector3& axis);
		virtual const Ogre::Vector3& getAdditionalAxis();
		virtual Ogre::Real getAngle();
		virtual Ogre::Real getAngleRate();
		virtual Ogre::Real getPosition();
		virtual Ogre::Real getPositionRate();

		virtual void setParameter(Joint::Parameter parameter,Ogre::Real value,int axis = 1);
		virtual Ogre::Real getParameter(Joint::Parameter parameter,int axis = 1);

		virtual unsigned long getID();

		virtual void addTorque(Ogre::Real torque, Ogre::Real torque2 = 0.0, Ogre::Real torque3 = 0.0);
		virtual void addForce(Ogre::Real force, Ogre::Real force2 = 0.0, Ogre::Real force3 = 0.0);

	protected:
		dJointID getJointID();

		dWorldID getWorldID();
		dJointGroupID getJointGroupID(const JointGroup* group) const;

	protected:
		dJointID _joint;
		dJointFeedback _feedback;
		Ogre::Vector3 _axis,_anchor,_anchor_error,_additional_axis;
        Ogre::Vector3 _first_force,_first_torque,_second_force,_second_torque;
        World *_world;
	};

	class _OgreOdeExport BallJoint:public Joint
	{
	public:
		BallJoint(World *world, const JointGroup* group = 0);
		~BallJoint();

		virtual void setAnchor(const Ogre::Vector3& position);
		virtual const Ogre::Vector3& getAnchor();
		virtual const Ogre::Vector3& getAnchorError();
	};

	class _OgreOdeExport HingeJoint:public Joint
	{
	public:
		HingeJoint(World *world, const JointGroup* group = 0);
		~HingeJoint();

		virtual void setAnchor(const Ogre::Vector3& position);
		virtual const Ogre::Vector3& getAnchor();
		virtual const Ogre::Vector3& getAnchorError();
		virtual void setAxis(const Ogre::Vector3& axis);
		virtual const Ogre::Vector3& getAxis();
		virtual Ogre::Real getAngle();
		virtual Ogre::Real getAngleRate();

		virtual void setParameter(Joint::Parameter parameter, Ogre::Real value,int axis = 1);
		virtual Ogre::Real getParameter(Joint::Parameter parameter,int axis = 1);

		virtual void addTorque(Ogre::Real torque, Ogre::Real torque2 = 0.0, Ogre::Real torque3 = 0.0);
	};

	class _OgreOdeExport SliderJoint:public Joint
	{
	public:
		SliderJoint(World *world, const JointGroup* group = 0);
		~SliderJoint();

		virtual void setAxis(const Ogre::Vector3& axis);
		virtual const Ogre::Vector3& getAxis();
		virtual Ogre::Real getPosition();
		virtual Ogre::Real getPositionRate();

		virtual void setParameter(Joint::Parameter parameter, Ogre::Real value,int axis = 1);
		virtual Ogre::Real getParameter(Joint::Parameter parameter, int axis = 1);

		virtual void addForce(Ogre::Real force, Ogre::Real force2 = 0.0, Ogre::Real force3 = 0.0);
	};

	class _OgreOdeExport UniversalJoint:public Joint
	{
	public:
		UniversalJoint(World *world, const JointGroup* group = 0);
		~UniversalJoint();

		virtual void setAnchor(const Ogre::Vector3& position);
		virtual const Ogre::Vector3& getAnchor();
		virtual const Ogre::Vector3& getAnchorError();
		virtual void setAxis(const Ogre::Vector3& axis);
		virtual const Ogre::Vector3& getAxis();
		virtual void setAdditionalAxis(const Ogre::Vector3& axis);
		virtual const Ogre::Vector3& getAdditionalAxis();

		virtual void setParameter(Joint::Parameter parameter, Ogre::Real value, int axis = 1);
		virtual Ogre::Real getParameter(Joint::Parameter parameter,int axis = 1);

		virtual void addTorque(Ogre::Real torque, Ogre::Real torque2 = 0.0, Ogre::Real torque3 = 0.0);
	};

	class _OgreOdeExport FixedJoint:public Joint
	{
	public:
		FixedJoint(World *world, const JointGroup* group = 0);
		~FixedJoint();

		virtual void attach(const Body* body);
		virtual void attach(const Body* body_a,const Body* body_b); 
	};

	class _OgreOdeExport SuspensionJoint:public Joint
	{
	public:
		SuspensionJoint(World *world, const JointGroup* group = 0);
		~SuspensionJoint();

		virtual void setAnchor(const Ogre::Vector3& position);
		virtual const Ogre::Vector3& getAnchor();
		virtual const Ogre::Vector3& getAdditionalAnchor();
		virtual const Ogre::Vector3& getAnchorError();
		virtual void setAxis(const Ogre::Vector3& axis);
		virtual const Ogre::Vector3& getAxis();
		virtual void setAdditionalAxis(const Ogre::Vector3& axis);
		virtual const Ogre::Vector3& getAdditionalAxis();
		virtual Ogre::Real getAngle();
		virtual Ogre::Real getAngleRate();
		virtual Ogre::Real getPositionRate();

		virtual void setParameter(Joint::Parameter parameter, Ogre::Real value,int axis = 1);
		virtual Ogre::Real getParameter(Joint::Parameter parameter,int axis = 1);

		virtual void addTorque(Ogre::Real torque, Ogre::Real torque2 = 0.0, Ogre::Real torque3 = 0.0);

	protected:
		Ogre::Vector3 _anchor2;
	};

	class _OgreOdeExport AngularMotorJoint:public Joint
	{
	public:
		enum Mode
		{
			Mode_UserAngularMotor	= dAMotorUser,
			Mode_EulerAngularMotor	= dAMotorEuler 
		};

		enum RelativeOrientation
		{
			RelativeOrientation_GlobalFrame = 0,
			RelativeOrientation_FirstBody	= 1,		
			RelativeOrientation_SecondBody	= 2
		};

	public:
		AngularMotorJoint(World *world, const JointGroup* group = 0);
		~AngularMotorJoint();

		void setMode(AngularMotorJoint::Mode mode);
		AngularMotorJoint::Mode getMode(); 

		void setAxisCount(int axes);
		int getAxisCount(); 

		void setAxis(int axis_number,AngularMotorJoint::RelativeOrientation orientation,const Ogre::Vector3& axis);
		const Ogre::Vector3& getAxis(int axis_number);
		AngularMotorJoint::RelativeOrientation getAxisRelativeOrientation(int axis_number); 

		void setAngle(int axis, Ogre::Real angle); 
		Ogre::Real getAngle(int axis); 
		Ogre::Real getAngleRate(int axis); 

		virtual void setParameter(Joint::Parameter parameter, Ogre::Real value,int axis = 1);
		virtual Ogre::Real getParameter(Joint::Parameter parameter,int axis = 1);

		virtual void addTorque(Ogre::Real torque, Ogre::Real torque2 = 0.0, Ogre::Real torque3 = 0.0);
	};

	class _OgreOdeExport PlanarJoint:public Joint
	{
	public:
		PlanarJoint(World *world, const JointGroup* group = 0);
		~PlanarJoint();

		virtual void setParameterX(Joint::Parameter parameter, Ogre::Real value,int axis = 1);
		virtual void setParameterY(Joint::Parameter parameter, Ogre::Real value,int axis = 1);
		virtual void setParameterAngle(Joint::Parameter parameter, Ogre::Real value,int axis = 1);
	};
}

#endif 
