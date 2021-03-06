#pragma once
#include "Bound.hpp"
#include "MoveAI.hpp"

class ShotBound : public Bound
{
private:
	ShotBound() = default;

	MoveAI moveAI;

public:
	virtual ~ShotBound() = default;
	static ShotBound* create(ObjectLayer* objectLayer, CharacterID charaID, int level, const CharaData& charaData);
	virtual bool init(ObjectLayer* objectLayer, CharacterID charaID, int level, const CharaData& charaData);

	virtual void CharacterUpdate(float delta) override;

	virtual void OnRemove() override;
};
