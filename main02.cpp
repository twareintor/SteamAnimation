
////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Steam engine simple animation 
// Coded by Twareintor through extension of Irrlicht Tutorial #2
// Author: Claudiu Ciutacu (ciutacu/o'gmail.com)
// credits: Youtube Chris Smith <https://www.youtube.com/watch?v=o5bZDYuN_gc>
// credits: Chris Smith <a href = https://www.youtube.com/channel/UCUsT1MT1mUGQ2WWV80DC50g>
// the model is downoladed from here: <a href = http://www.chrissmith.org.uk/thomas> where the moving parts 
// were taking apart and used as separate nodes 

#ifdef _MSC_VER
// We'll also define this to stop MSVC complaining about sprintf().
#define _CRT_SECURE_NO_WARNINGS
#pragma comment(lib, "Irrlicht.lib")
#endif

#include <windows.h>
#include <irrlicht.h>
#include "driverChoice.h"

#define		PI				3.141f

#define		STENCIL_BUF		false

using namespace irr;

std::ostream& operator<<(std::ostream& out, const core::vector3df vec);

class MyEventReceiver : public IEventReceiver
{
public:
	// This is the one method that we have to implement
	virtual bool OnEvent(const SEvent& event)
	{
		// Remember whether each key is down or up
		if (event.EventType == irr::EET_KEY_INPUT_EVENT)
			KeyIsDown[event.KeyInput.Key] = event.KeyInput.PressedDown;

		return false;
	}

	// This is used to check whether a key is being held down
	virtual bool IsKeyDown(EKEY_CODE keyCode) const
	{
		return KeyIsDown[keyCode];
	}

	MyEventReceiver()
	{
		for (u32 i=0; i<KEY_KEY_CODES_COUNT; ++i)
			KeyIsDown[i] = false;
	}

private:
	// We use this array to store the current state of each key
	bool KeyIsDown[KEY_KEY_CODES_COUNT];
};


int main()
{
	// ask user for driver
	video::E_DRIVER_TYPE driverType=driverChoiceConsole();
	if (driverType==video::EDT_COUNT)
		return 1;

	// create device
	MyEventReceiver receiver;
	u32 nWidth = GetSystemMetrics(0);
	u32 nHeight = GetSystemMetrics(1);
	bool bStBuf = STENCIL_BUF;
	IrrlichtDevice* device = createDevice(driverType,
		core::dimension2d<u32>(nWidth*3/3, nHeight*3/3), 16, false, bStBuf, false, &receiver);

	if (device == 0)
		return 1; // could not create selected driver.

	video::IVideoDriver* driver = device->getVideoDriver();
	scene::ISceneManager* smgr = device->getSceneManager();
	// smgr->setLightManager(0);
	// driver->setAmbientLight(video::SColorf(.7f, .1f, .1f, 1.f));	//	Ambient light in the scene: Scene Manager does it!
	// smgr->setAmbientLight(video::SColorf(.7f, .1f, .1f, 1.f));
	driver->setAmbientLight(video::SColorf(.2f, .2f, .3f, 1.f));

	scene::ILightSceneNode* lgtSceneLight01 = 
		smgr->addLightSceneNode(0, core::vector3df(1000.f, 3000.f, 1000.f), video::SColorf(.9f, .9f, .6f), 6000.f);
	scene::ILightSceneNode* lgtSceneLight02 = 
		smgr->addLightSceneNode(0, core::vector3df(-100.f, -3000.f, -100.f), video::SColorf(.1f, .2f, .2f), 6000.f);

	// Possible bug?? Ambient light doesn't works this way!
	// the basement whre the track is lying:
	scene::ISceneNode* nodBas = 0;
	/*************		 *************/
	nodBas = smgr->addCubeSceneNode(1.f);
	nodBas->setScale(core::vector3df(32.f, 1.f, 32000.f));		// a platform
	nodBas->setPosition(core::vector3df(1.f, -1.f, 0.f));
	/****************	*******************/
	video::ITexture* norMap = driver->getTexture("d:/media/rockwall_NRM.tga");
	if (norMap)
	{
		driver->makeNormalMapTexture(norMap, 9.f);		// and forget mesh with tangents...
		nodBas->setMaterialTexture(0, driver->getTexture("d:/media/rockwall.jpg"));
		nodBas->getMaterial(0).getTextureMatrix(0).setTextureScale(32000, 32);
	}

	// Train Rail:
	scene::IAnimatedMesh* mshRail = smgr->getMesh("d:/media/Track/Track000.obj");
	scene::IAnimatedMeshSceneNode* nodRail = 0;

	for (int i = 0; i<32; i++)
	{
		nodRail = smgr->addAnimatedMeshSceneNode(mshRail);
		nodRail->setScale(core::vector3df(3.2f));
		core::aabbox3df boxRailSegment = nodRail->getBoundingBox();
		core::vector3df edgsRail[8];
		boxRailSegment.getEdges(edgsRail);
		// nodRail->setPosition(core::vector3df(0.f, 0.f, i*(edgsRail[1].Z-edgsRail[0].Z)));
		nodRail->setPosition(core::vector3df(0.f, 0.f, i*95.f));
		nodRail->setMaterialFlag(video::EMF_LIGHTING, true);

	}
	// MessageBoxA(NULL, "here", "", MB_OK);

	/************* THOMAS THE TANK ENGINE MADE FROM PARTS ************/
	scene::IAnimatedMesh* mshsParts[5];
	mshsParts[0] = smgr->getMesh("d:/media/Thomas/ThomasParts.001/ThomasFixedParts.obj"); // the case
	mshsParts[1] = smgr->getMesh("d:/media/Thomas/ThomasParts.001/ThomasUnderCase.obj"); // the under case
	mshsParts[2] = smgr->getMesh("d:/media/Thomas/ThomasParts.001/ThomasFaceComplett.obj"); // the face
	mshsParts[3] = smgr->getMesh("d:/media/Thomas/ThomasParts.001/ThomasMiddleAxle.obj"); // the axles
	mshsParts[4] = smgr->getMesh("d:/media/Thomas/ThomasParts.001/ThomasConnectionRod.obj"); // the rods
	// scene::IMeshManipulator* man = smgr->getMeshManipulator(); // not needed
	// 
	scene::IAnimatedMeshSceneNode* nodEngine =	smgr->addAnimatedMeshSceneNode(mshsParts[0]);
	if (nodEngine)
	{
		nodEngine->setMaterialFlag(video::EMF_LIGHTING, true);
		nodEngine->setMaterialFlag(video::EMF_BACK_FACE_CULLING, false); // this is not good!
		nodEngine->setScale(core::vector3df(2.0f, 2.0f, 2.0f));
		nodEngine->setPosition(core::vector3df(0.f, 0.72f, 0.f));
	}
	// nodEngine->setDebugDataVisible(true);
	core::array<scene::ISceneNode*> nodsEngineParts;
	nodsEngineParts.push_back(smgr->addAnimatedMeshSceneNode(mshsParts[1], nodEngine));		// 0
	nodsEngineParts.push_back(smgr->addAnimatedMeshSceneNode(mshsParts[2], nodEngine));		// 1
	nodsEngineParts.push_back(smgr->addAnimatedMeshSceneNode(mshsParts[3], nodEngine));		// 2
	nodsEngineParts.push_back(smgr->addAnimatedMeshSceneNode(mshsParts[4], nodEngine));		// 3
	for(u32 i = 0; i<nodsEngineParts.size(); i++)
	{
		nodsEngineParts[i]->setMaterialFlag(video::EMF_LIGHTING, true);
		nodsEngineParts[i]->setMaterialFlag(video::EMF_BACK_FACE_CULLING, false); // this is not good!
	}
	core::array<scene::ISceneNode*> nodsAxles;
	nodsAxles.push_back(nodsEngineParts[2]);				// 0
	// nodsAxles[0]->getPosition() == (0, 1.125, -0.123) // detected!!!
	nodsAxles[0]->setPosition(core::vector3df(0.f, 1.125f, 1.2f));
	nodsAxles.push_back(nodsEngineParts[2]->clone());		// 1
	const f32 fAxlesDist = 2.4f;	// the distance to the next axle
	nodsAxles[1]->setPosition(nodsAxles[0]->getPosition()+core::vector3df(0.f, 0.f, fAxlesDist));
	nodsAxles.push_back(nodsEngineParts[2]->clone());		// 2 (3 axles)
	nodsAxles[2]->setPosition(nodsAxles[0]->getPosition()+core::vector3df(0.f, 0.f, -fAxlesDist));
	core::array<scene::ISceneNode*> nodsRods;
	nodsRods.push_back(nodsEngineParts[3]);					// 0
	nodsRods.push_back(nodsEngineParts[3]->clone());		// 1 (2 rods, L-R)
	const f32 fRadWheel = 0.963f;	// the radius of the wheel (from Blender)
	const f32 fRadCrank = 0.277f;	// the radius of the crank of the wheel (from Blender)
	const f32 fGauge = 2.890f;
	nodsRods[0]->setPosition(nodsAxles[0]->getPosition()+core::vector3df(-fGauge/2.f, -fRadCrank, 0.f));
	nodsRods[1]->setPosition(nodsRods[0]->getPosition()+core::vector3df(fGauge, +2*fRadCrank, 0.f));
	f32 fth = 0.f;		// to rotate axles: WARNING!!! in radians
	f32 fSpeed = 0.f;	// the speed of the train
	bool bFollowTarget = false; // if true, the main camera follows the target engine..
	// nodsAxles[0]->setRotation(core::vector3df(90.f, 0.f, 0.f)); // for debug only
	/************************ add dinamic lights ************************/
	scene::ILightSceneNode* light01;
	// if(bStBuf) light01->enableCastShadow(true);
	scene::ISceneNodeAnimator* aniL;
	scene::IBillboardSceneNode* nodBil;
	// add light #1
	light01 = smgr->addLightSceneNode(0, core::vector3df(0.f, 0.f, 0.f), video::SColorf(1.f, 1.f, 1.f), 60.f, -1);;
	aniL = smgr->createFlyCircleAnimator(core::vector3df(10.f, 10.f, 0.f), 40.0f, 0.001f);
	light01->addAnimator(aniL);
	// aniL->drop();
	nodBil = smgr->addBillboardSceneNode(light01);
	nodBil->setMaterialFlag(video::EMF_LIGHTING, false);
	nodBil->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR);
	nodBil->setMaterialTexture(0, driver->getTexture("d:/media/particlewhite.bmp"));
	// light01->setVisible(false);															// DEBUG ONLY
	// add another light:
	light01 = smgr->addLightSceneNode(0, core::vector3df(0.f, 0.f, 0.f), video::SColorf(1.f, 1.f, 1.f), 40.f -1);;
	aniL = smgr->createFlyCircleAnimator(core::vector3df(10.f, 10.f, 0.f), 40.0f, -0.003f);
	light01->addAnimator(aniL);
	aniL->drop();
	nodBil = smgr->addBillboardSceneNode(light01);
	nodBil->setMaterialFlag(video::EMF_LIGHTING, false);
	nodBil->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR);
	nodBil->setMaterialTexture(0, driver->getTexture("d:/media/particlewhite.bmp"));
	// light01->setVisible(false);														// DEBUG ONLY
	/******************/ // only for configuration
	// a directional light simulating the lantern of the Thomas:
	scene::ISceneNode* nodDTo = smgr->addEmptySceneNode(nodEngine);		// for a directional light
	nodDTo->setPosition(core::vector3df(1.23f, 2.23f, 6.11f));
	scene::ILightSceneNode* lghTom = smgr->addLightSceneNode(nodDTo);	// for a directional light
	video::SLight& ld = lghTom->getLightData();
	ld.Type = video::ELT_SPOT;
	ld.Direction.set(core::vector3df(-1.f, 0.f, 0.f));
	ld.InnerCone = 120.f; 
	ld.OuterCone = 180.f;
	nodDTo->setRotation(core::vector3df(0.f, 0.f, 0.f));
	// a billboard to it...
	nodBil = smgr->addBillboardSceneNode(nodDTo);									
	nodBil->setMaterialFlag(video::EMF_LIGHTING, false);							
	nodBil->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR);						
	nodBil->setMaterialTexture(0, driver->getTexture("d:/media/particle.bmp"));		
	nodBil->setSize(core::vector2df(2.f, 2.f));

	// the smoke or the engine: a particle system:
	scene::IParticleSystemSceneNode* ps = smgr->addParticleSystemSceneNode(false);
	scene::IParticleEmitter* em = ps->createBoxEmitter(
		core::aabbox3d<f32>(-1, 0, -1, 1, 1, 1), // emitter size
		core::vector3df(0.0f, 0.006f, 0.0f),   // initial direction
		40, 40,                             // emit rate
		video::SColor(0, 255, 255, 255),       // darkest color
		video::SColor(0, 255, 255, 255),       // brightest color
		300, 1400, 0,                         // min and max age, angle
		core::dimension2df(2.5f, 2.5f),         // min size
		core::dimension2df(4.5f, 4.5f));        // max size
	em->setMinStartSize(core::dimension2df(2.5f, 2.5f));
	ps->setEmitter(em);
	em->drop();
	scene::IParticleAffector* paf = ps->createFadeOutParticleAffector();
	ps->addAffector(paf); // same goes for the affector
	paf->drop();
	ps->setParent(nodEngine);
	ps->setPosition(core::vector3df(0.f, 6.8f, 4.f));
	ps->setScale(core::vector3df(.1f, .1f, .1f));
	ps->setMaterialFlag(video::EMF_LIGHTING, false);
	// ps->setMaterialFlag(video::EMF_ZWRITE_ENABLE, false);
	ps->setMaterialTexture(0, driver->getTexture("d:/media/smoke2.jpg"));
	ps->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR);
	// test it:
	scene::ISceneNode* nodStone = smgr->addSphereSceneNode();
	if (nodStone)
	{
		nodStone->setPosition(core::vector3df(0.f, 5.3f, 40.f));
		nodStone->setMaterialTexture(0, driver->getTexture("d:/media/wall.bmp"));
		nodStone->setMaterialFlag(video::EMF_LIGHTING, true);
	}
	// nodStone->setDebugDataVisible(true);
	f32			fSpeedStone = 0.f;	// 
	f32 		fthStone = 0.f;		
	f32 		fRadStone = 1.f;

	/******************/ // only for configuration// FOR CONFIGURATION ONLY
	// the main camera:
	SKeyMap keyMap[] = 
	{
		{EKA_MOVE_FORWARD,		KEY_UP		}, 		{EKA_MOVE_FORWARD,		KEY_KEY_W	}, 
		{EKA_MOVE_BACKWARD,		KEY_DOWN	}, 		{EKA_MOVE_BACKWARD,		KEY_KEY_S	}, 
		{EKA_STRAFE_LEFT,		KEY_LEFT	}, 		{EKA_STRAFE_LEFT,		KEY_KEY_A	}, 
		{EKA_STRAFE_RIGHT,		KEY_RIGHT	}, 		{EKA_STRAFE_RIGHT,		KEY_KEY_D	},
		{EKA_JUMP_UP,			KEY_SPACE	}, 		{EKA_JUMP_UP,			KEY_INSERT	},
	};
	scene::ICameraSceneNode* camMain = 0;
	camMain = smgr->addCameraSceneNodeFPS(0, 50.f, 0.01f,-1, keyMap, sizeof(keyMap), false, 10.f, false, true);
	camMain->setPosition(core::vector3df(-30.f, 3.f, 0.f));
	camMain->setTarget(nodEngine->getAbsolutePosition());
	// 
	device->getCursorControl()->setVisible(false);
	/*	Add a colorful irrlicht logo	*/
	device->getGUIEnvironment()->addImage(
		driver->getTexture("d:/media/irrlichtlogoalpha2.tga"),
		core::position2d<s32>(10,20));

	gui::IGUIStaticText* diagnostics = device->getGUIEnvironment()->addStaticText(
		L"", core::rect<s32>(10, 10, 400, 20));
	diagnostics->setOverrideColor(video::SColor(255, 255, 255, 0));

	/*
	We have done everything, so lets draw it. We also write the current
	frames per second and the name of the driver to the caption of the
	window.
	*/
	int lastFPS = -1;

	// In order to do framerate independent movement, we have to know
	// how long it was since the last frame
	u32 then = device->getTimer()->getTime();

	// This is the movemen speed in units per second.
	const f32	MOVEMENT_SPEED = 5.f;
	f32			fposi = 0.1f;		// configuration only
	f32			fo = 0.01f;		// configuration only
	u32			nMat = 0;
	scene::ISceneNode* nodDeb = 0;			// debug and configuration only
	// to make the camera to automatically follow the engine while following the user commands too...
	core::vector3df prevPos = nodEngine->getPosition();
	core::vector3df dMoved = core::vector3df(0.f, 0.f, 0.f);
	// for the smoke: full power
	bool bFullPower = false;
	bool itBlows = false; // speed dependent to simulate the blwo off of the steam into the stack
	u32 startBlow = device->getTimer()->getTime();
	while(device->run())
	{
		// Work out a frame delta time.
		const u32 now = device->getTimer()->getTime();
		const f32 frameDeltaTime = (f32)(now - then) / 1000.f; // Time in seconds
		then = now;
		// BEGIN OF ONLY FOR DEBUG AND CONFIGURATION:
		/*******************
		if(receiver.IsKeyDown(KEY_KEY_X))
		{
		nodTom->getMaterial(nMat).EmissiveColor.set(255, 255, 255, 0);
		if(nMat)	nodTom->getMaterial(nMat-1).EmissiveColor.set(0, 0, 0, 0);
		std::cout<<nMat<<std::endl;
		nMat++;
		if(nMat==nodTom->getMaterialCount())	nMat = 0;
		Sleep(400);
		}
		*********************/
		nodDeb = nodStone;
		if(receiver.IsKeyDown(KEY_KEY_3)) bFollowTarget = true;
		if(receiver.IsKeyDown(KEY_KEY_4)) bFollowTarget = false;
		if(fabs(fSpeed)>0.01f)
		{
			nodEngine->setPosition(nodEngine->getPosition()+core::vector3df(0.f, 0.f, fSpeed/(f32)lastFPS));
			if(bFollowTarget) camMain->setTarget(nodEngine->getPosition()+core::vector3df(0.f, 3.f, 0.f));
		}
		if(receiver.IsKeyDown(KEY_NUMPAD8))
		{
			fSpeed+=0.025f*(fabs(fSpeed)<24.f);	
		}
		if(receiver.IsKeyDown(KEY_NUMPAD2))
		{
			fSpeed-=0.025*(fabs(fSpeed)<24.f);
		}
		bFullPower = receiver.IsKeyDown(KEY_NUMPAD8) || receiver.IsKeyDown(KEY_NUMPAD2);
		// for the smoke: more smoke when we accelerate (forward or backward)
		if(device->getTimer()->getTime() - startBlow > 340/fSpeed)
		{
			itBlows = false;
			if(device->getTimer()->getTime() - startBlow > 2200/fSpeed)
			{
				startBlow = device->getTimer()->getTime();
			}
		}
		else
		{
			itBlows = true;
		}
		if(bFullPower && itBlows)
		{
			em->setMaxStartSize(core::dimension2df(4.f+2.f*cos(device->getTimer()->getTime()), 4.f+2.f*sin(device->getTimer()->getTime())));
			em->setMaxParticlesPerSecond(800.f);
			em->setMinLifeTime(800.);
			em->setMaxLifeTime(2400.);
		}
		else
		{
			em->setMaxStartSize(core::dimension2df(2.f, 2.f));
			em->setMaxParticlesPerSecond(100.f);
			em->setMinLifeTime(300.);
			em->setMaxLifeTime(1400.);
		}
		// random initial direction to simulate wind:
		em->setDirection(em->getDirection() + 
			core::vector3df(0.001f*cos(device->getTimer()->getTime()/21.f), 0.f, 0.001f*sin(device->getTimer()->getTime()/21.f)));
		// ps->setPosition(core::vector3df(0.f, 6.9f, 4.f-0.1*fSpeed)); // not realistic
		// em->setDirection(em->getDirection() + core::vector3df(0.0f, 0.f, 0.0f));
		// speed:
		if(!receiver.IsKeyDown(KEY_NUMPAD8)||!receiver.IsKeyDown(KEY_NUMPAD8))
		{
			fSpeed*=0.9995f; // dimminish the speed
		}
		if(receiver.IsKeyDown(KEY_NUMPAD0))
		{
			fSpeed*=0.99f; // brake
		}
		if(receiver.IsKeyDown(KEY_KEY_P))
		{
			std::cout<<"nodsAxles[0]->getPosition() == "<<nodsAxles[0]->getPosition()<<std::endl;
			Sleep(400);
		}
		if(receiver.IsKeyDown(KEY_KEY_Q))
		{
			camMain->setPosition(camMain->getPosition()+core::vector3df(0.f, .1f, 0.f));
		}
		if(receiver.IsKeyDown(KEY_KEY_Z))
		{
			camMain->setPosition(camMain->getPosition()-core::vector3df(0.f, .1f, 0.f));
		}
		// the camera follows the engine...
		dMoved = nodEngine->getPosition()-prevPos;
		camMain->setPosition(camMain->getPosition()+dMoved);
		prevPos = nodEngine->getPosition();
		camMain->setTarget(nodEngine->getPosition());
		// debugging: node for debug the scene:
		nodDeb = 0;
		// this will "avoid" collision between engine and stone on the rail
		core::vector3df& dstStone = nodEngine->getPosition() - nodStone->getPosition();
		// if(fabs(dstStone.getLength())<5.f) nodStone->setVisible(false); else nodStone->setVisible(true);

		/******** this will move the sone when pushed by the engine *******/
		// core::aabbox3df boxEngine = nodEngine->getBoundingBox();
		// core::aabbox3df boxStone = nodStone->getBoundingBox();
		if(dstStone.getLength()<18.f) // collision detected
		{
			fSpeedStone+=fSpeed*.05f;	// this will give energy to the stone
			fSpeed*=0.73f;				// this will slow down the engine
		}
		else
		{
			fSpeedStone*=0.9997f; // the stone rolls faster than the engine advances or retreates
		}
		if(fabs(fSpeedStone>0.005f))
		{
			nodStone->setPosition(nodStone->getPosition()+core::vector3df(0.f, 0.f, fSpeedStone/(f32)lastFPS));
			nodStone->setRotation(core::vector3df(core::radToDeg(fthStone), 0.f, 0.f));
		}
		fthStone = nodStone->getPosition().Z/fRadStone/5.f; // don't forget to divide it by the scale of the stone (?)

		// DEBUG VAROIUS NODE POSITION, SCALE ROTATION ETC:
		// if(receiver.IsKeyDown(KEY_KEY_G)) nodRail->setScale(nodRail->getScale()+core::vector3df(.1f, .1f, .1f));
		// if(receiver.IsKeyDown(KEY_KEY_B)) nodRail->setScale(nodRail->getScale()-core::vector3df(.1f, .1f, .1f));
		// if(receiver.IsKeyDown(KEY_KEY_T)) std::cout<<nodRail->getScale()<<std::endl;

		// this rotates the wheels of the steam engine:	// this is for debug!!
		if(receiver.IsKeyDown(KEY_KEY_F)) fth+=0.01f;	// for debug!!!
		if(receiver.IsKeyDown(KEY_KEY_R)) fth-=0.01f;
		// if((fth>2.f*PI)||(fth<-2.f*PI)) fth = 0.f; // avoids overflow
		for(u32 i = 0; i<3; i++)
		{
			nodsAxles[i]->setRotation(core::vector3df(core::radToDeg(fth), 0.f, 0.f));
		}
		for(u32 i = 0; i<2; i++)
		{
			nodsRods[i]->setPosition(nodsAxles[0]->getPosition()+
				core::vector3df(-fGauge/2.f*(0==i)+fGauge/2.f*(1==i), fRadCrank*cosf(fth+PI*i), fRadCrank*sinf(fth+PI*i)));
			// 
		}
		fth = nodEngine->getPosition().Z/fRadWheel/2.f; // because the scale of the engine is (2, 2, 2)
		/****************
		if (receiver.IsKeyDown(KEY_KEY_R))		nodDeb->setScale(nodDeb->getScale()+core::vector3df(0.001f, 0.0f, 0.f));
		if (receiver.IsKeyDown(KEY_KEY_T))		nodDeb->setScale(nodDeb->getScale()-core::vector3df(0.001f, 0.0f, 0.f));
		if (receiver.IsKeyDown(KEY_KEY_P))		std::cout<<nodDeb->getScale().X<<std::endl;
		********************/
		// END OF ONLY FOR DEBUG AND CONFIGURATION:
		driver->beginScene(true, true, video::SColor(255, 113, 113, 133));
		smgr->drawAll(); // draw the 3d scene
		device->getGUIEnvironment()->drawAll(); // draw the gui environment (the logo)

		driver->endScene();

		int fps = driver->getFPS();
		if (lastFPS != fps)
		{
			core::stringw tmp(L"Movement Example - Irrlicht Engine [");
			tmp += driver->getName();
			tmp += L"] fps: ";
			tmp += fps;

			device->setWindowCaption(tmp.c_str());
			lastFPS = fps;
		}
	}

	/*
	In the end, delete the Irrlicht device.
	*/
	device->drop();

	return 0;
}


std::ostream& operator<<(std::ostream& out, const core::vector3df vec)
{
	out<<"("<<vec.X<<", "<<vec.Y<<", "<<vec.Z<<")";
	return out;
}

/*
That's it. Compile and play with the program.
**/
