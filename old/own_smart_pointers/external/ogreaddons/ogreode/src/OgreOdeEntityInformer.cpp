
#include "OgreOdePrecompiledHeaders.h"

#include "OgreOdeEntityInformer.h"
#include "OgreOdeMaintainedList.h"
#include "OgreOdeBody.h"
#include "OgreOdeGeometry.h"
#include "OgreOdeMass.h"
#include "OgreOdeEigenSolver.h"

using namespace OgreOde;
using namespace Ogre;

//------------------------------------------------------------------------------------------------
void EntityInformer::addVertexData(const VertexData *vertex_data,
                                   const VertexData *blended_data,
                                   const Mesh::IndexMap *indexMap)
{
	if (!vertex_data) 
        return;

	const VertexData *data = (blended_data) ? blended_data: vertex_data;

	const unsigned int prev_size = _vertex_count;
    _vertex_count += (unsigned int)data->vertexCount;

    Ogre::Vector3* tmp_vert = new Ogre::Vector3[_vertex_count];
	if (_vertices)
	{
		memcpy(tmp_vert,_vertices,sizeof(Vector3) * prev_size);
		delete[] _vertices;
	}
	_vertices = tmp_vert;

	// Get the positional buffer element
    {	
        const Ogre::VertexElement* posElem = data->vertexDeclaration->findElementBySemantic(Ogre::VES_POSITION);			
        Ogre::HardwareVertexBufferSharedPtr vbuf = data->vertexBufferBinding->getBuffer(posElem->getSource());
        const unsigned int vSize = (unsigned int)vbuf->getVertexSize();

	    unsigned char* vertex = static_cast<unsigned char*>(vbuf->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));
	    float* pReal;
        Ogre::Vector3 * curVertices = &_vertices[prev_size];
        const unsigned int vertexCount = (unsigned int)data->vertexCount;
	    for(unsigned int j = 0; j < vertexCount; ++j)
	    {
		    posElem->baseVertexPointerToElement(vertex, &pReal);
            vertex += vSize;

		    curVertices->x = (*pReal++);
		    curVertices->y = (*pReal++);
		    curVertices->z = (*pReal++);

		    *curVertices = _transform * (*curVertices);
            
            curVertices++;
        }
	    vbuf->unlock();
    }

	// Get the bone index element
	if (_entity->hasSkeleton())
	{

        Ogre::MeshPtr mesh = _entity->getMesh ();

		const Ogre::VertexElement* bneElem = vertex_data->vertexDeclaration->findElementBySemantic(Ogre::VES_BLEND_INDICES);
		assert (bneElem);
		{
            Ogre::HardwareVertexBufferSharedPtr vbuf = vertex_data->vertexBufferBinding->getBuffer(bneElem->getSource());
            const unsigned int vSize = (unsigned int)vbuf->getVertexSize();
			unsigned char* vertex = static_cast<unsigned char*>(vbuf->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));

			unsigned char* pBone;

            if (!_bone_mapping)
                _bone_mapping = new BoneMapping();	
            BoneMapping::iterator i;

            Ogre::Vector3 * curVertices = &_vertices[prev_size];
            
            const unsigned int vertexCount = (unsigned int)vertex_data->vertexCount;
			for(unsigned int j = 0; j < vertexCount; ++j)
			{
				bneElem->baseVertexPointerToElement(vertex, &pBone);
                vertex += vSize;
               
                const unsigned char currBone = (indexMap) ? (*indexMap)[*pBone] : *pBone;
				i = _bone_mapping->find (currBone);
				Vector3Array* l = 0;
				if (i == _bone_mapping->end())
				{
					l = new Vector3Array;
					_bone_mapping->insert(BoneMappingKey(currBone, l));
				}						
				else 
                {
                    l = i->second;
                }

				l->push_back(*curVertices);

                curVertices++;
			}
			vbuf->unlock();
		}
	}
}


//------------------------------------------------------------------------------------------------
void EntityInformer::addIndexData(IndexData *data, const unsigned int offset)
{
    const unsigned int prev_size = _index_count;
    _index_count += (unsigned int)data->indexCount;

	unsigned int* tmp_ind = new unsigned int[_index_count];
	if (_indices)
	{
		memcpy (tmp_ind, _indices, sizeof(unsigned int) * prev_size);
		delete[] _indices;
	}
	_indices = tmp_ind;

	const unsigned int numTris = (unsigned int) data->indexCount / 3;
	HardwareIndexBufferSharedPtr ibuf = data->indexBuffer;	
	const bool use32bitindexes = (ibuf->getType() == HardwareIndexBuffer::IT_32BIT);
    unsigned int index_offset = prev_size;

	if (use32bitindexes) 
    {
        const unsigned int* pInt = static_cast<unsigned int*>(ibuf->lock(HardwareBuffer::HBL_READ_ONLY));
        for(unsigned int k = 0; k < numTris; ++k)
        {
            _indices[index_offset ++] = offset + *pInt++;
            _indices[index_offset ++] = offset + *pInt++;
            _indices[index_offset ++] = offset + *pInt++;
        }
        ibuf->unlock();
    }
	else 
    {
        const unsigned short* pShort = static_cast<unsigned short*>(ibuf->lock(HardwareBuffer::HBL_READ_ONLY));
		for(unsigned int k = 0; k < numTris; ++k)
        {
            _indices[index_offset ++] = offset + static_cast<unsigned int> (*pShort++);
            _indices[index_offset ++] = offset + static_cast<unsigned int> (*pShort++);
            _indices[index_offset ++] = offset + static_cast<unsigned int> (*pShort++);
        }
        ibuf->unlock();
    }

}
//------------------------------------------------------------------------------------------------
void EntityInformer::addEntity(Entity *entity,const Matrix4 &transform)
{
	// Each entity added need to reset size and radius
	// next time getRadius and getSize are asked, they're computed.
	_size  = Ogre::Vector3(-1,-1,-1);
	_radius = -1;

	_entity = entity;
	_node = (SceneNode*)(_entity->getParentNode());
	_transform = transform;

	const bool isSkeletonAnimated = _entity->hasSkeleton();
	//const bool isHWanimated = isSkeletonAnimated && entity->isHardwareAnimationEnabled();
	if (isSkeletonAnimated)
	{
		_entity->addSoftwareAnimationRequest(false);
		_entity->_updateAnimation();
	}


	if (_entity->getMesh()->sharedVertexData)
	{
		if (!isSkeletonAnimated)
			addVertexData (_entity->getMesh()->sharedVertexData);
		else
			addVertexData (_entity->getMesh()->sharedVertexData, 
			_entity->_getSkelAnimVertexData(),
			&_entity->getMesh()->sharedBlendIndexToBoneIndexMap); 
	}

	for(unsigned int i = 0;i < _entity->getNumSubEntities();++i)
	{
		SubMesh *sub_mesh = _entity->getSubEntity(i)->getSubMesh();

		if (!sub_mesh->useSharedVertices)
		{
			addIndexData(sub_mesh->indexData,_vertex_count);

			if (!isSkeletonAnimated)
				addVertexData (sub_mesh->vertexData);
			else
				addVertexData (sub_mesh->vertexData, 
				_entity->getSubEntity(i)->_getSkelAnimVertexData(),
				&sub_mesh->blendIndexToBoneIndexMap); 
		}
		else 
		{
			addIndexData (sub_mesh->indexData);
		}

	}

	if (isSkeletonAnimated) 
		_entity->removeSoftwareAnimationRequest(false);
}
//------------------------------------------------------------------------------------------------
void EntityInformer::addMesh(const MeshPtr &mesh, const Matrix4 &transform)
{
	// Each entity added need to reset size and radius
	// next time getRadius and getSize are asked, they're computed.
	_size  = Ogre::Vector3(-1,-1,-1);
	_radius = -1;

	//_entity = entity;
	//_node = (SceneNode*)(_entity->getParentNode());
	_transform = transform;

	if (mesh->hasSkeleton ())
		Ogre::LogManager::getSingleton().logMessage("EntityInformer::addMesh : Mesh " + mesh->getName () + " as skeleton but added to trimesh non animated");
// 		OGRE_EXCEPT(1, "Mesh " + mesh->getName () + " as skeleton but added to trimesh non animated", 
// 					"EntityInformer::addMesh");

	if (mesh->sharedVertexData)
	{
		addVertexData (mesh->sharedVertexData);
	}

	for(unsigned int i = 0;i < mesh->getNumSubMeshes();++i)
	{
		SubMesh *sub_mesh = mesh->getSubMesh(i);

		if (!sub_mesh->useSharedVertices)
		{
			addIndexData(sub_mesh->indexData, _vertex_count);
			addVertexData (sub_mesh->vertexData);
		}
		else 
		{
			addIndexData (sub_mesh->indexData);
		}

	}
}
//------------------------------------------------------------------------------------------------
EntityInformer::EntityInformer(Entity *entity,const Matrix4 &transform) :
	_vertices (0),
	_indices (0),
	_vertex_count (0),
	_index_count (0),
	_size (Vector3(-1,-1,-1)),
	_radius (-1),
	_bone_mapping (0)
{
	addEntity(entity, transform);	
}
//------------------------------------------------------------------------------------------------
EntityInformer::EntityInformer() :
		_vertices (0),
		_indices (0),
		_vertex_count (0),
		_index_count (0),
		_size (Vector3(-1,-1,-1)),
		_radius (-1),
        _bone_mapping (0)
{
	
}
//------------------------------------------------------------------------------------------------
Real EntityInformer::getRadius()
{
	if (_radius == (-1))
	{
		getSize();
		_radius = (std::max(_size.x,std::max(_size.y,_size.z)) * 0.5);
	}
	return _radius;
}
//------------------------------------------------------------------------------------------------
Vector3 EntityInformer::getSize()
{
    const unsigned int vCount = getVertexCount();
	if (_size == Ogre::Vector3(-1,-1,-1) && vCount > 0)
	{

		const Ogre::Vector3 * const v = getVertices();

        Ogre::Vector3 vmin(v[0]);
        Ogre::Vector3 vmax(v[0]);

		for(unsigned int j = 1; j < vCount; j++)
		{
			vmin.x = std::min(vmin.x, v[j].x);
			vmin.y = std::min(vmin.y, v[j].y);
			vmin.z = std::min(vmin.z, v[j].z);

			vmax.x = std::max(vmax.x, v[j].x);
			vmax.y = std::max(vmax.y, v[j].y);
			vmax.z = std::max(vmax.z, v[j].z);
		}

		_size.x = vmax.x - vmin.x;
		_size.y = vmax.y - vmin.y;
		_size.z = vmax.z - vmin.z;
	}

	return _size;
}
//------------------------------------------------------------------------------------------------
const Ogre::Vector3* EntityInformer::getVertices()
{
	return _vertices;
}
//------------------------------------------------------------------------------------------------
unsigned int EntityInformer::getVertexCount()
{
	return _vertex_count;
}
//------------------------------------------------------------------------------------------------
const unsigned int* EntityInformer::getIndices()
{
	return _indices;
}
//------------------------------------------------------------------------------------------------
unsigned int EntityInformer::getIndexCount()
{
	return _index_count;
}
//------------------------------------------------------------------------------------------------
Body* EntityInformer::createSingleDynamicSphere(Real mass, World *world, Space* space)
{
	const Ogre::Real rad = getRadius();

	assert((rad > 0.0) && 
        ("Sphere radius must be greater than zero"));

	Body* body = new Body(world, "OgreOde::Body_" + _entity->getName());
	body->setMass(SphereMass(mass, rad));

	SphereGeometry* geom = new SphereGeometry(rad, world, space);
	geom->setBody(body);

	body->setPosition(_node->getPosition());
	body->setOrientation(_node->getOrientation());

	_node->attachObject(body);

	return body;
}
//------------------------------------------------------------------------------------------------
Body* EntityInformer::createSingleDynamicBox(Real mass, World *world, Space* space)
{
	const Ogre::Vector3 sz = getSize();

	assert((sz.x > 0.0) && (sz.y > 0.0) && (sz.y > 0.0) && 
        ("Size of box must be greater than zero on all axes"));

	Body* body = new Body(world, "OgreOde::Body_" + _node->getName());
	body->setMass(BoxMass(mass,sz));

	BoxGeometry* geom = new BoxGeometry(sz, world, space);
	geom->setBody(body);

	body->setPosition(_node->getPosition());
	body->setOrientation(_node->getOrientation());

	_node->attachObject(body);

	return body;
}
//------------------------------------------------------------------------------------------------
TriangleMeshGeometry* EntityInformer::createStaticTriangleMesh(World *world, Space* space)
{
	assert(_vertex_count && (_index_count >= 6) && 
        ("Mesh must have some vertices and at least 6 indices (2 triangles)"));

	return new TriangleMeshGeometry(_vertices, _vertex_count,_indices, _index_count, world, space);
}
//------------------------------------------------------------------------------------------------
BoxGeometry* EntityInformer::createSingleStaticBox(World *world, Space* space)
{
	BoxGeometry* geom = new BoxGeometry(getSize(), world, space);

	geom->setPosition(_node->getPosition());
	geom->setOrientation(_node->getOrientation());

	return geom;
}
//------------------------------------------------------------------------------------------------
bool EntityInformer::getBoneVertices(unsigned char bone, unsigned int &vertex_count, Ogre::Vector3* &vertices)
{
	BoneMapping::iterator i = _bone_mapping->find(bone);
	
    if (i == _bone_mapping->end()) 
        return false;

	if (i->second->empty()) 
        return false;

    vertex_count = (unsigned int) i->second->size() + 1;

	vertices = new Ogre::Vector3[vertex_count];
	vertices[0] = _entity->_getParentNodeFullTransform() * 
            _entity->getSkeleton()->getBone(bone)->_getDerivedPosition();

	unsigned int o = 1;
	for(Vector3Array::iterator j = i->second->begin();
        j != i->second->end(); ++j,++o) 
    {
        vertices[o] = (*j);
    }       
	return true;
}
//------------------------------------------------------------------------------------------------
BoxGeometry* EntityInformer::createAlignedBox(unsigned char bone, World *world, Space* space)
{
	unsigned int vertex_count;
	Vector3* vertices;
	if (!getBoneVertices(bone, vertex_count, vertices)) 
        return 0;

	Vector3 min_vec(vertices[0]);
	Vector3 max_vec(vertices[0]);

	for(unsigned int j = 1; j < vertex_count ;j++)
	{
		min_vec.x = std::min(min_vec.x,vertices[j].x);
		min_vec.y = std::min(min_vec.y,vertices[j].y);
		min_vec.z = std::min(min_vec.z,vertices[j].z);

		max_vec.x = std::max(max_vec.x,vertices[j].x);
		max_vec.y = std::max(max_vec.y,vertices[j].y);
		max_vec.z = std::max(max_vec.z,vertices[j].z);
	}
    const Ogre::Vector3 maxMinusMin(max_vec - min_vec);
    BoxGeometry* box = new BoxGeometry(maxMinusMin, world, space);

    const Ogre::Vector3 pos(min_vec.x + (maxMinusMin.x * 0.5),
                      min_vec.y + (maxMinusMin.y * 0.5),
                      min_vec.z + (maxMinusMin.z * 0.5));

	box->setPosition(pos);

	delete[] vertices;

	return box;
}
//------------------------------------------------------------------------------------------------
BoxGeometry* EntityInformer::createOrientedBox(unsigned char bone, World *world, Space* space)
{
	unsigned int vertex_count;
	Vector3* vertices;
	if (!getBoneVertices(bone, vertex_count, vertices)) 
        return 0;

	Vector3 box_kCenter;
    Ogre::Vector3 box_akAxis[3];
   Ogre::Real box_afExtent[3];

	EigenSolver::GaussPointsFit (vertex_count, vertices, box_kCenter, box_akAxis, box_afExtent);

    // Let C be the box center and let U0, U1, and U2 be the box axes.  Each
    // input point is of the form X = C + y0*U0 + y1*U1 + y2*U2.  The
    // following code computes min(y0), max(y0), min(y1), max(y1), min(y2),
    // and max(y2).  The box center is then adjusted to be
    //   C' = C + 0.5*(min(y0)+max(y0))*U0 + 0.5*(min(y1)+max(y1))*U1 +
    //        0.5*(min(y2)+max(y2))*U2

    Ogre::Vector3 kDiff (vertices[0] - box_kCenter);
   Ogre::Real fY0Min = kDiff.dotProduct(box_akAxis[0]), fY0Max = fY0Min;
   Ogre::Real fY1Min = kDiff.dotProduct(box_akAxis[1]), fY1Max = fY1Min;
   Ogre::Real fY2Min = kDiff.dotProduct(box_akAxis[2]), fY2Max = fY2Min;

    for (unsigned int i = 1; i < vertex_count; i++)
    {
        kDiff = vertices[i] - box_kCenter;

        const Ogre::Real fY0 = kDiff.dotProduct(box_akAxis[0]);
        if ( fY0 < fY0Min )
            fY0Min = fY0;
        else if ( fY0 > fY0Max )
            fY0Max = fY0;

        const Ogre::Real fY1 = kDiff.dotProduct(box_akAxis[1]);
        if ( fY1 < fY1Min )
            fY1Min = fY1;
        else if ( fY1 > fY1Max )
            fY1Max = fY1;

        const Ogre::Real fY2 = kDiff.dotProduct(box_akAxis[2]);
        if ( fY2 < fY2Min )
            fY2Min = fY2;
        else if ( fY2 > fY2Max )
            fY2Max = fY2;
    }

    box_afExtent[0] = ((Real)0.5)*(fY0Max - fY0Min);
    box_afExtent[1] = ((Real)0.5)*(fY1Max - fY1Min);
    box_afExtent[2] = ((Real)0.5)*(fY2Max - fY2Min);

    box_kCenter += (box_afExtent[0])*box_akAxis[0] +
                   (box_afExtent[1])*box_akAxis[1] +
                   (box_afExtent[2])*box_akAxis[2];

	BoxGeometry *geom = new BoxGeometry(Vector3(box_afExtent[0] * 2.0,
                                                box_afExtent[1] * 2.0,
                                                box_afExtent[2] * 2.0),
                                        world, space);
	geom->setOrientation(Quaternion(box_akAxis[0],box_akAxis[1],box_akAxis[2]));
	geom->setPosition(box_kCenter);
	return geom;
}
//------------------------------------------------------------------------------------------------
CapsuleGeometry* EntityInformer::createOrientedCapsule(unsigned char bone, World *world, Space* space)
{
	unsigned int vertex_count;
	Vector3* vertices;

	if (!getBoneVertices(bone,vertex_count,vertices)) 
        return 0;

	Vector3 cap_orig;
	Vector3 cap_dir;
	Real cap_rad;

	Vector3 line_orig;
	Vector3 line_dir;

	EigenSolver::orthogonalLineFit (vertex_count, vertices, line_orig, line_dir);

   Ogre::Real fMaxRadiusSqr = (Real) 0.0;
    unsigned int c;
    for (c = 0; c < vertex_count; c++)
    {
		const Ogre::Real fRadiusSqr = EigenSolver::SqrDistance(vertices[c], line_orig, line_dir);
        if ( fRadiusSqr > fMaxRadiusSqr ) 
            fMaxRadiusSqr = fRadiusSqr;
    }

    Ogre::Vector3 kU, kV, kW = line_dir;
	EigenSolver::GenerateOrthonormalBasis (kU, kV, kW, true);

   Ogre::Real fMin = FLT_MAX, fMax = -fMin;
    for (c = 0; c < vertex_count; c++)
    {
        const  Ogre::Vector3 kDiff (vertices[c] - line_orig);
        const Ogre::Real fU = kU.dotProduct (kDiff);
        const Ogre::Real fV = kV.dotProduct (kDiff);
        const Ogre::Real fW = kW.dotProduct (kDiff);
        const Ogre::Real fDiscr = fMaxRadiusSqr - (fU*fU + fV*fV);
        const Ogre::Real fRadical = sqrtf(fabs(fDiscr));

        fMin = std::min(fW + fRadical, fMin);
        fMax = std::max(fW - fRadical, fMax);
    }

    if ( fMin < fMax )
    {
        cap_orig = line_orig + fMin*line_dir;
        cap_dir = (fMax-fMin)*line_dir;
    }
    else
    {
        // enclosing capsule is really a sphere
        cap_orig = line_orig + (((Real)0.5)*(fMin+fMax))*line_dir;
        //----------------------------
        cap_dir = Ogre::Vector3::ZERO;
        //cap_dir = (fMax-fMin)*line_dir;
    }

    cap_rad = sqrtf(fMaxRadiusSqr);


    CapsuleGeometry* geom = 0;
    const Ogre::Real cap_dirLength = cap_dir.length();
    if (cap_rad > 0 && cap_dirLength > 0)
    {
        const Ogre::Vector3 orig_axis (Vector3::UNIT_Z);
        const Ogre::Vector3 reqd_axis (cap_dir.normalisedCopy());
        const Ogre::Vector3 rot_axis (orig_axis.crossProduct(reqd_axis));
        const Ogre::Real cos_ang = orig_axis.dotProduct(reqd_axis);
       Ogre::Real ang = acos(cos_ang);
        if (cos_ang < 0.0) 
            ang -= M_PI;

        const Ogre::Quaternion orient = Ogre::Quaternion(Radian(ang),rot_axis);

        geom = new CapsuleGeometry(cap_rad, cap_dirLength, world, space);
        geom->setOrientation(orient);
        geom->setPosition(cap_orig + (reqd_axis * (cap_dirLength * 0.5)));
    }
	return geom;
}
//------------------------------------------------------------------------------------------------
EntityInformer::~EntityInformer()
{
	delete[] _vertices;
	delete[] _indices;

	if (_bone_mapping)
	{
		for(BoneMapping::iterator i = _bone_mapping->begin();
            i != _bone_mapping->end();
            ++i)
		{
			delete i->second;
		}
		delete _bone_mapping;
	}
}
