#pragma once
#include "cocos2d.h"

enum EffectID
{
	PunchHit,
	BulletHit,
	DirtShotHit,
	FireBallHit,
	MuzzleFlash,
};

class Effect : public cocos2d::Node
{
private:
	float deltaTime;
	float duration;

public:
	virtual ~Effect() = default;

	/*
	duration秒後に自動的に削除する
	*/
	virtual bool init(float duration);

	virtual void update(float delta) override;
};