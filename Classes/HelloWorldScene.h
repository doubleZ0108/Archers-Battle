#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
enum { stop_music, continue_music, reply_music };
//������������Ŀ���
//stop_music������ֹͣ��ǰ���֣�������play�����ͷ���ŵ�ǰ����
//continue_music������ͣ��ǰ����,������play������Ÿղ���ͣ�ĺ����������
//reply_music�������²��ŵ�ǰ����

class HelloWorld : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    
	void Default_input();
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
