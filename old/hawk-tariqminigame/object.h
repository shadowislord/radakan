#ifndef __C_OBJECT_CLASS__
#define __C_OBJECT_CLASS__

#include <ogre.h>
#include <string>

#include "globals.h"

using namespace std;
using namespace Ogre;

class CObject
{
    public:
    CObject(string argName, string filename);
    ~CObject();

    SceneNode* getNode();
    Entity* getEntity();

    virtual void update();

    private:
    Entity *entity;
    SceneNode *node;

    string name;
};

#endif
