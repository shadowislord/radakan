
#include "input_engine.hpp"

Input_Engine::Input_Engine () : Engine("Input Engine")
{
}


Input_Engine::~Input_Engine ()
{
}


bool Input_Engine::is_initialized () const
{
    return Engine::is_initialized();
}


Event* Input_Engine::process (Event * event)
{
    return NULL;
}
