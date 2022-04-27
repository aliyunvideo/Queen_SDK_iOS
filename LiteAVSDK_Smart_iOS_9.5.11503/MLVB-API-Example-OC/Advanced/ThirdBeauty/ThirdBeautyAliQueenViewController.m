//
//  ThirdBeautyAliQueenViewController.m
//  MLVB-API-Example-OC
//
//  Created by ali on 2022/4/26.
//

/*
 第三方美颜功能示例
 MLVB APP 支持第三方美颜功能
 本文件展示如何集成第三方美颜功能
 1、打开扬声器 API:[self.livePusher startMicrophone];
 2、打开摄像头 API: [self.livePusher startCamera:true];
 3、开始推流 API：[self.livePusher startPush:url];
 4、开启自定义视频处理 API: [self.livePusher enableCustomVideoProcess:true pixelFormat:V2TXLivePixelFormatNV12 bufferType:V2TXLiveBufferTypePixelBuffer];
 5、使用第三方美颜SDK<Demo中使用的是AliyunQueen>: API: [self.beautyEngine processPixelBuffer:bufferData];
 
 参考文档：https://cloud.tencent.com/document/product/647/34066
 第三方美颜：https://github.com/aliyunvideo/Queen_SDK_iOS
 */
/*
 Third-Party Beauty Filter Example
 The MLVB app supports third-party beauty filters.
 This document shows how to integrate third-party beauty filters.
 1. Turn speaker on: [self.livePusher startMicrophone]
 2. Turn camera on: [self.livePusher startCamera:true]
 3. Start publishing: [self.livePusher startPush:url]
 4. Enable custom video processing: [self.livePusher enableCustomVideoProcess:true pixelFormat:V2TXLivePixelFormatNV12 bufferType:V2TXLiveBufferTypePixelBuffer]
 5. Use a third-party beauty filter SDK (AliyunQueen is used in the demo): [self.beautyEngine processPixelBuffer:bufferData]
 
 Documentation: https://cloud.tencent.com/document/product/647/34066
 Third-party beauty filter: https://github.com/aliyunvideo/Queen_SDK_iOS
 */

#define ENABLE_ALIYUN_QUEEN_BEAUTY 0

#import "ThirdBeautyAliQueenViewController.h"
#if ENABLE_ALIYUN_QUEEN_BEAUTY
#import <queen/queen.h>
#endif

@interface ThirdBeautyAliQueenViewController () <V2TXLivePusherObserver>
@property (weak, nonatomic) IBOutlet UILabel *setBeautyLabel;
@property (weak, nonatomic) IBOutlet UILabel *beautyNumLabel;
@property (weak, nonatomic) IBOutlet UISlider *setBeautySlider;

@property (weak, nonatomic) IBOutlet UILabel *streamIdLabel;
@property (weak, nonatomic) IBOutlet UITextField *streamIdTextField;

@property (weak, nonatomic) IBOutlet UIButton *startPushStreamButton;
@property (weak, nonatomic) IBOutlet NSLayoutConstraint *bottomConstraint;

@property (strong, nonatomic) V2TXLivePusher *livePusher;
#if ENABLE_ALIYUN_QUEEN_BEAUTY
@property (nonatomic, strong) QueenEngine *beautyEngine;
#endif

@end

@implementation ThirdBeautyAliQueenViewController

- (V2TXLivePusher *)livePusher {
    if (!_livePusher) {
        _livePusher = [[V2TXLivePusher alloc] initWithLiveMode:V2TXLiveMode_RTC];
        [_livePusher setObserver:self];
    }
    return _livePusher;
}

- (void)viewDidLoad {
    [super viewDidLoad];
    [self setupDefaultUIConfig];
    [self addKeyboardObserver];
}

- (void)viewDidAppear:(BOOL)animated {
    [self setupBeautySDK];
}

- (void)setupDefaultUIConfig {
    self.streamIdTextField.text = [NSString generateRandomStreamId];
    
    self.streamIdLabel.text = Localize(@"MLVB-API-Example.ThirdBeauty.streamIdInput");
    self.streamIdLabel.adjustsFontSizeToFitWidth = true;
    
    self.startPushStreamButton.backgroundColor = [UIColor themeBlueColor];
    [self.startPushStreamButton setTitle:Localize(@"MLVB-API-Example.ThirdBeauty.startPush") forState:UIControlStateNormal];
    [self.startPushStreamButton setTitle:Localize(@"MLVB-API-Example.ThirdBeauty.stopPush") forState:UIControlStateSelected];
    
    self.setBeautyLabel.text = Localize(@"MLVB-API-Example.ThirdBeauty.beautyLevel");
    NSInteger value = self.setBeautySlider.value * 6;
    self.beautyNumLabel.text = [NSString stringWithFormat:@"%ld",value];
}

- (void)setupBeautySDK {
    [self setupQueenEngine];
}

- (void)startPush:(NSString*)streamId {
    self.title = streamId;
    [self.livePusher setRenderView:self.view];
    [self.livePusher startCamera:true];
    [self.livePusher startMicrophone];
    
    [self.livePusher enableCustomVideoProcess:true pixelFormat:V2TXLivePixelFormatNV12 bufferType:V2TXLiveBufferTypePixelBuffer];
    
    [self.livePusher startPush:[URLUtils generateTRTCPushUrl:streamId]];
}

- (void)stopPush {
    [self.livePusher stopCamera];
    [self.livePusher stopMicrophone];
    [self.livePusher stopPush];
}

#pragma mark - IBActions
- (IBAction)onPushStreamClick:(UIButton *)sender {
    sender.selected = !sender.selected;
    if (sender.selected) {
        [self startPush:self.streamIdTextField.text];
    } else {
        [self stopPush];
    }
}

#pragma mark - Slider ValueChange
- (IBAction)setBeautySliderValueChange:(UISlider *)sender {
    [self updateSkinBuffForQueenEngine:sender.value];
    NSInteger value = sender.value * 6;
    self.beautyNumLabel.text = [NSString stringWithFormat:@"%ld",value];
}

#pragma mark - V2TXLivePusherObserver
- (void)onProcessVideoFrame:(V2TXLiveVideoFrame *)srcFrame dstFrame:(V2TXLiveVideoFrame *)dstFrame {
    [self processVideoFrameByQueen:srcFrame];
    dstFrame.pixelBuffer = srcFrame.pixelBuffer;
}

- (void)touchesBegan:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event {
    [self.view endEditing:true];
}

- (void)dealloc {
    [self releaseQueenEngine];
    [self removeKeyboardObserver];
}

#pragma mark - Notification
- (void)addKeyboardObserver {
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(keyboardWillShow:) name:UIKeyboardWillShowNotification object:nil];
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(keyboardWillHide:) name:UIKeyboardWillHideNotification object:nil];
}

- (void)removeKeyboardObserver {
    [[NSNotificationCenter defaultCenter] removeObserver:self name:UIKeyboardWillShowNotification object:nil];
    [[NSNotificationCenter defaultCenter] removeObserver:self name:UIKeyboardWillHideNotification object:nil];
}

- (BOOL)keyboardWillShow:(NSNotification *)noti {
    CGFloat animationDuration = [[[noti userInfo] objectForKey:UIKeyboardAnimationDurationUserInfoKey] floatValue];
    CGRect keyboardBounds = [[[noti userInfo] objectForKey:UIKeyboardFrameEndUserInfoKey] CGRectValue];
    [UIView animateWithDuration:animationDuration animations:^{
        self.bottomConstraint.constant = keyboardBounds.size.height;
    }];
    return YES;
}

- (BOOL)keyboardWillHide:(NSNotification *)noti {
    CGFloat animationDuration = [[[noti userInfo] objectForKey:UIKeyboardAnimationDurationUserInfoKey] floatValue];
    [UIView animateWithDuration:animationDuration animations:^{
        self.bottomConstraint.constant = 25;
    }];
    return YES;
}

#pragma mark - Queen

- (void)setupQueenEngine
{
#if ENABLE_ALIYUN_QUEEN_BEAUTY
    if (self.beautyEngine)
    {
        return;
    }
    
    NSString *bundlPath = [[NSBundle mainBundle] bundlePath];
    QueenEngineConfigInfo *configInfo = [QueenEngineConfigInfo new];
    configInfo.resRootPath = [bundlPath stringByAppendingString:@"/res"];
    configInfo.runOnCustomThread = NO;
#error please input licenseKey and licenseFile for your bundleID 
    configInfo.licenseKey = @"";
    configInfo.licenseFile = @"";
    self.beautyEngine = [[QueenEngine alloc] initWithConfigInfo:configInfo];
#endif    
}

- (void)updateSkinBuffForQueenEngine:(float)value
{
#if ENABLE_ALIYUN_QUEEN_BEAUTY
    [self.beautyEngine setQueenBeautyType:kQueenBeautyTypeSkinBuffing enable:YES];
    [self.beautyEngine setQueenBeautyParams:kQueenBeautyParamsSkinBuffing value:value];
#endif    
}

- (void)processVideoFrameByQueen:(V2TXLiveVideoFrame *)videoFrame
{
#if ENABLE_ALIYUN_QUEEN_BEAUTY
    QEPixelBufferData *bufferData = [QEPixelBufferData new];
    bufferData.bufferIn = videoFrame.pixelBuffer;
    bufferData.bufferOut = videoFrame.pixelBuffer;    
    [self.beautyEngine processPixelBuffer:bufferData];
#endif    
}

- (void)releaseQueenEngine
{
#if ENABLE_ALIYUN_QUEEN_BEAUTY
    [self.beautyEngine destroyEngine];
    self.beautyEngine = nil;
#endif
}

@end
