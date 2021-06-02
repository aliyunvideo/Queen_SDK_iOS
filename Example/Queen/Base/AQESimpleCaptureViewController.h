//
//  AQESimpleQueenExampleViewController.h
//  Queen_Example
//
//  Created by Aliyun on 2021/3/14.
//  Copyright © 2021 Alibaba-inc. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <CoreVideo/CoreVideo.h>

NS_ASSUME_NONNULL_BEGIN

@interface AQESimpleCaptureViewController : UIViewController

- (void)captureBegin;
- (void)captureEnd;
- (void)captureReset;
- (CVPixelBufferRef)getProcessedPixelBufferRefWithCurrentPixelBufferRef:(CVPixelBufferRef)pixelBufferRef;

@end

NS_ASSUME_NONNULL_END

