//
//  GuideController.h
//  IF
//
//  Created by ganxiaohua on 13-11-6.
//
//

#ifndef __IF__GuideController__
#define __IF__GuideController__
#include "GuideView.h"
#include "UserUpgradeView.h"

#define PLOT_PLAY_OVER "plot_play_over"
#define CLEAR_ARROW "clearArrow"
#define GUIDE_STEP "guideStep"
#define GUIDE_INDEX "guideIndex"

#define GUIDE_TROOP_CRT_ST "3073200"
#define GUIDE_TROOP_CRT_ED "3073500"
#define GUIDE_TROOP_CD_ST "3073700"
#define GUIDE_TROOP_CD_ED "3073900"
#define GUIDE_TROOP_GET "3074000"
#define GUIDE_REWD_ST "3075900"
#define GUIDE_REWD_ED "3076900"

#define GUIDE_BUILD_4 "3074200"

#define GUIDE_BATTLE_ST "3030100"
#define GUIDE_BATTLE_ED "3030800"
#define GUIDE_HAVEST_ST "3080100"

#define GUIDE_NEW_RD_ST "3130100"
#define GUIDE_NEW_RD_ED "3130400"

#define GUIDE_MERCHANTE_ST "3140100"
#define GUIDE_FIRSTSHOW_STAR "3000300"

#define MSG_BIND_GUIDE_FROM_POP_TIME "MSG_BIND_GUIDE_FROM_POP_TIME"
#define MSG_BIND_GUIDE_FROM_PAY_TIME "MSG_BIND_GUIDE_FROM_PAY_TIME"
#define MSG_CLOSING_RechargeACTVCell "MSG_CLOSING_RechargeACTVCell"
#define MSG_STOP_GUIDE "MSG_STOP_GUIDE"
enum OpenType{
    BUILDING_OPEN = 1,
    NORMAL_OPEN,
};

class GuideController:public CCNode{
public:
    CREATE_FUNC(GuideController);
    static GuideController* share();

    bool init();
    void onEnter();
    void onExit();
    void purgeData();
    bool start();
    void setGuide(std::string guideId, bool from = true);
    bool hasGuide();
    void createGuideView();
    void buildingUpdate(int buildingId);
    void willGuide(std::string guideId);
    void next(float t = 0.0);
    bool isInTutorial();
    void addLevelUpPop(UserUpgradeView *pop);
    void removeLevelUpPop();
    std::string getCurrentId();
    bool updateWorldInfo;
    void openSciencePanel();
    void startScienceGuide(CCObject* pSender, CCControlEvent event);
    int  getWillBuildItemId();
    string getCurGuideID();
    string m_netSaveStep;
    
    void clearGuideData();
    bool checkSubGuide(string guide, bool save = true);
    bool checkSubGuideAvaliable(string guide);
    void setLagStopTouch(float dt);
    
    bool IsSkip;
    bool IsStartAni;
    string TmpGuideId;
    void setGuideEnd();
    bool showBindGuide();
    void showBindGuideFromPop();//进游戏弹窗关闭触发
    void showBindGuideFromPay();//付费触发
    
    bool m_isfinishAcRwd;
private:
    void goToMainScence(CCNode* p);
    void getPositionNode(float t);
    CCNode* getNode(std::string str);
    void onEnterFrame(float dt);
    void recordStep(std::string stepId);
    bool checkStep(std::string guideId);
    void doGuide();
    void saveCurrentStepToLocal();
    void checkUserAction(CCObject *obj);
    void addGuideView(bool movingFlag = true);
    void movingCallBack(CCNode* p);
    void clearLock(float t);
    void removeGuidView(float t);
    void checkNewRdView();
    std::string checkNetStep(std::string guideId);
    std::string checkGuideStepB(std::string guideId);
    std::string checkOpenSpeGuide();
    
    std::string m_currentId;
    std::string m_willGuideId;
    CC_SYNTHESIZE_SAFEOBJECT(GuideView, m_gView, GView);
    CCNode* m_node;
    bool m_startGuide;
    int m_recordBuildingId;
    int m_offX;
    int m_offY;
    UserUpgradeView* m_panel;
    CCSafeObject<CCArray> arr;
    bool openScienceFlag;
    int tmpTime;
    
    bool m_bShowGuideEnd;
};
#endif /* defined(__IF__GuideController__) */
