//
//  StoreMallView.h
//  IF
//
//  Created by lifangkai on 14-11-11.
//
//

#ifndef __IF__StoreMallView__
#define __IF__StoreMallView__
#include "CommonInclude.h"
#include "PopupBaseView.h"
#include "DefaultTableViewDelegate.h"
#include "CCTableViewTouchIFCell.h"
#define  STORE_MALL_HIDE_DES "store.mall.hide.des"

class StoreMallView: public Layer
,public CCBSelectorResolver
,public CCBMemberVariableAssigner
,public CCTableViewDataSource
,public DefaultTableViewDelegate

{
public:
    static StoreMallView* create();
    static bool sortByUseOder(int itemId1, int itemId2);
    
protected:
    // scrollview delegate
    virtual CCSize tableCellSizeForIndex(CCTableView *table, ssize_t idx);
    virtual CCSize cellSizeForTable(CCTableView *table);
    virtual CCTableViewCell* tableCellAtIndex(CCTableView *table, ssize_t idx);
    virtual ssize_t numberOfCellsInTableView(CCTableView *table);
    virtual void tableCellWillRecycle(CCTableView* table, CCTableViewCell* cell);
    virtual void scrollViewDidScroll(CCScrollView* view);
    
private:
    virtual void onEnter();
    virtual void onExit();
    virtual bool init();
    
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    
    virtual bool onTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    void getHotItems();
    void refreshView(CCObject* obj);
    void onClickWarBtn(CCObject * pSender, Control::EventType pCCControlEvent);
    void onClickBuffBtn(CCObject * pSender, Control::EventType pCCControlEvent);
    void onClickResBtn(CCObject * pSender, Control::EventType pCCControlEvent);
    void onClickOthBtn(CCObject * pSender, Control::EventType pCCControlEvent);
    void onClickHotBtn(CCObject * pSender, Control::EventType pCCControlEvent);
    void sortDataByOrderNum();
    void refreshTime(CCObject* param);
    void onTimer(float dt);
    string getTimeStr(int secs);
    
    CCSafeObject<CCControlButton> m_warBtn;
    CCSafeObject<CCControlButton> m_buffBtn;
    CCSafeObject<CCControlButton> m_resBtn;
    CCSafeObject<CCControlButton> m_othBtn;
    CCSafeObject<CCControlButton> m_hotBtn;
    CCSafeObject<CCSprite> m_sprHot;
    
    CCSafeObject<CCNode> m_leftNode;
    CCSafeObject<CCNode> m_rightNode;
    CCSafeObject<CCNode> m_btnPartNode;
    CCSafeObject<CCNode> m_infoList;
    CCSafeObject<CCArray>m_data;
    CCSafeObject<CCNode> m_bgNode;
    
    CCSafeObject<CCNode> m_timeNode;
    
    CCSafeObject<CCLabelIF> m_timeTitleText;
    CCSafeObject<CCLabelIF> m_timeLeftText;
    
    CCSafeObject<CCLabelIF> m_hotTxt;
    CCSafeObject<CCLabelIF> m_buffTxt;
    CCSafeObject<CCLabelIF> m_warTxt;
    CCSafeObject<CCLabelIF> m_resTxt;
    CCSafeObject<CCLabelIF> m_othTxt;
    
    CCSafeObject<CCNode> m_height_hot;
    CCSafeObject<CCNode> m_height_res;
    CCSafeObject<CCNode> m_height_war;
    CCSafeObject<CCNode> m_height_good;
    CCSafeObject<CCNode> m_height_ot;
    
    CCSafeObject<CCNode> m_effect_hot;
    CCSafeObject<CCNode> m_effect_res;
    CCSafeObject<CCNode> m_effect_war;
    CCSafeObject<CCNode> m_effect_good;
    CCSafeObject<CCNode> m_effect_ot;
    double m_refreshTime;
    double m_leftTime;
    
    CCTableView* m_tabView;
    
    void generateData();
    int m_page;
    bool m_isInit;
    std::vector<int> m_curList;
    std::vector<int>* m_dataList;
    int m_cityLv;
    CCPoint m_startPoint1;
    CCPoint m_startPoint2;
    CCPoint m_startPoint3;
    CCPoint m_startPoint4;
    CCPoint m_startPoint5;
    bool m_onTimer;
};

//...cell class
class StoreMallCell :
public CCBSelectorResolver
 
,public CCBMemberVariableAssigner
,public CCTableViewTouchIFCell
{
public:
    static StoreMallCell* create(int itemId, int itemId1, CCNode* touchNode);
    StoreMallCell();
    void setData(int itemId, int itemId1);
private:
    bool init(int itemId, int itemId1, CCNode* touchNode);
    virtual void onEnter();
    virtual void onExit();
    
    bool onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	void onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    
    void refreashData(CCObject* obj);
    
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    
    void onClickBuyBtn1(CCObject * pSender, Control::EventType pCCControlEvent);
    void onClickBuyBtn2(CCObject * pSender, Control::EventType pCCControlEvent);
    void onClickBuyBtnByType(int type);
    
    void onYes();
    void onCloseConfirm();
    void onBuyTool1(CCObject *ccObj = NULL);
    void onBuyTool2(CCObject *ccObj = NULL);
    void onRetBuyTool();
    void setDesNode(int itemId,int index);
    void setLeft(float dt);
    void setRight(float dt);
    void setLeftHot(bool ishot,int num);
    void setRightHot(bool ishot,int num);
    void setLeftNewHot(float flag);
    void setRightNewHot(float flag);
    
    CCSafeObject<CCLabelIF> m_nameLabel1;
    CCSafeObject<CCLabelIF> m_nameLabel2;
    CCSafeObject<CCLabelIF> m_priceLabel1;
    CCSafeObject<CCLabelIF> m_priceLabel2;
    CCSafeObject<CCNode> m_picNode1;
    CCSafeObject<CCNode> m_picNode2;
    CCSafeObject<CCNode> m_itemNode1;
    CCSafeObject<CCNode> m_itemNode2;
    CCSafeObject<CCSprite> m_picBG1;
    CCSafeObject<CCSprite> m_picBG2;
    CCSafeObject<CCNode> m_desNode;
    CCSafeObject<CCLabelIF> m_desName;
    CCSafeObject<CCLabelIF> m_desLabel;
    CCSafeObject<CCScale9Sprite> m_buyBtnSpr1;
    CCSafeObject<CCScale9Sprite> m_buyBtnSpr2;
    CCSafeObject<CCScale9Sprite> m_buyBtngraySpr1;
    CCSafeObject<CCScale9Sprite> m_buyBtngraySpr2;
    CCSafeObject<CCScale9Sprite> m_saleoutSpr1;
    CCSafeObject<CCScale9Sprite> m_saleoutSpr2;
    CCSafeObject<CCSprite> m_goldSpr1;
    CCSafeObject<CCSprite> m_goldSpr2;

    CCSafeObject<CCNode> m_itemHotNode1;
    CCSafeObject<CCSprite> m_hotSpr1;
    CCSafeObject<CCSprite> m_hotgraySpr1;
    CCSafeObject<CCSprite> m_hotNumSpr1;
    CCSafeObject<CCSprite> m_hotNumgraySpr1;
    CCSafeObject<CCLabelIF> m_hotdesText1;
    CCSafeObject<CCLabelIF> m_hotNum1Text;
    CCSafeObject<CCScale9Sprite> m_hotLineSpr1;
    CCSafeObject<CCLabelIF> m_hotpriceLabel1;

    CCSafeObject<CCNode> m_itemHotNode2;
    CCSafeObject<CCSprite> m_hotSpr2;
    CCSafeObject<CCSprite> m_hotgraySpr2;
    CCSafeObject<CCSprite> m_hotNumSpr2;
    CCSafeObject<CCSprite> m_hotNumgraySpr2;
    CCSafeObject<CCLabelIF> m_hotdesText2;
    CCSafeObject<CCLabelIF> m_hotNum2Text;
    CCSafeObject<CCScale9Sprite> m_hotLineSpr2;
    CCSafeObject<CCLabelIF> m_hotpriceLabel2;
    
    CCSafeObject<CCNode> m_newHotNodeL;
    CCSafeObject<CCSprite> m_newHotSprPL;
    CCSafeObject<CCLabelIF> m_newHotTxtL;
    CCSafeObject<CCSprite> m_newHotSprRL;
    CCSafeObject<CCNode> m_newHotNodeR;
    CCSafeObject<CCSprite> m_newHotSprPR;
    CCSafeObject<CCLabelIF> m_newHotTxtR;
    CCSafeObject<CCSprite> m_newHotSprRR;
    
    CCSafeObject<CCNode> m_touchNode;
    int m_itemId;
    int m_itemId1;
    int m_price;
    int m_boostBid;
    bool m_clickInSide;
    bool m_isClick;
    CCPoint m_startPoint;
    int m_num;
};


#endif /* defined(__IF__StoreMallView__) */
