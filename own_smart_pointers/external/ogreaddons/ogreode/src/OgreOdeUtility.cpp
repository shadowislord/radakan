
#include "OgreOdePrecompiledHeaders.h"

#include "OgreOdeUtility.h"

using namespace OgreOde;
using namespace Ogre;

const Ogre::Real Utility::Infinity = dInfinity;

Real Utility::randomReal()
{
	return (Real)dRandReal();
}

