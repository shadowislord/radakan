#ifndef _OGREODEENTITYINFORMER_H_
#define _OGREODEENTITYINFORMER_H_

#include "OgreOdePreReqs.h"

namespace OgreOde 
{
    class _OgreOdeExport EntityInformer
    {
	public:
		EntityInformer(Ogre::Entity *entity,const Ogre::Matrix4 &transform = Ogre::Matrix4::IDENTITY);
		EntityInformer();
		~EntityInformer();

		void addEntity(Ogre::Entity *entity,const Ogre::Matrix4 &transform = Ogre::Matrix4::IDENTITY);
		// Cannot be animated. 
		void addMesh(const Ogre::MeshPtr &mesh, const Ogre::Matrix4 &transform = Ogre::Matrix4::IDENTITY);

		Ogre::Real getRadius();
		Ogre::Vector3 getSize();

		Body* createSingleDynamicSphere(Ogre::Real mass, World *world, Space* space = 0);
		Body* createSingleDynamicBox(Ogre::Real mass, World *world, Space* space = 0);

		TriangleMeshGeometry* createStaticTriangleMesh(World *world, Space* space = 0);
		BoxGeometry* createSingleStaticBox(World *world, Space* space = 0);

		CapsuleGeometry* createOrientedCapsule(unsigned char bone, World *world, Space* space = 0);
		BoxGeometry* createOrientedBox(unsigned char bone, World *world, Space* space = 0);
		BoxGeometry* createAlignedBox(unsigned char bone, World *world, Space* space = 0);

		const Ogre::Vector3* getVertices();
		unsigned int getVertexCount();
		const unsigned int* getIndices();
		unsigned int getIndexCount();

	protected:
        void addVertexData(const Ogre::VertexData *vertex_data, 
            const Ogre::VertexData *blended_data = 0, 
            const Ogre::Mesh::IndexMap *indexMap = 0);
		void addIndexData(Ogre::IndexData *data, const unsigned int offset = 0);
		bool getBoneVertices(unsigned char bone,unsigned int &vertex_count, Ogre::Vector3* &vertices);

		Ogre::Entity*		_entity;
		Ogre::SceneNode*	_node;
		Ogre::Matrix4		_transform;

		Ogre::Real		_radius;
		Ogre::Vector3		_size;

        Ogre::Vector3*	   _vertices;
		unsigned int*  _indices;
		unsigned int   _vertex_count;
		unsigned int   _index_count;


		BoneMapping *_bone_mapping;
	};
}

#endif


