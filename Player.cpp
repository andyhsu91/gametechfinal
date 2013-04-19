/*
-----------------------------------------------------------------------------
Filename:    Player.cpp
-----------------------------------------------------------------------------
*/
#include "Player.h"
#include "PhysicsSimulator.h"
#include <OgreAnimationState.h>
#include <cstdlib>
#include <string>

#define PAD_LEFT  0
#define PAD_RIGHT 1
#define PAD_UP    2
#define PAD_DOWN  3

using namespace std;

int edgeSize = 500;

double ninjaModifier = 100.0f;
double ninjaScale = 0.75f;
bool test = true;
Ogre::Entity* ent;

enum ninjaStates {
	Attack1, Attack2, Attack3, Backflip, Block, Climb, Crouch, Death1, Death2, HighJump,
		Idle1, Idle2, Idle3, Jump, JumpNoHeight, Kick, SideKick, Spin, Stealth, Walk
};

bool stateActive[20];



Player::Player(Ogre::SceneManager* pSceneMgr, PhysicsSimulator* sim, 
	std::string node, std::string color, bool isCloseToCamera)
{
	forceUpdate = false;
	
	mSceneMgr = pSceneMgr;
	bullet = sim;
	
	ent = mSceneMgr->createEntity("PosXYEntity" + node, "robot.mesh");
   	Ogre::SceneNode* snode = mSceneMgr->getRootSceneNode()->
  		createChildSceneNode(node);

	enableState(Walk, true, true);
	
	Ogre::Vector3 shapeDim = Ogre::Vector3(edgeSize/5, edgeSize/5, 0.01);
   	
   	int size = edgeSize/2;
   	
   	if(!isCloseToCamera) {
   		size = -size;
   	}
   	Ogre::Vector3 position = Ogre::Vector3(0, -size, size);
		
	snode->attachObject(ent);
	snode->scale(1, 1, 1);
	snode->translate(position);
	//snode->rotate(Ogre::Vector3(0,1,0), Ogre::Radian(90.0));
   	ent->setMaterialName(color);
   	
   	//snode->yaw(Ogre::Radian(Ogre::Degree(90.0)));
   	snode->setOrientation(1.0, 0.0, 90.0, 0.0);
   	ent->setCastShadows(false);
   	
   	ninja = bullet->setRigidBoxBody(snode, shapeDim, position, 0.0);
   	
	ninja->setCollisionFlags( ninja->getCollisionFlags() | 
		btCollisionObject::CF_KINEMATIC_OBJECT);
	ninja->setActivationState(DISABLE_DEACTIVATION);
	
	mPlayerState = new gameUpdate; //allocating mem on heap
	
	for(int i=0; i<20; i++){
		stateActive[i]=false;
	}
	
	mPlayerState->ninjaDir[PAD_UP] = false;
	mPlayerState->ninjaDir[PAD_DOWN] = false;
	mPlayerState->ninjaDir[PAD_LEFT] = false;
	mPlayerState->ninjaDir[PAD_RIGHT] = false;

}
//---------------------------------------------------------------------------
Player::~Player(void)
{	
	if(mPlayerState){delete mPlayerState;} //freeing heap mem

}
btRigidBody* Player::getRigidBody(void)
{
	return ninja;
}

std::string getStringFromEnum(int ninjaState)
{
  switch (ninjaState)
  {
  	case Attack1: 	return "Shoot";
	case Attack2: 	return "Shoot";
	case Attack3: 	return "Shoot";
	/*case Backflip:	return "Backflip";
	case Block:		return "Block";
	case Climb:		return "Climb";
	case Crouch:	return "Crouch";
	case Death1:	return "Death1";
	case Death2:	return "Death2";
	case HighJump:	return "HighJump";
	case Idle1:		return "Idle1";
	case Idle2:		return "Idle2";
	case Idle3:		return "Idle3";
	case Jump:		return "Jump";
	case JumpNoHeight:return "JumpNoHeight";
	case Kick:		return "Kick";
	case SideKick:	return "SideKick";
	case Spin:		return "Spin";
	case Stealth:	return "Stealth";*/
	case Walk:		return "Walk";
	default:		return "Invalid";
	
	
  };
}

void Player::enableState(int ninjaState, bool enabled, bool loop){
	
	for(int i=0; i<20; i++){
		stateActive[i]=false;
		std::string animationState = getStringFromEnum(i);
		if(animationState != "Invalid"){
			ent->getAnimationState(animationState)->setEnabled(false);
		}
	}
	
	std::string animationState = getStringFromEnum(ninjaState);
	if(animationState != "Invalid"){
		stateActive[ninjaState]=enabled;
		ent->getAnimationState(animationState)->setLoop(loop);
		ent->getAnimationState(animationState)->setEnabled(enabled);
	}
}

void Player::updateAnimation(int ninjaState, double seconds){
	std::string animationState = getStringFromEnum(ninjaState);
	ent->getAnimationState(animationState)->addTime(seconds);
}

void Player::updateAllAnimations(double seconds){
	for(int i=0; i<20; i++){
		if(stateActive[i]){
			std::string animationState = getStringFromEnum(i);
			if(animationState != "Invalid"){
				ent->getAnimationState(animationState)->addTime(seconds);
			}
		}
	}
}

void Player::attack(bool val){
	std::cout<<"Attacking"<<std::endl;
	enableState(Attack3, val, true);

}
void Player::updatePosition(const Ogre::FrameEvent& evt)
{
	ninja->getMotionState()->getWorldTransform(trans);
	
	btVector3 pos = trans.getOrigin();

	//do not change this line, otherwise one step of animation doesn't match up with distance moved
	float movement = 100.0 * evt.timeSinceLastFrame; 
	
	//change this line to make ninja walk faster/slower
	float movement_scale = 5.0; 
	bool updateWalk = false;
	//need to change right/left to rotate instead of moving left/right
	if( mPlayerState->ninjaDir[PAD_RIGHT] )
	{
		pos.setX(pos.getX() + movement*movement_scale);
		updateWalk = true;
	}
	if( mPlayerState->ninjaDir[PAD_LEFT] )
	{
		pos.setX(pos.getX() - movement*movement_scale);
		updateWalk = true;
	}
	
	
	if( mPlayerState->ninjaDir[PAD_UP] )
	{
		pos.setZ(pos.getZ() - movement*movement_scale);
		updateWalk = true;
		
	}
	if( mPlayerState->ninjaDir[PAD_DOWN] )
	{
		pos.setZ(pos.getZ() + movement*movement_scale);
		updateWalk = true;
	}
	
	if(updateWalk){
		enableState(Walk, true, true);
		updateAnimation(Walk, evt.timeSinceLastFrame*movement_scale);
	}
	
	if(stateActive[Attack3]=true){
		if(test){
			std::cout<<"Animating Attack"<<std::endl;
			test=false;
		}
		updateAnimation(Attack3, evt.timeSinceLastFrame);
	}
	
	if(forceUpdate) {
		pos.setX(mPlayerState->ninjaPos[0]);
		pos.setY(mPlayerState->ninjaPos[1]);
		pos.setZ(mPlayerState->ninjaPos[2]);
		forceUpdate = false;	
	}

	trans.setOrigin(pos);
	
	ninja->getMotionState()->setWorldTransform(trans);
	mSceneMgr->getSceneNode("paddlex0")->yaw(Ogre::Radian(Ogre::Degree(90.0)));
	
	mPlayerState->ninjaPos[0] = pos.getX();
	mPlayerState->ninjaPos[1] = pos.getY();
	mPlayerState->ninjaPos[2] = pos.getZ();

}
void Player::updatePosition(const Ogre::FrameEvent& evt, gameUpdate* update)
{
	mPlayerState = update;
	forceUpdate = true;
	updatePosition(evt);
}
void Player::updatePadDirection(int element, bool value)
{
	mPlayerState->ninjaDir[element] = value;
	
	bool allFalse = true;
	for(int i=0; i<4 && allFalse; i++){
		if(mPlayerState->ninjaDir[i]){
			allFalse=false;
		}
	}
	
	if(allFalse){
		//enableState(Walk, false, false);
	}
	
}
gameUpdate* Player::getPlayerGameState(void)
{
	return mPlayerState;
}
