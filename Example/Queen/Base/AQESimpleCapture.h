//
//  AQESimpleCapture.h
//  Queen_Example
//
//  Created by Aliyun on 2021/3/14.
//  Copyright © 2021 Alibaba-inc. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <AVFoundation/AVFoundation.h>
#import <CoreMedia/CoreMedia.h>

NS_ASSUME_NONNULL_BEGIN

@protocol AQESimpleCaptureDelegate <NSObject>

- (void)outputSampleBuffer:(CMSampleBufferRef)sampleBuffer;

@end

@interface AQESimpleCapture : NSObject

@property(nonatomic, weak)id <AQESimpleCaptureDelegate> delegate;

- (void)initCaptureSession;
- (void)startSession;
- (void)stopSession;

@end

NS_ASSUME_NONNULL_END

