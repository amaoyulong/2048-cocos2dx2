#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"

USING_NS_CC;

#define DIRECT_DOWN		0
#define DIRECT_UP		1
#define DIRECT_LEFT		2
#define DIRECT_RIGHT	3


class Game : public cocos2d::CCLayer
{
public:
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  

    virtual void onExit();

    virtual void onEnter();

    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::CCScene* scene();
    
    // a selector callback
    void menuCloseCallback(CCObject* pSender);

    void restart();

    void toGameOverTestScene();
    
    // implement the "static node()" method manually
    CREATE_FUNC(Game);

public:
	virtual bool ccTouchBegan(CCTouch *touch, CCEvent * pEvent);
    virtual void ccTouchEnded(CCTouch *touch, CCEvent * pEvent);

private:
	int drawBoard(float w, float h);  
	int randomCreateCard(int direct=0 , int index = -1); 
	int drawMenu(CCNode * p, int eleX, int eleY);
	bool checkIfGameOver();

private:
	void doDirect(CCPoint &p, int direct);


//相应事件
private:
	CCPoint pBegin, pEnd;
	bool bIfInit;

//16个card数据
private:	
	int mValues[16];
	int useNum;

	int iNowScore;
	int iHistoryScore;

//绘制board的几个数据 
private:
	int iFrameLeft;  //边框左
	int iFrameRight;
	int iInterval;	//card 间隔
	int iCardLength; //边长
	int iBoardLength;
	float angle;	
	CCPoint pBoard;	//board中心

	CCLayerColor* boardlayer;

	int iBottomLeft;//屏幕下边还剩多少高度
	int iTopLeft;//屏幕上边还剩多少高度

	CCLabelTTF * pScroeNum;
	CCLabelTTF * pHisScroeNum;

	CCLayerColor * pGameMenu; //菜单按钮

};

#endif // __GAME_SCENE_H__
