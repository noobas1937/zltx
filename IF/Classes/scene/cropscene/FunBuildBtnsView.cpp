//
//  FunBuildBtnsView.cpp
//  IF
//
//  Created by fubin on 14-7-4.
//
//
#include "UpstarView.h"
#include "FunBuildBtnsView.h"
#include "SceneController.h"
#include "SoundController.h"
#include "PopupViewController.h"
#include "ScienceListView.h"
#include "WatchtowerView.h"
#include "MainCityView.h"
#include "FunBuildController.h"
#include "BuildingHospitalPopUpView.h"
#include "AllianceMarketView.h"
#include "BattleView.h"
#include "QueueController.h"
#include "ToolController.h"
#include "WorldController.h"
#include "ImperialScene.h"
#include "BuildListView.h"
#include "CityDefenseView.h"

#include "FunBuildView.h"
#include "MakingSoldierView.h"
#include "ProductionSoldiersView.h"
#include "ItemStatusView.h"
#include "YesNoDialog.h"
#include "AllianceMarketView.h"
#include "AllianceHelpView.h"
#include "BuildingAddOutPutCommand.h"
#include "YuanJunDetailView.h"
#include "AllianceManager.h"
#include "AllianceWarView.h"
#include "SacrificePopUpView.h"
#include "PropSpeedupView.h"
#include "EquipmentBagView.h"
#include "EquipSiteView.h"
#include "GuideController.h"

#include "AllianceDailyPublishView.h"
#include "AllianceDailyController.h"

#include "SkinListView.hpp"
#include "UseResToolView.h"

FunBuildBtnsView* FunBuildBtnsView::create()
{
    FunBuildBtnsView *pRet = new FunBuildBtnsView();
    if (pRet && pRet->init())
    {
        pRet->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(pRet);
    }
    return pRet;
}

bool FunBuildBtnsView::init()
{
    CCBLoadFile("BuildBtnView",this,this);
    m_name1->setString(_lang("102271"));
    m_name2->setString(_lang("102270"));
    m_name1->setFntFile("Arial_Bold_Border.fnt");
    m_name2->setFntFile("Arial_Bold_Border.fnt");
    m_name3->setFntFile("Arial_Bold_Border.fnt");
    m_name4->setFntFile("Arial_Bold_Border.fnt");
    m_msg5->setFntFile("Arial_Bold_Border.fnt");
    m_goldLabel->setFntFile("Arial_Bold_Border.fnt");
    m_IncreasedGlodTxt->setFntFile("Arial_Bold_Border.fnt");
    m_name5->setFntFile("Arial_Bold_Border.fnt");
    m_IncreasedTime->setFntFile("Arial_Bold_Border.fnt");
    m_msgV5->setFntFile("Arial_Bold_Border.fnt");
    
    this->getAnimationManager()->setAnimationCompletedCallback(this, callfunc_selector(FunBuildBtnsView::animationFunc));
    this->setScale(1.2);
    return true;
}

void FunBuildBtnsView::onHide()
{
    m_initEnd = false;
    this->getAnimationManager()->runAnimationsForSequenceNamed("FadeOut");
}

void FunBuildBtnsView::onShow(int buildId, int questType)
{
    guidKey = "";
    m_toolInfoId = 0;
    m_questType = questType;
    m_info = &((*FunBuildController::getInstance()->curBuildsInfo)[buildId]);
    m_buildingKey = buildId;
    m_toolNode->setVisible(false);
    m_nameLabel->setString(_lang(m_info->name));
    m_qid = QID_MAX;
    
    m_btn1Node->setPosition(ccp(-52-10, -10));
    m_btn2Node->setPosition(ccp(-156-20, 30));
    m_btn3Node->setPosition(ccp(52+10, -10));
    m_btn4Node->setPosition(ccp(156+20, 30));
    
    m_btn1->setTouchPriority(Touch_Imperial_City);
    m_btn2->setTouchPriority(Touch_Imperial_City);
    m_btn3->setTouchPriority(Touch_Imperial_City);
    m_btn4->setTouchPriority(Touch_Imperial_City);
    m_btn5->setTouchPriority(Touch_Imperial_City);
    
    int cellCnt = 2;
    buildType = m_buildingKey/1000;
    vector<string> btnIcons;
    if (buildType == FUN_BUILD_MAIN) {
        int mainCityLv = FunBuildController::getInstance()->getMainCityLv();
        if (mainCityLv >= 4 && mainCityLv < 5) {//城市增益按钮4级以上显示 城堡装扮按钮6级以上显示
            btnIcons.push_back("tile_pop_icon20.png");
            m_name3->setString(_lang("102282"));
        }
        else if (mainCityLv >= 5) {
            //btnIcons.push_back("cityskin_status.png");
            btnIcons.push_back("tile_pop_icon20.png");
            //m_name4->setString(_lang("101482"));
            m_name3->setString(_lang("102282"));
        }
    }
    else if (buildType == FUN_BUILD_TAVERN) {
        btnIcons.push_back("alliances_trade.png");
        m_name3->setString(_lang("102280"));
    }
    else if (buildType == FUN_BUILD_SCIENE) {
        btnIcons.push_back("science_icon.png");
        m_name3->setString(_lang("102275"));
    }
    else if (buildType == FUN_BUILD_FORGE) {
        btnIcons.push_back("icon_forging01.png");
        m_name3->setString(_lang("102349"));
        btnIcons.push_back("glft003.png");
        m_name4->setString(_lang("102350"));
    }
    else if(buildType == FUN_BUILD_BARRACK1 || buildType == FUN_BUILD_BARRACK2 || buildType == FUN_BUILD_BARRACK3 || buildType == FUN_BUILD_BARRACK4){
        btnIcons.push_back("shangBing.png");
        m_name3->setString(_lang("102276"));
    }
    else if (buildType == FUN_BUILD_WAR) {
        btnIcons.push_back("item501.png");
    }
    else if (buildType == FUN_BUILD_WATCH_TOWER) {
        btnIcons.push_back("icon_junshixiangqing.png");
        m_name3->setString(_lang("102278"));
    }
    else if (buildType == FUN_BUILD_HOSPITAL) {
        btnIcons.push_back("icon_treatment.png");
        m_name3->setString(_lang("102147"));
    }
    else if (buildType == FUN_BUILD_WALL) {
        btnIcons.push_back("cityDef_icon.png");
        m_name3->setString(_lang("102140"));
    }
    else if(buildType == FUN_BUILD_FORT){
        btnIcons.push_back("icon_trap.png");
        m_name3->setString(_lang("102277"));
    }
    else if(buildType == FUN_BUILD_SMITHY){
        btnIcons.push_back("allianceWar.png");
        m_name3->setString(_lang("115190"));
    }
    else if(buildType == FUN_BUILD_SACRIFICE){
        btnIcons.push_back("icon_sacrifice.png");
        m_name3->setString(_lang("102328"));
    }
    else if(buildType == FUN_BUILD_STABLE) {
        btnIcons.push_back("alliances_help.png");
        m_name3->setString(_lang("108541"));
        btnIcons.push_back("allianceYuanYun.png");
        m_name4->setString(_lang("115151"));
        
    }else if(buildType == FUN_BUILD_CELLAR){//1.将联盟任务入口调整为仓库
//        if(AllianceDailyController::getInstance()->isSwitchOpen()){
//            btnIcons.push_back("alliance_dail_quest_b.png");
//            m_name3->setString(_lang("134000"));//联盟任务
//            if (GlobalData::shared()->fun_bindRes_switch) {
//                btnIcons.push_back("res_bind_enter_icon.png");
//                m_name4->setString(_lang("139509"));//绑定资源
//            }
//        }
//        else
//        {
        //liu to do  去掉联盟宝藏
            if (GlobalData::shared()->fun_bindRes_switch) {
                btnIcons.push_back("res_bind_enter_icon.png");
                m_name3->setString(_lang("139509"));//绑定资源
            }
//        }
//        btnIcons.push_back("icon_sacrifice.png");
//        m_name3->setString(_lang("102328"));
//        if(AllianceDailyController::getInstance()->isSwitchOpen()){
//            auto icon = CCLoadSprite::createSprite("alliance_dail_quest_r.png");
//            CCCommonUtils::setSpriteMaxSize(icon, 90, true);
//            m_icon5Node->addChild(icon);
//            m_msg5->setString(_lang("134000"));//联盟任务
//        }
    }
    else if(buildType == FUN_BUILD_FOOD || buildType == FUN_BUILD_WOOD || buildType == FUN_BUILD_IRON || buildType == FUN_BUILD_STONE) {
        btnIcons.push_back("icon_jiasu.png");//_lang("104903")
        double effectTime = m_info->effectTime;
        double gapTime = effectTime - GlobalData::shared()->getWorldTime();
        if(gapTime>0 && effectTime > 0){
            m_name3->setString(_lang("102291"));
        }else{
            int k6 = FunBuildController::getInstance()->building_base_k6;
            m_name3->setString(CCCommonUtils::changeTimeAddUnit(k6));
        }
        
        m_name4->setString(_lang("102292"));
        if(buildType == FUN_BUILD_WOOD) {
            string strPic = CCCommonUtils::getResourceIconByType(Wood);
            btnIcons.push_back(strPic);
        }
        else if(buildType == FUN_BUILD_IRON) {
            string strPic = CCCommonUtils::getResourceIconByType(Iron);
            btnIcons.push_back(strPic);
        }
        else if(buildType == FUN_BUILD_STONE) {
            string strPic = CCCommonUtils::getResourceIconByType(Stone);
            btnIcons.push_back(strPic);
        }
        else if(buildType == FUN_BUILD_FOOD) {
            string strPic = CCCommonUtils::getResourceIconByType(Food);
            btnIcons.push_back(strPic);
        }
    }
    m_btn3Node->setVisible(false);
    m_btn3->setEnabled(false);
    m_IncreasedNode->setVisible(false);
    if (btnIcons.size() >= 1) {
        m_icon3Node->removeAllChildren();
        auto icon = CCLoadSprite::createSprite(btnIcons[0].c_str());
        CCCommonUtils::setSpriteMaxSize(icon, 90, true);
        m_icon3Node->addChild(icon);
        m_btn3Node->setVisible(true);
        m_btn3->setEnabled(true);
        cellCnt=3;
        if(buildType == FUN_BUILD_FOOD || buildType == FUN_BUILD_WOOD || buildType == FUN_BUILD_IRON || buildType == FUN_BUILD_STONE) {
            
            m_IncreasedNode->setVisible(true);
            double effectTime = m_info->effectTime;
            double gapTime = effectTime - GlobalData::shared()->getWorldTime();
            if(gapTime<=0){
                m_IncreasedGold->setVisible(true);
                m_IncreasedGlodTxt->setString(m_info->para[3].c_str());
                int k6 = FunBuildController::getInstance()->building_base_k6;
                m_IncreasedTime->setString(CCCommonUtils::changeTimeAddUnit(k6));
                m_name3->setString(_lang("102293"));
                m_IncreasedTime->setColor({129,212,150});
                m_btn3->setOpacity(255);
            }else{
                m_IncreasedGold->setVisible(false);
                m_IncreasedGlodTxt->setString("");
                std::string timeLeft = CCCommonUtils::changeTimeAddUnit(gapTime);
                m_IncreasedTime->setString(timeLeft);
                CCCommonUtils::setSpriteGray(icon, true);
                m_IncreasedTime->setColor(ccWHITE);
                m_btn3->setOpacity(200);
            }
        }
        else if (buildType == FUN_BUILD_SCIENE){
            if (m_info->level <= 3 && m_questType == 2) {
                auto btnEffect = FunBuildBtnsEffect::create();
                m_icon3Node->addChild(btnEffect);
            }
        }
    }
    
    m_btn4Node->setVisible(false);
    m_btn4->setEnabled(false);
    if (btnIcons.size() >= 2) {
        m_icon4Node->removeAllChildren();
        auto icon = CCLoadSprite::createSprite(btnIcons[1].c_str());
        CCCommonUtils::setSpriteMaxSize(icon, 90, true);
        m_btn4Node->setVisible(true);
        m_btn4->setEnabled(true);
        cellCnt=4;
        if (buildType == FUN_BUILD_FOOD || buildType == FUN_BUILD_WOOD || buildType == FUN_BUILD_IRON || buildType == FUN_BUILD_STONE) {
            m_btn4->setEnabled(false);
            CCCommonUtils::setSpriteGray(icon, true);
        }
        m_icon4Node->addChild(icon);
    }
    
    m_btn5Node->setVisible(false);
    m_btn5->setEnabled(false);
    
    if (cellCnt==3) {
        m_btn1Node->setPosition(ccp(0, -16));
        m_btn2Node->setPosition(ccp(-107-10, 6));
        m_btn3Node->setPosition(ccp(107+10, 6));
    }
    else if (cellCnt==2) {
        m_btn1Node->setPosition(ccp(52+10, -10));
        m_btn2Node->setPosition(ccp(-52-10, -10));
    }
    else if (cellCnt==4 && (buildType == FUN_BUILD_WOOD || buildType == FUN_BUILD_STONE || buildType == FUN_BUILD_IRON || buildType == FUN_BUILD_FOOD))
    {
        m_btn2Node->setPosition(ccp(-52-10, -10));
        m_btn3Node->setPosition(ccp(-156-20, 30));
        m_btn1Node->setPosition(ccp(52+10, -10));
        m_btn4Node->setPosition(ccp(156+20, 30));
    }
    m_btnNum = cellCnt;
    this->setVisible(true);
    onShowInfo();
    this->getAnimationManager()->runAnimationsForSequenceNamed("FadeIn");
    auto layer = dynamic_cast<ImperialScene*>(SceneController::getInstance()->getCurrentLayerByLevel(LEVEL_SCENE));
    if (layer) {
        layer->setQuestEffect(0);
    }
}

bool FunBuildBtnsView::onShowInfo()
{
    m_toolNode->setVisible(true);
//    m_toolNode->setScale(0);
    bool isTmp = false;
    int qType = CCCommonUtils::getQueueTypeByBuildType(buildType);
    if (qType>0){//判断是否是 特殊建筑，并取得是否有正在工作的 队列
        isTmp = true;
        m_qid = QueueController::getInstance()->getMinTimeQidByType(qType);
    }
    
    if (m_info->state == FUN_BUILD_NORMAL) {
        if (m_qid == QID_MAX) {
            m_toolNode->setPositionY(-93.6+60);
            m_nameNode->setPositionY(250);
            if (buildType == FUN_BUILD_WOOD || buildType == FUN_BUILD_STONE || buildType == FUN_BUILD_IRON || buildType == FUN_BUILD_FOOD || buildType == FUN_BUILD_BARRACK || buildType == FUN_BUILD_HOSPITAL){
                m_nameNode->setPositionY(150);
            }
            m_spdNode->setVisible(false);
            m_icon1Node->removeAllChildren();
            int lv = m_info->level;
            if ((lv >= 30 || buildType == FUN_BUILD_QIJI)&&FunBuildController::getInstance()->building_Miracle_open) {
                 auto icon = CCLoadSprite::createSprite("rongyuxunzhang_UP.png");
                CCCommonUtils::setSpriteMaxSize(icon, 90, true);
                m_icon1Node->addChild(icon);
                m_name2->setString(_lang("160001"));
            }
            else{
                 auto icon = CCLoadSprite::createSprite("build_up_icon.png");
                CCCommonUtils::setSpriteMaxSize(icon, 90, true);
                m_icon1Node->addChild(icon);
                m_name2->setString(_lang("102270"));
                if (m_info->level <= 3 && m_questType == 1) {
                    auto btnEffect = FunBuildBtnsEffect::create();
                    m_icon1Node->addChild(btnEffect);
                }
            }
            
//            CCCommonUtils::setSpriteMaxSize(icon, 90, true);
//            m_icon1Node->addChild(icon);
            
            if(isTmp && buildType!=FUN_BUILD_FORGE) {
                m_btn1Node->setPosition(ccp(0, -16));
                m_btn2Node->setPosition(ccp(-107-10, 6));
                m_btn3Node->setPosition(ccp(107+10, 6));
                m_btn3Node->setVisible(true);
                m_btn3->setEnabled(true);
            }
            
            if(buildType == FUN_BUILD_WOOD || buildType == FUN_BUILD_STONE || buildType == FUN_BUILD_IRON || buildType == FUN_BUILD_FOOD) {
                double effectTime = m_info->effectTime;
                double gapTime = effectTime - GlobalData::shared()->getWorldTime();
                if(gapTime<=0){
                    if (onUpdateToolBtn(buildType)) {
                        m_btn2Node->setPosition(ccp(-0, -16));
                        m_btn3Node->setPosition(ccp(-107-10, 6));
                        m_btn1Node->setPosition(ccp(107+10, 6));
                        m_btn4Node->setPosition(ccp(200+20, 60));
                        m_btn5Node->setPosition(ccp(-200-20, 60));
                    }
                }
            }
//            if (buildType == FUN_BUILD_STABLE && AllianceDailyController::getInstance()->isSwitchOpen()) {
//                m_btn5Node->setVisible(true);
//                m_btn5->setEnabled(true);
//                m_btn1Node->setPosition(ccp(-107, 6));
//                m_btn2Node->setPosition(ccp(-200, 60));
//                m_btn3Node->setPosition(ccp(0, -16));
//                m_btn4Node->setPosition(ccp(107,6));
//                m_btn5Node->setPosition(ccp(200, 60));
//                
//            }
        }
        else {//建筑正在造兵 或者 正在研究
            m_toolNode->setPositionY(-93.6);
            m_nameNode->setPositionY(250+60);
            if (buildType == FUN_BUILD_HOSPITAL){
                m_nameNode->setPositionY(150+60);
            }
            m_spdNode->setVisible(true);
            m_icon1Node->removeAllChildren();
            m_name2->setString(_lang("104903"));
            
            if( onUpdateToolBtn(0) && isTmp) {
                if (buildType == FUN_BUILD_SCIENE || buildType == FUN_BUILD_BARRACK1 || buildType == FUN_BUILD_BARRACK2 || buildType == FUN_BUILD_BARRACK3 || buildType == FUN_BUILD_BARRACK4 || buildType == FUN_BUILD_FORT) {
                    m_btn1Node->setPosition(ccp(-52-10, -10));
                    m_btn2Node->setPosition(ccp(-156-20, 30));
                    m_btn3Node->setPosition(ccp(156+20, 30));
                    m_btn5Node->setPosition(ccp(52+10, -10));
                }
                else if (buildType == FUN_BUILD_FORGE) {
                    m_btn1Node->setPosition(ccp(-107-10, 6));
                    m_btn2Node->setPosition(ccp(-200-20, 60));
                    m_btn3Node->setPosition(ccp(107+10,6));
                    m_btn4Node->setPosition(ccp(200+20, 60));
                    m_btn5Node->setPosition(ccp(0, -16));
                }
                else {
                    m_btn3Node->setVisible(false);
                    m_btn3->setEnabled(false);
                    
                    m_btn1Node->setPosition(ccp(0, -16));
                    m_btn2Node->setPosition(ccp(-107-10, 6));
                    m_btn5Node->setPosition(ccp(107+10, 6));//显示道具加速按钮
                }
            }
            else {
                if(isTmp && buildType == FUN_BUILD_HOSPITAL) {
                    m_btn1Node->setPosition(ccp(52+10, -10));
                    m_btn2Node->setPosition(ccp(-52-10, -10));
                    m_btn3Node->setVisible(false);
                    m_btn3->setEnabled(false);
                }
            }
        }
    }
    else {//升级 或者 建造中， 显示 秒建筑的 cd按钮
        m_qid = QueueController::getInstance()->getMinTimeQidByType(TYPE_BUILDING, CC_ITOA(m_buildingKey));
        m_toolNode->setPositionY(-93.6);
        m_nameNode->setPositionY(250+60);
        if (buildType == FUN_BUILD_WOOD || buildType == FUN_BUILD_STONE || buildType == FUN_BUILD_IRON || buildType == FUN_BUILD_FOOD || buildType == FUN_BUILD_BARRACK || buildType == FUN_BUILD_HOSPITAL){
            m_nameNode->setPositionY(150+60);
        }
        m_spdNode->setVisible(true);
        m_icon1Node->removeAllChildren();
        m_name2->setString(_lang("104903"));
        
        if (m_btnNum==4 && (buildType == FUN_BUILD_WOOD || buildType == FUN_BUILD_STONE || buildType == FUN_BUILD_IRON || buildType == FUN_BUILD_FOOD)) {
            m_btn1Node->setPosition(ccp(52+10, -10));
            m_btn2Node->setPosition(ccp(-52-10, -10));
            m_btn3Node->setVisible(false);
            m_btn3->setEnabled(false);
            m_btn4Node->setVisible(false);
            m_btn4->setEnabled(false);
        }
        
        if(isTmp) {
            m_btn1Node->setPosition(ccp(52+10, -10));
            m_btn2Node->setPosition(ccp(-52-10, -10));
            m_btn3Node->setVisible(false);
            m_btn3->setEnabled(false);
            
            if (buildType == FUN_BUILD_FORGE) {
                m_btn1Node->setPosition(ccp(-52-10, -10));
                m_btn2Node->setPosition(ccp(-156-20, 30));
                m_btn4Node->setPosition(ccp(156+20, 30));
                m_btn5Node->setPosition(ccp(52-10, -10));
            }
        }
        if(onUpdateToolBtn(0)) {
            if (!isTmp && buildType!=FUN_BUILD_WOOD && buildType!=FUN_BUILD_STONE && buildType!=FUN_BUILD_IRON && buildType!=FUN_BUILD_FOOD) {
                if(m_btnNum==4) {
                    m_btn1Node->setPosition(ccp(-107-10, 6));
                    m_btn2Node->setPosition(ccp(-200-20, 60));
                    m_btn3Node->setPosition(ccp(107+10, 6));
                    m_btn4Node->setPosition(ccp(200+20, 60));
                    
                    m_btn5Node->setPosition(ccp(0, -16));
                }
                if(m_btnNum==3) {
                    m_btn1Node->setPosition(ccp(-52-10, -10));
                    m_btn2Node->setPosition(ccp(-156-20, 30));
                    m_btn3Node->setPosition(ccp(156+20, 30));
                    m_btn5Node->setPosition(ccp(52+10, -10));
                }
                if(m_btnNum==2) {
                    m_btn1Node->setPosition(ccp(0, -16));
                    m_btn2Node->setPosition(ccp(-107-10, 6));
                    m_btn5Node->setPosition(ccp(107+10, 6));//显示道具加速按钮
                }
            }
            else {
                m_btn1Node->setPosition(ccp(0, -16));
                m_btn2Node->setPosition(ccp(-107-10, 6));
                m_btn5Node->setPosition(ccp(107+10, 6));//显示道具加速按钮
                
                if (buildType == FUN_BUILD_FORGE) {
                    m_btn1Node->setPosition(ccp(-52-10, -10));
                    m_btn2Node->setPosition(ccp(-156-20, 30));
                    m_btn4Node->setPosition(ccp(156+20, 30));
                    m_btn5Node->setPosition(ccp(52+10, -10));
                }
            }
        }else {
            if (buildType == FUN_BUILD_FORGE) {
                m_btn1Node->setPosition(ccp(0, -16));
                m_btn2Node->setPosition(ccp(-107-10, 6));
                m_btn4Node->setPosition(ccp(107+10, 6));
            }
        }
    }
    
    onEnterFrame(0);
    
    return true;
}

void FunBuildBtnsView::animationFunc()
{
    string name = this->getAnimationManager()->getLastCompletedSequenceName();
    if (name == "FadeOut") {
        this->setVisible(false);
    }
    else if (name == "FadeIn") {
        m_initEnd = true;
    }
}

void FunBuildBtnsView::onEnter() {
    CCNode::onEnter();
    CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(FunBuildBtnsView::onEnterFrame), this, 1.0,kCCRepeatForever, 0.0f, false);
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);
    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, Touch_Imperial_City+1, true);
}

void FunBuildBtnsView::onExit() {
    CCNode::onExit();
    CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(FunBuildBtnsView::onEnterFrame), this);
    setTouchEnabled(false);
}

bool FunBuildBtnsView::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    if (this->isVisible() && isTouchInside(m_touchNode, pTouch)) {
        return true;
    }
    return false;
}

void FunBuildBtnsView::onTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
}

void FunBuildBtnsView::onEnterFrame(float dt)
{
    if (this->isVisible())
    {
        if(m_qid != QID_MAX) {
            auto& qInfo = GlobalData::shared()->allQueuesInfo[m_qid];
            lastTime = qInfo.finishTime - GlobalData::shared()->getWorldTime();
            m_gold = CCCommonUtils::getGoldByTime(lastTime);
            if (qInfo.type == TYPE_BUILDING) {
                if (lastTime <= GlobalData::shared()->freeSpdT) {
                    m_gold = 0;
                    this->setVisible(false);
                }
            }
            if (lastTime<=0 && qInfo.type != TYPE_BUILDING) {
                auto layer = dynamic_cast<ImperialScene*>(SceneController::getInstance()->getCurrentLayerByLevel(LEVEL_SCENE));
                if(layer) {
                    layer->setUnMoveScence(false);
                }
                CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(GUIDE_INDEX_CHANGE
                                                                                       , CCString::createWithFormat("BU_%d_cd", m_buildingKey/1000));
                this->setVisible(false);
            }
            m_goldLabel->setString(CC_ITOA(m_gold));
        }
        else if (m_info->state == FUN_BUILD_NORMAL) {
            if(!m_btn4->isEnabled() && FunBuildController::getInstance()->getOutPut(m_info->itemId)>0 )
            {
                m_btn4->setEnabled(true);
                m_icon4Node->removeAllChildren();
                string iconStr = "";
                if (buildType == FUN_BUILD_FOOD) {
                    iconStr = CCCommonUtils::getResourceIconByType(Food);
                }else if (buildType == FUN_BUILD_WOOD) {
                    iconStr = CCCommonUtils::getResourceIconByType(Wood);
                }else if (buildType == FUN_BUILD_IRON) {
                    iconStr = CCCommonUtils::getResourceIconByType(Iron);
                }else if (buildType == FUN_BUILD_STONE) {
                    iconStr = CCCommonUtils::getResourceIconByType(Stone);
                }
                auto icon = CCLoadSprite::createSprite(iconStr.c_str());
                CCCommonUtils::setSpriteMaxSize(icon, 90, true);
                m_icon4Node->addChild(icon);
            }
        }
        
        if (m_toolInfoId>0 && m_btn5->isEnabled())
        {
            auto& m_toolInfo = ToolController::getInstance()->getToolInfoById(m_toolInfoId);
            if (m_toolInfo.getCNT()<=0)
            {
                if (m_toolInfo.type == ITEM_TYPE_SPD) {//加速道具 没了
                    if(onUpdateToolBtn(0)==true)
                        return;
                    m_btn5Node->setVisible(false);
                    m_btn5->setEnabled(false);
                    if (m_info->state==FUN_BUILD_NORMAL && (buildType == FUN_BUILD_SCIENE || buildType == FUN_BUILD_BARRACK1 || buildType == FUN_BUILD_BARRACK2 || buildType == FUN_BUILD_BARRACK3 || buildType == FUN_BUILD_BARRACK4 || buildType == FUN_BUILD_FORT)) {//科技
                        m_btn1Node->setPosition(ccp(0, -16));
                        m_btn2Node->setPosition(ccp(-107, 6));
                        m_btn3Node->setPosition(ccp(107, 6));
                    }
                    else {
                        m_btn1Node->setPosition(ccp(52, -10));
                        m_btn2Node->setPosition(ccp(-52, -10));
                        int qType = CCCommonUtils::getQueueTypeByBuildType(buildType);
                        if (qType==-1 && (m_info->state==FUN_BUILD_UPING || m_info->state==FUN_BUILD_CREATE)) {
                            if (buildType!=FUN_BUILD_WOOD && buildType!=FUN_BUILD_FOOD && buildType!=FUN_BUILD_IRON && buildType!=FUN_BUILD_STONE) {
                                if(m_btnNum==4) {
                                    m_btn1Node->setPosition(ccp(-52, -10));
                                    m_btn2Node->setPosition(ccp(-156, 30));
                                    m_btn4Node->setPosition(ccp(156, 30));
                                    m_btn3Node->setPosition(ccp(52, -10));
                                }
                                if(m_btnNum==3) {
                                    m_btn1Node->setPosition(ccp(0, -16));
                                    m_btn2Node->setPosition(ccp(-107, 6));
                                    m_btn3Node->setPosition(ccp(107, 6));
                                }
                            }
                        }
                    }
                }
                else {//资源加速生产道具没了
                    m_btn5Node->setVisible(false);
                    m_btn5->setEnabled(false);
                    m_btn2Node->setPosition(ccp(-52, -10));
                    m_btn3Node->setPosition(ccp(-156, 30));
                    m_btn1Node->setPosition(ccp(52, -10));
                    m_btn4Node->setPosition(ccp(156, 30));
                }
            }
        }
    }
}

SEL_MenuHandler FunBuildBtnsView::onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName)
{return NULL;}

SEL_CCControlHandler FunBuildBtnsView::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName)
{
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onBtn1Click", FunBuildBtnsView::onBtn1Click);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onBtn2Click", FunBuildBtnsView::onBtn2Click);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onBtn3Click", FunBuildBtnsView::onBtn3Click);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onBtn4Click", FunBuildBtnsView::onBtn4Click);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onBtn5Click", FunBuildBtnsView::onBtn5Click);
    return NULL;
}

bool FunBuildBtnsView::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_goldLabel", CCLabelIF*, this->m_goldLabel);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btn1", CCControlButton*, this->m_btn1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btn2", CCControlButton*, this->m_btn2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btn3", CCControlButton*, this->m_btn3);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btn4", CCControlButton*, this->m_btn4);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btn5", CCControlButton*, this->m_btn5);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btn1Node", CCNode*, this->m_btn1Node);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btn2Node", CCNode*, this->m_btn2Node);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btn3Node", CCNode*, this->m_btn3Node);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btn4Node", CCNode*, this->m_btn4Node);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btn5Node", CCNode*, this->m_btn5Node);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_icon1Node", CCNode*, this->m_icon1Node);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_icon2Node", CCNode*, this->m_icon2Node);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_icon3Node", CCNode*, this->m_icon3Node);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_icon4Node", CCNode*, this->m_icon4Node);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_icon5Node", CCNode*, this->m_icon5Node);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_toolNode", CCNode*, this->m_toolNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_spdNode", CCNode*, this->m_spdNode);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameNode", CCNode*, this->m_nameNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameLabel", CCLabelIF*, this->m_nameLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameBgSpr", CCSprite*, this->m_nameBgSpr);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_name1", CCLabelIF*, this->m_name1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_name2", CCLabelIF*, this->m_name2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_name3", CCLabelIF*, this->m_name3);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_name4", CCLabelIF*, this->m_name4);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_name5", CCLabelIF*, this->m_name5);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_msg5", CCLabelIF*, this->m_msg5);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_msgV5", CCLabelIF*, this->m_msgV5);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_IncreasedNode", CCNode*, this->m_IncreasedNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_IncreasedGold", CCSprite*, this->m_IncreasedGold);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_IncreasedGlodTxt", CCLabelIF*, this->m_IncreasedGlodTxt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_IncreasedTime", CCLabelIF*, this->m_IncreasedTime);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_touchNode", CCNode*, this->m_touchNode);
    
    return false;
}

void FunBuildBtnsView::onBtn1Click(CCObject * pSender, Control::EventType pCCControlEvent)
{
    if (!m_initEnd) {
        return;
    }
    if (guidKey!="") {
        onGuidClick();
        return;
    }
    onHideShadow();
    m_toolNode->setVisible(false);
    if (m_info->state == FUN_BUILD_NORMAL) {
        if(m_qid == QID_MAX) {
            //TODO: 升星升级区别判断
            if(!FunBuildController::getInstance()->building_Miracle_open && m_info->level >= GlobalData::shared()->MaxBuildLv){
                CCCommonUtils::flyHint("", "", _lang("102103"));
            }else if ((m_info->level>= 30 || buildType == FUN_BUILD_QIJI) ) {//判断是要荣耀升星

                if(FunBuildController::getInstance()->checkOpenUpstar(buildType)){ //建筑可以升级荣耀
                    auto m_info = FunBuildController::getInstance()->getFunbuildById(m_buildingKey);
                    if(m_info.starNum >= GlobalData::shared()->starnum_k6){  //荣耀升级已满
                        CCCommonUtils::flyHint("", "", _lang_2("160021",_lang(m_info.name.c_str()).c_str(),CC_ITOA(m_info.starNum)));
                        return;
                    }
                    else{//荣耀升级未满
                        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_MAINSCENCE_SAVEPOS);
                        auto layer = dynamic_cast<ImperialScene*>(SceneController::getInstance()->getCurrentLayerByLevel(LEVEL_SCENE));
//                        layer->onMoveToBuildAndOpen(m_buildingKey, TYPE_POS_UP,0.25,false,true);
                        layer->onMoveToBuildAndOpen(m_buildingKey, TYPE_POS_UP,0.15,false,true);
                        PopupViewController::getInstance()->addPopupInView(UpstarView::create(m_buildingKey));
                    }
                }
                else{//建筑不能够升级荣耀
                    CCCommonUtils::flyHint("", "", _lang("160005"));
                }
            }else{//判断是要普通建筑升级
                CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_MAINSCENCE_SAVEPOS);
                auto layer = dynamic_cast<ImperialScene*>(SceneController::getInstance()->getCurrentLayerByLevel(LEVEL_SCENE));
                if(layer)
                    layer->onMoveToBuildAndOpen(m_buildingKey, TYPE_POS_UP);
                PopupViewController::getInstance()->addPopupInView(BuildUpgradeView::create(m_buildingKey));
            }
            CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(GUIDE_INDEX_CHANGE, CCString::createWithFormat("BU_%d_up", buildType));
        }
        else {
            if(PopupViewController::getInstance()->CanPopPushView) {
                double left = GlobalData::shared()->playerInfo.useGoldCureCDTime - GlobalData::shared()->getWorldTime();
                if(left>0 && GlobalData::shared()->serverType==SERVER_DRAGON_BATTLE && buildType == FUN_BUILD_HOSPITAL){
                    YesNoDialog::show( _lang("140192").c_str(), NULL);
                }else{
                    YesNoDialog::showTime( _lang("102283").c_str() , CCCallFunc::create(this, callfunc_selector(FunBuildBtnsView::onCostGoldBack)), lastTime, _lang("104903").c_str(), false);
                }
            }
            else {
                if (m_gold > 0) {
                    if (GlobalData::shared()->playerInfo.gold < m_gold) {
                        YesNoDialog::gotoPayTips();
                        return;
                    }
                }
                if (GuideController::share()->getCurGuideID() == GUIDE_TROOP_CD_ED) {
                    QueueController::getInstance()->startCCDQueue(m_qid, "", false, m_gold,"",1, GUIDE_TROOP_CD_ED);
                }else {
                    QueueController::getInstance()->startCCDQueue(m_qid, "", false, m_gold,"",1);
                }
            }
        }
    }
    else {
        if(PopupViewController::getInstance()->CanPopPushView && m_gold>0) {
            YesNoDialog::showTime( _lang("102283").c_str() , CCCallFunc::create(this, callfunc_selector(FunBuildBtnsView::onCostGoldBack)), lastTime, _lang("104903").c_str());
        }
        else {
            if (FunBuildController::getInstance()->costCD(m_buildingKey, "", m_gold) )
            {
//                CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(GUIDE_INDEX_CHANGE
//                                                                                       , CCString::createWithFormat("BU_%d_cd", buildType));
            }
        }
    }
}

void FunBuildBtnsView::onCostGoldBack()
{
    if (m_info->state == FUN_BUILD_NORMAL) {
        if (m_gold > 0) {
            if (GlobalData::shared()->playerInfo.gold < m_gold) {
                YesNoDialog::gotoPayTips();
                return;
            }
        }
        QueueController::getInstance()->startCCDQueue(m_qid, "", false, m_gold,"",1);
    }
    else {
        if (FunBuildController::getInstance()->costCD(m_buildingKey, "", m_gold) )
        {
//            CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(GUIDE_INDEX_CHANGE
//                                                                                   , CCString::createWithFormat("BU_%d_cd", buildType));
        }
    }
}

void FunBuildBtnsView::onBtn2Click(CCObject * pSender, Control::EventType pCCControlEvent)
{
    if (!m_initEnd) {
        return;
    }
    if (guidKey!="") {
        onGuidClick();
        return;
    }
    onHideShadow();
    m_toolNode->setVisible(false);
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_MAINSCENCE_SAVEPOS);
    auto layer = dynamic_cast<ImperialScene*>(SceneController::getInstance()->getCurrentLayerByLevel(LEVEL_SCENE));
    
    if(layer)
        layer->onMoveToBuildAndOpen(m_buildingKey, TYPE_POS_UP);
    PopupViewController::getInstance()->addPopupInView(FunBuildView::create(m_buildingKey), true, true);
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(GUIDE_INDEX_CHANGE
                                                                           , CCString::createWithFormat("BU_%d_detail", buildType));
}

void FunBuildBtnsView::onBtn3Click(CCObject * pSender, Control::EventType pCCControlEvent)
{
    if (!m_initEnd) {
        return;
    }
    if (guidKey!="") {
        onGuidClick();
        return;
    }
    onHideShadow();
    m_toolNode->setVisible(false);
    if(buildType == FUN_BUILD_WATCH_TOWER){
        PopupViewController::getInstance()->addPopupInView(WatchtowerView::create(m_buildingKey), true, true);
    }
    else if(buildType == FUN_BUILD_MAIN){
//        PopupViewController::getInstance()->addPopupInView(MainCityView::create(m_buildingKey));
        int mainCityLv = FunBuildController::getInstance()->getMainCityLv();
        if (mainCityLv >= 4 /*&& mainCityLv < 5*/) {//城市增益按钮4级以上显示 城堡装扮按钮6级以上显示
            PopupViewController::getInstance()->addPopupInView(ItemStatusView::create());
        }
//        else if (mainCityLv >= 5) {
//            PopupViewController::getInstance()->addPopupInView(SkinListView::create());
//        }
    }
    else if(buildType == FUN_BUILD_WALL){
//        PopupViewController::getInstance()->addPopupInView(CityDefencePopUpView::create(), true, true);

        PopupViewController::getInstance()->addPopupInView(CityDefenseView::create(FUN_BUILD_MAIN_CITY_ID));
    }
    else if(buildType == FUN_BUILD_TAVERN){
        AllianceManager::getInstance()->openMarketView();
    }
    else if(buildType == FUN_BUILD_SCIENE){
        PopupViewController::getInstance()->addPopupInView(ScienceListView::create(m_buildingKey), true, true);
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(GUIDE_INDEX_CHANGE
                                                                               , CCString::createWithFormat("BU_%d_science", buildType));
    }
    else if(buildType == FUN_BUILD_FORGE){
        PopupViewController::getInstance()->addPopupInView(EquipSiteView::create());
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(GUIDE_INDEX_CHANGE
                                                                               , CCString::createWithFormat("BU_%d_forge", buildType));
    }
    else if(buildType == FUN_BUILD_WAR){
    }
    else if(buildType == FUN_BUILD_STABLE){
        PopupViewController::getInstance()->addPopupInView(AllianceHelpView::create());
    }else if(buildType == FUN_BUILD_CELLAR){//1.将联盟任务入口调整为仓库
//        if (AllianceDailyController::getInstance()->isSwitchOpen()) {
//            if (FunBuildController::getInstance()->getMainCityLv() >= 10) {
//                PopupViewController::getInstance()->addPopupInView(AllianceDailyPublishView::create());
//            }else{
//                CCCommonUtils::flyHint("", "", _lang_1("134068", CC_ITOA(10)));
//            }
//        }
//        else
//        {
        // liu to do 去除联盟宝藏
            PopupViewController::getInstance()->addPopupInView(UseResToolView::create(Wood));
//        }
        
    }
    else if(buildType == FUN_BUILD_SMITHY){
        PopupViewController::getInstance()->addPopupInView(AllianceWarView::create());
    }
    else if(buildType == FUN_BUILD_SACRIFICE){
       PopupViewController::getInstance()->addPopupInView(SacrificePopUpView::create());
    }
    else if(buildType == FUN_BUILD_TRAINFIELD){
    }
    else if(buildType == FUN_BUILD_FOOD || buildType == FUN_BUILD_WOOD || buildType == FUN_BUILD_IRON || buildType == FUN_BUILD_STONE){
        double effectTime = m_info->effectTime;
        double gapTime = effectTime - GlobalData::shared()->getWorldTime();
        if(gapTime<=0){;
            int resType = 0;
            switch (buildType) {
                case FUN_BUILD_FOOD:
                    resType = Food;
                    break;
                case FUN_BUILD_WOOD:
                    resType = Wood;
                    break;
                case FUN_BUILD_IRON:
                    resType = Iron;
                    break;
                case FUN_BUILD_STONE:
                    resType = Stone;
                    break;
                default:
                    break;
            }
            std::string tips = _lang_1("102294", CCCommonUtils::getResourceNameByType(resType).c_str());
            int gold = atoi(m_info->para[3].c_str());
            YesNoDialog::showButtonAndGold(tips.c_str(),CCCallFunc::create(this, callfunc_selector(FunBuildBtnsView::yesUseIncreated)),_lang("confirm").c_str(),gold);
        }
    }
    else if(buildType == FUN_BUILD_HOSPITAL){
        int t_qid = QueueController::getInstance()->getMinTimeQidByType(TYPE_HOSPITAL);
        if (t_qid != QID_MAX) {
            PopupViewController::getInstance()->addPopupInView(HealthingView::create(m_buildingKey));
        }
        else {
            auto& info = FunBuildController::getInstance()->getFunbuildById(m_buildingKey);
            PopupViewController::getInstance()->addPopupInView(BuildingHospitalPopUpView::create(info));
        }
    }
    else if(buildType == FUN_BUILD_BARRACK1 || buildType == FUN_BUILD_BARRACK2 || buildType == FUN_BUILD_BARRACK3 || buildType == FUN_BUILD_BARRACK4 || buildType == FUN_BUILD_FORT){
        map<int, QueueInfo>::iterator it;
        auto curTime = GlobalData::shared()->getWorldTime();
        int btype = buildType;
        int qtype = 0;
        switch (btype) {
            case FUN_BUILD_BARRACK1:
                qtype = TYPE_FORCE;
                break;
            case FUN_BUILD_BARRACK2:
                qtype = TYPE_RIDE_SOLDIER;
                break;
            case FUN_BUILD_BARRACK3:
                qtype = TYPE_BOW_SOLDIER;
                break;
            case FUN_BUILD_BARRACK4:
                qtype = TYPE_CAR_SOLDIER;
                break;
            case FUN_BUILD_FORT:
                qtype = TYPE_FORT;
                break;
            default:
                qtype = TYPE_FORCE;
                break;
        }
        bool flag = false;
        bool m_isFort = false;
        std::string open_arms = "";
        if(m_buildingKey!=-1){
            int buildId = FunBuildController::getInstance()->getMaxLvBuildByType(m_buildingKey/1000);// 423000 FUN_BUILD_BARRACK1
            if (buildId>0) {
                FunBuildInfo& info = FunBuildController::getInstance()->getFunbuildById(buildId);
                open_arms = info.open_arms;
            }
            m_isFort = buildType == FUN_BUILD_FORT;
        }
        std::vector<std::string> armys;
        CCCommonUtils::splitString(open_arms,"|",armys);
        int size = armys.size();
        std::vector<std::string> armyVector;
        std::string armyId = "";
        if(m_isFort){
            for (int i=0; i<size; i++) {
                armyVector.clear();
                CCCommonUtils::splitString(armys[i],";",armyVector);
                map<std::string, ArmyInfo>::iterator it = GlobalData::shared()->fortList.find(armyVector[1]);
                if(it!=GlobalData::shared()->fortList.end() && it->second.finishTime!=0){
                    armyId = it->second.armyId;
                    break;
                }
            }
        }else{
            for (int i=0; i<size; i++) {
                armyVector.clear();
                CCCommonUtils::splitString(armys[i],";",armyVector);
                map<std::string, ArmyInfo>::iterator it = GlobalData::shared()->armyList.find(armyVector[1]);
                if(it!=GlobalData::shared()->armyList.end() && it->second.finishTime!=0){
                    armyId = it->second.armyId;
                    break;
                }
            }
        }

        if(!flag){
            CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(GUIDE_INDEX_CHANGE
                                                                                   , CCString::createWithFormat("BU_%d_soldier", buildType));
            PopupViewController::getInstance()->addPopupInView(ProductionSoldiersView::create(m_buildingKey));
        }
    }
}

void FunBuildBtnsView::yesUseIncreated(){
    m_info->effectTime = GlobalData::shared()->getWorldTime()+24*60*60-1;
    BuildingAddOutPutCommand* cmd = new BuildingAddOutPutCommand(m_info->uuid,m_buildingKey);
    cmd->sendAndRelease();
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_BUILD_FUNCTION_EFFECT,CCInteger::create(m_buildingKey));
}

void FunBuildBtnsView::onBtn4Click(CCObject * pSender, Control::EventType pCCControlEvent)
{
    if (!m_initEnd) {
        return;
    }
    if (guidKey!="") {
        onGuidClick();
        return;
    }
    m_toolNode->setVisible(false);
    if(buildType == FUN_BUILD_MAIN){
        int mainCityLv = FunBuildController::getInstance()->getMainCityLv();
        if (mainCityLv >= 5) {//城市增益按钮4级以上显示 城堡装扮按钮6级以上显示
            PopupViewController::getInstance()->addPopupInView(ItemStatusView::create());
            
        }
    }
    else if(buildType == FUN_BUILD_FOOD || buildType == FUN_BUILD_WOOD || buildType == FUN_BUILD_IRON || buildType == FUN_BUILD_STONE){
        if (m_btn4->isEnabled()) {
            FunBuildController::getInstance()->startGetFunBuildProduct(m_info->itemId);
        }else {
            return;
        }
    }else if(buildType == FUN_BUILD_STABLE){
        PopupViewController::getInstance()->addPopupInView(YuanJunDetailView::create()); //YuanJunView
    }
    else if(buildType == FUN_BUILD_FORGE){
        PopupViewController::getInstance()->addPopupInView(EquipmentBagView::create());
    }
    else if(buildType == FUN_BUILD_CELLAR){
        
        PopupViewController::getInstance()->addPopupInView(UseResToolView::create(Wood));
        
    }
    onHideShadow();
}

void FunBuildBtnsView::onBtn5Click(CCObject * pSender, Control::EventType pCCControlEvent)
{
    if (!m_initEnd) {
        return;
    }
    if (guidKey!="") {
        onGuidClick();
        return;
    }
    
    if (m_toolInfoId<=0) {
        return;
    }
    auto& m_toolInfo = ToolController::getInstance()->getToolInfoById(m_toolInfoId);
    
    if (m_toolInfo.type == ITEM_TYPE_SPD) {//加速道具
        if(m_qid != QID_MAX) {
            PropSpeedupView::show(getSPDTypeByBuildingState(), m_buildingKey, m_qid);
            /*
             if (m_info->state == FUN_BUILD_NORMAL) {
                QueueController::getInstance()->startCCDQueue(m_qid, CC_ITOA(m_toolInfo->itemId), false);
                m_toolInfo->cnt -=1;
                CCCommonUtils::flyHint(CCCommonUtils::getIcon(CC_ITOA(m_toolInfo->itemId)), "", _lang_1("104909",_lang(m_toolInfo->name).c_str()));
            }
            else {
                if (FunBuildController::getInstance()->costCD(m_buildingKey, CC_ITOA(m_toolInfo->itemId)) ) {
                    CCCommonUtils::flyHint(CCCommonUtils::getIcon(CC_ITOA(m_toolInfo->itemId)), "", _lang_1("104909",_lang(m_toolInfo->name).c_str()));
                }
            }
             */
        }
    }
    else {//资源加速生产道具
        m_info->effectTime = GlobalData::shared()->getWorldTime()+24*60*60-1;
        BuildingAddOutPutCommand* cmd = new BuildingAddOutPutCommand(m_info->uuid,m_buildingKey, m_toolInfo.uuid);
        cmd->sendAndRelease();
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_BUILD_FUNCTION_EFFECT,CCInteger::create(m_buildingKey));
        int tCnt = m_toolInfo.getCNT() - 1;
        m_toolInfo.setCNT(tCnt);
        CCCommonUtils::flyHintWithDefault(CCCommonUtils::getIcon(CC_ITOA(m_toolInfo.itemId)), "", _lang_1("104909",m_toolInfo.getName().c_str()));
    }
    
    m_toolNode->setVisible(false);
    onHideShadow();
}

void FunBuildBtnsView::onHideShadow(bool st)
{
    this->setVisible(false);
    auto layer = dynamic_cast<ImperialScene*>(SceneController::getInstance()->getCurrentLayerByLevel(LEVEL_SCENE));
    if(layer)
    {
        layer->stopShadow(m_buildingKey,NULL,NULL);
        if (st) {
            layer->onResetLastBuildId();
        }
    }
}

void FunBuildBtnsView::onHideBtn()
{
    m_toolNode->setVisible(false);
    onHideShadow(false);
}

CCNode* FunBuildBtnsView::getGuideNode(string _key)
{
    if (m_initEnd && this->isVisible() && m_toolNode->isVisible() && m_toolNode->getScale()>=1.0) {
        if(_key=="up") {
            guidKey = _key;
            return m_btn1;
        }
        else if(_key=="detail") {
            guidKey = _key;
            return m_btn2;
        }
        else if(_key=="cd") {
            if (m_btn1Node->isVisible()) {
                guidKey = _key;
                return m_btn1;
            }
            else {
                return NULL;
            }
        }
        else if(_key=="soldier") {
            guidKey = _key;
            return m_btn3;
        }
        else if(_key=="science") {
            guidKey = _key;
            return m_btn3;
        }
        else if(_key=="forge") {
            guidKey = _key;
            return m_btn3;
        }
        else if(_key=="heal") {
            guidKey = _key;
            return m_btn3;
        }
    }
    return NULL;
}

void FunBuildBtnsView::onGuidClick()
{
    if(guidKey=="up") {
        guidKey = "";
        onBtn1Click(NULL,Control::EventType::TOUCH_DOWN);
    }
    else if(guidKey=="detail") {
        guidKey = "";
        onBtn2Click(NULL,Control::EventType::TOUCH_DOWN);
    }
    else if(guidKey=="cd") {
        guidKey = "";
        onBtn1Click(NULL,Control::EventType::TOUCH_DOWN);
    }
    else if(guidKey=="soldier") {
        guidKey = "";
        onBtn3Click(NULL,Control::EventType::TOUCH_DOWN);
    }
    else if(guidKey=="science") {
        guidKey = "";
        onBtn3Click(NULL,Control::EventType::TOUCH_DOWN);
    }
    else if(guidKey=="forge") {
        guidKey = "";
        onBtn3Click(NULL,Control::EventType::TOUCH_DOWN);
    }
    else if(guidKey=="heal") {
        guidKey = "";
        onBtn3Click(NULL,Control::EventType::TOUCH_DOWN);
    }
}
int FunBuildBtnsView::getSPDTypeByBuildingState(){
    int speedType = 0;
    switch (m_info->state) {
        case FUN_BUILD_NORMAL:{
            switch (buildType) {
                case FUN_BUILD_BARRACK1:
                case FUN_BUILD_BARRACK2:
                case FUN_BUILD_BARRACK3:
                case FUN_BUILD_BARRACK4:{
                    speedType = ItemSpdMenu_Soldier;
                    break;
                }
                case FUN_BUILD_FORT:{
                    speedType = ItemSpdMenu_Trap;
                    break;
                }
                case FUN_BUILD_SCIENE:{
                    speedType = ItemSpdMenu_Science;
                    break;
                }
                case FUN_BUILD_HOSPITAL:{
                    speedType = ItemSpdMenu_Heal;
                    break;
                }
                case FUN_BUILD_FORGE:{
                    speedType = ItemSpdMenu_DuanZao;
                    break;
                }
                default:
                    break;
            }
            break;
        }
        case FUN_BUILD_UPING:
        case FUN_BUILD_CREATE:
        case FUN_BUILD_INIT:{
            speedType = ItemSpdMenu_City;
            break;
        }
        default:
            break;
    }
    return speedType;
}
bool FunBuildBtnsView::onUpdateToolBtn(int type)
{
    int toolId = 0;
    if (type == 0) {//加速
        toolId = ToolController::getInstance()->getSPDItem(getSPDTypeByBuildingState());
    }else if (type == FUN_BUILD_WOOD) {//木
        toolId = 200027;
    }else if (type == FUN_BUILD_FOOD) {//粮
        toolId = 200028;
    }else if (type == FUN_BUILD_IRON) {//铁
        toolId = 200029;
    }else if (type == FUN_BUILD_STONE) {//石
        toolId = 200030;
    }else {
        return false;
    }
    if(toolId <=0 )
        return false;
    
    auto& toolInfo = ToolController::getInstance()->getToolInfoById(toolId);
    if (toolInfo.getCNT()<=0) {
        return false;
    }
    
    m_toolInfoId = toolId;
    m_icon5Node->removeAllChildren();
    auto icon = CCLoadSprite::createSprite(CCCommonUtils::getIcon(CC_ITOA(toolId)).c_str());
    CCCommonUtils::setSpriteMaxSize(icon, 90, true);
    m_icon5Node->addChild(icon);
    m_btn5Node->setVisible(true);
    m_btn5->setEnabled(true);
    if (toolInfo.type == ITEM_TYPE_SPD) {
        m_name5->setString("");
        m_msg5->setString(_lang("104903"));
        //m_msgV5->setString("1"+_lang("105591"));
        m_msgV5->setString("");
        if (m_info->level <= 3 && m_questType == 1) {
            auto btnEffect = FunBuildBtnsEffect::create();
            m_icon5Node->addChild(btnEffect);
        }
    }
    else {
        m_name5->setString(_lang_1("104907", CC_ITOA(toolInfo.getCNT())));
        m_msg5->setString(_lang("102293"));
        m_msgV5->setString("1d");
    }
    
    return true;
}

FunBuildBtnsEffect *FunBuildBtnsEffect::create(){
    FunBuildBtnsEffect *ret = new FunBuildBtnsEffect();
    if(ret && ret->init()){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool FunBuildBtnsEffect::init(){
    auto bg = CCBLoadFile("BuildBtnEffect", this, this);
    this->getAnimationManager()->runAnimationsForSequenceNamed("Default Timeline");
    return true;
}
bool FunBuildBtnsEffect::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode){
    return false;
}
SEL_CCControlHandler FunBuildBtnsEffect::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){
    return NULL;
}
