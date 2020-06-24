#ifndef __LIVES_H__
#define __LIVES_H__

#include "GameObject.h"

class Lives : public GameObject
{
public:
	Lives(void);
	~Lives(void);

	bool CollisionTest(shared_ptr<GameObject> o);
	void OnCollision(const GameObjectList& objects);

};
#endif // !__LIVES_H__
