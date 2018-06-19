#include "ExpProgress.h"

using namespace cocos2d::ui;

ExpProgress::ExpProgress()
{
}

ExpProgress::~ExpProgress()
{
}

bool ExpProgress::init()
{
	expProgressView = new ProgressView;
	expProgressView->setPosition(ccp(640, 690));
	expProgressView->setScale(2, 1.2);
	expProgressView->setBackgroundTexture("energyback.png");
	expProgressView->setForegroundTexture("energyfore.png");
	expProgressView->setTotalProgress(1);
	expProgressView->setCurrentProgress(0);
	addChild(expProgressView);

	lv = Label::create("LV 1", "Arial", 40);
	lv->setPosition(ccp(640, 690));
	addChild(lv);

	return true;
}

void ExpProgress::ExpChange(int exp, int limit)
{
	expProgressView->setCurrentProgress((float)exp / limit);
}

void ExpProgress::LvChange(int level)
{
	removeChild(lv);
	if (level == 11)
	{
		lv = Label::create("LV MAX", "Arial", 40);
	}
	else
	{
		lv = Label::create(StringUtils::format("LV %d", level), "Arial", 40);
	}
	lv->setPosition(ccp(640, 690));
	addChild(lv);
}

void ExpProgress::ButtonAppear(Player *player)
{
	c1 = rand() % 3 + 1;
	choice1 = Button::create(StringUtils::format("choice%d.png", c1));
	choice1->setPosition(ccp(440, 150));
	choice1->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type)
	{
		Choices(player, c1);
		ButtonRemove();
	});
	addChild(choice1);

	c2 = rand() % 3 + 1;
	//while (c2 == c1)
	//	c2 = rand() % 3 + 1;
	choice2 = Button::create(StringUtils::format("choice%d.png", c2));
	choice2->setPosition(ccp(640, 150));
	choice2->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type)
	{
		Choices(player, c2);
		ButtonRemove();
	});
	addChild(choice2);

	c3 = rand() % 3 + 1;
	//while (c3 == c1 || c3 == c2)
	//	c2 = rand() % 3 + 1;
	choice3 = Button::create(StringUtils::format("choice%d.png", c3));
	choice3->setPosition(ccp(840, 150));
	choice3->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type)
	{
		Choices(player, c3);
		ButtonRemove();
	});
	addChild(choice3);
}

void ExpProgress::ButtonRemove()
{
	removeChild(choice1);
	removeChild(choice2);
	removeChild(choice3);

}

void ExpProgress::Choices(Player* player, int i)
{
	log("choice %d", i);
	switch (i)
	{
	case 1:
		break;
	case2:
		break;
	case3:
		break;
	}
}