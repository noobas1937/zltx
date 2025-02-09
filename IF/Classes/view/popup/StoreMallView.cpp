//
//  StoreMallView.cpp
//  IF
//
//  Created by lifangkai on 14-11-11.
//
//

#include "StoreMallView.h"
#include "UIComponent.h"
#include "PopupViewController.h"
#include "DataRecordCommand.h"
#include "ParticleController.h"
#include "ChangeNickNameView.h"
#include "RollingText.h"
#include "ChangePicView.h"
#include "ShieldUnlockPopUpView.h"
#include "SettingPopUpView.h"
#include "ToolController.h"
#include "StoreBuyConfirmDialog.h"
#include "ChangePicCommand.h"
#include "WorldController.h"
#include "RoleInfoView.h"
#include "GoldExchangeMoreListView.h"
#include "PaymentCommand.h"
#include "PayController.h"
#include "WorldMapView.h"
#include "FunBuildController.h"
#include "BuildingAddOutPutCommand.h"
#include "StoreMallCellTip.h"
#include "SceneController.h"
#include "StoreView.h"
#include "YesNoDialog.h"

static int advertiseCellW = 570;

StoreMallView* StoreMallView::create(){
    StoreMallView* ret = new StoreMallView();
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}


bool StoreMallView::init()
{
    auto bg = CCBLoadFile("StoreMallView",this,this);
    setContentSize(bg->getContentSize());
    auto size = CCDirector::sharedDirector()->getWinSize();
    float dh = size.height-852-8;
    if (CCCommonUtils::isIosAndroidPad())
    {
        advertiseCellW = 1450;
        dh = size.height-2048;
    }
    m_infoList->setContentSize(CCSize(m_infoList->getContentSize().width, m_infoList->getContentSize().height+dh));
    m_data = CCArray::create();
    m_tabView = CCTableView::create(this, m_infoList->getContentSize());
    m_tabView->setDirection(kCCScrollViewDirectionVertical);
    m_tabView->setVerticalFillOrder(kCCTableViewFillTopDown);
    m_tabView->setTouchPriority(Touch_Default);
    m_tabView->setDelegate(this);
    m_infoList->addChild(m_tabView);
    m_cityLv = FunBuildController::getInstance()->getMainCityLv();
    

    m_startPoint1 = m_buffBtn->getTitleLabel()->getPosition();
    m_startPoint2 = m_warBtn->getTitleLabel()->getPosition();
    m_startPoint3 = m_resBtn->getTitleLabel()->getPosition();
    m_startPoint4 = m_othBtn->getTitleLabel()->getPosition();
    m_startPoint5 = m_hotBtn->getTitleLabel()->getPosition();
    m_btnPartNode->setPositionY(m_btnPartNode->getPositionY()+20);
    int count = (size.height-170)/44+1;
    for (int i=0; i<count; i++) {
        auto spr = CCLoadSprite::createSprite("Items_bg_big.png");
        this->m_bgNode->addChild(spr);
        spr->setPositionY(-i*44);
    }
    
    m_isInit = false;
    m_page = 5;
    refreshView(NULL);
    m_onTimer = false;
    
    m_timeNode->setVisible(false);
    m_timeTitleText->setString(_lang_1("115824",""));
    
    //add effect m_height_ot
    Sprite* spr_4 = CCLoadSprite::createSprite("shop_item4.png");
    m_effect_ot->addChild(spr_4);
    spr_4->setPosition(ccp(-12,7));
    auto particle_4 = ParticleController::createParticle("shop_item4");
    m_effect_ot->addChild(particle_4);
    
    //add effect m_height_hot
    Sprite* spr_5 = Sprite::create();
    CCCommonUtils::makeEffectSpr(spr_5, "shop_item5_%d.png", 8, 0.1);
    m_effect_hot->addChild(spr_5);
    spr_5->setPosition(ccp(-5,30));
    
    //add effect m_height_res
    Sprite* spr_1 = Sprite::create();
    CCCommonUtils::makeEffectSpr(spr_1, "shop_item1_%d.png", 8, 0.1);
    m_effect_res->addChild(spr_1);
    spr_1->setPosition(ccp(0,13));
    
    //add effect m_height_war
    Sprite* spr_2 = Sprite::create();
    CCCommonUtils::makeEffectSpr(spr_2, "shop_item2_%d.png", 8, 0.1);
    m_effect_war->addChild(spr_2);
    spr_2->setRotation(52);
    spr_2->setPosition(ccp(0,5));
    
    //add effect m_height_good
   auto particle_3 = ParticleController::createParticle("shop_item3");
    m_effect_good->addChild(particle_3);
    return true;
}

void StoreMallView::onEnter(){
    
    CCNode::onEnter();
    m_leftNode->removeAllChildren();
    m_rightNode->removeAllChildren();

    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);

    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, Touch_Default, false);//MSG_TOOL_HOT_GET_RETURN
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(StoreMallView::refreshView), MSG_TOOL_HOT_GET_RETURN, NULL);
    if (m_isInit) {
//        if (m_onTimer) {
//            this->unschedule(schedule_selector(StoreMallView::onTimer));
//        }
//        this->schedule(schedule_selector(StoreMallView::onTimer));
    }
    if(!m_isInit){
        m_isInit = true;
        getHotItems();
    }
}

void StoreMallView::onExit(){
    setTouchEnabled(false);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_TOOL_HOT_GET_RETURN);
    if(m_onTimer) {
        this->unschedule(schedule_selector(StoreMallView::onTimer));
        m_onTimer = false;
    }
    CCNode::onExit();
}

void StoreMallView::getHotItems(){
    return;
    ToolHotGetCommand* cmd = new ToolHotGetCommand();
    cmd->setCallback(CCCallFuncO::create(this, callfuncO_selector(StoreMallView::refreshTime), NULL));
    cmd->sendAndRelease();
    GameController::getInstance()->showWaitInterface();
}

void StoreMallView::refreshTime(CCObject* param){
    return;
    NetResult* result = dynamic_cast<NetResult*>(param);
    auto dict = _dict(result->getData());
    if (dict->objectForKey("refreshTime")) {
        m_refreshTime = dict->valueForKey("refreshTime")->doubleValue();
        double nowTime = WorldController::getInstance()->getTime();
        m_leftTime = (m_refreshTime - nowTime) / 1000;
        string txt = getTimeStr(m_leftTime);
        m_timeLeftText->setString(txt);
        if (!m_onTimer) {
            this->schedule(schedule_selector(StoreMallView::onTimer),1.0);
            m_onTimer = true;
        } else {
            this->unschedule(schedule_selector(StoreMallView::onTimer));
            this->schedule(schedule_selector(StoreMallView::onTimer),1.0);
        }
        
        if(m_page == 2) {
//            m_timeNode->setVisible(true);
//            m_timeLeftText->setVisible(true);
//            m_timeTitleText->setVisible(true);
        }
    }
}

void StoreMallView::onTimer(float dt) {
    double nowTime = WorldController::getInstance()->getTime();
    m_leftTime = (m_refreshTime - nowTime) / 1000;
    if (m_leftTime < 0) {
        if(m_onTimer){
            this->unschedule(schedule_selector(StoreMallView::onTimer));
            m_onTimer = false;
        }
        ToolHotGetCommand* cmd = new ToolHotGetCommand();
        cmd->setCallback(CCCallFuncO::create(this, callfuncO_selector(StoreMallView::refreshTime), NULL));
        cmd->sendAndRelease();
        GameController::getInstance()->showWaitInterface();
    } else {
        string txt = getTimeStr(m_leftTime);
        m_timeLeftText->setString(txt);
    }
}

string StoreMallView::getTimeStr(int secs){
    int day = secs/(24*3600);
    secs = secs%(24*3600);
    int hour = secs/3600;
    secs = secs%(3600);
    int min = secs/60;
    secs = secs%(60);
    int sec = secs;
    std::string dayStr = _lang("105592");
    std::string hourStr = _lang("105591");
    std::string minStr = _lang("105590");
    string timeStr = "";
    if(day > 0){
        timeStr = CC_ITOA(day)+_lang("103005")+" ";
    }
    if(hour<10){
        timeStr.append("0");
        timeStr.append(CC_ITOA(hour));
    }else{
        timeStr.append(CC_ITOA(hour));
    }
    timeStr.append(":");
    if(min<10){
        timeStr.append("0");
        timeStr.append(CC_ITOA(min));
    }else{
        timeStr.append(CC_ITOA(min));
    }
    timeStr.append(":");
    if(sec<10){
        timeStr.append("0");
        timeStr.append(CC_ITOA(sec));
    }else{
        timeStr.append(CC_ITOA(sec));
    }
    
    return timeStr;
}


void StoreMallView::generateData(){
    m_data->removeAllObjects();
    m_dataList = NULL;
    m_warBtn->setEnabled(true);
    m_buffBtn->setEnabled(true);
    m_resBtn->setEnabled(true);
    m_othBtn->setEnabled(true);
    m_hotBtn->setEnabled(true);
    CCCommonUtils::setButtonTitlePoint(m_buffBtn,m_startPoint1);
    CCCommonUtils::setButtonTitlePoint(m_warBtn,m_startPoint2);
    CCCommonUtils::setButtonTitlePoint(m_resBtn,m_startPoint3);
    CCCommonUtils::setButtonTitlePoint(m_othBtn,m_startPoint4);
    CCCommonUtils::setButtonTitlePoint(m_hotBtn,m_startPoint5);

    if (m_page == 1) {
//        m_dataList = &(ToolController::getInstance()->m_allTools);
        m_dataList = &ToolController::getInstance()->m_warList;
        m_warBtn->setEnabled(false);
        m_timeNode->setVisible(false);
      //  CCCommonUtils::setButtonTitlePoint(m_warBtn,ccp(m_startPoint2.x, m_startPoint2.y-5));
    }
    else if (m_page == 2) {
//        m_dataList = &(ToolController::getInstance()->m_allTools);
        m_dataList = &ToolController::getInstance()->m_buffList;
        m_buffBtn->setEnabled(false);
        m_timeNode->setVisible(false);
      //  CCCommonUtils::setButtonTitlePoint(m_buffBtn,ccp(m_startPoint1.x, m_startPoint1.y-5));
    }
    else if (m_page == 3) {
//        m_dataList = &(ToolController::getInstance()->m_allTools);
        m_dataList = &ToolController::getInstance()->m_resList;
        m_resBtn->setEnabled(false);
        m_timeNode->setVisible(false);
     //   CCCommonUtils::setButtonTitlePoint(m_resBtn,ccp(m_startPoint3.x, m_startPoint3.y-5));
    }
    else if (m_page == 4) {
//        m_dataList = &(ToolController::getInstance()->m_resTools);
        m_dataList = &ToolController::getInstance()->m_otherList;
        m_othBtn->setEnabled(false);
        m_timeNode->setVisible(false);
       // CCCommonUtils::setButtonTitlePoint(m_othBtn,ccp(m_startPoint4.x, m_startPoint4.y-5));
    }
    else if (m_page == 5) {
        m_dataList = &ToolController::getInstance()->m_hotList;
        m_hotBtn->setEnabled(false);
        m_timeNode->setVisible(false);
       // CCCommonUtils::setButtonTitlePoint(m_hotBtn,ccp(m_startPoint5.x, m_startPoint5.y-5));
    }
    
    m_curList.clear();
    int hotPlace=0;
    for (int i=0; i<m_dataList->size(); i++) {
        int tmpToolId = (*m_dataList)[i];
        ToolInfo& info = ToolController::getInstance()->getToolInfoById(tmpToolId);
        /*info.isShowHot = false;
        if(m_page == 2){
            if(info.ishot){
                info.isShowHot = true;
                m_curList.push_back(tmpToolId);
            }
        }else */if (info.price>0 && info.limitLv<=m_cityLv && info.pages>0){
//            if(m_page==1){
//                if(info.page==4||info.page==1){
//                    m_curList.push_back(tmpToolId);
//                }
//            }else
                    if (info.hotPara==99.0) {//||(info.hotPara>=0.1 && info.hotPara<=9.9)
                        m_curList.insert(m_curList.begin()+hotPlace, tmpToolId);
                        hotPlace++;
                    }else{
                        m_curList.push_back(tmpToolId);
                    }
        }
    }
//    if(m_page==1){
//        sortDataByOrderNum();
//    }
    string itemId = "";
    int count = m_curList.size();
    for (int i=0; i<count; i++) {
        int tmpToolId = (m_curList)[i];
        if(i%2==0){
            itemId = CC_ITOA(tmpToolId);
        }else{
            itemId.append(":");
            itemId += CC_ITOA(tmpToolId);
            m_data->addObject(CCString::create(itemId));
            itemId = "";
        }
    }
    if(count%2==1){
        itemId.append(":");
        itemId.append("-1");
        m_data->addObject(CCString::create(itemId));
    }
}
bool StoreMallView::sortByUseOder(int itemId1, int itemId2){
    auto& info1 = ToolController::getInstance()->getToolInfoById(itemId1);
    auto& info2 = ToolController::getInstance()->getToolInfoById(itemId2);
    return (info1.store_order<info2.store_order);
    
}
void StoreMallView::sortDataByOrderNum(){
    if (m_curList.size()>0) {
        std::sort(m_curList.begin(),m_curList.end(),sortByUseOder);
    }
}

void StoreMallView::refreshView(CCObject* obj){
    if(m_page == 5){
        m_sprHot->setVisible(false);
    }else{
        //m_sprHot->setVisible(true);
    }
    generateData();
    m_tabView->reloadData();
    m_height_ot->setVisible(false);
    m_height_hot->setVisible(false);
    m_height_res->setVisible(false);
    m_height_war->setVisible(false);
    m_height_good->setVisible(false);
    
    m_effect_ot->setVisible(false);
    m_effect_hot->setVisible(false);
    m_effect_res->setVisible(false);
    m_effect_war->setVisible(false);
    m_effect_good->setVisible(false);
    if(m_page == 3){
        m_height_res->setVisible(true);
        m_effect_res->setVisible(true);
    }else if(m_page == 1){
        m_height_war->setVisible(true);
        m_effect_war->setVisible(true);
    }else if(m_page == 2){
        m_height_good->setVisible(true);
        m_effect_good->setVisible(true);
    }else if(m_page == 4){
        m_height_ot->setVisible(true);
        m_effect_ot->setVisible(true);
    }else if(m_page == 5){
        m_height_hot->setVisible(true);
        m_effect_hot->setVisible(true);
    }
    
}

SEL_CCControlHandler StoreMallView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName)
{
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickWarBtn", StoreMallView::onClickWarBtn);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickBuffBtn", StoreMallView::onClickBuffBtn);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickResBtn", StoreMallView::onClickResBtn);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickOthBtn", StoreMallView::onClickOthBtn);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickHotBtn", StoreMallView::onClickHotBtn);
    return NULL;
}

bool StoreMallView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_sprHot", CCSprite*, this->m_sprHot);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_warBtn", CCControlButton*, this->m_warBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_buffBtn", CCControlButton*, this->m_buffBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_resBtn", CCControlButton*, this->m_resBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_othBtn", CCControlButton*, this->m_othBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_hotBtn", CCControlButton*, this->m_hotBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoList", CCNode*, this->m_infoList);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_leftNode", CCNode*, this->m_leftNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_rightNode", CCNode*, this->m_rightNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btnPartNode", CCNode*, this->m_btnPartNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bgNode", CCNode*, this->m_bgNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_timeNode", CCNode*, this->m_timeNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_timeTitleText", CCLabelIF*, this->m_timeTitleText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_timeLeftText", CCLabelIF*, this->m_timeLeftText);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_hotTxt", CCLabelIF*, this->m_hotTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_buffTxt", CCLabelIF*, this->m_buffTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_warTxt", CCLabelIF*, this->m_warTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_resTxt", CCLabelIF*, this->m_resTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_othTxt", CCLabelIF*, this->m_othTxt);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_height_hot", CCNode*, this->m_height_hot);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_height_res", CCNode*, this->m_height_res);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_height_war", CCNode*, this->m_height_war);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_height_good", CCNode*, this->m_height_good);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_height_ot", CCNode*, this->m_height_ot);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_effect_hot", CCNode*, this->m_effect_hot);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_effect_res", CCNode*, this->m_effect_res);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_effect_war", CCNode*, this->m_effect_war);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_effect_good", CCNode*, this->m_effect_good);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_effect_ot", CCNode*, this->m_effect_ot);
    
    
    return false;
}


void StoreMallView::onClickWarBtn(CCObject * pSender, Control::EventType pCCControlEvent)
{
    if (m_page != 1) {
        m_page = 1;
        CCPoint pos = m_warBtn->getPosition();
        m_btnPartNode->setPosition(ccp(pos.x,pos.y+20));
 
        m_btnPartNode->removeAllChildren();
 
        refreshView(NULL);
    }
}

void StoreMallView::onClickBuffBtn(CCObject * pSender, Control::EventType pCCControlEvent)
{
    if (m_page != 2) {
        m_page = 2;
        CCPoint pos = m_buffBtn->getPosition();
        m_btnPartNode->setPosition(ccp(pos.x,pos.y+20));
 
        m_btnPartNode->removeAllChildren();
 
        refreshView(NULL);
    }
}

void StoreMallView::onClickResBtn(CCObject * pSender, Control::EventType pCCControlEvent)
{
    if (m_page != 3) {
        m_page = 3;
        CCPoint pos = m_resBtn->getPosition();
        m_btnPartNode->setPosition(ccp(pos.x,pos.y+20));
 
        m_btnPartNode->removeAllChildren();
 
        refreshView(NULL);
    }
}
void StoreMallView::onClickOthBtn(CCObject * pSender, Control::EventType pCCControlEvent)
{
    if (m_page != 4) {
        CCPoint pos = m_othBtn->getPosition();
        m_btnPartNode->setPosition(ccp(pos.x,pos.y+20));
 
        m_btnPartNode->removeAllChildren();
 
        m_page = 4;
        refreshView(NULL);
    }
}
void StoreMallView::onClickHotBtn(cocos2d::CCObject *pSender, Control::EventType pCCControlEvent){
    if(m_page != 5){
        CCPoint pos = m_hotBtn->getPosition();
        m_btnPartNode->setPosition(ccp(pos.x,pos.y+20));
        m_btnPartNode->removeAllChildren();
 
        m_page = 5;
        refreshView(NULL);
    }
}
bool StoreMallView::onTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
    if(!isTouchInside(m_infoList, pTouch)){
        return true;
    }
    return false;
}

void StoreMallView::onTouchMoved(CCTouch *pTouch, CCEvent *pEvent){
    
}

void StoreMallView::onTouchEnded(CCTouch *pTouch, CCEvent *pEvent){
    if(!isTouchInside(m_infoList, pTouch)){
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(STORE_MALL_HIDE_DES);
    }
    
}

CCSize StoreMallView::tableCellSizeForIndex(CCTableView *table, ssize_t idx)
{
    if(idx >= m_data->count()){
        return CCSizeZero;
    }
    if (CCCommonUtils::isIosAndroidPad())
    {
        return CCSize(advertiseCellW, 300);
    }
    return CCSize(advertiseCellW, 163);
}

CCSize StoreMallView::cellSizeForTable(CCTableView *table)
{
    if (CCCommonUtils::isIosAndroidPad())
    {
        return CCSize(advertiseCellW, 300);
    }
    return CCSize(advertiseCellW, 163);
}

CCTableViewCell* StoreMallView::tableCellAtIndex(CCTableView *table, ssize_t idx)
{
    if(idx >= m_data->count()){
        return NULL;
    }
    
    string itemId = dynamic_cast<CCString*>(m_data->objectAtIndex(idx))->getCString();
    vector<string> vector1;
    CCCommonUtils::splitString(itemId, ":", vector1);
    if(vector1.size()==2){
        string item1=vector1[0];
        string item2=vector1[1];
        StoreMallCell* cell = (StoreMallCell*)table->dequeueCell();
        if(cell){
            cell->setData(atoi(item1.c_str()),atoi(item2.c_str()));
        }else{
            cell = StoreMallCell::create(atoi(item1.c_str()),atoi(item2.c_str()),m_infoList);
        }
        return cell;
    }
    return NULL;
}

ssize_t StoreMallView::numberOfCellsInTableView(CCTableView *table)
{
    int num = ceil(m_data->count() / 1.0f);
    return num;
}

void StoreMallView::tableCellWillRecycle(CCTableView* table, CCTableViewCell* cell)
{
    
}

void StoreMallView::scrollViewDidScroll(CCScrollView* view){
    //    CCLOG("%f", view->getContentOffset().x);
    float mindy = m_tabView->minContainerOffset().y-20;
    float dy = m_tabView->getContentOffset().y;
    if(dy<mindy){
        m_tabView->setContentOffset(ccp(0, mindy));
    }
}

//class cell
StoreMallCell::StoreMallCell():m_itemId(0),m_itemId1(0),m_price(0),m_boostBid(0),m_touchNode(NULL),m_isClick(false){
}
StoreMallCell* StoreMallCell::create(int itemId, int itemId1, CCNode* touchNode)
{
    auto ret = new StoreMallCell();
    if (ret && ret->init(itemId, itemId1, touchNode)) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool StoreMallCell::init(int itemId, int itemId1, CCNode* touchNode)
{
    bool ret = true;
    CCBLoadFile("StoreMallCell",this,this);
    this->setContentSize(CCSize(570, 163));
    if(CCCommonUtils::isIosAndroidPad())
    {
        this->setContentSize(CCSize(1450, 300));
    }
    m_touchNode = touchNode;
    setData(itemId, itemId1);
    return ret;
}

void StoreMallCell::setData(int itemId, int itemId1)
{
    bool isPad = CCCommonUtils::isIosAndroidPad();
    m_itemId = itemId;
    m_itemId1 = itemId1;
    m_desNode->setVisible(false);
    m_priceLabel1->setColor({255,255,0});
    m_priceLabel2->setColor({255,255,0});
    m_goldSpr1->setColor({255,255,255});
    m_goldSpr2->setColor({255,255,255});
    m_buyBtngraySpr2->setVisible(false);
    m_buyBtngraySpr1->setVisible(false);
    ToolInfo& info = ToolController::getInstance()->getToolInfoById(m_itemId);
//    m_nameLabel1->setString(_lang(info.name).c_str());
    m_priceLabel1->setString(CC_CMDITOA(info.price));
//    m_picNode1->removeAllChildren();
    if(isPad){
        CCCommonUtils::createGoodsIcon(m_itemId, m_picNode1, CCSize(116*1.8, 116*1.8),NULL,m_nameLabel1);
    }else{
        CCCommonUtils::createGoodsIcon(m_itemId, m_picNode1, CCSize(116, 116),NULL,m_nameLabel1);
    }
    float hotPara = info.hotPara;
    setLeftNewHot(hotPara);
    
    if(itemId1!=-1){
        this->m_itemNode2->setVisible(true);
        ToolInfo& info1 = ToolController::getInstance()->getToolInfoById(m_itemId1);
        m_nameLabel2->setString(info1.getName().c_str());
        m_priceLabel2->setString(CC_CMDITOA(info1.price));
        
        if(isPad){
            CCCommonUtils::createGoodsIcon(m_itemId1, m_picNode2, CCSize(116*1.8, 116*1.8),NULL,m_nameLabel2);
        }else{
            CCCommonUtils::createGoodsIcon(m_itemId1, m_picNode2, CCSize(116, 116),NULL,m_nameLabel2);
        }
        float hotPara1 = info1.hotPara;
        setRightNewHot(hotPara1);
    }else{
        m_picNode2->removeAllChildren();
        this->m_itemNode2->setVisible(false);
    }
}

void StoreMallCell::onEnter() {
    CCNode::onEnter();

    setTouchEnabled(true);

    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, Touch_Default, false);//
    //    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(StoreMallCell::refreashData), STORE_MALL_HIDE_DES, NULL);
    ToolController::getInstance()->m_mallCurrItemId=0;
}

void StoreMallCell::onExit() {
    setTouchEnabled(false);
    //    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, STORE_MALL_HIDE_DES);
    CCNode::onExit();
}

void StoreMallCell::refreashData(CCObject* obj)
{
    ToolController::getInstance()->m_mallCurrItemId =0;
    this->m_desNode->setVisible(false);
}

SEL_CCControlHandler StoreMallCell::onResolveCCBCCControlSelector(cocos2d::CCObject *pTarget, const char *pSelectorName) {
    //    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickBuyBtn2", StoreMallCell::onClickBuyBtn2);
    //    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickBuyBtn1", StoreMallCell::onClickBuyBtn1);
    return NULL;
}


bool StoreMallCell::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    m_clickInSide = false;
    m_num = 0;
    if (isTouchInside(m_touchNode,pTouch)) {
        m_clickInSide = true;
        if (isTouchInside(this,pTouch)) {
            m_startPoint=pTouch->getStartLocation();
            if(isTouchInside(m_picBG1,pTouch)){
                this->schedule(schedule_selector(StoreMallCell::setLeft),0.2);
            }else if(isTouchInside(m_picBG2,pTouch)){
                if(m_itemId1==-1){
                    return false;
                }
                this->scheduleOnce(schedule_selector(StoreMallCell::setRight),0.2);
                
            }else if(isTouchInside(m_buyBtnSpr1,pTouch)){
                ToolInfo& info = ToolController::getInstance()->getToolInfoById(m_itemId);
                if(info.ishot&&info.isShowHot&&info.hotnum<=0){
                    return false;
                }else{
                    m_buyBtnSpr1->setScale(1.2);
                    
                }
                
            }else if(isTouchInside(m_buyBtnSpr2,pTouch)){
                if(m_itemId1==-1){
                    return false;
                }
                ToolInfo& info = ToolController::getInstance()->getToolInfoById(m_itemId1);
                if(info.ishot&&info.isShowHot&&info.hotnum<=0){
                    return false;
                }else{
                    m_buyBtnSpr2->setScale(1.2);
                }
                
            }
            return true;
        }
    }
    
    return false;
}
void StoreMallCell::setLeft(float dt){
    this->unschedule(schedule_selector(StoreMallCell::setLeft));
    setDesNode(m_itemId,0);
}
void StoreMallCell::setRight(float dt){
    this->unschedule(schedule_selector(StoreMallCell::setRight));
    setDesNode(m_itemId1,1);
}
void StoreMallCell::onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    m_buyBtnSpr1->setScale(1.0);
    m_buyBtnSpr2->setScale(1.0);
    m_desNode->setVisible(false);
    this->unschedule(schedule_selector(StoreMallCell::setLeft));
    this->unschedule(schedule_selector(StoreMallCell::setRight));
    CCPoint endPoint = pTouch->getLocation();
    if(fabs(endPoint.x-m_startPoint.x)>10||fabs(endPoint.y-m_startPoint.y)>10){
        return;
    }
    //    if(ToolController::getInstance()->m_mallCurrItemId ==99){
    //        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(STORE_MALL_HIDE_DES);
    //        return;
    //    }
    if(isTouchInside(m_buyBtnSpr1,pTouch)){
        onClickBuyBtn1(NULL,Control::EventType::TOUCH_DOWN);
    }else if(isTouchInside(m_buyBtnSpr2,pTouch)){
        onClickBuyBtn2(NULL,Control::EventType::TOUCH_DOWN);
    }
    auto layer = SceneController::getInstance()->getCurrentLayerByLevel(LEVEL_TIP);
    layer->removeAllChildren();
}


void StoreMallCell::setDesNode(int itemId,int index){
    m_desNode->setVisible(true);
    ToolController::getInstance()->m_mallCurrItemId = 99;
    if(index==0){//left
        m_desNode->setPositionX(290);
    }else{
        m_desNode->setPositionX(172);
    }
    if(CCCommonUtils::isIosAndroidPad())
    {
       float _p = m_desNode->getPositionX();
        m_desNode->setPositionX(_p*2);
        if (index!=0)
            m_desNode->setPositionX(_p*4-40);
    }
    ToolInfo& info = ToolController::getInstance()->getToolInfoById(itemId);
    if(info.type==5 && !info.getPara().empty()){
        m_desNode->setVisible(false);
        auto layer = SceneController::getInstance()->getCurrentLayerByLevel(LEVEL_TIP);
        if(layer->getChildrenCount()<=0){
            StoreMallCellTip* tip = StoreMallCellTip::create(info.getPara(),info.getName());
            layer->addChild(tip);
            float x = m_startPoint.x;
            if(index==1){
                x =100;
            }
            tip->setPosition(x, m_startPoint.y);
        }
    }else{
        m_desName->setString(info.getName().c_str());
        m_desLabel->setString(_lang(info.des).c_str());
    }
    CCSize size = m_desLabel->getContentSize();
    size.width *= m_desLabel->getOriginScaleX();
    size.height *= m_desLabel->getOriginScaleY();
    if(size.height>80){
        CCScale9Sprite* bgScale9 = dynamic_cast<CCScale9Sprite*>(m_desNode->getChildByTag(10));
        if(bgScale9){
            CCSize psize = bgScale9->getContentSize();
            psize.height +=  size.height - 80;
            bgScale9->setPreferredSize(psize);
            bgScale9->setPositionY(bgScale9->getPositionY() - (size.height - 80)*0.5);
            m_desNode->setPositionY(m_desNode->getPositionY() +  (size.height - 80)*0.5);
        }
    }else{
        CCScale9Sprite* bgScale9 = dynamic_cast<CCScale9Sprite*>(m_desNode->getChildByTag(10));
        if(bgScale9){
            CCSize psize = bgScale9->getContentSize();
            bgScale9->setPreferredSize(CCSize(psize.width, 149));
            bgScale9->setPositionY(bgScale9->getPositionY() + (psize.height - 149)*0.5);
            m_desNode->setPositionY(m_desNode->getPositionY()- (psize.height - 149)*0.5);
        }
    }
}
void StoreMallCell::onClickBuyBtn1(CCObject * pSender, Control::EventType pCCControlEvent)
{
    onClickBuyBtnByType(1);
//    if(!m_clickInSide){
//        return;
//    }
//    if(m_isClick)
//        return;
//    m_isClick = true;
//    ToolInfo& info = ToolController::getInstance()->getToolInfoById(m_itemId);
//    int price = 0;
//    if(info.ishot&&info.isShowHot)
//        price = info.hotprice;
//    else
//        price = info.price;
//    //    if (price > GlobalData::shared()->playerInfo.gold) {
//    //        YesNoDialog::gotoPayTips();
//    //        return;
//    //    }
//    int cCount = 0;
//    if(info.type==3 && !info.getPara().empty()){
//        cCount = atoi(info.getPara().c_str());
//    }
//    StoreView *view = dynamic_cast<StoreView*>(PopupViewController::getInstance()->getCurrentPopupView());
//    StoreBuyConfirmDialog* dialog = StoreBuyConfirmDialog::show(CCCommonUtils::getIcon(CC_ITOA(info.itemId)).c_str(), info.getName().c_str(), _lang(info.des).c_str(), price,info.color, CCCallFuncO::create(this, callfuncO_selector(StoreMallCell::onBuyTool1), NULL),m_picBG1->convertToWorldSpace(CCPointZero),Gold,cCount);
//    if(view && dialog){
//        int maxNum = floor(GlobalData::shared()->playerInfo.gold/price);
//        maxNum = MAX(1, maxNum);
//        dialog->showSliderBar(maxNum);
//        dialog->setEffNodeRect(view->getBagTabRect());
//        dialog->setCloseCallback(CCCallFunc::create(this, callfunc_selector(StoreMallCell::onCloseConfirm)));
//    }
//    //YesNoDialog::showButtonAndGold(_lang("104919").c_str(), CCCallFunc::create(this, callfunc_selector(StoreMallCell::onBuyTool1)), _lang("102148").c_str(), price);
}
void StoreMallCell::onClickBuyBtn2(CCObject * pSender, Control::EventType pCCControlEvent)
{
    onClickBuyBtnByType(2);
//    if(!m_clickInSide){
//        return;
//    }
//    if(m_isClick)
//        return;
//    m_isClick = true;
//    ToolInfo& info = ToolController::getInstance()->getToolInfoById(m_itemId1);
//    int price = 0;
//    if(info.ishot&&info.isShowHot)
//        price = info.hotprice;
//    else
//        price = info.price;
//    //    if (price > GlobalData::shared()->playerInfo.gold) {
//    //        YesNoDialog::gotoPayTips();
//    //        return;
//    //    }
//    int cCount = 0;
//    if(info.type==3 && !info.getPara().empty()){
//        cCount = atoi(info.getPara().c_str());
//    }
//    StoreView *view = dynamic_cast<StoreView*>(PopupViewController::getInstance()->getCurrentPopupView());
//    StoreBuyConfirmDialog* dialog = StoreBuyConfirmDialog::show(CCCommonUtils::getIcon(CC_ITOA(info.itemId)).c_str(), info.getName().c_str(), _lang(info.des).c_str(), price,info.color, CCCallFuncO::create(this, callfuncO_selector(StoreMallCell::onBuyTool2), NULL),m_picBG2->convertToWorldSpace(CCPointZero),Gold,cCount);
//    if(view && dialog){
//        int maxNum = floor(GlobalData::shared()->playerInfo.gold/price);
//        maxNum = MAX(1, maxNum);
//        dialog->showSliderBar(maxNum);
//        dialog->setEffNodeRect(view->getBagTabRect());
//        dialog->setCloseCallback(CCCallFunc::create(this, callfunc_selector(StoreMallCell::onCloseConfirm)));
//    }
//    //YesNoDialog::showButtonAndGold(_lang("104919").c_str(), CCCallFunc::create(this, callfunc_selector(StoreMallCell::onBuyTool2)), _lang("102148").c_str(), price);
}

void StoreMallCell::onClickBuyBtnByType(int type){
    int item_id = 0;
    if (type==1) { item_id = m_itemId;}
    else if (type==2){ item_id = m_itemId1;}
    else{ return;}
    if(!m_clickInSide){ return;}
    if(m_isClick){ return;}
    
    m_isClick = true;
    ToolInfo& info = ToolController::getInstance()->getToolInfoById(item_id);
    
    if (VipUtil::isSvipPointId(info.itemId)) {//svip点数
        if(GlobalData::shared()->playerInfo.SVIPLevel<1){
            YesNoDialog::show(_lang("103107").c_str(), NULL);//SVIPLevel<1 不能用spoint
            m_isClick = false;
            return;
        }
        if(GlobalData::shared()->playerInfo.SVIPPoint>= VipUtil::getMaxSpoint()){
            YesNoDialog::show(_lang("103105").c_str(), NULL);//103105=您的SVIP已经达到最高级，无需继续购买！
            m_isClick = false;
            return;
        }
    }
    if (VipUtil::isSvipTimeId(info.itemId) && GlobalData::shared()->playerInfo.SVIPLevel<1) {//svip时间
        YesNoDialog::show(_lang("103106").c_str(), NULL);//103106=您现在还不是SVIP，不能使用SVIP权限激活卡哦。
        m_isClick = false;
        return;
    }
    
    int price = 0;
    if(info.ishot&&info.isShowHot)
        price = info.hotprice;
    else
        price = info.price;
    int cCount = 0;
    if(info.type==3 && !info.getPara().empty()){
        cCount = atoi(info.getPara().c_str());
    }
    StoreView *view = dynamic_cast<StoreView*>(PopupViewController::getInstance()->getCurrentPopupView());
    
    StoreBuyConfirmDialog* dialog = nullptr;
    if (type==1) {
        dialog = StoreBuyConfirmDialog::show(CCCommonUtils::getIcon(CC_ITOA(info.itemId)).c_str(), info.getName().c_str(), _lang(info.des).c_str(), price,info.color, CCCallFuncO::create(this, callfuncO_selector(StoreMallCell::onBuyTool1), NULL),m_picBG1->convertToWorldSpace(CCPointZero),Gold,cCount);
    }
    else if (type==2){
        dialog = StoreBuyConfirmDialog::show(CCCommonUtils::getIcon(CC_ITOA(info.itemId)).c_str(), info.getName().c_str(), _lang(info.des).c_str(), price,info.color, CCCallFuncO::create(this, callfuncO_selector(StoreMallCell::onBuyTool2), NULL),m_picBG2->convertToWorldSpace(CCPointZero),Gold,cCount);
    }
    else{ return;}
    
    if(view && dialog){
        int maxNum = floor(GlobalData::shared()->playerInfo.gold/price);
        maxNum = MAX(1, maxNum);
        if(VipUtil::isVipPointId(info.itemId)){//vip点数
            int addPoints = atoi(info.getPara().c_str());
            int point = VipUtil::getMaxPoint() - GlobalData::shared()->playerInfo.vipPoints;
            if (point>0) {
                int maxBuy = point/addPoints;
                if (point%addPoints>0) {
                    maxBuy++;
                }
                maxNum = MIN(maxBuy, maxNum);
            }else{
                maxNum = 1;
            }
        }
        
        if (VipUtil::isSvipPointId(info.itemId)) {//svip点数
            int addPoints = atoi(info.getPara().c_str());
            int point = VipUtil::getMaxSpoint()- GlobalData::shared()->playerInfo.SVIPPoint;
            if (point>0) {
                int maxBuy = point/addPoints;
                if (point%addPoints>0) {
                    maxBuy++;
                }
                maxNum = MIN(maxBuy, maxNum);
            }else{
                maxNum = 1;
            }
        }

        
        if((info.itemId>=200700 && info.itemId<=200707) || (info.itemId>=200710 && info.itemId<=200719)){//龙币 铜币购买最大值
            int para1 = atoi(CCCommonUtils::getPropById(CC_ITOA(info.itemId), "para1").c_str());
            if(para1>0){
                maxNum = MIN(para1, maxNum);
            }
        }
        dialog->showSliderBar(maxNum);
        dialog->setEffNodeRect(view->getBagTabRect());
        dialog->setCloseCallback(CCCallFunc::create(this, callfunc_selector(StoreMallCell::onCloseConfirm)));
    }
}

void StoreMallCell::onBuyTool1(CCObject *ccObj)
{
    int buyCount = 1;
    if(ccObj){
        CCInteger* cInt = dynamic_cast<CCInteger*>(ccObj);
        if(cInt){
            buyCount = cInt->getValue();
        }
    }
    ToolInfo& info = ToolController::getInstance()->getToolInfoById(m_itemId);
    if(info.ishot&&info.isShowHot)
        ToolController::getInstance()->buyHotTool(m_itemId, buyCount, CCCallFunc::create(this, callfunc_selector(StoreMallCell::onRetBuyTool)),0,"StoreMall");
    else
        ToolController::getInstance()->buyTool(m_itemId, buyCount, CCCallFunc::create(this, callfunc_selector(StoreMallCell::onRetBuyTool)),0,false,true,"StoreMall");
}
void StoreMallCell::onBuyTool2(CCObject *ccObj)
{
    int buyCount = 1;
    if(ccObj){
        CCInteger* cInt = dynamic_cast<CCInteger*>(ccObj);
        if(cInt){
            buyCount = cInt->getValue();
        }
    }
    ToolInfo& info1 = ToolController::getInstance()->getToolInfoById(m_itemId1);
    if(info1.ishot&&info1.isShowHot)
        ToolController::getInstance()->buyHotTool(m_itemId1,buyCount, CCCallFunc::create(this, callfunc_selector(StoreMallCell::onRetBuyTool)),0,"StoreMall");
    else
        ToolController::getInstance()->buyTool(m_itemId1,buyCount, CCCallFunc::create(this, callfunc_selector(StoreMallCell::onRetBuyTool)),0,false,true,"StoreMall");
}
void StoreMallCell::onRetBuyTool()
{
    ToolInfo& info = ToolController::getInstance()->getToolInfoById(m_itemId);
    if(info.ishot&&info.isShowHot)
        setLeftHot(true,info.hotnum);
    
    if(m_itemId1!=-1){
        ToolInfo& info1 = ToolController::getInstance()->getToolInfoById(m_itemId1);
        if(info1.ishot&&info1.isShowHot)
            setRightHot(true, info1.hotnum);
    }
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_BUY_CONFIRM_OK);
    m_isClick = false;
}

void StoreMallCell::onYes()
{
    //    ToolController::getInstance()->buyTool(m_itemId, 1, CCCallFunc::create(this, callfunc_selector(StoreMallCell::onUseTool)));
}
void StoreMallCell::onCloseConfirm(){
    m_isClick=false;
}


bool StoreMallCell::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode) {
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameLabel1", CCLabelIF*, m_nameLabel1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameLabel2", CCLabelIF*, m_nameLabel2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_picNode1", CCNode*, m_picNode1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_picNode2", CCNode*, m_picNode2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_priceLabel1", CCLabelIF*, m_priceLabel1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_priceLabel2", CCLabelIF*, m_priceLabel2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_itemNode1", CCNode*, m_itemNode1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_itemNode2", CCNode*, m_itemNode2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_picBG1", CCSprite*, m_picBG1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_picBG2", CCSprite*, m_picBG2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_desNode", CCNode*, m_desNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_desName", CCLabelIF*, m_desName);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_desLabel", CCLabelIF*, m_desLabel);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_buyBtnSpr1", CCScale9Sprite*, m_buyBtnSpr1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_buyBtnSpr2", CCScale9Sprite*, m_buyBtnSpr2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_saleoutSpr1", CCScale9Sprite*, m_saleoutSpr1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_saleoutSpr2", CCScale9Sprite*, m_saleoutSpr2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_buyBtngraySpr1", CCScale9Sprite*, m_buyBtngraySpr1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_buyBtngraySpr2", CCScale9Sprite*, m_buyBtngraySpr2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_goldSpr1", CCSprite*, m_goldSpr1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_goldSpr2", CCSprite*, m_goldSpr2);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_itemHotNode1", CCNode*, m_itemHotNode1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_hotSpr1", CCSprite*, m_hotSpr1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_hotgraySpr1", CCSprite*, m_hotgraySpr1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_hotNumSpr1", CCSprite*, m_hotNumSpr1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_hotNumgraySpr1", CCSprite*, m_hotNumgraySpr1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_hotdesText1", CCLabelIF*, m_hotdesText1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_hotNum1Text", CCLabelIF*, m_hotNum1Text);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_hotLineSpr1", CCScale9Sprite*, m_hotLineSpr1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_hotpriceLabel1", CCLabelIF*, m_hotpriceLabel1);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_itemHotNode2", CCNode*, m_itemHotNode2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_hotSpr2", CCSprite*, m_hotSpr2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_hotgraySpr2", CCSprite*, m_hotgraySpr2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_hotNumSpr2", CCSprite*, m_hotNumSpr2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_hotNumgraySpr2", CCSprite*, m_hotNumgraySpr2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_hotdesText2", CCLabelIF*, m_hotdesText2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_hotNum2Text", CCLabelIF*, m_hotNum2Text);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_hotLineSpr2", CCScale9Sprite*, m_hotLineSpr2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_hotpriceLabel2", CCLabelIF*, m_hotpriceLabel2);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_newHotNodeL", CCNode*, m_newHotNodeL);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_newHotSprPL", CCSprite*, m_newHotSprPL);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_newHotTxtL", CCLabelIF*, m_newHotTxtL);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_newHotSprRL", CCSprite*, m_newHotSprRL);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_newHotNodeR", CCNode*, m_newHotNodeR);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_newHotSprPR", CCSprite*, m_newHotSprPR);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_newHotTxtR", CCLabelIF*, m_newHotTxtR);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_newHotSprRR", CCSprite*, m_newHotSprRR);
    
    return false;
}
void StoreMallCell::setLeftHot(bool ishot,int num){
    m_itemHotNode1->setVisible(ishot);
    m_hotpriceLabel1->setVisible(ishot);
    m_hotNum1Text->setString(CC_CMDITOA(num));
    m_hotdesText1->setString(_lang("101224"));
    
    m_hotLineSpr1->setVisible(ishot);
    m_priceLabel1->setPositionY(11);
    if(ishot){
        if(num>0){
            m_buyBtngraySpr1->setVisible(false);
            m_saleoutSpr1->setVisible(false);
            m_hotgraySpr1->setVisible(false);
            m_hotSpr1->setVisible(true);
            m_hotNumSpr1->setVisible(true);
            m_hotNumgraySpr1->setVisible(false);
            m_nameLabel1->setColor({194,158,116});
            m_priceLabel1->setColor({255,255,0});
            m_hotpriceLabel1->setColor({221,172,33});
        }else{
            m_buyBtngraySpr1->setVisible(true);
            m_saleoutSpr1->setVisible(true);
            m_hotNumSpr1->setVisible(false);
            m_hotNumgraySpr1->setVisible(true);
            m_hotgraySpr1->setVisible(true);
            m_hotSpr1->setVisible(false);
            m_nameLabel1->setColor({156,156,156});
            m_priceLabel1->setColor({156,156,156});
            m_hotpriceLabel1->setColor({156,156,156});
            auto pic = dynamic_cast<CCSprite*>(m_picNode1->getChildByTag(GOODS_ICON_TAG));
            if(pic){
                pic->setColor({90,85,81});
            }
            auto pic1 = dynamic_cast<CCSprite*>(m_picNode1->getChildByTag(GOODS_BG_TAG));
            if(pic1){
                pic1->setColor({90,85,81});
            }
            m_goldSpr1->setColor({90,85,81});
            
        }
    }else{
        m_priceLabel1->setPositionY(0);
    }
}
void StoreMallCell::setRightHot(bool ishot,int num){
    m_itemHotNode2->setVisible(ishot);
    m_hotpriceLabel2->setVisible(ishot);
    m_hotNum2Text->setString(CC_CMDITOA(num));
    m_hotdesText2->setString(_lang("101224"));
    m_hotLineSpr2->setVisible(ishot);
    m_priceLabel2->setPositionY(11);
    if(ishot){
        if(num>0){
            m_saleoutSpr2->setVisible(false);
            m_buyBtngraySpr2->setVisible(false);
            m_hotNumSpr2->setVisible(true);
            m_hotNumgraySpr2->setVisible(false);
            m_hotgraySpr2->setVisible(false);
            m_hotSpr2->setVisible(true);
            m_nameLabel2->setColor({194,158,116});
            m_priceLabel2->setColor({255,255,0});
            m_hotpriceLabel2->setColor({221,172,33});
        }else{
            m_saleoutSpr2->setVisible(true);
            m_buyBtngraySpr2->setVisible(true);
            m_hotNumSpr2->setVisible(false);
            m_hotNumgraySpr2->setVisible(true);
            m_hotgraySpr2->setVisible(true);
            m_hotSpr2->setVisible(false);
            m_nameLabel2->setColor({156,156,156});
            m_priceLabel2->setColor({156,156,156});
            m_hotpriceLabel2->setColor({156,156,156});
            auto pic = dynamic_cast<CCSprite*>(m_picNode2->getChildByTag(GOODS_ICON_TAG));
            if(pic){
                pic->setColor({90,85,81});
            }
            auto pic1 = dynamic_cast<CCSprite*>(m_picNode2->getChildByTag(GOODS_BG_TAG));
            if(pic1){
                pic1->setColor({90,85,81});
            }
            m_goldSpr2->setColor({90,85,81});
        }
    }else{
        m_priceLabel2->setPositionY(0);
    }
}
void StoreMallCell::setLeftNewHot(float flag){
    m_itemHotNode1->setVisible(false);
    m_hotLineSpr1->setVisible(false);
    m_hotpriceLabel1->setVisible(false);
    if (flag==99.0 || (flag>=0.1 && flag<=9.9)) {
        m_newHotNodeL->setVisible(true);
        if (flag==99.0) {
            m_newHotSprPL->setVisible(false);
            m_newHotTxtL->setVisible(false);
            m_newHotSprRL->setVisible(true);
        }else if (flag>=0.1 && flag<=9.9){
           // m_newHotSprPL->setVisible(true);
            //m_newHotTxtL->setVisible(true);
            int count = 100-flag*10;
            string value = "-";
            value+=CC_ITOA(count);
            value+="%";
            m_newHotTxtL->setString(value);
            m_newHotSprRL->setVisible(false);
        }
    }else{
        m_newHotNodeL->setVisible(false);
    }
    m_priceLabel1->setPositionY(0);
}
void StoreMallCell::setRightNewHot(float flag){
    m_itemHotNode2->setVisible(false);
    m_hotpriceLabel2->setVisible(false);
    m_hotLineSpr2->setVisible(false);
    if (flag==99.0 || (flag>=0.1 && flag<=9.9)) {
        m_newHotNodeR->setVisible(true);
        if (flag==99.0) {
            m_newHotSprPR->setVisible(false);
            m_newHotTxtR->setVisible(false);
            m_newHotSprRR->setVisible(true);
        }else if (flag>=0.1 && flag<=9.9){
           // m_newHotSprPR->setVisible(true);
          //  m_newHotTxtR->setVisible(true);
            int count = 100-flag*10;
            string value = "-";
            value+=CC_ITOA(count);
            value+="%";
            m_newHotTxtR->setString(value);
            m_newHotSprRR->setVisible(false);
        }
    }else{
        m_newHotNodeR->setVisible(false);
    }
    m_priceLabel2->setPositionY(0);
}
