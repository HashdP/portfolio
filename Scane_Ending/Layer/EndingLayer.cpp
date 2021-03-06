#pragma execution_character_set("utf-8")

#include "EndingLayer.hpp"
#include "Ending0.hpp"
#include "Ending1.hpp"
#include "Ending2.hpp"
#include "Ending3.hpp"
#include "Ending4.hpp"
#include "SaveData.hpp"
#include "InputManager.hpp"
#include "GamePlayScene.hpp"
#include "Player.hpp"
#include "Skull.hpp"
#include "SoundManager.hpp"
#include "myutil.hpp"
#include "GameManager.hpp"
#include "CharaData.hpp"

USING_NS_CC;

EndingLayer* EndingLayer::create(int endingNumber)
{
	EndingLayer* pRet = new EndingLayer();

	if (pRet && pRet->init(endingNumber))
	{
		pRet->autorelease();
		return pRet;
	}
	else
	{
		delete pRet;
		pRet = nullptr;
		return nullptr;
	}
}

bool EndingLayer::init(int endingNumber)
{
	if (!Node::init())
		return false;

	scheduleUpdate();

	animEnd = false;
	processActive = true;

	if (endingNumber == 0)
		mainLabel = Label::createWithTTF(TTFConfig("Fonts/misaki_gothic.ttf", 40), "GAMEOVER");
	else
		mainLabel = Label::createWithTTF(TTFConfig("Fonts/misaki_gothic.ttf", 40), "ENDING" + std::to_string(endingNumber));
	mainLabel->setPositionY(290);
	mainLabel->setOpacity(0);
	this->addChild(mainLabel);

	Node* fieldNode;
	switch (endingNumber)
	{
	case 0: fieldNode = Ending0::create(); break;
	case 1: fieldNode = Ending1::create(); break;
	case 2: fieldNode = Ending2::create(); break;
	case 3: fieldNode = Ending3::create(); break;
	case 4: fieldNode = Ending4::create(); break;
	}
		
	fieldNode->setScale(2);
	fieldNode->setPositionY(50);
	this->addChild(fieldNode);

	if (endingNumber == 0)
	{
		mainCharaSprite = Skull::create(nullptr);
	}
	else
	{
		mainCharaSprite = Player::create(nullptr, nullptr, CharaData("Player"));
	}

	mainCharaSprite->SetState("Stay");
	mainCharaSprite->unscheduleUpdate();
	mainCharaSprite->setPositionY(-30);
	fieldNode->addChild(mainCharaSprite);

	ifs.open("Text/ending" + std::to_string(endingNumber) + ".txt");

	//1回切り捨て
	std::string str;
	std::getline(ifs, str);

	std::getline(ifs, hukidashiText);

	std::getline(ifs, str);

	std::string text;
	while (std::getline(ifs, str))
	{
		if (text != "")
			text += "\n";

		text += str;
	}

	Label* description = Label::createWithTTF(TTFConfig("Fonts/misaki_gothic.ttf", 20), text);
	description->setPositionY(-230);
	description->setAlignment(TextHAlignment::CENTER);
	description->setLineHeight(25);
	description->setWidth(450);
	description->setOpacity(0);
	this->addChild(description);

	int textLength = description->getStringLength();
	for (int i = 0; i < textLength; ++i)
	{
		Sprite* letter = description->getLetter(i);

		//改行文字からSpriteは得られない
		if (letter)
		{
			letters.push_back(letter);
		}
	}

	this->runAction(
		Sequence::create(
			DelayTime::create(4.0f),
			CallFuncN::create([this](Node*) { mainCharaSprite->Speak(hukidashiText, -1.0f); }),
			DelayTime::create(1.0f),
			CallFuncN::create([this](Node*) { LetterAnimation(); }),
			NULL
		)
	);

	//Ending4を見たら最初に戻す
	if (endingNumber != 0)
	{
		SaveData::getInstance()->SaveContinueFlag(false);
		SaveData::getInstance()->SaveEndingNumber(endingNumber == 4 ? 0 : endingNumber);
		SaveData::getInstance()->SaveInterruptionPoint(Point::ZERO);
		SaveData::getInstance()->SavePlayerPosition(myutil::ConvertFloorPosToPos(12, 3));
		SaveData::getInstance()->SaveItem();
		SaveData::getInstance()->SaveWeapon();
		SaveData::getInstance()->SaveMap();
	}

	return true;
}

void EndingLayer::update(float delta)
{
	if (processActive && InputManager::getInstance()->GetAnyInputDown())
	{
		if (animEnd)
		{
			SaveData::getInstance()->finalize();
			GameManager::getInstance()->ReplaceGamePlayScene();
			processActive = false;
		}
		else
		{
			mainCharaSprite->Speak(hukidashiText, -1.0f);

			this->stopAllActions();

			for (Sprite* letter : letters)
			{
				letter->stopAllActions();
				letter->setOpacity(255);
			}

			EndAnim();
		}
	}
}

void EndingLayer::LetterAnimation()
{
	for (unsigned i = 0; i < letters.size(); ++i)
	{
		if (i < letters.size() - 1)
		{
			letters[i]->runAction(
				Sequence::create(
					DelayTime::create(0.1f * i),
					CallFuncN::create([this, i](Node*) { letters[i]->setOpacity(255); SoundManager::getInstance()->Play2DSound(SoundID::Ending_Text_Appear); }),
					NULL
				)
			);
		}
		else
		{
			//ラスト一文字
			letters[i]->runAction(
				Sequence::create(
					DelayTime::create(0.1f * i),
					CallFuncN::create(
						[this, i](Node*)
			{
				letters[i]->setOpacity(255);
				mainLabel->runAction(Sequence::create(
					CallFuncN::create([this](Node*) { SoundManager::getInstance()->Play2DSound(SoundID::Ending_Text_Appear); }),
					DelayTime::create(1.0f),
					CallFuncN::create([this](Node*) { EndAnim(); }),
					NULL
				));
			}),
					NULL
				)
			);
		}
	}
}

void EndingLayer::EndAnim()
{
	mainLabel->setOpacity(255);
	mainLabel->setScale(1.1f);
	mainLabel->runAction(
		Sequence::create(
			ScaleTo::create(0.1f, 1.0f),
			CallFuncN::create([](Node*) { SoundManager::getInstance()->Play2DSound(SoundID::Ending_Result_Appear); }),
			NULL
		)
	);

	animEnd = true;
}