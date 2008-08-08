#ifndef _DotLoader_H_
#define _DotLoader_H_

#include "OgreOde_Loader.h"

class TiXmlNode;
class TiXmlDocument;

namespace OgreOde_Loader 
{

	class _OgreOdeExport_Loader DotLoader 
	{
	public:
        DotLoader(OgreOde::World * world = 0, OgreOde::Space * space = 0);
		~DotLoader(){};

        void setWorld(OgreOde::World * world)  {_world = world;};
        void setSpace(OgreOde::Space * space)  {_space = space;};

        ObjectList  *load(const Ogre::String &filename, const Ogre::StringVector &objects_names, const Ogre::StringVector &instance_names);
        void         save(const Ogre::String &filename, const Ogre::StringVector &objects_names, ObjectList  *objects);

        OgreOde_Prefab::Object    *loadObject(const Ogre::String &filename, const Ogre::String &object_name, const Ogre::String &instance_name = Ogre::StringUtil::BLANK);
        void                       saveObject(const Ogre::String &filename, const Ogre::String &object_name, OgreOde_Prefab::Object  *object);


    private:

        void                serializeObject(const Ogre::String &filename, const Ogre::String &object_name,  OgreOde_Prefab::Object  *object);
        void                serializeVehicle(const Ogre::String &filename, const Ogre::String &vehicle_name, OgreOde_Prefab::Vehicle *vehicle);
        void                serializeRagdoll(const Ogre::String &filename, const Ogre::String &ragdoll_name, OgreOde_Prefab::Ragdoll *ragdoll);

        OgreOde_Prefab::Object      *parseObject  (const TiXmlNode *child, const Ogre::String &object_name, const Ogre::String &instance_name = Ogre::StringUtil::BLANK);
        OgreOde_Prefab::Object      *parseCompositeObject  (const TiXmlNode *child, const Ogre::String &object_name, const Ogre::String &instance_name = Ogre::StringUtil::BLANK);
        OgreOde_Prefab::Object      *parseSingleObject  (const TiXmlNode *child, const Ogre::String &object_name, const Ogre::String &instance_name = Ogre::StringUtil::BLANK);
        OgreOde_Prefab::Vehicle     *parseVehicle (const TiXmlNode *child, const Ogre::String &vehicle_name, const Ogre::String &instance_name = Ogre::StringUtil::BLANK);
        OgreOde_Prefab::Ragdoll     *parseRagdoll (const TiXmlNode *child, const Ogre::String &ragdoll_name, const Ogre::String &instance_name = Ogre::StringUtil::BLANK);



		void parseRagdollSettings(OgreOde_Prefab::Ragdoll::BoneSettings *bone_settings, const void *tag) const;

    protected:
        // load and check file
        TiXmlDocument * loadFile(const Ogre::String &filename);

        OgreOde::World * _world;
        OgreOde::Space * _space;
	};
}

#endif //DotLoader
