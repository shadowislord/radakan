#ifndef _OGREODEUTILITY_H_
#define _OGREODEUTILITY_H_

#include "OgreOdePreReqs.h"

namespace OgreOde
{
	class _OgreOdeExport Utility
	{
	public:
		Utility(){}
		~Utility(){}

		static Ogre::Real randomReal();

		static const Ogre::Real Infinity;
	};
}

#endif

