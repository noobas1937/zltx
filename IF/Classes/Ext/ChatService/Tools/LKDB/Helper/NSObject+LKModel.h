//
//  NSObject+LKModel.h
//  LKDBHelper
//
//  Created by upin on 13-4-15.
//  Copyright (c) 2013年 ljh. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <objc/runtime.h>

@class LKDBProperty;
@class LKModelInfos;
@class LKDBHelper;

#pragma mark- 表结构
@interface NSObject(LKTabelStructure)

/**
 *  overwrite in your models(option)
 *
 *  @return # table name #
 */
+(NSString*)getTableName;

/**
 *  if you set it will use it as a table name
 */
@property(copy,nonatomic)NSString* db_tableName;

/**
 *  the model is inserting ..
 */
@property(readonly,nonatomic)BOOL db_inserting;

/**
 *  sqlite comes with rowid
 */
@property int rowid;

/**
 *  overwrite in your models, if your table has primary key
 
 *  主键列名 如果rowid<0 则跟据此名称update 和delete
 
 *  @return # column name  #
 */
+(NSString*)getPrimaryKey;

/**
 *  multi primary key
 *  联合主键
 *  @return
 */
+(NSArray*) getPrimaryKeyUnionArray;





/**
 *  overwrite in your models set column attribute
 *
 *  @param property infos
 */
+(void)columnAttributeWithProperty:(LKDBProperty*)property;


/**
 *	@brief   get saved pictures and data file path,can overwirte
 
    获取保存的 图片和数据的文件路径
 */
+(NSString*)getDBImagePathWithName:(NSString*)filename;
+(NSString*)getDBDataPathWithName:(NSString*)filename;
@end



#pragma mark- 表数据操作
@interface NSObject(LKTableData)

/***
 *	@brief      overwrite in your models,return insert sqlite table data
 *
 *
 *	@return     property the data after conversion
 */
-(id)userGetValueForModel:(LKDBProperty*)property;

/***
 *	@brief	overwrite in your models,return insert sqlite table data
 *
 *	@param 	property        will set property
 *	@param 	value           sqlite value (NSString(NSData UTF8 Coding) or NSData)
 */
-(void)userSetValueForModel:(LKDBProperty*)property value:(id)value;

///overwrite
+(NSDateFormatter*)getModelDateFormatter;

//lkdbhelper use
-(id)modelGetValue:(LKDBProperty*)property;
-(void)modelSetValue:(LKDBProperty*)property value:(id)value;

-(id)singlePrimaryKeyValue;
-(BOOL)singlePrimaryKeyValueIsEmpty;
-(LKDBProperty*)singlePrimaryKeyProperty;
@end

@interface NSObject (LKModel)

/**
 *  return model use LKDBHelper , default return global LKDBHelper;
 *
 *  @return LKDBHelper
 */
+(LKDBHelper*)getUsingLKDBHelper;

/**
 *  class attributes
 *
 *  @return LKModelInfos
 */
+(LKModelInfos*)getModelInfos;

/**
 *	@brief Containing the super class attributes	设置是否包含 父类 的属性
 */
+(BOOL)isContainParent;

/**
 *  当前表中的列是否包含自身的属性。
 *
 *  @return BOOL
 */
+(BOOL)isContainSelf;

/**
 *	@brief log all property 	打印所有的属性名称和数据
 */
-(NSString*)printAllPropertys;
-(NSString*)printAllPropertysIsContainParent:(BOOL)containParent;

-(NSMutableString*)getAllPropertysString;
@end