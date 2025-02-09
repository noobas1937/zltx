//
//  BuildListView.cpp
//  IF
//
//  Created by fubin on 14-5-19.
//
//

#include "BuildListView.h"
#include "PopupViewController.h"
#include "LocalController.h"
#include "CCCommonUtils.h"
#include "SceneController.h"

#include "FunBuildController.h"
#include "ScienceView.h"
#include "YesNoDialog.h"
#include "UIComponent.h"

#include "ScienceListView.h"
#include "WatchtowerView.h"
#include "MainCityView.h"
#include "BuildingHospitalPopUpView.h"
#include "AllianceMarketView.h"
#include "BattleView.h"
#include "QueueController.h"
#include "ToolController.h"
#include "WorldController.h"
#include "ImperialScene.h"
#include "UseCDToolView.h"
#include "ProductionSoldiersView.h"
#include "GuideController.h"
#include "QuestController.h" 
#include "IFNormalSkNode.h"

BuildListView* BuildListView::create(int pos){
    BuildListView* ret = new BuildListView();
    if(ret && ret->init(pos)){
        ret->autorelease();
    }else{
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool BuildListView::init(int pos)
{
    if (!ArcPopupBaseView::init(TYPE_POS_UP)) {
        return false;
    }
    setIsHDPanel(true);
    CCLoadSprite::doResourceByCommonIndex(511, true);
    setCleanFunction([](){
        CCLoadSprite::doResourceByCommonIndex(511, false);
    });
    auto size = CCDirector::sharedDirector()->getWinSize();
    setContentSize(size);
    
    CCBLoadFile("OptBuildView02",this,this);
    m_handBg->setVisible(false);
    auto _bg = CCLoadSprite::createSprite("frame_04.png");
    if(_bg)
    {
        auto _size = _bg->getContentSize();
        if(m_buildBG)
        {
            m_buildBG->setCapInsets(CCRect(_size.width/2,_size.height/2, 1, 1));
        }
        if(m_buildBG_2)
        {
            m_buildBG_2->setCapInsets(CCRect(_size.width/2,_size.height/2, 1, 1));
        }
        
    }
    updateInfo(pos);

    m_openNum = 0;
    return true;
}

CCNode* BuildListView::getGuideNode(string _key)
{
    if (_key=="build") {
        return m_upBtn;
    }
    else {
        if(m_initEnd) {
            gBuildId = atoi(_key.c_str());
//            this->getAnimationManager()->runAnimationsForSequenceNamed("Show");
//            return m_guidNode;
            
            int idx = -1;
            for(int i=0; i<m_buildIds.size(); i++) {
                if (m_buildIds[i] == gBuildId) {
                    idx = i;
                    break;
                }
            }
            if (idx>0) {
                int curIdx = m_buildIds.size()-1;//m_arcScroll->m_currentIndex;
                bool up = false;
                if (idx > curIdx) {
                    up = true;
                }
                int moveCnt = abs(curIdx-idx);
                for (int i=0; i < moveCnt; i++) {
//                    m_arcScroll->moveOneCell(up);
                }
                refeash(idx);
//                return m_arcScroll->getShowAreaByPos(idx);
                CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(GUIDE_INDEX_CHANGE
                                                                                       , CCString::createWithFormat("LI_%d",gBuildId));
            }
        }
    }
    return NULL;
}

void BuildListView::updateInfo(int pos)
{
    int willBuild = GuideController::share()->getWillBuildItemId();
    m_pos = pos;
    if (1) {
        m_upBtnMsgLabel->setString(_lang("102277"));
        m_btnNode1->setVisible(true);
        m_btnNode1->setScale(0);
        CCActionInterval * scaleto = CCScaleTo ::create(0.3, 1);
        CCActionInterval * easeElasticIn= CCEaseElasticIn::create(scaleto);
        m_btnNode1->runAction(easeElasticIn);
    }
    
    int cityLv = FunBuildController::getInstance()->getMainCityLv();
    int tmpCityLv = 0;
    m_buildLimitMap.clear();
    
    int cellCnt = 0;
    vector<string> btnNames;
    vector<string> btnIcons;
    string tmpbuilds = FunBuildController::getInstance()->getBuildByPos(pos);
    tmpbuilds = FunBuildController::getInstance()->orderBuild(tmpbuilds);
    std::vector<std::string> buildIds;
    CCCommonUtils::splitString(tmpbuilds, ";", buildIds);
    for(int i=buildIds.size()-1;i>=0;i--)
    {
        bool _tb = true;
        int tmpItemId = atoi(buildIds[i].c_str());
        int limitNum = atoi(CCCommonUtils::getPropById(CC_ITOA(tmpItemId), "num").c_str());
        tmpCityLv = 0;
        
        //加入由大本等级来限制 个别建筑的 建造数量
        string limitNumInfo = CCCommonUtils::getPropById(CC_ITOA(tmpItemId), "level_control");
        if (limitNumInfo != "")
        {
            vector<string> tmpVec;
            CCCommonUtils::splitString(limitNumInfo, "|", tmpVec);
            for (int j=0; j<tmpVec.size(); j++)
            {
                vector<string> limitCellVec;
                CCCommonUtils::splitString(tmpVec[j], ";", limitCellVec);
                if (limitCellVec.size() == 2) {
                    int curTmpLv = atoi(limitCellVec[0].c_str());
                    if (curTmpLv>tmpCityLv && curTmpLv<=cityLv) {
                        tmpCityLv = curTmpLv;
                        limitNum = atoi(limitCellVec[1].c_str());
                    }
                }
            }
        }
        //end
        m_buildLimitMap[tmpItemId] = limitNum;
        
        string name = CCCommonUtils::getNameById(CC_ITOA(tmpItemId));
        string pic = CCCommonUtils::getPropById(CC_ITOA(tmpItemId), "pic");
        pic = pic+"_" + CC_ITOA(GlobalData::shared()->contryResType)+".png";
        if (limitNum>0) {
            int curNum = FunBuildController::getInstance()->getBuildNumByType(tmpItemId);
            if (curNum >= limitNum && limitNumInfo == "") {
                _tb=false;
            }
        }
        
        if (_tb) {
            cellCnt++;
            btnNames.push_back(name);
            btnIcons.push_back(pic);
            m_buildIds.push_back(tmpItemId);
        }
    }
    
    int showPos = cellCnt-1;
    CCArray* arr = CCArray::create();
    for(int i=0;i<cellCnt;i++){
        
        auto dict = LocalController::shared()->DBXMLManager()->getObjectByKey(CC_ITOA(m_buildIds[i]));
        auto buildInfo = FunBuildInfo(dict);
        bool isLock = false;
        if (!buildInfo.isUnLock()) {
            isLock = true;
            
            string tmpBuildings = _lang("102130")+" ";
            int tmpIdx = 0;
            std::vector<std::string> lockItems;
            CCCommonUtils::splitString(buildInfo.building, "|", lockItems);
            for(int i=0; i<lockItems.size(); i++)
            {
                std::vector<std::string> tinyItems;
                CCCommonUtils::splitString(lockItems[i], ";", tinyItems);
                int tmpType = atoi(tinyItems[0].c_str());
                int tmLv = atoi(tinyItems[1].c_str());
                if(!FunBuildController::getInstance()->isExistBuildByTypeLv(tmpType, tmLv))
                {
                    if (tmpIdx>0) {
                        tmpBuildings +=",";
                    }
                    string tmpName = CCCommonUtils::getNameById(CC_ITOA(tmpType));
                    tmpBuildings += tmpName+" "+_lang_1("102272", CC_ITOA(tmLv));
                    tmpIdx++;
                }
            }
            m_buildLockInfos[i] = tmpBuildings;
        }
        else {
            m_buildLockInfos[i] = "";
        }
        
        if (willBuild>0 && willBuild==m_buildIds[i]) {
            showPos = i;
        }
        
        ArcInfo* info = new ArcInfo(i,btnNames[i],btnIcons[i], isLock);
        info->maxIconSize = 100;
        int limitNum = atoi(CCCommonUtils::getPropById(CC_ITOA(m_buildIds[i]), "num").c_str());
        string limitNumInfo = CCCommonUtils::getPropById(CC_ITOA(m_buildIds[i]), "level_control");
        if (limitNum>0) {
            int curNum = FunBuildController::getInstance()->getBuildNumByType(m_buildIds[i]);
            if (curNum >= limitNum && limitNumInfo != "") {
                info->isGrey = true;
            }
        }
        arr->addObject(info);
        info->release();
    }
    m_arcScroll = ArcScrollView::create(arr, 2, showPos, 0, 1);//arr->count()-1
    m_arcScroll->setCallback(this, callfunc_selector(BuildListView::arcButtonClick));
    m_arcNode->addChild(m_arcScroll);
    
    refeash(showPos);
}

void BuildListView::refeash(int idx)
{
    if (m_buildIds.size()<=0 || idx >= m_buildIds.size()) {
        return;
    }
    m_curIdx = idx;
    m_itemId = m_buildIds[idx];
    auto layer = dynamic_cast<ImperialScene*>(SceneController::getInstance()->getCurrentLayerByLevel(LEVEL_SCENE));
    if( layer )
    {
        layer->hideTmpBuild(m_pos);
        layer->showTmpBuild(m_pos, m_buildIds[idx]);
    }
    string name = CCCommonUtils::getNameById(CC_ITOA(m_buildIds[idx]));
    string des = CCCommonUtils::getPropById(CC_ITOA(m_buildIds[idx]), "destip");
    des = _lang(des);
    m_nameLabel->setString(name);
    CCActionInterval *textScaleto = CCScaleTo ::create(0.3, 1);
    CCActionInterval *easeElasticIn= CCEaseElasticIn::create(textScaleto);
    m_titleLvNode->setScaleX(0);
    m_titleLvBG->runAction(CCFadeIn::create(0.5));
    m_nameLabel->runAction(CCFadeIn::create(0.5));
    m_titleLvNode->runAction(easeElasticIn);
    
    m_desLabel->setString(des);
    m_desLabel->runAction(CCFadeIn::create(0.5));
    
    float tmpY = m_desLabel->getPositionY()-m_desLabel->getContentSize().height*m_desLabel->getOriginScaleY();
    m_lockLabel->setPositionY(tmpY);
    m_lockLabel->setString(m_buildLockInfos[idx]);
    m_lockLabel->runAction(CCFadeIn::create(0.5));
    
    int limitNum = atoi(CCCommonUtils::getPropById(CC_ITOA(m_buildIds[idx]), "num").c_str());
    if (m_buildLimitMap.find(m_buildIds[idx]) != m_buildLimitMap.end()) {
        limitNum = m_buildLimitMap[ m_buildIds[idx] ];
    }
    
    if (limitNum>1) {
        tmpY = tmpY-m_lockLabel->getContentSize().height*m_lockLabel->getOriginScaleY();
        m_numLabel->setPositionY(tmpY);
        int curNum = FunBuildController::getInstance()->getBuildNumByType(m_buildIds[idx]);
        m_numLabel->setString(_lang_2("102167", CC_ITOA(curNum), CC_ITOA(limitNum)));
        m_numLabel->runAction(CCFadeIn::create(0.5));
        
        if (curNum >= limitNum) {
            m_upBtn->setEnabled(false);
        }
        else {
            m_upBtn->setEnabled(true);
        }
        
        if (m_buildLockInfos[idx] != "") {
            m_numLabel->setString("");
        }
    }
    else {
        m_numLabel->setString("");
        m_upBtn->setEnabled(true);
    }
    
    if((m_questId=="2201001" || m_questId=="2102601") && m_pos>16 && m_pos<52)
    {
        if (m_questId == "2201001"){
            if (m_itemId==FUN_BUILD_BARRACK) {
                m_upBtn->setEnabled(true);
                m_handParNode->removeAllChildren();
                this->getAnimationManager()->runAnimationsForSequenceNamed("Loop");
                m_handBg->setVisible(false);
                m_npcNode->setVisible(false);
            }else {
                m_upBtn->setEnabled(false);
                m_handBg->setVisible(true);
                showHand();
            }
        }
        if (m_questId == "2102601") {
            if (m_itemId==FUN_BUILD_WOOD) {
                m_upBtn->setEnabled(true);
                m_handParNode->removeAllChildren();
                this->getAnimationManager()->runAnimationsForSequenceNamed("Loop");
                m_handBg->setVisible(false);
                m_npcNode->setVisible(false);
            }else {
                m_upBtn->setEnabled(false);
                m_handBg->setVisible(true);
                showHand();
            }
        }
    }
}

void BuildListView::arcButtonClick()
{
//    if (gBuildId>0) {
//        int idx = m_arcScroll->m_currentIndex;
//        if (m_buildIds.size()<=0 || idx >= m_buildIds.size()) {
//            return;
//        }
//        m_itemId = m_buildIds[idx];
//        if (gBuildId != m_itemId) {
//            int fidx = -1;
//            for(int i=0; i<m_buildIds.size(); i++) {
//                if (m_buildIds[i] == gBuildId) {
//                    fidx = i;
//                    break;
//                }
//            }
//            if (fidx>0) {
//                m_arcScroll->m_currentIndex = idx;
//                m_itemId = gBuildId;
//                m_arcScroll->setShowPos(fidx);
//                return ;
//            }
//        }
//    }
    
    refeash(m_arcScroll->m_currentIndex);
    if(gBuildId == m_itemId) {
        m_handBg->setVisible(false);
        this->getAnimationManager()->runAnimationsForSequenceNamed("Loop");
        CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(GUIDE_INDEX_CHANGE
                                                                               , CCString::createWithFormat("LI_%d",m_itemId));
    }
}

SEL_CCControlHandler BuildListView::onResolveCCBCCControlSelector(cocos2d::CCObject *pTarget, const char *pSelectorName){
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onCreateOrUpClick", BuildListView::onCreateOrUpClick);
    return NULL;
}

bool BuildListView::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode){
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_desLabel", CCLabelIF*, this->m_desLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_lockLabel", CCLabelIF*, this->m_lockLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_numLabel", CCLabelIF*, this->m_numLabel);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_titleLvNode", CCNode*, this->m_titleLvNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_titleLvBG", Sprite*, this->m_titleLvBG);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameLabel", CCLabelIF*, this->m_nameLabel);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_handParNode", CCNode*, this->m_handParNode);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_arcNode", CCNode*, this->m_arcNode);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_btnNode1", CCNode*, this->m_btnNode1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_upNode", CCNode*, this->m_upNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_moveNode", CCNode*, this->m_moveNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_guidNode", CCNode*, this->m_guidNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_upBtn", CCControlButton*, this->m_upBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_upBtnMsgLabel", CCLabelIF*, this->m_upBtnMsgLabel);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_spineNode", CCNode*, this->m_spineNode);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_npcNode", CCNode*, this->m_npcNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_nameText", CCLabelIF*, this->m_nameText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_contentText", CCLabelIF*, this->m_contentText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_handBg", CCSprite*, this->m_handBg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_buildBG", CCScale9Sprite*, this->m_buildBG);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "m_buildBG_2", CCScale9Sprite*, this->m_buildBG_2);
    
    return false;
}

bool BuildListView::onTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
    if (isTouchInside(m_upNode, pTouch)) {
        m_upBtn->onTouchBegan(pTouch, pEvent);
    }
    return true;
}

void BuildListView::onTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent) {
    
    if (isTouchInside(m_upNode, pTouch) && m_upBtn->isEnabled()) {
        m_upBtn->onTouchEnded(pTouch, pEvent);
    }
    else if (m_arcScroll->m_isInTouch) {
    }
    else {
       // onCloseView();
    }
}

void BuildListView::onEnter(){
    CCNode::onEnter();
//    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, Touch_Default, true);
//    UIComponent::getInstance()->hideReturnBtn();
    setSwallowsTouches(true);
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    setTouchEnabled(true);
    if(m_openNum>0) {
        auto layer = dynamic_cast<ImperialScene*>(SceneController::getInstance()->getCurrentLayerByLevel(LEVEL_SCENE));
        if( layer )
            layer->onMoveToBuildAndOpen(m_pos,TYPE_POS_UP);
    }
    m_openNum++;
    m_questId = "init";
    scheduleOnce(schedule_selector(BuildListView::onInitEnd), 1.0f);
}

void BuildListView::onExit(){
    setTouchEnabled(false);
    m_initEnd = false;
    CCNode::onExit();
}

void BuildListView::onInitEnd(float _time)
{
    m_npcNode->setVisible(false);
    m_initEnd = true;
    m_questId = "";
    m_itemIdx = -1;
    QuestInfo *info = QuestController::getInstance()->getRecommendQuest(false);
    if(info && (/*info->itemId=="2201001" || */info->itemId=="2102601") && m_pos>16 && m_pos<52) {
//        this->getAnimationManager()->setAnimationCompletedCallback(this, callfunc_selector(BuildListView::AnimationCallback));
        this->getAnimationManager()->setAnimationCompletedCallback(this, callfunc_selector(BuildListView::AnimationCallbackEnd));
        if (info->itemId=="2201001") {
            m_contentText->setString(_lang("106034"));
            for (int i=0; i<m_buildIds.size(); i++) {
                if (m_buildIds[i] == FUN_BUILD_BARRACK) {
                    m_itemIdx = i;
                    break;
                }
            }
        }else if (info->itemId=="2102601") {
            m_contentText->setString(_lang("106033"));
            for (int i=0; i<m_buildIds.size(); i++) {
                if (m_buildIds[i] == FUN_BUILD_WOOD) {
                    m_itemIdx = i;
                    break;
                }
            }
        }
        showHand();
        m_questId = info->itemId;
    }
}

void BuildListView::AnimationCallback()
{
    string aniName = this->getAnimationManager()->getLastCompletedSequenceName();
    if (aniName == "Show") {
        this->getAnimationManager()->setAnimationCompletedCallback(this, callfunc_selector(BuildListView::AnimationCallbackEnd));
        this->getAnimationManager()->runAnimationsForSequenceNamed("Show");
    }
}

void BuildListView::AnimationCallbackEnd() {
    
}

void BuildListView::showHand()
{
    m_handParNode->removeAllChildren();
    m_upBtn->setEnabled(false);
    CCLoadSprite::doResourceByGeneralIndex(100, true);
    
    ////spine动画
    m_spineNode->removeAllChildren();
    
    auto per1Spr = CCSprite::create();
    CCCommonUtils::makeEffectSpr(per1Spr, "guide_hand_%d.png", 4, 0.1);
    per1Spr->setAnchorPoint(Vec2(0.5,1));
    per1Spr->setPosition(Vec2(-30,-50));
    per1Spr->setRotation(60);
    m_handParNode->addChild(per1Spr);
    
    // if (CCFileUtils::sharedFileUtils()->isFileExist("Spine/UIComponent/guide_woman.json"))
    // {
    //     auto chilun = new IFSkeletonAnimation("Spine/UIComponent/guide_woman.json", "Common/Common_511.atlas");
    //     if (chilun) {
    //         {
    //             chilun->setVisibleStop(false);
                
    //             chilun->setPositionX(0);
    //             chilun->setPositionY(0);
    //             m_spineNode->setScaleX(-1);
    //             m_spineNode->addChild(chilun,3);
    //             chilun->setAnimation(0.01, "animation", true);
                
    //         }
    //     }
    // }

    
    m_handBg->setVisible(true);
    if (m_curIdx > m_itemIdx) {
        this->getAnimationManager()->runAnimationsForSequenceNamed("Show");
    }else {
        this->getAnimationManager()->runAnimationsForSequenceNamed("Show_3");
    }
    if(!m_npcNode->isVisible()) {
        m_npcNode->setPosition(ccp(-650, -140));
        m_npcNode->setVisible(true);
        m_npcNode->runAction(CCMoveTo::create(0.3, ccp(-22, -140)));
    }
}

void BuildListView::onCreateOrUpClick(cocos2d::CCObject *pSender, Control::EventType pCCControlEvent)
{
    if (!m_arcScroll->getAniState()) {
        CCLOG("fubin .....fubin .....fubin .....fubin .....fubin .....");
        return;
    }
    
    if (m_questId == "init") {
        return;
    }
    if (m_questId == "2201001" && m_itemId!=FUN_BUILD_BARRACK){
        return;
    }
    if (m_questId == "2102601" && m_itemId!=FUN_BUILD_WOOD) {
        return;
    }
    
    auto layer = dynamic_cast<ImperialScene*>(SceneController::getInstance()->getCurrentLayerByLevel(LEVEL_SCENE));
    if( layer )
        layer->onMoveToBuildAndOpen(m_pos, TYPE_POS_UP);
    PopupViewController::getInstance()->addPopupInView(BuildUpgradeView::create(m_itemId, m_pos));
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(GUIDE_INDEX_CHANGE
                                                                           , CCString::createWithFormat("LI_build"));
}

void BuildListView::onCloseView()
{
    CCSafeNotificationCenter::sharedNotificationCenter()->postNotification(MSG_MAINSCENCE_GOBACK);
    auto layer = dynamic_cast<ImperialScene*>(SceneController::getInstance()->getCurrentLayerByLevel(LEVEL_SCENE));
    if( layer )
        layer->hideTmpBuild(m_pos);
    PopupViewController::getInstance()->removePopupView(this);
}
