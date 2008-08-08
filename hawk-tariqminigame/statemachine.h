#ifndef __C_STATE_MACHINE__
#define __C_STATE_MACHINE__

#include <windows.h>
#include "state.h"

template <class entity_type>
class CStateMachine
{
    private:
    entity_type* owner;

    CState<entity_type>* currentState;
    CState<entity_type>* previousState;
    CState<entity_type>* globalState;

    public:
    CStateMachine(entity_type* argOwner):owner(argOwner),
                                        currentState(NULL),
                                        previousState(NULL),
                                        globalState(NULL)
    {
    }


    void setCurrentState(CState<entity_type>* s)
    {
        currentState = s;
    }

    void setGlobalState(CState<entity_type>* s)
    {
        globalState = s;
    }

    void setPreviousState(CState<entity_type>* s)
    {
        previousState = s;
    }

    void update() const
    {
        if(globalState) globalState->Execute(owner);
        if(currentState) currentState->Execute(owner);
    }

    void changeState(CState<entity_type>* newState)
    {
        previousState = currentState;
        currentState->Exit(owner);
        currentState = newState;
        currentState->Enter(owner);
    }

    void revertToPreviousState()
    {
        ChangeState(previousState);
    }

    CState<entity_type>* getCurrentState() const
    {
        return currentState;
    }

    CState<entity_type>* getGlobalState() const
    {
        return globalState;
    }

    CState<entity_type>* getPreviousState() const
    {
        return previousState;
    }

    bool isInState(const CState<entity_type>& st) const
    {
        return false;
    }
};

#endif
