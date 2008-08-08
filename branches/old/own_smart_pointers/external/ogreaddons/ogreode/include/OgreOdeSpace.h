#ifndef _OGREODESPACE_H_
#define _OGREODESPACE_H_

#include "OgreOdePreReqs.h"
#include "OgreOdeMaintainedList.h"

namespace OgreOde 
{
	class _OgreOdeExport Space
	{
        friend class Geometry;
		friend class World;

	public:
		Space(World *world, const Space* space = 0);
		virtual ~Space();

		void registerSpace();

		void setAutoCleanup(bool on); 
		bool getAutoCleanup(); 

		void addGeometry(const Geometry& geometry); 
		void removeGeometry(const Geometry& geometry); 
		bool containsGeometry(const Geometry& geometry); 
		int getGeometryCount(); 
		Geometry* getGeometry(int index);

		virtual void collide(void* data = 0);
		virtual void collide(Space* space,void* data = 0);
		virtual void collide(Geometry* geometry,void* data = 0);

		void setInternalCollisions(bool collide);
		bool getInternalCollisions();

		virtual const Ogre::AxisAlignedBox& getAxisAlignedBox(); 

		virtual unsigned long getID();

	protected:
		dSpaceID getSpaceID() const;
		dSpaceID getSpaceID(const Space* space) const;

	protected:
		dSpaceID                _space;
		bool                    _internal_collisions;
		Ogre::AxisAlignedBox    _bounding_box;
        World                   *_world;
	};

	class _OgreOdeExport SimpleSpace:public Space
	{
	public:
		SimpleSpace(World *world, const Space* space = 0);
		~SimpleSpace();
	};

	class _OgreOdeExport HashTableSpace:public Space
	{
	public:
		HashTableSpace(World *world, const Space* space = 0);
		~HashTableSpace();

		void setLevels(int min_level,int max_level);
		int getMinimumLevel();
		int getMaximumLevel();
	};

	class _OgreOdeExport QuadTreeSpace:public Space
	{
	public:
		QuadTreeSpace(const Ogre::Vector3& center,const Ogre::Vector3& extents,int depth,World *world, const Space* space = 0);
		~QuadTreeSpace();
    };

    /*
    * use Scene manager specialized scene partition algo to find collision
    * 
    */
    class _OgreOdeExport OgreSceneManagerSpace:public Space
    {
    public:
        OgreSceneManagerSpace(const Ogre::Vector3& center,
                                const Ogre::Vector3& extents,
                                int depth,
                                Ogre::SceneManager *scn_mgr,
                                World *world, const Space* space = 0);
        ~OgreSceneManagerSpace();

        void collide(void* data = 0);

    private:
        Ogre::SceneManager              *_scn_mgr;
        Ogre::IntersectionSceneQuery    *_intersection_query;

        // does scene manager has plane collision geometry support
        // BSP scene manager has that.
        bool                            _scene_geometry;
    };
}

#endif

