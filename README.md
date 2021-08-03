# Queen SDK

[![Version](https://img.shields.io/cocoapods/v/Queen.svg?style=flat)](https://cocoapods.org/pods/Queen)
[![License](https://img.shields.io/cocoapods/l/Queen.svg?style=flat)](https://cocoapods.org/pods/Queen)
[![Platform](https://img.shields.io/cocoapods/p/Queen.svg?style=flat)](https://cocoapods.org/pods/Queen)

## 简介
- 本文为您介绍iOS端[阿里云Queen SDK](https://help.aliyun.com/document_detail/211047.html?spm=a2c4g.11186623.6.735.6a1b192eB31nYi)的接口文档、集成操作及简单使用示例，用于实现美颜特效功能。

### 设备和系统版本

- iOS设备：arm64的移动设备（iPad/iPhone，不包含支持arm64架构的Mac）
- iOS系统版本：iOS 11.0及以上

## 集成Queen SDK

支持pods与本地集成两种方式。

### pods集成方式：
```ruby
pod 'Queen', '1.2.0-official-pro'
```
### 本地集成方式：

1. 下载并解压Sample示例工程，获取以下framework文件:
```
queen.framework
FaceDetection.framework
Face3D.framework
opencv2.framework
pixelai.framework
MNN.framework
bokeh_ios.framework
```
2. 打开Xcode，在工程target的General页签下，在Frameworks, Libraries, and Embedded Content区域中添加以上framework。其中，opencv2.framework和bokeh_ios.framework的Embed属性设置成Embed & Sign，其他framework的Embed属性设置成Do Not Embed。
3. 在Frameworks, Libraries, and Embedded Content区域中添加以下系统依赖。
```
libc++.tbd
libcompression.tbd
Metal.framework
Accelerate.framework
QuartzCore.framework
OpenGLES.framework
CoreMedia.framework
CoreMotion.framework
CoreImage.framework
Foundation.framework
AssetsLibrary.framework
CoreGraphics.framework
CoreVideo.framework
CoreML.framework
```
4. 分别将获取到的queen.framework文件中的queen-ios.Bundle、Face3D.framework文件中的face3d_res.Bundle添加到工程目录中。

## 使用示例

```objc
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
    [self testDebug];
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
}

- (void)testFaceMakeup
{
    // 打开美妆功能开关
    [self.beautyEngine setQueenBeautyType:kQueenBeautyTypeMakeup enable:YES];

    // 设置整妆资源，也可以是资源的绝对路径
    [self.beautyEngine setMakeupWithType:kQueenBeautyMakeupTypeWhole paths:@[@"makeup/活力妆.png"] blendType:kQueenBeautyBlendNormal];
    // 设置高光资源，也可以是资源的绝对路径
    [self.beautyEngine setMakeupWithType:kQueenBeautyMakeupTypeHighlight paths:@[@"makeup/highlight.png"] blendType:kQueenBeautyBlendOverlay];

//    // 取消设置整妆资源
//    [self.beautyEngine setMakeupWithType:kQueenBeautyMakeupTypeWhole paths:nil blendType:kQueenBeautyBlendNormal];
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
    [self.beautyEngine setLutImagePath:@"lookups/lookup_1.png"];
    // 设置滤镜强度
    [self.beautyEngine setQueenBeautyParams:kQueenBeautyParamsLUT value:0.8f];
}

- (void)testSticker
{
    // 添加贴纸，也可以是资源的绝对路径
    [self.beautyEngine addMaterialWithPath:@"sticker/1"];

//    // 添加贴纸，贴纸图层从下往上叠加
//    [self.beautyEngine addMaterialWithPath:@"sticker/2"];

//    // 删除贴纸
//    [self.beautyEngine removeMaterialWithPath:@"sticker/1"];
//    [self.beautyEngine removeMaterialWithPath:@"sticker/2"];
}

- (void)testGreenScreenOrBlueScreenCutout
{
    // 开启绿幕抠图功能
    NSString *backgroundImgPath = @"background/red.png";//也可以是资源的绝对路径
    BOOL enableBlue = NO;
    float threshold = 1.0f;
    [self.beautyEngine setGreenScreen:backgroundImgPath blueScreenEnabled:enableBlue threshold:threshold autoThresholdEnabled:NO];
    // 开启蓝幕抠图功能
    enableBlue = YES;
    [self.beautyEngine setGreenScreen:backgroundImgPath blueScreenEnabled:enableBlue threshold:threshold autoThresholdEnabled:NO];
    // 取消抠图功能
    [self.beautyEngine setGreenScreen:nil blueScreenEnabled:enableBlue threshold:threshold autoThresholdEnabled:NO];
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
    [self.beautyEngine showFaceDetectPoint:YES];
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
```

## License

Queen is available under the Apache License, Version 2.0. See the LICENSE file for more info.
