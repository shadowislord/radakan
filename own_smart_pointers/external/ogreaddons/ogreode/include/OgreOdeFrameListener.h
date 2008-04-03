/*
    PhysicsFrameListener.h

	A very simple wrapper class for ODE
	primarily for the example car application
*/
#ifndef _PHYSICSFRAMELISTENER_H_
#define _PHYSICSFRAMELISTENER_H_

// Uncomment this if you're planning to use the Plane2D joint
// But you'll have to have modified ODE first!
// #define INCLUDE_PLANE2D_JOINT

/*
A physical thing
*/
class PhysicalThing : public RaySceneQueryListener
{
public:
	// Doesn't actually do anything
	PhysicalThing(dSpace *space);
	~PhysicalThing(void);

	// Just create geoms
    void makePlane(Real a,Real b,Real c,Real d);				// Plane
    void makeBox(Real x,Real y,Real z,Real lx,Real ly,Real lz);	// Box
    void makeSphere(Real x,Real y,Real z,Real r);				// Sphere

	// Create bodies and geoms
    void makePlane(dWorld *world,SceneNode *node);
	void makeBox(dWorld *world,SceneNode *node,Real mass = 1,Vector3 const &offs = Vector3(0,0,0));
    void makeSphere(dWorld *world,SceneNode *node,Real mass = 1);
	void makeTerrainTriMesh(dWorld *world,SceneManager *mgr,Real grid_size,Real grid_spacing);

	// Callback functions for the ray scene query functionality
	virtual bool queryResult(SceneQuery::WorldFragment *fragment,Real distance);
	virtual bool queryResult(MovableObject *obj,Real distance);

	// Update the position and orientation of the scene node we're controlling
	void update(void);

	// Utility functions to get at internal stuff
	SceneNode *getNode(void);
	dBodyID getBodyID(void);
	dSpaceID getSpaceID(void);

protected:
	dGeom		*_geom;
	dBody		*_body;
	dSpace		*_space;
	SceneNode	*_node;

	std::list<Vector3> _frag;

	dVector3 *_vertices;
	int *_indices;
};

/*
A Frame listener that implements the physics
*/
class PhysicsFrameListener : public FrameListener
{
protected:
	// All the stuff we need for ODE
    dWorld          *world;
    dHashSpace      *space;
    dJointGroup     *contactgroup;
    dSpace          *current_space;
	bool			paused;

	// Keep track of anything we've created
    std::list<PhysicalThing*>   things;
    std::list<dSimpleSpace*>    spaces;
	std::list<dJoint*>			joints;
    
	// Adjustable variables
    Real total_time,step_size;
	Real k_spring,k_damper,k_tyre,k_mu,k_erp,k_cfm;

	// Utility function to find the physical thing associated with an Ogre scene node
	dBodyID getBodyID(SceneNode *node);

public:
	// Standard constructor/destructor
    PhysicsFrameListener(void);
    ~PhysicsFrameListener(void);

	// Tell us when every frame's about to be started
    bool frameStarted(const FrameEvent& evt);
	void pause(bool p = true);

	// Mainly so we can access internal stuff from the collision callback
    dWorld *getWorld(void){return world;};
    dSpace *getSpace(void){return space;};
    dJointGroup *getContactGroup(void){return contactgroup;};

	// Create things manually, no physical bodies 
	// so they won't move, but things will collide with them
    PhysicalThing *createPlane(Real a,Real b,Real c,Real d);
    PhysicalThing *createBox(Real x,Real y,Real z,Real lx,Real ly,Real lz);
    PhysicalThing *createSphere(Real x,Real y,Real z,Real r);

	// Create objects based on Ogre nodes, these nodes will then
	// come under control of the physics simulation and will move about
	// as if by magic!
	// Except for the plane, which can't actually be a physical body
	// and the trimesh which we're not going to allow to be a physical body
    PhysicalThing *createPlane(SceneNode *node);
	PhysicalThing *createBox(SceneNode *node,Real mass = 1,Vector3 const &offs = Vector3(0,0,0));
    PhysicalThing *createSphere(SceneNode *node,Real mass = 1);
	PhysicalThing *createTerrainTriMesh(SceneManager *mgr,Real grid_size,Real grid_spacing);

	// Create and activate a new space inside the standard hash space
	// Things won't collide within a space so (for example) the objects
	// that make up a car won't collide with each other
    dSimpleSpace *createSpace(void);

	// Create joints between the nodes specified
	// The order of n1/n2 does matter; n1 is what you want attaching
	// and n2 is what you want it attaching to
	dJoint *createHinge2(SceneNode *n1,SceneNode *n2,const Vector3 &suspension = Vector3(0,1,0),const Vector3 &axle = Vector3(1,0,0));
	dJoint *createHinge(SceneNode *n1,SceneNode *n2,const Vector3 &axis = Vector3(1,0,0));
	dJoint *createSlider(SceneNode *n1,SceneNode *n2,const Vector3 &axis = Vector3(0,1,0));
	dJoint *createBall(SceneNode *n1,SceneNode *n2);
#ifdef INCLUDE_PLANE2D_JOINT
	dJoint *createPlane2D(SceneNode *n);
#endif

	// Apply forces to a body under our control
	void addTorque(SceneNode *n,const Vector3 &force);

	// Set the parameters used for the CFM/ERP joint parameters
	void setSuspension(Real spring,Real damper);

	// Set the acceleration due to gravity
	void setGravity(Real g);

	// Get our constants
	Real getTyre(void){return k_tyre;};
	Real getMu(void){return k_mu;};
	Real getCFM(void){return k_cfm;};
	Real getERP(void){return k_erp;};

	// Static function for ODE to call when collisions (potentially) occur
    static void collisionCallback(void *data,dGeomID o1,dGeomID o2);
};

#endif


