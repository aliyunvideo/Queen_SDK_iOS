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

#if kEnableQueenEngine
@interface AQESimpleQueenExampleViewController () <QueenEngineDelegate>
#else
@interface AQESimpleQueenExampleViewController ()
#endif

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
    // 填写licenseKey和licenseFile，也可以在应用的Info.plist文件加入AlivcLicenseKey字段和AlivcLicenseFile字段，内容类型为字符串，填入相应值即可。优先级：configInfo指定>Info.plist指定
    configInfo.licenseKey = @"xoQuEoYgOHs225cwh634fa27df8d3490e88337161a4d5ee92";
    // 相当于mainBundlePath的文件路径，示例工程的crt文件放在跟目录，所以直接填写文件名即可。
    configInfo.licenseFile = @"AliVideoCert_1649317505780.crt";
    
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
//    [self testAdvancedFaceBeauty];
//    [self testFaceMakeup];
//    [self testFaceShape];
//    [self testBodyShape];
//    [self testFilter];
//    [self testSticker];
//    [self testGreenScreenOrBlueScreenCutout];
//    [self testAutoFilter];
//    [self testGestureDetect];
//    [self testBackgroundCutout];
//    [self testReHairColor];
//    [self testARWriting];
//    [self testSkinHSV];
//    [self testFaceMosaicing];
//    [self testDebug];
}

- (void)testBaseFaceBeauty
{
    // 打开磨皮锐化功能开关
    // 第三个参数为基础美颜的模式，设置为kBMSkinBuffing_Natural，则美颜的效果更自然，细节保留更多；设置为kQueenBeautyFilterModeSkinBuffing_Strong，则效果更夸张，细节去除更多。
    [self.beautyEngine setQueenBeautyType:kQueenBeautyTypeSkinBuffing enable:YES mode:kQueenBeautyFilterModeSkinBuffing_Natural];
    
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
    // 第三个参数是美妆的模式，目前仅对眉毛有作用，设置为BeautyFilterMode.kBMFaceMakeup_High，眉毛的形变会更明显，设置为kQueenBeautyFilterModeFaceMakeup_Baseline，眉毛的形变会收敛一点
    [self.beautyEngine setQueenBeautyType:kQueenBeautyTypeMakeup enable:YES mode:kQueenBeautyFilterModeFaceMakeup_Baseline];
    
    BOOL makeupWhole = false;
    
    if (makeupWhole)
    {
        // 设置美妆整妆效果，资源路径也可以是资源的绝对路径
        [self.beautyEngine setMakeupWithType:kQueenBeautyMakeupTypeWhole paths:@[@"makeup/jichu.png"] blendType:kQueenBeautyBlendLabMix];
        // 设置美妆整妆效果的透明度，目前female参数的值统一传YES/true，男性妆容还在优化中
        [self.beautyEngine setMakeupAlphaWithType:kQueenBeautyMakeupTypeWhole female:YES alpha:1.0];
    }
    else
    {
        // 设置美妆局部妆效果：（注：设置局部妆后，如果之前设置了整妆，整妆会失效。即整妆和局部妆不能共存，但每个局部妆之间可以叠加使用，而整妆设置单个素材即可实现全脸上妆，但是无法调节各部位细节）
        // 设置美妆高光效果，资源路径也可以是资源的绝对路径
        [self.beautyEngine setMakeupWithType:kQueenBeautyMakeupTypeHighlight paths:@[@"makeup/highlight/highlight.2.13.png"] blendType:kQueenBeautyBlendSoftLight];
        // 设置美妆高光效果的透明度，目前female参数的值统一传YES/true，男性妆容还在优化中
        [self.beautyEngine setMakeupAlphaWithType:kQueenBeautyMakeupTypeHighlight female:YES alpha:0.9];
        // 设置美妆口红效果，资源路径也可以是资源的绝对路径
        [self.beautyEngine setMakeupWithType:kQueenBeautyMakeupTypeMouth paths:@[@"makeup/mouth_yaochun/shiliuhong.3.3.png"] blendType:kQueenBeautyBlendMultiply];
        // 设置美妆口红效果的透明度，目前female参数的值统一传YES/true，男性妆容还在优化中
        [self.beautyEngine setMakeupAlphaWithType:kQueenBeautyMakeupTypeMouth female:YES alpha:0.8];
        // 设置美妆卧蚕效果，目前采用内置素材，不支持定制
        [self.beautyEngine setMakeupWithType:kQueenBeautyMakeupTypeWocan paths:@[@"makeup/wocan.png"] blendType:kQueenBeautyBlendCurve];
        // 设置美妆卧蚕效果的透明度，目前female参数的值统一传YES/true，男性妆容还在优化中
        [self.beautyEngine setMakeupAlphaWithType:kQueenBeautyMakeupTypeWocan female:YES alpha:0.2];
        // 设置美妆眉毛效果，资源路径也可以是资源的绝对路径
        [self.beautyEngine setMakeupWithType:kQueenBeautyMakeupTypeEyeBrow paths:@[@"makeup/eyebrow/cupingmei.2.3.png"] blendType:kQueenBeautyBlendMultiply];
        // 设置美妆眉毛效果的透明度，目前female参数的值统一传YES/true，男性妆容还在优化中
        [self.beautyEngine setMakeupAlphaWithType:kQueenBeautyMakeupTypeEyeBrow female:YES alpha:0.3];
        // 设置美妆腮红效果，资源路径也可以是资源的绝对路径
        [self.beautyEngine setMakeupWithType:kQueenBeautyMakeupTypeBlush paths:@[@"makeup/blush_color7/shaonv.2.3.png"] blendType:kQueenBeautyBlendMultiply];
        // 设置美妆腮红效果的透明度，目前female参数的值统一传YES/true，男性妆容还在优化中
        [self.beautyEngine setMakeupAlphaWithType:kQueenBeautyMakeupTypeBlush female:YES alpha:0.7];
        // 设置美妆眼影效果，资源路径也可以是资源的绝对路径
        [self.beautyEngine setMakeupWithType:kQueenBeautyMakeupTypeEyeShadow paths:@[@"makeup/eyeshadow/pengkezong.3.3.png"] blendType:kQueenBeautyBlendMultiply];
        // 设置美妆眼影效果的透明度，目前female参数的值统一传YES/true，男性妆容还在优化中
        [self.beautyEngine setMakeupAlphaWithType:kQueenBeautyMakeupTypeEyeShadow female:YES alpha:0.8];
        // 设置美妆眼线效果，资源路径也可以是资源的绝对路径
        [self.beautyEngine setMakeupWithType:kQueenBeautyMakeupTypeEyeliner paths:@[@"makeup/eyeliner_292929/xiaoyemao.2.3.png"] blendType:kQueenBeautyBlendMultiply];
        // 设置美妆眼线效果的透明度，目前female参数的值统一传YES/true，男性妆容还在优化中
        [self.beautyEngine setMakeupAlphaWithType:kQueenBeautyMakeupTypeEyeliner female:YES alpha:0.75];
        // 设置美妆睫毛效果，资源路径也可以是资源的绝对路径
        [self.beautyEngine setMakeupWithType:kQueenBeautyMakeupTypeEyelash paths:@[@"makeup/eyelash/ziran.2.3.png"] blendType:kQueenBeautyBlendMultiply];
        // 设置美妆睫毛效果的透明度，目前female参数的值统一传YES/true，男性妆容还在优化中
        [self.beautyEngine setMakeupAlphaWithType:kQueenBeautyMakeupTypeEyelash female:YES alpha:0.75];
    }
    
//    // 清除美妆效果
//    [self.beautyEngine resetAllMakeupType];
    
//    // 关闭美妆功能开关
//    [self.beautyEngine setQueenBeautyType:kQueenBeautyTypeMakeup enable:NO];

/*
    建议采用组合妆来替换整妆的效果，可以调节各部分细节，下面提供几种组合妆的模式：
    1、素颜妆：
     睫毛（makeup/eyelash/ziran.2.3.png 透明度：0.75）、
     眼线（makeup/eyeliner_292929/xiaoyemao.2.3.png 透明度：0.75）、
     眼影（makeup/eyeshadow/pengkezong.3.3.png 透明度：0.8）、
     腮红（makeup/blush_color7/shaonv.2.3.png 透明度：0.7）、
     眉毛（makeup/eyebrow/cupingmei.2.3.png 透明度：0.3）、
     口红（makeup/mouth_yaochun/shiliuhong.3.3.png 透明度：0.8）、
     高光（makeup/highlight/highlight.2.13.png, 透明度：0.9）
    2、清纯妆：
     睫毛（makeup/eyelash/lingdong.2.3.png 透明度：0.75）、
     眼线（makeup/eyeliner_292929/juanqiao.2.3.png 透明度：0.75）、
     眼影（makeup/eyeshadow/fangtangfen.3.3.png 透明度：0.8）、
     腮红（makeup/blush_color4/chayi.2.3.png 透明度：0.7）、
     眉毛（makeup/eyebrow/biaozhunmei.2.3.png 透明度：0.3）、
     口红（makeup/mouth_wumian/chidousha.3.3.png 透明度：0.8）、
     高光（makeup/highlight/highlight.2.13.png, 透明度：0.9）
    3、复古妆：
     睫毛（makeup/eyelash/jichu.2.3.png 透明度：0.75）、
     眼线（makeup/eyeliner_292929/wenrou.2.3.png 透明度：0.75）、
     眼影（makeup/eyeshadow/yuanqicheng.3.3.png 透明度：0.8）、
     腮红（makeup/blush_color7/chunqing.2.3.png 透明度：0.7）、
     眉毛（makeup/eyebrow/juanyanmei.2.3.png 透明度：0.3）、
     口红（makeup/mouth_wumian/jiangguose.3.3.png 透明度：0.8）、
     高光（makeup/highlight/highlight.2.13.png, 透明度：0.9）
    4、熟女妆：
     睫毛（makeup/eyelash/wugu.2.3.png 透明度：0.75）、
     眼线（makeup/eyeliner_292929/qizhi.2.3.png 透明度：0.75）、
     眼影（makeup/eyeshadow/yanfense.3.3.png 透明度：0.8）、
     腮红（makeup/blush_color5/shaonv.2.3.png 透明度：0.7）、
     眉毛（makeup/eyebrow/wanyuemei.2.3.png 透明度：0.3）、
     口红（makeup/mouth_yaochun/standout.3.3.png 透明度：0.8）、
     高光（makeup/highlight/highlight.2.13.png, 透明度：0.9）
*/
}

- (void)testFaceShape
{
    // 打开美型功能开关
    // 第三个参数为美型的模式，可以设置为kQueenBeautyFilterModeFaceShape_Baseline、kQueenBeautyFilterModeFaceShape_Main、kQueenBeautyFilterModeFaceShape_High、kQueenBeautyFilterModeFaceShape_Max四种模式，形变的幅度会依次变强
    [self.beautyEngine setQueenBeautyType:kQueenBeautyTypeFaceShape enable:YES mode:kQueenBeautyFilterModeFaceShape_Main];
    
    // 设置大眼系数
    [self.beautyEngine setFaceShape:kQueenBeautyFaceShapeTypeBigEye value:1.0f];
    // 设置发际线系数
    [self.beautyEngine setFaceShape:kQueenBeautyFaceShapeTypeHairLine value:1.0f];
    // 设置嘴角上扬(微笑)系数
    [self.beautyEngine setFaceShape:kQueenBeautyFaceShapeTypeSmile value:1.0f];
}

- (void)testBodyShape
{
    // 打开美型功能开关
    [self.beautyEngine setQueenBeautyType:kQueenBeautyTypeBodyShape enable:YES];
    
    // 设置长腿系数
    [self.beautyEngine setBodyShape:kQueenBeautyBodyShapeTypeLongLag value:1.0f];
    //    // 设置小头系数
    //    [self.beautyEngine setBodyShape:kQueenBeautyBodyShapeTypeSmallHead value:1.0f];
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
    [self.beautyEngine setGreenScreen:backgroundImgPath blueScreenEnabled:enableBlue threshold:threshold autoThresholdEnabled:autoThreshold backgroundProcessType:kQueenBackgroundNormal];
    
//    // 开启蓝幕抠图功能
//    enableBlue = YES;
//    [self.beautyEngine setGreenScreen:backgroundImgPath blueScreenEnabled:enableBlue threshold:threshold autoThresholdEnabled:autoThreshold backgroundProcessType:kQueenBackgroundNormal];
    
//    // 取消幕布抠图功能
//    [self.beautyEngine setGreenScreen:nil blueScreenEnabled:enableBlue threshold:threshold autoThresholdEnabled:autoThreshold backgroundProcessType:kQueenBackgroundNormal];
}

- (void)testBackgroundCutout
{
//    // 人像背景虚化开启
//    [self.beautyEngine setQueenBeautyType:kQueenBeautyTypeBackgroundProcess enable:YES];
//    // 开启后默认是背景虚化，可以通过如下API设置为背景透明，适用于将输出当做前景，自行合成背景的场合
//    [self.beautyEngine setSegmentBackgroundProcessType:kQueenBackgroundTransparent];
//    // 人像背景虚化关闭
//    [self.beautyEngine setQueenBeautyType:kQueenBeautyTypeBackgroundProcess enable:NO];
//
    NSString *backgroundResPath = @"background/static_changlang";//也可以是资源的绝对路径
    // 替换人像背景为静态图，相同资源不能重复添加
    [self.beautyEngine addMaterialWithPath:backgroundResPath];
//    // 取消人像背景设置为静态图
//    [self.beautyEngine removeMaterialWithPath:backgroundResPath];
}

- (void)testAutoFilter
{
    // 开启智能动态优化：
    [self.beautyEngine setQueenBeautyType:kQueenBeautyTypeAutoFilter enable:YES];
//    // 关闭智能动态优化：
//    [self.beautyEngine setQueenBeautyType:kQueenBeautyTypeAutoFilter enable:NO];
}

- (void)testGestureDetect
{
    // 开启手势检测
    [self.beautyEngine setQueenBeautyType:kQueenBeautyTypeHandGestureDetect enable:YES];
//    // 关闭手势检测
//    [self.beautyEngine setQueenBeautyType:kQueenBeautyTypeHandGestureDetect enable:NO];

    // 设置代理
    self.beautyEngine.delegate = self;
}

- (void)testReHairColor
{
    // 打开换发色功能开关
    [self.beautyEngine setQueenBeautyType:kQueenBeautyTypeHairColor enable:YES];
    
    // 设置头发颜色
    [self.beautyEngine setHairColorWithRed:0.3137254901960784
                                 withGreen:0.3137254901960784
                                  withBlue:0.6274509803921569];
/*
 * 参考颜色：
 * ('blue', [0.3137254901960784, 0.3137254901960784, 0.6274509803921569])
 * ('purple', [0.6078431372549019, 0.35294117647058826, 0.6274509803921569])
 * ('sky', [0.3333333333333333, 0.5490196078431373, 0.5490196078431373])
 * ('yellow', [0.6470588235294118, 0.5294117647058824, 0.35294117647058826])
 * ('green', [0.37254901960784315, 0.5882352941176471, 0.3137254901960784])
 * ('original', [0.39215686274509803, 0.3333333333333333, 0.3137254901960784])
 * ('red', [0.5098039215686274, 0.27450980392156865, 0.27450980392156865])
 */
}

-(void)testARWriting
{
    // 打开AR隔空写字功能开关
    [self.beautyEngine setARWriting:YES mode:0];
}

- (void)testSkinHSV
{
    // 打开HSV调节功能开关
    [self.beautyEngine setQueenBeautyType:kQueenBeautyTypeHSV enable:YES];

    // 设置饱和度系数
    [self.beautyEngine setQueenBeautyParams:kQueenBeautyParamsHSVSaturation value:0.2f];
    // 设置对比度系数
    [self.beautyEngine setQueenBeautyParams:kQueenBeautyParamsHSVContrast value:0.2f];
}

- (void)testFaceMosaicing
{
    // 打开脸部马赛克功能开关
    [self.beautyEngine setQueenBeautyType:kQueenBeautyTypeFaceMosaicing enable:YES];

    // 设置马赛克大小系数
    [self.beautyEngine setQueenBeautyParams:kQueenBeautyParamsFaceMosaicing value:0.2f];
}

- (void)testDebug
{
    // 展示人脸识别特征点
    [self.beautyEngine showFaceDetectPoint:YES];
    // 展示美妆三角剖分信息, 需要先开启美妆功能
    [self.beautyEngine showMakeupLine:YES];
    // 展示手部识别特征点
    [self.beautyEngine showHandDetectPoint:YES];
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

#pragma mark - QueenEngineDelegate

- (void)queenEngine:(QueenEngine *)engine didDetectGesture:(QEGestureData *)gestureData
{
    NSLog(@"识别到手势类型：%ld，动作类型：%ld", (long)gestureData.gesture, (long)gestureData.action);
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
