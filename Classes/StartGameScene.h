#ifndef __STARTGAME_SCENE_H__
#define __STARTGAME_SCENE_H__

#include "cocos2d.h"

class StartGameLayer : public cocos2d::CCLayerColor
{
public:
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  

    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::CCScene* scene();
    
    // a selector callback
	// 游戏开始关闭菜单按钮回调函数
    void menuCloseCallback(CCObject* pSender);

	// a start game selector callback
	// Play回调函数
	void menuPlayCallback(CCObject* pSender);

	// High Scores回调函数
	void menuHighScoresCallback(CCObject* pSender);

	//	About回调函数
	void menuAboutCallback(CCObject* pSender);
    
    // implement the "static node()" method manually
    CREATE_FUNC(StartGameLayer);
};

#endif // __HELLOWORLD_SCENE_H__
