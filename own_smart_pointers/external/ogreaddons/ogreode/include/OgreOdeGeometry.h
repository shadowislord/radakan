#ifndef _OGREODEGEOMETRY_H_
#define _OGREODEGEOMETRY_H_

#include "OgreOdePreReqs.h"
#include "OgreOdeMaintainedList.h"

namespace OgreOde 
{
    /** 
    This object is the base class for all Geometry objects in OgreOde.
    @remarks
        This object is tied either to a Body (acting as collision proxy for a physic object) 
        or to a Entity Object (which then is physically "static")
    @remarks
        It extends the OGRE UserDefinedObject to allow reverse links from Ogre::Entity.
        Note that this class does not override the UserDefinedObject's getTypeId method 
        because this class is abstract.
    */  
    class _OgreOdeExport Geometry : public Ogre::UserDefinedObject
	{
		friend class Space;
        friend class World;
        friend class Body;
		friend class TransformGeometry;

	public:
		enum Class
		{
			Class_Sphere		= dSphereClass,
			Class_Box			= dBoxClass,
			Class_Capsule		= dCapsuleClass,
			Class_Cylinder		= dCCylinderClass,
			Class_InfinitePlane = dPlaneClass,
			Class_Ray			= dRayClass,
			Class_TriangleMesh	= dTriMeshClass,
			Class_Terrain		= dHeightfieldClass,
			Class_Convex		= dConvexClass,
			Class_Transform		= dGeomTransformClass,
			Class_NoGeometry
		};

	public:
        Geometry(World *world,  Space* space = 0);
		virtual ~Geometry();

		virtual void                        setBody(Body* body);
		Body*                               getBody(); 


		virtual void                        setPosition(const Ogre::Vector3& position);
		virtual void                        setOrientation(const Ogre::Quaternion& orientation);	
		virtual const Ogre::Vector3&        getPosition();
		virtual const Ogre::Quaternion&     getOrientation();
		virtual const Ogre::AxisAlignedBox& getAxisAlignedBox(); 


		Space*                              getSpace(); 

		void                                 enable();
		void                                 disable();
		bool                                 isEnabled(); 

		Geometry::Class                     getClass() const; 

        void                                 setCategoryBitfield(unsigned long bits); 
        void                                 setCollisionBitfield(unsigned long bits); 
		unsigned long                         getCategoryBitfield(); 
		unsigned long                         getCollisionBitfield(); 

		int                                 collide(Geometry* geometry, CollisionListener* listener = 0);

		void                                setMaxContacts(unsigned int max_contacts);
		unsigned int                        getMaxContacts() const;
		unsigned int                        getContactHighWaterMark() const;

		void                                setEncapsulator(Geometry* encapsulator);
		Geometry*                           getEncapsulator(){return _encapsulator;}

		void                                setUserData(unsigned long user_data){_user_data = user_data;}
		unsigned long                       getUserData() const {return _user_data;}

		void                                setUserObject(void* object){_user_object = object;}
		void*                               getUserObject(){return _user_object;}

		virtual unsigned long               getID() const ;
		virtual void                        notify(Body* body);


        virtual void                        setDebug(const bool debug);
        virtual void                        createDebugObject();
        virtual void                        destroyDebugObject();
        void                                reparentDebugObject(Ogre::Node* node);

        virtual void                        setDebugContact(const bool debug);
        void                                updateDebugContact();

		void                                clearOffset();
		int                                 isOffset();

		void                                setOffsetPosition (const Ogre::Vector3 &position);
		void                                setOffsetQuaternion(const Ogre::Quaternion &orientation);

		 void                               setOffsetWorldPosition(const Ogre::Vector3 &position);		
		 void                               setOffsetWorldQuaternion(const Ogre::Quaternion &orientation) ;

		 Ogre::Vector3                      getOffsetPosition() ;
		 Ogre::Quaternion                   getOffsetQuaternion() ;

	protected:
		dGeomID                             getGeometryID() const;
		void                                registerGeometry();
		dSpaceID                            getSpaceID(Space* space) const;

	protected:
		dGeomID                             _geom;
		unsigned int                        _max_contacts;
        unsigned int                        _contact_high_water_mark;
        unsigned int                        _last_contact_num;
		dContact*                           _contacts;

		DebugObject*                        _debug_obj;
        Ogre::Node*                         _debug_node;
        DebugContact**                      _debug_contacts;

		Geometry*                           _encapsulator;
		Ogre::AxisAlignedBox                _bounding_box;
		Ogre::Quaternion                    _orientation;
		Ogre::Vector3                       _position;

		static int                          _geometry_count;
		unsigned long                       _user_data;
		void*                               _user_object;
        World                               *_world;

	};

	//------------------------------------------------------------------------------------------------
    class _OgreOdeExport SphereGeometry:public Geometry
	{
	public:
		SphereGeometry(Ogre::Real radius, World *world, Space* space = 0);
		~SphereGeometry();

		virtual void createDebugObject();

		void setRadius(Ogre::Real radius);
		Ogre::Real getRadius();
        Ogre::Real getPointDepth(const Ogre::Vector3& point);  
        /** Return a string identifying the type of user defined object.
           @remarks
           Used to differentiate between different Bodies, Geometries and prefab_object
        */
        const Ogre::String& getTypeName(void) const
        {static Ogre::String sName("Geometry Sphere");return sName;};

	};

	//------------------------------------------------------------------------------------------------
	class _OgreOdeExport BoxGeometry:public Geometry
	{
	public:
		BoxGeometry(const Ogre::Vector3& size,  World *world, Space* space = 0);
		~BoxGeometry();

		virtual void createDebugObject();

		void setSize(const Ogre::Vector3& size);
		const Ogre::Vector3& getSize();
		Ogre::Real getPointDepth(const Ogre::Vector3& point);

        /** Return a string identifying the type of user defined object.
        @remarks
        Used to differentiate between different Bodies, Geometries and prefab_object
        */
        const Ogre::String& getTypeName(void) const
        {static Ogre::String sName("Geometry Box");return sName;};

	protected:
		Ogre::Vector3 _size;
	};

	//------------------------------------------------------------------------------------------------
	class _OgreOdeExport InfinitePlaneGeometry:public Geometry
	{
	public:
		InfinitePlaneGeometry(const Ogre::Plane& plane, World *world, Space* space = 0);
		~InfinitePlaneGeometry();

		void setDefinition(const Ogre::Plane& plane);
		const Ogre::Plane& getDefinition();
		Ogre::Real getPointDepth(const Ogre::Vector3& point);

		void setPosition(const Ogre::Vector3& position);
		void setOrientation(const Ogre::Quaternion& orientation);	
		const Ogre::Vector3& getPosition();
		const Ogre::Quaternion& getOrientation();
		const Ogre::AxisAlignedBox& getAxisAlignedBox(); 

        /** Return a string identifying the type of user defined object.
        @remarks
        Used to differentiate between different Bodies, Geometries and prefab_object
        */
        const Ogre::String& getTypeName(void) const
        {static Ogre::String sName("Geometry Infinity Plane");return sName;};

	protected:
		Ogre::Plane _plane;
    };
    //------------------------------------------------------------------------------------------------
	class _OgreOdeExport CapsuleGeometry:public Geometry
	{
	public:
		CapsuleGeometry(Ogre::Real radius, Ogre::Real length, World *world, Space* space = 0);
		~CapsuleGeometry();

		virtual void createDebugObject();
 
		void setDefinition(Ogre::Real radius, Ogre::Real length);
		Ogre::Real getRadius();
		Ogre::Real getLength();
		Ogre::Real getPointDepth(const Ogre::Vector3& point);


        /** Return a string identifying the type of user defined object.
        @remarks
        Used to differentiate between different Bodies, Geometries and prefab_object
        */
        const Ogre::String& getTypeName(void) const
        {static Ogre::String sName("Geometry Capsule");return sName;};

	};
	//------------------------------------------------------------------------------------------------
	class _OgreOdeExport CylinderGeometry:public Geometry
	{
	public:
		CylinderGeometry(Ogre::Real radius, Ogre::Real length, World *world, Space* space = 0);
		~CylinderGeometry();

		virtual void createDebugObject();

		void setDefinition(Ogre::Real radius, Ogre::Real length);
		Ogre::Real getRadius();
		Ogre::Real getLength();
		Ogre::Real getPointDepth(const Ogre::Vector3& point);

        /** Return a string identifying the type of user defined object.
        @remarks
        Used to differentiate between different Bodies, Geometries and prefab_object
        */
        const Ogre::String& getTypeName(void) const
        {static Ogre::String sName("Geometry Cylinder");return sName;};

	};
	//------------------------------------------------------------------------------------------------
	class _OgreOdeExport RayGeometry:public Geometry
	{
	public:
		RayGeometry(Ogre::Real length, World *world, Space* space = 0);
		~RayGeometry();

		virtual void createDebugObject();
		
		void setLength(Ogre::Real length);
		Ogre::Real getLength();
		void setDefinition(const Ogre::Vector3& start, const Ogre::Vector3& direction);
		const Ogre::Vector3& getStart();
		const Ogre::Vector3& getDirection();

        /** Return a string identifying the type of user defined object.
        @remarks
        Used to differentiate between different Bodies, Geometries and prefab_object
        */
        const Ogre::String& getTypeName(void) const
        {static Ogre::String sName("Geometry Ray");return sName;};

	protected:
		Ogre::Vector3 _direction,_start;
	};

	//------------------------------------------------------------------------------------------------
	class _OgreOdeExport TransformGeometry:public Geometry
	{
	public:
		TransformGeometry(World *world, Space* space);
		~TransformGeometry();

		virtual void createDebugObject();
		virtual void destroyDebugObject();

		void setEncapsulatedGeometry(Geometry* geometry);
		Geometry* getEncapsulatedGeometry() const;

        /** Return a string identifying the type of user defined object.
        @remarks
        Used to differentiate between different Bodies, Geometries and prefab_object
        */
        const Ogre::String& getTypeName(void) const
        {static Ogre::String sName("Geometry Transform");return sName;};

	};
	//------------------------------------------------------------------------------------------------
	class _OgreOdeExport TriangleMeshCollisionListener
	{
	public:
		TriangleMeshCollisionListener(){}
		virtual ~TriangleMeshCollisionListener(){}

		virtual bool collide(TriangleMeshGeometry* triangle_mesh, Geometry* geometry, int triangle) {return true;}
	};

	//------------------------------------------------------------------------------------------------
	class _OgreOdeExport TriangleMeshIntersectionListener
	{
	public:
		TriangleMeshIntersectionListener(){}
		virtual ~TriangleMeshIntersectionListener(){}

		virtual void intersect(TriangleMeshGeometry* triangle_mesh, Geometry* geometry, const int* triangles, int triangle_count){}
	};

	//------------------------------------------------------------------------------------------------
	class _OgreOdeExport TriangleMeshRayListener
	{
	public:
		TriangleMeshRayListener(){}
		virtual ~TriangleMeshRayListener(){}

		virtual bool collide(TriangleMeshGeometry* triangle_mesh, RayGeometry* ray,int triangle, const Ogre::Vector3& uv){return true;}
	};

	//------------------------------------------------------------------------------------------------
	struct _OgreOdeExport TriangleMeshTriangle
	{
		Ogre::Vector3 v0,v1,v2;
	};

	//------------------------------------------------------------------------------------------------
	class _OgreOdeExport TriangleMeshGeometry:public Geometry
	{
	public:
		TriangleMeshGeometry(const Ogre::Vector3* vertices, 
                             unsigned int vertex_count, 
                             const unsigned int* indices,
                             unsigned int index_count,
                             World *world, 
                             Space* space = 0);
		~TriangleMeshGeometry();

		Ogre::Vector3 getPoint(unsigned int index, const Ogre::Vector3& uv);
		TriangleMeshTriangle getTriangle(int index);

		void clearTemporalCoherenceCache(); 
		void enableTemporalCoherence(Geometry::Class geometry_class,bool enable); 
		bool isTemporalCoherenceEnabled(Geometry::Class geometry_class);

		static int _collisionCallback(dGeomID mesh,dGeomID object,int triangle);
		void setCollisionListener(TriangleMeshCollisionListener* collision_listener);

		static void _intersectionCallback(dGeomID mesh,dGeomID object,const int* triangles,int triangle_count);
		void setIntersectionListener(TriangleMeshIntersectionListener* intersection_listener); 

		static int _rayCallback(dGeomID mesh,dGeomID ray,int triangle,dReal u,dReal v);
		void setRayListener(TriangleMeshRayListener* ray_listener); 

		virtual void createDebugObject();

        /** Return a string identifying the type of user defined object.
        @remarks
        Used to differentiate between different Bodies, Geometries and prefab_object
        */
        const Ogre::String& getTypeName(void) const
        {static Ogre::String sName("Geometry Trimesh");return sName;};

	protected:
		dTriMeshDataID _data;
		dVector3* _vertices;
		unsigned int* _indices;
		unsigned int _vertex_count;
        unsigned int _index_count;

		TriangleMeshCollisionListener* _collision_listener;
		TriangleMeshIntersectionListener* _intersection_listener;
		TriangleMeshRayListener* _ray_listener;
	};

	//------------------------------------------------------------------------------------------------
	class _OgreOdeExport ConvexGeometry:public Geometry
	{
	public:
		ConvexGeometry(const Ogre::Vector3* vertices, 
						unsigned int vertex_count, 
						const unsigned int* indices,
						unsigned int index_count,
						World *world, 
                        Space* space = 0);
		~ConvexGeometry();


		virtual void createDebugObject();

        /** Return a string identifying the type of user defined object.
        @remarks
        Used to differentiate between different Bodies, Geometries and prefab_object
        */
        const Ogre::String& getTypeName(void) const
        {static Ogre::String sName("Geometry Convex");return sName;};

	protected:
		dReal	* _vertices;
		unsigned int* _indices;
		unsigned int _vertex_count;
		unsigned int _index_count;
	};

	//------------------------------------------------------------------------------------------------
	class _OgreOdeExport TerrainGeometryHeightListener
	{
	public:
		TerrainGeometryHeightListener(){}
		virtual ~TerrainGeometryHeightListener(){}

		virtual Ogre::Real heightAt(const Ogre::Vector3& position) = 0;
	};

	//------------------------------------------------------------------------------------------------
	class _OgreOdeExport TerrainGeometry:public Geometry, public Ogre::RaySceneQueryListener
	{
	public:
        TerrainGeometry(World *world, 
                        Space* space,
                        const Ogre::Vector3 &scale,
                        int nodes_per_sideX,
                        int nodes_per_sideY,
                        Ogre::Real worldSizeX,
                        Ogre::Real worldSizeZ,
                        bool centered,
                        Ogre::Real thickness = 10.0f);

		~TerrainGeometry();

		Ogre::Real getPointDepth(const Ogre::Vector3& point);
		const Ogre::Vector3& getPosition();
		const Ogre::Quaternion& getOrientation();
		
		static dReal _heightCallback(void* data, dReal x, dReal z);
		static dReal _heightCallback(void* data, int x, int z);
		void setHeightListener(TerrainGeometryHeightListener* listener);

        inline Ogre::Real getHeightAt(const Ogre::Vector3& position)
        {
            _num_query++;
            const Ogre::Vector3 pos (position.x * _sample_width - (_centered?_halfWorldSizeX:0),
                position.y,
                position.z * _sample_height - (_centered?_halfWorldSizeZ:0)); 
            _ray.setOrigin(pos);
            _ray_query->setRay(_ray);
            _distance_to_terrain = static_cast<Ogre::Real>(0.0);
            _ray_query->execute(this);
            return position.y - _distance_to_terrain;
        }

		bool queryResult(Ogre::MovableObject *obj, Ogre::Real distance);
		bool queryResult(Ogre::SceneQuery::WorldFragment *fragment, Ogre::Real distance);

        size_t getNumQueries() const {return _num_query;};
        void resetNumQueries(){_num_query = 0;};

        /** Return a string identifying the type of user defined object.
        @remarks
        Used to differentiate between different Bodies, Geometries and prefab_object
        */
        const Ogre::String& getTypeName(void) const
        {static Ogre::String sName("Geometry Terrain");return sName;};

	protected:
		Ogre::Real _max_height;
		Ogre::Real _distance_to_terrain;
		Ogre::Real _sample_width, _sample_height;
		TerrainGeometryHeightListener* _listener;
		Ogre::Ray _ray;
		Ogre::RaySceneQuery* _ray_query;
        bool _centered;
        Ogre::Real _halfWorldSizeX; 
        Ogre::Real _halfWorldSizeZ;
        size_t _num_query;
    };
    //------------------------------------------------------------------------------------------------
    class _OgreOdeExport PlaneBoundedRegionGeometryPlaneListener
    {
    public:
        PlaneBoundedRegionGeometryPlaneListener(){}
        virtual ~PlaneBoundedRegionGeometryPlaneListener(){}

        virtual std::list<Ogre::Plane>* planesAt(const Ogre::Vector3& position) = 0;
    };

    //------------------------------------------------------------------------------------------------
    class _OgreOdeExport PlaneBoundedRegionGeometry:public Geometry, public Ogre::RaySceneQueryListener
    {
    public:
        PlaneBoundedRegionGeometry(World *world, 
                                    Space* space,
                                    const Ogre::AxisAlignedBox &scale);

        ~PlaneBoundedRegionGeometry();

        Ogre::Real getPointDepth(const Ogre::Vector3& point);
        const Ogre::Vector3& getPosition();
        const Ogre::Quaternion& getOrientation();

        static std::list<Ogre::Plane> * _planeCallback(void* data, dReal x, dReal z);
        static std::list<Ogre::Plane> * _planeCallback(void* data, int x, int z);
        void setPlaneListener(PlaneBoundedRegionGeometryPlaneListener* listener);

        inline std::list<Ogre::Plane> *planesAt(const Ogre::Vector3& position)
        {
            _num_query++;
//             const Ogre::Vector3 pos (position.x * _sample_width - (_centered?_halfWorldSizeX:0),
//                                     position.y,
//                                     position.z * _sample_height - (_centered?_halfWorldSizeZ:0)); 
//             _ray.setOrigin(pos);
//             _ray_query->setRay(_ray);
//             _distance_to_terrain = static_cast<Ogre::Real>(0.0);
//             _ray_query->execute(this);

            return _last_query_results;

        }

        bool queryResult(Ogre::MovableObject *obj, Ogre::Real distance);
        bool queryResult(Ogre::SceneQuery::WorldFragment *fragment, Ogre::Real distance);

        size_t getNumQueries() const {return _num_query;};
        void resetNumQueries(){_num_query = 0;};

        /** Return a string identifying the type of user defined object.
        @remarks
        Used to differentiate between different Bodies, Geometries and prefab_object
        */
        const Ogre::String& getTypeName(void) const
        {static Ogre::String sName("Geometry Plane Bounded Region");return sName;};

    protected:
        Ogre::Real _max_height;
        Ogre::Real _distance_to_terrain;
        Ogre::Real _sample_width, _sample_height;
        PlaneBoundedRegionGeometryPlaneListener* _listener;
        Ogre::Ray _ray;
        Ogre::RaySceneQuery* _ray_query;
        bool _centered;
        Ogre::Real _halfWorldSizeX; 
        Ogre::Real _halfWorldSizeZ;
        size_t _num_query;

        std::list<Ogre::Plane> *_last_query_results;
    };
}

#endif
