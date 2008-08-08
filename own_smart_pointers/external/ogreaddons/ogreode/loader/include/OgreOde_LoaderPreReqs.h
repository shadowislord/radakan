#ifndef _OGREODELOADERPREREQS_H_
#define _OGREODELOADERPREREQS_H_


#include "Ogre.h"
#include "OgreNoMemoryMacros.h"
#include "ode/ode.h"
#include "OgreMemoryMacros.h"
#include "OgreOde_Core.h"
#include "OgreOde_Prefab.h"

namespace OgreOde_Loader
{
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#   if defined( OGREODEEXPORT_LOADER )
#       define _OgreOdeExport_Loader __declspec( dllexport )
#   else
#       define _OgreOdeExport_Loader __declspec( dllimport )
#   endif
#else
#   define _OgreOdeExport_Loader
#endif

    typedef std::list<OgreOde_Prefab::Object  *>   ObjectList;
	class DotLoader;
}

#endif //_OGREODELOADERPREREQS_H_
