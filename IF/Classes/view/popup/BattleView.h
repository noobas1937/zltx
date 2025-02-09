//
//  BattleView.h
//  IF
//
//  Created by fubin on 14-3-11.
//
//

#ifndef IF_BattleView_h
#define IF_BattleView_h

#include "CommonInclude.h"
#include "CCBExtension.h"
#include "PopupBaseView.h"
#include "CCLabelIF.h"
#include "CCMultiColTableView.h"
#include "CheckBox.h"
#include "CCSliderBar.h"

class BattleView: public PopupBaseView
,public CCBSelectorResolver
,public CCBMemberVariableAssigner
, public CCMultiColTableViewDataSource
, public CCMultiColTableViewDelegate
{
public:
    static BattleView* create(unsigned int startIndex,unsigned int targetIndex,unsigned int haveOwner,float slow=1.0,int rally=0,int bType=-1,int wtIndex=-1,std::string other="", int targetType=0);
    BattleView():m_startIndex(0),m_targetIndex(0),m_haveOwner(0),m_rally(0),m_bType(-1),m_wtIndex(0),m_other(string()),m_targetType(0),m_slow(1.0){};
    virtual ~BattleView();
    CCNode* getGuideNode(string _key);
protected:
    // scrollview delegate
    virtual void scrollViewDidScroll(CCScrollView* view){};
    virtual void scrollViewDidZoom(CCScrollView* view){};
    
    virtual void gridTouched(cocos2d::extension::CCMultiColTableView* table, CCTableViewCell* cell);
	virtual cocos2d::CCSize gridSizeForTable(cocos2d::extension::CCMultiColTableView *table);
    virtual CCTableViewCell* gridAtIndex(cocos2d::extension::CCMultiColTableView *table, ssize_t idx);
    virtual ssize_t numberOfCellsInTableView(cocos2d::extension::CCMultiColTableView *table);
    virtual ssize_t numberOfGridsInCell(cocos2d::extension::CCMultiColTableView *multiTable);
    
private:
    virtual void onEnter();
    virtual void onExit();
    
    virtual bool init(unsigned int startIndex,unsigned int targetIndex,unsigned int haveOwner,float slow,int rally,int bType,int wtIndex,std::string other, int targetType);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);

    void onClickQuickBtn(CCObject * pSender, Control::EventType pCCControlEvent);
    void onAddClick(CCObject * pSender, Control::EventType pCCControlEvent);
    void onClickMarchBtn(CCObject * pSender, Control::EventType pCCControlEvent);
    void onHelpClick(CCObject * pSender, Control::EventType pCCControlEvent);
    void onAddStamineClick(CCObject * pSender, Control::EventType pCCControlEvent);
    void onFormation1Click(CCObject *pSender, CCControlEvent event);
    void onFormation2Click(CCObject *pSender, CCControlEvent event);
    void onFormation3Click(CCObject *pSender, CCControlEvent event);
    void onFormation4Click(CCObject *pSender, CCControlEvent event);
    void onFormation5Click(CCObject *pSender, CCControlEvent event);
    void clickFormation(int index);
    void updateInfo();
    void generateData();
    void loadResource(float _time);
    void updateLoadInfo(CCObject* obj);
    void setAddBtnState();
    void makeArrTime(CCObject* obj);
    void march();
    void notice();
    void generalSelect();
    void selectAll();
    void unselectAll();
    void callSuccess(CCObject* param);
    void checkCanMarch(CCObject* param);
    void updateArmyNumber(CCObject* obj);
    void loadResource();
    void selectInGuide();
    CCSafeObject<CCControlButton> m_marchBtn;
    CCSafeObject<CCControlButton> m_quickBtn;
    CCSafeObject<CCControlButton> m_helpBtn;
    CCSafeObject<CCLabelIF> m_hintText1;
    CCSafeObject<CCNode> m_infoList;
    CCSafeObject<CCNode> m_checkBoxContainer;
    CCSafeObject<CCLabelIF> m_timeLabel;
    CCSafeObject<CCLabelIF> m_timeText;
    CCSafeObject<CCLabelIF> m_msg1Label;
    CCSafeObject<CCLabelIF> m_msg2Label;
    CCSafeObject<CCScale9Sprite> m_msg2Dizi;
    
    CCSafeObject<CCLabelIF> m_msg3Label;
    CCSafeObject<CCLabelIF> m_hintText;
    CCSafeObject<CCControlButton> m_addBtn;
    CCSafeObject<Node> m_formationNode;
    CCSafeObject<Node> m_formation5;
    CCSafeObject<Node> m_formation4;
    CCSafeObject<Node> m_formation3;
    CCSafeObject<Node> m_formation2;
    CCSafeObject<Node> m_formation1;
    CCSafeObject<CCControlButton> m_formationBtn5;
    CCSafeObject<CCControlButton> m_formationBtn4;
    CCSafeObject<CCControlButton> m_formationBtn3;
    CCSafeObject<CCControlButton> m_formationBtn2;
    CCSafeObject<CCControlButton> m_formationBtn1;
    CCSafeObject<CCSprite> m_addIcon;
    CCSafeObject<CCSprite> m_addIcon1;
    CCSafeObject<CCSprite> m_pic_di;
    
    CCSafeObject<CCScale9Sprite> m_bg;
    CCSafeObject<CCScale9Sprite> m_renderBG;
//    CCSafeObject<CheckBox> m_box;
    CCSafeObject<CCNode> m_downNode;
    CCSafeObject<CCNode> m_alertNode;
    CCSafeObject<CCNode> m_iconContainer;
    CCSafeObject<CCNode> m_stamineNode;
    CCSafeObject<CCMultiColTableView > m_tabView;
    CCSafeObject<CCArray> m_tmpArray;
    unsigned int m_startIndex;
    unsigned int m_targetIndex;
    int m_rally;
    int m_bType;
    int m_wtIndex;
    std::string m_other;
    int m_targetType;
    unsigned int m_haveOwner;
    int m_index;
    float m_slow;
};

class SoldierCell :
public CCBSelectorResolver
,public CCBMemberVariableAssigner
,public CCTableViewCell
,public CCEditBoxDelegate

{
public:
    static SoldierCell* create(string itemId, int num, int type, int rally);
    SoldierCell():m_soldierId(""),m_num(0),m_cntNum(0),m_type(0),m_rally(0), m_invalidSliderMove(false){};
    void setData(string itemId, int num, int type, int rally);
    void refresh();
    void cellTouchEnded(CCTouch* pTouch);
private:
    virtual bool init(string itemId, int num, int type, int rally);
    virtual void onEnter();
    virtual void onExit();
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){return NULL;}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    
    void valueChange(CCObject * pSender, Control::EventType pCCControlEvent);
    void onSubClick(CCObject * pSender, Control::EventType pCCControlEvent);
    void onAddClick(CCObject * pSender, Control::EventType pCCControlEvent);
    void editBoxReturn(CCEditBox *editBox);

    CCSafeObject<CCLabelIF> m_nameLabel;
    CCSafeObject<CCControlButton> m_subBtn;
    CCSafeObject<CCControlButton> m_addBtn;
    CCSafeObject<CCNode> m_picNode;
    CCSafeObject<CCNode> m_levelNode;
    CCSafeObject<CCNode> m_sliderNode;
    CCSafeObject<CCSliderBar> m_slider;
    CCSafeObject<CCNode> m_editNode;
    CCSafeObject<CCNode> m_headTouchNode;
    CCEditBox* m_editBox;
    string m_soldierId;
    int m_num;
    int m_cntNum;
    int m_type;
    int m_rally;
    string m_iconPath;
    bool m_invalidSliderMove;
};

enum{
    MARCH_TYPE,
    DEFENCE_TYPE,
};

#endif
