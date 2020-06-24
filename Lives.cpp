#include <stdlib.h>
#include "GameUtil.h"
#include "Lives.h"
#include "BoundingShape.h"
#include "Player.h"

Lives::Lives(void) : GameObject("Lives")
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

Lives::~Lives(void)
{
}



bool Lives::CollisionTest(shared_ptr<GameObject> o)
{
	//if (GetType() == o->GetType()) return false;
	if (o->GetType() != GameObjectType("Bullet")) return false;
	if (mBoundingShape.get() == NULL) return false;
	if (o->GetBoundingShape().get() == NULL) return false;
	return mBoundingShape->CollisionTest(o->GetBoundingShape());

}

void Lives::OnCollision(const GameObjectList& objects)
{

	mWorld->FlagForRemoval(GetThisPtr());

}