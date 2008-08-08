
#include "game.h"

CGame::CGame()
{
   // Load resources
   ResourceGroupManager *rgm = ResourceGroupManager::getSingletonPtr();
   rgm->addResourceLocation("models/attack", "FileSystem", "Models", true);
   rgm->addResourceLocation("models/fighter", "FileSystem", "Models", true);
   rgm->addResourceLocation("models/quad", "FileSystem", "Models", true);
   rgm->initialiseAllResourceGroups();

   // Set up basic lights
   globals->getScene()->setAmbientLight(ColourValue(1.0, 1.0, 1.0));

   // Misc
   object = new CObject("player", "attack.mesh");
   object->getNode()->setPosition(0, 0, 200);
}

bool CGame::frameStarted(const FrameEvent &evt)
{
    return true;
}

bool CGame::frameEnded(const FrameEvent &evt)
{
    return true;
}
