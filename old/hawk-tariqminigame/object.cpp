
#include "object.h"

CObject::CObject(string argName, string filename)
{
    name = argName;
    entity = globals->getScene()->createEntity(argName, filename);
    node = globals->getScene()->getRootSceneNode()->createChildSceneNode();
    node->setPosition(0, 0, 0);
    node->attachObject(entity);
}

CObject::~CObject()
{
    node->detachAllObjects();
}

SceneNode* CObject::getNode()
{
    return node;
}

Entity* CObject::getEntity()
{
    return entity;
}

void CObject::update()
{
}
