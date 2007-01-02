#ifndef _OGREODESTEPPER_H_
#define _OGREODESTEPPER_H_

#include "OgreOdePreReqs.h"

namespace OgreOde 
{
    //------------------------------------------------------------------------------------------------
    class _OgreOdeExport StepMode
    {
    public:
        StepMode(World *world) : _world(world){};
        virtual ~StepMode(){};

        virtual void step(const Ogre::Real time) = 0;

    protected:
        World* _world;
    };
    //------------------------------------------------------------------------------------------------
    class _OgreOdeExport BasicStepMode : public StepMode
    {
    public:
        BasicStepMode(World *world) : StepMode(world) {};
        virtual ~BasicStepMode(){};

        inline void step(const Ogre::Real time){ dWorldStep(_world->getWorldID(), (dReal)time);};
    };
    //------------------------------------------------------------------------------------------------
    class _OgreOdeExport QuickStepMode : public StepMode
    {
    public:
        QuickStepMode(World *world) : StepMode(world) {};
        virtual ~QuickStepMode(){};

        inline void step(const Ogre::Real time){ dWorldQuickStep(_world->getWorldID(), (dReal)time);};
    };
    //------------------------------------------------------------------------------------------------
    class _OgreOdeExport FastStepMode : public StepMode
    {
    public:
        FastStepMode(World *world, int max_iteration = 40) : StepMode(world), _max_iteration(max_iteration) {};
        virtual ~FastStepMode(){};

        inline void step(const Ogre::Real time){ dWorldStepFast1(_world->getWorldID(), (dReal)time, _max_iteration);};
    private:
        int _max_iteration;
    };
    //------------------------------------------------------------------------------------------------
	class _OgreOdeExport StepListener
	{
	public:
		StepListener(){};
		virtual ~StepListener(){};

        virtual bool preStep(Ogre::Real time) { return true; };
        virtual bool postStep(Ogre::Real time)  { return true; };
        virtual bool middleStep(Ogre::Real time)  { return true; };
    };
    //------------------------------------------------------------------------------------------------
	class _OgreOdeExport StepHandler:public Ogre::FrameListener
    {
	public:
		enum AutoMode
		{
			AutoMode_NotAutomatic,
			AutoMode_PreFrame,
			AutoMode_PostFrame
        };
        enum StepModeType
        {
            BasicStep = 0,
            QuickStep,
            FastStep,
            StepModeTypeCount
        };

	public:
        StepHandler(World *world, 
            StepModeType stepModeType = QuickStep,
            const Ogre::Real step_size = Ogre::Real (0.01), 
            const Ogre::Real max_interval = Ogre::Real(1.0 / 4), 
            const Ogre::Real time_scale = Ogre::Real(1.0));
		virtual ~StepHandler();

		virtual bool step(const Ogre::Real time);

		Ogre::Real getStepSize() const {return _step_size;}
		void setStepSize (const Ogre::Real step_size){_step_size = step_size;}

		void pause(bool pause);
		bool isPaused();

		void setStepListener(StepListener* listener);
		void setAutomatic(StepHandler::AutoMode mode, Ogre::Root* root = 0);

		bool frameStarted(const Ogre::FrameEvent& evt);
		bool frameEnded(const Ogre::FrameEvent& evt);

    protected:
        bool isRunning(const Ogre::Real time);
        bool prepareSteppingTime(const Ogre::Real time);
        bool basicStep(const Ogre::Real time);

	protected:
		World* _world;
		bool _paused,_auto_pre,_auto_post;
		StepListener* _listener;
		Ogre::Root* _root;
		Ogre::Real _step_size;
        Ogre::Real _total_time;
        Ogre::Real _max_interval;
        Ogre::Real _time_scale;

        StepMode *_current_stepper;
    };

    //------------------------------------------------------------------------------------------------
	class _OgreOdeExport ForwardFixedStepHandler:public StepHandler
	{
	public:
        ForwardFixedStepHandler(World *world, 
            StepModeType stepModeType = QuickStep,
            const Ogre::Real step_size = Ogre::Real (0.01), 
            const Ogre::Real max_interval = Ogre::Real(1.0 / 4), 
            const Ogre::Real time_scale = Ogre::Real(1.0));
		virtual ~ForwardFixedStepHandler();

		virtual bool step(const Ogre::Real time);
    };
    //------------------------------------------------------------------------------------------------
	class _OgreOdeExport ExactVariableStepHandler:public StepHandler
	{
	public:
        ExactVariableStepHandler(World *world ,
            StepModeType stepModeType = QuickStep,
            const Ogre::Real step_size = Ogre::Real (0.01), 
            const Ogre::Real max_interval = Ogre::Real(1.0 / 4), 
            const Ogre::Real time_scale = Ogre::Real(1.0));
		virtual ~ExactVariableStepHandler();

		virtual bool step(const Ogre::Real time);
    };
    //------------------------------------------------------------------------------------------------
	// Fix your timestep Gaffer implementation.
	// http://www.gaffer.org/articles/Timestep.html
	// Gaffer :"If you implement this interpolation technique you ensure that there will
	// not be any visual stuttering when your display and physics framerates 
	// are out of sync. It will also perfectly handle the undersampling case so 
	// your objects will move smoothly when you view your simulation in slow 
	// motion or ten years from now on that Sexium." 
	class _OgreOdeExport ForwardFixedInterpolatedStepHandler :public StepHandler
	{
	public:
        ForwardFixedInterpolatedStepHandler(World *world, 
            StepModeType stepModeType = QuickStep,
            const Ogre::Real step_size = Ogre::Real (0.01), 
            const Ogre::Real frame_rate = Ogre::Real (1.0 / 60),
            const Ogre::Real max_interval = Ogre::Real(1.0 / 4), 
            const Ogre::Real time_scale = Ogre::Real(1.0));
		virtual ~ForwardFixedInterpolatedStepHandler();

		virtual bool step(const Ogre::Real time);
    private :
        Ogre::Real _dbl_step_size;
        Ogre::Real _inv_step_size;

        Ogre::Real _next_total_time;
        unsigned int _next_frame_step_count;
        Ogre::Real _inv_next_total_time;
        bool _fixed_frame_rate;
        Ogre::Real _frame_rate;
	};

}

#endif
