#include "tslrpg.hpp"

using namespace std;

Tslrpg::
	Tslrpg ():
	Object ("Tslrpg")
{
}

Tslrpg::
	~Tslrpg ()
{
}

//	virtual
void
	Tslrpg::
	createCamera ()
{
	mCamera = mSceneMgr->createCamera("PlayerCam");
	mCamera->setPosition(Vector3(0,0,0));
	mCamera->lookAt(Vector3(0,0,-300));
	mCamera->setNearClipDistance(5);
	mCamera->setFarClipDistance(2000);
}

//	virtual
void Tslrpg::createScene ()
{
	Entity * ent = mSceneMgr->createEntity("ogre", "fort.mesh");
	//ent->setMaterialName("metal_plate");
	SceneNode* node = mSceneMgr->getRootSceneNode()->createChildSceneNode();
	node->attachObject(ent);
	node->setPosition(0,0,0);

	for (int i = 0; i < 100; i++)
	{
		char name[50];
		sprintf(name, "cluster_%d", i);
		BillboardSet* bbs2 = mSceneMgr->createBillboardSet(name, 1);
		bbs2->setBillboardRotationType(BBR_VERTEX);
		SceneNode* bbsNode2 = mSceneMgr->getRootSceneNode()->createChildSceneNode();
		
		char type[50];
		int ran = ((int)(Math::RangeRandom(0, 1)*100))%8+1;
		sprintf(type, "spacebillboard/cluster_%d", ran);
		bbs2->setMaterialName(type);
		bbs2->getMaterial()->setTextureFiltering(TFO_TRILINEAR);
		bbs2->getMaterial()->setDepthCheckEnabled(true);
		int px = Math::RangeRandom(-2500, 2500);
		int py = Math::RangeRandom(-2500, 2500);
		int pz = Math::RangeRandom(-2500, 2500);
		px = 0;
		Billboard* bb = bbs2->createBillboard(0, 0, 0, ColourValue());
		int dim = Math::RangeRandom(1.5f, 2);
		bb->setDimensions(100*dim, 100*dim);
		bb->setRotation(Radian(Math::RangeRandom(0, 2*Math::PI)));
		bbsNode2->attachObject(bbs2);
		bbsNode2->setPosition(px, py, pz);
	}
	for(int i = 0; i < 200; i++)
	{
		char name[50];
		sprintf(name, "star_%d", i);
		BillboardSet* bbs2 = mSceneMgr->createBillboardSet(name, 1);
		bbs2->setBillboardRotationType(BBR_VERTEX);
		SceneNode* bbsNode2 = mSceneMgr->getRootSceneNode()->createChildSceneNode();
		
		char type[50];
		int ran = ((int)(Math::RangeRandom(0, 1)*100))%2+1;
		sprintf(type, "spacebillboard/star_%d", ran);
		bbs2->setMaterialName(type);
		bbs2->getMaterial()->setTextureFiltering(TFO_TRILINEAR);
		bbs2->getMaterial()->setDepthCheckEnabled(true);
		int px = Math::RangeRandom(-2500, 2500);
		int py = Math::RangeRandom(-2500, 2500);
		int pz = Math::RangeRandom(-2500, 2500);
		px = 0;
		Billboard* bb = bbs2->createBillboard
		(
			0, 0, 0,
			ColourValue
			(
				Math::RangeRandom (0.6f,1),
				Math::RangeRandom (0.6f,1),
				Math::RangeRandom (0.6f,1)
			)
		);
		int dim = Math::RangeRandom(1.5f, 2);
		bb->setDimensions(5*dim, 5*dim);
		bb->setRotation(Radian(Math::RangeRandom(0, 2*Math::PI)));
		bbsNode2->attachObject (bbs2);
		bbsNode2->setPosition (px, py, pz);
	}
}

void Tslrpg::createFrameListener ()
{
	mFrameListener = new FrameLstr (mSceneMgr, mWindow, mCamera);
	mRoot->addFrameListener (mFrameListener);
}
