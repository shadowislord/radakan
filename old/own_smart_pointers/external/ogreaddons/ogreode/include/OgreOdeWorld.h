#ifndef _OGREODEWORLD_H_
#define _OGREODEWORLD_H_

#include "OgreOdePreReqs.h"
#include "OgreOdeMaintainedList.h"
#include "OgreSingleton.h"

#include <map>

namespace OgreOde 
{
	/** The main class which you will use to simulate your world.
     * 
	 * This is the core class of OgreOde, it is directly analogous to the ODE world except that
	 * in OgreOde you can have only one world, which is why it is Singleton.
	 *
	 * A World is a container for all the bodies and geometries in you simulation. You also
	 * use it to set global options for things like gravity, ERP, CFM and automatic sleeping.
	 * Also, the World is what you step when you want to advance your simulation by a certain time
	 * period.
	 */
	class _OgreOdeExport World
    {
        friend class Body;
		friend class JointGroup;
		friend class Joint;
		friend class Space;
		friend class Geometry;
		friend class StepHandler;

	public:
		/** Construct a new World.
		 * 
		 * You will need to call this to create the World into which you cabn place your physical objects.
		 * The World requires a reference to the scene manager you are using in order to create things
		 * like nodes for debug objects and to obtain configuration options for world geometry.
		 * 
		 * \param manager The scene manager you are using.
		 */
		World(Ogre::SceneManager* manager);
		~World();

		void setGravity(const Ogre::Vector3& gravity = Ogre::Vector3::ZERO);
		const Ogre::Vector3& getGravity();

		void setHistorySize (size_t historySize);
		size_t getHistorySize () const;

		void setERP(Ogre::Real erp = 0.2);
		Ogre::Real getERP();
		void setCFM(Ogre::Real cfm = 10e-5);
		Ogre::Real getCFM();

		void setContactCorrectionVelocity(Ogre::Real velocity = 0.1);
		void setContactSurfaceLayer(Ogre::Real layer = 0.001);

		void setAutoSleep(bool auto_sleep = false);
		bool getAutoSleep();
		void setAutoSleepLinearThreshold(Ogre::Real linear_threshold = 0.01);
		Ogre::Real getAutoSleepLinearThreshold();
		void setAutoSleepAngularThreshold(Ogre::Real angular_threshold = 0.01);
		Ogre::Real getAutoSleepAngularThreshold();
		void setAutoSleepSteps(int steps = 10);
		int getAutoSleepSteps();
		void setAutoSleepTime(Ogre::Real time = 0);
		Ogre::Real getAutoSleepTime(); 
        void setAutoSleepAverageSamplesCount(size_t time = 10);
        size_t getAutoSleepAverageSamplesCount();
		inline void step(Ogre::Real stepsize); 
        inline void quickStep(Ogre::Real stepsize);
        inline void fastStep(Ogre::Real stepsize);

		void setQuickStepIterations(int iterations);
		int getQuickStepIterations();

		Body* findBody(Ogre::SceneNode* node);
		Body* findBody(const Ogre::String& name);

		inline void clearContacts();

		void setCollisionListener(CollisionListener* collision_listener);
		CollisionListener* getCollisionListener();

		void setShowDebugGeometries(bool show);
        bool getShowDebugGeometries(){return _show_debug_geoms;}

        void setShowDebugContact(bool show);
        bool getShowDebugContact(){return _show_debug_contact;}

		inline void notifyGeometry(Body* body);

		Space* getDefaultSpace(){return _default_space;}
		void setDefaultSpace(Space* space);

		inline Ogre::SceneManager* getSceneManager(){return _manager;}

		void setDamping(Ogre::Real linear_damping,Ogre::Real angular_damping);
		Ogre::Real getLinearDamping();
		Ogre::Real getAngularDamping();

		inline void synchronise();
	
		inline void updateDrawState();
		inline void updatePreviousState();
		inline void updateCurrentState();
		inline void interpolateDrawState(const Ogre::Real alpha);


		inline MaintainedList<Body>			&getBodyList()		{return _body_list;}
		inline MaintainedList<Joint>		&getJointList()		{return _joint_list;}
		inline MaintainedList<JointGroup>	&getJointGroupList(){return _joint_group_list;}
		inline MaintainedList<Geometry>		&getGeometryList()	{return _geometry_list;}
		inline MaintainedList<Space>		&getSpaceList()		{return _space_list;}

        inline dWorldID                            getWorldID();
	protected:

        inline dJointGroupID                       getContactGroupID();
		static void                         collisionCallback(void *data,
                                                            dGeomID geom_a,
                                                            dGeomID geom_b);

	protected:
		dWorldID		            _world;
		dJointGroupID	            _contacts;

		Space*		                _default_space;
		
		static CollisionListener*   _collision_listener;

		MaintainedList<Body>		_body_list;
		MaintainedList<Joint>		_joint_list;
		MaintainedList<JointGroup>	_joint_group_list;
		MaintainedList<Geometry>	_geometry_list;
		MaintainedList<Space>		_space_list;

        bool			            _show_debug_geoms;
        bool			            _show_debug_contact;

		Ogre::SceneManager*        	_manager;

		Ogre::Vector3               _gravity;
		dReal	                    _linear_damping,_angular_damping;
		size_t                      _history_size;
	};


    //------------------------------------------------------------------------------------------------
    //INLINED Methods
    //------------------------------------------------------------------------------------------------
    //------------------------------------------------------------------------------------------------
    inline dWorldID World::getWorldID()
    {
        return _world;
    }
    //------------------------------------------------------------------------------------------------
    inline dJointGroupID World::getContactGroupID()
    {
        return _contacts;
    }
    //------------------------------------------------------------------------------------------------
    inline void World::notifyGeometry(Body* body)
    {
        _geometry_list.notify(body);
    }
    //------------------------------------------------------------------------------------------------
    inline void World::synchronise()
    {
        _body_list.synchronise ();
    }
    //------------------------------------------------------------------------------------------------
    inline void World::updateDrawState()
    {
        _body_list.updateDrawState ();
        if (_show_debug_contact)
            _geometry_list.updateDebugContact();
    }
    //------------------------------------------------------------------------------------------------
    inline void World::interpolateDrawState(const Ogre::Real alpha)
    {
        _body_list.interpolateDrawState(alpha);
        if (_show_debug_contact)
            _geometry_list.updateDebugContact();
    }
    //------------------------------------------------------------------------------------------------
    inline void World::updatePreviousState()
    {
        _body_list.updatePreviousState();
    }
    //------------------------------------------------------------------------------------------------
    inline void World::updateCurrentState()
    {
        _body_list.updateCurrentState ();
    }
    //------------------------------------------------------------------------------------------------
    inline void World::clearContacts()
    {
        dJointGroupEmpty(_contacts);
    }
    //------------------------------------------------------------------------------------------------
    inline void World::step(Ogre::Real stepsize)
    {
        dWorldStep(_world,(dReal)stepsize); 
    }
    //------------------------------------------------------------------------------------------------
    inline void World::quickStep(Ogre::Real stepsize)
    {
        dWorldQuickStep(_world,(dReal)stepsize);
    }
    //------------------------------------------------------------------------------------------------
    inline void World::fastStep(Ogre::Real stepsize)
    {
        dWorldStepFast1(_world,(dReal)stepsize,20);
    }
}
#endif
