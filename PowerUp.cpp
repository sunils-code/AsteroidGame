#include <stdlib.h>
#include "GameUtil.h"
#include "PowerUp.h"
#include "BoundingShape.h"
#include "Player.h"

PowerUp::PowerUp(void) : GameObject("PowerUp")
{
	mAngle = rand() % 360;
	mRotation = rand() % 20;
	mPosition.x = rand() / 2;
	mPosition.y = rand() / 2;
	mPosition.z = 0.0;
	mVelocity.x = 10.0 * cos(DEG2RAD*mAngle);
	mVelocity.y = 10.0 * sin(DEG2RAD*mAngle);
	mVelocity.z = 0.0;
}

PowerUp::~PowerUp(void)
{
}



bool PowerUp::CollisionTest(shared_ptr<GameObject> o)
{
	if (o->GetType() != GameObjectType("Bullet")) return false;
	if (mBoundingShape.get() == NULL) return false;
	if (o->GetBoundingShape().get() == NULL) return false;
	return mBoundingShape->CollisionTest(o->GetBoundingShape());

}

void PowerUp::OnCollision(const GameObjectList& objects)
{

	mWorld->FlagForRemoval(GetThisPtr());

}