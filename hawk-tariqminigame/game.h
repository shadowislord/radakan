#ifndef __C_GAME__
#define __C_GAME__

#include <ogre.h>

#include "object.h"
#include "globals.h"

using namespace Ogre;

class CGame : public FrameListener
{
    public:
    CGame();
    bool frameStarted(const FrameEvent &evt);
    bool frameEnded(const FrameEvent &evt);

    private:
    CObject *object;

};

#endif
