//
//  BattleView.cpp
//  IF
//
//  Created by fubin on 14-3-11.
//
//

#include "BattleView.h"
#include "PopupViewController.h"
#include "CCCommonUtils.h"
#include "YesNoDialog.h"
#include "ToolController.h"
#include "UseCDToolView.h"
#include "ArmyInfo.h"
#include "LocalController.h"
#include "FunBuildController.h"
#include "TroopsController.h"
#include "GeneralManager.h"
#include "GeneralInfo.h"
#include "WorldMapView.h"
#include "GeneralManager.h"
#include "UIComponent.h"
#include "CCGraySprite.h"
#include "TipsView.h"
#include "WorldCommand.h"
#include "SceneController.h"
#include "AllianceWarDetailView.h"
#include "AllianceTeamInfo.h"
#include "UseItemStatusView.h"
#include "ItemStatusView.h"
#include "ArmyController.h"
#include "UseResToolView.h"
#include "WorldMarchCheck.h"
#include "GameController.h"
#include "SoundController.h"
#include "fireandcomman.h"
#include "BuildUpgradeView.h"
#include "ChatController.h"
#include "CountryChatCommand.h"
#include "SoldierIconCell.hpp"
const float numPerRow = 1.0;
BattleView* BattleView::create(unsigned int startIndex ,unsigned int targetIndex,unsigned int haveOwner,float slow,int rally,int bType,int wtIndex,std::string other, int targetType){
    BattleView* ret = new BattleView();
    if(ret && ret->init(startIndex,targetIndex,haveOwner,slow,rally,bType,wtIndex,other,targetType)){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}
BattleView::~BattleView()
{
    WorldController::getInstance()->alertProectFlag = false;
}

void BattleView::loadResource(){
    CCLoadSprite::doResourceByCommonIndex(8, true,true);
    CCLoadSprite::doResourceByCommonIndex(7, true,true);
    CCLoadSprite::doResourceByCommonIndex(504, true,true);
    CCLoadSprite::doResourceByCommonIndex(105, true,true);
    CCLoadSprite::doResourceByCommonIndex(204, true,true);
}


bool BattleView::init(unsigned int startIndex,unsigned int targetIndex,unsigned int haveOwner,float slow,int rally,int bType,int wtIndex,std::string other, int targetType)
{
    if (!PopupBaseView::init()) {
        return false;
    }
    loadResource();
    setCleanFunction([](){
        CCLoadSprite::doResourceByCommonIndex(8, false,true);
        CCLoadSprite::doResourceByCommonIndex(7, false,true);
        CCLoadSprite::doResourceByCommonIndex(504, false,true);
        CCLoadSprite::doResourceByCommonIndex(105, false,true);
        CCLoadSprite::doResourceByCommonIndex(204, false,true);
    });
    setIsHDPanel(true);
    m_slow=slow;
    TroopsController::getInstance()->resetGeneralToSend();
    m_startIndex = startIndex;
    m_targetIndex = targetIndex;
    m_haveOwner = haveOwner;
    m_rally = rally;
    m_bType = bType;
    m_wtIndex = wtIndex;
    m_other = other;
    m_targetType = targetType;
    auto tmpCCB = CCBLoadFile("March02",this,this);
    this->setContentSize(tmpCCB->getContentSize());
    if(m_pic_di)
    {
        CCCommonUtils::addNewDizi(m_pic_di);
    }
    int dy = 852 - CCDirector::sharedDirector()->getWinSize().height;
    if (CCCommonUtils::isIosAndroidPad())
    {
        dy = 2048 - CCDirector::sharedDirector()->getWinSize().height;
    }
    m_bg->setPreferredSize(m_bg->getContentSize() + CCSize(0, -dy));
    m_renderBG->setPreferredSize(m_renderBG->getContentSize() + CCSize(0, -dy));
    m_renderBG->setPositionY(m_renderBG->getPositionY() + dy / 2);
    m_downNode->setPositionY(m_downNode->getPositionY() + dy);
    m_infoList->setPositionY(m_infoList->getPositionY() + dy);
    m_infoList->setContentSize(m_infoList->getContentSize() - CCSize(0, dy));
    m_tabView = CCMultiColTableView::create(this, m_infoList->getContentSize());
    
    
    m_tabView->setBounceable(false);
    m_tabView->setDirection(kCCScrollViewDirectionVertical);
    m_tabView->setVerticalFillOrder(kCCTableViewFillTopDown);
    m_tabView->setMultiColTableViewDelegate(this);
//    m_tabView->setTouchPriority(Touch_Popup);
    m_infoList->addChild(m_tabView);
    
//    m_box = CheckBox::create();
//    this->m_checkBoxContainer->addChild(m_box);
//    auto &generalInfo = GlobalData::shared()->generals.begin()->second;
//    if(generalInfo.state == COUNTRY_BATTLE){
//        m_box->setVisible(false);
//    }
    TroopsController::getInstance()->m_curSelHeroNum = 0;
    TroopsController::getInstance()->m_curLoadNum = 0;
    CCCommonUtils::setButtonTitle(m_quickBtn, _lang("105149").c_str());
    
    //判断是否有编队功能按钮:15本两个,SVIP根据等级取值
    int formationCount = 0;
    if (FunBuildController::getInstance()->getMainCityLv() >= 15) {
        formationCount += 2;
    }
    auto &playerInfo = GlobalData::shared()->playerInfo;
    int svipLv = playerInfo.SVIPLevel;
    if(VipUtil::isSVIP() && svipLv > 0 && playerInfo.vipEndTime > GlobalData::shared()->getWorldTime()) {
        int itemID = 7010+svipLv-1;
        int value = atoi(CCCommonUtils::getPropById(CC_ITOA(itemID), "troop").c_str());
        if (value > 0) {
            formationCount += value;
        }
    }
    vector<CCSafeObject<Node>> formationVec;
    formationVec.clear();
    formationVec.push_back(m_formation1);
    formationVec.push_back(m_formation2);
    formationVec.push_back(m_formation3);
    formationVec.push_back(m_formation4);
    formationVec.push_back(m_formation5);
    if (formationCount > 0 && GlobalData::shared()->march_formation_switch == 2) {
        m_formationNode->setVisible(true);
        m_formation1->setVisible(false);
        m_formation2->setVisible(false);
        m_formation3->setVisible(false);
        m_formation4->setVisible(false);
        m_formation5->setVisible(false);
        for(int i = 0; i < formationCount; i++) {
            formationVec[i]->setVisible(true);
            formationVec[i]->setPositionX((i - formationCount + 1) * 60);
        }
    }
    else {
        m_formationNode->setVisible(false);
    }
    m_timeText->setString(_lang("108536"));
    m_msg1Label->setString("0/");
    m_msg3Label->setString(CC_ITOA(TroopsController::getInstance()->getMaxSoilder(m_bType)));
    m_msg2Label->setString(CC_ITOA(TroopsController::getInstance()->m_curLoadNum));
    m_timeLabel->setString(CC_SECTOA(0));
    _modelLayer->setOpacity(200);
//    m_hintText1->setString(_lang("105147"));
    m_tmpArray = CCArray::create();
    generateData();
    if(m_tmpArray->count() == 0){
        m_hintText->setString(_lang("102151").c_str());
    }else{
        m_hintText->setString("");
    }
    switch (m_bType) {
        case MethodRally:
            CCCommonUtils::setButtonTitle(m_marchBtn, "");
            setTitleName(_lang("108582"));
            m_timeText->setString(_lang("108582"));
            break;
        case MethodUnion:
            CCCommonUtils::setButtonTitle(m_marchBtn, "");
            setTitleName(_lang("108582"));
            m_timeText->setString(_lang("108582"));
            break;
        case MethodYuanSolider:
            CCCommonUtils::setButtonTitle(m_marchBtn, "");
            setTitleName(_lang("115176"));
            m_timeText->setString(_lang("115176"));
            break;
    }
    m_marchBtn->setEffectStr("");
    updateInfo();
    this->scheduleOnce(schedule_selector(BattleView::loadResource), 0.02);
    updateLoadInfo(NULL);
    auto info = WorldController::getInstance()->m_cityInfo.find(m_targetIndex);
    int type = -1;
    if(info != WorldController::getInstance()->m_cityInfo.end()){
        type = info->second.cityType;
    }
    if(m_targetType==ActBossTile || (type == FieldMonster || type == MonsterTile || type == MonsterRange || type == ActBossTile)){
        m_iconContainer->removeAllChildren();
        auto sprite = CCLoadSprite::createSprite("Ui_tili.png");
        sprite->setScale(0.65);
        m_iconContainer->addChild(sprite);
//        int currt = WorldController::getInstance()->currentStamine;
//        int total = GlobalData::shared()->worldConfig.stamineMax;
        string str = _lang("3100112");
        
        if (m_targetType==ActBossTile || type == ActBossTile) {
            str += CC_ITOA(GlobalData::shared()->worldConfig.boss_decr);
        }else {
            int usePower = WorldController::getInstance()->getMonsterUsePower(m_targetIndex);
            if (usePower == 0) {
                usePower = GlobalData::shared()->worldConfig.stamineCostPerTime;
            }
            str += CC_ITOA(usePower);
        }
//        str += "   ";
//        str += std::string("") + CC_ITOA(currt) + "/" + CC_ITOA(total);
        m_msg2Label->setString(str);
    }else if(info != WorldController::getInstance()->m_cityInfo.end() && (info->second.cityType == Resource_new || info->second.cityType == ResourceTile)){
        int tempType = 0;
        if (info->second.cityType == Resource_new) {
            tempType = Gold;
        }
        else if (info->second.cityType == ResourceTile){
            tempType = info->second.resource.type;
        }
        m_iconContainer->removeAllChildren();
        auto sprite = CCLoadSprite::createSprite(CCCommonUtils::getResourceIconByType(tempType).c_str());
        m_iconContainer->addChild(sprite);
    }
//    else if(info != WorldController::getInstance()->m_cityInfo.end()&&(info->second.cityType == OriginTile || info->second.cityType == CityTile || info->second.cityType == Tile_allianceArea||info->second.cityType == tile_tower)){
//        m_iconContainer->setVisible(false);
//        m_msg2Label->setVisible(false);
//        m_helpBtn->setVisible(false);
//    }
    onClickQuickBtn(NULL, Control::EventType::TOUCH_DOWN);
    makeArrTime(nullptr);
    SoundController::sharedSound()->playEffects(Music_Sfx_world_click_attack);
    return true;
}

void BattleView::loadResource(float _time){
    
    updateInfo();
}

void BattleView::setAddBtnState(){
    if(CCCommonUtils::getStateEffectValueByNum(COMMAND_EFFECT) > FLT_EPSILON){
        this->m_addBtn->setVisible(false);
        this->m_addIcon->setVisible(false);
        this->m_addIcon1->setVisible(false);
       // this->m_msg3Label->setColor(ccc3(86, 180, 29));
    }else{
        this->m_addBtn->setVisible(true);
        this->m_addIcon->setVisible(true);
        this->m_addIcon1->setVisible(true);
        //this->m_msg3Label->setColor(ccc3(255, 247, 255));
        this->getAnimationManager()->runAnimationsForSequenceNamed("Default Timeline");
    }
}

void BattleView::generateData(){
    TroopsController::getInstance()->m_tmpFreeSoldiers.clear();
    TroopsController::getInstance()->m_tmpBattleInfos.clear();
    TroopsController::getInstance()->m_tmpConfSoldiers.clear();
    m_tmpArray->removeAllObjects();
    std::map<string, ArmyInfo>::iterator it;
    for (it = GlobalData::shared()->armyList.begin(); it!=GlobalData::shared()->armyList.end(); it++) {
        if(it->second.free <= 0 || !it->second.isArmy){
            continue;
        }
        TroopsController::getInstance()->m_tmpFreeSoldiers[it->first] = (it->second).free;
        TroopsController::getInstance()->m_tmpConfSoldiers[it->first] = (it->second).free;
        int index = 0;
        bool addFlag = false;
        while (index < m_tmpArray->count()) {
            std::string armyId = dynamic_cast<CCString*>(m_tmpArray->objectAtIndex(index))->getCString();
            auto &army = GlobalData::shared()->armyList[armyId];
            if(army.armyLevel < it->second.armyLevel){
                m_tmpArray->insertObject(CCString::create(it->first), index);
                addFlag = true;
                break;
            }
            index++;
        }
        if(!addFlag){
            m_tmpArray->addObject(CCString::create(it->first));
        }
    }
}

void BattleView::updateInfo()
{
    m_tabView->reloadData();
}

void BattleView::onEnter()
{
    CCNode::onEnter();
    setAddBtnState();
    updateLoadInfo(NULL);
    switch (m_bType) {
        case MethodRally:
            setTitleName(_lang("108582"));
            break;
        case MethodUnion:
            setTitleName(_lang("108582"));
            break;
        case MethodYuanSolider:
            setTitleName(_lang("115176"));
            break;
            default:
            setTitleName(_lang("105150"));
    }
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(BattleView::updateArmyNumber), MSG_TROOPS_BACK, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(BattleView::updateLoadInfo), MSG_TROOPS_BATTLE_LOAD, NULL);
    CCSafeNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(BattleView::makeArrTime), MSG_TROOPS_TIME, NULL);
}

void BattleView::onExit()
{
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_TROOPS_BATTLE_LOAD);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_TROOPS_TIME);
    CCSafeNotificationCenter::sharedNotificationCenter()->removeObserver(this,MSG_TROOPS_BACK);
    CCNode::onExit();
}

void BattleView::generalSelect(){
//    std::string generalUid = GlobalData::shared()->generals.begin()->first;
//    TroopsController::getInstance()->m_generalToSend[0] = (m_box->isSelected() ? generalUid : "");
}

void BattleView::updateLoadInfo(CCObject* obj)
{
    int loadNum = TroopsController::getInstance()->m_curLoadNum;
    auto info = WorldController::getInstance()->m_cityInfo.find(m_targetIndex);
    int type = -1;
    if(info != WorldController::getInstance()->m_cityInfo.end()){
        int a = info->second.playerInfo.type;
        type = info->second.cityType;
    }
    if(info != WorldController::getInstance()->m_cityInfo.end() && (type == ResourceTile || type == tile_superMine || type == Resource_new)){
        if(type == ResourceTile){
            loadNum = loadNum / CCCommonUtils::getResourceLoadByType(info->second.resource.type);
        }else if (type == tile_superMine){
            loadNum = loadNum / CCCommonUtils::getResourceLoadByType(info->second.m_superMineInfo.type);
        }else if (type == Resource_new){
            loadNum = loadNum / CCCommonUtils::getResourceLoadByType(info->second.m_newResourceInfo.type);
        }
        
        if(loadNum == 0){
            m_msg2Label->setColor(ccRED);
            auto fadeOut = CCFadeTo::create(0.6f, 127);
            auto fadeIn = CCFadeTo::create(0.6f, 255);
            auto delay = CCDelayTime::create(2.0f);
            CCSequence *action = CCSequence::create(fadeOut, fadeIn, delay, NULL);
            m_msg2Label->runAction(CCRepeatForever::create(action));
        }else{
            m_msg2Label->stopAllActions();
            m_msg2Label->setOpacity(255);
            m_msg2Label->setColor(ccc3(255, 225, 0));
        }
    }
    m_helpBtn->setVisible(true);
    m_stamineNode->setVisible(false);
    if(m_targetType==ActBossTile || (type == FieldMonster || type == MonsterTile || type == MonsterRange || type == ActBossTile)){
        m_iconContainer->removeAllChildren();
        auto sprite = CCLoadSprite::createSprite("Ui_tili.png");
        sprite->setScale(0.65);
        m_iconContainer->addChild(sprite);
//        int currt = WorldController::getInstance()->currentStamine;
//        int total = GlobalData::shared()->worldConfig.stamineMax;
        string str = _lang("3100112");
        if (m_targetType==ActBossTile || type == ActBossTile) {
            str += CC_ITOA(GlobalData::shared()->worldConfig.boss_decr);
        }else {
            int usePower = WorldController::getInstance()->getMonsterUsePower(m_targetIndex);
            if (usePower == 0) {
                usePower = GlobalData::shared()->worldConfig.stamineCostPerTime;
            }
            str += CC_ITOA(usePower);
        }
//        str += "   ";
//        str += std::string("") + CC_ITOA(currt) + "/" + CC_ITOA(total);
        m_msg2Label->setString(str);
        m_helpBtn->setVisible(false);
        m_stamineNode->setVisible(true);
    }else{
        string loadInfo = CC_CMDITOA(loadNum);
        m_msg2Label->setString(loadInfo.c_str());
    }

    int playertype = PlayerNone;
    bool isHomePlace = false;
    if (info != WorldController::getInstance()->m_cityInfo.end()) {
        playertype = info->second.playerInfo.type;
        isHomePlace = info->second.m_isMyAArea;
    }
    bool isAllianceCity = (type == CityTile && playertype != PlayerOther);
    bool isOriginTile = (type == OriginTile);
    bool isHomeAllianceArea = (type == Tile_allianceArea);
    bool isHomeTower = (type == tile_tower&& isHomePlace);
    bool isThroneFight = (type == Throne);
    if (isAllianceCity || isOriginTile || isHomeAllianceArea || isHomeTower || isThroneFight) {
        m_iconContainer->setVisible(false);
        m_msg2Label->setVisible(false);
        m_msg2Dizi->setVisible(false);
        m_helpBtn->setVisible(false);
    }
    
    if(info != WorldController::getInstance()->m_cityInfo.end() && info->second.cityType == tile_superMine  && info->second.cityType == Resource_new ){
        int tempType = -1;
        if (info->second.cityType == tile_superMine) {
            tempType = info->second.m_superMineInfo.type;
        }
        else if (info->second.cityType == Resource_new){
            tempType = info->second.m_newResourceInfo.type;
        }
        if (tempType != -1) {
            m_iconContainer->removeAllChildren();
            auto sprite = CCLoadSprite::createSprite(CCCommonUtils::getResourceIconByType(tempType).c_str());
            m_iconContainer->addChild(sprite);
        }
    }
    
    int total = 0;
    map<string, int>::iterator it;
    for (it = TroopsController::getInstance()->m_tmpBattleInfos.begin(); it != TroopsController::getInstance()->m_tmpBattleInfos.end(); it++) {
        if (TroopsController::getInstance()->m_tmpBattleInfos[it->first] > 0) {
            total += TroopsController::getInstance()->m_tmpBattleInfos[it->first];
        }
    }
    m_msg1Label->setString((std::string("") + CC_ITOA(total) + "/").c_str());
    int maxSoilder = TroopsController::getInstance()->getMaxSoilder(m_bType);
    if (m_bType == MethodUnion || m_bType == MethodYuanSolider)
    {
        maxSoilder = MIN(m_rally, maxSoilder);
    }
    m_msg3Label->setString(CC_ITOA(maxSoilder));
}

SEL_CCControlHandler BattleView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName)
{
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickQuickBtn", BattleView::onClickQuickBtn);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClickMarchBtn", BattleView::onClickMarchBtn);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onHelpClick", BattleView::onHelpClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onAddClick", BattleView::onAddClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onAddStamineClick", BattleView::onAddStamineClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onFormation1Click", BattleView::onFormation1Click);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onFormation2Click", BattleView::onFormation2Click);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onFormation3Click", BattleView::onFormation3Click);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onFormation4Click", BattleView::onFormation4Click);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onFormation5Click", BattleView::onFormation5Click);
    return NULL;
}

bool BattleView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_pic_di", CCSprite*, m_pic_di);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_marchBtn", CCControlButton*, this->m_marchBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_quickBtn", CCControlButton*, this->m_quickBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_msg1Label", CCLabelIF*, this->m_msg1Label);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_msg3Label", CCLabelIF*, this->m_msg3Label);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_msg2Label", CCLabelIF*, this->m_msg2Label);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_msg2Dizi", CCScale9Sprite*, this->m_msg2Dizi);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_timeLabel", CCLabelIF*, this->m_timeLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_timeText", CCLabelIF*, this->m_timeText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_infoList", CCNode*, this->m_infoList);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_downNode", CCNode*, this->m_downNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_iconContainer", CCNode*, this->m_iconContainer);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_alertNode", CCNode*, this->m_alertNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_checkBoxContainer", CCNode*, this->m_checkBoxContainer);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_bg", CCScale9Sprite*, this->m_bg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_renderBG", CCScale9Sprite*, this->m_renderBG);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_hintText", CCLabelIF*, m_hintText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_hintText1", CCLabelIF*, m_hintText1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_addBtn", CCControlButton*, m_addBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_addIcon", CCSprite*, m_addIcon);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_addIcon1", CCSprite*, m_addIcon1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_helpBtn", CCControlButton*, m_helpBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_stamineNode", CCNode*, this->m_stamineNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_formationNode", Node*, this->m_formationNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_formation5", Node*, this->m_formation5);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_formation4", Node*, this->m_formation4);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_formation3", Node*, this->m_formation3);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_formation2", Node*, this->m_formation2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_formation1", Node*, this->m_formation1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_formationBtn5", CCControlButton*, m_formationBtn5);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_formationBtn4", CCControlButton*, m_formationBtn4);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_formationBtn3", CCControlButton*, m_formationBtn3);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_formationBtn2", CCControlButton*, m_formationBtn2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_formationBtn1", CCControlButton*, m_formationBtn1);
    return false;
}

void BattleView::onFormation1Click(CCObject *pSender, CCControlEvent event){
    clickFormation(1);
}

void BattleView::onFormation2Click(CCObject *pSender, CCControlEvent event){
    clickFormation(2);
}

void BattleView::onFormation3Click(CCObject *pSender, CCControlEvent event){
    clickFormation(3);
}

void BattleView::onFormation4Click(CCObject *pSender, CCControlEvent event){
    clickFormation(4);
}

void BattleView::onFormation5Click(CCObject *pSender, CCControlEvent event){
    clickFormation(5);
}

void BattleView::clickFormation(int index)
{
    m_index = index;
    std::string formationStr = GlobalData::shared()->playerInfo.uid + "formation";
    formationStr.append(CC_ITOA(m_index));
    
    bool flag = CCUserDefault::sharedUserDefault()->getBoolForKey(formationStr.c_str(),false);
    bool hintFlag = false;
    if (flag == true) {
        //先判断总上限是否超出
        std::string forNum = GlobalData::shared()->playerInfo.uid + "forNum";
        forNum.append(CC_ITOA(m_index));
        int numTotal = CCUserDefault::sharedUserDefault()->getIntegerForKey(forNum.c_str(), 0);
        int maxForceNum = TroopsController::getInstance()->getMaxSoilder(m_bType);
        if (m_bType == MethodUnion || m_bType == MethodYuanSolider)
        {
            maxForceNum = MIN(m_rally, maxForceNum);
        }
        if (numTotal > maxForceNum) {
            if (m_bType == MethodUnion || m_bType == MethodYuanSolider) {
                CCCommonUtils::flyHint("", "", _lang("103693"), 3, 0, true);
                return;
            }
            else {
                unselectAll();
                std::string forMethod = GlobalData::shared()->playerInfo.uid + "forMethod";
                forMethod.append(CC_ITOA(m_index));
                CCUserDefault::sharedUserDefault()->setStringForKey(forMethod.c_str(), "");
                CCUserDefault::sharedUserDefault()->setIntegerForKey(forNum.c_str(), 0);
                CCUserDefault::sharedUserDefault()->flush();
                CCCommonUtils::flyHint("", "", _lang("103684"), 3, 0, true);
                updateInfo();
                return;
            }
        }
        //判断单支兵种数量是否超出，超出就用现有部分
        unselectAll();
        std::string forMethod = GlobalData::shared()->playerInfo.uid + "forMethod";
        forMethod.append(CC_ITOA(m_index));
        std::string method = CCUserDefault::sharedUserDefault()->getStringForKey(forMethod.c_str(), "");
        vector<std::string> vector1;
        CCCommonUtils::splitString(method, "|", vector1);
        int num = vector1.size();
        for (int i = 0; i < num; i++) {
            vector<std::string> vector2;
            CCCommonUtils::splitString(vector1[i], ",", vector2);
            if (vector2.size() == 2) {
                int selNum = atoi(vector2[1].c_str());
                int totalNum = TroopsController::getInstance()->m_tmpFreeSoldiers[vector2[0]] + TroopsController::getInstance()->m_tmpBattleInfos[vector2[0]];
                if (selNum > totalNum) {
                    hintFlag = true;
                    TroopsController::getInstance()->updateTmpBattleData(vector2[0], totalNum, vector2[0]);
                }
                else
                    TroopsController::getInstance()->updateTmpBattleData(vector2[0], selNum, vector2[0]);
            }
            else {
                CCCommonUtils::flyText(_lang("103686"));
                return;
            }
        }
        updateInfo();
        if (num == 0) {
            CCCommonUtils::flyText(_lang("103685"));
        }
        if (hintFlag) {
            CCCommonUtils::flyText(_lang("103697"));
        }
        //        else {
        //            unselectAll();
        //            for (int i = 0; i < num; i++) {
        //                vector<std::string> vector2;
        //                CCCommonUtils::splitString(vector1[i], ",", vector2);
        //                if (vector2.size() == 2) {
        //                    TroopsController::getInstance()->updateTmpBattleData(vector2[0], atoi(vector2[1].c_str()), vector2[0]);
        //                }
        //                else {
        //                    CCCommonUtils::flyText(_lang("103686"));
        //                    return;
        //                }
        //            }
        //            updateInfo();
        //        }
    }
    else {
        CCCommonUtils::flyText(_lang("103685"));
    }
}

void BattleView::onHelpClick(CCObject * pSender, Control::EventType pCCControlEvent){
    PopupViewController::getInstance()->addPopupView(TipsView::create(_lang("102194")));
}

void BattleView::onClickMarchBtn(CCObject * pSender, Control::EventType pCCControlEvent)
{
    bool haveSoldier = false;
    for (auto &troop:TroopsController::getInstance()->m_tmpBattleInfos) {
        if (troop.second > 0) {
            haveSoldier = true;
        }
    }
    

    auto info = WorldController::getInstance()->m_cityInfo.find(m_targetIndex);
    
    if (TroopsController::getInstance()->isHaveDefHeroInBattle() && TroopsController::getInstance()->m_isNotice) {
        YesNoDialog::show( _lang("102193").c_str() ,
                          CCCallFunc::create(this, callfunc_selector(BattleView::march)),
                          0,
                          false,
                          NULL,
                          CCCallFunc::create(this, callfunc_selector(BattleView::notice)));
    }
    else
    {
        if(info == WorldController::getInstance()->m_cityInfo.end()){
            march();
        }else{
            if(info->second.cityType == OriginTile || info->second.cityType == CampTile)
            {
                WorldMarchCheck* cmd = new WorldMarchCheck(m_targetIndex,info->second.cityType,WorldController::getInstance()->m_alertStateFlag);
                cmd->setCallback(CCCallFuncO::create(this, callfuncO_selector(BattleView::checkCanMarch), NULL));
                cmd->sendAndRelease();
                GameController::getInstance()->showWaitInterface();
            }
            else
            {
                march();
            }
        }
    }
}

void BattleView::checkCanMarch(CCObject* param){
    GameController::getInstance()->removeWaitInterface();
    NetResult* result = dynamic_cast<NetResult*>(param);
    
    if(result && result->getErrorCode() == Error_OP_FAILURE)
    {
        CCLOGFUNCF("出现错误,关闭此面板");
        PopupViewController::getInstance()->removeAllPopupView();
    }
    else
    {
        auto dic = _dict(result->getData());
        if(dic){
            bool ok = dic->valueForKey("needBreakShield")->boolValue();
            if(ok){
                YesNoDialog::show(_lang("101438").c_str(),CCCallFunc::create(this, callfunc_selector(BattleView::march)));
            }else{
                
                auto info = WorldController::getInstance()->m_cityInfo.find(m_targetIndex);
                if(info == WorldController::getInstance()->m_cityInfo.end()){
                    march();
                }else if (info->second.cityType == ResourceTile && info->second.playerName.empty()) {
                    bool marchingAlertFlag = false;
                    for(auto it :WorldController::getInstance()->m_marchInfo)
                    {
                        if(it.second.stateType == StateMarch && it.second.endPointIndex == m_targetIndex)
                        {
                            marchingAlertFlag = true;
                            break;
                        }
                    }
                    if (marchingAlertFlag) {
                        YesNoDialog::marchAlertShow(_lang("108897").c_str(),CCCallFunc::create(this, callfunc_selector(BattleView::march)),CCCallFunc::create(this, callfunc_selector(BattleView::closeSelf)));
                    }
                    else
                        march();
                }
                else
                    march();
            }
        }else{
             march();
        }
    }
}
void BattleView::updateArmyNumber(CCObject* obj)
{
    std::map<string, ArmyInfo>::iterator it;
    m_tmpArray->removeAllObjects();
    for (it = GlobalData::shared()->armyList.begin(); it!=GlobalData::shared()->armyList.end(); it++) {
        if(!it->second.isArmy){
            continue;
        }
        if((it->second).free != TroopsController::getInstance()->m_tmpConfSoldiers[it->first])
        {
            int count = (it->second).free-TroopsController::getInstance()->m_tmpConfSoldiers[it->first];
            TroopsController::getInstance()->m_tmpConfSoldiers[it->first] = (it->second).free;
            TroopsController::getInstance()->m_tmpFreeSoldiers[it->first] += count;
        }
        if((it->second).free==0)
        {
            continue;
        }
        int index = 0;
        bool addFlag = false;
        while (index < m_tmpArray->count()) {
            std::string armyId = dynamic_cast<CCString*>(m_tmpArray->objectAtIndex(index))->getCString();
            auto &army = GlobalData::shared()->armyList[armyId];
            if(army.armyLevel < it->second.armyLevel){
                m_tmpArray->insertObject(CCString::create(it->first), index);
                addFlag = true;
                break;
            }
            index++;
        }
        if(!addFlag){
            m_tmpArray->addObject(CCString::create(it->first));
        }
    }
    updateInfo();
}

void BattleView::march()
{
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(GUIDE_INDEX_CHANGE
                                                                           , CCString::createWithFormat("ED_attack"));
    auto dict = CCDictionary::create();
    auto soldierDict = TroopsController::getInstance()->saveBattle();
    if(soldierDict->count() == 0){
        YesNoDialog::showYesDialog(_lang("105112").c_str());
        return;
    }

    dict->setObject(soldierDict, "soldier");
    dict->setObject(CCInteger::create(m_targetIndex), "targetIndex");
    dict->setObject(CCInteger::create(m_haveOwner), "haveOwner");
    CCArray *arr = CCArray::create();
    dict->setObject(arr, "generals");
    std::string other = "";
    switch (m_bType) {
        case MethodRally:
            other = CC_ITOA(m_wtIndex);
            break;
        case MethodUnion:
            other = m_other;
            if(TroopsController::getInstance()->m_saveSoldierNum > m_rally){
                YesNoDialog::showYesDialog(_lang_1("115194",CC_ITOA(m_rally)));
                return;
            }
            break;
        case MethodYuanSolider:
            if(TroopsController::getInstance()->m_saveSoldierNum > m_rally){
                YesNoDialog::showYesDialog(_lang_1("115166",CC_ITOA(m_rally)));
                return;
            }
            break;
    }
    SoundController::sharedSound()->playEffects(Music_Sfx_world_click_march);
    m_marchBtn->setEnabled(false);
    if(SceneController::getInstance()->currentSceneId==SCENE_ID_WORLD && m_bType!=MethodRally){
        if (GlobalData::shared()->playerInfo.regCountry == "JP")
        {
            ///日本攻击忍者打点
            auto info = WorldController::getInstance()->m_cityInfo.find(m_targetIndex);
            if (info != WorldController::getInstance()->m_cityInfo.end())
            {
                if (info->second.cityType == FieldMonster)
                {
                    string mId = info->second.fieldMonsterInfo.monsterId;
                    int imId = atoi(mId.c_str());
                    if (imId >= 900125 && imId <= 900151) //忍者 ID
                    {
                        string killer = CCUserDefault::sharedUserDefault()->getStringForKey("riben.renzhe.killer");
                        if (killer.empty())
                        {
                            CCUserDefault::sharedUserDefault()->setStringForKey("riben.renzhe.killer", "1");
                            CCUserDefault::sharedUserDefault()->flush();
                        }
                    }
                }
            }
        }
        WorldMapView::instance()->afterMarchDeploy(dict,m_bType,other,m_targetType);
        PopupViewController::getInstance()->removeAllPopupView();
    }else{
        if(m_bType == -1){
            return;
        }
        auto cmd = new WorldMarchCommand(m_targetIndex, (MarchMethodType)m_bType,1,soldierDict,NULL,-1, arr,m_wtIndex,m_other);
        cmd->setCallback(CCCallFuncO::create(this, callfuncO_selector(BattleView::callSuccess), NULL));
        cmd->sendAndRelease();
    }
}

void BattleView::callSuccess(CCObject* param){
    int type = m_bType;
    AutoSafeRef temp(this);
    
    PopupViewController::getInstance()->removeAllPopupView();
    if(type==MethodRally)
    {
        auto info = WorldController::getInstance()->m_cityInfo.find(m_targetIndex);
        if(info != WorldController::getInstance()->m_cityInfo.end()){
            if (info->second.cityType == ActBossTile || m_targetType==ActBossTile) {
                WorldController::getInstance()->currentStamine -= GlobalData::shared()->worldConfig.boss_decr;
                CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_CURRENT_STAMINE);
            }
        }
        
        NetResult* result = dynamic_cast<NetResult*>(param);
        auto dic = _dict(result->getData());
        if(dic){
            auto teamInfo = _dict(dic->objectForKey("allianceBattleTeam"));
            if(teamInfo){
                AllianceTeamInfo*  team = new AllianceTeamInfo();
                team->parseInfo(teamInfo);
                PopupViewController::getInstance()->addPopupInView(AllianceWarDetailView::create(team));
                team->autorelease();
                
                //发聊天信息
                string nameStr = "";
                if (team->getTargetType() == ActBossTile) {
                    string resStr("");
                    auto allDic = GET_GROUPDIC_BY_GROUPNAME("field_monster");
                    CCDictElement* pelem = nullptr;
                    CCDICT_FOREACH(allDic, pelem){
                        auto modelDic = _dict(pelem->getObject());
                        string content = modelDic->valueForKey("id")->getCString();
                        if (content == team->getMonsterId()) {
                            resStr = modelDic->valueForKey("name")->getCString();
                            break;
                        }
                    }
                    nameStr = _lang(resStr);
                }
                else {
                    if (team->getTargetAAbbr() != "") {
                        nameStr.append("(").append(team->getTargetAAbbr()).append(")").append(team->getTargetName());
                    }
                    else
                        nameStr.append(team->getTargetName());
                }
                
                string msg = _lang_1("132118", nameStr.c_str());
                string dialog = "132118";
                CCArray* msgArr = CCArray::create();
                msgArr->addObject(CCString::create(nameStr));
                CountryChatCommand * cmd = new CountryChatCommand(CHAT_STATE_ALLIANCE_COMMAND,msg,CHAT_TYPE_ALLIANCE_RALLY,"",dialog,msgArr);
                cmd->putParam("teamUuid", CCString::create(team->getTeamId()));
                cmd->sendAndRelease();
            }
        }
    }else{
        if (m_targetType==ActBossTile) {
            WorldController::getInstance()->currentStamine -= GlobalData::shared()->worldConfig.boss_decr;
            CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_CURRENT_STAMINE);
        }
        SceneController::getInstance()->gotoScene(SCENE_ID_WORLD);
    }
}

void BattleView::notice()
{
    TroopsController::getInstance()->m_isNotice = !TroopsController::getInstance()->m_isNotice;
}

void BattleView::selectAll(){
    int totalNum = 0;
    int maxForceNum = TroopsController::getInstance()->getMaxSoilder(m_bType);
    
    if(m_rally>0){
        maxForceNum = MIN(m_rally, maxForceNum);
    }
    if (m_bType == MethodUnion || m_bType == MethodYuanSolider)
    {
        maxForceNum = MIN(m_rally, maxForceNum);
    }
//    if(m_box->isVisible()){
//        m_box->setSelect(true);
//        TroopsController::getInstance()->m_generalToSend[0] = GlobalData::shared()->generals.begin()->first;
//    }
    for (int i = 0; i<m_tmpArray->count(); i++) {
        std::string m_soldierId = dynamic_cast<CCString*>(m_tmpArray->objectAtIndex(i))->getCString();
        TroopsController::getInstance()->updateTmpBattleData(m_soldierId, 0, m_soldierId);
    }

    bool isResourceLimit = false;
    int totalResource = 0;
    float loadPerUnit = 0;
    auto info = WorldController::getInstance()->m_cityInfo.find(m_targetIndex);
    if(info != WorldController::getInstance()->m_cityInfo.end() && (info->second.cityType == ResourceTile)){
        bool isInMarch = false;
        if(info->second.cityType == ResourceTile){
            loadPerUnit = CCCommonUtils::getResourceLoadByType(info->second.resource.type);
        }else if (info->second.cityType == tile_superMine){
            loadPerUnit = CCCommonUtils::getResourceLoadByType(info->second.m_superMineInfo.type);
        }
        else if (info->second.cityType == Resource_new){
            loadPerUnit = CCCommonUtils::getResourceLoadByType(info->second.m_newResourceInfo.type);
        }
        for (auto march = WorldController::getInstance()->m_marchInfo.begin(); march != WorldController::getInstance()->m_marchInfo.end(); march++) {
            if (march->second.endPointIndex == m_targetIndex) {
                isInMarch = true;
                break;
            }
        }
        if(!isInMarch){
            isResourceLimit = true;
            totalResource = info->second.resource.sum+3;//防止采集剩余容错3个资源量
        }
    }
    
    for (int i = 0; i<m_tmpArray->count(); i++) {
        std::string m_soldierId = dynamic_cast<CCString*>(m_tmpArray->objectAtIndex(i))->getCString();
        int tmpCntNum = 0;
        tmpCntNum = TroopsController::getInstance()->m_tmpConfSoldiers[m_soldierId];
        tmpCntNum = totalNum + tmpCntNum >= maxForceNum ? maxForceNum - totalNum : tmpCntNum;
        if(isResourceLimit){
            float loadPerSoldier = ArmyController::getInstance()->getLoad(&GlobalData::shared()->armyList[m_soldierId], TroopsController::getInstance()->m_generalToSend.size());
            if(totalResource < int(int(TroopsController::getInstance()->m_curLoadNum + tmpCntNum * loadPerSoldier) / loadPerUnit)){
                int num = ceil((totalResource * loadPerUnit - TroopsController::getInstance()->m_curLoadNum) / loadPerSoldier);
                tmpCntNum = (num > tmpCntNum ? tmpCntNum : num);
            }
        }
        TroopsController::getInstance()->updateTmpBattleData(m_soldierId, tmpCntNum, m_soldierId);
        if(totalNum + tmpCntNum >= maxForceNum){
            break;
        }
        if(isResourceLimit && (totalResource*loadPerUnit < int(int(TroopsController::getInstance()->m_curLoadNum) * 1.0))){
            break;
        }
        totalNum += tmpCntNum;
    }
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(GUIDE_INDEX_CHANGE
                                                                           , CCString::createWithFormat("ED_quick"));
}

void BattleView::unselectAll(){
    for (int i = 0; i<m_tmpArray->count(); i++) {
        std::string m_soldierId = dynamic_cast<CCString*>(m_tmpArray->objectAtIndex(i))->getCString();
        TroopsController::getInstance()->updateTmpBattleData(m_soldierId, 0, m_soldierId);
    }
}

void BattleView::onAddClick(CCObject * pSender, Control::EventType pCCControlEvent){
    auto getTypeArray = [](int type){
        int city_lv = FunBuildController::getInstance()->getMainCityLv();
        auto goodDic = LocalController::shared()->DBXMLManager()->getGroupByKey("goods");
        CCArray* array = CCArray::create();//new CCArray();
        //array->init();
        int num = 0;
        if(goodDic)
        {
            CCDictElement* element;
            CCDICT_FOREACH(goodDic, element)
            {
                CCDictionary* dictInfo = _dict(element->getObject());
                if(type==dictInfo->valueForKey("type2")->intValue() && dictInfo->valueForKey("type")->intValue()==4 && city_lv>=dictInfo->valueForKey("lv")->intValue()){
                    array->addObject(dictInfo);
                }
            }
        }
        num = array->count();
        for (int i=0; i<num; i++) {
            for (int j=i; j<num; j++) {
                CCDictionary* dict1 = _dict(array->objectAtIndex(i));
                CCDictionary* dict2 = _dict(array->objectAtIndex(j));
                if(dict1->valueForKey("order_num")->intValue()<dict2->valueForKey("order_num")->intValue()){
                    array->swap(i, j);
                }
            }
        }
        return array;
    };
    ToolController::getInstance()->m_typeItems[14] = getTypeArray(14);
    PopupViewController::getInstance()->addPopupInView(UseItemStatusView::create(14,_lang("101413"),_lang("101450")));
}

void BattleView::onAddStamineClick(CCObject * pSender, Control::EventType pCCControlEvent){
    PopupViewController::getInstance()->addPopupInView(UseResToolView::create(13));
}

void BattleView::selectInGuide()
{
    for (int i = 0; i<m_tmpArray->count(); i++) {
        std::string m_soldierId = dynamic_cast<CCString*>(m_tmpArray->objectAtIndex(i))->getCString();
        TroopsController::getInstance()->updateTmpBattleData(m_soldierId, 15, m_soldierId);
    }
}

void BattleView::onClickQuickBtn(CCObject * pSender, Control::EventType pCCControlEvent)
{
    if(m_tmpArray->count() == 0){
        return;
    }
    if(int(TroopsController::getInstance()->m_curLoadNum) == 0){
//        TroopsController::getInstance()->m_curLoadNum = 0;
        selectAll();
        if (GuideController::share()->getCurGuideID() == "3030500") {
            selectInGuide();
        }
    }else{
        unselectAll();
//        TroopsController::getInstance()->m_curLoadNum = 0;
    }
    
    TroopsController::getInstance()->changeArrTime();
    auto& children = m_tabView->getContainer()->getChildren();
    for (auto child : children)
    {
        auto& childrenArr = child->getChildren();
        SoldierCell *cell = dynamic_cast<SoldierCell*>(childrenArr.at(0));
        if(cell){
            cell->refresh();
        }
    }
//    
//    CCArray *arr = m_tabView->getContainer()->getChildren();
//    int i = 0;
//    while(i < arr->count()){
//        CCArray *children = dynamic_cast<CCNode*>(arr->objectAtIndex(i++))->getChildren();
//        SoldierCell *cell = dynamic_cast<SoldierCell*>(children->objectAtIndex(0));
//        if(cell){
//            cell->refresh();
//        }
//    }
}

void BattleView::makeArrTime(CCObject* obj)
{
    int spd = TroopsController::getInstance()->m_minSpeed;
    if (!spd) {
        m_timeLabel->setString(CC_SECTOA(0));
        return;
    }
    
    MarchMethodType mType = MethodBattle;
    if(m_bType >= 0){
        mType = MarchMethodType(m_bType);
    }
    WorldCityType cityType = CityTile;
    auto info = WorldController::getInstance()->m_cityInfo.find(m_targetIndex);
    if(info != WorldController::getInstance()->m_cityInfo.end()){
        cityType = info->second.cityType;
    }
    int time = WorldController::getInstance()->getMarchTime(m_startIndex, m_targetIndex, cityType, mType) / 1000;
    time*=m_slow;
    m_timeLabel->setString(CC_SECTOA(time));
}

void BattleView::gridTouched(cocos2d::extension::CCMultiColTableView* table, CCTableViewCell* cell)
{
    SoldierCell* soldierCell = dynamic_cast<SoldierCell*>(cell);
    if (soldierCell && isTouchInside(m_infoList, m_tabView->m_pCurTouch)) {
        soldierCell->cellTouchEnded(m_tabView->m_pCurTouch);
    }
}

cocos2d::CCSize BattleView::gridSizeForTable(cocos2d::extension::CCMultiColTableView *table)
{
    if (CCCommonUtils::isIosAndroidPad()) {
        return CCSize(1410, 250);
    }
    return CCSize(640, 125);
}

CCTableViewCell* BattleView::gridAtIndex(cocos2d::extension::CCMultiColTableView *table, ssize_t idx)
{
    if(idx >= m_tmpArray->count()){
        return NULL;
    }
    
    SoldierCell* cell = (SoldierCell*)table->dequeueGrid();
    if(idx < m_tmpArray->count()){
        std::string gid = dynamic_cast<CCString*>(m_tmpArray->objectAtIndex(idx))->getCString();
        int num = 0;
        if(TroopsController::getInstance()->m_tmpBattleInfos.find(gid) != TroopsController::getInstance()->m_tmpBattleInfos.end())
        {
            num = TroopsController::getInstance()->m_tmpBattleInfos[gid];
        }
        if(cell){
            cell->setData(gid, num, m_bType, m_rally);
            cell->setTag(idx);
        }else{
            cell = SoldierCell::create(gid, num, m_bType, m_rally);
            cell->setTag(idx);
        }
    }
    return cell;
}

ssize_t BattleView::numberOfCellsInTableView(cocos2d::extension::CCMultiColTableView *table){
    int num = ceil(m_tmpArray->count() / numPerRow);
    return num;
}

ssize_t BattleView::numberOfGridsInCell(cocos2d::extension::CCMultiColTableView *multiTable){
    return numPerRow;
}

CCNode* BattleView::getGuideNode(string _key)
{
    if (_key=="attack") {
        return m_marchBtn;
    }
    else if (_key=="quick") {
        return m_quickBtn;
    }
    return NULL;
}

#pragma mark -
#pragma mark HeroCell Part

SoldierCell* SoldierCell::create(string itemId, int num, int type, int rally)
{
    SoldierCell *ret = new SoldierCell();
    if (ret && ret->init(itemId, num, type, rally)) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool SoldierCell::init(string itemId, int num, int type, int rally)
{
    bool ret = true;
    CCBLoadFile("March2",this,this);
    setContentSize(CCSize(620, 170));
    int sliderW = 255;
//    m_subBtn->setTouchPriority(Touch_Popup_Item);
//    m_addBtn->setTouchPriority(Touch_Popup_Item);
    m_subBtn->setZOrder(2);
    m_addBtn->setZOrder(2);
    auto m_sliderBg = CCLoadSprite::createScale9Sprite("cs_jindutiaoBG.png");
    m_sliderBg->setInsetBottom(5);
    m_sliderBg->setInsetLeft(5);
    m_sliderBg->setInsetRight(5);
    m_sliderBg->setInsetTop(5);
    m_sliderBg->setAnchorPoint(ccp(0.5,0.5));
    m_sliderBg->setPosition(ccp(sliderW/2, 25));
    m_sliderBg->setContentSize(CCSize(sliderW,9));
    
    auto bgSp = CCLoadSprite::createSprite("cs_jindutiaoBG.png");
    bgSp->setVisible(false);
    auto proSp = CCLoadSprite::createSprite("cs_jindutiao.png");
    auto thuSp = CCLoadSprite::createSprite("cs_jindutiao_tr.png");
    
    m_slider = CCSliderBar::createSlider(m_sliderBg, proSp, thuSp);
    m_slider->isPrecies=true;
    m_slider->setMinimumValue(0.0f);
    m_slider->setMaximumValue(1.0f);
    m_slider->setProgressScaleX(sliderW/proSp->getContentSize().width);
    m_slider->setTag(1);
    m_slider->setPosition(ccp(-33, -49));

    m_slider->setLimitMoveValue(15);
//    m_slider->setTouchPriority(Touch_Popup);
    m_slider->addTargetWithActionForControlEvents(this, cccontrol_selector(SoldierCell::valueChange), CCControlEventValueChanged);
    m_sliderNode->addChild(m_slider, 1);
    
    auto editSize = m_editNode->getContentSize();
     #if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        editSize = CCSizeMake(editSize.width,editSize.height+5);
    #endif
    m_editBox = CCEditBox::create(editSize, CCLoadSprite::createScale9Sprite("btn_bg_op.png"));
    m_editBox->setInputMode(kEditBoxInputModeNumeric);
    m_editBox->setText("0");
    m_editBox->setDelegate(this);
//    m_editBox->setTouchPriority(Touch_Popup);
    m_editBox->setMaxLength(12);
    m_editBox->setReturnType(kKeyboardReturnTypeDone);
    m_editBox->setPosition(ccp(editSize.width/2, editSize.height/2));
    m_editNode->addChild(m_editBox);

    setCleanFunction([](){
        CCLoadSprite::doResourceByCommonIndex(204, false);
    });

    setData(itemId, num, type, rally);
    return ret;
}

void SoldierCell::setData(string itemId, int num, int type, int rally)
{
    m_invalidSliderMove = true;
    m_soldierId = itemId;
    m_num = num;
    m_type = type;
    m_rally = rally;
    string name = CCCommonUtils::getNameById(m_soldierId);
    string picName = GlobalData::shared()->armyList[m_soldierId].getHeadIcon();
    m_cntNum = TroopsController::getInstance()->m_tmpConfSoldiers[m_soldierId];
    m_slider->setEnabled(true);
    m_nameLabel->setString(name.c_str());
    m_nameLabel->enableCOKShadow();
    m_picNode->removeAllChildren();
    
    m_iconPath = picName;
//    auto pic = CCLoadSprite::createSprite(picName.c_str());
//    CCCommonUtils::setSpriteMaxSize(pic, 110);
//    m_picNode->addChild(pic);
//    pic->setPositionY(-10);
    int star = ArmyController::getInstance()->getStarlvById(m_soldierId);
    auto pic = SoldierIconCell::create(picName, 80,m_soldierId,true,star);
//    CCCommonUtils::setSpriteMaxSize(pic, 110);
    m_picNode->addChild(pic);
    pic->setPositionY(-10);

    this->m_levelNode->removeAllChildren();
    string num1 = m_soldierId.substr(m_soldierId.size()-2);
    auto numLab = CCLabelIF::create();
    numLab->setFntFile("Arial_Bold_Border.fnt");
    numLab->setFontSize(16);
    numLab->setColor(ccc3(255,225,0));
    numLab->setString(CCCommonUtils::getChineseLevelTxtByNum(atoi(num1.c_str())+1).c_str());
    //auto pic1= CCCommonUtils::getRomanSprite(atoi(num1.c_str())+1);
    m_levelNode->addChild(numLab);
    
    float pro = 0;
    if (m_cntNum>0) {
        pro = m_num * 1.0 / m_cntNum;
    }
    m_slider->setValue(pro);
    m_editBox->setText(CC_ITOA(m_num));

    TroopsController::getInstance()->updateTmpBattleData(m_soldierId, m_num, m_soldierId);
    
}

void SoldierCell::refresh(){
    int num = 0;
    if(TroopsController::getInstance()->m_tmpBattleInfos.find(m_soldierId) != TroopsController::getInstance()->m_tmpBattleInfos.end())
    {
        num = TroopsController::getInstance()->m_tmpBattleInfos[m_soldierId];
    }

    setData(m_soldierId, num, m_type, m_rally);
}

void SoldierCell::valueChange(CCObject * pSender, Control::EventType pCCControlEvent)
{
    if (m_invalidSliderMove) {
        m_invalidSliderMove = false;
        return;
    }
    if (m_cntNum<=0) {
        return;
    }
    
    SoundController::sharedSound()->playEffects(Music_Sfx_click_button);
    float value = m_slider->getValue();
    int curNum = round(value * 1.0 * m_cntNum);
    int retNum = TroopsController::getInstance()->updateTmpBattleData(m_soldierId, curNum, m_soldierId);
    m_editBox->setText(CC_ITOA(retNum));
    TroopsController::getInstance()->changeArrTime();
    
    int maxForceNum = TroopsController::getInstance()->getMaxSoilder(m_type);
    if (m_type == MethodUnion || m_type == MethodYuanSolider)
    {
        maxForceNum = MIN(m_rally, maxForceNum);
    }
    
    int total = 0;
    map<string, int>::iterator it;
    for (it = TroopsController::getInstance()->m_tmpBattleInfos.begin(); it != TroopsController::getInstance()->m_tmpBattleInfos.end(); it++) {
        if (TroopsController::getInstance()->m_tmpBattleInfos[it->first] > 0) {
            total += TroopsController::getInstance()->m_tmpBattleInfos[it->first];
        }
    }
    
    int d = maxForceNum - total;
    if(d < 0){
        curNum += d;
         m_invalidSliderMove = true;
        if (curNum >= 0)
            m_slider->setValue(curNum * 1.0 / m_cntNum);
        else
            m_slider->setValue(0);
        
       
        
    }
}

void SoldierCell::onEnter() {
    CCNode::onEnter();
}

void SoldierCell::onExit() {
    CCNode::onExit();
}

SEL_CCControlHandler SoldierCell::onResolveCCBCCControlSelector(cocos2d::CCObject *pTarget, const char *pSelectorName) {
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onSubClick", SoldierCell::onSubClick);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onAddClick", SoldierCell::onAddClick);

    return NULL;
}

void SoldierCell::onSubClick(CCObject * pSender, Control::EventType pCCControlEvent){
    int num = ceil(m_slider->getValue() * m_cntNum) - 1;
    if(num < 0){
        num = 0;
    }
    m_slider->setValue(num * 1.0f / m_cntNum);
}

void SoldierCell::onAddClick(CCObject * pSender, Control::EventType pCCControlEvent){
    int num = ceil(m_slider->getValue() * m_cntNum) + 1;
    if(num > m_cntNum){
        num = m_cntNum;
    }
    m_slider->setValue(num * 1.0f / m_cntNum);
}

bool SoldierCell::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode) {
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameLabel", CCLabelIF*, m_nameLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_picNode", CCNode*, m_picNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_levelNode", CCNode*, m_levelNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_sliderNode", CCNode*, m_sliderNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_editNode", CCNode*, m_editNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_subBtn", CCControlButton*, m_subBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_addBtn", CCControlButton*, m_addBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_headTouchNode", CCNode*, m_headTouchNode);

    return false;
}

void SoldierCell::editBoxReturn(CCEditBox *editBox) {
    int value = atoi(editBox->getText());
    
    if (value > m_cntNum) {
        value = m_cntNum;
    } else if (value < 0) {
        value = 0;
    }
    value = TroopsController::getInstance()->updateTmpBattleData(m_soldierId, value, m_soldierId);
    
    if (m_cntNum > 0) {
        m_slider->setValue(1.0f * value / m_cntNum);
    }else{
        m_slider->setValue(0.0f);
    }
}

void SoldierCell::cellTouchEnded(CCTouch* pTouch)
{
    if (isTouchInside(m_headTouchNode, pTouch)) {
        string name = CCCommonUtils::getNameById(m_soldierId);
        string des = _lang( CCCommonUtils::getPropById(m_soldierId, "description") );
        PopupViewController::getInstance()->addPopupView(UnlockItemInfoView::create(name, des, m_iconPath));
    }
}
