//
//  AllianceInfoView.cpp
//  IF
//
//  Created by ganxiaohua on 14-3-25.
//
//

#include "AllianceInfoView.h"
#include "PopupViewController.h"
#include "SoundController.h"
#include "AllianceInfoMembersView.h"
#include "MailWritePopUpView.h"
#include "AllianceInfoMembersView.h"
#include "UpdateAllianceInfoView.h"
#include "AllianceMarketView.h"
#include "FunBuildController.h"
#include "AllianceHelpView.h"
#include "ChangeAllianceFlagView.h"
#include "AllianceApplyView.h"
#include "UIComponent.h"
#include "ParticleFireAni.h"
#include "AllianceFlagPar.h"
#include "AllianceEditAnnounceView.h"
#include "RollingText.h"
#include "ChatController.h"
#include "AllianceScienceView.h"
#include "ClippingNodeExt.h"
#include "AllianceWarView.h"
#include "AllianceCheckView.h"
#include "AllianceShopView.h"
#include "AllianceEventCommand.h"
#include "SceneController.h"
#include "WorldMapView.h"
#include "AllianceInviteView.h"
#include "MailController.h"
#include "Utf8Utils.h"
#include "AllianceFunView.h"
#include "AllianceCommentView.h"
#include "ChatView.h"
#include "SoundController.h"
#include "AllianceTerritoryView.h"

#include "AllianceDailyPublishView.h"
#include "AllianceDailyController.h"

static const char* iconEvents[15] = {"event_fail.png","event_fail.png","event_science.png","event_add.png","event_exit.png","event_announce.png","event_announce.png","tile_pop_icon21.png","tile_pop_icon21.png","event_announce.png","event_announce.png","event_announce.png","",""};//占领为0，掠夺为1，科技为2，加入联盟为3，退出为4 公告为5
static string m_guideKey = "";

#define CellCount 7
enum CellNum
{
    Cell_0,
    Cell_1,
    Cell_2,
    Cell_3,
    Cell_4,
    Cell_5,
    Cell_6,
    Cell_7
};

AllianceInfoView* AllianceInfoView::create(AllianceInfo* info, bool bRef){
    AllianceInfoView* ret = new AllianceInfoView(info, bRef);
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}
 

bool AllianceInfoView::init()
{
    if(GlobalData::shared()->playerInfo.isInAlliance()){
       // GlobalData::shared()->playerInfo.isfirstJoin = 0;
    }
    if (!PopupBaseView::init()) {
        return false;
    }
     setIsHDPanel(true);
    m_eventNum = 0;
    m_waitInterface = NULL;
    CCLoadSprite::doResourceByCommonIndex(205, true,true);
    CCLoadSprite::doResourceByCommonIndex(7, true,true);
    setCleanFunction([](){
        CCLoadSprite::doResourceByCommonIndex(205, false,true);
        CCLoadSprite::doResourceByCommonIndex(7, false,true);
    });
    auto tbg = CCLoadSprite::loadResource("technology_09.png");
    auto tBatchNode = CCSpriteBatchNode::createWithTexture(tbg->getTexture());
    int maxHight = CCDirector::sharedDirector()->getWinSize().height;
    int curHight = -500;
    while (curHight<maxHight) {
        auto bg = CCLoadSprite::createSprite("technology_09.png");
        bg->setAnchorPoint(ccp(0, 1));
        bg->setPosition(ccp(0, curHight));
        curHight += bg->getContentSize().height;
        tBatchNode->addChild(bg);
        if (CCCommonUtils::isIosAndroidPad())
        {
            bg->setScale(2.4f);
        }
    }
    this->addChild(tBatchNode);
    
    auto tmpCCB = CCBLoadFile("NewAllianceInfoView02",this,this);
    this->setContentSize(tmpCCB->getContentSize());
    int preH = m_viewBg->getContentSize().height;
    changeBGHeight(m_viewBg);
    int newH = m_viewBg->getContentSize().height;
    m_add = newH- preH;
    m_viewBg->setVisible(false);
    m_funList->setContentSize(CCSize(m_funList->getContentSize().width,m_funList->getContentSize().height+m_add));
    m_funList->setPositionY(m_funList->getPositionY()-m_add);
    m_eventNode->setContentSize(CCSize(m_eventNode->getContentSize().width,m_eventNode->getContentSize().height+m_add));
    m_eventNode->setPositionY(m_eventNode->getPositionY()-m_add);
    m_btnNode->setPositionY(m_btnNode->getPositionY()-m_add);
    
    if (m_guideNode2) {
        m_guideNode2->setContentSize(m_eventNode->getContentSize());
        m_guideNode2->setPosition(m_eventNode->getPosition());
    }
    
    if(m_info!=nullptr){
        std::string str = "(";
        str.append(m_info->shortName.c_str());
        str.append(") ");
        str.append(m_info->name.c_str());
        m_nameTxt->setString(str.c_str());
        str = _lang_2("115016", CC_ITOA(m_info->currentNum),CC_ITOA(m_info->getSelfAllianceNum()));
        m_membersTxt->setString(str);
        str = _lang_1("115015", m_info->leader.c_str());
        m_leaderTxt->setString(str.c_str());
        str = _lang_1("115017", CC_CMDITOAL(m_info->totalForce).c_str());
        m_powerTxt->setString(str);
    }

    m_allianceIcon->removeAllChildrenWithCleanup(true);

    m_bottomNode->setVisible(false);
    m_bottomNode->setPositionY(m_bottomNode->getPositionY()-m_add);
    //m_btnNode->removeFromParent();

    m_testTxt = CCLabelIFTTF::create("", 20);
    m_testTxt->setAlignment(kCCTextAlignmentLeft);
    m_testTxt->setVerticalAlignment(kCCVerticalTextAlignmentTop);
    m_testTxt->setDimensions(CCSize(515,0));
    m_testTxt->setAnchorPoint(ccp(0, 1));
    if (CCCommonUtils::isIosAndroidPad())
    {
        m_testTxt->setDimensions(CCSize(1350,0));
        m_testTxt->setFontSize(36);
    }
    
    m_inviteTxt->setString(_lang("115265"));
    m_mailTxt->setString(_lang("115900"));
    m_memberTxt->setString(_lang("115266"));
    m_managerTxt->setString(_lang("115258"));
    
    m_data = CCArray::create();
    m_srcData = CCArray::create();
    m_dataCells = CCArray::create();
    
    m_tabView = CCTableView::create(this, m_eventNode->getContentSize());
    m_tabView->setDirection(kCCScrollViewDirectionVertical);
    m_tabView->setVerticalFillOrder(kCCTableViewFillTopDown);
    m_tabView->setTouchPriority(Touch_Default);
    m_eventNode->addChild(m_tabView);
    
    m_scrollView = CCScrollView::create(m_funList->getContentSize());
    m_scrollView->setDirection(kCCScrollViewDirectionVertical);
    m_scrollView->setTouchPriority(Touch_Popup);
    m_funList->addChild(m_scrollView);
    //CCCommonUtils::setSpriteGray(m_inviteSpr, true);
    
    map<string, AllianceEventInfo* >::iterator it = AllianceManager::getInstance()->eventsMap.find(ALLIANCE_NOTICE_KEY);
    if(it!=AllianceManager::getInstance()->eventsMap.end()){
        m_data->addObject(it->second);
    }else{
        AllianceManager::getInstance()->addAnnounceEvent();
        map<string, AllianceEventInfo* >::iterator its = AllianceManager::getInstance()->eventsMap.find(ALLIANCE_NOTICE_KEY);
        if(its!=AllianceManager::getInstance()->eventsMap.end()){
            m_data->addObject(its->second);
        }
    }
    
    m_init = true;
    m_guideKey = "";
    
    auto& build = FunBuildController::getInstance()->getFunbuildById(FUN_BUILD_MAIN_CITY_ID);
    if (build.level <= 6) {
        if (GlobalData::shared()->playerInfo.isInAlliance() && m_info->uid==GlobalData::shared()->playerInfo.allianceInfo.uid) {
            GuideController::share()->checkSubGuide("3160100");
        }
    }
    
    return true;
}

void AllianceInfoView::sortData(){
    m_data->removeAllObjects();
    int num = m_srcData->count();
    for (int i=0; i<num; i++) {
        for (int j=i; j<num; j++) {
            AllianceEventInfo* event1 = (AllianceEventInfo*)m_srcData->objectAtIndex(i);
            AllianceEventInfo* event2 = (AllianceEventInfo*)m_srcData->objectAtIndex(j);
            if(event2->createTime < event1->createTime){
                m_srcData->swap(i, j);
            }
        }
    }
    m_srcData->reverseObjects();
    m_data->addObjectsFromArray(m_srcData);
}

void AllianceInfoView::addLoadingAni(){
    if (m_loadingIcon==nullptr) {
        m_loadingIcon = CCLoadSprite::createSprite("loading_1.png");
        m_loadingIcon->setAnchorPoint(ccp(0.5, 0.5));
        auto cSize = m_eventNode->getContentSize();
        m_loadingIcon->setPosition(ccp(cSize.width/2,cSize.height/2));
    }
    CCActionInterval * rotateto1 = CCRotateTo::create(0.5, 720);
    CCFiniteTimeAction* rotateAction = CCSequence::create(rotateto1,NULL);
    CCActionInterval * rotateForever =CCRepeatForever::create((CCActionInterval* )rotateAction);
    m_loadingIcon->runAction(rotateForever);
    if(m_loadingIcon->getParent()==NULL){
        m_eventNode->addChild(m_loadingIcon,1000000);
    }
}

void AllianceInfoView::removeLoadingAni(CCObject* p){
    if(m_loadingIcon!=nullptr){
        m_loadingIcon->stopAllActions();
        m_loadingIcon->removeFromParent();
    }
}

CCNode* AllianceInfoView::getGuideNode(string _key)
{
    m_guideKey = _key;
    if ( _key == "ALINFO_1" && m_guideNode1 ) {
        return m_guideNode1;
    }else if ( _key == "ALINFO_2" && m_guideNode2) {
        return m_guideNode2;
    }else if ( _key == "ALINFO_3" && m_guideNode3 ) {
        return m_guideNode3;
    }
    return NULL;
}

void AllianceInfoView::callMore(){
    if (m_eventNum!=0 && m_data->count()-1>=m_eventNum) {
        return;
    }
    std::string lastTime = "0";
    double minTime = GlobalData::shared()->getWorldTime()+10000000000;
    map<string, AllianceEventInfo* >::iterator it;
    for(it = AllianceManager::getInstance()->eventsMap.begin();it != AllianceManager::getInstance()->eventsMap.end();it++)
    {
        AllianceEventInfo* event = it->second;
        if (it->second->createTime<=minTime && it->second->type!=5) {
            minTime = it->second->createTime;
            lastTime = it->second->srcTime;
        }
    }
    AllianceEventCommand* cmd = new AllianceEventCommand(lastTime);
    cmd->setCallback(CCCallFuncO::create(this, callfuncO_selector(AllianceInfoView::getCallData), NULL));
    //cmd->setFailCallback(CCCallFuncO::create(this, callfuncO_selector(AllianceInfoView::removeLoadingAni), NULL));
    cmd->sendAndRelease();
    if(m_waitInterface!=NULL){
        m_waitInterface->remove();
        m_waitInterface = NULL;
    }
    m_waitInterface = GameController::getInstance()->showWaitInterface(m_EditNode);
    //this->addLoadingAni();
}

void AllianceInfoView::getCallData(CCObject* param){
    if(this->getParent()==NULL){
        return;
    }
    if(m_waitInterface!=NULL){
        m_waitInterface->remove();
        m_waitInterface = NULL;
    }
    NetResult* result = dynamic_cast<NetResult*>(param);
    if(result==NULL) return ;
    CCDictionary* dict = dynamic_cast<CCDictionary*>(result->getData());
    if(dict==NULL || dict->objectForKey("errorCode")) return;
    
    m_data->removeAllObjects();
    m_srcData->removeAllObjects();
    m_eventNum = dict->valueForKey("eventNum")->intValue();
    CCArray* array = dynamic_cast<CCArray*>(dict->objectForKey("event"));
    if (array==NULL) return;
    int num = array->count();
    for (int i=0; i<num; i++) {
        auto event = _dict(array->objectAtIndex(i));
        AllianceEventInfo* info = AllianceEventInfo::create();
        info->parse(event);
        map<string, AllianceEventInfo* >::iterator it = AllianceManager::getInstance()->eventsMap.find(info->srcTime);
        if(it!=AllianceManager::getInstance()->eventsMap.end()){
            it->second->parse(event);
        }else{
            info->retain();
            AllianceManager::getInstance()->eventsMap[info->srcTime] = info;
        }
    }
    this->getEvents();
    this->sortData();
    if(!m_bottomNode->isVisible() && m_init){
        initFun();
    }else{
        CCPoint curr = m_tabView->getContentOffset();
        CCPoint min = m_tabView->minContainerOffset();
        m_tabView->reloadData();
        curr.y += m_tabView->minContainerOffset().y - min.y;
        m_tabView->setContentOffset(curr);
    }
    m_init = false;
    //m_tabView->reloadData();
}

CCSize AllianceInfoView::tableCellSizeForIndex(CCTableView *table, ssize_t idx)
{
    if(idx >= m_data->count()){
        return CCSizeZero;
    }
    AllianceEventInfo* info = dynamic_cast<AllianceEventInfo*>(m_data->objectAtIndex(idx));
    m_testTxt->setString(info->getContent().c_str());
    int msgHeight = m_testTxt->getContentSize().height*m_testTxt->getOriginScaleY() + 6;
    msgHeight = MAX(msgHeight, 56);
    if (CCCommonUtils::isIosAndroidPad()) {
        msgHeight = MAX(msgHeight, 112);
    }
    msgHeight = msgHeight + 8;
    if(idx==0){
        msgHeight += 40;
        if (CCCommonUtils::isIosAndroidPad()) {
            msgHeight += 40;
        }
    }else{
        if (idx > 0)
        {
            AllianceEventInfo* info = dynamic_cast<AllianceEventInfo*>(m_data->objectAtIndex(idx));
            AllianceEventInfo* last = dynamic_cast<AllianceEventInfo*>(m_data->objectAtIndex(idx-1));
            auto gapTime = last->createTime - info->createTime;
            if (gapTime >= 60*10) {
                msgHeight += 30;
                if (CCCommonUtils::isIosAndroidPad()) {
                    msgHeight += 30;
                }
            }
        }
    }
    if (CCCommonUtils::isIosAndroidPad())
    {
        return CCSize(1536, msgHeight);
    }
    return CCSize(640, msgHeight);
}

CCSize AllianceInfoView::cellSizeForTable(CCTableView *table)
{
    if (CCCommonUtils::isIosAndroidPad())
    {
        return CCSize(1536, 120);
    }
    return CCSize(640, 60);
}

CCTableViewCell* AllianceInfoView::tableCellAtIndex(CCTableView *table, ssize_t idx)
{
    if(idx >= m_data->count()){
        return NULL;
    }
    AllianceEventCell* cell = (AllianceEventCell*)table->dequeueCell();
    AllianceEventInfo* info = dynamic_cast<AllianceEventInfo*>(m_data->objectAtIndex(idx));
    AllianceEventInfo* last = NULL;
    if(idx>0){
        last = dynamic_cast<AllianceEventInfo*>(m_data->objectAtIndex(idx-1));
    }
    if(idx < m_data->count()){
        if(cell){
            cell->setData(info,last);
        }else{
            cell = AllianceEventCell::create(info,last);
            m_dataCells->addObject(cell);
        }
    }
    return cell;
}

ssize_t AllianceInfoView::numberOfCellsInTableView(CCTableView *table)
{
    return m_data->count();
}

void AllianceInfoView::tableCellWillRecycle(CCTableView* table, CCTableViewCell* cell)
{
    m_dataCells->addObject(cell);
}

void AllianceInfoView::initFun(){
    if(!m_info){
        this->closeSelf();
        return;
    }
    CCLoadSprite::doResourceByCommonIndex(7, true);
    
    //m_btnNode->removeFromParent();
    m_scrollView->getContainer()->removeAllChildrenWithCleanup(true);
    std::string str = "(";
    str.append(m_info->shortName.c_str());
    str.append(") ");
    str.append(m_info->name.c_str());
    m_nameTxt->setString(str.c_str());
    str = CC_ITOA(m_info->currentNum);
    str.append("/");
    str.append(CC_ITOA(m_info->getSelfAllianceNum()));
    m_membersTxt->setString(str);
    str = _lang_1("115015", m_info->leader.c_str());
    m_leaderTxt->setString(str.c_str());
    m_powerTxt->setString(CC_CMDITOAL(m_info->totalForce).c_str());

    const char* titles3[CellCount] = {"115190","115301","115159","115206","115077","115929","134000"};
    const char* icons3[CellCount] = {"allianceWar.png","AllianceTerritory.png","allianceScience.png","allianceShop.png","allianceHelp.png","icon_comment.png","alliance_dail_quest_b.png"};
    
    int num =6;
    if(AllianceDailyController::getInstance()->isSwitchOpen()){
        num = CellCount;
    }
    int totalH = 85* num+30 + 85 +140;
    if(CCCommonUtils::isIosAndroidPad())
    {
        totalH = 170.0* num+30 + 170 +140;
    }
    if(m_funList->getContentSize().height>totalH){
        totalH = m_funList->getContentSize().height;
    }
    if(m_data->count()>0){
        AllianceEventInfo* eventInfo = dynamic_cast<AllianceEventInfo*>(m_data->objectAtIndex(0));
        if( eventInfo )
        {
            AllianceEventCell* eventCell = AllianceEventCell::create(eventInfo);
            eventCell->showCell(eventInfo);
            if(CCCommonUtils::isIosAndroidPad())
            {
                eventCell->setPosition(ccp(5, totalH - 84*2 - 50));
            }
            else
                eventCell->setPosition(ccp(5, totalH - 84 - 25));
            m_scrollView->addChild(eventCell);
        }
    }
    for (int i=0; i<num; i++) {
        // liu  屏蔽联盟其他项
        bool isContinue = true;
        if(i == 0 || i == 4)
        {
            isContinue = false;
        }
        if(isContinue)
        {
            continue;
        }
        int kV = i;
        if(kV == 4)
            kV = 1;
        if(AllianceDailyController::getInstance()->isSwitchOpen()){
           
            
          //  if (i==0) {
                const char*  title = titles3[i];
                const char*  iconStr = icons3[i];
                AllianceFunCell* cell = AllianceFunCell::create(_lang(title), m_info,iconStr,m_funList,i+1);
                if(CCCommonUtils::isIosAndroidPad())
                    cell->setPosition(ccp(5, totalH-(kV+1)*111*2. - 110*2.));
                else
                    cell->setPosition(ccp(5, totalH-(kV+1)*111 - 110));
                m_scrollView->addChild(cell);
//            }
//            else if (i>0 && i<num-1){
//                const char*  title = titles3[i];
//                const char*  iconStr = icons3[i];
//                AllianceFunCell* cell = AllianceFunCell::create(_lang(title), m_info,iconStr,m_funList,i+1);
//                if(CCCommonUtils::isIosAndroidPad())
//                    cell->setPosition(ccp(5, totalH-(1+1+1)*111*2. - 110*2.));
//                else
//                    cell->setPosition(ccp(5, totalH-(1+1+1)*111 - 110));
//                m_scrollView->addChild(cell);
//            }else if (i==num-1){
//                const char*  title = titles3[i];
//                const char*  iconStr = icons3[i];
//                AllianceFunCell* cell = AllianceFunCell::create(_lang(title), m_info,iconStr,m_funList,Cell_7);
//                if(CCCommonUtils::isIosAndroidPad())
//                    cell->setPosition(ccp(5, totalH-(1+1)*111*2. - 110*2.));
//                else
//                    cell->setPosition(ccp(5, totalH-(1+1)*111 - 110));
//                m_scrollView->addChild(cell);
//            }
            
        }else{
            
            const char*  title = titles3[i];
            const char*  iconStr = icons3[i];
            AllianceFunCell* cell = AllianceFunCell::create(_lang(title), m_info,iconStr,m_funList,i+1);
            if(CCCommonUtils::isIosAndroidPad())
                cell->setPosition(ccp(5, totalH-(kV+1)*111*2. - 110*2.));
            else
                cell->setPosition(ccp(5, totalH-(kV+1)*111 - 110));
            m_scrollView->addChild(cell);
        }
    }
    m_scrollView->setContentSize(CCSize(m_funList->getContentSize().width,totalH));
    m_scrollView->setContentOffset(ccp(0, m_funList->getContentSize().height - totalH));
    
//    m_btnNode->removeFromParent();
//    m_btnNode->setPositionY(22);
//    m_scrollView->addChild(m_btnNode);
    //m_scrollView->setTouchEnabled(totalH < m_funList->getContentSize().height);
    m_allianceIcon->removeAllChildren();
    AllianceFlagPar* flag = AllianceFlagPar::create(m_info->getAllianceIcon() );
    m_allianceIcon->addChild(flag);
    string lang = ( strcmp(m_info->language.c_str(), "")==0?"115600":m_info->language );
    m_language->setString(_lang(lang).c_str());
    
 
}

void AllianceInfoView::delayShowAnnounce(float t){
    this->unschedule(schedule_selector(AllianceInfoView::delayShowAnnounce));
    std::string annStr = "";
    if(GlobalData::shared()->playerInfo.isInAlliance() && m_info->uid==GlobalData::shared()->playerInfo.allianceInfo.uid){
        if(GlobalData::shared()->playerInfo.allianceInfo.rank==5 && m_info->announce==""){
            annStr = _lang("115086").c_str();
        }else if(m_info->announce==""){
            annStr = _lang("115087").c_str();
        }else{
            annStr = m_info->announce.c_str();
        }
    }else{
        annStr = m_info->intro;
    }
    this->showAnnounce(annStr);
}

void AllianceInfoView::onEnter()
{
    CCNode::onEnter();
    UIComponent::getInstance()->showPopupView(1);
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);
    m_scrollView->setTouchEnabled(true);
    CCLoadSprite::doResourceByCommonIndex(7, true);
    CCLoadSprite::doResourceByCommonIndex(205, true);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(AllianceInfoView::changeAllicaneIcon), MSG_CHANGE_ALLIANCE_ICON, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(AllianceInfoView::updateAnnounce), MSG_CHANGE_ALLIANCE_ANNOUNCE, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(AllianceInfoView::updateAllianceEvent), MSG_ADD_ALLIANCE_EVENT, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(AllianceInfoView::updateApplyFlag), MSG_CHANGE_ALLIANCE_APPLY_NUM, NULL);
    if(GlobalData::shared()->playerInfo.isInAlliance() && GlobalData::shared()->playerInfo.allianceInfo.uid == m_info->uid){
//        this->setTitleName(_lang("115000"));
        m_titleTxt->setString(_lang("115000"));
    }else{
//        this->setTitleName(_lang("115025"));
        m_titleTxt->setString(_lang("115025"));
    }
    if(!m_bottomNode->isVisible()){
         initFun();
    }
    if(m_eventNum==0){
        this->callMore();
    }
    if (GlobalData::shared()->playerInfo.allianceInfo.uid == m_info->uid && GlobalData::shared()->playerInfo.allianceInfo.rank >= 4 && GlobalData::shared()->playerInfo.allianceInfo.applyNum > 0) {
        m_applyNode->setVisible(true);
        m_applyTipNum->setString("N");
        playApplyTipAnim();
    }
    else
        m_applyNode->setVisible(false);
}

void AllianceInfoView::onExit()
{
    if(m_waitInterface!=NULL){
        m_waitInterface->remove();
        m_waitInterface = NULL;
    }
    m_scrollView->setTouchEnabled(false);
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_ALLIACNE_HELP_NUM_CHANGE);
    
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_CHANGE_ALLIANCE_ICON);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_CHANGE_ALLIANCE_ANNOUNCE);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_ADD_ALLIANCE_EVENT);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_CHANGE_ALLIANCE_APPLY_NUM);
    setTouchEnabled(false);
    CCNode::onExit();
}

//播放上下动画三次
void AllianceInfoView::playApplyTipAnim(){
    m_applyNodePt = m_applyNode->getPosition();
    float x = m_applyNodePt.x;
    float y = m_applyNodePt.y;
    CCEaseExponentialIn* easeInAction = CCEaseExponentialIn::create(dynamic_cast<CCActionInterval*>(CCMoveTo::create(0.2, ccp(x, y+10))));
    CCFiniteTimeAction* easeInActionRevers = CCMoveTo::create(0.2, ccp(x, y));
    
    CCEaseExponentialIn* easeInAction1 = CCEaseExponentialIn::create(dynamic_cast<CCActionInterval*>(CCMoveTo::create(0.2, ccp(x, y-10))));
    CCMoveTo* easeInAction1Revers = CCMoveTo::create(0.2, ccp(x, y));
    CCRepeat* repeatAction = CCRepeat::create(CCSequence::create(
                                                                 easeInAction
                                                                 , easeInActionRevers
                                                                 , easeInAction1
                                                                 ,easeInAction1Revers
                                                                 ,NULL)
                                              , 3);
    this->m_applyNode->runAction(CCSequence::create(
                                                       repeatAction
                                                       , CCCallFunc::create(this, callfunc_selector(AllianceInfoView::playApplyTipAnimFinish))
                                                       , NULL
                                                       ));
    
}
void AllianceInfoView::playApplyTipAnimFinish(){
    this->m_applyNode->setPosition(m_applyNodePt);
}

void AllianceInfoView::updateAnnounce(CCObject* param){
    std::string annStr = "";
    annStr = GlobalData::shared()->playerInfo.allianceInfo.announce;
    map<string, AllianceEventInfo* >::iterator it = AllianceManager::getInstance()->eventsMap.find(ALLIANCE_NOTICE_KEY);
    if(it!=AllianceManager::getInstance()->eventsMap.end()){
        it->second->content = annStr;
        it->second->createTime = GlobalData::shared()->getTimeStamp();
    }
    this->getEvents();
    this->sortData();
    CCPoint curr = m_tabView->getContentOffset();
    CCPoint min = m_tabView->minContainerOffset();
    m_tabView->reloadData();
    curr.y += m_tabView->minContainerOffset().y - min.y;
    m_tabView->setContentOffset(curr);
    //this->showAnnounce(annStr);
    //发聊天信息
    annStr.append("  (").append(_lang("115181")).append(")");
    ChatController::getInstance()->sendCountryChat(annStr.c_str(), CHAT_ALLIANCE, 1);
}

void AllianceInfoView::updateAllianceEvent(CCObject* param){
    m_data->removeAllObjects();
    m_srcData->removeAllObjects();
    this->getEvents();
    this->sortData();

    CCPoint curr = m_tabView->getContentOffset();
    CCPoint min = m_tabView->minContainerOffset();
    m_btnNode->setVisible(false);
    m_scrollView->getContainer()->removeAllChildrenWithCleanup(true);
    m_bottomNode->setVisible(true);
    m_funList->setVisible(false);
    m_eventNode->setVisible(true);
    m_tabView->reloadData();
    curr.y += m_tabView->minContainerOffset().y - min.y;
    m_tabView->setContentOffset(curr);
}

void AllianceInfoView::getEvents(){
    m_srcData->removeAllObjects();
    map<string, AllianceEventInfo* >::iterator it;
    for(it = AllianceManager::getInstance()->eventsMap.begin();it != AllianceManager::getInstance()->eventsMap.end();it++)
    {
        m_srcData->addObject(it->second);
    }
}

void AllianceInfoView::showAnnounce(std::string announce){

}

void AllianceInfoView::changeAllicaneIcon(CCObject* param){
    CCLoadSprite::doResourceByCommonIndex(205, true);
    CCLoadSprite::doResourceByCommonIndex(7, true);
    m_allianceIcon->removeAllChildrenWithCleanup(true);
    AllianceFlagPar* spr = AllianceFlagPar::create(m_info->getAllianceIcon().c_str());
    m_allianceIcon->addChild(spr);
}

bool AllianceInfoView::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
    m_touchPos = pTouch->getLocation();
    m_isMove = false;
    m_eventNode->setVisible(m_bottomNode->isVisible());
//    if(isTouchInside(m_funList, pTouch)){
//        return true;
//    }
//    if(isTouchInside(m_allianceIconClickNode, pTouch)){
//        //m_allianceIcon->setScale(1.1);
//        return true;
//    }
    return true;
}

void AllianceInfoView::onTouchMoved(CCTouch *pTouch, CCEvent *pEvent){
    CCPoint pos = pTouch->getLocation();
    if (fabsf(m_touchPos.y - pos.y)>30) {
        m_isMove = true;
    }
}

void AllianceInfoView::onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    if (m_guideKey == "ALINFO_1" && m_guideNode1) {
        if(isTouchInside(m_guideNode1, pTouch)) {
            m_guideKey = "";
            CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(GUIDE_INDEX_CHANGE, CCString::create("ALINFO_1"));
            return;
        }
    }else if (m_guideKey == "ALINFO_2" && m_guideNode2) {
        if(isTouchInside(m_guideNode2, pTouch)) {
            CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(GUIDE_INDEX_CHANGE, CCString::create("ALINFO_2"));
            return;
        }
    }else if (m_guideKey == "ALINFO_3" && m_guideNode3) {
        if(isTouchInside(m_guideNode3, pTouch)) {
            m_guideKey = "";
            CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(GUIDE_INDEX_CHANGE, CCString::create("ALINFO_3"));
            return;
        }
    }
    
    
    CCPoint pos = pTouch->getLocation();
    m_eventNode->setVisible(m_bottomNode->isVisible());
    if(fabsf(pos.y - m_touchPos.y)>30 && m_bottomNode->isVisible()){
        if (m_isMove) {
            m_isMove = false;
            CCPoint curr = m_tabView->getContentOffset();
            //CCPoint min = m_tabView->minContainerOffset();
            CCPoint max = m_tabView->maxContainerOffset();
            if ((curr.y - max.y) > 40 && m_data->count()>=5 && m_data->count()<100) {
                this->callMore();
            }
        }
    }
    if(isTouchInside(m_allianceIconClickNode, pTouch)){
        //PopupViewController::getInstance()->addPopupView(AllianceCheckView::create(),true,true);
    }if(m_bottomNode->isVisible() && fabsf(pos.y - m_touchPos.y)<30 ){
        bool flag = false;
        if(GlobalData::shared()->playerInfo.isInAlliance() && AllianceManager::getInstance()->checkRight(EDIT_ALLIANCE_NOTICE, GlobalData::shared()->playerInfo.allianceInfo.rank) && GlobalData::shared()->playerInfo.allianceInfo.uid == m_info->uid){
            flag = true;
        }
        for (int i=0; i<m_dataCells->count(); i++) {
            AllianceEventCell* cell = dynamic_cast<AllianceEventCell*>(m_dataCells->objectAtIndex(i));
            if (cell->checkIsEventInSide(pTouch) && isTouchInside(m_eventNode, pTouch)) {
                int type = cell->m_info->type;
                if((type==5 && !flag) || type==4){
                    return ;
                }
                CCFadeTo* fade = CCFadeTo::create(0.25, 100);
                CCFadeTo* fade4 = CCFadeTo::create(0.15, 2);
                CCDelayTime* de = CCDelayTime::create(0.05);
                CCCallFuncND* fun = CCCallFuncND::create(this, callfuncND_selector(AllianceInfoView::onClickEvent), (void *)(cell->m_info));
                CCSequence* sc = CCSequence::create(fade,fade4,de,fun,NULL);
                cell->m_clickBg->runAction(sc);
                return ;
            }
        }
    }else if(isTouchInside(m_EditNode, pTouch) && fabsf(pos.y - m_touchPos.y)<30){
        SoundController::sharedSound()->playEffects(Music_Sfx_click_button);
        this->showFunOrEvent(false);
    }else if(!m_bottomNode->isVisible() && fabsf(pos.y - m_touchPos.y)<30){
        if(isTouchInside(m_mailSpr, pTouch)){
            SoundController::sharedSound()->playEffects(Music_Sfx_click_button);
            int selfRank = GlobalData::shared()->playerInfo.allianceInfo.rank;
            if(!AllianceManager::getInstance()->checkRight(MAIL_ALL, selfRank)){
                CCCommonUtils::flyHint("", "", _lang("115836"));
            }
            else{
                MailController::getInstance()->openMailDialogViewFirst(_lang("105564"), GlobalData::shared()->playerInfo.uid);
            }
        }else if(isTouchInside(m_inviteSpr, pTouch)){
            SoundController::sharedSound()->playEffects(Music_Sfx_click_button);
            PopupViewController::getInstance()->addPopupInView(AllianceInviteView::create());
        }else if(isTouchInside(m_memberSpr, pTouch)){
            SoundController::sharedSound()->playEffects(Music_Sfx_click_button);
            PopupViewController::getInstance()->addPopupInView(AllianceInfoMembersView::create(GlobalData::shared()->playerInfo.allianceInfo.uid));
        }else if(isTouchInside(m_managerSpr, pTouch)){
            SoundController::sharedSound()->playEffects(Music_Sfx_click_button);
            PopupViewController::getInstance()->addPopupInView(AllianceFunView::create());
        }
    }
    m_allianceIcon->setScale(1.0);
}

void AllianceInfoView::showFunOrEvent(bool showFun){
    if(showFun){
        m_funList->setVisible(true);
        m_eventNode->setVisible(false);
        m_btnNode->setVisible(true);
        m_tabView->getContainer()->removeAllChildrenWithCleanup(true);
    }else{
        m_btnNode->setVisible(false);
        m_scrollView->getContainer()->removeAllChildrenWithCleanup(true);
        m_bottomNode->setVisible(true);
        m_funList->setVisible(false);
        m_eventNode->setVisible(true);
        m_tabView->reloadData();
    }
}

void AllianceInfoView::onClickEvent(cocos2d::CCNode *p,void* p1)
{
    clickEventCell(p, p1);
}

void AllianceInfoView::clickEventCell(CCObject* sender,void* pData){
    AllianceEventInfo* event =  (AllianceEventInfo*)pData;
    if(event==NULL) return ;
    long type = (long) event->type;
    int worldIndex = 0;
    switch (type) {
        case 0:
        case 1:
        {
            CCPoint pt = ccp(event->posX, event->posY);
            
            AutoSafeRef temp(this);
            //PopupViewController::getInstance()->removeAllPopupView();
            //zym 2015.12.11
            PopupViewController::getInstance()->forceClearAll(true);
            if(WorldMapView::instance()&&SceneController::getInstance()->currentSceneId == SCENE_ID_WORLD){
                WorldMapView::instance()->gotoTilePoint(pt);
            }else{
                worldIndex = WorldController::getIndexByPoint(pt);
                SceneController::getInstance()->gotoScene(SCENE_ID_WORLD, false, true, worldIndex);
            }
        }
            break;
        case 2:
            PopupViewController::getInstance()->addPopupInView(AllianceScienceView::create());
            break;
        case 3:
            PopupViewController::getInstance()->addPopupInView(AllianceInfoMembersView::create(GlobalData::shared()->playerInfo.allianceInfo.uid));
            break;
        case 5:
        {
            bool flag = false;
            if(GlobalData::shared()->playerInfo.isInAlliance() && AllianceManager::getInstance()->checkRight(EDIT_ALLIANCE_NOTICE, GlobalData::shared()->playerInfo.allianceInfo.rank) && GlobalData::shared()->playerInfo.allianceInfo.uid == m_info->uid){
                flag = true;
            }
            std::string annStr = "";
            if(GlobalData::shared()->playerInfo.isInAlliance() && m_info->uid==GlobalData::shared()->playerInfo.allianceInfo.uid){
                if(GlobalData::shared()->playerInfo.allianceInfo.rank==5 && m_info->announce==""){
                    annStr = _lang("115086").c_str();
                }else if(m_info->announce==""){
                    annStr = _lang("115087").c_str();
                }else{
                    annStr = m_info->announce.c_str();
                }
            }else{
                annStr = m_info->intro;
            }
            PopupViewController::getInstance()->addPopupView(AllianceEditAnnounceView::create(annStr,flag));
        }
            break;
        case 9:
        case 10:{
            unsigned int index = WorldController::getIndexByPoint(WorldController::getInstance()->selfPoint);
            PopupViewController::getInstance()->addPopupInView(AllianceTerritoryView::create(index,false));
        }
            break;
        default:
            break;
    }
}

void AllianceInfoView::onClickFlag(CCObject * pSender, Control::EventType pCCControlEvent){
    if(GlobalData::shared()->playerInfo.isInAlliance() && m_info->uid==GlobalData::shared()->playerInfo.allianceInfo.uid && GlobalData::shared()->playerInfo.allianceInfo.rank>=4){
         //PopupViewController::getInstance()->addPopupView(ChangeAllianceFlagView::create());
    }
}

void AllianceInfoView::updateApplyFlag(CCObject* param){
    if(GlobalData::shared()->playerInfo.isInAlliance() && GlobalData::shared()->playerInfo.allianceInfo.rank >= 4 && GlobalData::shared()->playerInfo.allianceInfo.uid == m_info->uid && GlobalData::shared()->playerInfo.allianceInfo.applyNum>0){
        m_applyNode->setVisible(true);
        m_applyTipNum->setString("N");
        playApplyTipAnim();
    }
    if(GlobalData::shared()->playerInfo.isInAlliance() && GlobalData::shared()->playerInfo.allianceInfo.rank >= 4 && GlobalData::shared()->playerInfo.allianceInfo.uid == m_info->uid && GlobalData::shared()->playerInfo.allianceInfo.applyNum==0){
        m_applyNode->setVisible(false);
    }
}

void AllianceInfoView::saveSuccess(){
   
}

void AllianceInfoView::onEditClick(CCObject * pSender, Control::EventType pCCControlEvent){
    auto sprite9 = CCLoadSprite::createScale9Sprite("UI_Alliance_TextBox.png");
    m_announceEditBox = CCEditBox::create(CCSizeMake(580,45),sprite9);
    //m_announceEditBox = CCEditText::create(CCSizeMake(580.0,130),"UI_Alliance_text02.png",true,false);
    m_announceEditBox->setMaxLength(200);
    m_announceEditBox->setFontSize(20);
    m_announceEditBox->setFontColor(ccBLACK);
    m_announceEditBox->setReturnType(kKeyboardReturnTypeDone);
    m_announceEditBox->setInputFlag(kEditBoxInputFlagInitialCapsSentence);
    m_announceEditBox->setText(GlobalData::shared()->playerInfo.allianceInfo.announce.c_str());
    m_announceNode->addChild(m_announceEditBox);
}

void AllianceInfoView::onArrowBtnClick(CCObject * pSender, Control::EventType pCCControlEvent){
    m_bottomNode->setVisible(false);
    m_funList->setVisible(true);
    m_btnNode->setVisible(true);                                                       
    m_tabView->getContainer()->removeAllChildrenWithCleanup(true);
    this->initFun();
}

void AllianceInfoView::onTipBtnClick(cocos2d::CCObject *pSender, CCControlEvent pCCControlEvent)
{
#if (CC_TARGET_PLATFORM != CC_PLATFORM_IOS)
    GlobalData::shared()->isBind = true;
#endif
    CCCommonUtils::showHelpShiftFAQ("45210");
}

SEL_CCControlHandler AllianceInfoView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName)
{
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onArrowBtnClick", AllianceInfoView::onArrowBtnClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onTipBtnClick", AllianceInfoView::onTipBtnClick);
    return NULL;
}

bool AllianceInfoView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_viewBg", CCScale9Sprite*, this->m_viewBg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_leaderTxt", CCLabelIFTTF*, this->m_leaderTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_powerTxt", CCLabelIF*, this->m_powerTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_membersTxt", CCLabelIF*, this->m_membersTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameTxt", CCLabelIFTTF*, this->m_nameTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_language", CCLabelIF*, this->m_language);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_EditNode", CCNode*, this->m_EditNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_announceNode", CCNode*, this->m_announceNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_allianceIcon", CCNode*, this->m_allianceIcon);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_funList", CCNode*, this->m_funList);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btnArrow", CCControlButton*, this->m_btnArrow);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bottomNode", CCNode*, this->m_bottomNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_allianceIconClickNode", CCNode*, this->m_allianceIconClickNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_eventNode", CCNode*, this->m_eventNode);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_guideNode1", CCNode*, this->m_guideNode1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_guideNode2", CCNode*, this->m_guideNode2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_guideNode3", CCNode*, this->m_guideNode3);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btnNode", CCNode*, this->m_btnNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_mailSpr", CCSprite*, this->m_mailSpr);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_inviteSpr", CCSprite*, this->m_inviteSpr);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_memberSpr", CCSprite*, this->m_memberSpr);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_managerSpr", CCSprite*, this->m_managerSpr);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_mailTxt", CCLabelIF*, this->m_mailTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_inviteTxt", CCLabelIF*, this->m_inviteTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_memberTxt", CCLabelIF*, this->m_memberTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_managerTxt", CCLabelIF*, this->m_managerTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_applyFlag", CCSprite*, this->m_applyFlag);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_applyNode", CCNode*, this->m_applyNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_applyTipNum", CCLabelIF*, this->m_applyTipNum);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_titleTxt", CCLabelIF*, this->m_titleTxt);
    return false;
}

AllianceFunCell *AllianceFunCell::create(std::string title,AllianceInfo* info,std::string icon,CCNode* clickArea,int index){
    AllianceFunCell *ret = new AllianceFunCell(title,info,icon,clickArea,index);
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool AllianceFunCell::init(){
    auto node = CCBLoadFile("AllianceFunCell", this, this);
    this->setContentSize(node->getContentSize());
    this->m_nameTxt->setString(m_titleStr.c_str());
    this->m_nameTxt->enableCOKShadow(Color4B::BLACK, Size(1,-1));    //shadow
    m_iconNode->removeAllChildrenWithCleanup(true);
    CCSprite* spr = CCLoadSprite::createSprite(m_icon.c_str());
    if (CCCommonUtils::isIosAndroidPad())
    {
        spr->setPosition(ccp(67.7, 33.1));
        spr->setScale(2.);
    }
    CCCommonUtils::setSpriteMaxSize(spr, 80);
    m_iconNode->addChild(spr);
    return true;
}

void AllianceFunCell::onEnter(){
    CCNode::onEnter();
    CCLoadSprite::doResourceByCommonIndex(7, true);
    CCLoadSprite::doResourceByCommonIndex(205, true);
    setSwallowsTouches(false);
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);
    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, Touch_Popup, false);
    if(GlobalData::shared()->playerInfo.isInAlliance() && GlobalData::shared()->playerInfo.allianceInfo.uid == m_info->uid && GlobalData::shared()->playerInfo.allianceInfo.helpcount>0 && m_index==Cell_5){
        m_helpNumNode->setVisible(true);
        m_helpNumTxt->setString(CC_ITOA(GlobalData::shared()->playerInfo.allianceInfo.helpcount));
    }else if(GlobalData::shared()->playerInfo.isInAlliance() && GlobalData::shared()->playerInfo.allianceInfo.uid == m_info->uid && GlobalData::shared()->playerInfo.allianceInfo.militaryNum>0 && m_index==Cell_1){
        m_helpNumNode->setVisible(true);
        m_helpNumTxt->setString(CC_ITOA(GlobalData::shared()->playerInfo.allianceInfo.militaryNum));
    }else{
        m_helpNumNode->setVisible(false);
    }
    //联盟任务
    if(m_index==Cell_7){
        m_timeTxt->setVisible(true);
        updateTime(1);
        this->schedule(schedule_selector(AllianceFunCell::updateTime), 1);
        int finish = AllianceDailyController::getInstance()->m_finishCount;
        if(finish>0){
            m_helpNumNode->setVisible(true);
            m_helpNumTxt->setString(CC_ITOA(finish));
        }else{
            m_helpNumNode->setVisible(false);
        }
    }else{
        m_timeTxt->setVisible(false);
    }
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(AllianceFunCell::updateNum), MSG_CHANGE_ALLIANCE_MILITARY_NUM, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(AllianceFunCell::playHelpNumAnim), AllianceDailyFinishAdd, NULL);
}

void AllianceFunCell::playHelpNumAnim(CCObject* obj){
    //播放上下动画三次
    if(m_index==Cell_7){
        m_helpNumNode->stopAllActions();
        CCPoint py = m_helpNumNode->getPosition();
        CCEaseExponentialIn* easeInAction = CCEaseExponentialIn::create(dynamic_cast<CCActionInterval*>(CCMoveTo::create(0.2, ccp(py.x, py.y+10))));
        CCFiniteTimeAction* easeInActionRevers =  CCMoveTo::create(0.2, ccp(py.x, py.y));
        CCEaseExponentialIn* easeInAction1 = CCEaseExponentialIn::create(dynamic_cast<CCActionInterval*>(CCMoveTo::create(0.2, ccp(py.x, py.y-10))));
        CCMoveTo* easeInAction1Revers = CCMoveTo::create(0.2, ccp(py.x, py.y));
        CCRepeat* repeatAction = CCRepeat::create(CCSequence::create(easeInAction, easeInActionRevers, easeInAction1,easeInAction1Revers,NULL), 3);
        m_helpNumNode->runAction(CCSequence::create(repeatAction,CCDelayTime::create(5.0),CCCallFunc::create(this, callfunc_selector(AllianceFunCell::helpNumAnimFinish)), NULL));
    }
}

void AllianceFunCell::helpNumAnimFinish(){
    m_helpNumNode->setPosition(-217.0, 15.2);
    if (CCCommonUtils::isIosAndroidPad())
    {
        m_helpNumNode->setPosition(-133.5, 72.8);
    }
}

void AllianceFunCell::updateTime(float t){
    double time = AllianceDailyController::getInstance()->refreshTime - WorldController::getInstance()->getTime();
    time/=1000;
    string timeStr = CC_SECTOA(time);
    string str = _lang_1("134049", timeStr.c_str());
    if(time<0){
        str="";
    }
    m_timeTxt->setString(str);
    int finish = AllianceDailyController::getInstance()->m_finishCount;
    if(finish>0){
        m_helpNumNode->setVisible(true);
        m_helpNumTxt->setString(CC_ITOA(finish));
    }else{
        m_helpNumNode->setVisible(false);
    }
}

void AllianceFunCell::updateNum(CCObject* param){
    if(GlobalData::shared()->playerInfo.isInAlliance() && GlobalData::shared()->playerInfo.allianceInfo.uid == m_info->uid && GlobalData::shared()->playerInfo.allianceInfo.helpcount>0 && m_index==Cell_5){
        m_helpNumNode->setVisible(true);
        m_helpNumTxt->setString(CC_ITOA(GlobalData::shared()->playerInfo.allianceInfo.helpcount));
    }else if(GlobalData::shared()->playerInfo.isInAlliance() && GlobalData::shared()->playerInfo.allianceInfo.uid == m_info->uid && GlobalData::shared()->playerInfo.allianceInfo.militaryNum>0 && m_index==Cell_1){
        m_helpNumNode->setVisible(true);
        m_helpNumTxt->setString(CC_ITOA(GlobalData::shared()->playerInfo.allianceInfo.militaryNum));
    }else{
        m_helpNumNode->setVisible(false);
    }
}

void AllianceFunCell::onExit(){
    setTouchEnabled(false);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_CHANGE_ALLIANCE_MILITARY_NUM);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, AllianceDailyFinishAdd);
    
    CCNode::onExit();
}

bool AllianceFunCell::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
    m_movePoint = pTouch->getLocation();
    if(isTouchInside(m_renderBG, pTouch) && isTouchInside(m_clickArea, pTouch)){
        return true;
    }
    return false;
}

void AllianceFunCell::onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
    if(fabs(m_movePoint.y - pTouch->getLocation().y) < 30 && isTouchInside(m_clickArea, pTouch) && isTouchInside(m_renderBG, pTouch)){
        SoundController::sharedSound()->playEffects(Music_Sfx_click_button);
         this->clickHandle(NULL,Control::EventType::TOUCH_DOWN);
    }
}

void AllianceFunCell::onTouchMoved(CCTouch *pTouch, CCEvent *pEvent){
//    m_moveFlag = true;
}

void AllianceFunCell::clickHandle(CCObject *pSender, CCControlEvent event)
{
    if (m_guideKey == "ALINFO_2") {
        return;
    }

    std::string uid = m_info->uid;
    int buildId = FunBuildController::getInstance()->getMaxLvBuildByType(FUN_BUILD_TAVERN);
    bool haveMarket = false;
    if (buildId>0) {
        FunBuildInfo& info = FunBuildController::getInstance()->getFunbuildById(buildId);
        if(&info!=NULL){
            haveMarket = true;
        }
    }
    
    int warBuildId = FunBuildController::getInstance()->getMaxLvBuildByType(FUN_BUILD_SMITHY);
    bool haveWarBuild = false;
    if (warBuildId>0) {
        FunBuildInfo& info = FunBuildController::getInstance()->getFunbuildById(warBuildId);
        if(&info!=NULL){
            haveWarBuild = true;
        }
    }
    switch (m_index) {
        case 0:
        {
            bool isApply = GlobalData::shared()->playerInfo.allianceInfo.uid !=m_info->uid;
            PopupViewController::getInstance()->addPopupInView(AllianceInfoMembersView::create(uid,isApply));
        }
            break;
        case Cell_1:
        {
            //            if(haveWarBuild){
            //
            //            }else{
            //                CCCommonUtils::flyHint("", "", _lang("115127"));
            //            }
            PopupViewController::getInstance()->addPopupInView(AllianceWarView::create());
        }
            break;
        case Cell_2:
        {
            unsigned int index = WorldController::getIndexByPoint(WorldController::getInstance()->selfPoint);
            PopupViewController::getInstance()->addPopupInView(AllianceTerritoryView::create(index,false));
        }
            break;
        case Cell_3:
            PopupViewController::getInstance()->addPopupInView(AllianceScienceView::create());
            break;
        case Cell_4:
            //  CCCommonUtils::flyHint("", "", _lang("E100008"));
            PopupViewController::getInstance()->addPopupInView(AllianceShopView::create());
            break;
        case Cell_5:
            PopupViewController::getInstance()->addPopupInView(AllianceHelpView::create());
            break;
        case Cell_6:
            PopupViewController::getInstance()->addPopupInView(ChatView::create(CHAT_COMMENT,m_info->uid),false);
            break;
        case Cell_7:
            if (FunBuildController::getInstance()->getMainCityLv() >= 10) {
                PopupViewController::getInstance()->addPopupInView(AllianceDailyPublishView::create());
            }else{
                CCCommonUtils::flyHint("", "", _lang_1("134068", CC_ITOA(10)));
            }
//            PopupViewController::getInstance()->addPopupInView(AllianceDailyPublishView::create());
            break;
    }
}

bool AllianceFunCell::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode){
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameTxt", CCLabelIF*, this->m_nameTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_iconNode", CCNode*, this->m_iconNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_renderBG", CCNode*, this->m_renderBG);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_helpNumNode", CCNode*, this->m_helpNumNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_helpNumTxt", CCLabelIF*, this->m_helpNumTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_timeTxt", CCLabelIF*, this->m_timeTxt);
    return false;
}

SEL_CCControlHandler AllianceFunCell::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){
    //CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "clickHandle", AllianceFunCell::clickHandle);
    return NULL;
}

//
AllianceEventCell *AllianceEventCell::create(AllianceEventInfo* info,AllianceEventInfo* lastInfo){
    AllianceEventCell *ret = new AllianceEventCell(info,lastInfo);
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool AllianceEventCell::init(){
    CCLoadSprite::doResourceByCommonIndex(7, true);
    auto node = CCBLoadFile("AllianceEventCell", this, this);
    this->setContentSize(node->getContentSize());
    this->setData(m_info,m_lastInfo);
    return true;
}

void AllianceEventCell::onEnter(){
    CCNode::onEnter();
}

void AllianceEventCell::onExit(){
    CCNode::onExit();
}

void AllianceEventCell::setData(AllianceEventInfo* info,AllianceEventInfo* lastInfo){
    CCLoadSprite::doResourceByCommonIndex(7, true);
    m_info = info;
    m_lastInfo = lastInfo;
    if(m_info->getContent()=="" && m_info->type==5){
        m_contentTxt->setString(_lang("115087").c_str());
    }else{
       m_contentTxt->setString(m_info->getContent().c_str());
    }
    m_icon->removeAllChildrenWithCleanup(true);
    string icon = "";
    if(m_info->type<15 && m_info->type>=0){
        icon = iconEvents[m_info->type];
    }
    CCSprite* typeIcon = CCLoadSprite::createSprite(icon.c_str());
    m_icon->addChild(typeIcon);
//    if (CCCommonUtils::isIosAndroidPad())
//    {
//        typeIcon->setScaleX(1.277);
//        typeIcon->setScaleY(1.198);
////        typeIcon->setScale(50.0*2/typeIcon->getContentSize().width);
//        typeIcon->setPosition(ccp(-88.0, 2.0));
//    }
//    else
    {
        typeIcon->setScale(50.0/typeIcon->getContentSize().width);
        typeIcon->setPositionY(-8);
    }
    if(m_info->type==0||m_info->type==1){
//        m_arrow->initWithSpriteFrame(CCLoadSprite::loadResource("Alliance_info_Red_02.png"));
//        m_cellBg->initWithSpriteFrame(CCLoadSprite::loadResource("Alliance_info_Red.png"));
        //m_timeTxt->setColor({161,90,87});
        m_contentTxt->setColor({122,60,57});
    }else if(m_info->type==2){
//        m_arrow->initWithSpriteFrame(CCLoadSprite::loadResource("Alliance_info_blue_01.png"));
//        m_cellBg->initWithSpriteFrame(CCLoadSprite::loadResource("Alliance_info_blue.png"));
        //m_timeTxt->setColor({80,143,159});
        m_contentTxt->setColor({29,101,131});
    }else{
//        m_arrow->initWithSpriteFrame(CCLoadSprite::loadResource("Alliance_info_green_01.png"));
//        m_cellBg->initWithSpriteFrame(CCLoadSprite::loadResource("Alliance_info_green.png"));
        m_timeTxt->setColor({141,166,80});
        m_contentTxt->setColor({90,119,57});
    }
    m_timeTxt->setColor({143,143,143});
    m_node2->setVisible(true);
    if(m_lastInfo==NULL){
        
    }else{
        time_t lt = m_lastInfo->createTime;
        if ((lt - m_info->createTime) < 60*10) {
            m_node2->setVisible(false);
        }
    }
    time_t infoTime = GlobalData::shared()->getTimeStamp();
    string lastTime = CCCommonUtils::timeStampToDHM(infoTime).substr(0,10);
    string nowTime = CCCommonUtils::timeStampToDHM(m_info->createTime).substr(0,10);
    if (lastTime == nowTime) {
        m_timeTxt->setString(CCCommonUtils::timeStampToHour(m_info->createTime).c_str());
    }
    else {
        m_timeTxt->setString(CCCommonUtils::timeStampToDHM(m_info->createTime).c_str());
    }
    int msgHeight = m_contentTxt->getContentSize().height*m_contentTxt->getOriginScaleY() + 6;
    msgHeight = MAX(msgHeight, 56);
    if (CCCommonUtils::isIosAndroidPad()) {
        msgHeight = MAX(msgHeight, 112);
    }
//    if (CCCommonUtils::isIosAndroidPad())
//    {
//        msgHeight *= 1.2;
//    }
    m_cellBg->setContentSize(CCSizeMake(534.0, msgHeight));
    m_cellBg->setAnchorPoint(ccp(0.5,0));
    m_moveNode->setPositionY(msgHeight-52);
    m_node2->setPositionY(msgHeight -10);
    m_clickBg->setContentSize(CCSizeMake(534.0, msgHeight));
    m_clickBg->setAnchorPoint(ccp(0.5,0));
    if (CCCommonUtils::isIosAndroidPad())
    {
        m_cellBg->setContentSize(CCSizeMake(1370, msgHeight));
        m_cellBg->setAnchorPoint(ccp(0.5,0));
        m_moveNode->setPositionY(msgHeight-104);
        m_node2->setPositionY(msgHeight -20);
        m_clickBg->setContentSize(CCSizeMake(1530, msgHeight));
        m_clickBg->setAnchorPoint(ccp(0.5,0));
    }
}

void AllianceEventCell::showCell(AllianceEventInfo* info){
    m_info = info;
    if(m_info->getContent()=="" && m_info->type==5){
        m_contentTxt->setString(_lang("115087").c_str());
    }else{
        string temp = m_info->getContent();
        int totalLen = Utf8Utils::strlen(temp.c_str());
        if(totalLen>50){
            temp = CCCommonUtils::subStrByUtf8(temp,0,50);
            temp.append("...");
        }
        m_contentTxt->setString(temp.c_str());
    }
    m_icon->removeAllChildrenWithCleanup(true);
    string icon = "";
    if(m_info->type<15 && m_info->type>=0){
        icon = iconEvents[m_info->type];
    }
    CCSprite* typeIcon = CCLoadSprite::createSprite(icon.c_str());
//    if (CCCommonUtils::isIosAndroidPad())
//    {
//        typeIcon->setScale(50.0*1.8/typeIcon->getContentSize().width);
//        typeIcon->setPosition(ccp(-88.0, 2.0));
//    }
//    else
    {
        typeIcon->setScale(50.0/typeIcon->getContentSize().width);
        typeIcon->setPositionY(-8);
    }
    
    m_icon->addChild(typeIcon);
    
    if(m_info->type==0||m_info->type==1){
        m_arrow->initWithSpriteFrame(CCLoadSprite::loadResource("Alliance_info_Red_02.png"));
        m_cellBg->initWithSpriteFrame(CCLoadSprite::loadResource("Alliance_info_Red.png"));
        //m_timeTxt->setColor({161,90,87});
        m_contentTxt->setColor({122,60,57});
    }else if(m_info->type==2){
        m_arrow->initWithSpriteFrame(CCLoadSprite::loadResource("Alliance_info_blue_01.png"));
        m_cellBg->initWithSpriteFrame(CCLoadSprite::loadResource("Alliance_info_blue.png"));
        //m_timeTxt->setColor({80,143,159});
        m_contentTxt->setColor({29,101,131});
    }else{
        m_arrow->initWithSpriteFrame(CCLoadSprite::loadResource("Alliance_info_green_01.png"));
        m_cellBg->initWithSpriteFrame(CCLoadSprite::loadResource("Alliance_info_green.png"));
        m_timeTxt->setColor({141,166,80});
        m_contentTxt->setColor({90,119,57});
    }
    m_timeTxt->setColor({143,143,143});
    m_node2->setVisible(true);
    if(m_lastInfo==NULL){
        
    }else{
        time_t lt = m_lastInfo->createTime;
        if ((m_info->createTime-lt) < 60*10) {
            m_node2->setVisible(false);
        }
    }
    time_t infoTime = GlobalData::shared()->getTimeStamp();
    string lastTime = CCCommonUtils::timeStampToDHM(infoTime).substr(0,10);
    string nowTime = CCCommonUtils::timeStampToDHM(m_info->createTime).substr(0,10);
    if (lastTime == nowTime) {
        m_timeTxt->setString(CCCommonUtils::timeStampToHour(m_info->createTime).c_str());
    }
    else {
        m_timeTxt->setString(CCCommonUtils::timeStampToDHM(m_info->createTime).c_str());
    }
    int msgHeight = m_contentTxt->getContentSize().height*m_contentTxt->getOriginScaleY() + 6;
    msgHeight = MAX(msgHeight, 56);
    if (CCCommonUtils::isIosAndroidPad()) {
        msgHeight = MAX(msgHeight, 112);
    }
//    if (CCCommonUtils::isIosAndroidPad())
//    {
//        msgHeight *= 1.2;
//    }
    m_cellBg->setContentSize(CCSizeMake(534.0, msgHeight));
    m_cellBg->setAnchorPoint(ccp(0.5,0));
    m_moveNode->setPositionY(msgHeight-52);
    m_node2->setPositionY(msgHeight -10);
    m_clickBg->setContentSize(CCSizeMake(534.0, msgHeight));
    m_clickBg->setAnchorPoint(ccp(0.5,0));
    if (CCCommonUtils::isIosAndroidPad())
    {
        m_cellBg->setContentSize(CCSizeMake(1370, msgHeight));
        m_cellBg->setAnchorPoint(ccp(0.5,0));
        m_moveNode->setPositionY(msgHeight-104);
        m_node2->setPositionY(msgHeight -20);
        m_clickBg->setContentSize(CCSizeMake(1530, msgHeight));
        m_clickBg->setAnchorPoint(ccp(0.5,0));
    }
}

int AllianceEventCell::getMaxHeight(){
    if(m_node2->isVisible()){
        if (CCCommonUtils::isIosAndroidPad())
        {
            return 166 + 8;
        }
        return m_cellBg->getContentSize().height + 8 + 32;
    }
    if (CCCommonUtils::isIosAndroidPad()) {
        return 112 + 8;
    }
    return m_cellBg->getContentSize().height+8;
}

bool AllianceEventCell::checkIsEventInSide(cocos2d::CCTouch *pTouch){
    bool flag = false;
    if(isTouchInside(m_clickBg, pTouch)){
        flag = true;
    }
    return flag;
}

bool AllianceEventCell::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode){
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_moveNode", CCNode*, this->m_moveNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_icon", CCNode*, this->m_icon);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_cellBg", CCScale9Sprite*, this->m_cellBg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_clickBg", CCScale9Sprite*, this->m_clickBg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_timeTxt", CCLabelIF*, this->m_timeTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_contentTxt", CCLabelIFTTF*, this->m_contentTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_arrow", CCSprite*, this->m_arrow);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_node1", CCNode*, this->m_node1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_node2", CCNode*, this->m_node2);
    return false;
}

SEL_CCControlHandler AllianceEventCell::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){
    return NULL;
}
