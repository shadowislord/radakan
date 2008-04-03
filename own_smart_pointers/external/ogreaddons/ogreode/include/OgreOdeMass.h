#ifndef _OGREODEMASS_H_
#define _OGREODEMASS_H_

#include "OgreOdePreReqs.h"

namespace OgreOde 
{
	class _OgreOdeExport Mass
	{
		friend class Body;

	public:
		Mass();
        Mass(Ogre::Real mass, const Ogre::Vector3& center_of_gravity, const Ogre::Matrix3& intertia_matrix); 
		virtual ~Mass();

		inline Mass(const Mass& other)
		{
			memcpy(&_mass,&(other._mass),sizeof(_mass));
		}

		inline Mass& operator=(const Mass& other)
		{
			memcpy(&_mass,&(other._mass),sizeof(_mass));
			return *this;
		}

		inline Mass& operator=(const dMass* other)
		{
			assert(other);
			memcpy(&_mass,other,sizeof(_mass));
			return *this;
		}

		void adjust(Ogre::Real mass); 
		void translate(const Ogre::Vector3& offset); 
		void rotate(const Ogre::Quaternion& orientation); 
		void add(const Mass& other); 

	protected:
		const dMass* getMassPtr() const;
		dMass _mass;
	};

	class _OgreOdeExport SphereMass:public Mass
	{
	public:
		SphereMass();
		SphereMass(Ogre::Real mass, Ogre::Real radius);
		~SphereMass();

		void setDensity(Ogre::Real density, Ogre::Real radius);
	};

	class _OgreOdeExport CapsuleMass:public Mass
	{
	public:
		CapsuleMass();
		CapsuleMass(Ogre::Real mass, Ogre::Real radius,const Ogre::Vector3& direction, Ogre::Real length);
		~CapsuleMass();

		void setDensity(Ogre::Real density, Ogre::Real radius,const Ogre::Vector3& direction, Ogre::Real length);
	};

	class _OgreOdeExport CylinderMass:public Mass
	{
	public:
		CylinderMass();
		CylinderMass(Ogre::Real mass,const Ogre::Vector3& direction, Ogre::Real radius, Ogre::Real length);
		~CylinderMass();

		void setDensity(Ogre::Real density,const Ogre::Vector3& direction, Ogre::Real radius, Ogre::Real length);
	};

	class _OgreOdeExport BoxMass:public Mass
	{
	public:
		BoxMass();
		BoxMass(Ogre::Real mass,const Ogre::Vector3& size);
		~BoxMass();

		void setDensity(Ogre::Real density,const Ogre::Vector3& size);
	};
}

#endif
