
#include "OgreOdePrecompiledHeaders.h"

#include "OgreOdePreReqs.h"
#include "OgreOdeMass.h"

using namespace OgreOde;
using namespace Ogre;

Mass::Mass()
{
	dMassSetZero(&_mass); 
}

Mass::Mass(Real mass,const Ogre::Vector3& center_of_gravity,const Matrix3& intertia_matrix)
{
	dMassSetParameters
	(
		&_mass,
		(dReal)mass,
		(dReal)center_of_gravity.x,(dReal)center_of_gravity.y,(dReal)center_of_gravity.z,
		(dReal)intertia_matrix[0][0],(dReal)intertia_matrix[1][1],(dReal)intertia_matrix[2][2],
		(dReal)intertia_matrix[0][1],(dReal)intertia_matrix[0][2],(dReal)intertia_matrix[1][2]
	); 
}

Mass::~Mass()
{
}

void Mass::adjust(Real mass)
{
	dMassAdjust(&_mass,(dReal)mass); 
}

void Mass::translate(const Ogre::Vector3& offset)
{
	dMassTranslate(&_mass,offset.x,offset.y,offset.z); 
}

void Mass::rotate(const Ogre::Quaternion& orientation)
{
	Matrix3 m;
	orientation.ToRotationMatrix(m);
	dMatrix3 r;

	r[0] = m[0][0];
	r[1] = m[0][1];
	r[2] = m[0][2];

	r[3] = m[1][0];
	r[4] = m[1][1];
	r[5] = m[1][2];

	r[6] = m[2][0];
	r[7] = m[2][1];
	r[8] = m[2][2];

	r[9] = 0.0;
	r[10] = 0.0;
	r[11] = 0.0;

	dMassRotate(&_mass,r); 
}

void Mass::add(const Mass& other)
{
	dMassAdd(&_mass,&(other._mass)); 
}

const dMass* Mass::getMassPtr() const
{
	return &_mass;
}

SphereMass::SphereMass():Mass()
{
}

SphereMass::SphereMass(Real mass,Real radius)
{
	dMassSetSphereTotal(&_mass,(dReal)mass,(dReal)radius); 
}

SphereMass::~SphereMass()
{
}

void SphereMass::setDensity(Real density,Real radius)
{
	dMassSetSphere(&_mass,(dReal)density,(dReal)radius); 
}

CapsuleMass::CapsuleMass():Mass()
{
}

CapsuleMass::CapsuleMass(Real mass,Real radius,const Ogre::Vector3& direction,Real length)
{
	int dir = (direction == Ogre::Vector3::UNIT_X)?1:(direction == Ogre::Vector3::UNIT_Y)?2:(direction == Ogre::Vector3::UNIT_Z)?3:0;
	assert(dir);
	dMassSetCappedCylinderTotal(&_mass,(dReal)mass,dir,(dReal)radius,(dReal)length); 
}

CapsuleMass::~CapsuleMass()
{
}

void CapsuleMass::setDensity(Real density,Real radius,const Ogre::Vector3& direction,Real length)
{
	int dir = (direction == Ogre::Vector3::UNIT_X)?1:(direction == Ogre::Vector3::UNIT_Y)?2:(direction == Ogre::Vector3::UNIT_Z)?3:0;
	assert(dir && "Invalid direction specified for CapsuleMass");
	dMassSetCappedCylinder(&_mass,(dReal)density,dir,(dReal)radius,(dReal)length); 
}

CylinderMass::CylinderMass():Mass()
{
}

CylinderMass::CylinderMass(Real mass,const Ogre::Vector3& direction,Real radius,Real length)
{
	int dir = (direction == Ogre::Vector3::UNIT_X)?1:(direction == Ogre::Vector3::UNIT_Y)?2:(direction == Ogre::Vector3::UNIT_Z)?3:0;
	assert(dir);
	dMassSetCylinderTotal(&_mass,(dReal)mass,dir,(dReal)radius,(dReal)length); 
}

CylinderMass::~CylinderMass()
{
}

void CylinderMass::setDensity(Real density,const Ogre::Vector3& direction,Real radius,Real length)
{
	int dir = (direction == Ogre::Vector3::UNIT_X)?1:(direction == Ogre::Vector3::UNIT_Y)?2:(direction == Ogre::Vector3::UNIT_Z)?3:0;
	assert(dir);
	dMassSetCylinder(&_mass,(dReal)density,dir,(dReal)radius,(dReal)length); 
}

BoxMass::BoxMass():Mass()
{
}

BoxMass::BoxMass(Real mass,const Ogre::Vector3& size)
{
	dMassSetBoxTotal(&_mass,(dReal)mass,(dReal)size.x,(dReal)size.y,(dReal)size.z);
}

BoxMass::~BoxMass()
{
}

void BoxMass::setDensity(Real density,const Ogre::Vector3& size)
{
	dMassSetBox(&_mass,(dReal)density,(dReal)size.x,(dReal)size.y,(dReal)size.z); 
}
