#ifndef __C_STATE__
#define __C_STATE__

template<class entity_type>
class CState
{
    public:
    virtual void Enter(entity_type*)=0;
    virtual void Execute(entity_type*)=0;
    virtual void Exit(entity_type*)=0;
    virtual ~CState() {}
};

#endif
