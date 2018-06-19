#ifndef _SETTING_H__
#define _SETTING_H__
#include "cocos2d.h"

class SettingScene : public cocos2d::Scene
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init();
	// a selector callback
	void menuHellowWorldScene(cocos2d::Ref* pSender);

	void ScenePrinter();
	void MusicPrinter();
	void LanguagePrinter();
	void SizePrinter();
	//music setting
	void play(cocos2d::Object* pSender);
	void stop(cocos2d::Object* pSender);

	void pause(cocos2d::Object* pSender);
	//language setting
	void language_change_tochinese(cocos2d::Object* pSender);
	void language_change_toenglish(cocos2d::Object* pSender);

	void size_change_tosmall(cocos2d::Object* pSender);
	void size_change_tomiddle(cocos2d::Object* pSender);
	void size_change_tolarge(cocos2d::Object* pSender);
	void size_change();

	// implement the "static create()" method manually
	CREATE_FUNC(SettingScene);
};

#endif // _SETTINGSCENE_H_
