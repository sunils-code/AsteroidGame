#ifndef __ASTEROIDS_H__
#define __ASTEROIDS_H__

#include "GameUtil.h"
#include "GameSession.h"
#include "IKeyboardListener.h"
#include "IGameWorldListener.h"
#include "IScoreListener.h" 
#include "ScoreKeeper.h"
#include "Player.h"
#include "IPlayerListener.h"
#include "Enemy.h"

class GameObject;
class Spaceship;
class GUILabel;

class Asteroids : public GameSession, public IKeyboardListener, public IGameWorldListener, public IScoreListener, public IPlayerListener
{
public:
	Asteroids(int argc, char *argv[]);
	virtual ~Asteroids(void);

	virtual void Start(void);
	virtual void Stop(void);

	// Declaration of IKeyboardListener interface ////////////////////////////////

	void OnKeyPressed(uchar key, int x, int y);
	void OnKeyReleased(uchar key, int x, int y);
	void OnSpecialKeyPressed(int key, int x, int y);
	void OnSpecialKeyReleased(int key, int x, int y);

	// Declaration of IScoreListener interface //////////////////////////////////

	void OnScoreChanged(int score);

	// Declaration of the IPlayerLister interface //////////////////////////////

	void OnPlayerKilled(int lives_left);

	// Declaration of IGameWorldListener interface //////////////////////////////

	//void OnWorldUpdated(GameWorld* world);
	void OnWorldUpdated(GameWorld* world);
	void OnObjectAdded(GameWorld* world, shared_ptr<GameObject> object) {}
	//void OnObjectAdded(GameWorld* world, shared_ptr<GameObject> object);
	void OnObjectRemoved(GameWorld* world, shared_ptr<GameObject> object);

	// Override the default implementation of ITimerListener ////////////////////
	void OnTimer(int value);

private:
	shared_ptr<Spaceship> mSpaceship;
	shared_ptr<Enemy> mEnemy;
	shared_ptr<GUILabel> mScoreLabel;
	shared_ptr<GUILabel> mLivesLabel;
	shared_ptr<GUILabel> mGameOverLabel;
	shared_ptr<GUILabel> nextLevelLable;
	shared_ptr<GUILabel> mlevelLabel;
	uint mLevel;
	uint mAsteroidCount;
	bool singleOrBurst = true;
	bool isShooting = false;
	//int mSmallAsteroidCount;
	int mliveDropCount = 0;
	int mLiveCount = 0;
	int mPowerUpCount = 0;
	float dy;
	float dx;
	float eAngle;
	int mEnemyLives = 3;
	int fps;

	void ResetSpaceship();
	shared_ptr<GameObject> CreateSpaceship();
	shared_ptr<GameObject> CreateEnemy();
	//void CreateEnemy();


	void CreateGUI();
	void CreateAsteroids(const uint num_asteroids);
	void CreateSmallAsteroids(const uint num_asteroids, GLVector3f position);
	void CreateLives(GLVector3f position);
	void CreatePwrUp();
	//void CreateEnemy(shared_ptr<Spaceship> mSpaceship);

	//shared_ptr<GameObject> CreateSmallAsteroids();
	shared_ptr<GameObject> CreateExplosion();

	const static uint SHOW_GAME_OVER = 0;
	const static uint START_NEXT_LEVEL = 1;
	const static uint CREATE_NEW_PLAYER = 2;

	ScoreKeeper mScoreKeeper;
	Player mPlayer;
};

#endif