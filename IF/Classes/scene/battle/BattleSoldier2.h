//
//  BattleSoldier2.h
//  aoe
//
//  Created by ganxiaohua on 13-8-27.
//
//

#ifndef __IF__BattleSoldier2__
#define __IF__BattleSoldier2__

#include "CommonInclude.h"
#include "CCLoadSprite.h"
#include "SpriteSheetAni.h"
#include "CCMathUtils.h"
#include "GongJian2.h"
#include "BattleManager.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;

enum SoldierStatus{
    S_STAND,
    S_MOVE,
    S_ATTACK,
    S_HURT,
    S_DEATH,
};

enum ATTACK_DIRECT{
    ATTACK_UP,
    ATTACK_DOWN,
    ATTACK_LEFT,
    ATTACK_RIGHT,
};

enum ActionStatus{
    ACTION_STAND,
    ACTION_MOVE,
    ACTION_ATTACK,
    ACTION_HURT,
    ACTION_DEATH,
};

class BattleSoldier2 :public CCNode
{
public:
	~BattleSoldier2(){
        //m_enemyList->removeAllObjects();
        //CCLOG("$test battle soldier$");

    }

    BattleSoldier2(CCNode* batchNode,CCNode* pNode,int armType,int side,std::string icon,std::string direct,bool isHead,int sqerType, int firstInSqer):m_side(side),m_completeTarget(NULL),m_status(ACTION_STAND)
    ,m_batchNode(batchNode),m_pNode(pNode),m_icon(icon),m_armType(armType)
    ,m_iconSpr(NULL)
    ,m_key(std::string())
    ,m_position(0)
    ,m_isHead(isHead)
    ,m_direct(direct)
    ,m_moveSpd(90)
    ,m_lastDirect(direct)
    ,m_sqerType(sqerType)
    ,m_firstInSqer(firstInSqer)
    ,m_aniPlaying(false)
    {};
    
    static BattleSoldier2* create(CCNode* batchNode,CCNode* pNode,int armType,int side,std::string icon,std::string direct,bool isHead=false,int sqerType = -1, int firstInSqer = -1);
    virtual void onEnter();
    virtual void onExit();
    
    void addObserver(int type);//1为校场士兵
    void removeObserver(int type);
    void playAnimation(ActionStatus status,float delayTime=0.0,CCCallFunc* completeFunc=NULL,int loopTimes=0);
    void delayPlayAnimation();
    bool init();
    void changeDirect(std::string direct,bool replay=false);
    void setSoldierPosition(CCPoint p);
    CCPoint getSoldierPosition();
    void playAttack(float t);
    void moveToPosition(vector<CCPoint> posV,float delayTime=0, float moveSpd=0);

    void removeFromBatchNode(float dt=-1);
    void moveOnePoint(float t);
    void moveOnePoint();

    std::string getDirection(CCPoint from,CCPoint to);
    void destory();
    void playDie();
    void setSprScale(float sc);
    void playAnimationByTouch(CCObject* obj);
    void postAnimationMsg();
    int m_side;
    CCPoint m_point;
    int m_status;
    int m_armType;
    int m_position;
    std::string m_icon;
    bool m_isHead;//
    std::string m_key;
    std::string m_direct;
    std::string m_lastDirect;
    
    vector<CCPoint> m_movePositions;
    int m_moveIndex;
    float m_moveSpd;
    int m_sqerType;
    int m_firstInSqer;
    bool m_aniPlaying;
private:
    CC_SAFE_CALLFUNC(m_completeTarget, m_completeFunc, CompleteFun);
    CC_SYNTHESIZE(int, m_loopTimes, LoopTimes);
    CC_SYNTHESIZE_SAFEOBJECT(CCNode , m_batchNode, BatchNode);
    CC_SYNTHESIZE_SAFEOBJECT(CCNode , m_container, Container);
    CC_SYNTHESIZE_SAFEOBJECT(CCSprite , m_iconSpr, IconSpr);
    CC_SYNTHESIZE_SAFEOBJECT(CCNode , m_pNode, PNode);
};
#endif /* defined(__IF__BattleSoldier2__) */
