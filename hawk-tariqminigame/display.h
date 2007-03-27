#ifndef __C_DISPLAY__
#define __C_DISPLAY__

#include <iostream>
#include <ogre.h>

#include "globals.h"

using namespace Ogre;
using namespace std;

class CDisplay
{
    public:
    /// Sets the current screen resolution
    /// and creates a new window.
    CDisplay(int width, int height, bool fullScreen);

};

#endif
