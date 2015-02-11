//
//  getBandSpeed.h
//  getBandSpeed
//
//  Created by POWER on 14-7-31.
//  Copyright (c) 2014年 ditaon. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "AsyncSocket.h"

@interface getBandSpeed : NSObject<AsyncSocketDelegate>

@property (strong, nonatomic) NSString *secSpeedStr;

@property (strong, nonatomic) NSString *avgSpeedStr;

- (void)StartSpeedTestWith:(NSArray *)ipStr and:(BOOL)isDownload;

//- (void)starSpeedQueueWith:(NSArray *)array;

@end
