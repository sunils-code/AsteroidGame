#include "GameUtil.h"
#include "GameWorld.h"
#include "Bullet.h"
#include "Spaceship.h"
#include "BoundingSphere.h"

using namespace std;

// PUBLIC INSTANCE CONSTRUCTORS ///////////////////////////////////////////////

/**  Default constructor. */
Spaceship::Spaceship()
	: GameObject("Spaceship"), mThrust(0)
{
}

/** Construct a spaceship with given position, velocity, acceleration, angle, and rotation. */
Spaceship::Spaceship(GLVector3f p, GLVector3f v, GLVector3f a, GLfloat h, GLfloat r)
	: GameObject("Spaceship", p, v, a, h, r), mThrust(0)
{
}

/** Copy constructor. */
Spaceship::Spaceship(const Spaceship& s)
	: GameObject(s), mThrust(0)
{
}

/** Destructor. */
Spaceship::~Spaceship(void)
{
}

// PUBLIC INSTANCE METHODS ////////////////////////////////////////////////////

/** Update this spaceship. */
void Spaceship::Update(int t)
{
	// Call parent update function
	GameObject::Update(t);
}

/** Render this spaceship. */
void Spaceship::Render(void)
{
	if (mSpaceshipShape.get() != NULL) mSpaceshipShape->Render();

	// If ship is thrusting
	if ((mThrust > 0) && (mThrusterShape.get() != NULL)) {
		mThrusterShape->Render();
	}

	GameObject::Render();
}

/** Fire the rockets. */
void Spaceship::Thrust(float t)
{
	mThrust = t;
	// Increase acceleration in the direction of ship
	mAcceleration.x = mThrust * cos(DEG2RAD*mAngle);
	mAcceleration.y = mThrust * sin(DEG2RAD*mAngle);
}

/** Set the rotation. */
void Spaceship::Rotate(float r)
{
	mRotation = r;
}

/** Shoot a bullet. */
void Spaceship::Shoot(string shootingStyle)
{
	if (shootingStyle == "single")
	{
		// Check the world exists
		if (!mWorld) return;
		// Construct a unit length vector in the direction the spaceship is headed
		GLVector3f spaceship_heading(cos(DEG2RAD*mAngle), sin(DEG2RAD*mAngle), 0);
		spaceship_heading.normalize();
		// Calculate the point at the node of the spaceship from position and heading
		GLVector3f bullet_position = mPosition + (spaceship_heading * 4);

		// Calculate how fast the bullet should travel
		float bullet_speed = 30;
		// Construct a vector for the bullet's velocity
		GLVector3f bullet_velocity = mVelocity + spaceship_heading * bullet_speed;
		// Construct a new bullet
		shared_ptr<GameObject> bullet
		(new Bullet(bullet_position, bullet_velocity, mAcceleration, mAngle, 0, 2000));
		bullet->SetBoundingShape(make_shared<BoundingSphere>(bullet->GetThisPtr(), 2.0f));
		bullet->SetShape(mBulletShape);
		// Add the new bullet to the game world
		mWorld->AddObject(bullet);
	}
	else if (shootingStyle == "burst")
	{
		// Check the world exists
		if (!mWorld) return;
		// Construct a unit length vector in the direction the spaceship is headed
		GLVector3f spaceship_heading(cos(DEG2RAD*mAngle), sin(DEG2RAD*mAngle), 0);
		spaceship_heading.normalize();
		// Calculate the point at the node of the spaceship from position and heading
		GLVector3f bullet_position = mPosition + (spaceship_heading * 4);
		GLVector3f bullet_position1 = mPosition + (spaceship_heading * 8);
		GLVector3f bullet_position2 = mPosition + (spaceship_heading * 12);
		//GLVector3f bullet_position2 = mPosition - GLVector3f(-10,0,0) + (spaceship_heading * 4);
		//GLVector3f bullet_position3 = mPosition + (spaceship_heading * 4);
		// Calculate how fast the bullet should travel
		float bullet_speed = 30;
		// Construct a vector for the bullet's velocity
		GLVector3f bullet_velocity = mVelocity + spaceship_heading * bullet_speed;
		// Construct a new bullet
		shared_ptr<GameObject> bullet
		(new Bullet(bullet_position, bullet_velocity, mAcceleration, mAngle, 0, 2000));
		shared_ptr<GameObject> bullet1
		(new Bullet(bullet_position1, bullet_velocity, mAcceleration, mAngle, 0, 2000));
		shared_ptr<GameObject> bullet2
		(new Bullet(bullet_position2, bullet_velocity, mAcceleration, mAngle, 0, 2000));
		bullet->SetBoundingShape(make_shared<BoundingSphere>(bullet->GetThisPtr(), 2.0f));
		bullet->SetShape(mBulletShape);
		bullet1->SetBoundingShape(make_shared<BoundingSphere>(bullet->GetThisPtr(), 2.0f));
		bullet1->SetShape(mBulletShape);
		bullet2->SetBoundingShape(make_shared<BoundingSphere>(bullet->GetThisPtr(), 2.0f));
		bullet2->SetShape(mBulletShape);
		// Add the new bullet to the game world
		mWorld->AddObject(bullet);
		mWorld->AddObject(bullet1);
		mWorld->AddObject(bullet2);
	}


}


bool Spaceship::CollisionTest(shared_ptr<GameObject> o)
{
	if (o->GetType() != GameObjectType("Asteroid") && o->GetType() != GameObjectType("EnemyBullet")) return false;
	if (mBoundingShape.get() == NULL) return false;
	if (o->GetBoundingShape().get() == NULL) return false;
	return mBoundingShape->CollisionTest(o->GetBoundingShape());
}

void Spaceship::OnCollision(const GameObjectList &objects)
{
	mWorld->FlagForRemoval(GetThisPtr());
}