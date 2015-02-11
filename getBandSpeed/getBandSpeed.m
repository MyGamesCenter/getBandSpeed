//
//  getBandSpeed.m
//  getBandSpeed
//
//  Created by POWER on 14-7-31.
//  Copyright (c) 2014年 ditaon. All rights reserved.
//

#import "getBandSpeed.h"

@implementation getBandSpeed{

    AsyncSocket *asyncSocket;
    
    int sumDataLength;
    
    double sumAveDataLength;
    
    int recCount;
    
    char randomBuffer[1024*10];
    
    BOOL speedWay;
}

@synthesize secSpeedStr = _secSpeedStr;
@synthesize avgSpeedStr = _avgSpeedStr;

- (void)StartSpeedTestWith:(NSArray *)ipStr and:(BOOL)isDownload
{
    //创建socket客户端
    asyncSocket = [[AsyncSocket alloc]initWithDelegate:self];
    
    //设置RunLoop模式
    [asyncSocket setRunLoopModes:[NSArray arrayWithObject:NSRunLoopCommonModes]];
    
    if (isDownload) {
        
        NSLog(@"download");
        
        speedWay = YES;
        
        //进行TCP连接
        [self SocketOpen:@"61.145.85.202" port:12345];
        
        //定义认证数组
        char dataBuffer[3] = {88,1,0};
        
        NSData *readData = [NSData dataWithBytes:dataBuffer  length:strlen(dataBuffer)];
        
        //发送认证数据
        [asyncSocket writeData:readData withTimeout:-1 tag:0];
        
        //        [NSThread detachNewThreadSelector:@selector(initTimeTest) toTarget:self withObject:nil];
        
        //接受计数定义
        recCount = 0;
        
        //打开定时器，每秒统接受数据并操作
        [NSTimer scheduledTimerWithTimeInterval:1.0 target:self selector:@selector(timerGetSpeed) userInfo:nil repeats:YES];
        
    } else {
        
        NSLog(@"upLoad");
        
        speedWay = NO;
        
        [self SocketOpen:@"61.145.85.202" port:12346];
        
        char dataBuffer[3] = {88,10,0};
        
        NSData *readData = [NSData dataWithBytes:dataBuffer length:strlen(dataBuffer)];
        
        for (int i = 0; i < 3; i++) {
            
            [asyncSocket writeData:readData withTimeout:-1 tag:0];
            
            //            NSLog(@"%d",i);
            
        }
        
        for (int j = 0; j < 1024*10; j++) {
            
            randomBuffer[j] = abs(arc4random()*254+1);
        }
        
        NSData *randData = [NSData dataWithBytes:randomBuffer length:strlen(randomBuffer)];
        
        [asyncSocket writeData:randData withTimeout:-1 tag:0];
    }
}

//打开接口
- (NSInteger)SocketOpen:(NSString *)addr port:(NSInteger)port
{
    NSError *error = nil;
    
    if (![asyncSocket isConnected])
    {
        [asyncSocket connectToHost:addr onPort:port withTimeout:1.0f error:&error];
        
        NSLog(@"connect to Host:%@ Port:%d",addr,port);
        
    }
    
    return 0;
}

//关闭接口
- (NSInteger)SocketClose
{
    if ([asyncSocket isConnected]) {
        
        [asyncSocket disconnectAfterReadingAndWriting];
        
    }
    
    return 0;
}

#pragma mark -
#pragma mark Delegate

//连接错误信息
- (void)onSocket:(AsyncSocket *)sock willDisconnectWithError:(NSError *)err
{
    NSLog(@"willDisconnectWithError:%@",err);
}

//连接端口失败
- (void)onSocketDidDisconnect:(AsyncSocket *)sock
{
    
    NSLog(@"onSocketDidDisconnect");
    
    asyncSocket = nil;
    
}

//连接端口成功
- (void)onSocket:(AsyncSocket *)sock didConnectToHost:(NSString *)host port:(UInt16)port
{
    NSLog(@"didConnectToHost");
    
    [sock readDataWithTimeout:-1 tag:0];
    
}

//接受服务器返回数据
- (void)onSocket:(AsyncSocket *)sock didReadData:(NSData *)data withTag:(long)tag
{
//    NSLog(@"%@",data);
    
    if (speedWay == YES) {
        
        //获取下载数据字节大小
        sumDataLength += [data length];
        
        //继续发送数据
        char dataBuffer[3] = {88,1,0};
        
        NSData *readData = [NSData dataWithBytes:dataBuffer  length:strlen(dataBuffer)];
        
        [sock writeData:readData withTimeout:-1 tag:0];
        
        [sock readDataWithTimeout:-1 tag:0];
        
    } else {
        
        sumDataLength += [data length];
        
        NSData *readData = [NSData dataWithBytes:randomBuffer  length:strlen(randomBuffer)];
        
        [sock writeData:readData withTimeout:-1 tag:0];
        
        [sock readDataWithTimeout:-1 tag:0];
    }
}

//利用日期差计算速度方法，由于大量占用内存，放弃使用
- (void)initTimeTest{
    
    int i = 1;
    
    NSDate *nowDate = [NSDate date];
    
    double startTime = [nowDate timeIntervalSince1970]*1000;
    
    while (i < 16) {
        
        NSDate *nowDate = [NSDate date];
        
        double time = [nowDate timeIntervalSince1970]*1000 - startTime;
        
        if ((int)time > 1000*i) {
            
            if (i > 3 && i < 14) {
                
                NSLog(@"%.3fMbps",(double)((sumDataLength*8)/1000000)/i);
                
                sumAveDataLength += (double)((sumDataLength*8)/1000000)/i;
                
            }
            
            i++;
        }
    }
    
    NSLog(@"%.3f",sumAveDataLength/10);
    
    NSLog(@"complete");
    
}

//计时器每秒统计操作
- (void)timerGetSpeed
{
    //计数值增加
    recCount++;
    
    //每秒瞬时速度
    _secSpeedStr = [NSString stringWithFormat:@"%.2f",(double)((sumDataLength*8)/1000000)/recCount];
    
    //获取中间10个值
    if (recCount > 3 && recCount < 14) {
        
        //中间10秒接受字节总长
        sumAveDataLength += (double)((sumDataLength*8)/1000000)/recCount;
        
    } else if (recCount == 15) {
        
        //统计完毕，计算平均值
        _avgSpeedStr = [NSString stringWithFormat:@"%.2f",sumAveDataLength/10];
        
        NSLog(@"complete");
        
        //接受完成后关闭接口
        [asyncSocket disconnectAfterReadingAndWriting];
    }
    
}


@end
