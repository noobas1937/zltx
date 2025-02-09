//
//  LKDBHelper.m
//  upin
//
//  Created by Fanhuan on 12-12-6.
//  Copyright (c) 2012年 linggan. All rights reserved.
//

#import "LKDBHelper.h"
#import "NSString+Cocos2dHelper.h"
#define checkClassIsInvalid(modelClass)if([LKDBUtils checkStringIsEmpty:[modelClass getTableName]]){\
LKErrorLog(@"model class name %@ table name is invalid!",NSStringFromClass(modelClass));\
return NO;}

#define checkModelIsInvalid(model){if(model == nil){LKErrorLog(@"model is nil");return NO;}\
NSString* _model_tableName = model.db_tableName?:[model.class getTableName];\
if(_model_tableName.length == 0){LKErrorLog(@"model class name %@ table name is invalid!",_model_tableName);return NO;}}

@interface NSObject(LKTabelStructure_Private)
-(void)setDb_inserting:(BOOL)db_inserting;
@end

@interface LKDBWeakObject : NSObject
@property(LKDBWeak,nonatomic)LKDBHelper* obj;
@end

@interface LKDBHelper()

@property(strong,nonatomic)NSMutableArray* createdTableNames;

@property(LKDBWeak,nonatomic)FMDatabase* usingdb;
@property(strong,nonatomic)FMDatabaseQueue* bindingQueue;
@property(copy,nonatomic)NSString* dbPath;

@property(strong,nonatomic)NSRecursiveLock* threadLock;
@end

@implementation LKDBHelper
+(NSMutableArray*)dbHelperSingleArray
{
    static __strong NSMutableArray* dbArray;
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        dbArray = [NSMutableArray array];
    });
    return dbArray;
}
+(LKDBHelper*)dbHelperWithPath:(NSString*)dbFilePath save:(LKDBHelper*)helper
{
    NSMutableArray* dbArray = [self dbHelperSingleArray];
    
    if(helper)
    {
        LKDBWeakObject* weakObj = [LKDBWeakObject new];
        weakObj.obj = helper;
        [dbArray addObject:weakObj];
    }
    else if(dbFilePath)
    {
        for (int i=0; i<dbArray.count;)
        {
            LKDBWeakObject* weakObj = [dbArray objectAtIndex:i];
            if(weakObj.obj == nil)
            {
                [dbArray removeObject:weakObj];
                continue;
            }
            else if([weakObj.obj.dbPath isEqualToString:dbFilePath])
            {
                return weakObj.obj;
            }
            i++;
        }
    }
    return nil;
}
- (instancetype)init
{
    return [self initWithDBName:@"LKDB"];
}
-(instancetype)initWithCustomNameDB{
    //DB 路径 :/uid/database/chat_service.db
    return [self initWithDBName:@"chat_service"];
}
+(NSString *)gettingDBPath{
    NSString *uidString = [NSString gettingCurrentAccountUserUid];
    if (uidString.length> 0 && uidString != nil) {
        NSString *dbPath = [NSString stringWithFormat:@"/%@/database",uidString];
        return dbPath;
    }else{
        return @"db";
    }
    
}
-(instancetype)initWithDBName:(NSString *)dbname
{
    return [self initWithDBPath:[LKDBHelper getDBPathWithDBName:dbname]];
}
-(instancetype)initWithDBPath:(NSString *)filePath
{
    if([LKDBUtils checkStringIsEmpty:filePath])
    {
        self = nil;
        return self;
    }
    LKDBHelper* helper = [LKDBHelper dbHelperWithPath:filePath save:nil];
    if(helper)
    {
        self = helper;
    }
    else
    {
        self = [super init];
        if (self)
        {
            self.threadLock = [[NSRecursiveLock alloc]init];
            self.createdTableNames = [NSMutableArray array];
            
            [self setDBPath:filePath];
            [LKDBHelper dbHelperWithPath:nil save:self];
        }
    }
    return self;
}

#pragma mark- init FMDB
+(NSString*)getDBPathWithDBName:(NSString*)dbName
{
    NSString* fileName = nil;
    if([dbName hasSuffix:@".db"] == NO) {
        fileName = [NSString stringWithFormat:@"%@.db",dbName];
    }
    else {
        fileName = dbName;
    }
    
//    NSString* filePath = [LKDBUtils getPathForDocuments:fileName inDir:@"db"];
       NSString *filePath = [LKDBUtils getPathForDocuments:fileName inDir:[self gettingDBPath]];
    LKLog(@"数据库路径:%@",filePath);
    return filePath;
}
-(void)setDBName:(NSString *)dbName
{
    [self setDBPath:[LKDBHelper getDBPathWithDBName:dbName]];
}

-(void)setDBPath:(NSString *)filePath
{
    if(self.bindingQueue && [self.dbPath isEqualToString:filePath])
    {
        return;
    }
    
    //创建数据库目录
    NSRange lastComponent = [filePath rangeOfString:@"/" options:NSBackwardsSearch];
    if(lastComponent.length > 0)
    {
        NSString* dirPath = [filePath substringToIndex:lastComponent.location];
        BOOL isDir = NO;
        BOOL isCreated = [[NSFileManager defaultManager] fileExistsAtPath:dirPath isDirectory:&isDir];
        if ( isCreated == NO || isDir == NO )
        {
            NSError* error = nil;
            BOOL success = [[NSFileManager defaultManager] createDirectoryAtPath:dirPath withIntermediateDirectories:YES attributes:nil error:&error];
            if(success == NO)
            {
                NSLog(@"create dir error: %@",error.debugDescription);
            }
        }
    }
    self.dbPath = filePath;
    [self.bindingQueue close];
    self.bindingQueue = [[FMDatabaseQueue alloc]initWithPath:filePath];
    
#ifdef DEBUG
    //debug 模式下  打印错误日志
    [_bindingQueue inDatabase:^(FMDatabase *db) {
        db.logsErrors = YES;
    }];
#endif
}

#pragma mark- core
-(void)executeDB:(void (^)(FMDatabase *db))block
{
    [_threadLock lock];
    if(self.usingdb != nil)
    {
        block(self.usingdb);
    }
    else
    {
        if(_bindingQueue == nil)
        {
            _bindingQueue = [[FMDatabaseQueue alloc]initWithPath:_dbPath];
        }
        [_bindingQueue inDatabase:^(FMDatabase *db) {
            self.usingdb = db;
            block(db);
            self.usingdb = nil;
        }];
    }
    [_threadLock unlock];
}
-(BOOL)executeSQL:(NSString *)sql arguments:(NSArray *)args
{
    __block BOOL execute = NO;
    [self executeDB:^(FMDatabase *db) {
        if(args.count>0)
            execute = [db executeUpdate:sql withArgumentsInArray:args];
        else
            execute = [db executeUpdate:sql];
    }];
    return execute;
}
-(NSString *)executeScalarWithSQL:(NSString *)sql arguments:(NSArray *)args
{
    __block NSString* scalar = nil;
    [self executeDB:^(FMDatabase *db) {
        FMResultSet* set = nil;
        if(args.count>0)
            set = [db executeQuery:sql withArgumentsInArray:args];
        else
            set = [db executeQuery:sql];
        
        if([set columnCount]>0 && [set next])
        {
            scalar = [set stringForColumnIndex:0];
        }
        [set close];
    }];
    return scalar;
}


//splice 'where' 拼接where语句
- (NSMutableArray *)extractQuery:(NSMutableString *)query where:(id)where
{
    NSMutableArray* values = nil;
    if([where isKindOfClass:[NSString class]] && [LKDBUtils checkStringIsEmpty:where]==NO)
    {
        [query appendFormat:@" where %@",where];
    }
    else if ([where isKindOfClass:[NSDictionary class]])
    {
        NSDictionary* dicWhere = where;
        if(dicWhere.count > 0)
        {
            values = [NSMutableArray arrayWithCapacity:dicWhere.count];
            NSString* wherekey = [self dictionaryToSqlWhere:where andValues:values];
            [query appendFormat:@" where %@",wherekey];
        }
    }
    return values;
}

//dic where parse
-(NSString*)dictionaryToSqlWhere:(NSDictionary*)dic andValues:(NSMutableArray*)values
{
    NSMutableString* wherekey = [NSMutableString stringWithCapacity:0];
    if(dic != nil && dic.count >0 )
    {
        NSArray* keys = dic.allKeys;
        for (int i=0; i< keys.count;i++) {
            
            NSString* key = [keys objectAtIndex:i];
            id va = [dic objectForKey:key];
            if([va isKindOfClass:[NSArray class]])
            {
                NSArray* vlist = va;
                if(vlist.count==0)
                    continue;
                
                if(wherekey.length > 0)
                    [wherekey appendString:@" and"];
                
                [wherekey appendFormat:@" %@ in(",key];
                
                for (int j=0; j<vlist.count; j++) {
                    
                    [wherekey appendString:@"?"];
                    if(j== vlist.count-1)
                        [wherekey appendString:@")"];
                    else
                        [wherekey appendString:@","];
                    
                    [values addObject:[vlist objectAtIndex:j]];
                }
            }
            else
            {
                if(wherekey.length > 0)
                    [wherekey appendFormat:@" and %@=?",key];
                else
                    [wherekey appendFormat:@" %@=?",key];
                
                [values addObject:va];
            }
            
        }
    }
    return wherekey;
}
//where sql statements about model primary keys
-(NSMutableString*)primaryKeyWhereSQLWithModel:(NSObject*)model addPValues:(NSMutableArray*)addPValues
{
    LKModelInfos* infos = [model.class getModelInfos];
    NSArray* primaryKeys = infos.primaryKeys;
    NSMutableString* pwhere = [NSMutableString string];
    if(primaryKeys.count>0)
    {
        for (int i=0; i<primaryKeys.count; i++) {
            NSString* pk = [primaryKeys objectAtIndex:i];
            if([LKDBUtils checkStringIsEmpty:pk] == NO)
            {
                LKDBProperty* property = [infos objectWithSqlColumnName:pk];
                id pvalue = nil;
                if(property && [property.type isEqualToString:LKSQL_Mapping_UserCalculate])
                {
                    pvalue = [model userGetValueForModel:property];
                }
                else if(pk && property)
                {
                    pvalue = [model modelGetValue:property];
                }
                
                if(pvalue)
                {
                    if(pwhere.length>0)
                    {
                        [pwhere appendString:@"and"];
                    }
                    
                    if(addPValues)
                    {
                        [pwhere appendFormat:@" %@=? ",pk];
                        [addPValues addObject:pvalue];
                    }
                    else
                    {
                        [pwhere appendFormat:@" %@='%@' ",pk,pvalue];
                    }
                }
            }
        }
    }
    return pwhere;
}

#pragma mark- dealloc
-(void)dealloc
{
    NSArray* array = [LKDBHelper dbHelperSingleArray];
    for (LKDBWeakObject* weakObject in array) {
        if([weakObject.obj isEqual:self])
        {
            weakObject.obj = nil;
        }
    }
    
    [self.bindingQueue close];
    self.usingdb = nil;
    self.bindingQueue = nil;
    self.dbPath = nil;
    self.threadLock = nil;
}
@end
@implementation LKDBHelper(DatabaseManager)

-(void)dropAllTable
{
    [self executeDB:^(FMDatabase *db) {
        FMResultSet* set = [db executeQuery:@"select name from sqlite_master where type='table'"];
        NSMutableArray* dropTables = [NSMutableArray arrayWithCapacity:0];
        while ([set next]) {
            [dropTables addObject:[set stringForColumnIndex:0]];
        }
        [set close];
        
        for (NSString* tableName in dropTables) {
            NSString* dropTable = [NSString stringWithFormat:@"drop table %@",tableName];
            [db executeUpdate:dropTable];
        }
    }];
}
-(BOOL)dropTableWithClass:(Class)modelClass
{
    checkClassIsInvalid(modelClass);
    
    NSString* tableName = [modelClass getTableName];
    NSString* dropTable = [NSString stringWithFormat:@"drop table %@",tableName];
    
    BOOL isDrop = [self executeSQL:dropTable arguments:nil];
    return isDrop;
}
-(void)fixSqlColumnsWithClass:(Class)clazz tableName:(NSString*)tableName
{
    [self executeDB:^(FMDatabase *db) {
        
        LKModelInfos* infos = [clazz getModelInfos];
        
        NSString* select = [NSString stringWithFormat:@"select * from %@ limit 0",tableName];
        FMResultSet* set = [db executeQuery:select];
        NSArray*  columnArray = set.columnNameToIndexMap.allKeys;
        [set close];
        
        NSMutableArray* alterAddColumns = [NSMutableArray array];
        for (int i=0; i<infos.count; i++)
        {
            LKDBProperty* property = [infos objectWithIndex:i];
            if([property.sqlColumnName.lowercaseString isEqualToString:@"rowid"])
            {
                continue;
            }
            
            ///数据库中不存在 需要alter add
            if([columnArray containsObject:property.sqlColumnName.lowercaseString] == NO)
            {
                NSMutableString* addColumePars = [NSMutableString stringWithFormat:@"%@ %@",property.sqlColumnName,property.sqlColumnType];
                [clazz columnAttributeWithProperty:property];
                
                if(property.length>0 && [property.sqlColumnType isEqualToString:LKSQL_Type_Text])
                    [addColumePars appendFormat:@"(%d)",property.length];
                
                if(property.isNotNull)
                    [addColumePars appendFormat:@" %@",LKSQL_Attribute_NotNull];
                
                if(property.checkValue)
                    [addColumePars appendFormat:@" %@(%@)",LKSQL_Attribute_Check,property.checkValue];
                
                if(property.defaultValue)
                    [addColumePars appendFormat:@" %@ %@",LKSQL_Attribute_Default,property.defaultValue];
                
                NSString* alertSQL = [NSString stringWithFormat:@"alter table %@ add column %@",tableName,addColumePars];
                NSString* initColumnValue = [NSString stringWithFormat:@"update %@ set %@=%@",tableName,property.sqlColumnName,[property.sqlColumnType isEqualToString:LKSQL_Type_Text]?@"''":@"0"];
                
                BOOL success = [db executeUpdate:alertSQL];
                if(success)
                {
                    [db executeUpdate:initColumnValue];
                    [alterAddColumns addObject:property];
                }
            }
        }
        
        if(alterAddColumns.count > 0)
        {
            [clazz dbDidAlterTable:self tableName:tableName addColumns:alterAddColumns];
        }
    }];
}
-(BOOL)_createTableWithModelClass:(Class)modelClass tableName:(NSString*)tableName
{
    checkClassIsInvalid(modelClass);
    if([self getTableCreatedWithTableName:tableName])
    {
        //已创建表 就跳过
        if([_createdTableNames containsObject:tableName] == NO)
        {
            [_createdTableNames addObject:tableName];
        }
        [self fixSqlColumnsWithClass:modelClass tableName:tableName];
        return YES;
    }

    LKModelInfos* infos = [modelClass getModelInfos];
    NSArray* primaryKeys = infos.primaryKeys;
    BOOL isAutoinc = NO;
    if(primaryKeys.count == 1)
    {
        //主键只有一个 并且是 int 类型 则设置为自增长
        NSString* primaryType = [infos objectWithSqlColumnName:[primaryKeys lastObject]].sqlColumnType;
        if([primaryType isEqualToString:LKSQL_Type_Int])
        {
            isAutoinc = YES;
        }
    }
    NSMutableString* table_pars = [NSMutableString string];
    for (int i=0; i<infos.count; i++)
    {
        if(i > 0)
        {
            [table_pars appendString:@","];
        }
        
        LKDBProperty* property =  [infos objectWithIndex:i];
        [modelClass columnAttributeWithProperty:property];
        
        NSString* columnType = property.sqlColumnType;
        if([columnType isEqualToString:LKSQL_Type_Double])
        {
            columnType = LKSQL_Type_Text;
        }
        
        [table_pars appendFormat:@"%@ %@",property.sqlColumnName,columnType];
        
        if([property.sqlColumnType isEqualToString:LKSQL_Type_Text])
        {
            if(property.length>0)
            {
                [table_pars appendFormat:@"(%d)",property.length];
            }
        }
        if(property.isNotNull)
        {
            [table_pars appendFormat:@" %@",LKSQL_Attribute_NotNull];
        }
        if(property.isUnique)
        {
            [table_pars appendFormat:@" %@",LKSQL_Attribute_Unique];
        }
        if(property.checkValue)
        {
            [table_pars appendFormat:@" %@(%@)",LKSQL_Attribute_Check,property.checkValue];
        }
        if(property.defaultValue)
        {
            [table_pars appendFormat:@" %@ %@",LKSQL_Attribute_Default,property.defaultValue];
        }
        if(isAutoinc)
        {
            if([property.sqlColumnName isEqualToString:[primaryKeys lastObject]])
            {
                [table_pars appendString:@" primary key autoincrement"];
            }
        }
    }
    NSMutableString* pksb = [NSMutableString string];

    ///联合主键
    if(isAutoinc == NO)
    {
        if(primaryKeys.count>0)
        {
            pksb = [NSMutableString string];
            for (int i=0; i<primaryKeys.count; i++) {
                NSString* pk = [primaryKeys objectAtIndex:i];
                
                if(pksb.length>0){
                    [pksb appendString:@","];
                }
                
                [pksb appendString:pk];
            }
            if(pksb.length>0)
            {
                [pksb insertString:@",primary key(" atIndex:0];
                [pksb appendString:@")"];
            }
        }
    }
    NSString* createTableSQL = [NSString stringWithFormat:@"CREATE TABLE IF NOT EXISTS %@(%@%@)",tableName,table_pars,pksb];
    
    BOOL isCreated = [self executeSQL:createTableSQL arguments:nil];
    
    if(isCreated)
    {
        [_createdTableNames addObject:tableName];
        [modelClass dbDidCreateTable:self tableName:tableName];
    }
    
    return isCreated;
}
-(BOOL)getTableCreatedWithClass:(Class)modelClass
{
    return [self getTableCreatedWithTableName:[modelClass getTableName]];
}
-(BOOL)getTableCreatedWithTableName:(NSString *)tableName
{
    __block BOOL isTableCreated = NO;
    [self executeDB:^(FMDatabase *db) {
        FMResultSet* set = [db executeQuery:@"select count(name) from sqlite_master where type='table' and name=?",tableName];
        [set next];
        if([set intForColumnIndex:0]>0)
        {
            isTableCreated = YES;
        }
        [set close];
    }];
    return isTableCreated;
}
@end

@implementation LKDBHelper(DatabaseExecute)

-(id)modelValueWithProperty:(LKDBProperty *)property model:(NSObject *)model {
    id value = nil;
    if(property.isUserCalculate)
    {
        value = [model userGetValueForModel:property];
    }
    else
    {
        value = [model modelGetValue:property];
    }
    if(value == nil)
    {
        value = @"";
    }
    return value;
}
-(void)asyncBlock:(void(^)(void))block
{
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0),block);
}
#pragma mark - row count operation
-(int)rowCount:(Class)modelClass where:(id)where
{
    return [self rowCountBase:modelClass where:where];
}

/** zyt扩展 添加传入表名 */
-(int)rowCountWithModelClassName:(NSString *)vTableName withModelClass:(Class)vModelClass where:(id)where{
    return [self rowCountBaseWithTableName:vTableName  withModelClass:vModelClass where:where];
}


-(void)rowCount:(Class)modelClass where:(id)where callback:(void (^)(int))callback
{
    [self asyncBlock:^{
        int result = [self rowCountBase:modelClass where:where];
        if(callback != nil)
        {
            callback(result);
        }
    }];
}
-(int)rowCountBase:(Class)modelClass where:(id)where
{
    NSMutableString* rowCountSql = [NSMutableString stringWithFormat:@"select count(rowid) from %@",[modelClass getTableName]];
    
    NSMutableArray* valuesarray = [self extractQuery:rowCountSql where:where];
    int result = [[self executeScalarWithSQL:rowCountSql arguments:valuesarray] intValue];
    
    return result;
}
- (int )rowCountBaseWithTableName:(NSString *)tableName withModelClass:(Class)vModelClass where:(id)where
{
  

    NSMutableString *rowCountSql = [NSMutableString stringWithFormat:@"select count(rowid) from %@", tableName];
    
    NSMutableArray *valuesarray = [self extractQuery:rowCountSql where:where];
    LKLog(@"查询条数的seq：%@",rowCountSql);
    int  result = [[self executeScalarWithSQL:rowCountSql arguments:valuesarray] intValue];
    
    return result;
}
#pragma mark- search operation
-(NSMutableArray *)search:(Class)modelClass where:(id)where orderBy:(NSString *)orderBy offset:(int)offset count:(int)count
{
    return [self searchBase:modelClass columns:nil where:where orderBy:orderBy offset:offset count:count];
}
/** zyt  */
-(NSMutableArray *)searchWithModelTableName:(NSString *)vTableNameString
                          andWithModelClass:(Class)vModelClass
                                      where:(id)where
                                    orderBy:(NSString *)orderBy
                                     offset:(int)offset
                                      count:(int)count{
    NSMutableArray* array = [self searchBaseWithModelTableName:vTableNameString andWithModelClass:vModelClass where:where orderBy:orderBy offset:0 count:count];
    if(array.count>0)
        return array  ;
    
    return nil;
}


-(NSMutableArray *)search:(Class)modelClass column:(id)columns where:(id)where orderBy:(NSString *)orderBy offset:(int)offset count:(int)count
{
    return [self searchBase:modelClass columns:columns where:where orderBy:orderBy offset:offset count:count];
}
-(id)searchSingle:(Class)modelClass where:(id)where orderBy:(NSString *)orderBy
{
    NSMutableArray* array = [self searchBase:modelClass columns:nil where:where orderBy:orderBy offset:0 count:1];
    
    if(array.count>0)
    {
        return [array objectAtIndex:0];
    }
    
    return nil;
}


/** zyt扩展 添加传入表名 */
-(id)searchSingleWithModelTableName:(NSString *)vTableNameString
                  andWithModelClass:(Class)vModelClass
                              where:(id)where
                            orderBy:(NSString *)orderBy{
    NSArray* array = [self searchBaseWithModelTableName:vTableNameString andWithModelClass:vModelClass where:where orderBy:orderBy offset:0 count:1];
    if(array.count>0)
        return [array objectAtIndex:0]  ;
    
    return nil;
}
-(void)searchSingleWithModelTableName:(NSString *)vTableNameString
                    andWithModelClass:(Class)vModelClass
                                where:(id)where orderBy:(NSString *)orderBy
                               offset:(int)offset
                                count:(int)count
                             callback:(void (^)(NSMutableArray *))block
{
    [self asyncBlock:^{
       NSArray* array = [self searchBaseWithModelTableName:vTableNameString andWithModelClass:vModelClass where:where orderBy:orderBy offset:0 count:1];
        if(block != nil)
            block(array);
    }];
}

-(void)search:(Class)modelClass where:(id)where orderBy:(NSString *)orderBy offset:(int)offset count:(int)count callback:(void (^)(NSMutableArray *))block
{
    [self asyncBlock:^{
        NSMutableArray* array = [self searchBase:modelClass columns:nil where:where orderBy:orderBy offset:offset count:count];
        
        if(block != nil)
            block(array);
    }];
}

-(NSMutableArray *)searchBase:(Class)modelClass columns:(id)columns where:(id)where orderBy:(NSString *)orderBy offset:(int)offset count:(int)count
{
    NSString* columnsString = nil;
    NSUInteger columnCount = 0;
    if([columns isKindOfClass:[NSArray class]] && [columns count]>0)
    {
        columnCount = [columns count];
        columnsString = [columns componentsJoinedByString:@","];
    }
    else if([LKDBUtils checkStringIsEmpty:columns]==NO)
    {
        columnsString = columns;
        NSArray* array = [columns componentsSeparatedByString:@","];
        
        columnCount = array.count;
    }
    
    if(columnCount > 1)
    {
        columnsString = [NSString stringWithFormat:@"rowid,%@",columnsString];
    }
    
    if(columnCount==0)
    {
        columnsString = @"rowid,*";
    }
    
    NSMutableString* query = [NSMutableString stringWithFormat:@"select %@ from @t",columnsString];
    NSMutableArray * values = [self extractQuery:query where:where];
    
    [self sqlString:query AddOder:orderBy offset:offset count:count];
    
    NSString* db_tableName = [modelClass getTableName];
    //replace @t to model table name
    NSString* replaceTableName = [NSString stringWithFormat:@" %@ ",db_tableName];
    if([query hasSuffix:@" @t"])
    {
        [query appendString:@" "];
    }
    [query replaceOccurrencesOfString:@" @t " withString:replaceTableName options:NSCaseInsensitiveSearch range:NSMakeRange(0, query.length)];
    
    __block NSMutableArray* results = nil;
    [self executeDB:^(FMDatabase *db) {
        FMResultSet* set = nil;
        if(values == nil)
        {
            set = [db executeQuery:query];
        }
        else
        {
            set = [db executeQuery:query withArgumentsInArray:values];
        }
        
        if(columnCount == 1)
        {
            results = [self executeOneColumnResult:set];
        }
        else
        {
            results = [self executeResult:set Class:modelClass tableName:db_tableName];
        }
        
        [set close];
    }];
    return results;
}
-(NSMutableArray *)searchBaseWithModelTableName:(NSString *)vTableName
                              andWithModelClass:(Class)vModelClass
                                          where:(id)where
                                        orderBy:(NSString *)orderBy
                                         offset:(int)offset
                                          count:(int)count
{

    NSString* columnsString = nil;
    NSUInteger columnCount = 0;
//    if([_createdTableNames containsObject:vTableName] == NO)
//    {
//        [self _createTableWithModelClass:vModelClass  tableName:vTableName];
//    }
//    if([columns isKindOfClass:[NSArray class]] && [columns count]>0)
//    {
//        columnCount = [columns count];
//        columnsString = [columns componentsJoinedByString:@","];
//    }
//    else if([LKDBUtils checkStringIsEmpty:columns]==NO)
//    {
//        columnsString = columns;
//        NSArray* array = [columns componentsSeparatedByString:@","];
//        
//        columnCount = array.count;
//    }
    
    if(columnCount > 1)
    {
        columnsString = [NSString stringWithFormat:@"rowid,%@",columnsString];
    }
    
    if(columnCount==0)
    {
        columnsString = @"rowid,*";
    }
    
    NSMutableString* query = [NSMutableString stringWithFormat:@"select %@ from @t",columnsString];
    NSMutableArray * values = [self extractQuery:query where:where];
    
    [self sqlString:query AddOder:orderBy offset:offset count:count];
    
    NSString* db_tableName = vTableName;
    //replace @t to model table name
    NSString* replaceTableName = [NSString stringWithFormat:@" %@ ",db_tableName];
    if([query hasSuffix:@" @t"])
    {
        [query appendString:@" "];
    }
    [query replaceOccurrencesOfString:@" @t " withString:replaceTableName options:NSCaseInsensitiveSearch range:NSMakeRange(0, query.length)];
    LKLog(@"查询查询的SQL  :%@",query);
    __block NSMutableArray* results = nil;
    [self executeDB:^(FMDatabase *db) {
        FMResultSet* set = nil;
        if(values == nil)
        {
            set = [db executeQuery:query];
        }
        else
        {
            set = [db executeQuery:query withArgumentsInArray:values];
        }
        
        if(columnCount == 1)
        {
            results = [self executeOneColumnResult:set];
        }
        else
        {
            results = [self executeResult:set Class:vModelClass tableName:db_tableName];
        }
        
        [set close];
    }];
    return results;


}


-(NSMutableArray *)searchWithSQL:(NSString *)sql toClass:(Class)modelClass
{
    //replace @t to model table name
    NSString* replaceString = [NSString stringWithFormat:@" %@ ",[modelClass getTableName]];
    if([sql hasSuffix:@" @t"]){
        sql = [sql stringByAppendingString:@" "];
    }
    NSString* executeSQL = [sql stringByReplacingOccurrencesOfString:@" @t " withString:replaceString options:NSCaseInsensitiveSearch range:NSMakeRange(0, sql.length)];
    
    __block NSMutableArray* results = nil;
    [self executeDB:^(FMDatabase *db) {
        FMResultSet* set = [db executeQuery:executeSQL];
        results = [self executeResult:set Class:modelClass tableName:nil];
        [set close];
    }];
    return results;
}

-(void)sqlString:(NSMutableString*)sql AddOder:(NSString*)orderby offset:(int)offset count:(int)count
{
    if([LKDBUtils checkStringIsEmpty:orderby] == NO)
    {
        [sql appendFormat:@" order by %@",orderby];
    }
    if(count>0)
    {
        [sql appendFormat:@" limit %d offset %d",count,offset];
    }
    else if(offset > 0)
    {
        [sql appendFormat:@" limit %d offset %d",INT_MAX,offset];
    }
}
- (NSMutableArray *)executeOneColumnResult:(FMResultSet *)set
{
    NSMutableArray* array = [NSMutableArray arrayWithCapacity:0];
    while ([set next]) {
        NSString* string = [set stringForColumnIndex:0];
        if(string)
        {
            [array addObject:string];
        }
        else
        {
            NSData* data = [set dataForColumnIndex:0];
            if(data)
            {
                [array addObject:data];
            }
        }
    }
    return array;
}
- (NSMutableArray *)executeResult:(FMResultSet *)set Class:(Class)modelClass tableName:(NSString*)tableName
{
    NSMutableArray* array = [NSMutableArray arrayWithCapacity:0];
    LKModelInfos* infos = [modelClass getModelInfos];
    int columnCount = [set columnCount];
    while ([set next]) {
        
        NSObject* bindingModel = [[modelClass alloc]init];
        
        for (int i=0; i<columnCount; i++) {
            
            NSString* sqlName = [set columnNameForIndex:i];
            LKDBProperty* property = [infos objectWithSqlColumnName:sqlName];

            if(property == nil)
            {
                continue;
            }
            
            BOOL isUserCalculate = [property.type isEqualToString:LKSQL_Mapping_UserCalculate];
            if([[sqlName lowercaseString] isEqualToString:@"rowid"] && isUserCalculate==NO)
            {
                bindingModel.rowid = [set intForColumnIndex:i];
            }
            else
            {
                if(property.propertyName && isUserCalculate == NO)
                {
                    NSString* sqlValue = [set stringForColumnIndex:i];
                    [bindingModel modelSetValue:property value:sqlValue];
                }
                else
                {
                    NSData* sqlData = [set dataForColumnIndex:i];
                    NSString* sqlValue = [[NSString alloc] initWithData:sqlData encoding:NSUTF8StringEncoding];
                    [bindingModel userSetValueForModel:property value:sqlValue?:sqlData];
                }
            }
        }
        bindingModel.db_tableName = tableName;
        [modelClass dbDidSeleted:bindingModel];
        [array addObject:bindingModel];
    }
    return array;
}
#pragma mark- insert operation
-(BOOL)insertToDB:(NSObject *)model
{
    return [self insertBase:model];
}

-(BOOL)insertToDBWithModelTableName:(NSString *)vTableName andWithModel:(NSObject *)model
{
    return [self insertBaseWithModelTableName:vTableName andWithModel:model];
    
}


-(void)insertToDB:(NSObject *)model callback:(void (^)(BOOL))block
{
    [self asyncBlock:^{
        BOOL result = [self insertBase:model];
        if(block != nil)
        {
            block(result);
        }
    }];
}
-(BOOL)insertWhenNotExists:(NSObject *)model
{
    if([self isExistsModel:model]==NO)
    {
        return [self insertToDB:model];
    }
    return NO;
}
-(void)insertWhenNotExists:(NSObject *)model callback:(void (^)(BOOL))block
{
    [self asyncBlock:^{
        if(block != nil)
        {
            block([self insertWhenNotExists:model]);
        }
        else
        {
            [self insertWhenNotExists:model];
        }
    }];
}
-(BOOL)insertBase:(NSObject*)model{
    
    checkModelIsInvalid(model);
    
    Class modelClass = model.class;
    
    //callback
//    if([modelClass dbWillInsert:model]==NO)
//    {
//        LKErrorLog(@"your cancel %@ insert",model);
//        return NO;
//    }

    [model setDb_inserting:YES];
    
    NSString* db_tableName = model.db_tableName?:[modelClass getTableName];
    
    if([_createdTableNames containsObject:db_tableName] == NO)
    {
        [self _createTableWithModelClass:modelClass tableName:db_tableName];
    }
    
    //--
    LKModelInfos* infos = [modelClass getModelInfos];
    
    NSMutableString* insertKey = [NSMutableString stringWithCapacity:0];
    NSMutableString* insertValuesString = [NSMutableString stringWithCapacity:0];
    NSMutableArray* insertValues = [NSMutableArray arrayWithCapacity:infos.count];
    
    
    LKDBProperty* primaryProperty = [model singlePrimaryKeyProperty];
    
    for (int i=0; i<infos.count; i++)
    {
        LKDBProperty* property = [infos objectWithIndex:i];
        if([LKDBUtils checkStringIsEmpty:property.sqlColumnName])
        {
            continue;
        }
        
        if([property isEqual:primaryProperty])
        {
            if([property.sqlColumnType isEqualToString:LKSQL_Type_Int] && [model singlePrimaryKeyValueIsEmpty])
            {
                continue;
            }
        }
        
        if(insertKey.length>0)
        {
            [insertKey appendString:@","];
            [insertValuesString appendString:@","];
        }
        
        [insertKey appendString:property.sqlColumnName];
        [insertValuesString appendString:@"?"];
        
        id value = [self modelValueWithProperty:property model:model];
        
        [insertValues addObject:value];
    }
    
    //拼接insertSQL 语句  采用 replace 插入
    NSString* insertSQL = [NSString stringWithFormat:@"replace into %@(%@) values(%@)",db_tableName,insertKey,insertValuesString];
    
    __block BOOL execute = NO;
    __block sqlite_int64 lastInsertRowId = 0;
    
    [self executeDB:^(FMDatabase *db) {
        execute = [db executeUpdate:insertSQL withArgumentsInArray:insertValues];
        lastInsertRowId= db.lastInsertRowId;
    }];
    
    model.rowid = (int)lastInsertRowId;
    
    [model setDb_inserting:NO];
    
    if(execute == NO)
    {
        LKErrorLog(@"database insert fail %@, sql:%@",NSStringFromClass(modelClass),insertSQL);
    }
    
    //callback
    [modelClass dbDidInserted:model result:execute];
    return execute;
}

-(BOOL)insertBaseWithModelTableName:(NSString *)vTableName
                       andWithModel:(NSObject *)vModel {
    
    
 
    
    Class modelClass = vModel.class;
    if([_createdTableNames containsObject:vTableName] == NO)
    {
        [self _createTableWithModelClass:modelClass tableName:vTableName];
    }
    //callback
//    if([modelClass dbWillInsert:vModel]==NO)
//    {
//        LKErrorLog(@"your cancel %@ insert",vModel);
//        return NO;
//    }
    
    [vModel setDb_inserting:YES];
    
    NSString* db_tableName = vTableName;
    
    if([_createdTableNames containsObject:db_tableName] == NO)
    {
        [self _createTableWithModelClass:modelClass tableName:db_tableName];
    }
    
    //--
    LKModelInfos* infos = [modelClass getModelInfos];
    
    NSMutableString* insertKey = [NSMutableString stringWithCapacity:0];
    NSMutableString* insertValuesString = [NSMutableString stringWithCapacity:0];
    NSMutableArray* insertValues = [NSMutableArray arrayWithCapacity:infos.count];
    
    
    LKDBProperty* primaryProperty = [vModel singlePrimaryKeyProperty];
    
    for (int i=0; i<infos.count; i++)
    {
        LKDBProperty* property = [infos objectWithIndex:i];
        if([LKDBUtils checkStringIsEmpty:property.sqlColumnName])
        {
            continue;
        }
        
        if([property isEqual:primaryProperty])
        {
            if([property.sqlColumnType isEqualToString:LKSQL_Type_Int] && [vModel singlePrimaryKeyValueIsEmpty])
            {
                continue;
            }
        }
        
        if(insertKey.length>0)
        {
            [insertKey appendString:@","];
            [insertValuesString appendString:@","];
        }
        
        [insertKey appendString:property.sqlColumnName];
        [insertValuesString appendString:@"?"];
        
        id value = [self modelValueWithProperty:property model:vModel];
        
        [insertValues addObject:value];
    }
    
    //拼接insertSQL 语句  采用 replace 插入
    NSString* insertSQL = [NSString stringWithFormat:@"replace into %@(%@) values(%@)",db_tableName,insertKey,insertValuesString];
    
    __block BOOL execute = NO;
    __block sqlite_int64 lastInsertRowId = 0;
    
    [self executeDB:^(FMDatabase *db) {
        execute = [db executeUpdate:insertSQL withArgumentsInArray:insertValues];
        lastInsertRowId= db.lastInsertRowId;
    }];
    
    vModel.rowid = (int)lastInsertRowId;
    
    [vModel setDb_inserting:NO];
    
    if(execute == NO)
    {
        LKErrorLog(@"database insert fail %@, sql:%@",NSStringFromClass(modelClass),insertSQL);
    }
    
    //callback
    [modelClass dbDidInserted:vModel result:execute];
    return execute;


}
#pragma mark- update operation
-(BOOL)updateToDB:(NSObject *)model where:(id)where
{
    return [self updateToDBBase:model where:where];
}
-(void)updateToDB:(NSObject *)model where:(id)where callback:(void (^)(BOOL))block
{
    [self asyncBlock:^{
        BOOL result = [self updateToDBBase:model where:where];
        if(block != nil)
            block(result);
    }];
}
/** zyt */
-(BOOL)updateToDBWithModelTableName:(NSString *)vTableName
                       andWithModel:(NSObject *)vModel
                              where:(id)where
{
    return [self updateToDBBaseWithModelTableName:vTableName andWithModel:vModel where:where];
    
}


-(BOOL)updateToDBBase:(NSObject *)model where:(id)where
{
    checkModelIsInvalid(model);
    
    Class modelClass = model.class;
    
    //callback
//    if([modelClass dbWillUpdate:model] == NO)
//    {
//        LKErrorLog(@"you cancel %@ update.",model);
//        return NO;
//    }
    NSString* db_tableName = model.db_tableName?:[modelClass getTableName];
    
    LKModelInfos* infos = [modelClass getModelInfos];
    
    NSMutableString* updateKey = [NSMutableString string];
    NSMutableArray* updateValues = [NSMutableArray arrayWithCapacity:infos.count];
    for (int i=0; i<infos.count; i++) {
        
        LKDBProperty* property = [infos objectWithIndex:i];
        
        if(i>0)
            [updateKey appendString:@","];
        
        [updateKey appendFormat:@"%@=?",property.sqlColumnName];
        
        id value = [self modelValueWithProperty:property model:model];
        
        [updateValues addObject:value];
    }
    
    NSMutableString* updateSQL = [NSMutableString stringWithFormat:@"update %@ set %@ where ",db_tableName,updateKey];
    
    //添加where 语句
    if([where isKindOfClass:[NSString class]] && [LKDBUtils checkStringIsEmpty:where]== NO)
    {
        [updateSQL appendString:where];
    }
    else if([where isKindOfClass:[NSDictionary class]] && [(NSDictionary*)where count]>0)
    {
        NSMutableArray* valuearray = [NSMutableArray array];
        NSString* sqlwhere = [self dictionaryToSqlWhere:where andValues:valuearray];
        
        [updateSQL appendString:sqlwhere];
        [updateValues addObjectsFromArray:valuearray];
    }
    else if(model.rowid > 0)
    {
        [updateSQL appendFormat:@" rowid=%d",model.rowid];
    }
    else
    {
        //如果不通过 rowid 来 更新数据  那 primarykey 一定要有值
        NSString* pwhere = [self primaryKeyWhereSQLWithModel:model addPValues:updateValues];
        if(pwhere.length ==0)
        {
            LKErrorLog(@"database update fail : %@ no find primary key!",NSStringFromClass(modelClass));
            return NO;
        }
        [updateSQL appendString:pwhere];
    }
    
    BOOL execute = [self executeSQL:updateSQL arguments:updateValues];
    if(execute == NO)
    {
        LKErrorLog(@"database update fail : %@   -----> update sql: %@",NSStringFromClass(modelClass),updateSQL);
    }
    
    //callback
    [modelClass dbDidUpdated:model result:execute];
    
    return execute;
}

-(BOOL)updateToDBBaseWithModelTableName:(NSString *)vTableName
                           andWithModel:(NSObject *)vModel
                                  where:(id)where
{
   
    if([_createdTableNames containsObject:vTableName] == NO)
    {
        [self _createTableWithModelClass:[vModel class] tableName:vTableName];
    }
    
    Class modelClass = vModel.class;
    
    //callback
//    if([modelClass dbWillUpdate:vModel] == NO)
//    {
//        LKErrorLog(@"you cancel %@ update.",vModel);
//        return NO;
//    }
    NSString* db_tableName =vTableName;
    
    LKModelInfos* infos = [modelClass getModelInfos];
    
    NSMutableString* updateKey = [NSMutableString string];
    NSMutableArray* updateValues = [NSMutableArray arrayWithCapacity:infos.count];
    for (int i=0; i<infos.count; i++) {
        
        LKDBProperty* property = [infos objectWithIndex:i];
        
        if(i>0)
            [updateKey appendString:@","];
        
        [updateKey appendFormat:@"%@=?",property.sqlColumnName];
        
        id value = [self modelValueWithProperty:property model:vModel];
        
        [updateValues addObject:value];
    }
    
    NSMutableString* updateSQL = [NSMutableString stringWithFormat:@"update %@ set %@ where ",db_tableName,updateKey];
    
    //添加where 语句
    if([where isKindOfClass:[NSString class]] && [LKDBUtils checkStringIsEmpty:where]== NO)
    {
        [updateSQL appendString:where];
    }
    else if([where isKindOfClass:[NSDictionary class]] && [(NSDictionary*)where count]>0)
    {
        NSMutableArray* valuearray = [NSMutableArray array];
        NSString* sqlwhere = [self dictionaryToSqlWhere:where andValues:valuearray];
        
        [updateSQL appendString:sqlwhere];
        [updateValues addObjectsFromArray:valuearray];
    }
    else if(vModel.rowid > 0)
    {
        [updateSQL appendFormat:@" rowid=%d",vModel.rowid];
    }
    else
    {
        //如果不通过 rowid 来 更新数据  那 primarykey 一定要有值
        NSString* pwhere = [self primaryKeyWhereSQLWithModel:vModel addPValues:updateValues];
        if(pwhere.length ==0)
        {
            LKErrorLog(@"database update fail : %@ no find primary key!",NSStringFromClass(modelClass));
            return NO;
        }
        [updateSQL appendString:pwhere];
    }
    
    BOOL execute = [self executeSQL:updateSQL arguments:updateValues];
    if(execute == NO)
    {
        LKErrorLog(@"database update fail : %@   -----> update sql: %@",NSStringFromClass(modelClass),updateSQL);
    }
    
    //callback
    [modelClass dbDidUpdated:vModel result:execute];
    
    return execute;
}

-(BOOL)updateToDB:(Class)modelClass set:(NSString *)sets where:(id)where
{
    checkClassIsInvalid(modelClass);
    
    NSMutableString* updateSQL = [NSMutableString stringWithFormat:@"update %@ set %@ ",[modelClass getTableName],sets];
    NSMutableArray* updateValues = [self extractQuery:updateSQL where:where];
    
    BOOL execute = [self executeSQL:updateSQL arguments:updateValues];
    
    if(execute == NO)
        LKErrorLog(@"database update fail %@   ----->sql:%@",NSStringFromClass(modelClass),updateSQL);
    
    return execute;
}
#pragma mark - delete operation
-(BOOL)deleteToDB:(NSObject *)model
{
    return [self deleteToDBBase:model];
}
-(void)deleteToDB:(NSObject *)model callback:(void (^)(BOOL))block
{
    [self asyncBlock:^{
        BOOL isDeleted = [self deleteToDBBase:model];
        if(block != nil)
            block(isDeleted);
    }];
}

-(BOOL)deleteToDBBase:(NSObject *)model
{
    checkModelIsInvalid(model);
    
    Class modelClass = model.class;
    
//    //callback
//    if([modelClass dbWillDelete:model] == NO)
//    {
//        LKErrorLog(@"you cancel %@ delete",model);
//        return NO;
//    }
    NSString* db_tableName = model.db_tableName?:[modelClass getTableName];
    
    NSMutableString*  deleteSQL = [NSMutableString stringWithFormat:@"delete from %@ where ",db_tableName];
    NSMutableArray* parsArray = [NSMutableArray array];
    if(model.rowid > 0)
    {
        [deleteSQL appendFormat:@"rowid = %d",model.rowid];
    }
    else
    {
        NSString* pwhere = [self primaryKeyWhereSQLWithModel:model addPValues:parsArray];
        if(pwhere.length==0)
        {
            LKErrorLog(@"delete fail : %@ primary value is nil",NSStringFromClass(modelClass));
            return NO;
        }
        [deleteSQL appendString:pwhere];
    }
    
    if(parsArray.count == 0)
    {
        parsArray = nil;
    }
    
    BOOL execute = [self executeSQL:deleteSQL arguments:parsArray];
    
    //callback
    [modelClass dbDidDeleted:model result:execute];
    
    return execute;
}

-(BOOL)deleteWithClass:(Class)modelClass where:(id)where
{
    return [self deleteWithClassBase:modelClass where:where];
}
//zyt
//删除
-(BOOL)deleteToDBWithModelTableName:(NSString *)vTableName where:(id)vWhere{
    return [self deleteToDBBaseWithModelTableName:vTableName andWithModel:nil where:vWhere];
}
-(void)deleteWithClass:(Class)modelClass where:(id)where callback:(void (^)(BOOL))block
{
    [self asyncBlock:^{
        BOOL isDeleted = [self deleteWithClassBase:modelClass where:where];
        if (block != nil) {
            block(isDeleted);
        }
    }];
}
-(BOOL)deleteWithClassBase:(Class)modelClass where:(id)where
{
    checkClassIsInvalid(modelClass);
    
    NSMutableString* deleteSQL = [NSMutableString stringWithFormat:@"delete from %@",[modelClass getTableName]];
    NSMutableArray* values = [self extractQuery:deleteSQL where:where];
    
    BOOL result = [self executeSQL:deleteSQL arguments:values];
    return result;
}



-(BOOL)deleteToDBBaseWithModelTableName:(NSString *)vTableName
                           andWithModel:(NSObject *)vModel
                                  where:(id)where{
    NSMutableString* deleteSQL = [NSMutableString stringWithFormat:@"delete from %@",vTableName];
    NSMutableArray* values = [self extractQuery:deleteSQL where:where];
    LKLog(@"删除数据库数据的Seq_log :%@",deleteSQL);
    BOOL result = [self executeSQL:deleteSQL arguments:values];
    return result;
}
#pragma mark - other operation
-(BOOL)isExistsModel:(NSObject *)model
{
    checkModelIsInvalid(model);
    NSString* pwhere = nil;
    if(model.rowid>0){
        pwhere = [NSString stringWithFormat:@"rowid=%d",model.rowid];
    }
    else{
        pwhere = [self primaryKeyWhereSQLWithModel:model addPValues:nil];
    }
    if(pwhere.length == 0)
    {
        LKErrorLog(@"exists model fail: primary key is nil or invalid");
        return NO;
    }
    return [self isExistsClass:model.class where:pwhere];
}
-(BOOL)isExistsClass:(Class)modelClass where:(id)where
{
    return [self isExistsClassBase:modelClass where:where];
}
-(BOOL)isExistsClassBase:(Class)modelClass where:(id)where
{
    return [self rowCount:modelClass where:where] > 0;
}

#pragma mark- clear operation

+(void)clearTableData:(Class)modelClass
{
    [[modelClass getUsingLKDBHelper] executeDB:^(FMDatabase *db) {
        NSString* delete = [NSString stringWithFormat:@"DELETE FROM %@",[modelClass getTableName]];
        [db executeUpdate:delete];
    }];
}

+(void)clearNoneImage:(Class)modelClass columns:(NSArray *)columns
{
    [self clearFileWithTable:modelClass columns:columns type:1];
}
+(void)clearNoneData:(Class)modelClass columns:(NSArray *)columns
{
    [self clearFileWithTable:modelClass columns:columns type:2];
}
#define LKTestDirFilename @"LKTestDirFilename111"
+(void)clearFileWithTable:(Class)modelClass columns:(NSArray*)columns type:(int)type
{
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_LOW, 0), ^{
        
        NSString* testpath = nil;
        switch (type) {
            case 1:
                testpath = [modelClass getDBImagePathWithName:LKTestDirFilename];
                break;
            case 2:
                testpath = [modelClass getDBDataPathWithName:LKTestDirFilename];
                break;
        }
        
        if([LKDBUtils checkStringIsEmpty:testpath])
            return ;
        
        NSString* dir  = [testpath stringByReplacingOccurrencesOfString:LKTestDirFilename withString:@""];
        
        NSUInteger count =  columns.count;
        
        //获取该目录下所有文件名
        NSArray* files = [LKDBUtils getFilenamesWithDir:dir];
        
        NSString* seleteColumn = [columns componentsJoinedByString:@","];
        NSMutableString* whereStr =[NSMutableString string];
        for (int i=0; i<count ; i++) {
            [whereStr appendFormat:@" %@ != '' ",[columns objectAtIndex:i]];
            if(i< count -1)
            {
                [whereStr appendString:@" or "];
            }
        }
        NSString* querySql = [NSString stringWithFormat:@"select %@ from %@ where %@",seleteColumn,[modelClass getTableName],whereStr];
        __block NSArray* dbfiles;
        [[modelClass getUsingLKDBHelper] executeDB:^(FMDatabase *db) {
            NSMutableArray* tempfiles = [NSMutableArray arrayWithCapacity:6];
            FMResultSet* set = [db executeQuery:querySql];
            while ([set next]) {
                for (int j=0; j<count; j++) {
                    NSString* str = [set stringForColumnIndex:j];
                    if([LKDBUtils checkStringIsEmpty:str] ==NO)
                    {
                        [tempfiles addObject:str];
                    }
                }
            }
            [set close];
            dbfiles = tempfiles;
        }];
        //遍历  当不再数据库记录中 就删除
        for (NSString* deletefile in files) {
            if([dbfiles indexOfObject:deletefile] == NSNotFound)
            {
                [LKDBUtils deleteWithFilepath:[dir stringByAppendingPathComponent:deletefile]];
            }
        }
    });
}
@end

@implementation LKDBHelper(Deprecated_Nonfunctional)
+(LKDBHelper *)sharedDBHelper
{
    return [LKDBHelper getUsingLKDBHelper];
}
-(BOOL)createTableWithModelClass:(Class)modelClass
{
    return [self _createTableWithModelClass:modelClass tableName:[modelClass getTableName]];
}
+(LKDBHelper*)getUsingLKDBHelper
{
    return [[LKDBHelper alloc]init];
}
@end

@implementation LKDBWeakObject

@end
