//
// Student Name:
// Student ID:
// Student Email Address:
//
//
// 3D Game Programming
// NCTU
// Instructor: SAI-KEUNG WONG
//
#include "TutorialApplication.h"
#include "BasicTools.h"
#include "map.h"


using namespace Ogre;
using namespace std;
BasicTutorial_00::BasicTutorial_00(void) {}

int count1 = 0;
int conerCount = 0;
float distance1;
float distance2;




void BasicTutorial_00::createCamera(void)
{
	mCameraMap = mSceneMgr->createCamera("MapCamera");

	/*
	// Position it at 500 in Z direction
	mCamera->setPosition(Ogre::Vector3(0,0,80));
	// Look back along -Z
	mCamera->lookAt(Ogre::Vector3(0,0,-300));
	*/


	mCameraMap->setPosition(Ogre::Vector3(0,700,0));
	mCameraMap->lookAt(Ogre::Vector3(0,0,0.001));

	mCameraMap->setNearClipDistance(5);
	mCameraMap->yaw(Degree(180));

	// Create the camera
	mCamera = mSceneMgr->createCamera("PlayerCamera");

	/*
	// Position it at 500 in Z direction
	mCamera->setPosition(Ogre::Vector3(0,0,80));
	// Look back along -Z
	mCamera->lookAt(Ogre::Vector3(0,0,-300));
	*/


	mCamera->setPosition(Ogre::Vector3(0,750,500));
	mCamera->lookAt(Ogre::Vector3(0,0,0));

	mCamera->setNearClipDistance(5);

	mCameraMan = new OgreBites::SdkCameraMan(mCamera);   // create a default camera controller
}

void BasicTutorial_00::createViewports(void)
{
	// Create one viewport, entire window
	Ogre::Viewport* vp = mWindow->addViewport(
		mCameraMap, 1, 0.0, 0, 0.25, 0.25);
	vp->setBackgroundColour(Ogre::ColourValue(0,0,1));
	vp->setOverlaysEnabled(false);
	vp->setSkiesEnabled(false);
	// Alter the camera aspect ratio to match the viewport
	mCameraMap->setAspectRatio(
		Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp->getActualHeight()));

	// Create one viewport, entire window
	vp = mWindow->addViewport(mCamera);
	vp->setBackgroundColour(Ogre::ColourValue(0,0,0));
    //vp->setOverlaysEnabled(false);
	// Alter the camera aspect ratio to match the viewport
	mCamera->setAspectRatio(
		Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp->getActualHeight()));
}

void BasicTutorial_00::createScene(void) 
{
    mCurrentPathPointIndex = 0;
	ColourValue fadeColour(0.9, 0.9, 0.9); 
	//mSceneMgr->setFog(FOG_LINEAR, fadeColour, 0, 1400, 1600);

	
	
	mSceneMgr->setSkyBox(
		true, 
		"Examples/SpaceSkyBox"); 
		

    mSceneMgr->setAmbientLight( ColourValue( 0.9, 0.9, 0.9 ) ); 
	//mSceneMgr->setAmbientLight( ColourValue( 1, 1, 1 ) );  
	mSceneMgr->setShadowTechnique(
		SHADOWTYPE_STENCIL_ADDITIVE); 

	//
	Light *light = mSceneMgr->createLight("Light1"); 
	light->setType(Light::LT_POINT); 
	light->setPosition(Vector3(0, 150, 250)); 
	light->setDiffuseColour(1.0, 1.0, 1.0);		//red
	light->setSpecularColour(0.5, 0.5, 0.5);	//red

	mLightSceneNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
	mLightSceneNode->attachObject(light);
	int i;

	//Ogre::LogManager::getSingletonPtr()->logMessage("*** Initializing SCENE ***");

	//

    //
    mRobotNode = mSceneMgr
		->getRootSceneNode()
		->createChildSceneNode();
    mRobotEntity = mSceneMgr->createEntity(
		"robot", "robot.mesh"); 
    mRobotNode->attachObject( mRobotEntity );
    //
    mRobotNode->scale(1.0, 1.0, 1.0 );
	mRobotNode->setPosition(Vector3(-13.1579, 0, -13.1579));
    
    //mAnimationState = mRobotEntity->getAnimationState("Idle");
	//	mAnimationState->setEnabled(true);
	//	mAnimationState->setLoop(true);

    //
    mMap = new MAP;
    mMap->generateMap( mSceneMgr );
    //

	Plane floor(Vector3::UNIT_Y, 0);
	MeshPtr mFloor = MeshManager::getSingleton().createPlane(
		"floor1",
		ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
		floor,
		1500, 1500,
		20, 20,
		true,
		1,
		5, 5,
		Vector3::UNIT_Z
	);

	Entity *floorEnt = mSceneMgr->createEntity("floor1");
	floorEnt->setMaterialName("Examples/TerrRock");

	SceneNode *fnode = mSceneMgr->getRootSceneNode()->createChildSceneNode();

	fnode->attachObject(floorEnt);

	floorG = floor;

}
bool BasicTutorial_00::mouseMoved( const OIS::MouseEvent &arg )
{
/*
	if (mFlgSelectNow == true) {
		Ray mRay =mTrayMgr->getCursorRay(mCamera);

		Vector2 scn = mTrayMgr->sceneToScreen(mCamera, mRay.getOrigin());
		left = scn.x;
		top = scn.y;
		mSelectionRect->setCorners(left, top, right, bottom);
		mSelectionRect->setVisible(true);
	}
    */
	return BaseApplication::mouseMoved( arg);

}

void BasicTutorial_00::singleClickSelect ( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
	Ray mRay =mTrayMgr->getCursorRay(mCamera);

}

//
// Compute the intersection point between 
// the mouse ray and the plane.
// Save the intersection point to mGoalPosition
//
// Algorithm:
// 1. Get the mouse ray using mTrayMgr->getCursorRay(mCamera)
// 2. Set up the plane p: Plane p(Vector3(0, 1, 0), 0)
// 3. Compute the intersection point: 
//    std::pair<bool,Real> result = ray.intersects(p);
//	  Vector3 point = ray.getOrigin()+result.second*ray.getDirection();
// 4. mGoalPosition = point
//
void BasicTutorial_00::computeGoalPosition( )
{


	Ray mRay = mTrayMgr->getCursorRay(mCamera);


	Vector3 clickPositin = mRay.getPoint(mRay.intersects(floorG).second);
	//cout << clickPositin << endl;

	
	bool result = mMap->performPathFinding(mRobotNode->getPosition(), clickPositin);
	

	
    //mGoalPosition = point;
}

bool BasicTutorial_00::mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
	/*
	if (left==right && top==bottom) {
	return BaseApplication::mouseReleased( arg, id );

	}
	*/
	/* CEGUI
	Ray topLeft = mCamera->getCameraToViewportRay(left, top);
	Ray topRight = mCamera->getCameraToViewportRay(right, top);
	Ray bottomLeft = mCamera->getCameraToViewportRay(left, bottom);
	Ray bottomRight = mCamera->getCameraToViewportRay(right, bottom);
	*/
	if (id == OIS::MB_Right) {
		//computeTargetPosition(1);
		//return true;
		return BaseApplication::mouseReleased( arg, id );
	}
	return BaseApplication::mouseReleased( arg, id );

}

//
// If MouseButtonID id == OIS::MB_Left do
//   computeGoalPosition( )
//   perform path finding
//   Save the path points: mPathPoints = mMap->getPathPoints();
//
bool BasicTutorial_00::mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
    if (id == OIS::MB_Left) {
		
		computeGoalPosition();

		if (mMap->mGoalNode->flgCanMove) 
		{
			updateDirection();
		}

		return BaseApplication::mouseReleased( arg, id );
	}


	return BaseApplication::mousePressed( arg, id );
}

//
// Control the robot to move along the path
// Update the look-at direction of the robot
// Set the animation state of the robot properly
//
void BasicTutorial_00::controlRobotMovingAlongPath( const FrameEvent &evt )
{
	
	Real speed = 2;
	Real t = evt.timeSinceLastFrame;
	

	if (mPathPoints.size() == 0)
	{
		mAnimationState = mRobotEntity->getAnimationState("Idle");
		mAnimationState->setEnabled(true);
		mAnimationState->setLoop(true);
		mAnimationState->addTime(t);
		return;
	}


	if (distance1 > 0 && count1 < direction.size())
	{
		Vector3 walk = direction[count1] * speed * t;
		float walkLength = sqrt(walk.x * walk.x + walk.y * walk.y + walk.z * walk.z);
		mAnimationState = mRobotEntity->getAnimationState("Walk");
		mRobotNode->translate(walk);
		mAnimationState->setEnabled(true);
		mAnimationState->setLoop(true);
		mAnimationState->addTime(t);

		distance1 = distance1 - walkLength;

		
	}
	else if (count1 < direction.size())
	{
		distance1 = sqrt(direction[0].x * direction[0].x + direction[0].y * direction[0].y + direction[0].z * direction[0].z);
		count1++;
		
		
	}
	else
	{
		mRobotNode->translate((mGoalPosition - mRobotNode->getPosition())*0.1);
		mAnimationState = mRobotEntity->getAnimationState("Idle");
		mAnimationState->setEnabled(true);
		mAnimationState->setLoop(true);
		mAnimationState->addTime(t);

	}


	if (distance2 > 0 && conerCount < cornerTarget.size())
	{
		Vector3 walk = direction[count1] * speed * t;
		float walkLength = sqrt(walk.x * walk.x + walk.y * walk.y + walk.z * walk.z);

		robotRotation();
		
		distance2 = distance2 - walkLength;

		
	}
	else if (conerCount < cornerTarget.size())
	{
		conerCount++;
		distance2 = computeDistance(cornerTarget[conerCount - 1], cornerTarget[conerCount]);


	}
	else
	{
		//Vector3 finalTarget = Vector3((mPathPoints[mPathPoints.size() - 1] + direction[direction.size() - 1]).x, mRobotNode->getPosition().y, (mPathPoints[mPathPoints.size() - 1] + direction[direction.size() - 1]).z);

		//mRobotNode->lookAt(finalTarget, Node::TS_WORLD);
	}

}


float BasicTutorial_00::computeDistance(Vector3 p, Vector3 q)
{
	return(sqrt(pow(p.x - q.x, 2) + pow(p.y - q.y, 2) + pow(p.z - q.z, 2)));
}
	


bool BasicTutorial_00::frameStarted(const FrameEvent &evt)
{
    controlRobotMovingAlongPath( evt );
	return BaseApplication::frameStarted(evt);
}

void BasicTutorial_00::updateDirection()
{
	mPathPoints = mMap->getPathPoints();

	direction.clear();
	cornerTarget.clear();
	count1 = 0;
	conerCount = 0;


	
	mGoalPosition = mPathPoints[mPathPoints.size() - 1];

	for (int i = 0; i < mPathPoints.size() - 1; i++)
	{
		direction.push_back(mPathPoints[i + 1] - mPathPoints[i]);
	}

	for (int i = 0; i < mPathPoints.size() - 1; i++)
	{
		if (computeDistance(direction[i], direction[i + 1]) > 5)
		{
			cornerTarget.push_back(mPathPoints[i + 1]);
			//cout << mPathPoints[i+1] << endl;
		}

		
	}

	cornerTarget.push_back(mPathPoints[mPathPoints.size() - 1]);

	//cout << direction.size();
	//cout << cornerTarget.size() << endl;

	distance1 = sqrt(direction[0].x * direction[0].x + direction[0].y * direction[0].y + direction[0].z * direction[0].z);
	distance2 = computeDistance(mMap->mStartNode->position, cornerTarget[0]);
}

void BasicTutorial_00::robotRotation()
{


		Quaternion q0 = mRobotNode->getOrientation();

		Vector3 target = cornerTarget[conerCount];

		target.y = mRobotNode->getPosition().y;
		mRobotNode->lookAt(target, Node::TS_WORLD);
		mRobotNode->yaw(Degree(90));
		Quaternion q1 = mRobotNode->getOrientation();
		float w = 0.9;

		mRobotNode->setOrientation(w*q0 + (1 - w)*q1);
	

	

}

int main(int argc, char *argv[]) {
	BasicTutorial_00 app;
	app.go();  
	return 0;
}
