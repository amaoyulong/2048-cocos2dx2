#ifndef __CardSprite_H_
#define __CardSprite_H_

#include "cocos2d.h"

USING_NS_CC;

class CardSprite : public cocos2d::CCSprite {
public:
    static CardSprite* create(int number, int width, int height);

    virtual bool init();

    //CREATE_FUNC(CardSprite); //MYNOTE: CREATE_FUNC:  define a create function for a specific type, such as Layer

    int getNumber();
    int setNumber(int num);
    int setMoveIndex(int index);
    int getMoveIndex();

private:
	int index;
	int moveIndex;
    int number;
    int initCard(int num, int width, int height);

    CCLabelTTF* pNumLabel;
    CCLayerColor * pCardbg;
};

#endif //__CardSprite_H_
