//
//  AQEDisplayView.h
//  Queen_Example
//
//  Created by Aliyun on 2021/3/14.
//  Copyright © 2021 Alibaba-inc. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <AVFoundation/AVFoundation.h>

NS_ASSUME_NONNULL_BEGIN

@interface AQEDisplayView  : UIView

- (void)enqueueSampleBuffer:(CMSampleBufferRef)sampleBuffer;
- (void)enqueuePixelBuffer:(CVPixelBufferRef)pixelBuffer withOriSampleBuffer:(CMSampleBufferRef)sampleBuffer;
- (void)flushAndRemoveImage;

@end

NS_ASSUME_NONNULL_END
