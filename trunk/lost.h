#ifndef __lost_h_
#define __lost_h_

#include "Ogre.h"
#include "ExampleApplication.h"
#include <string>

class LFrameListener: public ExampleFrameListener{
	private:
		SceneManager* mSceneMgr;
		float totaltime;
		Vector3 vec;

	public:
		LFrameListener(SceneManager *sceneMgr, RenderWindow* win, Camera* cam): ExampleFrameListener(win, cam), mSceneMgr(sceneMgr){
			mSceneMgr = sceneMgr;
			totaltime = 0;
		}

		bool frameStarted(const FrameEvent& evt){
			totaltime += evt.timeSinceLastFrame;
			for(int i = 0; i < 2000; i++){
				char name[50];
				sprintf(name, "star_%d", i);
			 	BillboardSet* bbs = mSceneMgr->getBillboardSet(name);
				Billboard* temp = bbs->getBillboard(0);
				ColourValue col = temp->getColour();
				float m = abs(sin(totaltime));
				m -= Math::RangeRandom(0, m);
				if(m < 0.8){
					m = 0.8;
				}
				col.r = m;
				m -= Math::RangeRandom(0, m);
				col.g = m;
				m -= Math::RangeRandom(0, m);
				col.b = m;
				temp->setColour(col);
				temp->setRotation(Radian(totaltime));
				//vec = bbs->getParentNode()->getPosition();
				//vec.x = totaltime;
				//bbs->getParentSceneNode()->setPosition(vec);
			}
			bool ret = ExampleFrameListener::frameStarted(evt);
			return ret;
		}

		bool frameEnded(const FrameEvent& evt){
			for(int i = 0; i < 1000; i++){
				char name[50];
				sprintf(name, "cluster_%d", i);
			 	BillboardSet* bbs = mSceneMgr->getBillboardSet(name);
				Billboard* temp = bbs->getBillboard(0);
				ColourValue col = temp->getColour();
				float m = abs(sin(totaltime));
				m -= Math::RangeRandom(0, m);
				if(m < 0.8){
					m = 0.8;
				}
				col.r = m;
				m -= Math::RangeRandom(0, m);
				col.g = m;
				m -= Math::RangeRandom(0, m);
				col.b = m;
				//temp->setColour(col);
				temp->setRotation(Radian(totaltime/100.0f));
				//vec = bbs->getParentNode()->getPosition();
				//vec.x = totaltime;
				//bbs->getParentSceneNode()->setPosition(vec);
			}
			bool ret = ExampleFrameListener::frameEnded(evt);
			return ret;
		}

};


class LApp: public ExampleApplication{
	public:
		LApp(){
		}
		~LApp(){
		}

	protected:
		virtual void createCamera(void){
			mCamera = mSceneMgr->createCamera("PlayerCam");
			mCamera->setPosition(Vector3(0,0,0));
			mCamera->lookAt(Vector3(0,0,-300));
			mCamera->setNearClipDistance(5);
			mCamera->setFarClipDistance(2000);
		}

		virtual void createScene(void){
			Entity* ent = mSceneMgr->createEntity("ogre", "fort.mesh");
			//ent->setMaterialName("metal_plate");
			SceneNode* node = mSceneMgr->getRootSceneNode()->createChildSceneNode();
			node->attachObject(ent);
			node->setPosition(0,0,0);

			for(int i = 0; i < 1000; i++){
				char name[50];
				sprintf(name, "cluster_%d", i);
				BillboardSet* bbs2 = mSceneMgr->createBillboardSet(name, 1);
				bbs2->setBillboardRotationType(Ogre::BillboardRotationType::BBR_VERTEX);
				SceneNode* bbsNode2 = mSceneMgr->getRootSceneNode()->createChildSceneNode();
				
				char type[50];
				int ran = ((int)(Math::RangeRandom(0, 1)*100))%8+1;
				sprintf(type, "spacebillboard/cluster_%d", ran);
				bbs2->setMaterialName(type);
				bbs2->getMaterial()->setTextureFiltering(TextureFilterOptions::TFO_TRILINEAR);
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
			for(int i = 0; i < 2000; i++){
				char name[50];
				sprintf(name, "star_%d", i);
				BillboardSet* bbs2 = mSceneMgr->createBillboardSet(name, 1);
				bbs2->setBillboardRotationType(Ogre::BillboardRotationType::BBR_VERTEX);
				SceneNode* bbsNode2 = mSceneMgr->getRootSceneNode()->createChildSceneNode();
				
				char type[50];
				int ran = ((int)(Math::RangeRandom(0, 1)*100))%2+1;
				sprintf(type, "spacebillboard/star_%d", ran);
				bbs2->setMaterialName(type);
				bbs2->getMaterial()->setTextureFiltering(TextureFilterOptions::TFO_TRILINEAR);
				bbs2->getMaterial()->setDepthCheckEnabled(true);
				int px = Math::RangeRandom(-2500, 2500);
				int py = Math::RangeRandom(-2500, 2500);
				int pz = Math::RangeRandom(-2500, 2500);
				px = 0;
				Billboard* bb = bbs2->createBillboard(0,0,0,ColourValue(Math::RangeRandom(0.6f,1),Math::RangeRandom(0.6f,1),Math::RangeRandom(0.6f,1)));
				int dim = Math::RangeRandom(1.5f, 2);
				bb->setDimensions(5*dim, 5*dim);
				bb->setRotation(Radian(Math::RangeRandom(0, 2*Math::PI)));
				bbsNode2->attachObject(bbs2);
				bbsNode2->setPosition(px, py, pz);
			}	
		}

		void createFrameListener(void){
			mFrameListener= new LFrameListener(mSceneMgr, mWindow, mCamera);
			mRoot->addFrameListener(mFrameListener);
		}
};


#endif // #ifndef __lost_h_