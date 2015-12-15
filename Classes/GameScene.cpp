#include "GameScene.h"
#include "CardSprite.h"
#include "Constants.h"
#include "GameOverScene.h"
#include "MenuScene.h"

USING_NS_CC;

bool ifRestart = false;
int iThisScore = 0;

CCScene* Game::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    Game *layer = Game::create();

    layer->setTag(0);

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool Game::init()
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

    //绘制棋盘，但是棋盘的位置在外面设置。
    drawBoard(visibleSize.width, visibleSize.height);
    boardlayer->ignoreAnchorPointForPosition(false);
    boardlayer->setPosition(CCPointMake(visibleSize.width / 2, visibleSize.height / 2 - iInterval - iCardLength));
    addChild(boardlayer, 1, 1);

    //计算上下还剩多少距离
    iTopLeft = (visibleSize.height - iBoardLength ) / 2  +  iInterval + iCardLength;
    iBottomLeft = (visibleSize.height - iBoardLength ) / 2  - iInterval - iCardLength;


    //增加菜单
    int iMenuHeight = iTopLeft - iInterval - iCardLength;
    int iMenuWidth = visibleSize.width - iFrameLeft - iFrameRight;

    int tmpX = iMenuHeight;
    int tmpY = (iMenuWidth - 3 * tmpX) / 2;

    if (tmpY <= 0){
        tmpY = iFrameLeft;
        tmpX = (iMenuWidth - 2 * tmpY) / 3;
    }

    CCNode* pMenu = CCNode::create(); 
    pMenu->setContentSize(ccp(iMenuWidth, iMenuHeight));
    int iMenuX = visibleSize.width / 2;
    int iMenuY = visibleSize.height / 2 - iInterval - iCardLength + iBoardLength / 2 + iCardLength + iMenuHeight / 2;
    pMenu->ignoreAnchorPointForPosition(false);
    pMenu->setAnchorPoint(ccp(0.5f, 0.5f));
    pMenu->setPosition(ccp(iMenuX, iMenuY));
    addChild(pMenu, 1, 1);

    //绘制菜单内的其他元素
    drawMenu(pMenu, tmpX, tmpY);

    //增加一个card
    randomCreateCard();
    randomCreateCard();

    return true;
}

void Game::restart(){
    for (int index=0; index<16; index++){
        mValues[index] = 0;
        CardSprite * card = (CardSprite *)boardlayer->getChildByTag(index);
        if (card){
            boardlayer->removeChild(card);
        }
    }

    useNum = 0;

    iNowScore = 0;
    pScroeNum->setString(CCString::createWithFormat("%i", iNowScore)->getCString());

    randomCreateCard();
    randomCreateCard();

}

void Game::onExit()
{
    CCLog("Game::onExit");
    setTouchEnabled(false);
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
    CCLayer::onExit();
}

void Game::onEnter(){

    CCLog("Game::onEnter：%d", ifRestart?1:0);
    CCLayer::onEnter();
    if (ifRestart){
        restart();
        ifRestart = false;
    }
    setTouchEnabled(true);  
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, true);  
}


bool Game::ccTouchBegan(CCTouch *touch, CCEvent * pEvent)
{
    pBegin = touch->getLocationInView();

    CCPoint touchLocation = touch->getLocation(); // 返回GL坐标
    CCPoint nodePos = pGameMenu->convertToNodeSpace(touchLocation);
    CCSize nodesize = pGameMenu->getContentSize();

    if (nodePos.x>0 && nodePos.x<nodesize.width && nodePos.y>0 && nodePos.y<nodesize.height){
        CCScene* MenuScene = Menu::scene();
        CCDirector::sharedDirector()->pushScene(MenuScene);
        //    toGameOverTestScene();

    }

    CCLog("ccTouchBegan, x=%f, y=%f, newX=%f, newY=%f", touchLocation.x, touchLocation.y, nodePos.x, nodePos.y);


    return true;
}

#define CHECK_MOVE 5

void Game::ccTouchEnded(CCTouch *touch, CCEvent * pEvent){
    pEnd = touch->getLocationInView();

    float X = pEnd.x - pBegin.x;
    float Y = pEnd.y - pBegin.y;

    CCLog("ccTouchEnd:%f, %f", X, Y);

    if (abs(X) >= abs(Y)){
        if (X > CHECK_MOVE){
            CCLog("doRight");
            doDirect(pBegin, DIRECT_RIGHT);
        }else if (-X > CHECK_MOVE){
            CCLog("doLeft");
            doDirect(pBegin, DIRECT_LEFT);
        }
    }else{
        if (Y > CHECK_MOVE){
            CCLog("doDown");
            doDirect(pBegin, DIRECT_DOWN);
        }else if (-Y > CHECK_MOVE){
            CCLog("doUp");
            doDirect(pBegin, DIRECT_UP);
        }
    }
}

void Game::doDirect(CCPoint &p, int direct){

    bool needAdd = false;
    int lastNum = -1;
    int needMove = 0;

    bool isNeed = false;


    for (int i=0; i<4; i++){
        needMove = 0;
        lastNum = -1;
        for (int j=0; j<4; j++){
            int l=0;
            int c=0;
            switch(direct){
            case DIRECT_DOWN:
                l = j;
                c = i;
                break;
            case DIRECT_UP:
                l = 3-j;
                c = i;
                break;
            case DIRECT_RIGHT:
                l = i;
                c = 3-j;
                break;
            case DIRECT_LEFT:
            default:
                l = i;
                c = j;
                break;
            }

            int index = l * 4 + c;
            CCLog("l=%d, c=%d, value=%d", l, c, mValues[index] );
            if (mValues[index] == 0) {  //没有东西，下一个移动的步数＋1
                needMove++;
                CCLog("index=%d, needMove=%d", index, needMove);
            }else {
                CardSprite * card = (CardSprite *)boardlayer->getChildByTag(index);
                if (mValues[index] == lastNum) {
                    needMove++;
                    needAdd = true;   //需要跟上一个合并，移动的步数＋1
                }
                
                if (needMove == 0) {    //到目前为止前面都没有出现过0，或是跟自己是一样的，不需要移动
                    lastNum = mValues[index];
                    continue;
                }

                //需要移动，计算需要移动到的新坐标，以及新的index。
                int newX, newY, newIndex;
                switch(direct){
                case DIRECT_DOWN: //down, Y移动－
                    newX = card->getPositionX();
                    newY = card->getPositionY() - needMove * (iInterval + iCardLength);
                    newIndex = (l - needMove) * 4 + c;
                    break;
                case DIRECT_UP:   //down, Y移动＋
                    newX = card->getPositionX();
                    newY = card->getPositionY() + needMove * (iInterval + iCardLength);
                    newIndex = (l + needMove) * 4 + c;
                    break;
                case DIRECT_RIGHT: //left，X移动＋
                    newX = card->getPositionX() + needMove * (iInterval + iCardLength);
                    newY = card->getPositionY();
                    newIndex = l * 4 + c+needMove;
                    break;
                case DIRECT_LEFT: //left，X移动－
                default:
                    newX = card->getPositionX() - needMove * (iInterval + iCardLength);
                    newY = card->getPositionY();
                    newIndex = l * 4 + c - needMove;
                    break;
                }
       
                CCLog("index=%d, needMove=%d, X=%d, Y=%d, interval=%d, card=%d", index, needMove, newX, newY, iInterval, iCardLength);
                CCLog("newX1=%f, newY1=%f", card->getPositionX() , card->getPositionY());
                CCActionInterval*  actionTo = CCMoveTo::create(0.1, ccp(newX, newY));
                
                card->runAction( actionTo);
                isNeed = true;
                
                //修改新的
                lastNum = mValues[index];
                mValues[newIndex] = mValues[index];
                mValues[index] = 0;

                if (needAdd){  //如果是合并的，之前的要干掉
                    CardSprite * oldCard = (CardSprite *)boardlayer->getChildByTag(newIndex); 
                    if (oldCard){
                        boardlayer->removeChild(oldCard);
                        oldCard->release();
                        CCActionInterval*  actionBy = CCScaleBy::create(0.1f, 1.2f, 1.2f);
                        card->runAction( CCSequence::create(actionBy, actionBy->reverse(), NULL));
                        card->setNumber(card->getNumber() * 2);
                        mValues[newIndex] = card->getNumber() ;
                        iNowScore += mValues[newIndex];
                        pScroeNum->setString(CCString::createWithFormat("%i", iNowScore)->getCString());
                        if (iHistoryScore < iNowScore){
                            iHistoryScore = iNowScore;
                            pHisScroeNum->setString(CCString::createWithFormat("%i", iHistoryScore)->getCString());
                            CCUserDefault::sharedUserDefault()->setIntegerForKey(HISTORY_SCORE, iHistoryScore);
                        }

                        lastNum = -1;
                        useNum--;
                    }
                    needAdd = false;
                }
                
                card->setTag(newIndex);
            }
        }
    }

    if (isNeed){
        randomCreateCard(direct);
        if (checkIfGameOver()){
            toGameOverTestScene();
        }
    }

    // for(int l=0; l<4;l++){ 
    //     for(int c=0; c<4; c++){

    //         int index = l * 4 + c;   
    //         CCLog("l=%d, c=%d, value=%d", l, c, mValues[index] );
    //     }
    // } 

}
    
int Game::drawMenu(CCNode * pNode, int eleX, int eleY){
    CCLayerColor * pGamebg = CCLayerColor::create(ccc4(234, 193, 44, 255), eleX, eleX);
    pGamebg->setPosition(0, (pNode->getContentSize().height - eleX) / 2);
    pNode->addChild(pGamebg);
    CCLabelTTF * pNum = CCLabelTTF::create("2048", "Georgia-Bold", 100);
    pNum->setPosition(ccp(pGamebg->getContentSize().width/2, pGamebg->getContentSize().height/2));
    pGamebg->addChild(pNum);

    CCLayerColor * pScorebg = CCLayerColor::create(ccc4(186, 172, 162, 255), eleX, eleX/2);  //高度是1/2
    pScorebg->setPosition(eleX+eleY, (pNode->getContentSize().height - eleX) / 2 + eleX/2);
    pNode->addChild(pScorebg);
    CCLabelTTF * pScroeString = CCLabelTTF::create("分数", "Georgia-Bold", 40);
    pScroeString->setPosition(ccp(pScorebg->getContentSize().width/2, pScorebg->getContentSize().height *3 /4));
    pScorebg->addChild(pScroeString);
    pScroeNum = CCLabelTTF::create("0", "Georgia-Bold", 60);
    pScroeNum->setPosition(ccp(pScorebg->getContentSize().width/2, pScorebg->getContentSize().height/4));
    pScorebg->addChild(pScroeNum);

    CCLayerColor * pHisScorebg = CCLayerColor::create(ccc4(186, 172, 162, 255), eleX, eleX/2);  //高度是1/2
    pHisScorebg->setPosition((eleX+eleY)*2, (pNode->getContentSize().height - eleX) / 2 + eleX/2);
    pNode->addChild(pHisScorebg);
    CCLabelTTF * pHisScroeString = CCLabelTTF::create("历史最高分数", "Georgia-Bold", 40);
    pHisScroeString->setPosition(ccp(pHisScorebg->getContentSize().width/2, pHisScorebg->getContentSize().height *3 /4));
    pHisScorebg->addChild(pHisScroeString);
    pHisScroeNum = CCLabelTTF::create("0", "Georgia-Bold", 60);
    pHisScroeNum->setPosition(ccp(pHisScorebg->getContentSize().width/2, pHisScorebg->getContentSize().height/4));
    pHisScorebg->addChild(pHisScroeNum);

    pGameMenu = CCLayerColor::create(ccc4(235, 151, 96, 255), eleX, eleX/3);  //高度是1/3
    pGameMenu->setPosition(eleX+eleY, pNode->getContentSize().height / 6);
    pNode->addChild(pGameMenu);
    CCLabelTTF * pMenuString = CCLabelTTF::create("菜单", "Georgia-Bold", 40);
    pMenuString->setPosition(ccp(pGameMenu->getContentSize().width/2, pGameMenu->getContentSize().height / 2));
    pGameMenu->addChild(pMenuString);

    CCLayerColor * pRank = CCLayerColor::create(ccc4(235, 151, 96, 255), eleX, eleX/3);  //高度是1/3
    pRank->setPosition((eleX+eleY)*2, pNode->getContentSize().height / 6);
    pNode->addChild(pRank);
    CCLabelTTF * pRankString = CCLabelTTF::create("排行榜", "Georgia-Bold", 40);
    pRankString->setPosition(ccp(pRank->getContentSize().width/2, pRank->getContentSize().height / 2));
    pRank->addChild(pRankString);


    //回复历史数据
    iHistoryScore = CCUserDefault::sharedUserDefault()->getIntegerForKey(HISTORY_SCORE);
    pHisScroeNum->setString(CCString::createWithFormat("%i", iHistoryScore)->getCString());
                            

}

int Game::drawBoard(float w, float h){

    int width = (int)w;

    iInterval = width / 24;
    iCardLength = iInterval * 4; 
    iBoardLength = iCardLength * 4 + iInterval * 5;

    int iFrame = width - iBoardLength;
    iFrameLeft = iFrame / 2;
    iFrameRight = iFrame - iFrameLeft;

    int x = width / 2; 

    //绘制
    boardlayer = CCLayerColor::create(ccc4(187, 173, 161, 255), iBoardLength, iBoardLength);

    //绘制card
    for(int i=0; i<4; i++){
        for(int j=0;j<4; j++){
            CCLayerColor* cardlayer = CCLayerColor::create(COLOR_CARD_BACKGROUND_V4, iCardLength, iCardLength);
            cardlayer->setPosition(CCPointMake( (j+1)*iInterval+j*iCardLength, (i+1)*iInterval+i*iCardLength));
            boardlayer->addChild(cardlayer);
            CCLog("newX3X=%f, newY3Y=%f", cardlayer->getPositionX() , cardlayer->getPositionY());
        }
    }

    return 0;

}

int Game::randomCreateCard(int direct, int index){

    //保证一次遍历就能遍历到
    if (index == -1){
        int left = 16-useNum;
        int random = std::min((int)(CCRANDOM_0_1()*left), 4);

        CCLog("random=%d", random);
        if (random < 0) random = 0;
        if (random >= 16) random = 15;

        for (int i=0; i<4; i++){
            for (int j=0; j<4; j++){
                int l=0;
                int c=0;
                switch(direct){
                case DIRECT_DOWN:
                    l = 3- i;
                    c = j;
                    break;
                case DIRECT_RIGHT:
                    l = j;
                    c = i;
                    break;
                case DIRECT_LEFT:
                    l = j;
                    c = 3-i;
                    break;
                case DIRECT_UP:
                default:
                    l = i;
                    c = j;
                    break;
                }

                CCLog("tmp index=%d， random=%d,  idx=%d, value=%d", index, random, l * 4 + c, mValues[l * 4 + c]);

                if (random <= 0){
                    if(mValues[l * 4 + c] == 0){
                        index = l * 4 + c;
                        i=5; // 跳出双重循环
                        break;
                    }
                }else{
                    random--;
                }

            }
        } 

        CCLog("index=%d", index);
    }    

    int line = index / 4;
    int colomn = index % 4;

    int y = (line + 1) * iInterval + line * iCardLength + iCardLength / 2 ;
    int x = (colomn + 1) * iInterval + colomn * iCardLength + iCardLength / 2;

    CardSprite * card = CardSprite::create((((index+useNum) % 3) ? 2 : 4), iCardLength, iCardLength);
    card->setTag(index);
    card->setPosition(ccp(x, y));
    card->setScale(0.3f);
    boardlayer->addChild(card);

    CCPoint pa = card->getAnchorPoint();
    CCLog("anchor:x=%f, y=%f", pa.x, pa.y);

    CCActionInterval*  actionTo = CCScaleTo::create(0.5f, 1.0f);
    card->runAction( actionTo);

    CCLog("newX2X=%f, newY2Y=%f", card->getPositionX() , card->getPositionY());

    mValues[index] = card->getNumber();

    useNum++;

    return index;
}


bool Game::checkIfGameOver(){
    //游戏结束的条件，mValue已经全部满了，也就是useNum为16，并且没有相邻的两个元素是一样的

    CCLog("useNum=%d", useNum);
    if (useNum < 16) return false;

    for(int i=0; i<4; i++){
        for (int j=0; j<3; j++){
            if (mValues[i*4+j] == mValues[i*4+j+1]){
                CCLog("index1=%d, index2=%d, v1=%d, v2=%d", i*4+j, i*4+j+1, mValues[i*4+j], mValues[i*4+j+1]);
                return false;
            } 
            if (mValues[j*4+i] == mValues[(j+1)*4+i]) {
                CCLog("index1=%d, index2=%d, v1=%d, v2=%d", j*4+i, (j+1)*4+i, mValues[j*4+i], mValues[(j+1)*4+i]);
                return false;
            }
        }
    }

    return true;
}


void Game::menuCloseCallback(CCObject* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT) || (CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
	CCMessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
#else
    CCDirector::sharedDirector()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
#endif
}


void Game::toGameOverTestScene(){
    iThisScore = iNowScore;
    CCScene* GameOverScene = GameOver::scene();
    CCDirector::sharedDirector()->replaceScene(GameOverScene);
}
