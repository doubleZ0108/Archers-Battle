#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
enum { stop_music, continue_music, reply_music };
//用于声音引擎的控制
//stop_music代表彻底停止当前音乐，如果点击play键会从头播放当前音乐
//continue_music代表暂停当前音乐,如果点击play键会接着刚才暂停的后面继续播放
//reply_music代表重新播放当前音乐

class HelloWorld : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    
	void ScenePrinter();
	void MusicPrinter();
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
	void menuStartButton(cocos2d::Ref* pSender);
	void menuSettingButton(cocos2d::Ref* pSender);
	void menuHelpButton(cocos2d::Ref* pSender);


    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);
	bool isCanReach(float x, float y);
};

#endif // __HELLOWORLD_SCENE_H__
