/*
-----------------------------------------------------------------------------
Filename:    Player.cpp
-----------------------------------------------------------------------------
*/
#include "Player.h"
#include "PhysicsSimulator.h"
#include <OgreAnimationState.h>
#include <cstdlib>


#define PAD_LEFT  0
#define PAD_RIGHT 1
#define PAD_UP    2
#define PAD_DOWN  3

using namespace std;

int edgeSize = 500;

double paddleModifier = 100.0f;
double paddleScale = 0.75f;
Ogre::Entity* ent;

Player::Player(Ogre::SceneManager* pSceneMgr, PhysicsSimulator* sim, 
	std::string node, std::string color, bool isCloseToCamera)
{
	forceUpdate = false;
	
	mSceneMgr = pSceneMgr;
	bullet = sim;
	
	ent = mSceneMgr->createEntity("PosXYEntity" + node, "ninja.mesh");
   	Ogre::SceneNode* snode = mSceneMgr->getRootSceneNode()->
  		createChildSceneNode(node);
	/*
	Animation States for Ninja:
		Attack1 Attack2 Attack3 Backflip Block Climb Crouch Death1 Death2 HighJump
		Idle1 Idle2 Idle3 Jump JumpNoHeight Kick SideKick Spin Stealth Walk
	*/
	ent->getAnimationState("Walk")->setLoop(true);
	ent->getAnimationState("Walk")->setEnabled(true);
	Ogre::Vector3 shapeDim = Ogre::Vector3(edgeSize/5, edgeSize/5, 0.01);
   	
   	int size = edgeSize/2;
   	
   	if(!isCloseToCamera) {
   		size = -size;
   	}
   	Ogre::Vector3 position = Ogre::Vector3(0, -size, size);
		
	snode->attachObject(ent);
	snode->scale(.5, .5, .5);
	snode->translate(position);
   	ent->setMaterialName(color);
   	ent->setCastShadows(false);
   	
   	paddle = bullet->setRigidBoxBody(snode, shapeDim, position, 0.0);
   	
	paddle->setCollisionFlags( paddle->getCollisionFlags() | 
		btCollisionObject::CF_KINEMATIC_OBJECT);
	paddle->setActivationState(DISABLE_DEACTIVATION);
	
	mPlayerState = new gameUpdate; //allocating mem on heap
	
	mPlayerState->paddleDir[PAD_UP] = false;
	mPlayerState->paddleDir[PAD_DOWN] = false;
	mPlayerState->paddleDir[PAD_LEFT] = false;
	mPlayerState->paddleDir[PAD_RIGHT] = false;

}
//---------------------------------------------------------------------------
Player::~Player(void)
{	
	if(mPlayerState){delete mPlayerState;} //freeing heap mem

}
btRigidBody* Player::getRigidBody(void)
{
	return paddle;
}
void Player::updatePosition(const Ogre::FrameEvent& evt)
{
	paddle->getMotionState()->getWorldTransform(trans);
	btVector3 pos = trans.getOrigin();

	//do not change this line, otherwise one step of animation doesn't match up with distance moved
	float movement = 100.0 * evt.timeSinceLastFrame; 
	
	//change this line to make ninja walk faster/slower
	float movement_scale = 2.0; 
	
	//need to change right/left to rotate instead of moving left/right
	if( mPlayerState->paddleDir[PAD_RIGHT] )
	{
		pos.setX(pos.getX() + movement*movement_scale);
		ent->getAnimationState("Walk")->addTime(evt.timeSinceLastFrame*movement_scale);
	}
	if( mPlayerState->paddleDir[PAD_LEFT] )
	{
		pos.setX(pos.getX() - movement*movement_scale);
		ent->getAnimationState("Walk")->addTime(evt.timeSinceLastFrame*movement_scale);
	}
	
	
	if( mPlayerState->paddleDir[PAD_UP] )
	{
		pos.setZ(pos.getZ() - movement*movement_scale);
		ent->getAnimationState("Walk")->addTime(evt.timeSinceLastFrame*movement_scale);
		
	}
	if( mPlayerState->paddleDir[PAD_DOWN] )
	{
		pos.setZ(pos.getZ() + movement*movement_scale);
		ent->getAnimationState("Walk")->addTime(evt.timeSinceLastFrame*movement_scale);
	}
		
	if(forceUpdate) {
		pos.setX(mPlayerState->paddlePos[0]);
		pos.setY(mPlayerState->paddlePos[1]);
		pos.setZ(mPlayerState->paddlePos[2]);
		forceUpdate = false;	
	}

	trans.setOrigin(pos);
	paddle->getMotionState()->setWorldTransform(trans);
	
	mPlayerState->paddlePos[0] = pos.getX();
	mPlayerState->paddlePos[1] = pos.getY();
	mPlayerState->paddlePos[2] = pos.getZ();

}
void Player::updatePosition(const Ogre::FrameEvent& evt, gameUpdate* update)
{
	mPlayerState = update;
	forceUpdate = true;
	updatePosition(evt);
}
void Player::updatePadDirection(int element, bool value)
{
	mPlayerState->paddleDir[element] = value;
}
gameUpdate* Player::getPlayerGameState(void)
{
	return mPlayerState;
}
