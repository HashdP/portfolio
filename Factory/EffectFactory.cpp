#include "EffectFactory.hpp"
#include "ObjectLayer.hpp"
#include "Effect_PunchHit.hpp"
#include "Effect_BulletHit.hpp"
#include "Effect_DirtShotHit.hpp"
#include "Effect_FireBallHit.hpp"
#include "Effect_MuzzleFlash.hpp"

EffectFactory* EffectFactory::instance = nullptr;

EffectFactory* EffectFactory::getInstance()
{
	if (instance == nullptr)
		instance = new EffectFactory();
	return instance;
}

Effect* EffectFactory::CreateEffect(ObjectLayer* objectLayer, EffectID effectID)
{
	Effect* effect = nullptr;

	switch (effectID)
	{
	case EffectID::PunchHit:      effect = Effect_PunchHit::create(); break;
	case EffectID::BulletHit:     effect = Effect_BulletHit::create(); break;
	case EffectID::DirtShotHit:   effect = Effect_DirtShotHit::create(); break;
	case EffectID::FireBallHit:   effect = Effect_FireBallHit::create(); break;
	case EffectID::MuzzleFlash:   effect = Effect_MuzzleFlash::create(); break;
	}

	objectLayer->addChild(effect, INT_MAX);

	return effect;
}
