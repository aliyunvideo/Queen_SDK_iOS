//
//  AQESimpleQueenExampleViewController.m
//  Queen_Example
//
//  Created by Aliyun on 2021/3/14.
//  Copyright © 2021 Alibaba-inc. All rights reserved.
//

#import "AQESimpleQueenExampleViewController.h"

#define kEnableQueenEngine 1

#if kEnableQueenEngine
#import <queen/Queen.h>
#import <CoreMotion/CoreMotion.h>
#endif

@interface AQESimpleQueenExampleViewController ()

#if kEnableQueenEngine

@property (nonatomic, strong) QueenEngine *beautyEngine;

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
    NSString *bundlPath = [[NSBundle mainBundle] bundlePath];
    // 设置资源根目录
    configInfo.resRootPath = [bundlPath stringByAppendingString:@"/res"];
    
    // 引擎初始化
    self.beautyEngine = [[QueenEngine alloc] initWithConfigInfo:configInfo];
    
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
    
    // 打开美妆功能开关
    [self.beautyEngine setQueenBeautyType:kQueenBeautyTypeMakeup enable:YES];
    // 设置整妆资源
    [self.beautyEngine setMakeupWithType:kQueenBeautyMakeupTypeWhole paths:@[@"makeup/活力妆.png"] blendType:kQueenBeautyBlendNormal];
    // 设置高光资源
    [self.beautyEngine setMakeupWithType:kQueenBeautyMakeupTypeHighlight paths:@[@"makeup/highlight.png"] blendType:kQueenBeautyBlendOverlay];
    
    // 打开美型功能开关
    [self.beautyEngine setQueenBeautyType:kQueenBeautyTypeFaceShape enable:YES];
    // 设置大眼系数
    [self.beautyEngine setFaceShape:kQueenBeautyFaceShapeTypeBigEye value:1.0f];
    
    // 打开滤镜功能开关
    [self.beautyEngine setQueenBeautyType:kQueenBeautyTypeLUT enable:YES];
    // 设置滤镜强度
    [self.beautyEngine setLutImagePath:@"lookups/lookup_1.png"];
    
    // 添加贴纸
    [self.beautyEngine addMaterialWithPath:@"sticker/baiyang"];
    //    // 删除贴纸
    //    [self.beautyEngine removeMaterialWithPath:@"sticker/baiyang"];
    
    //    // 展示人脸识别特征点
    //    [self.beautyEngine showFaceDetectPoint:YES];
    //    // 展示美妆三角剖分信息
    //    [self.beautyEngine showFaceDetectPoint:YES];
}

- (CVPixelBufferRef)getProcessedPixelBufferRefWithCurrentPixelBufferRef:(CVPixelBufferRef)pixelBufferRef
{
    if (self.beautyEngine && pixelBufferRef)
    {
        QEPixelBufferData *bufferData = [QEPixelBufferData new];
        bufferData.bufferIn = pixelBufferRef;
        bufferData.bufferOut = pixelBufferRef;
        bufferData.inputAngle = self.cameraRotate; //要正确传入pixelBufferRef的方向，否则人脸识别会失败，如果不知道pixelBufferRef的方向，可参考此demo属性cameraRotate取值的方法
        bufferData.outputAngle = self.cameraRotate; //一般和inputAngle取值一样就可以了
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
        self.beautyEngine = nil;//释放queen，确保当前线程与执行processPixelBuffer:是同一条线程
    }
}

- (void)captureBegin
{
    [self startRetainCameraRotate];
}

- (void)captureEnd
{
    [self stopRetainCameraRotate];
}

#pragma mark - CameraRotate

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

@end
