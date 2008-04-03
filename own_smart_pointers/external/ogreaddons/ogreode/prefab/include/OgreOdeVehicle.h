#ifndef _OGREODEVEHICLE_H_
#define _OGREODEVEHICLE_H_

#include "OgreOde_Prefab.h"

#include <vector>

namespace OgreOde_Prefab 
{

    class _OgreOdeExport_Prefab Vehicle : public Object
    {
	public:
		class _OgreOdeExport_Prefab Engine
		{
		public:
			Engine();
			~Engine();

            void setInputs(Ogre::Real throttle_position, Ogre::Real brake_position);
            void setInputs(Ogre::Real throttle_brake);
			void setInputs(bool throttle, bool brake);
			
			Ogre::Real getBrakeForce();
			Ogre::Real getPowerAtRPM(Ogre::Real rpm);
			Ogre::Real getDesiredRPM();

			void update(Ogre::Real time);

			// Manual gearbox
			void changeUp();
			void changeDown();
			void setGear(char code);
			void setGear(unsigned int i);

			// Information functions
			Ogre::Real getRevFactor();
			char getGearCode();
		
			// Engine parameters
			void setRevLimit(Ogre::Real rpm);
			void setTorque(Ogre::Real* torque,unsigned int samples);
			void setTorque(Ogre::Real constant);
			void setTorque(Ogre::Real minimum, Ogre::Real maximum);

			// Braking parameters
			void setBrakeForce(Ogre::Real force);

			// Drivetrain parameters
			void addGear(Ogre::Real ratio, char code);
            void setFinalDrive(Ogre::Real ratio);
			void setAutoShiftRevs(Ogre::Real up, Ogre::Real down);
			void setAutoBox(bool automatic);

		protected:
			std::vector<std::pair<char,Ogre::Real> >	_gears;
			unsigned int					        	_current_gear_num;
			std::pair<char,Ogre::Real>		        	_current_gear;
			Ogre::Real*						        	_torque_curve;
			unsigned int					        	_curve_length;
			Ogre::Real						        	_rev_limit,_throttle_position,_brake_position,_wheel_rpm,_inv_rev_limit;
			Ogre::Real						        	_brake_force;
			Ogre::Real						        	_final_drive;
			Ogre::Real						        	_shift_up,_shift_down;
			bool							        	_auto_box;
		};

		class _OgreOdeExport_Prefab Wheel : public Object
		{
		public:
			static const Ogre::Real FudgeFactor;

            Wheel(Vehicle* vehicle,
                const Ogre::String& name,
                const Ogre::String& mesh,
                const Ogre::Vector3& position, 
                Ogre::Real mass, 
                OgreOde::World *world, 
                OgreOde::Space* space);

			~Wheel();

			void setFactors(Ogre::Real steer, Ogre::Real power, Ogre::Real brake);
			void setContact(Ogre::Real bouncyness, Ogre::Real friction, Ogre::Real fds)
			{
				_bouncyness = bouncyness;
				_friction = friction;
				_fds = fds;
			}

			void setSteerFactor(Ogre::Real factor);
			void setSteerLimit(Ogre::Real limit);
			void setSteerForce(Ogre::Real force){_steer_force = force;}
			void setSteerSpeed(Ogre::Real speed){_steer_speed = speed;}
			void setSteer(Ogre::Real position);
			
			void setPowerFactor(Ogre::Real power){_power_factor = power;}
			void setBrakeFactor(Ogre::Real brake){_brake_factor = brake;}

			void setSuspension(Ogre::Real spring_constant, Ogre::Real damping_constant, Ogre::Real step_size);

			const Ogre::Vector3& getPosition();
			const Ogre::Quaternion& getOrientation();
			void setPosition(const Ogre::Vector3& position);

			Ogre::Real getRPM();
			Ogre::Real getSteer();

			void update(Ogre::Real power_force, Ogre::Real desired_rpm, Ogre::Real brake_force);
	
            Ogre::SceneNode* getSceneNode(){return _node;}
			Ogre::Entity*	getEntity(){return _entity;}
            OgreOde::Body* getBody(){return _body;}

			OgreOde::Geometry* getGeometry(){return _geometry;}

            Ogre::Real getRadius()const{return _radius;}

			void snapOff();

			void setupTyreContact(OgreOde::Contact* contact);

		protected:
			void adjustJointParameters();

			Ogre::SceneNode*				_node;
			Ogre::Entity*					_entity;
			Ogre::Real				    	_steer_factor,_steer_limit,_steer_force,_steer_speed;
			Ogre::Real				    	_power_factor;
			Ogre::Real				    	_brake_factor;
			Ogre::Real				    	_spring,_damping,_step;
			Ogre::Real				    	_bouncyness,_friction,_fds;
			Ogre::Real				    	_radius;
            OgreOde::Body*					_body;
            OgreOde::Geometry*				_geometry;
            OgreOde::SuspensionJoint*		_joint;
            Ogre::String					_name;
			Vehicle*				        _owner;
		};

	public:
        Vehicle(const Ogre::String& name,const Ogre::String& mesh, Ogre::Real mass, 
            OgreOde::World *world, 
            OgreOde::Space* space,
            const Ogre::Vector3& offset = Ogre::Vector3::ZERO,
            OgreOde::Geometry* geometry = 0);

        Vehicle(const Ogre::String& name, 
                OgreOde::World *world, 
                OgreOde::Space* space = 0,
                OgreOde::Geometry* geometry = 0);
		~Vehicle();

        Vehicle::Wheel* addWheel(const Ogre::String& mesh,const Ogre::Vector3& position, Ogre::Real mass);
		
		const Ogre::Vector3& getPosition();
		const Ogre::Quaternion& getOrientation();
		void setPosition(const Ogre::Vector3& position);

		void setSuspension(Ogre::Real spring_constant, Ogre::Real damping_constant, Ogre::Real step_size);
		
		void setInputs(Ogre::Real steering, Ogre::Real throttle,Ogre::Real brake);
		void setInputs(Ogre::Real steering, Ogre::Real throttle_brake);
		void setInputs(bool left, bool right, bool throttle, bool brake);

		void setSteering(Ogre::Real steering_position);

		const Ogre::String&	getName(){return _name;}

		virtual void		applyAntiSwayBarForces();
		void				update(Ogre::Real time);

		Ogre::Entity*				getEntity(){return _entity;}
		void				        setEntity(Ogre::Entity *e){ _entity = e;}

        OgreOde::Body*				getBody(){return _body;}

		Ogre::SceneNode*			getSceneNode(){return _node;}

		Vehicle::Wheel*		getWheel(unsigned int i){return _wheels[i];}
		size_t				getWheelCount()const{return _wheels.size();}

		OgreOde::TransformGeometry*	getTransformGeometry(){return _transform;}
		void				setTransformGeometry(OgreOde::TransformGeometry*g){_transform = g;}

        void				setTransNode(Ogre::SceneNode *s){_trans_node = s;}
		Ogre::SceneNode*			getTransNode(){return _trans_node;}

        OgreOde::Geometry*			getGeometry(){return _geometry;}
		void				setGeometry(OgreOde::Geometry*g){_geometry = g;}

		Engine*				getEngine(){return _engine;}

		Vehicle::Wheel*		findWheel(OgreOde::Geometry* geometry);

		OgreOde::Space*		getSpace(){return _space;}

        Ogre::Vector3				getOffset(){return _offset;}
		void				setOffset(const Ogre::Vector3 &of)
		{
			_offset = of;
			if (_geometry) 
				_geometry->setPosition(of);
			_trans_node->setPosition(of);
		}

		Ogre::Real getVelocity();
		
		static bool handleTyreCollision(OgreOde::Contact* contact);

		void load(const Ogre::String &filename, const Ogre::String &definition_name = Ogre::StringUtil::BLANK);

		bool isAntiSwayEnabled()const {return _antisway;};
		void enableAntiSway(bool enable){_antisway = enable;};

		void  setSwayForce(Ogre::Real swayForce){_swayForce = swayForce;};
		void  setSwayForceLimit(unsigned int swayForceLimit){_swayForceLimit = swayForceLimit;}
		void  setSwayForceRate(Ogre::Real swayRate){_swayRate = swayRate;}

		Ogre::Real		  getSwayForce (Ogre::Real swayForce) const{return _swayForce;};
		unsigned int  getSwayForceLimit(unsigned int swayForceLimit) const{return _swayForceLimit;}
		Ogre::Real          getSwayForceRate(Ogre::Real swayRate) const{return _swayRate;}

	protected:
		Ogre::SceneNode*						_node;
		Ogre::SceneNode*						_trans_node;
		Ogre::Entity*							_entity;
        OgreOde::Body*							_body;
        OgreOde::TransformGeometry*				_transform;
        OgreOde::Geometry*						_geometry;
		std::vector<Vehicle::Wheel*>	_wheels;
		Ogre::String							_name;
        OgreOde::SimpleSpace*					_space;
        Ogre::Vector3							_offset;
		Ogre::Vector3							_lastPosition;
		Engine*							_engine;
		
		bool							_antisway;
		Ogre::Real							_swayForce;
		unsigned int					_swayForceLimit;
		Ogre::Real							_swayRate;
		Ogre::Real							_swayLastUpdate;
	};
}

#endif

