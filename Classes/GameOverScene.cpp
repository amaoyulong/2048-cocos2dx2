#include "GameOverScene.h"
#include "Constants.h"
#include "GameScene.h"

USING_NS_CC;

CCScene* GameOver::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    GameOver *layer = GameOver::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool GameOver::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }
    
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();


    //背景色
    CCLayerColor* bglayer = CCLayerColor::create(COLOR_SCENE_BACKGTOUND_V4, visibleSize.width, visibleSize.height);
    bglayer->ignoreAnchorPointForPosition(false);
    bglayer->setPosition(CCPointMake(visibleSize.width / 2, visibleSize.height / 2));
    addChild(bglayer, 1, 1);

    //菜单两个字  高度为200往上
    CCLabelTTF * pMenuString = CCLabelTTF::create("Game Over!", "Georgia-Bold", 100);
    pMenuString->setColor(COLOR_CARD_NUM_BLACK_V3);
    pMenuString->setPosition(ccp(visibleSize.width/2, visibleSize.height - 200));
    bglayer->addChild(pMenuString);


    //继续按钮，宽600，高200，上在400，下在600
    CCLayerColor* pContinuebg = CCLayerColor::create(COLOR_CARD_BACKGROUND_V4, 600, 200);  
    pContinuebg->ignoreAnchorPointForPosition(false);
    pContinuebg->setAnchorPoint(ccp(0.5f, 0.5f));
    pContinuebg->setPosition(visibleSize.width/2, visibleSize.height - 500);
    bglayer->addChild(pContinuebg);
    CCLabelTTF * pContinueString = CCLabelTTF::create("分数", "Georgia-Bold", 50);
    pContinueString->setPosition(ccp(pContinuebg->getContentSize().width/2, pContinuebg->getContentSize().height * 3 / 4));
    pContinuebg->addChild(pContinueString);

    CCLabelTTF * pScoreString = CCLabelTTF::create(CCString::createWithFormat("%i", iThisScore)->getCString(), "Georgia-Bold", 100);
    pScoreString->setPosition(ccp(pContinuebg->getContentSize().width/2, pContinuebg->getContentSize().height / 3));
    pContinuebg->addChild(pScoreString);


    //重新开始，宽600，高200，上在700，下在900
    pRestartbg = CCLayerColor::create(COLOR_CARD_BACKGROUND_V4, 600, 200);  
    pRestartbg->ignoreAnchorPointForPosition(false);
    pRestartbg->setAnchorPoint(ccp(0.5f, 0.5f));
    pRestartbg->setPosition(visibleSize.width/2, visibleSize.height - 800);
    bglayer->addChild(pRestartbg);
    CCLabelTTF * pRestartString = CCLabelTTF::create("重新开始", "Georgia-Bold", 80);
    pRestartString->setPosition(ccp(pRestartbg->getContentSize().width/2, pRestartbg->getContentSize().height/2));
    pRestartbg->addChild(pRestartString);

    setTouchEnabled(true);  

    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, true);  

    
    return true;
}

bool GameOver::ccTouchBegan(CCTouch *touch, CCEvent * pEvent)
{
    CCPoint touchLocation = touch->getLocation(); // 返回GL坐标

    CCPoint nodePos = pRestartbg->convertToNodeSpace(touchLocation);
    CCSize nodesize = pRestartbg->getContentSize();
    if (nodePos.x>0 && nodePos.x<nodesize.width && nodePos.y>0 && nodePos.y<nodesize.height){
        CCLog("set ifRestart=true");
        ifRestart = true;
        CCScene* GameScene = Game::scene();
        CCDirector::sharedDirector()->replaceScene(GameScene);
    }

    // CCLog("ccTouchBegan, x=%f, y=%f, newX=%f, newY=%f", touchLocation.x, touchLocation.y, nodePos.x, nodePos.y);


    return true;
}





