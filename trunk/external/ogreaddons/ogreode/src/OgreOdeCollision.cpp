
#include "OgreOdePrecompiledHeaders.h"

#include "OgreOdeCollision.h"
#include "OgreOdeGeometry.h"

using namespace OgreOde;
using namespace Ogre;

//------------------------------------------------------------------------------------------------
CollisionListener::CollisionListener()
{
}
//------------------------------------------------------------------------------------------------
CollisionListener::~CollisionListener()
{
}
//------------------------------------------------------------------------------------------------
Contact::Contact()
{
}
//------------------------------------------------------------------------------------------------
Contact::~Contact()
{
}
//------------------------------------------------------------------------------------------------
const Ogre::Vector3& Contact::getPosition()
{
	_position.x = (Real)(_contact->geom.pos[0]);
	_position.y = (Real)(_contact->geom.pos[1]);
	_position.z = (Real)(_contact->geom.pos[2]);
	return _position;
}
//------------------------------------------------------------------------------------------------
const Ogre::Vector3& Contact::getNormal()
{
	_normal.x = (Real)(_contact->geom.normal[0]);
	_normal.y = (Real)(_contact->geom.normal[1]);
	_normal.z = (Real)(_contact->geom.normal[2]);
	return _normal;
}
//------------------------------------------------------------------------------------------------
Real Contact::getPenetrationDepth()
{
	return (Real)(_contact->geom.depth);
}
//------------------------------------------------------------------------------------------------
Geometry* Contact::getFirstGeometry()
{
	dGeomID g = _contact->geom.g1;
	if(!g) return 0;

	return (Geometry*)dGeomGetData(g);
}
//------------------------------------------------------------------------------------------------
Geometry* Contact::getSecondGeometry()
{
	dGeomID g = _contact->geom.g2;
	if(!g) return 0;

	return (Geometry*)dGeomGetData(g);
}
//------------------------------------------------------------------------------------------------
int Contact::getFirstSide()
{
   return _contact->geom.side1;
}
//------------------------------------------------------------------------------------------------
int Contact::getSecondSide()
{
   return _contact->geom.side2;
} 
//------------------------------------------------------------------------------------------------
void Contact::setFirstFrictionDirection(const Ogre::Vector3& vector)
{
	_contact->fdir1[0] = (dReal)vector.x;
	_contact->fdir1[1] = (dReal)vector.y;
	_contact->fdir1[2] = (dReal)vector.z;

	_contact->surface.mode |= (int)Flag_UseFirstFrictionDirection;
}
//------------------------------------------------------------------------------------------------
void Contact::setCoulombFriction(Real mu,Real additional_mu)
{
	_contact->surface.mu = mu;
	
	if(additional_mu >= 0.0)
	{
		_contact->surface.mu2 = additional_mu;
		_contact->surface.mode |= (int)Flag_UseAdditionalFriction;
	}
}
//------------------------------------------------------------------------------------------------
void Contact::setBouncyness(Real bouncyness,Real velocity_threshold)
{
	_contact->surface.bounce = bouncyness;
	if(velocity_threshold >= 0.0)
	{
		_contact->surface.bounce_vel = velocity_threshold;
	}
	_contact->surface.mode |= (int)Flag_SurfaceIsBouncy;
}
//------------------------------------------------------------------------------------------------
void Contact::setSoftness(Real ERP,Real CFM)
{
	_contact->surface.soft_erp = ERP;
	_contact->surface.soft_cfm = CFM;

	_contact->surface.mode |= (int)Flag_UseERP;
	_contact->surface.mode |= (int)Flag_UseCFM;
}
//------------------------------------------------------------------------------------------------
void Contact::setIndependentMotion(Real velocity,Real additional_velocity)
{
	_contact->surface.motion1 = velocity;
	_contact->surface.mode |= (int)Flag_IndependentMotion; 

	if(additional_velocity >= 0.0)
	{
		_contact->surface.motion2 = additional_velocity;
		_contact->surface.mode |= (int)Flag_AdditionalIndependentMotion;
	}
}
//------------------------------------------------------------------------------------------------
void Contact::setForceDependentSlip(Real FDS)
{
	_contact->surface.slip1 = FDS;
	_contact->surface.mode |= (int)Flag_UseFDS;
}
//------------------------------------------------------------------------------------------------
void Contact::setAdditionalFDS(Real FDS)
{
	_contact->surface.slip2 = FDS;
	_contact->surface.mode |= (int)Flag_UseAdditionalFDS;
}
//------------------------------------------------------------------------------------------------
void Contact::setFrictionMode(Contact::Flag flag)
{
	assert((flag == Flag_FrictionPyramid)||(flag == Flag_BothFrictionPyramids)||(flag == Flag_AdditionalFrictionPyramid));
	_contact->surface.mode |= (int)flag;			
}
//------------------------------------------------------------------------------------------------
ContactMapCollisionListener::ContactMapCollisionListener()
{
}
//------------------------------------------------------------------------------------------------
bool ContactMapCollisionListener::collision(Contact* contact)
{
	return false;
	/*
	unsigned long mt1 = contact->getFirstGeometry()->getMaterialType();
	if(!mt1) return false;
	
	unsigned long mt2 = contact->getSecondGeometry()->getMaterialType();
	if(!mt2) return false;

	Contact *c = getContactPtr(mt1,mt2);
	if(!c) return false;

	contact->_contact->fdir1[0] = c->_contact->fdir1[0];
	contact->_contact->fdir1[1] = c->_contact->fdir1[1];
	contact->_contact->fdir1[2] = c->_contact->fdir1[2];

	contact->_contact->surface.mu = c->_contact->surface.mu;
	contact->_contact->surface.mu2 = c->_contact->surface.mu2;

	contact->_contact->surface.bounce = c->_contact->surface.bounce;
	contact->_contact->surface.bounce_vel = c->_contact->surface.bounce_vel;

	contact->_contact->surface.soft_erp = c->_contact->surface.soft_erp;
	contact->_contact->surface.soft_cfm = c->_contact->surface.soft_cfm;

	contact->_contact->surface.motion1 = c->_contact->surface.motion1;
	contact->_contact->surface.motion2 = c->_contact->surface.motion2;

	contact->_contact->surface.slip1 = c->_contact->surface.slip1;
	contact->_contact->surface.slip2 = c->_contact->surface.slip2;

	contact->_contact->surface.mode = c->_contact->surface.mode;

	return true;
	*/
}
//------------------------------------------------------------------------------------------------
void ContactMapCollisionListener::createContact(MaterialID materialA,MaterialID materialB)
{
	std::map<MaterialID,MaterialMap* >::iterator i = _map.find(materialA);
	if(i == _map.end())
	{
		_map.insert(MaterialMapPair(materialA,new MaterialMap()));
		i = _map.find(materialA);
	}

	MaterialMap::iterator j = i->second->find(materialB);
	if(j == i->second->end())
	{
		Contact *c = new Contact();
		memset(c->_contact,0,sizeof(dContact));

		i->second->insert(std::pair<MaterialID,Contact*>(materialB,c));
	}
}
//------------------------------------------------------------------------------------------------
Contact *ContactMapCollisionListener::getContactPtr(MaterialID materialA,MaterialID materialB)
{
	Contact *c = 0;
	MaterialID A = materialA;
	MaterialID B = materialB;

	std::map<MaterialID,MaterialMap* >::iterator i = _map.find(A);
	if(i == _map.end())
	{
		B = materialA;
		A = materialB;

		i = _map.find(A);
	}

	if(i != _map.end())
	{
		MaterialMap::iterator j = i->second->find(B);
		if(j != i->second->end())
		{
			c = j->second;
		}
	}

	return c;
}
//------------------------------------------------------------------------------------------------
ContactMapCollisionListener::~ContactMapCollisionListener()
{
	std::map<MaterialID,MaterialMap* >::iterator i = _map.begin();
	std::map<MaterialID,MaterialMap* >::iterator i_end = _map.end();

	for(;i != i_end;++i)
	{
		MaterialMap::iterator j = i->second->begin();
		MaterialMap::iterator j_end = i->second->end();

		for(;j != j_end;++j) 
			delete j->second;

		delete i->second;
	}
}
//------------------------------------------------------------------------------------------------