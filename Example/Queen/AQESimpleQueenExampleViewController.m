//
//  AQESimpleQueenExampleViewController.m
//  Queen_Example
//
//  Created by Aliyun on 2021/3/14.
//  Copyright © 2021 Alibaba-inc. All rights reserved.
//

#import "AQESimpleQueenExampleViewController.h"

#define kEnableQueenEngine 1
#define kEnableCustomSettingImgAngle 1

#if kEnableQueenEngine
#import <queen/Queen.h>
#endif

#if kEnableCustomSettingImgAngle
#import <CoreMotion/CoreMotion.h>
#endif

@interface AQESimpleQueenExampleViewController ()

#if kEnableQueenEngine
@property (nonatomic, strong) QueenEngine *beautyEngine;
#endif

#if kEnableCustomSettingImgAngle
@property (nonatomic, strong) CMMotionManager *motionManager;
@property (nonatomic, strong) NSOperationQueue *motionObserveQueue;
@property (nonatomic, assign) int cameraRotate;
#endif

@end

@implementation AQESimpleQueenExampleViewController

#if kEnableQueenEngine

- (void)viewDidLoad
{
    [super viewDidLoad];
    [self initBeautyEngine];
}

- (void)initBeautyEngine
{
    // 初始化引擎配置信息对象
    QueenEngineConfigInfo *configInfo = [QueenEngineConfigInfo new];
    
    // 设置是否自动设置图片旋转角度，如设备锁屏，并且默认图像采集来自摄像头的话可以设置自动设置图片旋转角度
#if kEnableCustomSettingImgAngle
    configInfo.autoSettingImgAngle = NO;
#else
    configInfo.autoSettingImgAngle = YES;
#endif
    
    // 设置资源根目录
    NSString *bundlPath = [[NSBundle mainBundle] bundlePath];
    configInfo.resRootPath = [bundlPath stringByAppendingString:@"/res"];
    
    // 引擎初始化
    self.beautyEngine = [[QueenEngine alloc] initWithConfigInfo:configInfo];
    
    [self testBaseFaceBeauty];
    [self testAdvancedFaceBeauty];
    [self testFaceMakeup];
    [self testFaceShape];
    [self testFilter];
    [self testSticker];
    [self testGreenScreenOrBlueScreenCutout];
    [self testBackgroundCutout];
//    [self testDebug];
}

- (void)testBaseFaceBeauty
{
    // 打开磨皮锐化功能开关
    [self.beautyEngine setQueenBeautyType:kQueenBeautyTypeSkinBuffing enable:YES];
    
    // 设置磨皮系数
    [self.beautyEngine setQueenBeautyParams:kQueenBeautyParamsSkinBuffing value:0.5f];
    // 设置锐化系数
    [self.beautyEngine setQueenBeautyParams:kQueenBeautyParamsSharpen value:0.5f];
    
    // 打开美白功能开关
    [self.beautyEngine setQueenBeautyType:kQueenBeautyTypeSkinWhiting enable:YES];
    
    // 设置美白系数
    [self.beautyEngine setQueenBeautyParams:kQueenBeautyParamsWhitening value:0.5f];
}

- (void)testAdvancedFaceBeauty
{
    // 打开高级美颜功能开关
    [self.beautyEngine setQueenBeautyType:kQueenBeautyTypeFaceBuffing enable:YES];
    
    // 设置去眼袋系数
    [self.beautyEngine setQueenBeautyParams:kQueenBeautyParamsPouch value:0.5f];
    // 设置去法令纹系数
    [self.beautyEngine setQueenBeautyParams:kQueenBeautyParamsNasolabialFolds value:0.5f];
    // 设置白牙系数
    [self.beautyEngine setQueenBeautyParams:kQueenBeautyParamsWhiteTeeth value:0.5f];
    // 设置口红系数
    [self.beautyEngine setQueenBeautyParams:kQueenBeautyParamsLipstick value:0.5f];
    // 设置腮红系数
    [self.beautyEngine setQueenBeautyParams:kQueenBeautyParamsBlush value:0.5f];
    // 设置口红色相系数
    [self.beautyEngine setQueenBeautyParams:kQueenBeautyParamsLipstickColorParam value:0.1f];
    // 设置口红饱和度系数
    [self.beautyEngine setQueenBeautyParams:kQueenBeautyParamsLipstickGlossParam value:0.5f];
    // 设置口红明度系数
    [self.beautyEngine setQueenBeautyParams:kQueenBeautyParamsLipstickBrightnessParam value:0.5f];
    // 设置亮眼系数
    [self.beautyEngine setQueenBeautyParams:kQueenBeautyParamsBrightenEye value:0.5f];
    // 设置红润系数
    [self.beautyEngine setQueenBeautyParams:kQueenBeautyParamsSkinRed value:0.5f];
    // 设置去皱纹系数
    [self.beautyEngine setQueenBeautyParams:kQueenBeautyParamsWrinkles value:0.2f];
    // 设置去暗沉系数
    [self.beautyEngine setQueenBeautyParams:kQueenBeautyParamsBrightenFace value:0.2f];
}

- (void)testFaceMakeup
{
    // 打开美妆功能开关
    [self.beautyEngine setQueenBeautyType:kQueenBeautyTypeMakeup enable:YES];
    
    // 设置美妆整妆效果，资源路径也可以是资源的绝对路径
    [self.beautyEngine setMakeupWithType:kQueenBeautyMakeupTypeWhole paths:@[@"makeup/huoli.png"] blendType:kQueenBeautyBlendLabMix];
    
//    // 设置美妆高光效果，资源路径也可以是资源的绝对路径
//    [self.beautyEngine setMakeupWithType:kQueenBeautyMakeupTypeHighlight paths:@[@"makeup/highlight.png"] blendType:kQueenBeautyBlendOverlay];
//    // 设置美妆美瞳效果，资源路径也可以是资源的绝对路径
//    [self.beautyEngine setMakeupWithType:kQueenBeautyMakeupTypeEyeball paths:@[@"makeup/eyeball.png"] blendType:kQueenBeautyBlendLabMix];
//    // 设置美妆口红效果，资源路径也可以是资源的绝对路径
//    [self.beautyEngine setMakeupWithType:kQueenBeautyMakeupTypeMouth paths:@[@"makeup/mouth.png"] blendType:kQueenBeautyBlendLabMix];
//    // 设置美妆卧蚕效果，资源路径也可以是资源的绝对路径
//    [self.beautyEngine setMakeupWithType:kQueenBeautyMakeupTypeWocan paths:@[@"makeup/wocan.png"] blendType:kQueenBeautyBlendCurve];
//    // 设置美妆眼妆效果，资源路径也可以是资源的绝对路径
//    [self.beautyEngine setMakeupWithType:kQueenBeautyMakeupTypeEyeBrow paths:@[@"makeup/eyebrow.png"] blendType:kQueenBeautyBlendLabMix];
//    // 设置美妆腮红效果，资源路径也可以是资源的绝对路径
//    [self.beautyEngine setMakeupWithType:kQueenBeautyMakeupTypeBlush paths:@[@"makeup/blush_daizi.png"] blendType:kQueenBeautyBlendLabMix];

//    // 清除美妆效果
//    [self.beautyEngine resetAllMakeupType];
}

- (void)testFaceShape
{
    // 打开美型功能开关
    [self.beautyEngine setQueenBeautyType:kQueenBeautyTypeFaceShape enable:YES];
    
    // 设置大眼系数
    [self.beautyEngine setFaceShape:kQueenBeautyFaceShapeTypeBigEye value:1.0f];
    // 设置发际线系数
    [self.beautyEngine setFaceShape:kQueenBeautyFaceShapeTypeHairLine value:1.0f];
    // 设置嘴角上扬(微笑)系数
    [self.beautyEngine setFaceShape:kQueenBeautyFaceShapeTypeSmile value:1.0f];
}

- (void)testFilter
{
    // 打开滤镜功能开关
    [self.beautyEngine setQueenBeautyType:kQueenBeautyTypeLUT enable:YES];
    
    // 设置滤镜资源，也可以是资源的绝对路径
    [self.beautyEngine setLutImagePath:@"lookups/ly1.png"];
    // 设置滤镜强度
    [self.beautyEngine setQueenBeautyParams:kQueenBeautyParamsLUT value:0.8f];
}

- (void)testSticker
{
    // 添加贴纸，也可以是资源的绝对路径
    [self.beautyEngine addMaterialWithPath:@"sticker/1"];
//    // 添加贴纸，贴纸图层从下往上叠加
//    [self.beautyEngine addMaterialWithPath:@"sticker/2"];
//    // 删除贴纸
//    [self.beautyEngine removeMaterialWithPath:@"sticker/1"];
//    [self.beautyEngine removeMaterialWithPath:@"sticker/2"];
}

- (void)testGreenScreenOrBlueScreenCutout
{
    // 开启绿幕抠图功能
    NSString *backgroundImgPath = @"background/red.png";//也可以是资源的绝对路径
    BOOL enableBlue = NO;
    float threshold = 0;
    BOOL autoThreshold = YES;
    [self.beautyEngine setGreenScreen:backgroundImgPath blueScreenEnabled:enableBlue threshold:threshold autoThresholdEnabled:autoThreshold];
    
//    // 开启蓝幕抠图功能
//    enableBlue = YES;
//    [self.beautyEngine setGreenScreen:backgroundImgPath blueScreenEnabled:enableBlue threshold:threshold autoThresholdEnabled:autoThreshold];
    
//    // 取消幕布抠图功能
//    [self.beautyEngine setGreenScreen:nil blueScreenEnabled:enableBlue threshold:threshold autoThresholdEnabled:autoThreshold];
}

- (void)testBackgroundCutout
{
//    // 人像背景虚化开启
//    [self.beautyEngine setQueenBeautyType:kQueenBeautyTypeBackgroundProcess enable:YES];
//    // 人像背景虚化关闭
//    [self.beautyEngine setQueenBeautyType:kQueenBeautyTypeBackgroundProcess enable:NO];
//
    NSString *backgroundResPath = @"background/static_changlang";//也可以是资源的绝对路径
    // 替换人像背景为静态图，相同资源不能重复添加
    [self.beautyEngine addMaterialWithPath:backgroundResPath];
//    // 取消人像背景设置为静态图
//    [self.beautyEngine removeMaterialWithPath:backgroundResPath];
}

- (void)testDebug
{
    // 展示人脸识别特征点
    [self.beautyEngine showFaceDetectPoint:YES];
    // 展示美妆三角剖分信息, 需要先开启美妆功能
    [self.beautyEngine showMakeupLine:YES];
}

- (CVPixelBufferRef)getProcessedPixelBufferRefWithCurrentPixelBufferRef:(CVPixelBufferRef)pixelBufferRef
{
    if (self.beautyEngine && pixelBufferRef)
    {
        QEPixelBufferData *bufferData = [QEPixelBufferData new];
        bufferData.bufferIn = pixelBufferRef;
        bufferData.bufferOut = pixelBufferRef;
#if kEnableCustomSettingImgAngle
        bufferData.inputAngle = self.cameraRotate; //要正确传入pixelBufferRef的方向，否则人脸识别会失败，如果不知道pixelBufferRef的方向，可参考此demo属性cameraRotate取值的方法
        bufferData.outputAngle = self.cameraRotate; //一般和inputAngle取值一样就可以了
#endif
        // 对pixelBuffer进行图像处理，输出处理后的buffer
        kQueenResultCode resultCode = [self.beautyEngine processPixelBuffer:bufferData];//执行此方法的线程需要始终是同一条线程
        if (resultCode == kQueenResultCodeOK && bufferData.bufferOut)
        {
            return bufferData.bufferOut;
        }
        else if (resultCode == kQueenResultCodeInvalidLicense)
        {
            NSLog(@"license校验失败。");
        }
        else if (resultCode == kQueenResultCodeInvalidParam)
        {
            NSLog(@"非法参数");
        }
        else if (resultCode == kQueenResultCodeNoEffect)
        {
            NSLog(@"没有开启任何特效");
        }
        return pixelBufferRef;
    }
    else
    {
        return pixelBufferRef;
    }
}

- (void)captureReset
{
    if (self.beautyEngine)
    {
        //释放queen，确保当前线程与执行processPixelBuffer:是同一条线程
        [self.beautyEngine destroyEngine];
        self.beautyEngine = nil;
    }
}

- (void)captureBegin
{
#if kEnableCustomSettingImgAngle
    [self startRetainCameraRotate];
#endif
}

- (void)captureEnd
{
#if kEnableCustomSettingImgAngle
    [self stopRetainCameraRotate];
#endif
}

#pragma mark - CameraRotate

#if kEnableCustomSettingImgAngle

- (void)startRetainCameraRotate
{
    //初始化全局管理对象
    if (!_motionManager)
    {
        _motionManager = [[CMMotionManager alloc] init];
    }
    if (!_motionObserveQueue)
    {
        _motionObserveQueue = [[NSOperationQueue alloc] init];
    }
    if ([self.motionManager isDeviceMotionAvailable])
    {
        self.motionManager.deviceMotionUpdateInterval = 1;
        [self.motionManager startDeviceMotionUpdatesToQueue:self.motionObserveQueue
                                                withHandler:^(CMDeviceMotion * _Nullable motion, NSError * _Nullable error) {
            // Gravity 获取手机的重力值在各个方向上的分量，根据这个就可以获得手机的空间位置，倾斜角度等
            double gravityX = motion.gravity.x;
            double gravityY = motion.gravity.y;
            double xyTheta = atan2(gravityX,gravityY)/M_PI*180.0;//手机旋转角度。
            if (xyTheta >= -45 && xyTheta <= 45) //home键在上面
            {
                self->_cameraRotate = 180;
            }
            else if (xyTheta > 45 && xyTheta < 135) //home键在左边
            {
                self->_cameraRotate = 90;
            }
            else if ((xyTheta >= 135 && xyTheta < 180) || (xyTheta >= -180 && xyTheta < -135)) //home键在下面
            {
                self->_cameraRotate = 0;
            }
            else if (xyTheta >= -135 && xyTheta < -45) //home键在右边
            {
                self->_cameraRotate = 270;
            }
            //NSLog(@"手机旋转的角度为 --- %d", self->_cameraRotate);
        }];
    }
}

- (void)stopRetainCameraRotate
{
    [self.motionManager stopDeviceMotionUpdates];
    [self.motionObserveQueue cancelAllOperations];
}

#endif

#endif

@end
