
#include "OgreOdePrecompiledHeaders.h"

#include "OgreOdeGeometry.h"
#include "OgreOdeWorld.h"
#include "OgreOdeBody.h"
#include "OgreOdeGeometry.h"
#include "OgreOdeJoint.h"

using namespace OgreOde;
using namespace Ogre;

//------------------------------------------------------------------------------------------------
JointGroup::JointGroup(World *world):
_world(world)
{
	_joint_group = dJointGroupCreate(0); 
	_world->getJointGroupList().registerItem(this);
}
//------------------------------------------------------------------------------------------------
dJointGroupID JointGroup::getJointGroupID() const
{
	return _joint_group;
}

unsigned long JointGroup::getID()
{
	return (unsigned long)_joint_group;
}

void JointGroup::empty()
{
	dJointGroupEmpty(_joint_group); 
}

JointGroup::~JointGroup()
{
	_world->getJointGroupList().unregisterItem((long unsigned int)_joint_group);
	dJointGroupDestroy(_joint_group); 
}

Joint::Joint(World *world, const JointGroup* group):
    _world(world)
{
}

void Joint::registerJoint()
{
	_world->getJointList().registerItem(this);
}

void Joint::setAnchor(const Ogre::Vector3& position)
{
}

void Joint::addTorque(Real torque,Real torque2,Real torque3)
{
}

void Joint::addForce(Real force,Real force2,Real force3)
{
}

const Ogre::Vector3& Joint::getAnchor()
{
	return Ogre::Vector3::ZERO;
}

const Ogre::Vector3& Joint::getAnchorError()
{
	return Ogre::Vector3::ZERO;
}

void Joint::setAxis(const Ogre::Vector3& axis)
{
}

const Ogre::Vector3& Joint::getAxis()
{
	return Ogre::Vector3::ZERO;
}

void Joint::setAdditionalAxis(const Ogre::Vector3& axis)
{
}

const Ogre::Vector3& Joint::getAdditionalAxis()
{
	return Ogre::Vector3::ZERO;
}

Real Joint::getAngle()
{
	return 0.0;
}

Real Joint::getAngleRate()
{
	return 0.0;
}

Real Joint::getPosition()
{
	return 0.0;
}

Real Joint::getPositionRate()
{
	return 0.0;
}

unsigned long Joint::getID()
{
	return (unsigned long)_joint;
}

Joint::~Joint()
{
	_world->getJointList().unregisterItem((unsigned long)_joint);
	dJointDestroy(_joint); 
}

dWorldID Joint::getWorldID()
{
	return _world->getWorldID();
}

dJointGroupID Joint::getJointGroupID(const JointGroup* group) const
{
	return ((group)?group->getJointGroupID():0);
}

dJointID Joint::getJointID()
{
	return _joint;
}

Joint::Type Joint::getType()
{
	return (Joint::Type)(dJointGetType(_joint));
}

Body* Joint::getFirstBody()
{
	dBodyID b = dJointGetBody(_joint, 0); 
	return ((Body*)dBodyGetData(b));
}

Body* Joint::getSecondBody()
{
	dBodyID b = dJointGetBody(_joint,1);
	return b ? ((Body*)dBodyGetData(b)) : 0; 
}

bool Joint::areConnected(const Body* body_a,const Body* body_b)
{
	return (dAreConnected(body_a->getBodyID(),body_b->getBodyID()))?true:false; 
}

bool Joint::areConnectedExcluding(const Body* body_a,const Body* body_b,Joint::Type joint_type)
{
	return (dAreConnectedExcluding(body_a->getBodyID(),body_b->getBodyID(),(int)joint_type))?true:false; 
}

void Joint::enableFeedback()
{
	dJointSetFeedback(_joint,&_feedback); 
}

void Joint::disableFeedback()
{
	dJointSetFeedback(_joint,0); 
}

void Joint::detach()
{
	dJointAttach(_joint,0,0); 
}

void Joint::attach(const Body* body)
{
	dJointAttach(_joint,body->getBodyID(),0); 
}

void Joint::attach(const Body* body_a,const Body* body_b)
{
	dJointAttach(_joint,body_a->getBodyID(),body_b->getBodyID()); 
}

const Ogre::Vector3& Joint::getFirstForce()
{
	assert(dJointGetFeedback(_joint) == &_feedback);

	_first_force.x = _feedback.f1[0];
	_first_force.y = _feedback.f1[1];
	_first_force.z = _feedback.f1[2];
	return _first_force;
}

const Ogre::Vector3& Joint::getFirstTorque()
{
	assert(dJointGetFeedback(_joint) == &_feedback);

	_first_torque.x = _feedback.t1[0];
	_first_torque.y = _feedback.t1[1];
	_first_torque.z = _feedback.t1[2];
	return _first_torque;
}

const Ogre::Vector3& Joint::getSecondForce()
{
	assert(dJointGetFeedback(_joint) == &_feedback);

	_second_force.x = _feedback.f2[0];
	_second_force.y = _feedback.f2[1];
	_second_force.z = _feedback.f2[2];
	return _second_force;
}

const Ogre::Vector3& Joint::getSecondTorque()
{
	assert(dJointGetFeedback(_joint) == &_feedback);

	_second_torque.x = _feedback.t2[0];
	_second_torque.y = _feedback.t2[1];
	_second_torque.z = _feedback.t2[2];
	return _second_torque;
}

void Joint::setParameter(Joint::Parameter parameter,Real value,int axis)
{
}

Real Joint::getParameter(Joint::Parameter parameter,int axis)
{
	return 0.0;
}

BallJoint::BallJoint(World *world, const JointGroup* group) : Joint(world, group)
{
	_joint = dJointCreateBall(getWorldID(),getJointGroupID(group));
	registerJoint();
}

void BallJoint::setAnchor(const Ogre::Vector3& position)
{
	dJointSetBallAnchor(_joint,(dReal)position.x,(dReal)position.y,(dReal)position.z); 
}

const Ogre::Vector3& BallJoint::getAnchor()
{
	dVector3 result;
	dJointGetBallAnchor(_joint,result); 
	_anchor.x = (Real)result[0];
	_anchor.y = (Real)result[1];
	_anchor.z = (Real)result[2];
	return _anchor;
}

const Ogre::Vector3& BallJoint::getAnchorError()
{
	dVector3 result1,result2;
	dJointGetBallAnchor(_joint,result1); 
	dJointGetBallAnchor2(_joint,result2); 
	_anchor_error.x = (Real)(result1[0] - result2[2]);
	_anchor_error.y = (Real)(result1[1] - result2[2]);
	_anchor_error.z = (Real)(result1[2] - result2[2]);
	return _anchor_error;
}

BallJoint::~BallJoint()
{
}

HingeJoint::HingeJoint(World *world, const JointGroup* group) : Joint(world, group)
{
	_joint = dJointCreateHinge(getWorldID(),getJointGroupID(group)); 
	registerJoint();
}

void HingeJoint::setAnchor(const Ogre::Vector3& position)
{
	dJointSetHingeAnchor(_joint,(dReal)position.x,(dReal)position.y,(dReal)position.z); 
}

void HingeJoint::addTorque(Real torque,Real torque2,Real torque3)
{
	dJointAddHingeTorque(_joint,(dReal)torque);
}

const Ogre::Vector3& HingeJoint::getAnchor()
{
	dVector3 result;
	dJointGetHingeAnchor(_joint,result); 
	_anchor.x = (Real)result[0];
	_anchor.y = (Real)result[1];
	_anchor.z = (Real)result[2];
	return _anchor;
}

const Ogre::Vector3& HingeJoint::getAnchorError()
{
	dVector3 result1,result2;
	dJointGetHingeAnchor(_joint,result1); 
	dJointGetHingeAnchor2(_joint,result2); 
	_anchor_error.x = (Real)(result1[0] - result2[2]);
	_anchor_error.y = (Real)(result1[1] - result2[2]);
	_anchor_error.z = (Real)(result1[2] - result2[2]);
	return _anchor_error;
}

void HingeJoint::setAxis(const Ogre::Vector3& axis)
{
	dJointSetHingeAxis(_joint,(dReal)axis.x,(dReal)axis.y,(dReal)axis.z);
}

const Ogre::Vector3& HingeJoint::getAxis()
{
	dVector3 result;
	dJointGetHingeAxis(_joint,result); 
	_axis.x = (Real)result[0];
	_axis.y = (Real)result[1];
	_axis.z = (Real)result[2];
	return _axis;
}

Real HingeJoint::getAngle()
{
	return (Real)dJointGetHingeAngle(_joint); 
}

Real HingeJoint::getAngleRate()
{
	return (Real)dJointGetHingeAngleRate(_joint); 
}

void HingeJoint::setParameter(Joint::Parameter parameter,Real value,int axis)
{
	dJointSetHingeParam(_joint,((int)parameter) + dParamGroup * (axis - 1),(dReal)value); 
}

Real HingeJoint::getParameter(Joint::Parameter parameter,int axis)
{
	return (Real)dJointGetHingeParam(_joint,((int)parameter) + dParamGroup * (axis - 1));
}

HingeJoint::~HingeJoint()
{
}

SliderJoint::SliderJoint(World *world, const JointGroup* group) : Joint(world, group)
{
	_joint = dJointCreateSlider(getWorldID(),getJointGroupID(group)); 
	registerJoint();
}

void SliderJoint::setAxis(const Ogre::Vector3& axis)
{
	dJointSetSliderAxis(_joint,(dReal)axis.x,(dReal)axis.y,(dReal)axis.z);
}

const Ogre::Vector3& SliderJoint::getAxis()
{
	dVector3 result;
	dJointGetSliderAxis(_joint,result); 
	_axis.x = (Real)result[0];
	_axis.y = (Real)result[1];
	_axis.z = (Real)result[2];
	return _axis;
}

void SliderJoint::addForce(Real force,Real force2,Real force3)
{
	dJointAddSliderForce(_joint,(dReal)force);
}

Real SliderJoint::getPosition()
{
	return (Real)dJointGetSliderPosition(_joint); 
}

Real SliderJoint::getPositionRate()
{
	return (Real)dJointGetSliderPositionRate(_joint); 
}

void SliderJoint::setParameter(Joint::Parameter parameter,Real value,int axis)
{
	dJointSetSliderParam(_joint,((int)parameter) + dParamGroup * (axis - 1),(dReal)value); 
}

Real SliderJoint::getParameter(Joint::Parameter parameter,int axis)
{
	return (Real)dJointGetSliderParam(_joint,((int)parameter) + dParamGroup * (axis - 1));
}

SliderJoint::~SliderJoint()
{
}

UniversalJoint::UniversalJoint(World *world, const JointGroup* group) : Joint(world, group)
{
	_joint = dJointCreateUniversal(getWorldID(),getJointGroupID(group)); 
	registerJoint();
}

void UniversalJoint::setAnchor(const Ogre::Vector3& position)
{
	dJointSetUniversalAnchor(_joint,(dReal)position.x,(dReal)position.y,(dReal)position.z); 
}

void UniversalJoint::addTorque(Real torque,Real torque2,Real torque3)
{
	dJointAddUniversalTorques(_joint,(dReal)torque,(dReal)torque2);
}

const Ogre::Vector3& UniversalJoint::getAnchor()
{
	dVector3 result;
	dJointGetUniversalAnchor(_joint,result); 
	_anchor.x = (Real)result[0];
	_anchor.y = (Real)result[1];
	_anchor.z = (Real)result[2];
	return _anchor;
}

const Ogre::Vector3& UniversalJoint::getAnchorError()
{
	dVector3 result1,result2;
	dJointGetUniversalAnchor(_joint,result1); 
	dJointGetUniversalAnchor2(_joint,result2); 
	_anchor_error.x = (Real)(result1[0] - result2[2]);
	_anchor_error.y = (Real)(result1[1] - result2[2]);
	_anchor_error.z = (Real)(result1[2] - result2[2]);
	return _anchor_error;
}

void UniversalJoint::setAxis(const Ogre::Vector3& axis)
{
	dJointSetUniversalAxis1(_joint,(dReal)axis.x,(dReal)axis.y,(dReal)axis.z);
}

const Ogre::Vector3& UniversalJoint::getAxis()
{
	dVector3 result;
	dJointGetUniversalAxis1(_joint,result); 
	_axis.x = (Real)result[0];
	_axis.y = (Real)result[1];
	_axis.z = (Real)result[2];
	return _axis;
}

void UniversalJoint::setAdditionalAxis(const Ogre::Vector3& axis)
{
	dJointSetUniversalAxis2(_joint,(dReal)axis.x,(dReal)axis.y,(dReal)axis.z);
}

const Ogre::Vector3& UniversalJoint::getAdditionalAxis()
{
	dVector3 result;
	dJointGetUniversalAxis2(_joint,result); 
	_additional_axis.x = (Real)result[0];
	_additional_axis.y = (Real)result[1];
	_additional_axis.z = (Real)result[2];
	return _additional_axis;
}

void UniversalJoint::setParameter(Joint::Parameter parameter,Real value,int axis)
{
	dJointSetUniversalParam(_joint,((int)parameter) + dParamGroup * (axis - 1),(dReal)value); 
}

Real UniversalJoint::getParameter(Joint::Parameter parameter,int axis)
{
	return (Real)dJointGetUniversalParam(_joint,((int)parameter) + dParamGroup * (axis - 1));
}

UniversalJoint::~UniversalJoint()
{
}

FixedJoint::FixedJoint(World *world, const JointGroup* group) : Joint(world, group)
{
	_joint = dJointCreateFixed(getWorldID(),getJointGroupID(group)); 
	registerJoint();
}

void FixedJoint::attach(const Body* body)
{
	Joint::attach(body);
	dJointSetFixed(_joint); 
}

void FixedJoint::attach(const Body* body_a,const Body* body_b)
{
	Joint::attach(body_a,body_b);
	dJointSetFixed(_joint); 
}

FixedJoint::~FixedJoint()
{
}

SuspensionJoint::SuspensionJoint(World *world, const JointGroup* group) : Joint(world, group)
{
	_joint = dJointCreateHinge2(getWorldID(),getJointGroupID(group)); 
	registerJoint();
}

void SuspensionJoint::setAnchor(const Ogre::Vector3& position)
{
	dJointSetHinge2Anchor(_joint,(dReal)position.x,(dReal)position.y,(dReal)position.z); 
}


void SuspensionJoint::addTorque(Real torque,Real torque2,Real torque3)
{
	dJointAddHinge2Torques(_joint,(dReal)torque,(dReal)torque2);
}

const Ogre::Vector3& SuspensionJoint::getAnchor()
{
	dVector3 result;
	dJointGetHinge2Anchor(_joint,result); 
	_anchor.x = (Real)result[0];
	_anchor.y = (Real)result[1];
	_anchor.z = (Real)result[2];
	return _anchor;
}

const Ogre::Vector3& SuspensionJoint::getAdditionalAnchor()
{
	dVector3 result;
	dJointGetHinge2Anchor2(_joint,result);
	_anchor2.x = (Real)result[0];
	_anchor2.y = (Real)result[1];
	_anchor2.z = (Real)result[2];
	return _anchor2;
}

const Ogre::Vector3& SuspensionJoint::getAnchorError()
{
	dVector3 result1,result2;
	dJointGetHinge2Anchor(_joint,result1); 
	dJointGetHinge2Anchor2(_joint,result2); 
	_anchor_error.x = (Real)(result1[0] - result2[2]);
	_anchor_error.y = (Real)(result1[1] - result2[2]);
	_anchor_error.z = (Real)(result1[2] - result2[2]);
	return _anchor_error;
}

void SuspensionJoint::setAxis(const Ogre::Vector3& axis)
{
	dJointSetHinge2Axis1(_joint,(dReal)axis.x,(dReal)axis.y,(dReal)axis.z);
}

const Ogre::Vector3& SuspensionJoint::getAxis()
{
	dVector3 result;
	dJointGetHinge2Axis1(_joint,result); 
	_axis.x = (Real)result[0];
	_axis.y = (Real)result[1];
	_axis.z = (Real)result[2];
	return _axis;
}

void SuspensionJoint::setAdditionalAxis(const Ogre::Vector3& axis)
{
	dJointSetHinge2Axis2(_joint,(dReal)axis.x,(dReal)axis.y,(dReal)axis.z);
}

const Ogre::Vector3& SuspensionJoint::getAdditionalAxis()
{
	dVector3 result;
	dJointGetHinge2Axis2(_joint,result); 
	_additional_axis.x = (Real)result[0];
	_additional_axis.y = (Real)result[1];
	_additional_axis.z = (Real)result[2];
	return _additional_axis;
}

Real SuspensionJoint::getAngle()
{
	return (Real)dJointGetHinge2Angle1(_joint); 
}

Real SuspensionJoint::getAngleRate()
{
	return (Real)dJointGetHinge2Angle1Rate(_joint); 
}

Real SuspensionJoint::getPositionRate()
{
	return (Real)dJointGetHinge2Angle2Rate(_joint); 
}

void SuspensionJoint::setParameter(Joint::Parameter parameter,Real value,int axis)
{
	dJointSetHinge2Param(_joint,((int)parameter) + dParamGroup * (axis - 1),(dReal)value); 
}

Real SuspensionJoint::getParameter(Joint::Parameter parameter,int axis)
{
	return (Real)dJointGetHinge2Param(_joint,((int)parameter) + dParamGroup * (axis - 1));
}

SuspensionJoint::~SuspensionJoint()
{
}

AngularMotorJoint::AngularMotorJoint(World *world, const JointGroup* group) : Joint(world, group)
{
	_joint = dJointCreateAMotor(getWorldID(),getJointGroupID(group)); 
	registerJoint();
}

void AngularMotorJoint::setMode(AngularMotorJoint::Mode mode)
{
	dJointSetAMotorMode(_joint,(int)mode); 
}

AngularMotorJoint::Mode AngularMotorJoint::getMode()
{
	return (AngularMotorJoint::Mode)dJointGetAMotorMode(_joint); 
}

void AngularMotorJoint::addTorque(Real torque,Real torque2,Real torque3)
{
	dJointAddAMotorTorques(_joint,(dReal)torque,(dReal)torque2,(dReal)torque3);
}

void AngularMotorJoint::setAxisCount(int axes)
{
	assert((axes >= 0) && (axes <= 3));
	dJointSetAMotorNumAxes(_joint,axes); 
}

int AngularMotorJoint::getAxisCount()
{
	return dJointGetAMotorNumAxes(_joint); 
}

void AngularMotorJoint::setAxis(int axis_number,AngularMotorJoint::RelativeOrientation orientation,const Ogre::Vector3& axis)
{
	dJointSetAMotorAxis(_joint,axis_number,(int)orientation,(dReal)axis.x,(dReal)axis.y,(dReal)axis.z); 
}

const Ogre::Vector3& AngularMotorJoint::getAxis(int axis_number)
{
	dVector3 result;
	dJointGetAMotorAxis(_joint,axis_number,result); 
	_axis.x = (Real)result[0];
	_axis.y = (Real)result[1];
	_axis.z = (Real)result[2];
	return _axis;
}

AngularMotorJoint::RelativeOrientation AngularMotorJoint::getAxisRelativeOrientation(int axis_number)
{
	return (AngularMotorJoint::RelativeOrientation)dJointGetAMotorAxisRel(_joint,axis_number); 
}

void AngularMotorJoint::setAngle(int axis,Real angle)
{
	dJointSetAMotorAngle(_joint,axis,(dReal)angle); 
}

Real AngularMotorJoint::getAngle(int axis)
{
	return (Real)dJointGetAMotorAngle(_joint,axis); 
}

Real AngularMotorJoint::getAngleRate(int axis)
{
	return (Real)dJointGetAMotorAngleRate(_joint,axis); 
}

void AngularMotorJoint::setParameter(Joint::Parameter parameter,Real value,int axis)
{
	dJointSetAMotorParam(_joint,((int)parameter) + dParamGroup * (axis - 1),(dReal)value); 
}

Real AngularMotorJoint::getParameter(Joint::Parameter parameter,int axis)
{
	return (Real)dJointGetAMotorParam(_joint,((int)parameter) + dParamGroup * (axis - 1));
}

AngularMotorJoint::~AngularMotorJoint()
{
}

PlanarJoint::PlanarJoint(World *world, const JointGroup* group) : Joint(world, group)
{
	_joint = dJointCreatePlane2D(getWorldID(),getJointGroupID(group)); 
	registerJoint();
}

void PlanarJoint::setParameterX(Joint::Parameter parameter,Real value,int axis)
{
	dJointSetPlane2DXParam(_joint,((int)parameter) + dParamGroup * (axis - 1),(dReal)value);
}

void PlanarJoint::setParameterY(Joint::Parameter parameter,Real value,int axis)
{
	dJointSetPlane2DYParam(_joint,((int)parameter) + dParamGroup * (axis - 1),(dReal)value);
}

void PlanarJoint::setParameterAngle(Joint::Parameter parameter,Real value,int axis)
{
	dJointSetPlane2DAngleParam(_joint,((int)parameter) + dParamGroup * (axis - 1),(dReal)value);
}

PlanarJoint::~PlanarJoint()
{
}
