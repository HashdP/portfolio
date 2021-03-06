#pragma once
#include "cocos2d.h"

/*
シングルトンのInputManagerのUpdate関数をまわす
*/

class MyScene : public cocos2d::Scene
{
public:
	virtual ~MyScene() = default;

	CREATE_FUNC(MyScene);

	virtual bool init() override;
	virtual void update(float delta) override;
};
