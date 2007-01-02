#ifndef _OGREODEDEBUGOBJECT_H_
#define _OGREODEDEBUGOBJECT_H_

#include "OgreOdePreReqs.h"

namespace OgreOde
{
    //------------------------------------------------------------------------------------------------
	class _OgreOdeExport DebugLines:public Ogre::SimpleRenderable
	{
	public:
		DebugLines(void);
		~DebugLines(void);

		void addLine(const Ogre::Vector3 &start,const Ogre::Vector3 &end)
		{
			clear();

			_points.push_back(start);
			_points.push_back(end);
		}
		void addLine(Ogre::Real start_x, Ogre::Real start_y, Ogre::Real start_z, 
            Ogre::Real end_x, Ogre::Real end_y, Ogre::Real end_z)
		{
			addLine(Ogre::Vector3(start_x,start_y,start_z),Ogre::Vector3(end_x,end_y,end_z));
		}
		void draw();
		void clear();

		Ogre::Real getSquaredViewDepth(const Ogre::Camera *cam) const;
		Ogre::Real getBoundingRadius(void) const;

	protected:

		Vector3Array _points;
		bool _drawn;

		static bool _materials_created;
	};
	//------------------------------------------------------------------------------------------------
	class _OgreOdeExport DebugObject:public DebugLines
	{
	public:
		enum Mode
		{
			Mode_Unknown,
			Mode_Enabled,
			Mode_Disabled,
			Mode_Static
		};

	public:
        DebugObject(DebugObject::Mode mode = DebugObject::Mode_Enabled);
		virtual ~DebugObject();

		void setMode(DebugObject::Mode mode);

	protected:
		DebugObject::Mode _mode;
	};
	//------------------------------------------------------------------------------------------------
	class _OgreOdeExport BoxDebugObject:public DebugObject
	{
	public:
		BoxDebugObject(const Ogre::Vector3& size);
		virtual ~BoxDebugObject();
	};
	//------------------------------------------------------------------------------------------------
	class _OgreOdeExport SphereDebugObject:public DebugObject
	{
	public:
		SphereDebugObject(Ogre::Real radius);
		virtual ~SphereDebugObject();
	};
	//------------------------------------------------------------------------------------------------
	class _OgreOdeExport CapsuleDebugObject:public DebugObject
	{
	public:
		CapsuleDebugObject(Ogre::Real radius, Ogre::Real length);
		virtual ~CapsuleDebugObject();
	};
	//------------------------------------------------------------------------------------------------
	class _OgreOdeExport CylinderDebugObject:public DebugObject
	{
	public:
		CylinderDebugObject(Ogre::Real radius, Ogre::Real length);
		virtual ~CylinderDebugObject();
	};
	//------------------------------------------------------------------------------------------------
	class _OgreOdeExport TriangleMeshDebugObject:public DebugObject
	{
	public:
		TriangleMeshDebugObject(int vertex_count);
		virtual ~TriangleMeshDebugObject();

		void beginDefinition();
		void setVertex(int index, const Ogre::Vector3& vertex);
		void endDefinition();
	};
	//------------------------------------------------------------------------------------------------
	class _OgreOdeExport RayDebugObject:public DebugObject
	{
	public:
		RayDebugObject(const Ogre::Vector3& start,const Ogre::Vector3& direction,const Ogre::Real length);
		void setDefinition(const Ogre::Vector3& start,const Ogre::Vector3& direction,const Ogre::Real length);
		virtual ~RayDebugObject();
	};
}

#endif
