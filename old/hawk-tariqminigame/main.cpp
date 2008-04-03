
#include <windows.h>
#include <ogre.h>

#include "display.h"
#include "globals.h"
#include "game.h"

using namespace Ogre;


int main(void)
{
    CDisplay *display = new CDisplay(800, 600, false);

    CGame game;
    globals->getRoot()->addFrameListener(&game);
    globals->getRoot()->startRendering();

    delete display;
    return 0;
}
