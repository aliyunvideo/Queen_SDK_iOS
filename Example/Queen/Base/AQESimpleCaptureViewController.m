//
//  AQESimpleCaptureViewController.m
//  Queen_Example
//
//  Created by Aliyun on 2021/3/14.
//  Copyright © 2021 Alibaba-inc. All rights reserved.
//

#import "AQESimpleCaptureViewController.h"
#import <CoreMedia/CoreMedia.h>
#import "AQESimpleCapture.h"
#import "AQEDisplayView.h"

@interface AQESimpleCaptureViewController () <AQESimpleCaptureDelegate>

@property (nonatomic, strong) AQESimpleCapture *capture;
@property (nonatomic, strong) AQEDisplayView *displayView;

@property (nonatomic, strong) NSThread *processPixelThread;
@property (nonatomic, assign) BOOL processPixelThreadActived;

@property (nonatomic, assign) CVPixelBufferRef processedPixelBufferRef;

@end

@implementation AQESimpleCaptureViewController

- (void)viewDidLoad
{
    [super viewDidLoad];
    self.view.backgroundColor = [UIColor blackColor];
    
    [self initSubViews];
    [self initNotification];
    [self startProcessPixelThread];
}

- (void)viewDidAppear:(BOOL)animated
{
    [super viewDidAppear:animated];
    [self startCapture];
}

- (void)viewWillDisappear:(BOOL)animated
{
    [super viewWillDisappear:animated];
    [self stopCapture];
}

- (void)dealloc
{
    [[NSNotificationCenter defaultCenter] removeObserver:self];
    [[NSNotificationCenter defaultCenter] removeObserver:_displayView];
    [self stopCapture];
}

- (void)initSubViews
{
    _capture = [[AQESimpleCapture alloc] init];
    [_capture initCaptureSession];
    _capture.delegate = self;
    
    _displayView = [[AQEDisplayView alloc] initWithFrame:self.view.bounds];
    [self.view addSubview:_displayView];
    
    UIButton *button = [UIButton buttonWithType:UIButtonTypeCustom];
    button.backgroundColor = [[UIColor blackColor] colorWithAlphaComponent:0.2f];
    CGFloat buttonWidth = 50.0f;
    button.frame = CGRectMake(CGRectGetWidth(self.view.bounds) - buttonWidth - 20.0f,
                              (CGRectGetHeight(self.view.bounds) - buttonWidth) / 2,
                              buttonWidth,
                              buttonWidth);
    [button setTitle:@"close" forState:UIControlStateNormal];
    [button addTarget:self action:@selector(onCloseBtnClick) forControlEvents:UIControlEventTouchUpInside];
    [self.view addSubview:button];
}

- (void)onCloseBtnClick
{
    [self stopProcessPixelThread];
    [self dismissViewControllerAnimated:YES completion:nil];
}

- (void)initNotification
{
    [[NSNotificationCenter defaultCenter] addObserver:self  selector:@selector(startCapture) name:UIApplicationWillEnterForegroundNotification object:nil];
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(startCapture) name:UIApplicationDidBecomeActiveNotification object:nil];
    [[NSNotificationCenter defaultCenter] addObserver:self  selector:@selector(stopCapture) name:UIApplicationWillResignActiveNotification object:nil];
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(stopCapture) name:UIApplicationDidEnterBackgroundNotification object:nil];
    [[NSNotificationCenter defaultCenter] addObserver:_displayView selector:@selector(flushAndRemoveImage) name:UIApplicationDidEnterBackgroundNotification object:nil];
}

- (void)startCapture
{
    [_capture startSession];
    [self captureBegin];
}

- (void)stopCapture
{
    [_capture stopSession];
    [self captureEnd];
}

- (void)captureBegin
{
    ;
}

- (void)captureEnd
{
    ;
}

- (void)captureReset
{
    ;
}

- (CVPixelBufferRef)getProcessedPixelBufferRefWithCurrentPixelBufferRef:(CVPixelBufferRef)pixelBufferRef
{
    return nil;
}

- (void)processPixelBufferRef:(CVPixelBufferRef)pixelBufferRef
{
    self.processedPixelBufferRef = [self getProcessedPixelBufferRefWithCurrentPixelBufferRef:pixelBufferRef];
}

#pragma mark - AQESimpleCaptureDelegate

- (void)outputSampleBuffer:(CMSampleBufferRef)sampleBuffer
{
    if (_displayView)
    {
        if (self.processPixelThreadActived)
        {
            CVPixelBufferRef pixelBufferRef = CMSampleBufferGetImageBuffer(sampleBuffer);
            [self performSelector:@selector(processPixelBufferRef:) onThread:self.processPixelThread withObject:(__bridge id _Nullable)(pixelBufferRef) waitUntilDone:YES];
            CVPixelBufferRef processedPixelBufferRef = self.processedPixelBufferRef;
            if (processedPixelBufferRef && processedPixelBufferRef != pixelBufferRef)
            {
                [self.displayView enqueuePixelBuffer:processedPixelBufferRef withOriSampleBuffer:sampleBuffer];
                CFRelease(processedPixelBufferRef);
            }
            else
            {
                [self.displayView enqueueSampleBuffer:sampleBuffer];
            }
        }
        else
        {
            [self.displayView enqueueSampleBuffer:sampleBuffer];
        }
    }
}

#pragma mark - RenderThreadDo

- (void)startProcessPixelThread
{
    if (!_processPixelThread)
    {
        self.processPixelThreadActived = YES;
        if (@available(iOS 10.0, *))
        {
            __weak typeof(self) weakSelf = self;
            _processPixelThread = [[NSThread alloc] initWithBlock:^{
                [weakSelf initPixelThread:weakSelf];
            }];
        }
        else
        {
            _processPixelThread = [[NSThread alloc] initWithTarget:self selector:@selector(initPixelThread:) object:self];
        }
        [_processPixelThread start];
    }
}

- (void)initPixelThread:(id)object
{
    AQESimpleCaptureViewController *vc = (AQESimpleCaptureViewController *)object;
    [[NSRunLoop currentRunLoop] addPort:[[NSPort alloc] init] forMode:NSDefaultRunLoopMode];
    while (vc && vc.processPixelThreadActived) {
        [[NSRunLoop currentRunLoop] runMode:NSDefaultRunLoopMode beforeDate:[NSDate distantFuture]];
    }
}

- (void)stopProcessPixelThread
{
    [self performSelector:@selector(resetPixelThread)
                 onThread:self.processPixelThread
               withObject:nil
            waitUntilDone:YES];
}

- (void)resetPixelThread
{
    self.processPixelThreadActived = NO;
    CFRunLoopStop(CFRunLoopGetCurrent());
    self.processPixelThread = nil;
    [self captureReset];
}

@end

