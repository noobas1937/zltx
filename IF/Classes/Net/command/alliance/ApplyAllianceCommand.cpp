//
//  ApplyAllianceCommand.cpp
//  IF
//
//  Created by ganxiaohua on 14-3-25.
//
//

#include "ApplyAllianceCommand.h"
#include "NetController.h"
#include "AllianceManager.h"
#include "GlobalData.h"
#include "ActivityController.h"
#include "ChatServiceCocos2dx.h"
#include "ChatController.h"
#include "SceneController.h"
#include "WorldMapView.h"

ApplyAllianceCommand::ApplyAllianceCommand(std::string allianceId)
: CommandBase(APPLY_ALLIANCE),m_allianceId(allianceId)
{
    putParam("allianceId", CCString::create(m_allianceId.c_str()));
}

bool ApplyAllianceCommand::handleRecieve(cocos2d::CCDictionary *dict)
{
    if (dict->valueForKey("cmd")->compare(APPLY_ALLIANCE) != 0)
        return false;
    
    CCDictionary *params=_dict(dict->objectForKey("params"));
    
    if (!params) {
        return false;
    }
    const CCString *pStr = params->valueForKey("errorCode");
    if (pStr->compare("")!=0) {
        if(pStr->compare("EE000000")==0){
            //无效处理，不让提给玩家
        }else{
            
            if (params->objectForKey("name") && pStr->compare("115463") == 0) {
                CCCommonUtils::flyText(_lang_1(pStr->getCString(), params->valueForKey("name")->getCString()));
            } else {
                CCCommonUtils::flyText(_lang(pStr->getCString()));
            }
        }
    }else{
        auto allianceDic = dynamic_cast<CCDictionary*>(params->objectForKey("alliance"));
        if(allianceDic){//这只有是公开的，加进联盟成功后就会把联盟信息带过来了，非公开的，会有join 字段
            GlobalData::shared()->playerInfo.allianceInfo.updateAllianceInfo(allianceDic);
            GlobalData::shared()->playerInfo.setAllianceId(GlobalData::shared()->playerInfo.allianceInfo.uid);
             if(SceneController::getInstance()->currentSceneId==SCENE_ID_WORLD && WorldMapView::instance()){
                 WorldMapView::instance()->m_map->updateDynamicMap();
             }
        }
        map<std::string, AllianceInfo*>::iterator it = AllianceManager::getInstance()->allianceList.find(m_allianceId);
        if(it!=AllianceManager::getInstance()->allianceList.end()){
            it->second->join = params->valueForKey("join")->boolValue();
            if(!it->second->join){
                 it->second->isApplied = true;
            }else{
                if(allianceDic){
                    AllianceManager::getInstance()->allianceList.erase(it);
                }
            }
        }
        
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        ChatServiceCocos2dx::setPlayerAllianceInfo(GlobalData::shared()->playerInfo.allianceInfo.shortName.c_str(),GlobalData::shared()->playerInfo.getAllianceId().c_str(),GlobalData::shared()->playerInfo.allianceInfo.rank ,GlobalData::shared()->playerInfo.isfirstJoin);
        string param = ChatServiceCocos2dx::getChannelInfo();
        ChatController::getInstance()->getNewMsg(param);
#elif(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        ChatServiceCocos2dx::setPlayerAllianceInfo(GlobalData::shared()->playerInfo.allianceInfo.shortName.c_str(),GlobalData::shared()->playerInfo.getAllianceId().c_str(),GlobalData::shared()->playerInfo.allianceInfo.rank ,GlobalData::shared()->playerInfo.isfirstJoin);
#endif
        callSuccess(NetResult::create());
    }
    GameController::getInstance()->removeWaitInterface();
    return true;
}