
#include "OgreOdePrecompiledHeaders.h"

#include "OgreOdeGeometry.h"
#include "OgreOdeBody.h"
#include "OgreOdeWorld.h"
#include "OgreOdeSpace.h"
#include "OgreOdeStepper.h"

using namespace OgreOde;
using namespace Ogre;

//------------------------------------------------------------------------------------------------
const Ogre::Real cEpsilon = std::numeric_limits<Real>::epsilon();
//------------------------------------------------------------------------------------------------
StepHandler::StepHandler(World *world, 
                 StepModeType stepModeType,
                 const Ogre::Real step_size, 
                 const Ogre::Real max_interval, 
                 const Ogre::Real time_scale) :
    _world  (world),
    _paused (false),
    _listener (0),
    _auto_pre (false),
    _auto_post (false), 
    _root  (0),
    _step_size (step_size),
    _total_time (Ogre::Real(0.0)),
    _max_interval (max_interval),
    _time_scale (time_scale)
{
    switch (stepModeType)
    {
        case BasicStep:
            _current_stepper = new BasicStepMode(world);
            break;
        case QuickStep:
            _current_stepper = new QuickStepMode(world);
            break;
        case FastStep:
            _current_stepper = new FastStepMode(world);
            break;
        case StepModeTypeCount:
        default:
            assert(0);
    }
    assert((step_size > 0)&&("Step size must be greater than zero"));
    assert((max_interval > 0)&&("max interval must be greater than zero"));
}
//------------------------------------------------------------------------------------------------
void StepHandler::pause(bool pause)
{
	_paused = pause;
}
//------------------------------------------------------------------------------------------------
void StepHandler::setStepListener(StepListener* listener)
{
	_listener = listener;
}
//------------------------------------------------------------------------------------------------
bool StepHandler::isPaused()
{
	return _paused;
}
//------------------------------------------------------------------------------------------------
void StepHandler::setAutomatic(StepHandler::AutoMode mode, Root* root)
{
	if ((_root)&&((_auto_pre)||(_auto_post))) 
        _root->removeFrameListener(this);

	_auto_pre = (mode == AutoMode_PreFrame)?true:false;
	_auto_post = (mode == AutoMode_PostFrame)?true:false;

	if ((root)&&((_auto_pre)||(_auto_post)))
	{
		_root = root;
		_root->addFrameListener(this);
	}
}
//------------------------------------------------------------------------------------------------
bool StepHandler::frameStarted(const FrameEvent& evt)
{
	if (_auto_pre)
	{
		if (step (evt.timeSinceLastFrame)) 
			_world->synchronise();
	}
	return true;
}
//------------------------------------------------------------------------------------------------
bool StepHandler::frameEnded(const FrameEvent& evt)
{
	if (_auto_post)
	{
		if (step (evt.timeSinceLastFrame))
			_world->synchronise();
	}
	return true;
}
//------------------------------------------------------------------------------------------------
StepHandler::~StepHandler()
{
	setAutomatic(AutoMode_NotAutomatic);
}
//------------------------------------------------------------------------------------------------
bool StepHandler::basicStep(const Ogre::Real time)
{
    if (_listener && !_listener->preStep(time))
        return false;

    _world->getDefaultSpace()->collide();

    if (_listener && !_listener->middleStep(time))
        return false;

    _current_stepper->step(time);


	_world->clearContacts();

    if (_listener && !_listener->postStep(time))
        return false;

	return true;
}
//------------------------------------------------------------------------------------------------
bool StepHandler::isRunning(const Ogre::Real time)
{
    if (!_paused 
        && time - cEpsilon > 0
        )
    {
       return true;
    }
    return false;
}
//------------------------------------------------------------------------------------------------
bool StepHandler::prepareSteppingTime(const Ogre::Real time)
{
    // make sure time is not too high (alt-tab, debug)
    const Ogre::Real t = (_max_interval - time < cEpsilon) ? _max_interval : time;
    _total_time += t * _time_scale;
    return true;
}
//------------------------------------------------------------------------------------------------
bool StepHandler::step(const Ogre::Real time)
{
	if (isRunning(time) && prepareSteppingTime(time))
	{ 
		if (basicStep(_total_time))
		{
			_world->updateDrawState ();
            _total_time = Ogre::Real(0);
		}
		return true;
	}
	return false;
}
//------------------------------------------------------------------------------------------------
ExactVariableStepHandler::ExactVariableStepHandler(World *world, 
                                                   StepModeType stepModeType,
                                                   const Ogre::Real step_size, 
                                                   const Ogre::Real max_interval, 
                                                   const Ogre::Real time_scale):
    StepHandler(world, stepModeType, step_size,  max_interval, time_scale)
{
}
//------------------------------------------------------------------------------------------------
bool ExactVariableStepHandler::step(const Ogre::Real time)
{
    if (isRunning(time) && prepareSteppingTime(time))
    {
        // Steps in fixed time slices first...
        bool did_run = false;

        while (_total_time - _step_size > cEpsilon)
        {
            if (StepHandler::basicStep(_step_size))
            {
                did_run = true;
                _total_time -= _step_size;
            }
            else 
            {
                break;
            }
        } 

        // Then do a little clean-up step
        if (_total_time > cEpsilon)
        {
            if (StepHandler::basicStep(_total_time))
            {
                did_run = true;
                _total_time = Ogre::Real(0);
            }
        }

        if (did_run)
        {
            _world->updateDrawState ();
        }
        return did_run;
    }
    return false;
}
//------------------------------------------------------------------------------------------------
ExactVariableStepHandler::~ExactVariableStepHandler()
{
}
//------------------------------------------------------------------------------------------------
ForwardFixedStepHandler::ForwardFixedStepHandler(World *world, 
    StepModeType stepModeType,
    const Ogre::Real step_size, 
    const Ogre::Real max_interval, 
    const Ogre::Real time_scale):
    StepHandler(world, stepModeType, step_size, max_interval, time_scale)
{
}
//------------------------------------------------------------------------------------------------
bool ForwardFixedStepHandler::step(const Ogre::Real time)
{
	if (isRunning(time) && prepareSteppingTime(time))
	{
		// Slightly different type of ODE stepper, always makes sure
		// the ODE world is at least level with real-time, and possibly
		// up to one step ahead, seems to make things a bit smoother (?)
		bool did_run = false;

        while (_total_time  > cEpsilon)
        {
            if (StepHandler::basicStep(_step_size))
            {
                did_run = true;
                _total_time -= _step_size;
            }
            else 
            {
                break;
            }
        } 


		if (did_run)
			_world->updateDrawState ();

		return did_run;
	}
	return false;
}
//------------------------------------------------------------------------------------------------
ForwardFixedStepHandler::~ForwardFixedStepHandler()
{
}
//------------------------------------------------------------------------------------------------
ForwardFixedInterpolatedStepHandler::ForwardFixedInterpolatedStepHandler(World *world, 
    StepModeType stepModeType,
    const Ogre::Real step_size, 
    const Ogre::Real frame_rate, 
    const Ogre::Real max_interval, 
    const Ogre::Real time_scale):
        StepHandler(world, stepModeType, step_size, max_interval, time_scale),
        _dbl_step_size(step_size * 2),
        _frame_rate (frame_rate),
        _inv_step_size(1.0 / frame_rate)
{
    if (frame_rate == Ogre::Real(0))
    {
        _fixed_frame_rate = false;
        _next_frame_step_count = 1;
    }
    else
    {
        _fixed_frame_rate = true;
        _next_frame_step_count = _frame_rate / (_step_size * _time_scale);
        if (_next_frame_step_count == 0)
            _next_frame_step_count = 1;
    }
    // compute here the frame_rate adapted to step_size
    _next_total_time = _next_frame_step_count * _step_size;
    _inv_next_total_time = 1 / _next_total_time;

	// Need at least _previous and _current position
	if (_world->getHistorySize() < 2)
		_world->setHistorySize (2);
}
//------------------------------------------------------------------------------------------------
bool ForwardFixedInterpolatedStepHandler::step(const Ogre::Real time)
{
    if (isRunning(time))
	{	
		bool did_run = false;

        // if some time did elapse
        bool should_run = prepareSteppingTime(time);
        // if fixed frame rate.
        // make sure we're waiting until next fixed frame rate stepping.
        if (_fixed_frame_rate && _total_time - _next_total_time < cEpsilon)
            should_run = false;

        if (should_run)
        {
#ifdef _one_step_forward
            // normal steps, without storing results in history
            while (_total_time - _step_size > cEpsilon)
            {
                if (StepHandler::basicStep(_step_size))
                {
                    did_run = true;
                    _total_time -= _step_size;
                }
                else 
                {
                    break;
                }
            }
            if (did_run)
            {
                // last Special Single Step is specific as we save last state prior stepping.
                _world->updatePreviousState ();
                // compute next Frame physic state
                StepHandler::basicStep(_step_size)
                _world->updateCurrentState ();	
            }

#else// _OLD

            // last Special Step is specific as we save last state prior stepping.
            _world->updatePreviousState ();

            // normal steps, without storing results in history
            unsigned int next_frame_step_count = _next_frame_step_count;
            while (next_frame_step_count--)
            {
                if (StepHandler::basicStep(_step_size))
                {
                    did_run = true;
                }
                else 
                {
                    break;
                }
            }

            if (did_run)
                _world->updateCurrentState ();	

            _total_time = std::max(_total_time - _next_total_time, Ogre::Real(0.0));

#endif// _OLD
    		
        }
	    // even if no new physic step happened
        // interpolation may changed its value if time changed...	

	    // interpolate between previousState and currentState;
        // currentState*interpolationTime + previousState * (1.0f - interpolationTime)
        // interpolate 
	    _world->interpolateDrawState(
            Ogre::Real(1.0) -  (_next_total_time - _total_time) * _inv_next_total_time
            //(_total_time - _next_total_time) * _inv_next_total_time
            );
		return true;
	}
	return false;
}
//------------------------------------------------------------------------------------------------
ForwardFixedInterpolatedStepHandler::~ForwardFixedInterpolatedStepHandler()
{
}

