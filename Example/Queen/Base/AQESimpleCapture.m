//
//  AQESimpleCapture.m
//  Queen_Example
//
//  Created by Aliyun on 2021/3/14.
//  Copyright © 2021 Alibaba-inc. All rights reserved.
//

#import "AQESimpleCapture.h"
#import <UIKit/UIKit.h>

@interface AQESimpleCapture () <AVCaptureVideoDataOutputSampleBufferDelegate>
@property (nonatomic, strong) AVCaptureSession *captureSession;
@property (nonatomic, strong) dispatch_queue_t captureQueue;
@end

@implementation AQESimpleCapture

- (instancetype)init
{
    self = [super init];
    if (self)
    {
        _captureQueue = dispatch_queue_create("com.alibaba.queen.capture", DISPATCH_QUEUE_SERIAL);
    }
    return self;
}

- (void)startSession
{
    if (![_captureSession isRunning])
    {
        [_captureSession startRunning];
    }
}

- (void)stopSession
{
    if ([_captureSession isRunning])
    {
        [_captureSession stopRunning];
    }
}

- (void)initCaptureSession
{
    _captureSession = [[AVCaptureSession alloc] init];
    
    [_captureSession beginConfiguration];
    
    //    if ([_captureSession canSetSessionPreset:AVCaptureSessionPreset1920x1080])
    //    {
    //        [_captureSession setSessionPreset:AVCaptureSessionPreset1920x1080];
    //    }
    //    else
    if ([_captureSession canSetSessionPreset:AVCaptureSessionPreset1280x720])
    {
        [_captureSession setSessionPreset:AVCaptureSessionPreset1280x720];
    }
    
    NSArray<AVCaptureDevice *> *devices = [AVCaptureDevice devicesWithMediaType:AVMediaTypeVideo];
    AVCaptureDevice *aDevice = [AVCaptureDevice defaultDeviceWithMediaType:AVMediaTypeVideo];
    for ( AVCaptureDevice *device in devices )
    {
        if ( device.position == AVCaptureDevicePositionFront)
        {
            aDevice = device;
            break;
        }
    }
    
    
    AVCaptureDevice *captureDevice = aDevice;
    NSCAssert(captureDevice, @"no device");
    
    NSError *error;
    AVCaptureDeviceInput *input = [[AVCaptureDeviceInput alloc] initWithDevice:captureDevice error:&error];
    [_captureSession addInput:input];
    
    //-- Create the output for the capture session.
    AVCaptureVideoDataOutput * dataOutput = [[AVCaptureVideoDataOutput alloc] init];
    [dataOutput setAlwaysDiscardsLateVideoFrames:YES]; // Probably want to set this to NO when recording
    
    for (int i = 0; i < dataOutput.availableVideoCVPixelFormatTypes.count; i++)
    {
        char fourr[5] = {0};
        *((int32_t *)fourr) = CFSwapInt32([dataOutput.availableVideoCVPixelFormatTypes[i] intValue]);
        NSLog(@"%s", fourr);
    }
    //-- Set to YUV420.
    if (true)
    {
        BOOL supportsFullYUVRange = NO;
        NSArray *supportedPixelFormats = dataOutput.availableVideoCVPixelFormatTypes;
        for (NSNumber *currentPixelFormat in supportedPixelFormats)
        {
            if ([currentPixelFormat intValue] == kCVPixelFormatType_420YpCbCr8BiPlanarFullRange)
            {
                supportsFullYUVRange = YES;
            }
        }
        
        if (supportsFullYUVRange)
        {
            [dataOutput setVideoSettings:[NSDictionary dictionaryWithObject:[NSNumber numberWithInt:kCVPixelFormatType_420YpCbCr8BiPlanarFullRange] forKey:(id)kCVPixelBufferPixelFormatTypeKey]];
        }
        else
        {
            [dataOutput setVideoSettings:[NSDictionary dictionaryWithObject:[NSNumber numberWithInt:kCVPixelFormatType_420YpCbCr8BiPlanarVideoRange] forKey:(id)kCVPixelBufferPixelFormatTypeKey]];
        }
    }
    else
    {
        // Despite returning a longer list of supported pixel formats, only RGB, RGBA, BGRA, and the YUV 4:2:2 variants seem to return cleanly
        [dataOutput setVideoSettings:[NSDictionary dictionaryWithObject:[NSNumber numberWithInt:kCVPixelFormatType_32BGRA] forKey:(id)kCVPixelBufferPixelFormatTypeKey]];
        //        [videoOutput setVideoSettings:[NSDictionary dictionaryWithObject:[NSNumber numberWithInt:kCVPixelFormatType_422YpCbCr8_yuvs] forKey:(id)kCVPixelBufferPixelFormatTypeKey]];
    }
    //[dataOutput setVideoSettings:@{(id)kCVPixelBufferPixelFormatTypeKey:@(kCVPixelFormatType_32BGRA)}];
    
    // Set dispatch to be on the main thread so OpenGL can do things with the data
    [dataOutput setSampleBufferDelegate:self queue:self.captureQueue];
    
    NSAssert([_captureSession canAddOutput:dataOutput], @"can't output");
    
    [_captureSession addOutput:dataOutput];
    
    AVCaptureConnection *videoConnection = [dataOutput connectionWithMediaType:AVMediaTypeVideo];
    if ([videoConnection isVideoOrientationSupported])
    {
        videoConnection.videoOrientation = [self actualVideoOrientation];
    }
    
    AVCaptureDevicePosition currentPosition=[[input device] position];
    if (currentPosition == AVCaptureDevicePositionUnspecified || currentPosition == AVCaptureDevicePositionFront)
    {
        videoConnection.videoMirrored = YES;
    }
    else
    {
        videoConnection.videoMirrored = NO;
    }
    
    [_captureSession commitConfiguration];
    
    if ( [captureDevice lockForConfiguration:NULL] )
    {
        captureDevice.activeVideoMaxFrameDuration = CMTimeMake( 1, 30);
        captureDevice.activeVideoMinFrameDuration = CMTimeMake( 1, 30);
        [captureDevice unlockForConfiguration];
    }
}

- (AVCaptureVideoOrientation)actualVideoOrientation
{
    AVCaptureVideoOrientation videoOrientation = AVCaptureVideoOrientationPortrait;
    UIInterfaceOrientation deviceOrientation = [[UIApplication sharedApplication] statusBarOrientation];
    switch (deviceOrientation)
    {
        case UIInterfaceOrientationLandscapeLeft:
            videoOrientation = AVCaptureVideoOrientationLandscapeRight;
            break;
        case UIInterfaceOrientationLandscapeRight:
            videoOrientation = AVCaptureVideoOrientationLandscapeLeft;
            break;
        case UIInterfaceOrientationPortrait:
            videoOrientation = AVCaptureVideoOrientationPortrait;
            break;
        case UIInterfaceOrientationPortraitUpsideDown:
            videoOrientation = AVCaptureVideoOrientationPortraitUpsideDown;
            break;
        case UIInterfaceOrientationUnknown:
            videoOrientation = AVCaptureVideoOrientationPortrait;
            break;
    }
    return videoOrientation;
}

- (void)captureOutput:(AVCaptureOutput *)captureOutput didOutputSampleBuffer:(CMSampleBufferRef)sampleBuffer
       fromConnection:(AVCaptureConnection *)connection
{
    if (_delegate && [_delegate respondsToSelector:@selector(outputSampleBuffer:)])
    {
        [_delegate outputSampleBuffer:sampleBuffer];
    }
}

@end
