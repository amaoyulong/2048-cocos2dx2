#include "CardSprite.h"
#include "Constants.h"


bool CardSprite::init(){
	if (!CCSprite::init()) {
        return false;
    }

    return true;
}

int CardSprite::getNumber(){
	return number;
}

int CardSprite::setNumber(int num){
	number = num;

	if (number > 0) {
        pNumLabel->setString(CCString::createWithFormat("%i", number)->getCString());
    } else {
        pNumLabel->setString("");
    }

    switch(number){
    	case 2:
    		pCardbg->setColor(COLOR_CARD_2_BG_V3);
    		break;
    	case 4:
    		pCardbg->setColor(COLOR_CARD_4_BG_V3);
    		break;
    	case 8:
    		pCardbg->setColor(COLOR_CARD_8_BG_V3);
    		pNumLabel->setColor(COLOR_CARD_NUM_WHITE_V3);
    		break;
    	case 16:
    		pCardbg->setColor(COLOR_CARD_16_BG_V3);
    		break;
    	case 32:
    		pCardbg->setColor(COLOR_CARD_32_BG_V3);
    		break;
    	case 64:
    		pCardbg->setColor(COLOR_CARD_64_BG_V3);
    		break;
    	case 128:
    		pCardbg->setColor(COLOR_CARD_128_BG_V3);
    		break;
    	case 256:
    		pCardbg->setColor(COLOR_CARD_256_BG_V3);
    		break;
    	case 512:
    		pCardbg->setColor(COLOR_CARD_512_BG_V3);
    		break;
    	case 1024:
    		pCardbg->setColor(COLOR_CARD_1024_BG_V3);
    		break;
    	case 2048:
    		pCardbg->setColor(COLOR_CARD_2048_BG_V3);
    		break;
    	case 4096:
    		pCardbg->setColor(COLOR_CARD_4096_BG_V3);
    		break;
    	default:
    		break;
    }

	return num;
}

int CardSprite::getMoveIndex(){
	return moveIndex;
}

int CardSprite::setMoveIndex(int index){
	moveIndex = index;
	return index;
}


CardSprite* CardSprite::create(int number, int width, int height){
	CardSprite* card = new CardSprite();
    if (card && card->init()) {
    	card->setContentSize(ccp(width, height));
        card->initCard(number, width, height);
        return card;
    }

    CC_SAFE_DELETE(card); // MYNOTE: CC_SAFE_DELETE: do { delete (p); (p) = nullptr; } while(0)
    return NULL;
}

int CardSprite::initCard(int num, int width, int height){

	number = num;
	if (num == 4){
		pCardbg = CCLayerColor::create(COLOR_CARD_4_BG_V4, width, height);
	}else{
		pCardbg = CCLayerColor::create(COLOR_CARD_2_BG_V4, width, height);
	}

    //判断如果大于0就显示，否则显示空
    if (number > 0)
    {
    	pNumLabel = CCLabelTTF::create(CCString::createWithFormat("%i", num)->getCString(), "Georgia-Bold", 100);
    	pNumLabel->setColor(COLOR_CARD_NUM_BLACK_V3);
    
	    // position the label on the center of the screen
	    pNumLabel->setPosition(ccp(pCardbg->getContentSize().width/2,
	                            pCardbg->getContentSize().height/2));
   
        pCardbg->addChild(pNumLabel);
    } 

    this->addChild(pCardbg);
	return num;
}

