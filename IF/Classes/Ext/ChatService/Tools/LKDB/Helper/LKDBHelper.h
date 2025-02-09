//
//  LKDBHelper.h
//  upin
//
//  Created by Fanhuan on 12-12-6.
//  Copyright (c) 2012年 linggan. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "FMDatabaseQueue.h"
#import "FMDatabase.h"

#import "LKDBUtils.h"

#import "LKDB+Mapping.h"

#import "NSObject+LKModel.h"
#import "NSObject+LKDBHelper.h"

@interface LKDBHelper : NSObject
/**
 *	@brief  filepath the use of : "documents/db/" + fileName + ".db"
 *  refer:  FMDatabase.h  + (instancetype)databaseWithPath:(NSString*)inPath;
 */
-(instancetype)initWithDBName:(NSString*)dbname;
-(instancetype)initWithCustomNameDB;
-(void)setDBName:(NSString*)fileName;

/**
 *	@brief  path of database file
 *  refer:  FMDatabase.h  + (instancetype)databaseWithPath:(NSString*)inPath;
 */
-(instancetype)initWithDBPath:(NSString*)filePath;
-(void)setDBPath:(NSString*)filePath;

/**
 *	@brief  execute database operations synchronously,not afraid of recursive deadlock  同步执行数据库操作 可递归调用
 */
-(void)executeDB:(void (^)(FMDatabase *db))block;

-(BOOL)executeSQL:(NSString *)sql arguments:(NSArray *)args;
-(NSString *)executeScalarWithSQL:(NSString *)sql arguments:(NSArray *)args;
@end

@interface LKDBHelper(DatabaseManager)

///get table has created
-(BOOL)getTableCreatedWithClass:(Class)model;
-(BOOL)getTableCreatedWithTableName:(NSString*)tableName;

///drop all table
-(void)dropAllTable;

///drop table with entity class
-(BOOL)dropTableWithClass:(Class)modelClass;

@end

@interface LKDBHelper(DatabaseExecute)
/**
 *	@brief	The number of rows query table
 *
 *	@param 	modelClass      entity class
 *	@param 	where           can use NSString or NSDictionary or nil
 *
 *	@return	rows number
 */
-(int)rowCount:(Class)modelClass where:(id)where;
/** zyt扩展 添加传入表名 */
-(int)rowCountWithModelClassName:(NSString *)vTableName withModelClass:(Class)vModelClass where:(id)where;
-(void)rowCount:(Class)modelClass where:(id)where callback:(void(^)(int rowCount))callback;

/**
 *	@brief	query table
 *
 *	@param 	modelClass      entity class
 *	@param 	where           can use NSString or NSDictionary or nil
 
 *	@param 	orderBy         The Sort: Ascending "name asc",Descending "name desc"
                            For example: @"rowid desc"  or @"rowid asc"
 
 *	@param 	offset          Skip how many rows
 *	@param 	count           Limit the number
 *
 *	@return	query finished result is an array(model instance collection)
 */
-(NSMutableArray*)search:(Class)modelClass where:(id)where orderBy:(NSString*)orderBy offset:(int)offset count:(int)count;
-(NSMutableArray *)searchWithModelTableName:(NSString *)vTableNameString
                          andWithModelClass:(Class)vModelClass
                                      where:(id)where
                                    orderBy:(NSString *)orderBy
                                     offset:(int)offset
                                      count:(int)count;
/**
 *  query sql, query finished result is an array(model instance collection)
 *  you can use the "@t" replace Model TableName
 *  example: 
            NSMutableArray* array = [[LKDBHelper getUsingLKDBHelper] searchWithSQL:@"select * from @t where blah blah.." toClass:[ModelClass class]];
 *
 */
-(NSMutableArray*)searchWithSQL:(NSString*)sql toClass:(Class)modelClass;

-(void)search:(Class)modelClass where:(id)where orderBy:(NSString*)orderBy offset:(int)offset count:(int)count callback:(void(^)(NSMutableArray* array))block;
/**
    columns may NSArray or NSString   if query column count == 1  return single column string array
    other return models entity array
 */
-(NSMutableArray*)search:(Class)modelClass column:(id)columns where:(id)where orderBy:(NSString*)orderBy offset:(int)offset count:(int)count;

///return first model or nil
-(id)searchSingle:(Class)modelClass where:(id)where orderBy:(NSString*)orderBy;
-(id)searchSingleWithModelTableName:(NSString *)vTableNameString
                  andWithModelClass:(Class)vModelClass
                              where:(id)where
                            orderBy:(NSString *)orderBy;

-(void)searchSingleWithModelTableName:(NSString *)vTableNameString
                    andWithModelClass:(Class)vModelClass
                                where:(id)where orderBy:(NSString *)orderBy
                               offset:(int)offset
                                count:(int)count
                             callback:(void (^)(NSMutableArray *))block;
/**
 *	@brief	insert table
 *
 *	@param 	model 	you want to insert the entity
 *
 *	@return	the inserted was successful
 */
-(BOOL)insertToDB:(NSObject*)model;
/** zyt 插入 */
-(BOOL)insertToDBWithModelTableName:(NSString *)vTableName andWithModel:(NSObject *)model;
-(void)insertToDB:(NSObject*)model callback:(void(^)(BOOL result))block;

/**
 *	@brief	insert when the entity primary key does not exist
 *
 *	@param 	model 	you want to insert the entity
 *
 *	@return	the inserted was successful
 */
-(BOOL)insertWhenNotExists:(NSObject*)model;
-(void)insertWhenNotExists:(NSObject*)model callback:(void(^)(BOOL result))block;

/**
 *	@brief update table
 *
 *	@param 	model 	you want to update the entity
 *	@param 	where 	can use NSString or NSDictionary or nil
                    when "where" is nil : update the value based on rowid column or primary key column
 *
 *	@return	the updated was successful
 */
-(BOOL)updateToDB:(NSObject *)model where:(id)where;
-(BOOL)updateToDBWithModelTableName:(NSString *)vTableName
                       andWithModel:(NSObject *)vModel
                              where:(id)where;
-(void)updateToDB:(NSObject *)model where:(id)where callback:(void (^)(BOOL result))block;
-(BOOL)updateToDB:(Class)modelClass set:(NSString*)sets where:(id)where;
/**
 *	@brief	delete table
 *
 *	@param 	model 	you want to delete entity
                    when entity property "rowid" == 0  based on the primary key to delete
 *
 *	@return	the deleted was successful
 */
-(BOOL)deleteToDB:(NSObject*)model;
-(void)deleteWithClass:(Class)modelClass where:(id)where callback:(void (^)(BOOL result))block;
-(void)deleteToDB:(NSObject*)model callback:(void(^)(BOOL result))block;

/**
 *	@brief	delete table with "where" constraint
 *
 *	@param 	modelClass      entity class
 *	@param 	where           can use NSString or NSDictionary,  can not is nil
 *
 *	@return	the deleted was successful
 */
-(BOOL)deleteWithClass:(Class)modelClass where:(id)where;
/** zyt 删除 */
-(BOOL)deleteToDBWithModelTableName:(NSString *)vTableName where:(id)vWhere;
-(void)deleteWithClass:(Class)modelClass where:(id)where callback:(void (^)(BOOL result))block;

/**
 *	@brief   entity exists?
 *           for primary key column
            （if rowid > 0 would certainly exist so we do not rowid judgment）
 *	@param 	model 	entity
 *
 *	@return	YES: entity presence , NO: entity not exist
 */
-(BOOL)isExistsModel:(NSObject*)model;
-(BOOL)isExistsClass:(Class)modelClass where:(id)where;


/**
 *	@brief	Clear data based on the entity class
 *
 *	@param 	modelClass 	entity class
 */
+(void)clearTableData:(Class)modelClass;

/**
 *	@brief	Clear Unused Data File
            if you property has UIImage or NSData, will save their data in the (documents dir)
 *
 *	@param 	modelClass      entity class
 *	@param 	columns         UIImage or NSData Column Name
 */
+(void)clearNoneImage:(Class)modelClass columns:(NSArray*)columns;
+(void)clearNoneData:(Class)modelClass columns:(NSArray*)columns;

@end


@interface LKDBHelper(Deprecated_Nonfunctional)
/// you can use [LKDBHelper getUsingLKDBHelper]
#pragma mark- deprecated
+(LKDBHelper*)sharedDBHelper __deprecated_msg("Method deprecated. Use `[Model getUsingLKDBHelper]`");
-(BOOL)createTableWithModelClass:(Class)modelClass __deprecated_msg("Now you can not call it. Will automatically determine whether you need to create");
#pragma mark-
@end
