#ifndef _OGREODEPREFABOBJECT_H_
#define _OGREODEPREFABOBJECT_H_

#include "OgreOde_Prefab.h"

namespace OgreOde_Prefab 
{

    class _OgreOdeExport_Prefab Object
    {
	public:
        Object(ObjectType type, OgreOde::World *world):
          _type(type),
          _world(world)
        {
              instanceNumber++;
        }
		virtual ~Object()
        {
            instanceNumber--;
        }

		OgreOde_Prefab::ObjectType getObjectType() const
        {
            return _type;
        }

		static unsigned int getInstanceNumber()
		{
			return instanceNumber;
		}

        static unsigned int getInstanceNumberAndIncrement()
        {
            return instanceNumber++;
        }

	protected:
		OgreOde_Prefab::ObjectType  _type;
        OgreOde::World              *_world;

        static unsigned int instanceNumber;
	};
}

#endif

