#include "Asteroid.h"
#include "Asteroids.h"
#include "Animation.h"
#include "AnimationManager.h"
#include "GameUtil.h"
#include "GameWindow.h"
#include "GameWorld.h"
#include "GameDisplay.h"
#include "Spaceship.h"
#include "BoundingShape.h"
#include "BoundingSphere.h"
#include "GUILabel.h"
#include "Explosion.h"
#include "Lives.h"
#include "PowerUp.h"
#include "Enemy.h"
#include "IGameWorldListener.h"

// PUBLIC INSTANCE CONSTRUCTORS ///////////////////////////////////////////////

/** Constructor. Takes arguments from command line, just in case. */
Asteroids::Asteroids(int argc, char *argv[])
	: GameSession(argc, argv)
{
	mLevel = 0;
	mAsteroidCount = 0;
}

/** Destructor. */
Asteroids::~Asteroids(void)
{
}

// PUBLIC INSTANCE METHODS ////////////////////////////////////////////////////

/** Start an asteroids game. */
void Asteroids::Start()
{
	// Create a shared pointer for the Asteroids game object - DO NOT REMOVE
	shared_ptr<Asteroids> thisPtr = shared_ptr<Asteroids>(this);

	// Add this class as a listener of the game world
	mGameWorld->AddListener(thisPtr.get());

	// Add this as a listener to the world and the keyboard
	mGameWindow->AddKeyboardListener(thisPtr);

	// Add a score keeper to the game world
	mGameWorld->AddListener(&mScoreKeeper);

	// Add this class as a listener of the score keeper
	mScoreKeeper.AddListener(thisPtr);

	// Create an ambient light to show sprite textures
	GLfloat ambient_light[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat diffuse_light[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient_light);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse_light);
	glEnable(GL_LIGHT0);

	Animation *explosion_anim = AnimationManager::GetInstance().CreateAnimationFromFile("explosion", 64, 1024, 64, 64, "explosion_fs.png");
	Animation *asteroid1_anim = AnimationManager::GetInstance().CreateAnimationFromFile("asteroid1", 128, 8192, 128, 128, "asteroid1_fs.png");
	Animation *spaceship_anim = AnimationManager::GetInstance().CreateAnimationFromFile("spaceship", 128, 128, 128, 128, "spaceship_fs.png");
	Animation *lives_anim = AnimationManager::GetInstance().CreateAnimationFromFile("lives1", 50, 50, 50, 50, "lives.png");
	Animation *powerUp_anim = AnimationManager::GetInstance().CreateAnimationFromFile("pwr1", 50, 50, 50, 50, "pwr.png");
	Animation *enemy_anim = AnimationManager::GetInstance().CreateAnimationFromFile("enemy", 128, 128, 128, 128, "enemy_fs.png");

	// Create a spaceship and add it to the world
	mGameWorld->AddObject(CreateSpaceship());
	// Create some asteroids and add them to the world
	CreateAsteroids(3);

	//Create Enemey
	mGameWorld->AddObject(CreateEnemy());
	mEnemy->SetPosition(20);
	

	// Add Pwer Up
	CreatePwrUp();
	mPowerUpCount++;

	//Create the GUI
	CreateGUI();

	// Add a player (watcher) to the game world
	mGameWorld->AddListener(&mPlayer);

	// Add this class as a listener of the player
	mPlayer.AddListener(thisPtr);

	// Start the game
	GameSession::Start();
}

/** Stop the current game. */
void Asteroids::Stop()
{
	// Stop the game
	GameSession::Stop();
}

// PUBLIC INSTANCE METHODS IMPLEMENTING IKeyboardListener /////////////////////

void Asteroids::OnKeyPressed(uchar key, int x, int y)
{

	// create powerup then...create variable in asteroids class ...default value is 

	switch (key)
	{
	case ' ':
		if (singleOrBurst)
		{
			mSpaceship->Shoot("single");
			isShooting = true;
			/*GLVector3f a(1,0,0);
			mEnemy->AddPosition(mEnemy->GetPosition() - a);*/
		}
		else
		{
			mSpaceship->Shoot("burst");
			isShooting = true;
			/*	GLVector3f b(0.2, 0, 0);
				mEnemy->AddPosition(mEnemy->GetPosition() - b);*/
		}

		break;
	case 'z':

		mSpaceship->Shoot("single");
		isShooting = true;
		break;
	default:
		break;
	}
}

void Asteroids::OnKeyReleased(uchar key, int x, int y) {}

void Asteroids::OnSpecialKeyPressed(int key, int x, int y)
{
	switch (key)
	{
	// If up arrow key is pressed start applying forward thrust
	case GLUT_KEY_UP: mSpaceship->Thrust(10); break;
	// If left arrow key is pressed start rotating anti-clockwise
	case GLUT_KEY_LEFT: mSpaceship->Rotate(90); break;
	// If right arrow key is pressed start rotating clockwise
	case GLUT_KEY_RIGHT: mSpaceship->Rotate(-90); break;
	// Default case - do nothing
	default: break;
	}
}

void Asteroids::OnSpecialKeyReleased(int key, int x, int y)
{
	switch (key)
	{
	// If up arrow key is released stop applying forward thrust
	case GLUT_KEY_UP: mSpaceship->Thrust(0); break;
	// If left arrow key is released stop rotating
	case GLUT_KEY_LEFT: mSpaceship->Rotate(0); break;
	// If right arrow key is released stop rotating
	case GLUT_KEY_RIGHT: mSpaceship->Rotate(0); break;
	// Default case - do nothing
	default: break;
	} 
}


// PUBLIC INSTANCE METHODS IMPLEMENTING IGameWorldListener ////////////////////

void Asteroids::OnObjectRemoved(GameWorld* world, shared_ptr<GameObject> object)
{

	if (object->GetType() == GameObjectType("Asteroid"))
	{
		shared_ptr<GameObject> explosion = CreateExplosion();
		if (object->GetScale() == 0.2f) // This checks that the asteroid destroyed is not a big one
		{
			//creates between zero and four small asteroids
			int random = 0 + rand() % ((4 + 1) - 0);
			CreateSmallAsteroids(random, object->GetPosition());
			mAsteroidCount += random;
		
		}

		if (object->GetScale() == 0.05f) {
			int liveProbability = 0 + rand() % ((6 + 1) - 0);
			if (liveProbability < 3 && mliveDropCount < 3)
			{
				CreateLives(object->GetPosition());
				mliveDropCount++;

			}


		}


		explosion->SetPosition(object->GetPosition());
		explosion->SetRotation(object->GetRotation());
		explosion->SetScale(0.5f);
		//mGameWorld->AddObject(SmallAsteroid);
		mGameWorld->AddObject(explosion);
		mAsteroidCount--;
		//	SmallAsteroid->SetPosition(object->GetPosition());
			//SmallAsteroid->SetPosition(object->GetRotation());

		if (mAsteroidCount <= 0)
		{
			SetTimer(500, START_NEXT_LEVEL);
		}
	}
	else if (object->GetType() == GameObjectType("PowerUp"))
	{

		singleOrBurst = false;
		mPowerUpCount--;

	}
	else if (object->GetType() == GameObjectType("Lives"))
	{

		mLiveCount++;

	}
	/*else if (object->GetType() == GameObjectType("Enemy"))
	{
		mEnemy->Reset();
		mEnemyLives--;
		if (mEnemyLives != 0)
		{
			mGameWorld->AddObject(mEnemy);
		}


	}*/


}

// PUBLIC INSTANCE METHODS IMPLEMENTING ITimerListener ////////////////////////

void Asteroids::OnTimer(int value)
{
	if (value == CREATE_NEW_PLAYER)
	{
		mSpaceship->Reset();
		mGameWorld->AddObject(mSpaceship);
	}

	if (value == START_NEXT_LEVEL)
	{
		mLevel++;
		int num_asteroids = 3 + mLevel;
		singleOrBurst = true;
		if (mPowerUpCount == 0)
		{
			CreatePwrUp();
		}


		//nextLevelLable->SetVisible(true);
		//nextLevelLable->SetVisible(false);

		CreateAsteroids(num_asteroids);
		//CreateLives();
	}

	if (value == SHOW_GAME_OVER)
	{
		mGameOverLabel->SetVisible(true);
	}

}

// PROTECTED INSTANCE METHODS /////////////////////////////////////////////////
shared_ptr<GameObject> Asteroids::CreateSpaceship()
{
	// Create a raw pointer to a spaceship that can be converted to
	// shared_ptrs of different types because GameWorld implements IRefCount
	mSpaceship = make_shared<Spaceship>();
	mSpaceship->SetBoundingShape(make_shared<BoundingSphere>(mSpaceship->GetThisPtr(), 4.0f));
	shared_ptr<Shape> bullet_shape = make_shared<Shape>("bullet.shape");
	mSpaceship->SetBulletShape(bullet_shape);
	Animation *anim_ptr = AnimationManager::GetInstance().GetAnimationByName("spaceship");
	shared_ptr<Sprite> spaceship_sprite =
		make_shared<Sprite>(anim_ptr->GetWidth(), anim_ptr->GetHeight(), anim_ptr);
	mSpaceship->SetSprite(spaceship_sprite);
	mSpaceship->SetScale(0.1f);
	// Reset spaceship back to centre of the world
	mSpaceship->Reset();
	// Return the spaceship so it can be added to the world
	return mSpaceship;

}

void Asteroids::CreateAsteroids(const uint num_asteroids)
{
	mAsteroidCount = num_asteroids;
	for (uint i = 0; i < num_asteroids; i++)
	{
		Animation *anim_ptr = AnimationManager::GetInstance().GetAnimationByName("asteroid1");
		shared_ptr<Sprite> asteroid_sprite
			= make_shared<Sprite>(anim_ptr->GetWidth(), anim_ptr->GetHeight(), anim_ptr);
		asteroid_sprite->SetLoopAnimation(true);
		shared_ptr<GameObject> asteroid = make_shared<Asteroid>();
		asteroid->SetBoundingShape(make_shared<BoundingSphere>(asteroid->GetThisPtr(), 10.0f));
		asteroid->SetSprite(asteroid_sprite);
		asteroid->SetScale(0.2f);
		mGameWorld->AddObject(asteroid);
	}
}

void Asteroids::CreateSmallAsteroids(const uint num_asteroids, GLVector3f position)
{
	//mAsteroidCount = num_asteroids;
	for (uint i = 0; i < num_asteroids; i++)
	{
		Animation *anim_ptr = AnimationManager::GetInstance().GetAnimationByName("asteroid1");
		shared_ptr<Sprite> asteroid_sprite
			= make_shared<Sprite>(anim_ptr->GetWidth(), anim_ptr->GetHeight(), anim_ptr);
		asteroid_sprite->SetLoopAnimation(true);
		shared_ptr<GameObject> asteroid = make_shared<Asteroid>();
		asteroid->SetBoundingShape(make_shared<BoundingSphere>(asteroid->GetThisPtr(), 5.0f));
		asteroid->SetSprite(asteroid_sprite);
		asteroid->SetScale(0.05f);
		asteroid->SetPosition(position);
		mGameWorld->AddObject(asteroid);

	}
}

void Asteroids::CreateLives(GLVector3f position) {
	Animation *anim_ptr = AnimationManager::GetInstance().GetAnimationByName("lives1");
	shared_ptr<Sprite> lives_sprite
		= make_shared<Sprite>(anim_ptr->GetWidth(), anim_ptr->GetHeight(), anim_ptr);
	lives_sprite->SetLoopAnimation(true);
	shared_ptr<GameObject> lives = make_shared<Lives>();
	lives->SetBoundingShape(make_shared<BoundingSphere>(lives->GetThisPtr(), 5.0f));
	lives->SetSprite(lives_sprite);
	lives->SetScale(0.2f);
	lives->SetPosition(position);
	mGameWorld->AddObject(lives);

}

void Asteroids::CreatePwrUp() {
	Animation *anim_ptr = AnimationManager::GetInstance().GetAnimationByName("pwr1");
	shared_ptr<Sprite> powerUp_sprite
		= make_shared<Sprite>(anim_ptr->GetWidth(), anim_ptr->GetHeight(), anim_ptr);
	powerUp_sprite->SetLoopAnimation(true);
	shared_ptr<GameObject> powerUp = make_shared<PowerUp>();
	powerUp->SetBoundingShape(make_shared<BoundingSphere>(powerUp->GetThisPtr(), 5.0f));
	powerUp->SetSprite(powerUp_sprite);
	powerUp->SetScale(0.25f);
	//lives->SetPosition(position);
	mGameWorld->AddObject(powerUp);

}

shared_ptr<GameObject> Asteroids::CreateEnemy()
{
	mEnemy = make_shared<Enemy>();
	mEnemy->SetBoundingShape(make_shared<BoundingSphere>(mEnemy->GetThisPtr(), 4.0f));
	shared_ptr<Shape> eBullet_shape = make_shared<Shape>("ebullet.shape");
	mEnemy->SetBulletShape(eBullet_shape);
	Animation *anim_ptr = AnimationManager::GetInstance().GetAnimationByName("enemy");
	shared_ptr<Sprite> enemy_sprite =
		make_shared<Sprite>(anim_ptr->GetWidth(), anim_ptr->GetHeight(), anim_ptr);
	mEnemy->SetSprite(enemy_sprite);
	mEnemy->SetScale(0.15f);
	mEnemy->AddPosition(10);
	// Reset spaceship back to centre of the world
	mEnemy->Reset();
	// Return the spaceship so it can be added to the world
	return mEnemy;
}

void Asteroids::OnWorldUpdated(GameWorld* world) {

	dy = mSpaceship->GetPosition().y - mEnemy->GetPosition().y;
	dx = mSpaceship->GetPosition().x - mEnemy->GetPosition().x;
	eAngle = atan2f(dy, dx);
	mEnemy->SetAngle(eAngle * 180 / M_PI);
	//mEnemy->Shoot();


	fps++;
	if (fps % 720 == 0)
	{
		mEnemy->Shoot();
	}
}
void Asteroids::CreateGUI()
{
	// Add a (transparent) border around the edge of the game display
	mGameDisplay->GetContainer()->SetBorder(GLVector2i(10, 10));
	// Create a new GUILabel and wrap it up in a shared_ptr
	mScoreLabel = make_shared<GUILabel>("Score: 0");
	// Set the vertical alignment of the label to GUI_VALIGN_TOP
	mScoreLabel->SetVerticalAlignment(GUIComponent::GUI_VALIGN_TOP);
	// Add the GUILabel to the GUIComponent  
	shared_ptr<GUIComponent> score_component
		= static_pointer_cast<GUIComponent>(mScoreLabel);
	mGameDisplay->GetContainer()->AddComponent(score_component, GLVector2f(0.0f, 0.95f));

	// Create a new GUILabel and wrap it up in a shared_ptr
	mLivesLabel = make_shared<GUILabel>("Lives: 3");
	// Set the vertical alignment of the label to GUI_VALIGN_BOTTOM
	mLivesLabel->SetVerticalAlignment(GUIComponent::GUI_VALIGN_BOTTOM);
	// Add the GUILabel to the GUIComponent  
	shared_ptr<GUIComponent> lives_component = static_pointer_cast<GUIComponent>(mLivesLabel);
	mGameDisplay->GetContainer()->AddComponent(lives_component, GLVector2f(0.0f, 0.0f));

	//// Create a new GUILabel and wrap it up in a shared_ptr
	//mlevelLabel = make_shared<GUILabel>("Level : 1");
	//// Set the vertical alignment of the label to GUI_VALIGN_BOTTOM
	//mlevelLabel->SetVerticalAlignment(GUIComponent::GUI_VALIGN_BOTTOM);
	//// Add the GUILabel to the GUIComponent  
	//shared_ptr<GUIComponent> level_component = static_pointer_cast<GUIComponent>(mlevelLabel);
	//mGameDisplay->GetContainer()->AddComponent(lives_component, GLVector2f(0.0f, 0.0f));

	// Create a new GUILabel and wrap it up in a shared_ptr
	mGameOverLabel = shared_ptr<GUILabel>(new GUILabel("GAME OVER"));
	// Set the horizontal alignment of the label to GUI_HALIGN_CENTER
	mGameOverLabel->SetHorizontalAlignment(GUIComponent::GUI_HALIGN_CENTER);
	// Set the vertical alignment of the label to GUI_VALIGN_MIDDLE
	mGameOverLabel->SetVerticalAlignment(GUIComponent::GUI_VALIGN_MIDDLE);
	// Set the visibility of the label to false (hidden)
	mGameOverLabel->SetVisible(false);
	// Add the GUILabel to the GUIContainer  
	shared_ptr<GUIComponent> game_over_component
		= static_pointer_cast<GUIComponent>(mGameOverLabel);
	mGameDisplay->GetContainer()->AddComponent(game_over_component, GLVector2f(0.5f, 0.5f));

}

void Asteroids::OnScoreChanged(int score)
{
	// Format the score message using an string-based stream
	std::ostringstream msg_stream;
	msg_stream << "Score: " << score;
	// Get the score message as a string
	std::string score_msg = msg_stream.str();
	mScoreLabel->SetText(score_msg);
}

void Asteroids::OnPlayerKilled(int lives_left)
{
	shared_ptr<GameObject> explosion = CreateExplosion();
	explosion->SetPosition(mSpaceship->GetPosition());
	explosion->SetRotation(mSpaceship->GetRotation());
	mGameWorld->AddObject(explosion);

	// Format the lives left message using an string-based stream
	std::ostringstream msg_stream;
	msg_stream << "Lives: " << lives_left;
	// Get the lives left message as a string
	std::string lives_msg = msg_stream.str();
	mLivesLabel->SetText(lives_msg);

	if (lives_left > 0) 
	{ 
		SetTimer(1000, CREATE_NEW_PLAYER); 
	}
	else
	{
		SetTimer(500, SHOW_GAME_OVER);
	}
}

shared_ptr<GameObject> Asteroids::CreateExplosion()
{
	Animation *anim_ptr = AnimationManager::GetInstance().GetAnimationByName("explosion");
	shared_ptr<Sprite> explosion_sprite =
		make_shared<Sprite>(anim_ptr->GetWidth(), anim_ptr->GetHeight(), anim_ptr);
	explosion_sprite->SetLoopAnimation(false);
	shared_ptr<GameObject> explosion = make_shared<Explosion>();
	explosion->SetSprite(explosion_sprite);
	explosion->Reset();
	return explosion;
}




