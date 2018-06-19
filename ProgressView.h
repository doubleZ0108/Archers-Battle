#ifndef __PROGRESSVIEW_H__  
#define __PROGRESSVIEW_H__  

#include "cocos2d.h"  
using namespace cocos2d;

class ProgressView : public CCNode
{
public:
	ProgressView();

public:
	void setBackgroundTexture(const char *pName);//设置血条
	void setForegroundTexture(const char *pName);//设置血量
	void setTotalProgress(float total);//总血量
	void setCurrentProgress(float progress);//当前血量
	void setpoint(Point point);//设置坐标

private:
	void setForegroundTextureRect(const CCRect &rect);

private:
	CCSprite * m_progressBackground;
	CCSprite *m_progressForeground;
	float m_totalProgress;
	float m_currentProgress;
	float m_scale;
};

#endif