#ifndef _OgreOdeDotLoader_H_
#define _OgreOdeDotLoader_H_

#include "OgreOde_Prefab.h"

namespace OgreOde_Prefab 
{

	class _OgreOdeExport_Prefab OgreOdeDotLoader 
	{
	public:
		OgreOdeDotLoader(){};
		~OgreOdeDotLoader(){};


		Object  *loadObject(const String &filename, const String &object_name);
		Vehicle *loadVehicle(const String &filename, const String &vehicle_name);
		Ragdoll *loadRagdoll(const String &filename, Ragdoll *ragdoll, const String &ragdoll_name);


		void saveObject(const String &filename, const String &object_name, Object  *object);
		void saveVehicle(const String &filename, const String &vehicle_name, Vehicle *vehicle);
		void saveRagdoll(const String &filename, const String &ragdoll_name, Ragdoll *ragdoll);

		PrefabObjectList  *load(const String &filename);
		void save(const String &filename, const StringVector &objects_names, PrefabObjectList  *objects);

	private:
		void parseRagdollSettings(Ragdoll::BoneSettings *bone_settings, const void *tag) const;
	};
}

#endif //OgreOdeDotLoader
