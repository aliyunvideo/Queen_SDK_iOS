//
//  AQEDisplayView.m
//  Queen_Example
//
//  Created by Aliyun on 2021/3/14.
//  Copyright © 2021 Alibaba-inc. All rights reserved.
//

#import "AQEDisplayView.h"
#import <AVFoundation/AVFoundation.h>

@interface AQEDisplayView ()

@property(nonatomic, strong)AVSampleBufferDisplayLayer *displayLayer;

@end

@implementation AQEDisplayView

- (instancetype)initWithFrame:(CGRect)frame
{
    self = [super initWithFrame:frame];
    if (self)
    {
        _displayLayer = [[AVSampleBufferDisplayLayer alloc] init];
        _displayLayer.frame = self.bounds;
        _displayLayer.position = CGPointMake(CGRectGetMidX(self.bounds), CGRectGetMidY(self.bounds));
        _displayLayer.videoGravity = AVLayerVideoGravityResizeAspect;
        _displayLayer.opaque = YES;
        [self.layer addSublayer:_displayLayer];
    }
    return self;
}

- (void)enqueueSampleBuffer:(CMSampleBufferRef)sampleBuffer
{
    if (_displayLayer.status == AVQueuedSampleBufferRenderingStatusFailed)
    {
        NSLog(@"%@",_displayLayer.error);
        [_displayLayer flush];
    }
    if (sampleBuffer)
    {
        [_displayLayer enqueueSampleBuffer:sampleBuffer];
    }
}

- (void)enqueuePixelBuffer:(CVPixelBufferRef)pixelBuffer withOriSampleBuffer:(CMSampleBufferRef)sampleBuffer
{
    if (_displayLayer.status == AVQueuedSampleBufferRenderingStatusFailed)
    {
        NSLog(@"%@",_displayLayer.error);
        [_displayLayer flush];
    }
    if (pixelBuffer && sampleBuffer)
    {
        CMSampleBufferRef newSimpleBuffer = [[self class] createSampleBuffer:sampleBuffer pixelBuffer:pixelBuffer];
        if (newSimpleBuffer)
        {
            [_displayLayer enqueueSampleBuffer:newSimpleBuffer];
            CFRelease(newSimpleBuffer);
            return;
        }
    }
    if (sampleBuffer)
    {
        [_displayLayer enqueueSampleBuffer:sampleBuffer];
    }
}

- (void)flushAndRemoveImage
{
    [_displayLayer flushAndRemoveImage];
}

#pragma mark - Utils

+ (CMSampleBufferRef)createSampleBuffer:(CMSampleBufferRef)sampleBuffer
                            pixelBuffer:(CVPixelBufferRef)pixelBuffer
{
    if (!sampleBuffer) return nil;
    CMSampleBufferRef tempBuffer = nil;
    if (pixelBuffer) {
        CVPixelBufferLockBaseAddress(pixelBuffer, 0);
        CMSampleTimingInfo timingInfo = kCMTimingInfoInvalid;
        CMVideoFormatDescriptionRef videoInfo = NULL;
        CMTime t = CMSampleBufferGetPresentationTimeStamp(sampleBuffer);
        //NSLog(@"sample buffer - %f",CMTimeGetSeconds(t));
        timingInfo.presentationTimeStamp = t;
        timingInfo.duration = CMSampleBufferGetDuration(sampleBuffer);
        
        OSStatus ret = 0;
        ret = CMVideoFormatDescriptionCreateForImageBuffer(NULL, pixelBuffer, &videoInfo);
        ret = CMSampleBufferCreateForImageBuffer(kCFAllocatorDefault, pixelBuffer, true, NULL, NULL,videoInfo, &timingInfo, &tempBuffer);
        
        CVPixelBufferUnlockBaseAddress(pixelBuffer, 0);
        CVPixelBufferUnlockBaseAddress(pixelBuffer, kCVPixelBufferLock_ReadOnly);
        
        if (videoInfo) {
            CFRelease(videoInfo);
        }
    }
    return tempBuffer;
}

@end
