#include "MyScene.h"
#include "HelloWorldScene.h"
USING_NS_CC;

Scene* MyScene::createScene()
{
	return MyScene::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
	printf("Error while loading: %s\n", filename);
	printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool MyScene::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Scene::init())
	{
		return false;
	}
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	///////////////////////////////////
	auto closeItem = MenuItemImage::create(
		"CloseNormal.png",
		"CloseSelected.png",
		CC_CALLBACK_1(MyScene::menuHellowWorldScene, this));

	if (closeItem == nullptr ||
		closeItem->getContentSize().width <= 0 ||
		closeItem->getContentSize().height <= 0)
	{
		problemLoading("'CloseNormal.png' and 'CloseSelected.png'");
	}
	else
	{
		float x = origin.x + visibleSize.width - closeItem->getContentSize().width / 2;
		float y = origin.y + closeItem->getContentSize().height / 2;
		closeItem->setPosition(Vec2(x, y));
	}
	// create menu, it's an autorelease object
	auto menu = Menu::create(closeItem, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 1);


	auto *layer1 = Layer::create();
	auto *pBackground = Sprite::create("MyScene.jpg");
	pBackground->setAnchorPoint(Vec2(0.5f, 0.5f));
	pBackground->setPosition(1280 / 2, 720 / 2);
	layer1->addChild(pBackground);

	auto *layer2 = Layer::create();
	auto *plane = Sprite::create("plane.jpg");
	plane->setScale(0.5f);

	auto rect = Director::getInstance()->getOpenGLView()->getVisibleRect();
	float x = rect.origin.x + rect.size.width / 2;
	float y = rect.origin.y;
	plane->setAnchorPoint(Vec2(0.5f, 0.0f));
	plane->setPosition(Vec2(x, y));
	layer2->addChild(plane);

	auto *layer3 = Layer::create();
	auto *number = LabelTTF::create("13400", "Arial", 32);
	y = rect.origin.y + rect.size.height*(4.0 / 5.0);
	number->setAnchorPoint(Vec2(0.5f, 0.5f));
	number->setPosition(Vec2(x, y));
	number->setColor(Color3B::RED);
	layer3->addChild(number);

	this->addChild(layer1);
	this->addChild(layer2);
	this->addChild(layer3);

	//今后的场景加在这里
	return true;
}


void MyScene::menuHellowWorldScene(Ref* pSender)
{
	Director::getInstance()->popScene();
}