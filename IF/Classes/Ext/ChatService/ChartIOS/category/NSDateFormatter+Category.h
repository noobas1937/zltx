/************************************************************
  *  * EaseMob CONFIDENTIAL 
  * __________________ 
  * Copyright (C) 2013-2014 EaseMob Technologies. All rights reserved. 
  *  
  * NOTICE: All information contained herein is, and remains 
  * the property of EaseMob Technologies.
  * Dissemination of this information or reproduction of this material 
  * is strictly forbidden unless prior written permission is obtained
  * from EaseMob Technologies.
  */

#import <Foundation/Foundation.h>

@interface NSDateFormatter (Category)

+ (id)dateFormatter;
+ (id)dateFormatterWithFormat:(NSString *)dateFormat;
/** yyyy-MM-dd HH:mm:ss */
+ (id)defaultDateFormatter;
/** yyyy-MM-dd HH:mm:ss.FFF */
+(instancetype)custemDebugDateFormatter;
/** yyyy-MM-dd */
+ (instancetype) custemDateFarmetterWithYearMonthDay;
/** yyyy-MM-dd HH:mm */
+ (instancetype) custemDateFarmetterWithYearToMinutes;
@end
