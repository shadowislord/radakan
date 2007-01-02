#ifndef _OGREODEBODY_H_
#define _OGREODEBODY_H_

#include "OgreOdePreReqs.h"

namespace OgreOde 
{
	//-----------------------------------------------------------------------
	class _OgreOdeExport BodyState
	{
	public:
		BodyState(){};
		BodyState(const Ogre::Vector3	&position,
				  const Ogre::Quaternion &orientation);

        void operator=(const BodyState &other);
		bool operator==(const BodyState &other) const;
		bool operator!=(const BodyState &other) const;

		//-----------------------------------------------------------------------
		/// compare with another physics state for "significant" differences.
		/// used for detecting position or orientation snaps which need smoothing.
		bool isDifferent(const BodyState &other, const Ogre::Real threshold = Ogre::Real(0.01)) const;

        void interpolate(const BodyState * const _previous_state, 
                                const BodyState * const _current_state, 
                                const Ogre::Real alpha);

		Ogre::Vector3		_position;
		Ogre::Quaternion	_orientation;
	};

	//-----------------------------------------------------------------------
	template <class T>
	class _OgreOdeExport CircularBuffer
	{
	public:

		CircularBuffer() :
			head (0),
			tail (0)
		{
		}

		void resize(const size_t size)
		{
			head = 0;
			tail = 0;
			buffers.resize(size);
		}

		size_t size()
		{
			int count = (int)head - (int)tail;
			if (count<0)
				count += (int) buffers.size();
			return (size_t) count;
		}

		void add(const T &buffer)
		{
			buffers[head] = buffer;
			next(head);

		}

		void remove()
		{
			assert(!empty());
			next(tail);
		}

		T& oldest()
		{
			assert(!empty());
			return buffers[tail];
		}

		T& newest()
		{
			assert(!empty());
			int index = (int)head - 1;
			if (index == -1)
				index = (int) buffers.size() - 1;
			return buffers[index];
		}

		T& almostNewest()
		{
			assert(buffers.size() > 1);
			int index = (int)head - 2;
			if (index == -1)
				index = (int) buffers.size() - 1;
			return buffers[index];
		}

		bool empty() const
		{
			return head == tail;
		}

		void next(size_t &index)
		{
			if (index  == (size_t)buffers.size()) 
				index -= (size_t)buffers.size();
			else
				index ++;
		}

		void previous(size_t &index)
		{			
			if ((int)index - 1 < 0)
				index += (size_t)buffers.size();
			else
				index --;
		}

		T& operator[](size_t index)
		{
			assert(index<(size_t)buffers.size());
			return buffers[index];
		}

	public:

		size_t head;
		size_t tail;

	private:
		std::vector<T> buffers;
	};
    /** 
        This object is the class defining a for all Dynamically physical objects in OgreOde.
    @remarks
        This object is tied attached to a scene node, 
        or/and tied a Entity Object using UserDefinedObject
    @remarks
        It extends the OGRE UserDefinedObject to allow reverse links from Ogre::Entity.
        It extends the OGRE MovableObject to allow scene node attachment.
    */  
	class _OgreOdeExport Body : public Ogre::MovableObject, public Ogre::UserDefinedObject
    {
		friend class Joint;
		friend class World;
		friend class Geometry;

	public:
		Body(World *world, const Ogre::String& name = Ogre::StringUtil::BLANK);
		virtual ~Body();

        static const Ogre::String MovableType;

		void _historyResize(const size_t size);
		inline void updateParentNode();
		inline void updatePreviousState();
		inline void updateCurrentState();
		inline void updateDrawState ();
		inline void interpolateDrawState(const Ogre::Real alpha);
		inline void synchronise();

		void deriveLocation();

		void setPosition(const Ogre::Vector3& position); 
		void setOrientation(const Ogre::Quaternion& orientation); 
		void setLinearVelocity(const Ogre::Vector3& linear_velocity); 
		void setAngularVelocity(const Ogre::Vector3& angular_velocity); 

		inline const Ogre::Vector3& getPosition() const;

		inline const Ogre::Quaternion& getOrientation() const;
		inline void updatePosition(BodyState * const bodystate);
        inline void updateOrientation(BodyState * const bodystate);

		const Ogre::Vector3& getLinearVelocity(); 
		const Ogre::Vector3& getAngularVelocity(); 

		virtual const Ogre::String& getMovableType() const; 
        virtual void _notifyAttached(Ogre::Node* parent,bool isTagPoint = false);
		virtual const Ogre::String& getName(void) const;
		virtual void _notifyCurrentCamera(Ogre::Camera* camera);
		virtual const Ogre::AxisAlignedBox& getBoundingBox(void) const;
		virtual Ogre::Real getBoundingRadius(void) const;
        virtual void _updateRenderQueue(Ogre::RenderQueue* queue);
		
		void setMass(const Mass& mass);
		const Mass& getMass();

		void addForce(const Ogre::Vector3& force); 
		void addTorque(const Ogre::Vector3& torque); 
		void addRelativeForce(const Ogre::Vector3& force); 
		void addRelativeTorque(const Ogre::Vector3& torque); 
		void addForceAt(const Ogre::Vector3& force,const Ogre::Vector3& position); 
		void addForceAtRelative(const Ogre::Vector3& force,const Ogre::Vector3& position); 
		void addRelativeForceAt(const Ogre::Vector3& force,const Ogre::Vector3& position); 
		void addRelativeForceAtRelative(const Ogre::Vector3& force,const Ogre::Vector3& position); 

		const Ogre::Vector3& getForce(); 
        const Ogre::Vector3& getTorque(); 
        void setForce(const Ogre::Vector3&); 
        void setTorque(const Ogre::Vector3&); 


		Ogre::Vector3 getPointWorldPosition(const Ogre::Vector3& position); 
		Ogre::Vector3 getPointWorldVelocity(const Ogre::Vector3& position);
		Ogre::Vector3 getPointVelocity(const Ogre::Vector3& position); 
		Ogre::Vector3 getPointBodyPosition(const Ogre::Vector3& position);
		Ogre::Vector3 getVectorToWorld(const Ogre::Vector3& vector);
		Ogre::Vector3 getVectorFromWorld(const Ogre::Vector3& vector);

		void wake();
		void sleep(); 
		inline bool isAwake() const; 
		void setAutoSleep(bool auto_sleep);
		bool getAutoSleep(); 
		void setAutoSleepLinearThreshold(Ogre::Real linear_threshold);
		Ogre::Real getAutoSleepLinearThreshold(); 
		void setAutoSleepAngularThreshold(Ogre::Real angular_threshold);
		Ogre::Real getAutoSleepAngularThreshold(); 
		void setAutoSleepSteps(int steps);
		int  getAutoSleepSteps(); 
		void setAutoSleepTime(Ogre::Real time);
		Ogre::Real getAutoSleepTime(); 
		void setAutoSleepDefaults();

		void setFiniteRotationMode(bool on); 
		bool getFiniteRotationMode(); 
		void setFiniteRotationAxis(const Ogre::Vector3& axis); 
		const Ogre::Vector3& getFiniteRotationAxis(); 

        int getJointCount(); 
		Joint* getJoint(int index); 

        void addGeometry(Geometry *g); 
        void removeGeometry(Geometry *g); 
        size_t getGeometryCount(); 
        Geometry* getGeometry(int index);
        GeometryArray* getGeometries();

		void setAffectedByGravity(bool on);
		bool getAffectedByGravity(); 

		void setDamping(Ogre::Real linear_damping, Ogre::Real angular_damping);
		void setLinearDamping(Ogre::Real linear_damping);
		void setAngularDamping(Ogre::Real angular_damping);
		Ogre::Real getLinearDamping();
		Ogre::Real getAngularDamping();

		void setUserData(unsigned long user_data);
		unsigned long getUserData();

		virtual unsigned long getID();
		virtual void setDebug(const bool debug);

        /** Return a string identifying the type of user defined object.
        @remarks
            Used to differentiate between different Bodies, Geometries and prefab_object
        */
        const Ogre::String& getTypeName(void) const
        {static Ogre::String sName("Body");return sName;};

        bool collide(void* data, Geometry *g);
        bool collide(void* data, Body *b);
        bool collidePlaneBounds(void* data, Ogre::SceneQuery::WorldFragment *);

	protected:
		dBodyID getBodyID() const;

		void destroyDebugNode();
		void addDebugNode(Ogre::Node* node);

		void recursiveSetMode(Ogre::SceneNode* node);
		
		void applyDamping();

	protected:
		dBodyID	_body;
		Ogre::String _name;
		Ogre::Node* _debug_node;

		static int _body_count;

		Ogre::Vector3 _linear_vel;
		Ogre::Vector3 _angular_vel;
		Ogre::Vector3 _finite_axis;
		Ogre::Vector3 _force;
		Ogre::Vector3 _torque;
		Ogre::AxisAlignedBox _bounding_box;
		Mass* _mass;

		bool _is_damped;
		bool _is_linear_damped;
		dReal _linear_damping;
		bool _is_angular_damped;
		dReal _angular_damping;

		unsigned long _user_data;

		// Major members
		bool _isEnabled;

		BodyState	   _draw_state;
        CircularBuffer<BodyState *> _state_history;

        /// Collision proxies, must be set up if collision enabled
        GeometryArray _geometries;
        
        World *_world;
	};

    //-----------------------------------------------------------------------
    inline bool Body::isAwake() const { return _isEnabled; }
    //-----------------------------------------------------------------------
    inline void Body::updateParentNode()
    {
        if (mParentNode)
        { 
            mParentNode->setPosition(_draw_state._position);
            mParentNode->setOrientation(_draw_state._orientation);
        }

        if (_debug_node)
        {
            _debug_node->setPosition(_draw_state._position);
            _debug_node->setOrientation(_draw_state._orientation);

            recursiveSetMode(static_cast<Ogre::SceneNode*>(_debug_node));
        }
    } 
    //-----------------------------------------------------------------------
    inline void Body::updatePreviousState()
    {
        _isEnabled = dBodyIsEnabled(_body) || _debug_node;
        if (_isEnabled)
        {
            BodyState *previous = _state_history.almostNewest ();
            updatePosition (previous);
            updateOrientation (previous);
        }
    }
    //-----------------------------------------------------------------------
    inline void Body::updateCurrentState()
    {
        _isEnabled = dBodyIsEnabled(_body) || _debug_node;
        if (_isEnabled)
        {
            BodyState *current = _state_history.newest ();
            updatePosition(current);
            updateOrientation(current);
        }
    }
    //-----------------------------------------------------------------------
    inline void Body::updateDrawState ()
    {
        _isEnabled = dBodyIsEnabled(_body) || _debug_node;
        if (_isEnabled)
        {
            updatePosition(&_draw_state);
            updateOrientation(&_draw_state);
        }
    }
    //-----------------------------------------------------------------------
    inline void Body::interpolateDrawState(const Ogre::Real alpha)
    {
        if (_isEnabled)
        {
            BodyState *current = _state_history.newest ();
            BodyState *previous = _state_history.almostNewest ();
            assert (current != previous);
            _draw_state.interpolate (previous, current, alpha);
        }
    }
    //-----------------------------------------------------------------------
    inline void Body::synchronise()
    {
        if (_isEnabled)
        {
            if (_is_damped)
                applyDamping();
            updateParentNode();
        }
    }
    //-----------------------------------------------------------------------
    inline const Ogre::Vector3& Body::getPosition() const
    {
        return _draw_state._position;
    }
    //-----------------------------------------------------------------------
    inline const Ogre::Quaternion& Body::getOrientation() const
    {
        return _draw_state._orientation;
    }
    //-----------------------------------------------------------------------
    inline void Body::updatePosition(BodyState * const bodystate)
    {
        const dReal * const position = dBodyGetPosition(_body);

        bodystate->_position.x = (Ogre::Real)position[0];
        bodystate->_position.y = (Ogre::Real)position[1];
        bodystate->_position.z = (Ogre::Real)position[2];
    }
    //-----------------------------------------------------------------------
    inline void Body::updateOrientation(BodyState * const bodystate)
    {
        const dReal * const orientation = dBodyGetQuaternion(_body); 
        bodystate->_orientation.w = (Ogre::Real)orientation[0];
        bodystate->_orientation.x = (Ogre::Real)orientation[1];
        bodystate->_orientation.y = (Ogre::Real)orientation[2];
        bodystate->_orientation.z = (Ogre::Real)orientation[3];
    }
    //-----------------------------------------------------------------------
}

#endif
