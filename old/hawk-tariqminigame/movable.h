
#ifndef __C_MOVEABLE_CLASS__
#define __C_MOVEABLE_CLASS__

#include <ogre.h>
#include <string>

#include "object.h"

using namespace Ogre;
using namespace std;

class CMovable : public CObject
{
    public:
    CMovable(string argName, string filename) : CObject(argName, filename)
    {}

    private:
};

#endif

