#include "Help.h"
#include "HelloWorldScene.h"

USING_NS_CC;

extern bool language_flag;   //true->English   false->Chinese
extern char *FontToUTF8(const char* font);
//it is define in another .cpp file 
//and it is used to change character

Scene* HelpScene::createScene()
{
	return HelpScene::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
	printf("Error while loading: %s\n", filename);
	printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelpScene.cpp\n");
}

// on "init" you need to initialize your instance
bool HelpScene::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Scene::init())
	{
		return false;
	}

	ScenePrinter();
	PagePrinter();

	return true;
}

void HelpScene::ScenePrinter()
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto rect = Director::getInstance()->getOpenGLView()->getVisibleRect();
	float x = rect.origin.x + rect.size.width / 2;
	float y = rect.origin.y + rect.size.height / 2;

	///////////////////////////////////////////
	//add Setting_Background
	/*auto *background = Sprite::create("Help_Background.png");
	background->setPosition(Vec2(x, y));
	this->addChild(background);

	Texture2D * pic = Director::getInstance()->getTextureCache()->addImage("blue.png");
	background->setTexture(pic);*/

	///////////////////////////////////
	//a return button which click to back to HelloWorldScene
	auto *return_button = MenuItemImage::create(
		"Scene/Buttons/backtoupper.png",
		"Scene/Buttons/backtoupper_select.png",
		CC_CALLBACK_1(HelpScene::menuHellowWorldScene, this));

	auto *preturn = Menu::create(return_button, NULL);
	x = rect.origin.x + rect.size.width*(10.0f / 11.0f);
	y = rect.origin.y + rect.size.height*(1.0f / 10.0f);
	preturn->setPosition(Vec2(x, y));
	preturn->setScale(1.0f);
	this->addChild(preturn,10);

	//��ըЧ��*/
	/*CCParticleSystem* particleSystem = CCParticleExplosion::create();
	particleSystem->setTexture(CCTextureCache::sharedTextureCache()->addImage("fire.jpg"));
	addChild(particleSystem);*/
	//����Ч��  
	/*CCParticleSystem* particleSystem = CCParticleFire::create();
	particleSystem->setTexture(CCTextureCache::sharedTextureCache()->addImage("fire.jpg"));
	addChild(particleSystem);*/
	//�̻�Ч��  
	/*CCParticleSystem* particleSystem = CCParticleFireworks::create();
	particleSystem->setTexture(CCTextureCache::sharedTextureCache()->addImage("fire.jpg"));
	addChild(particleSystem);*/
	//��ϵЧ��  
	/*CCParticleSystem* particleSystem = CCParticleGalaxy::create();
	particleSystem->setTexture(CCTextureCache::sharedTextureCache()->addImage("fire.jpg"));
	addChild(particleSystem);*/
	//����Ч��  
	/*CCParticleSystem* particleSystem = CCParticleRain::create();
	particleSystem->setTexture(CCTextureCache::sharedTextureCache()->addImage("fire.jpg"));
	addChild(particleSystem);*/
	//��ѩЧ��  
	/*CCParticleSystem* particleSystem = CCParticleSnow::create();
	particleSystem->setTexture(CCTextureCache::sharedTextureCache()->addImage("snow.jpg"));
	addChild(particleSystem);*/
	//����Ч��  ����Ƚ���
	/*CCParticleSystem* particleSystem = CCParticleSpiral::create();
	particleSystem->setTexture(CCTextureCache::sharedTextureCache()->addImage("fire.jpg"));
	addChild(particleSystem);*/

	/*CCSprite* sp = CCSprite::create("fire.jpg");
	x = rect.origin.x + rect.size.width / 2;
	y = rect.origin.y + rect.size.height / 2;
	sp->setPosition(Vec2(x, y));
	auto gridNodeTarget = NodeGrid::create();
	this->addChild(gridNodeTarget);
	gridNodeTarget->addChild(sp);*/
	

	

	/*CCSprite* sp = CCSprite::create("fire.jpg");
	x = rect.origin.x + rect.size.width / 2;
	y = rect.origin.y + rect.size.height / 2;
	sp->setPosition(Vec2(x, y));
	auto gridNodeTarget = NodeGrid::create();
	this->addChild(gridNodeTarget);
	gridNodeTarget->addChild(sp);*/


	//3D�ζ�����Ч  ������ʧ���ǻ����
	/* CCActionInterval* shaky3D = CCShaky3D::create(5, CCSize(10, 10), 15, false);
	gridNodeTarget->runAction(shaky3D);*/

	//Һ����Ч  ������ʧ���ǻ����
	/*CCActionInterval* liquid = CCLiquid::create(5, CCSize(10, 10), 4, 20);
	gridNodeTarget->runAction(liquid);*/

	//��שϴ����Ч   ���ɧ  �����������ʧ
	//    CCActionInterval* shuffle = CCShuffleTiles::create(5, CCSize(50, 50), 50); 
	//    gridNodeTarget->runAction(shuffle); 

	//�����Ч��,�����½ǵ����Ͻ�   ɧ*2 ֱ�������������ʧ
	/*CCActionInterval* fadeOutTRTiles = CCFadeOutTRTiles::create(5, CCSize(50, 50));
	gridNodeTarget->runAction(fadeOutTRTiles); */

	//�����Ч���������Ͻǵ����½� 
	//    CCActionInterval* fadeOutBLTiles  = CCFadeOutBLTiles::create(5, CCSize(50, 50)); 
	//    gridNodeTarget->runAction(fadeOutBLTiles); 

	//�۵�Ч�� ���µ���  �����������ʧ
	//    CCActionInterval* fadeOutUpTiles = CCFadeOutUpTiles::create(5, CCSize(10, 10)); 
	//    gridNodeTarget->runAction(fadeOutUpTiles); 


	//������ʧ��Ч 
	//    CCActionInterval* turnOffFiels = CCTurnOffTiles::create(4, CCSize(50, 50)); 
	//    gridNodeTarget->runAction(turnOffFiels); 

	//�����ķ�����Ч  ������ʧ�����Ҳ������
	//    CCActionInterval* jumpTiles = CCJumpTiles3D::create(5, CCSize(20, 20), 5, 20); 
	//    gridNodeTarget->runAction(jumpTiles); 
}

void HelpScene::PagePrinter()
{
	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true); //�����Ƿ����´��ݴ���  
	listener->onTouchBegan = [&](Touch * touch, Event * unused_event)
	{
		return true;
	};
	listener->onTouchEnded = [&](Touch * touch, Event * unused_event)
	{

	};
	Director::getInstance()->getEventDispatcher()->
		addEventListenerWithSceneGraphPriority(listener, this);

	//�����ؿ�  
	Size WinSize = Director::getInstance()->getWinSize();

	pageView = PageView::create();
	pageView->setSize(Size(WinSize.width, WinSize.height));
	pageView->setPosition(Vec2::ZERO);
	//8������ѡ�  
	auto rect = Director::getInstance()->getOpenGLView()->getVisibleRect();
	float x = rect.origin.x + rect.size.width / 2;
	float y = rect.origin.y + rect.size.height / 2;

	for (int i = 0; i < 8; i++)
	{
		auto layout = Layout::create();
		layout->setSize(Size(WinSize.width, WinSize.height));

		/*auto imageView = ImageView::create(StringUtils::format("lv%d.png", i));
		imageView->setSize(Size(pWidth, pHeight));
		imageView->setPosition(Vec2(layout->getSize().width / 2, layout->getSize().height / 2));
		layout->setScale(3.0f);
		layout->addChild(imageView);*/
		auto sprite = Sprite::create(StringUtils::format("lv%d.png", i));
		sprite->setScale(3.0f);
		y = rect.origin.y + rect.size.height / 2;
		sprite->setPosition(Vec2(x, y));
		layout->addChild(sprite);

		auto label = Label::create(StringUtils::format("lv%d", i), "Arial", 46);
		label->setColor(Color3B(192, 192, 192));
		y = rect.origin.y + rect.size.height*(1.0f / 3.0f);
		label->setPosition(Vec2(x, y));
		layout->addChild(label);

		pageView->addPage(layout);

	}

	auto JumpSceneCallback_lambda= [&](Ref * pSender, PageView::EventType type)
	{};
	pageView->addEventListener(JumpSceneCallback_lambda);

	this->addChild(pageView);
}

void HelpScene::Func(Ref* pSender)
{
	//this->runAction(PageTurn3D::create(3.0f,Size(15,10)));
}
void HelpScene::menuHellowWorldScene(Ref* pSender)
{
	auto sc = HelloWorld::createScene();        //���зָ������л�����
	auto reScene = TransitionFadeBL::create(1.0f, sc);
	Director::getInstance()->replaceScene(reScene);
}


