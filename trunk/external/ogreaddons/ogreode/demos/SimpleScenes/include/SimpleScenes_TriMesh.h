/*
SimpleScenes_TriMesh.h
---------------------
A reimplementation of the ODE triangle mesh collision
demo using Ogre and the OgreOde wrapper.
*/
#ifndef _SIMPLESCENES_TRIMESH_H_
#define _SIMPLESCENES_TRIMESH_H_

/*
The box stacking test extends the box stacking demo, but adds a trimesh!
*/
class SimpleScenes_TriMesh:public SimpleScenes_BoxStack
{
public:
	// Standard constructor, but create a triangle mesh
    SimpleScenes_TriMesh(OgreOde::World *world) :
	      SimpleScenes_BoxStack (world)
	{
		// Create the visual entity and scene node
		Entity* entity = _mgr->createEntity("Landscape","landscape.mesh");

		SceneNode* node = _mgr->getRootSceneNode()->createChildSceneNode(entity->getName());
		node->attachObject(entity);
		node->setScale(Vector3(0.4,0.3,0.4));
		node->setPosition(Vector3(0,3,0));
        node->setOrientation(Quaternion(Degree(30),Vector3::UNIT_Y));

        entity->setNormaliseNormals(true);
		//entity->setCastShadows(false);
		
		// Turn it into a triangle mesh static geometry
		OgreOde::EntityInformer ei(entity,node->_getFullTransform());
        OgreOde::Geometry *geom = ei.createStaticTriangleMesh(_world, _space);
        entity->setUserObject (geom);
        _geoms.push_back(geom);
	}

	// Destructor, manually destroy the entity and node, since they're 
	// not associated with a body they won't get deleted automatically
	virtual ~SimpleScenes_TriMesh()
	{
		_mgr->destroySceneNode("Landscape");
		_mgr->destroyEntity("Landscape");
	}

	// Return our name for the test application to display
	virtual const String& getName()
	{
		static String name = "Test Triangle Mesh";
		return name;
	}
};

#endif
