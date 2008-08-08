#include "OgreOdeWorld.h"
#include "OgreOdeBody.h"
#include "OgreOdeJoint.h"
#include "OgreOdeGeometry.h"
#include "OgreOdeSpace.h"
#include "OgreOdeVehicle.h"
#include "OgreOdeEntityInformer.h"
#include "OgreOdeMass.h"
#include "OgreOdeUtility.h"

#include "OgreOdeVehicle.h"

using namespace Ogre;
using namespace OgreOde;
using namespace OgreOde_Prefab;

//------------------------------------------------------------------------------------------------
const Ogre::Real Vehicle::Wheel::FudgeFactor = 0.1;

//------------------------------------------------------------------------------------------------
Vehicle::Wheel::Wheel(Vehicle* vehicle,const String& name,const String& mesh,const Ogre::Vector3& position,Real mass, World *world, Space* space):
	Object(OgreOde_Prefab::ObjectType_Wheel, world) 
{
	_owner = vehicle;
	_name = name;

	_entity = _world->getSceneManager()->createEntity(name + "_Entity",mesh);
	_entity->setCastShadows(true);	

	EntityInformer ei(_entity);
	_radius = ei.getRadius();

	_node = _world->getSceneManager()->getRootSceneNode()->createChildSceneNode(name + "_Node");
	_node->setPosition(position);
	_body = new Body(_world, name + "_Body");


	_node->attachObject(_entity);
	_node->attachObject(_body);

	_body->setMass(SphereMass(mass,_radius));

	_geometry = new SphereGeometry(_radius, _world, space);
	_geometry->setBody(_body);
	_geometry->setUserObject(this);

	_joint = new SuspensionJoint(_world);
	_joint->attach(vehicle->getBody(),_body);
	_joint->setAnchor(_body->getPosition());
	_joint->setAxis(Vector3::UNIT_Y);
	_joint->setAdditionalAxis(Vector3::NEGATIVE_UNIT_X);
	_joint->setParameter(Joint::Parameter_FudgeFactor,Vehicle::Wheel::FudgeFactor);
	
	_bouncyness = _friction = _fds = 0.0;

	setSteerFactor(0.0);
	setSteerLimit(0.0);
	setSteerForce(0.0);
	setSteerSpeed(0.0);
			
	setPowerFactor(0.0);
			
	setBrakeFactor(0.0);

	setSteer(0.0);
}


//------------------------------------------------------------------------------------------------
void Vehicle::Wheel::setFactors(Real steer,Real power,Real brake)
{
	setSteerFactor(steer);
	setPowerFactor(power);
	setBrakeFactor(brake);
}


//------------------------------------------------------------------------------------------------
Real Vehicle::Wheel::getRPM()
{
	return ((_joint)&&(_power_factor > 0.0))?_joint->getParameter(Joint::Parameter_MotorVelocity,2):0.0;
}


//------------------------------------------------------------------------------------------------
Real Vehicle::Wheel::getSteer()
{
	return ((_joint)&&(_steer_factor != 0.0))?_joint->getAngle():0.0;
}


//------------------------------------------------------------------------------------------------
void Vehicle::Wheel::setSteerFactor(Real factor)
{
	_steer_factor = factor;
	adjustJointParameters();
}


//------------------------------------------------------------------------------------------------
void Vehicle::Wheel::setSteerLimit(Real limit)
{
	_steer_limit = limit;
	adjustJointParameters();
}


//------------------------------------------------------------------------------------------------
void Vehicle::Wheel::adjustJointParameters()
{
	if (_joint)
	{
		_joint->setParameter(Joint::Parameter_HighStop,_steer_limit * fabs(_steer_factor));
		_joint->setParameter(Joint::Parameter_LowStop,-(_steer_limit * fabs(_steer_factor)));
	}
}


//------------------------------------------------------------------------------------------------
/* 
According to the ODE docs;

By adjusting the values of ERP and CFM, you can achieve various effects. 
For example you can simulate springy constraints, where the two bodies oscillate 
as though connected by springs. Or you can simulate more spongy constraints, without 
the oscillation. In fact, ERP and CFM can be selected to have the same effect as any 
desired spring and damper constants. If you have a spring constant kp and damping constant kd, 
then the corresponding ODE constants are:

ERP = h kp / (h kp + kd)
CFM = 1 / (h kp + kd)

where h is the stepsize. These values will give the same effect as a spring-and-damper 
system simulated with implicit first order integration.
*/
void Vehicle::Wheel::setSuspension(Real spring_constant,Real damping_constant,Real step_size)
{
	_spring = spring_constant;
	_damping = damping_constant;
	_step = step_size;

	if (_joint)
	{
		_joint->setParameter(Joint::Parameter_SuspensionERP,_step * _spring / (_step * _spring + _damping));
		_joint->setParameter(Joint::Parameter_SuspensionCFM,1.0 / (_step * _spring + _damping));
	}
}
//------------------------------------------------------------------------------------------------
const Ogre::Vector3& Vehicle::Wheel::getPosition()
{
	return _body->getPosition();
}
//------------------------------------------------------------------------------------------------
const Ogre::Quaternion& Vehicle::Wheel::getOrientation()
{
	return _body->getOrientation();
}
void Vehicle::Wheel::setPosition(const Ogre::Vector3& position)
{
	_body->setPosition(position);
}
//------------------------------------------------------------------------------------------------
void Vehicle::Wheel::update(Real power_force,Real desired_rpm,Real brake_force)
{
	if (!_joint) return;

	Real force = (power_force * _power_factor) - (brake_force * _brake_factor);

	if (force > 0.0)
	{
		_joint->setParameter(Joint::Parameter_MotorVelocity,desired_rpm,2);
		_joint->setParameter(Joint::Parameter_MaximumForce,force,2);
	}
	else
	{
		_joint->setParameter(Joint::Parameter_MotorVelocity,0.01,2);
		_joint->setParameter(Joint::Parameter_MaximumForce,-force,2);
	}
}
//------------------------------------------------------------------------------------------------
void Vehicle::Wheel::setSteer(Real position)
{
	Real steer_angle = position * _steer_limit;

	// Steer
	if (_steer_limit * _steer_factor != 0.0)
	{
		Real vel = (steer_angle * _steer_factor) - _joint->getAngle();
		vel *= (_steer_speed * fabs(_steer_factor));

		_joint->setParameter(Joint::Parameter_MotorVelocity,vel);
		_joint->setParameter(Joint::Parameter_MaximumForce,_steer_force);
	}
}
//------------------------------------------------------------------------------------------------
void Vehicle::Wheel::snapOff()
{
	delete _joint;
	_joint = 0;
}
//------------------------------------------------------------------------------------------------
Vehicle::Wheel::~Wheel()
{
	delete _joint;
	delete _body;
	delete _geometry;

	_world->getSceneManager()->getRootSceneNode()->removeAndDestroyChild(_node->getName());
	_world->getSceneManager()->destroyEntity(_entity->getName());
}
//------------------------------------------------------------------------------------------------
Vehicle::Vehicle(const Ogre::String& name,const Ogre::String& mesh, Ogre::Real mass, 
                 OgreOde::World *world, 
                 OgreOde::Space* space,
                 const Ogre::Vector3& offset,
                 OgreOde::Geometry* geometry):
	Object(OgreOde_Prefab::ObjectType_Sphere_Wheeled_Vehicle, world),
    _name ( name),
    _antisway (false),
    _swayRate (Ogre::Real(0.0)),
    _swayLastUpdate (Ogre::Real(0.0)),
    _swayForce (Ogre::Real(0.0)),
    _swayForceLimit (0)

{
	
	_entity = _world->getSceneManager()->createEntity(name + "_Entity",mesh);
	_entity->setCastShadows(true);
	_node = _world->getSceneManager()->getRootSceneNode()->createChildSceneNode(name + "_Node");
    _body = new Body(_world, name + "_Body");

	_trans_node = _node->createChildSceneNode("Trans_" + _node->getName());
	_trans_node->attachObject(_entity);
	_node->attachObject(_body);

	EntityInformer ei(_entity);
	Vector3 box = ei.getSize();
	
	BoxMass box_mass(mass,Vector3(box.x,box.y,box.z));
	_body->setMass(box_mass);

    _space = new SimpleSpace(_world, space ? space : _world->getDefaultSpace());
	_space->setInternalCollisions(false);
	_space->setAutoCleanup(false);

	_offset = -offset;

	if (!geometry) 
		_geometry = new BoxGeometry(box, _world, space ? space : _world->getDefaultSpace());
	else 
		_geometry = geometry;
	_geometry->setUserObject(this);

	_transform = new TransformGeometry(_world, _space);
	_transform->setEncapsulatedGeometry(_geometry);
	_transform->setBody(_body);
	_geometry->setPosition(_offset);

	_trans_node->setPosition(_offset);

	_engine = new Vehicle::Engine();
}
//------------------------------------------------------------------------------------------------
    Vehicle::Vehicle(const Ogre::String& name, 
                    OgreOde::World *world, 
                    OgreOde::Space* space,
                    OgreOde::Geometry* geometry) : 
        Object(OgreOde_Prefab::ObjectType_Sphere_Wheeled_Vehicle, world),
        _name (name),
        _antisway(false),
        _swayRate (Ogre::Real(0.0)),
        _swayLastUpdate (Ogre::Real(0.0)),
        _swayForce (Ogre::Real(0.0)),
        _swayForceLimit (0),
        _entity (0),
        _geometry (geometry),
        _transform (0)
{

        _space = new SimpleSpace(_world, space?space:_world->getDefaultSpace());
	_space->setInternalCollisions(false);
	_space->setAutoCleanup(false);

	_engine = new Vehicle::Engine();

	_node = _world->getSceneManager()->getRootSceneNode()->createChildSceneNode(_name  + Ogre::StringConverter::toString (instanceNumber) + "_Node");
	_body = new Body(_world, _name  + Ogre::StringConverter::toString (instanceNumber) + "_Body");

	_trans_node = _node->createChildSceneNode("Trans_" + _node->getName());
	_node->attachObject(_body);

	if (_geometry) 
		_geometry->setUserObject(this);
	
}
//------------------------------------------------------------------------------------------------
Vehicle::Wheel* Vehicle::addWheel(const String& mesh,const Ogre::Vector3& position,Real mass)
{
	const String wheelName ( _name +  
		Ogre::StringConverter::toString (instanceNumber) +  
		"_Wheel" + 
		StringConverter::toString((unsigned int)_wheels.size() + 1));

	Vehicle::Wheel* wheel = new Vehicle::Wheel(this, 
												wheelName, 
												mesh, 
												position + _body->getPosition() + _offset,
												mass,
                                                _world,
												_space);
	_wheels.push_back(wheel);
	return wheel;
}
//------------------------------------------------------------------------------------------------
const Ogre::Vector3& Vehicle::getPosition()
{
    _lastPosition = _body->getPosition() + _offset;
	return _lastPosition;
}
//------------------------------------------------------------------------------------------------
const Ogre::Quaternion& Vehicle::getOrientation()
{
	return _body->getOrientation();
}
//------------------------------------------------------------------------------------------------
void Vehicle::setPosition(const Ogre::Vector3& position)
{
	for (std::vector<Vehicle::Wheel*>::iterator i = _wheels.begin();i != _wheels.end();i++)
	{
		Vector3 diff = (*i)->getPosition() - _body->getPosition();
		(*i)->setPosition(position + diff);
	}
	_body->setPosition(position);
}
//------------------------------------------------------------------------------------------------
void Vehicle::setSuspension(Real spring_constant,Real damping_constant,Real step_size)
{
	for (std::vector<Vehicle::Wheel*>::iterator i = _wheels.begin();i != _wheels.end();i++)
	{
		(*i)->setSuspension(spring_constant,damping_constant,step_size);
	}
}
//------------------------------------------------------------------------------------------------
void Vehicle::setInputs(bool left,bool right,bool throttle,bool brake)
{
    if (left || right || throttle)
    {
        for (std::vector<Vehicle::Wheel*>::iterator i = _wheels.begin();i != _wheels.end();i++)
        {
            (*i)->getBody()->wake();
        }
        _body->wake();
    }
	_engine->setInputs(throttle,brake);
	setSteering(0.0 + ((right)?1.0:0.0) - ((left)?1.0:0.0));
}
//------------------------------------------------------------------------------------------------
void Vehicle::setInputs(Real steering_position,Real throttle_brake)
{
	_engine->setInputs(throttle_brake);
	setSteering(steering_position);
}
//------------------------------------------------------------------------------------------------
void Vehicle::setInputs(Real steering_position,Real throttle_position,Real brake_position)
{
	_engine->setInputs(throttle_position,brake_position);
	setSteering(steering_position);
}
//------------------------------------------------------------------------------------------------
void Vehicle::setSteering(Real steering_position)
{
	for (std::vector<Vehicle::Wheel*>::iterator i = _wheels.begin();i != _wheels.end();i++)
	{
		(*i)->setSteer(steering_position);
	}
}
//------------------------------------------------------------------------------------------------
Real Vehicle::getVelocity()
{
	return _body->getLinearVelocity().length();
}
//------------------------------------------------------------------------------------------------
void Vehicle::applyAntiSwayBarForces (  )
{
	Real amt;
	Body * wheelBody;
	for( int i = 0; i < 4; i++)
	{
		SuspensionJoint * const wheelJoint = static_cast<SuspensionJoint*>( _wheels.at( i )->getBody()->getJoint(0) );
		const Ogre::Vector3 anchor2 (wheelJoint->getAdditionalAnchor());
		const Ogre::Vector3 anchor1 (wheelJoint->getAnchor());
		const Ogre::Vector3 axis (wheelJoint->getAxis());
		const Ogre::Real displacement = ( anchor1 - anchor2 ).dotProduct( axis );
		if( displacement > 0 )
		{

			amt = displacement * _swayForce;
			if( amt > _swayForceLimit ) 
				amt = _swayForceLimit;

			// force down
			wheelBody = _wheels.at( i )->getBody();
			wheelBody->addForce( -axis * amt );

			// force up
			wheelBody = _wheels.at( i^1 )->getBody();
			wheelBody->addForce( axis * amt );
		}
	}
}
//------------------------------------------------------------------------------------------------
void Vehicle::update(Real time)
{
	std::vector<Vehicle::Wheel*>::iterator b = _wheels.begin();
	std::vector<Vehicle::Wheel*>::iterator e = _wheels.end();

	Real rpm = FLT_MIN;

	for (std::vector<Vehicle::Wheel*>::iterator i = b;i != e;i++)
	{
		rpm = std::max(rpm,(*i)->getRPM());
	}

	_engine->update(time);
	Real power = _engine->getPowerAtRPM(rpm);
	Real desired_rpm = _engine->getDesiredRPM();
	Real brake = _engine->getBrakeForce();

	for (std::vector<Vehicle::Wheel*>::iterator i = b;i != e;i++)
	{
		(*i)->update(power,desired_rpm,brake);
	}
	
	if (_antisway)
	{
		_swayLastUpdate += time;
		if  (_swayLastUpdate > _swayRate)
		{
			applyAntiSwayBarForces ();
			_swayLastUpdate =Ogre::Real(0.0);
		}
	}
}
//------------------------------------------------------------------------------------------------
Vehicle::Wheel* Vehicle::findWheel(Geometry* geometry)
{
	for (std::vector<Vehicle::Wheel*>::iterator i = _wheels.begin();i != _wheels.end();i++)
	{
		if ((*i)->getGeometry() == geometry)
			return (*i);
	}
	return 0;
}
//------------------------------------------------------------------------------------------------
Vehicle::~Vehicle()
{
	// _world->unregisterVehicle(_name);

	for (std::vector<Vehicle::Wheel*>::iterator i = _wheels.begin();i != _wheels.end();i++) 
		delete (*i);

	delete _body;
	delete _geometry;
	delete _transform;
	delete _space;
	delete _engine;

	_node->removeAndDestroyChild(_trans_node->getName());
	_world->getSceneManager()->getRootSceneNode()->removeAndDestroyChild(_node->getName());
	_world->getSceneManager()->destroyEntity(_entity->getName());
}
//------------------------------------------------------------------------------------------------
Vehicle::Engine::Engine()  :
	_torque_curve(0),
	_curve_length (0)
{
	setInputs(false,false);
	setRevLimit(40.0);
	setTorque(0.5,5.0);
	setBrakeForce(500.0);
}
//------------------------------------------------------------------------------------------------
Real Vehicle::Engine::getPowerAtRPM(Real rpm)
{
	_wheel_rpm = rpm;

	const Ogre::Real pos = getRevFactor() * (_curve_length - 1);
	const unsigned int i = (unsigned int)pos;
	const Ogre::Real dx = pos - ((Real)i);

	assert (i < _curve_length);
	return _torque_curve[i] + (dx * (_torque_curve[i+1] - _torque_curve[i]));
}
//------------------------------------------------------------------------------------------------
Real Vehicle::Engine::getBrakeForce()
{
	return _brake_force * _brake_position;
}
//------------------------------------------------------------------------------------------------
void Vehicle::Engine::setInputs(Real throttle_position,Real brake_position)
{
	_throttle_position = throttle_position;
	_brake_position = _brake_position;
}
//------------------------------------------------------------------------------------------------
void Vehicle::Engine::setInputs(Real throttle_brake)
{
	_throttle_position = std::max((Real)0.0,throttle_brake);
	_brake_position = std::max((Real)0.0,-throttle_brake);
}
//------------------------------------------------------------------------------------------------
void Vehicle::Engine::setInputs(bool throttle,bool brake)
{
	_throttle_position = (throttle)?1.0:0.0;
	_brake_position = (brake)?1.0:0.0;
}
//------------------------------------------------------------------------------------------------
// Manual gearbox
void Vehicle::Engine::changeUp()
{
}
//------------------------------------------------------------------------------------------------
void Vehicle::Engine::changeDown()
{
}
//------------------------------------------------------------------------------------------------
void Vehicle::Engine::setGear(char code)
{
}
//------------------------------------------------------------------------------------------------
void Vehicle::Engine::setGear(unsigned int i)
{
}
//------------------------------------------------------------------------------------------------
// Information functions
Real Vehicle::Engine::getDesiredRPM()
{
	return _rev_limit * _throttle_position;
}
//------------------------------------------------------------------------------------------------
Real Vehicle::Engine::getRevFactor()
{
	return _wheel_rpm * _inv_rev_limit;
}
//------------------------------------------------------------------------------------------------
char Vehicle::Engine::getGearCode()
{
	return '\0';
}
//------------------------------------------------------------------------------------------------
// Engine parameters
void Vehicle::Engine::setRevLimit(Real rpm)
{
	_rev_limit = rpm;
	_inv_rev_limit = 1.0 / _rev_limit;
}
//------------------------------------------------------------------------------------------------
// Set a custom torque curve
void Vehicle::Engine::setTorque(Real* torque,unsigned int samples)
{
	assert((samples > 1)&&("Need more than one sample for setTorque"));

	delete[] _torque_curve;
	_curve_length = samples;
	_torque_curve = new Ogre::Real[_curve_length + 1];

	for (unsigned int i = 0;i < samples;i++) 
		_torque_curve[i] = torque[i];
	_torque_curve[_curve_length] = torque[samples - 1];
}
//------------------------------------------------------------------------------------------------
// Set a linear torque curve, minimum should be greater than zero, 
// or the vehicle won't go anywhere!
void Vehicle::Engine::setTorque(Real minimum,Ogre::Real maximum)
{
	Real curve[] = {minimum,maximum};
	setTorque(curve, 2);
}
//------------------------------------------------------------------------------------------------
// Set a constant torque "curve"
void Vehicle::Engine::setTorque(Real constant)
{
	setTorque(constant, constant);
}
//------------------------------------------------------------------------------------------------
// Braking parameters
void Vehicle::Engine::setBrakeForce(Real force)
{
	_brake_force = force;
}
//------------------------------------------------------------------------------------------------
void Vehicle::Engine::update(Real time)
{
}

//------------------------------------------------------------------------------------------------
// Drivetrain parameters
void Vehicle::Engine::addGear(Real ratio,char code)
{
}
//------------------------------------------------------------------------------------------------
void Vehicle::Engine::setFinalDrive(Real ratio)
{
}
//------------------------------------------------------------------------------------------------
void Vehicle::Engine::setAutoShiftRevs(Real up,Real down)
{
}
//------------------------------------------------------------------------------------------------
void Vehicle::Engine::setAutoBox(bool automatic)
{
}
//------------------------------------------------------------------------------------------------
Vehicle::Engine::~Engine()
{
	delete[] _torque_curve;
}
//------------------------------------------------------------------------------------------------
void Vehicle::Wheel::setupTyreContact(OgreOde::Contact* contact)
{
	Quaternion vehicle_orient = _owner->getBody()->getOrientation();

	Vector3 wheel_up = vehicle_orient * Ogre::Vector3::UNIT_Y;
	Quaternion wheel_rot(Radian(getSteer()), (Vector3) wheel_up);
	Vector3 wheel_forward = wheel_rot * (vehicle_orient * Ogre::Vector3::UNIT_Z);
	Vector3 wheel_velocity = _body->getLinearVelocity();

	contact->setFrictionMode(Contact::Flag_BothFrictionPyramids);
	contact->setBouncyness(_bouncyness);
	contact->setCoulombFriction(_friction);
	contact->setFirstFrictionDirection(wheel_forward);
	contact->setAdditionalFDS(wheel_velocity.length() * _fds);
}
//------------------------------------------------------------------------------------------------
bool Vehicle::handleTyreCollision(OgreOde::Contact* contact)
{
	Geometry *geom = contact->getFirstGeometry();

	Object* pObject = (Object*) geom->getUserObject();
	if (pObject && 
		(pObject->getObjectType() == OgreOde_Prefab::ObjectType_Wheel))
	{
		((OgreOde_Prefab::Vehicle::Wheel*)(pObject))->setupTyreContact(contact);
		return true;
	}
	else
	{
		geom = contact->getSecondGeometry();
		pObject = (Object*) geom->getUserObject();
		if (pObject && (pObject->getObjectType() == OgreOde_Prefab::ObjectType_Wheel))
		{
			((OgreOde_Prefab::Vehicle::Wheel*)pObject)->setupTyreContact(contact);
			return true;
		}
	}
	return false;
}
