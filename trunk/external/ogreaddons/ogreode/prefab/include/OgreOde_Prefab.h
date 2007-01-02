#ifndef _OGREODE_PREFAB_H_
#define _OGREODE_PREFAB_H_

#include "OgreOde_PrefabPreReqs.h"

namespace OgreOde_Prefab
{
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#   if defined( OGREODEEXPORT_PREFAB )
#       define _OgreOdeExport_Prefab __declspec( dllexport )
#   else
#       define _OgreOdeExport_Prefab __declspec( dllimport )
#   endif
#else
#   define _OgreOdeExport_Prefab
#endif

    enum ObjectType
    {
        ObjectType_Sphere_Wheeled_Vehicle,
        ObjectType_Ray_Wheeled_Vehicle,
        ObjectType_Wheel,
        ObjectType_Ragdoll,
        ObjectType_CompositeObject,
        ObjectType_Single_Object
        //,...
    };
}

#include "OgreOdePrefabObject.h"
#include "OgreOdeVehicle.h"
#include "OgreOdeRagdoll.h"


#endif
