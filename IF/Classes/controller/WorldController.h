//
//  WorldController.h
//  IF
//
//  Created by 邹 程 on 14-1-21.
//
//

#ifndef __IF__WorldController__
#define __IF__WorldController__

#include "CommonInclude.h"
#include <spine/Json.h>

//正常大小的地图
static const int _tile_count_x = 861;
static const int _tile_count_y = 861;
static const float _tile_width = 256;
static const float _tile_height = 128;
static const float _tile_true_height = 140;
static const float _map_width = _tile_count_x*_tile_width;
static const float _map_height = _tile_count_y*_tile_height;
static unsigned int _indexLimit = _tile_count_x*_tile_count_y;

//add boder *3
static const int _big_tilecountX = 864;
static const int _big_tilecountY = 864;
static const int _big_mapWidth = _big_tilecountX * _tile_width;
static const int _big_mapHeight = _big_tilecountY * _tile_height;
static const int _big_indexLimit = _big_tilecountX * _big_tilecountY;

static const CCSize _halfTileSize = CCSize(_tile_width/2,_tile_height/2);
static const int _limit_cross_server = 5;
static double AttackAnimationTime = 2000.0; // Millisecond
static double AttackTime = 10000.0;
static float MapGlobalScale = 0.8;
static int CapTecIndex = 229993;
static int DefaultBlankBG = 131;
static int MarchAddPerFrame = 20;
static int _relic_tile_width = 0;
static int _relic_tile_height = 0;
static int _relic_center_x = _tile_count_x/2;
static int _relic_center_y = _tile_count_y/2;
static int _throne_w = 3;
static int _throne_h = 3;

enum MapType{
    DEFAULT_MAP,        //默认地图，
    NORMAL_MAP,         //正常的地图，
    SERVERFIGHT_MAP,    //服务器战斗地图，
    DRAGON_MAP,         //龙图，
};

static const vector<std::string> _capTech = {
    "731000",
    "731600",
    "733200"
};

static const map<string,string> MarchStateName = {
    pair<string, string>("collect","108560")
    ,pair<string, string>("camp","108618")
    ,pair<string, string>("depart","108562")
    ,pair<string, string>("return","108572")
};

static const map<int,string> ThronePos = {
    pair<int, string>(301, "236,48")
    ,pair<int, string>(302, "223,38")
    ,pair<int, string>(303, "223,38")
    ,pair<int, string>(304, "242,45")
    ,pair<int, string>(309, "214,45")
};

static const vector<string> ResourceStr = {
    "108639"
    ,"108640"
    ,"108641"
    ,"108642"
    ,"108643"
    ,"108644"
};

enum FavoriteType {
    All = -1
    ,Favorite = 0
    ,Friend
    ,Enemy
};
//瓦片按钮类型
enum TileButtonState {
    ButtonNone = -1
    ,ButtonTeleport
    ,ButtonOccupy
    ,ButtonProfile
    ,ButtonScout
    ,ButtonMarch
    ,ButtonViewTroop
    ,ButtonGoHome
    ,ButtonRally
    ,ButtonEnterCity
    ,ButtonSupport
    ,ButtonResourceHelp
    ,ButtonInformation
    ,ButtonExplore
    ,ButtonInvite
    ,ButtonGoto
    ,ButtonSpeedUp
    ,ButtonRecall
    ,ButtonRalliedTroops
    ,ButtonMyKing
    ,AppointOfficals
    ,ButtonAddEffect
    ,ButtonAllianceAct
    ,ButtonWorldInviteTeleport
    ,ButtonBuilding
    ,ButtonStation
    ,ButtonRepair
    ,ButtonPlace
    ,ButtonGather
    ,ButttonTreasureMapSPD
    ,ButtonStorage
    ,ButtonRetreat
    ,ButtonFunction
    ,ButtonMarkLine
    ,ButtonOrderIcon
    ,ButtonTransKingdom
    ,ButtonKingdomWarRank
    ,ButtonDispatch//派遣
};

enum PlayerType {
    PlayerNone = -1
    ,PlayerSelf
    ,PlayerAlliance
    ,PlayerOther
};

//行军类型
enum MarchStateType {
    StateMarch//进攻
    ,StateOccupy//占据
    ,StateReturn//返回
    ,StateGarrison//驻守
};

enum MarchMethodType {
    MethodBattle
    ,MethodScout
    ,MethodBarrack
    ,MethodDeal
    ,MethodRally
    ,MethodUnion
    ,MethodFieldMonster
    ,MethodYuanSolider
    ,MethodHeiqishi
    ,MethodWarehouse
    ,MethodChristmasMarch
    ,MethodSALLCity//资源点
    ,MethodMainCity//主城
};

enum AAreaState{
    Astate_None = -1,
    Astate_Building,//建造中
    Astate_BeforeBuild,//未出兵建造
    Astate_Garrison,//已驻防
    Astate_UnGarrison,//未驻防
    Astate_Damaged,//破损-未采集
    Astate_FixIng,//修理中
    Astate_Destroy//被摧毁中-采集中
};

//世界建筑类型
enum WorldCityType {
    OriginTile//0
    ,CityTile
    ,CampTile//2 扎营地
    ,ResourceTile2// 3 资源
    ,KingTile//// 4 遗迹
    ,BattleTile// 5 塔
    ,MonsterTile// 6 地宫
    ,MonsterRange
    ,CityRange// 8 玩家周边
    ,FieldMonster // 9 野怪
    ,Throne //王座
    ,ThroneRange //王座周边
    ,Trebuchet //投石机
    ,TrebuchetRange //投石机周边
    ,Tile_allianceArea
    ,ActBossTile//活动怪物boss
    ,Tile_allianceRange//领地周边16
    ,ActBossTileRange
    ,tile_superMine//18
    ,tile_superMineRange
    ,tile_tower
    ,tile_wareHouse//21
    ,tile_wareHouseRange
    ,tile_banner//联盟国旗
    // ----- dragon building
    ,Crystal //24水晶
    ,Crystal_Range //25水晶周边
    ,Armory //26军械库
    ,Armory_Range //27军械库周边
    ,TrainingField //28训练场
    ,TrainingField_Range //29训练场周边
    ,SupplyPoint //30供给点
    ,BessingTower //31祝福塔
    ,MedicalTower //32医疗塔
    ,DragonTower //33龙塔
    ,Barracks //34兵营 骑士大厅
    ,Barracks_Range //35兵营周边
    ,TransferPoint//36传送点
    ,Resource_new //37 活动资源点
    ,Resource_newRang//38 range
    ,Cemetery/** 39 墓地 */
    ,Cemetery_Range/** 40墓地周边 */
    ,Main_City//41主城
    ,Main_City_Range//42主城周边
    ,ResourceTile//43资源田
    ,ResourceTile_Range//44资源田周边
    //以下是lua预留类型
    ,LuaItemTile = 1000 //1000 for lua use -- add by js 占位 2*2
    ,LuaItemTile_Range = 1001 // 1001
    ,LuaItemTile1 = 1002 //1002 for lua use -- add by js 占位 1*1
};

//方向
enum AAeraDir{
    Dir_None = -1,
    Dir_LeftTop,
    Dir_LeftBottom,
    Dir_RightTop,
    Dir_RightBottom,
    Dir_Left,
    Dir_Right,
    Dir_Top,
    Dir_Bottom
};

enum WorldActivityState{
    NotOpen,
    OpenNoKing,
    PeaceTime,
    WarTime,
};


enum WorldActivityType{
    FIGHT_OF_KING//国王争夺
};

//瓦片战斗状态
enum BattleTileType {
    BattleTileNone = -1
    ,BattleTileLightning
    ,BattleTileTree
    ,BattleTileBarrack
    ,BattleTileBuff
    ,BattleTileRelic
};

enum MarchEndAction {
    MarchActionNone = -1
    ,MarchActionPrepareAttack//准备攻击
    ,MarchActionAttack//攻击
    ,MarchActionCollectChest//收集箱
    ,MarchActionSetOccupy//集占据
    ,MarchActionUnsetOccupy//未占用
    ,MarchActionUnsetResource//设置资源
};

struct FavoriteTile {
    CCPoint point;
    std::string name;
    FavoriteType type;
    bool isNew;
    int serverId;
    double createTime;
    int citytype;
    int restype;
    ServerType serverType;
};

//玩家地图信息
struct PlayerTileInfo {
    unsigned int cityIndex;
    double protectStamp;
    double fireStamp;
    PlayerType type;
    unsigned int cityLv;
    int starLevel;
    string allianceShort;
    string allianceLong;
    string allianceFlag;
    double resourceProtectStamp;
    string officer;
    unsigned int userType;
    unsigned int playerVType;
    int status;
    double statusStamp;
    int endheartstamp;
    int hearttype;
};

enum {
    cityIndex_update =      1 << 0,
    protectStamp_update =   1 << 1,
    fireStamp_update =      1 << 2,
    type_update =           1 << 3,
    cityLv_update =         1 << 4,
    allianceShort_update =  1 << 5,
    allianceLong_update =   1 << 6,
    allianceFlag_update =   1 << 7,
    resourceProtectStamp_update = 1 << 8,
    officer_update =        1 << 9,
    userType_update =       1 << 10,
    status_update =         1 << 11,
    statusStamp_update =    1 << 12,
    starLevel_update =      1 << 13,
    heartstamp_update =     1 << 14,
    hearttype_update =      1 << 15,
    playerVType_update=     1 << 16,
};

#define UPDATE_CITYINDEX(t) (t) |= cityIndex_update
#define UPDATE_PROTECTSTAMP(t) (t) |= protectStamp_update
#define UPDATE_FIRESTAMP(t) (t) |= fireStamp_update
#define UPDATE_TYPE(t) (t) |= type_update
#define UPDATE_CITYLV(t) (t) |= cityLv_update
#define UPDATE_ALLIANCESHORT(t) (t) |= allianceShort_update
#define UPDATE_ALLIANCELONG(t) (t) |= allianceLong_update
#define UPDATE_ALLIANCEFLAG(t) (t) |= allianceFlag_update
#define UPDATE_RESOURCEPROTECTSTAMP(t) (t) |= resourceProtectStamp_update
#define UPDATE_OFFICER(t) (t) |= officer_update
#define UPDATE_USERTYPE(t) (t) |= userType_update
#define UPDATE_STATUS(t) (t) |= status_update
#define UPDATE_STATUSSTAMP(t) (t) |= statusStamp_update
#define UPDATE_STARLEVEL(t) (t) |= starLevel_update
#define UPDATE_HEARTSTAMP(t) (t) |= heartstamp_update
#define UPDATE_HEARTTYPE(t) (t) |= hearttype_update
#define UPDATE_PLAYERVTYPE(t) (t) |= playerVType_update

#define IS_UPDATE_CITYINDEX(t) ((t) & cityIndex_update)
#define IS_UPDATE_PROTECTSTAMP(t) ((t) & protectStamp_update)
#define IS_UPDATE_FIRESTAMP(t) ((t) & fireStamp_update)
#define IS_UPDATE_TYPE(t) ((t) & type_update)
#define IS_UPDATE_CITYLV(t) ((t) & cityLv_update)
#define IS_UPDATE_ALLIANCESHORT(t) ((t) & allianceShort_update)
#define IS_UPDATE_ALLIANCELONG(t) ((t) & allianceLong_update)
#define IS_UPDATE_ALLIANCEFLAG(t) ((t) & allianceFlag_update)
#define IS_UPDATE_RESOURCEPROTECTSTAMP(t) ((t) & resourceProtectStamp_update)
#define IS_UPDATE_OFFICER(t) ((t) & officer_update)
#define IS_UPDATE_USERTYPE(t) ((t) & userType_update)
#define IS_UPDATE_STATUS(t) ((t) & status_update)
#define IS_UPDATE_STATUSSTAMP(t) ((t) & statusStamp_update)
#define IS_UPDATE_STARLEVEL(t) ((t) & starLevel_update)
#define IS_UPDATE_HEARTSTAMP(t) ((t) & heartstamp_update)
#define IS_UPDATE_HEARTTYPE(t) ((t) & hearttype_update)
#define IS_UPDATE_PLAYERVTYPE(t) ((t) & playerVType_update)

//地图资源结构
struct ResourceTileInfo {
    WorldResourceType type;
    unsigned int tId;
    unsigned int lv;
    unsigned int sum;
    double digSpeed;
    unsigned int troopMax;
    double digStartTime;
    double refreshStamp;
    int digStartNum;
    double changeCollectSpdTime;
};
struct ResourceNewTileInfo{
    

};
//战斗地图结构
struct BattleTileInfo {
    BattleTileType type;
    string icon;
    string name;
    string desc;
    double startStamp;
    double refreshStamp;
};
//怪物地图结构
struct MonsterTileInfo{
    unsigned int tilePtDown;
    unsigned int tilePtUp;
    unsigned int tilePtLeft;
    unsigned int tilePtRight;
};

//行军士兵结构
struct MarchSoldierInfo{
    SOILDERTYPE type;
    int total;
    int dead;
};
//野外怪物结构
struct FieldMonsterInfo{
    std::string monsterId;
    int totalHp;
    int currentHp;
    bool isAppear;
    bool isHpChange;
    int christmasNum;
};

//投石车状态
enum TrebuchetState{
    Wait,
    Attack,
};
//世界活动时间机构
struct WorldActivityTimeInfo{
    double openTime;
    double startTime;
    double endTime;
};
//世界活动信息
struct WorldActivityInfo{
    WorldActivityTimeInfo timeInfo;
    WorldActivityState currentState;
};
//投石车信息结构
struct TrebuchetInfo{
    int direction;
    std::string allianceUid;
    TrebuchetState state;
    std::string uid;
    double openTime;
    double startTime;
    double endTime;
};
//联盟信息结构
struct AllianceAreaInfo{
    string tuid;
    AAreaState state;
    /*(状态)
     0, int //放置 未建造
     1,//建造或者升级中
     2,//修理中
     3,//建造完成，完整状态
     4;//被占领，摧毁中*/
    double updateTime;
    double startTime;
    int defence;//城防值
    int defenceMax;//最大城防
    double buildSpeed;
    int range;
    int dialogId;
    
};
//超级矿山结构
struct SuperMineInfo{
    string truid;
    double trutime;
    double trstime;
    int trdef;
    int trdefmax;
    AAreaState trstat;
    double buildSpeed;
    double digSpeed;
    unsigned int troopMax;
    double digStartTime;
    unsigned int sum;
    WorldResourceType type;
    double changeCollectSpdTime;
    int digStartNum;
};
//新的资源结构
struct NewResourseInfo{
    WorldResourceType type;
    int max_capacity;
    int now_capacity;
    string namdDialog;
    double digSpeed;
    double digStartTime;
    unsigned int sum;
    double refTime;
};
//塔结构
struct TowerInfo{
    string uid;
    double updateTime;
    double startTime;
    double buildSpeed;
    int defence;
    int defenceMax;
    AAreaState state;
    int dialog;
   
};
//旗帜结构
struct BannerInfo{
    string uid;
    double updateTime;
    double startTime;
    double buildSpeed;
    int defence;
    int defenceMax;
    AAreaState state;
    string banner;
};
//仓库结构
struct WarehouseInfo{
    string uid;
    double updateTime;
    double startTime;
    double buildSpeed;
    int defence;
    int defenceMax;
    AAreaState state;
};

//盟主结构
struct AllianceAreaOwnerInfo{
    string ownerId;
    string ownerName;
    string allianceId;
    string allianceAbbr;
    string ownerMarchUuid;
    bool team;
    string ownerPic;
    
    bool operator==( const AllianceAreaOwnerInfo& rInfo) {
        bool res = true;
        res &= (ownerId == rInfo.ownerId);
        res &= (ownerName == rInfo.ownerName);
        res &= (allianceId == rInfo.allianceId);
        res &= (allianceAbbr == rInfo.allianceAbbr);
        res &= (ownerMarchUuid == rInfo.ownerMarchUuid);
        res &= (team == rInfo.team);
        res &= (ownerPic == rInfo.ownerPic);
        return res;
    }
    bool operator!=( const AllianceAreaOwnerInfo& rInfo) {
        return !(*this == rInfo );
    }
};
//王活动建设
struct KingActivityBuildInfo{
    std::string tid;
    std::string allianceUid;
    std::string uid;
    double openTime;
    double startTime;
    double endTime;
};

struct MarchTimeInfo{
//    int startPoint;
//    int endPoint;
    int costTime;
};
//宝物结构
struct TreasureInfo{
    string uuid;
    string treasuerId;
    string currentOwner;
    string originalOwner;
    unsigned int targetId; //目标点
    int time; //挖宝持续时间 单位s
    int colour;//星级
    int state;
};
//龙结构
struct DragonTileInfo{
    std::string allianceUid;
    std::string uid;
    double startTime;
    double endTime;
};

struct DBuildinginfo{
    int id;
    std::string itemId;
    int index;
    int type;
    std::string ai;
    double occupyST;
    int radius;
    std::string vesion;
};

class MarchInfo {
public:
    MarchInfo() :
    uuid("")
    ,startPointIndex(0)
    ,endPointIndex(0)
    ,playerName("")
    ,isChangingSpeed(false)
    ,action(MarchActionNone)
    ,marchTag(0)
    ,marchToIndexLeague("")
    ,teamUid("")
    ,ownerType(PlayerOther)
    ,mt(0)
    ,isKilled(false)
    ,officer("")
    ,ouid("")
    ,monsterLevel(0)
    ,monsterCount(0)
    ,slowDown(false)
    ,treasureMapFinishTime(0)
    ,treasureMapTime(0)
    ,startStamp(0)
    ,endStamp(0)
    ,allianceTeamCreateTime(0)
    {}
    int serverId;
    string uuid;
    string ouid;//monsterId
    int monsterLevel;
    int monsterCount;
    unsigned int startPointIndex;
    unsigned int endPointIndex;
    PlayerType ownerType;
    WorldCityType targetType;
    MarchMethodType marchType;
    MarchStateType stateType;
    
    unsigned int marchColor;
    
    string playerName;
    
    double startStamp;
    double endStamp;
    double allianceTeamCreateTime;
    std::string teamUid;
    double speedX;
    double speedY;
    float  rotate;
    
    bool isChangingSpeed;
    MarchEndAction action;
    bool isMarchWin;//行军胜利
    
    unsigned int marchTag;
    std::string marchToIndexLeague;
    std::string asn;
    //采集
    int troopMax;
    float digSpeed;
    double digStartTime;
    int digStartNum;
    double changeCollectSpdTime;
    //联盟领地
    int territoryStat;
    double territoryStartTime;
    double territoryFinishTime;
    //挖宝
    double treasureMapFinishTime;//结束时间点
    int treasureMapTime; //总长度
    
    //迷宫
    double mazeStartTime;
    double mazeEndTime;
    double mt;
    bool isKilled;
    std::string officer;
    map<string,pair<double, double> > effect;
    map<int, MarchSoldierInfo> marchSoldier;
    map<int, MarchSoldierInfo> walkSoldier;

    bool isDelay();
    bool isLightning();
    bool isStatus(const string& effectId);
    
    ccColor3B getMarchColor();
    double getDigEndTime();
    void updateSoldiers(CCDictionary *dict);
    std::vector<MarchTimeInfo> m_timeInfo;
    void calculateCurrentLine(CCPoint &wholeLineStart, CCPoint &wholeLineEnd, CCPoint &currentLineStart, CCPoint &currentLineEnd);
    CCPoint calculateCurrentLineLeft(CCPoint &wholeLineStart, CCPoint &wholeLineEnd, CCPoint &currentPt);
    double getCurrentLineTotalTime();
    double getCurrentLineTimeLeft();
    CCPoint getCurrentPos(CCPoint &wholeLineStart, CCPoint &wholeLineEnd);
    int getCurrentLineIndex();
    void parseMarchTime(CCArray *arr);//
    
    //是否使用减速领主技能
    bool slowDown;
};

enum MonsterSearchResultType{
    FIGHT,
    CHEST,
    LOST_SOLDIER,
    RESOURCE,
};

enum MonsterFightResultType{
    BATTLE_WIN,
    BATTLE_FAIL,
    BATTLE_DRAW,
};

class MonsterSearchResultInfo : public CCObject{
public:
    CREATE_FUNC(MonsterSearchResultInfo);
    virtual bool init(){return true;};
    MonsterSearchResultInfo() :
    type(MonsterSearchResultType::FIGHT)
    , time(0)
    , fType(MonsterFightResultType::BATTLE_WIN)
    , exp(0)
    , dead(0)
    , itemId("")
    , npcId("")
    , fid("")
    , infoStr("")
    {};
    MonsterSearchResultType type;
    time_t time;
    MonsterFightResultType fType;
    int exp;
    CCSafeObject<CCDictionary> dead;
    std::string itemId;
    std::string npcId;
    std::string fid;
    std::string infoStr;
    std::string resType;
    std::string resValue;
};

class WorldCityInfo {
public:
    int cityIndex;
    int parentCityIndex;
//    WorldCityCountry cityCountry;
    int parentAAreCicyIndex;
    WorldCityType cityType;
    
    string citySubType;  // resource : 0:lumber 1:stone 2:ore 3:food 4:silver 5:gold
                         // battle : battle item id
                         // king : king item id
    
    CCPoint cityTilePoint;
    string cityName;
    
    int cityIcon; // 0-5: resource type 6: city 7: camp 8: monster 9 allianceArea 10 superMine 11 allianceTower 12 warehouse 13 banner 14 actBoss
    
    string playerName;
    string pic;
    
    ResourceTileInfo resource;
    BattleTileInfo battle;
    FieldMonsterInfo fieldMonsterInfo;
    AllianceAreaInfo m_allianceAreaInfo;
    SuperMineInfo m_superMineInfo;
    NewResourseInfo m_newResourceInfo;
    TowerInfo m_towerInfo;
    BannerInfo m_bannerInfo;//联盟国旗
    WarehouseInfo m_warehouseInfo;
    TrebuchetInfo trebuchetInfo;//王国活动投石机信息
    KingActivityBuildInfo kingBuildInfo;//中间的大建筑的信息
    AllianceAreaOwnerInfo m_allianceAreaOwnerInfo;//联盟领地owner信息
    DragonTileInfo dragonTileInfo;//巨龙战建筑信息
    bool isScouted;
    string cityOriginType;
    string leagueAsn;
    int tileGid;
    double startStamp;
    double refreshStamp;
    string allianceName;
    string throne_pic;
    double locateTime;
    int mc_state;
    bool m_isInAArea;//in area
    bool m_isMyAArea;//in myArea
    bool m_forceActive;// state is finished
    bool m_isInTowerRange;
    bool m_isInTowerAttackRange;
    string m_bigRangeAid;
    int m_parentTowerIndex;
    int m_parentTowerIndex2;//重合区域
    /*allianceArea*/
    string m_aArea_id;//联盟id
    int m_aArea_pointId;//领地中心点所在坐标
    int m_aArea_isActive;//领地范围是否激活
    string m_aArea_name;//领地联盟名称
    string m_aArea_simpleName;//领地联盟简称
    string m_aArea_flag;//领地联盟旗帜
    int m_aArea_lv;//等级
    string m_aArea_nickname;//名称
    string m_aArea_direction;
    /*allianceArea*/
    
    string m_aMainCity_direction;//联盟城范围
    int cross;//跨服标记，1为跨服的
    int srcServer;//所在服务器
    int winNum;// 连胜数
    int tileServerId;
    
    double beDetected_Time;
    string beDetected_mailUid;

    
    PlayerTileInfo playerInfo;
    bool isUpdatePlayerInfo;
    double openTime;
    double startTime;
    double endTime;
    bool isUpdateTime;
    uint32_t updatePlayerInfo_flag;
    
    //add for lua use -- add by js 原来的“openTime”，“startTime”，“endTime” 也为lua所用
    int luaType;
    string luaResName;
//    string luaMD5;
    string otherJson;
    
    WorldCityInfo():
    cityIndex(0),
    parentCityIndex(-1),
    parentAAreCicyIndex(-1)
//    ,cityCountry(CountryNotInit)
    ,cityType(OriginTile)
    ,citySubType("")
    ,cityTilePoint(CCPointZero)
    ,cityName("")
    ,cityIcon(-1)
    ,playerName("")
    ,pic("")
    ,resource(ResourceTileInfo{Wood,0,0,0,0,0,0.0,0.0})
    ,battle(BattleTileInfo{BattleTileNone,"","","",0.0,0.0})
    ,cityOriginType("")
    ,leagueAsn("")
    ,tileGid(0)
    ,allianceName("")
    ,throne_pic("301")
    ,locateTime(0)
    ,mc_state(0)
    ,isScouted(false)
    ,m_aArea_id("")
    ,m_aArea_pointId(-1)
    ,m_aArea_isActive(0)
    ,m_aArea_name("")
    ,m_aArea_simpleName("")
    ,m_aArea_flag("")
    ,m_aArea_lv(-1)
    ,m_aArea_nickname("")
    ,m_aArea_direction("")
    ,m_isInAArea(false)
    ,m_isMyAArea(false)
    ,m_forceActive(false)
    ,m_isInTowerRange(false)
    ,m_isInTowerAttackRange(false)
    ,m_parentTowerIndex(-1)
    ,m_parentTowerIndex2(-1)
    ,m_bigRangeAid("")
    ,cross(-1)
    ,srcServer(-1)
    ,winNum(0)
    ,isUpdateTime(false)
    ,isUpdatePlayerInfo(false)
    ,openTime(0.0)
    ,startTime(0.0)
    ,endTime(0.0)
    ,updatePlayerInfo_flag(0)
    ,beDetected_Time(0)
    ,beDetected_mailUid("")
    ,luaType(0)
    {}
    
    ~WorldCityInfo() {
    }
    
    void setInfo(Json* json,bool isRange,int tileServerId);
    void setCityIndex(unsigned int index);
    void setInfo(unsigned int index,int tileServerId);
    void setOriginInfo();
    void updateMonsterInfo(CCDictionary *dict);
    void copyCityInfo(const WorldCityInfo& cityinfo);
};

enum ServerStateType{
    PEACE,
    LOCAL_THRONE_FIGHTING,
    OPEN_THRONE_FIGHTING,
    THRONE_LOSER,
    THRONE_WINNER,
};

struct ServerEnemyInfo{
    int serverId;
    ServerStateType fightState;
};

// 已占领资源点的信息
struct ResCityInfo{
    int m_point;
    double m_StartTime;
    double m_DeadTime;  //倒计时
    string m_uID;       //表唯一ID
    double m_GarArmNum; //驻兵数量
    
};
class ServerListInfo{
public:
    int serverId;
    std::string serverName;
    bool isHot;
    int cityIconIndex;
    int x;
    int y;
    std::string banner;
//    std::string allianceName;
//    std::string allianceFlag;
    std::string kingName;
    std::string abbr;
    std::vector<ServerEnemyInfo> enemyList;//可以进攻服列表
    ServerStateType fightState;//自己服的王座状态
};



struct PICMap{
    string pic;
    int x;
    int y;
    string config;
};

struct MainCityID{
    string bid;         //xml表 ID
    string aid;         // 联盟ID
    string allianceName;
};
class WorldController : public CCObject {

public:
    static WorldController* getInstance();
    //支持不同地图大小
    int _current_tile_count_x;
    int _current_tile_count_y;
    int _current_map_width;
    int _current_map_height;
    unsigned int _current_index_limit;
    bool _current_map_has_relic_area;

    int serverMap_count_x;
    int serverMap_count_y;
    CCPoint serverMap_centerPoint;
    CCPoint serverMap_centerViewPoint;
    int  serverMap_width;
    int serverMap_height;
    
    void startVIPGuide();
    CC_SYNTHESIZE(bool, m_isAsyEnd, isAsyEnd);
    vector<pair<int, CCPoint>> m_helpCmdVec;
    
    void purge();
    void initCityInfo();
    
    void initFavoriteInfo();
    void onFavoriteInfoCallback(CCObject* obj);
    bool isInFavorite(WorldCityInfo &info, int serverId);
    bool inFavoriteForLua(WorldCityInfo *info, int serverId);
    double getFavoriteKey(int index, int serverId);
    
    FavoriteTile* getFavoriteKeyByIndex(int index);
    
    void resetTime(double time);
    double getTime();
    
    void initMarchInfo(CCArray* marchArr,int serverId = -1);
    
//    void addMarchQueue(const string& uuid);
//    void removeMarchQueue(const string& uuid);
    
    void updateTroop(const string& uuid);
    void onTroopBack(CCObject* obj);
    static CCPoint getPointByIndex(unsigned int index, int tileX = -1, int tileY = -1);
    static CCPoint getPointByMapTypeAndIndex(unsigned int index,MapType mType = NORMAL_MAP);
    
    static unsigned int getIndexByPoint(const CCPoint &point, int tileX = -1, int tileY = -1);
    static CCPoint getServerPosById(int serverId);
    static int getServerIdByServerPoint(const CCPoint & serverPoint);
    CCPoint getChangePointByTypeNum(int typeNum);
    CCPoint getChangeViewPosByServerPos(const CCPoint & serverPoint,bool shouldAddEdge = false);
    int getRoundServer(int centerServerId, int roundTypeNum);
    CCPoint getServerViewPosByPos(const CCPoint & serverPoint);
    bool isCityCanSettle(unsigned int index,int tileCount);
    virtual void update(float dt);
    
    void openMarchDeploy(unsigned int targetIndex,unsigned int haveOwner,int rally=0,int bType=-1,int wtIndex=-1,std::string other="", int targetType=0,float slow=1.0);
    void showMarchAlert(int max);
    unsigned int getMaxMarchCount();
    unsigned int getCurrentMarchCount();
    void addMonsetResearchInfo(CCDictionary *dict, bool postNotification = true);
    unsigned int getMarchTag();
    
    int getWalkTime(float distance);
    bool isHasMonsterAttackMarch();
    CCPoint selfPoint;
    
    double startTimeStampOffset;
    void doWhenBackToGame(CCArray *arr, bool isLogOut = false);
    void clearMarchInfo();//删除marchinfo，防止调用同步资源和login导致多加的问题
    void doResumeForGLSource(float fTime);  //zym 2015.9.22 resume时游戏内容相关的放到
    
    CCPoint getCrossPointAtCity(MarchInfo &info, float s = -1);
    CCPoint getCrossPointForLua(MarchInfo *info, float s = -1);
    std::vector<CCPoint> getCrossPointAtRelicGround(CCPoint &startPt, CCPoint &endPt);
    
    bool canAttackMonster(string monsterId = "");
    int getMonsterUsePower(int index);
    
    void onMainCityPush(cocos2d::CCDictionary* obj);
    
    void onSmallCityPush(cocos2d::CCDictionary* obj);
    
    bool getIsSelfResCity(const int point);
    void clearResCityInfo();
    //返回城池所属区域
    string getHomeOwnershipNameByCityIndex(int cityIndex);
    string getHomeOwnershipNameByMapPos(Vec2 mapPos);
    string getHomeOwnershipNameByType(int iAreaType);
    int getHomeOwnershipTypeByCityIndex(int cityIndex);//1-62
    int getHomeOwnershipTypeByMapPos(Vec2 mapPos);//1-62
    
    int getHomeOwnerRegionTypeByMapPos(int cityIndex);//1-13
    int getHomeOwnerRegionTypeByCityIndex(Vec2 mapPos);//1-13
    
    float currentMapZoom;
    
    bool initTime;
    bool isInWorld;
    bool initWorld;
    bool initKingFlag;
    bool alertProectFlag;//占领和扎营需要再次确认
    int m_alertStateFlag;// 资源和扎营行军目标状态是否发生了变化
    bool marchingAlertFlag;//资源行军目标是否有其他玩家正在行军
    bool forceUPdate;//联盟集结遣返士兵用
    
    bool firstInWorld;//判断是否是因为重建主城而进入世界
    bool forceUpdateTerritoryTile;//强制刷新哨塔用
    bool forceUpdateTerritoryInfo;//强制刷新哨塔信息用
    bool forceUpdateTerritoryResourceTile;//强制刷新联盟领地超级矿
    bool forceUpdateTerritoryTowerTile;//强制刷新联盟领地箭塔
    bool forceUpdateTerritoryWarehouseTile;//强制刷新联盟领地仓库
    bool forceUpdateTerritoryBannerTile;//强制刷新联盟领地国旗
    bool forceUpdateTerritoryResourceDetail;//强制刷新联盟领地超级矿详细信息
    unsigned int marchTagCounter;
    MapType currentMapType;
    
    time_t monsterAttStartTime;
    time_t monsterAttEndTime;
    //  cityIndex     cityInfo
    unordered_map<unsigned int, WorldCityInfo> m_cityInfo;
    
    //  cityIndex     favoriteInfo
    map<double, FavoriteTile> m_favoriteTiles;
    map<string, MainCityID> m_mapMainCity;
    
    //  marchId       MarchInfo
    map<string,       MarchInfo> m_marchInfo;
    
    map<int,          ResCityInfo> m_resCityInfo;   //占领的资源点信息
    //  playerName    PlayerInfo
    unordered_map<string,       PlayerTileInfo> m_playerInfo;
    //  cityIndex     marchUuid
    map<unsigned int , string> m_selfMarchUuid;
    //  monster attack info
    CCSafeObject<CCArray> m_monsterAttackInfo;
    //  服务器列表
    std::vector<ServerListInfo> m_serverList;
    map<unsigned int , DBuildinginfo> m_dbuildingInfo;//巨龙战的传送点，TransferPortinfo
    int freePortCnt;//巨龙战免费传送的次数
    
    bool isInfoTake;
    pthread_t m_mainThreadID;
    int currentStamine;
    double lastStamineTime;
    void resetStaime(int num, double time);
    void autoAddStamine();
    int getStatmineIntervalTime();
    double getMarchTime(double dis, WorldCityType type, MarchMethodType marchType = MethodBattle, float slowDown = 1.0);
    double getMarchTime(int startIndex, int endIndex, WorldCityType type, MarchMethodType marchType = MethodBattle, bool toTileFourCity = false, bool isThrone = false);
    
    double getDisForLua(WorldCityInfo *from, WorldCityInfo *to);
    double getDisForLua(WorldCityInfo *to);
    double getDisForLua(CCPoint from, CCPoint to, bool isFromFourTile, bool isToFourTile, bool isThrone);
    double getDisForLua(CCPoint to, bool isToForTile, bool isThrone);
    
    double getDistance(WorldCityInfo &from, WorldCityInfo &to);
    double getDistance(WorldCityInfo &to);
    double getDistance(CCPoint &from, CCPoint &to, bool isFromFourTile, bool isToFourTile, bool isThrone);
    double getDistance(CCPoint &to, bool isToForTile, bool isThrone);
    int getMinSpeed();
    double getMarchTime(MarchInfo &info);
    double marchTimeForLua(MarchInfo *info);
    double marchTimeForLua(double dis, WorldCityType type, MarchMethodType marchType = MethodBattle, float slowDown = 1.0);
    double marchTimeForLua(int startIndex, int endIndex, WorldCityType type, MarchMethodType marchType = MethodBattle, bool toTileFourCity = false, bool isThrone = false);
    
    void addGuideData();
    void addTestData();
    bool checkOverTimeMarch(MarchInfo &info, float dt);
    void getServerList();
    void parseServerList(CCObject *obj);
    int lastInputX;
    int lastInputY;
    int lastInputZ;
    int openTargetIndex;
    string openMarchId;
    string openAddmonsterId;
    int currentMonsterLevel;
    bool isServerCrossOpen;
    bool isRefreshQueue;
    int playAniTargetIndex;//播放动画的点
    int buildAllianceTerritory;//联盟领地id、位置、类型以及超级矿的类型
    string allianceTerritoryId;
    int allianceTerritoryIndex;
    int isDetectFlag;
    int scoutTime;
    //用来显示资源田不同牌子
    map<string, int> resTileKey;
    
    WorldCityType allianceTerritoryType;
    WorldResourceType allianceTerritoryResourceType;
    string allianceTerritoryBanner;
    WorldActivityState getKingActivityStateByType(WorldActivityType type);
    WorldActivityState getKingActivityState(double openTime, double startTime, double endTime);

    void updateWorldActivityTime(CCDictionary *dict);
    void updateWorldActivityTime(Json *json, WorldActivityType type);
    void updateWorldActivityTime(WorldActivityType type, double openTime, double startTime, double endTime);
    std::map<WorldActivityType, WorldActivityInfo> m_worldActivity;
    bool selfThroneForLua(WorldCityInfo *info);
    bool selfTrebuchetForLua(WorldCityInfo *info);
    bool isSelfThrone(WorldCityInfo &info);
    bool isSelfTrebuchet(WorldCityInfo &info);
    bool isInFresherForbidenArea(int index, bool isInServerFight = false);
    void removeAllQueue();
    int getArmyDirectionByInfo(MarchInfo &marchInfo);
    int m_bAAreaCost;
    int m_aTerritory10;//联盟箭塔满建筑值
    int m_aTerritory11;//联盟箭塔射程
    int m_aTerritory12;//联盟箭塔每S攻击次数
    int m_territoryBannerCost;//联盟国旗所需金币
    int effectTag;
    string monsterCountry;
    int monsterSkinId;
    bool isFourGrid(WorldCityType cityType);
//    void enterMap(MapType mapType = NORMAL_MAP, int mapx = _tile_count_x, int mapy = _tile_count_y);
    void enterMap(MapType mapType = DEFAULT_MAP);
    void enterMap(MapType mapType, int mapx, int mapy);
    MapType getMapTypeByDefault();
    bool isInMap(int x, int y, int tileX = -1, int tileY = -1);
    bool isInMap(CCPoint &pt, int tileX = -1, int tileY = -1);
    int getMapXByType(MapType type);
    int getMapYByType(MapType type);
    void initServerMapPos();
    static bool isInSelfServer(int tileServerId);//for two server inview and old logical
    //宝藏 (玩家个人的宝藏信息)
    void initTreasureInfo(CCArray* array);
    void addTreasureInfo(CCDictionary *dict);
    void removeTreasureInfo(string treasureuuid);
    bool inTreasureArea(const CCPoint &pos);
    void clearTreasureInfo();
    void sendGetTreasureCMD(int itemid);
    void speedupTreasureMap(string marchid);
    int getTreasureInfoCount();
    void getTreasureList(int color , vector<TreasureInfo> &list);
    bool isInCrossMap();
    int getSelfCityIndex();
    void changeScoutStateByName(string Name, int state);//0 delete 1,update
    vector<PICMap>& getCityPicArr(int addIndex, int level, bool isKing ,int nSpecialId = -1,bool bCastle=false);
    vector<PICMap>& getCityPicArr(WorldCityInfo &info, int level,int nSpecialId = -1);
    CC_SYNTHESIZE(int, m_touchIndex, touchIndex);
protected:
    vector<TreasureInfo> m_treasureInfo;
    CC_PROPERTY(bool, mCastleSkinSwitch,CastleSkinSwitch);
    unordered_map<int, vector<PICMap>> m_SkinPicMap1;
    unordered_map<int, vector<PICMap>> m_SkinPicMap2;
    unordered_map<int, vector<PICMap>> m_SkinPicMap3;
    unordered_map<int, vector<PICMap>> m_SkinPicMap4;
};

#define MARCH_TIMEUP_DELAY_REQUEST_TIME 5
#define MAX_RETRY_TIME 3

class TimeUpMarchProcesser : public CCObject{
public:
    static TimeUpMarchProcesser *getInstance();
    void addTimeUpMarch(std::string marchUid);
    void removeTimeUpMarch(std::string marchUid);
    void check(MarchInfo &info);
    void refresh();
    void requestDataBack(CCDictionary *dict);
    void clear();
    void resetRetryTime();
    TimeUpMarchProcesser() : m_retryTime(0){};
private:
    bool isNeedSendRequest();
    void sendRequest();
    std::map<std::string, double> m_requestMap;
    int m_retryTime;
};

#endif /* defined(__IF__WorldController__) */
