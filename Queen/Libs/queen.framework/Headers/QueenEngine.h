//
//  QueenEngine.h
//  queen
//
//  Created by aliyun on 2021/3/14.
//  Copyright © 2021 alibaba-inc. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <CoreVideo/CoreVideo.h>
#import "QueenEngineConfigInfo.h"
#import <CoreMedia/CoreMedia.h>

@interface QEPixelBufferData : NSObject

/**
 * 需要处理的CVPixelBuffer。
 */
/****
 * The CVPixelBuffer that you want to process.
 */
@property (nonatomic, assign) CVPixelBufferRef bufferIn;

/**
 * 处理完后的CVPixelBuffer。
 */
/****
 * The processed CVPixelBuffer will be placed here.
 */
@property (nonatomic, assign) CVPixelBufferRef bufferOut;

/**
 * bufferIn中如果存在人脸/人体，人脸/人体顺时针旋转到正向所需要的角度，值为0/90/180/270，默认值为0。
 * 注意：仅当QueenEngineConfigInfo的autoSettingImgAngle设置为NO才会生效。
 */
/****
 * Specifies the rotation angle of the bufferIn by which the face/body (if any) in the bufferIn needs to be rotated clockwise to the frontal face/body. Examples are 0/90/180/270, default is 0.
 * Note: Only takes effect if QueenEngineConfigInfo's member variable autoSettingImgAngle is set to NO.
 */
@property (nonatomic, assign) size_t inputAngle;

/**
 * 识别到的正向人脸/人体结果需要逆时针旋转的角度，值为0/90/180/270，默认值为0。
 * 注意：仅当QueenEngineConfigInfo的autoSettingImgAngle设置为NO才会生效。一般情况下设置的值应该要和inputAngle一致，才会使得识别到的人脸/人体结果坐标位置和原图像贴合。
 */
/****
 * Specifies the rotation angle by which the detected frontal face/body needs to be rotated counterclockwise. Examples are 0/90/180/270, default is 0.
 * Note: Only takes effect if QueenEngineConfigInfo's member variable autoSettingImgAngle is set to NO. In general, the value set should be consistent with the inputAngle, so that the coordinate position of the recognized face/human body result fits the original image.
 */
@property (nonatomic, assign) size_t outputAngle;

/**
 * 识别到的人脸/人体结果坐标需要翻转的方向，默认值为kQueenBeautyFlipAxisNone。
 */
/****
 * The direction in which the detected face/body coordinates need to be flipped, the default value is kQueenBeautyFlipAxisNone.
 */
@property (nonatomic, assign) kQueenBeautyFlipAxis outputFlipAxis;

/**
 * 需要处理的CMSampleBuffer。
 */
/****
 * The CMSampleBuffer that you want to process.
 */
@property (nonatomic, assign) CMSampleBufferRef sampleBufferIn;

@end

@interface QETextureData : NSObject

/**
 * 需要处理的OpenGL纹理ID。
 */
/****
 * The OpenGL textureID that you want to process.
 */
@property (nonatomic, assign) uint32_t inputTextureID;

/**
 * 处理完后的OpenGL纹理ID。
 */
/****
 * The processed OpenGL textureID will be placed here.
 */
@property (nonatomic, assign) uint32_t outputTextureID;

/**
 * OpenGL纹理的宽度。
 */
/****
 * Texture width.
 */
@property (nonatomic, assign) int width;

/**
 * OpenGL纹理的高度。
 */
/****
 * Texture height.
 */
@property (nonatomic, assign) int height;

@end

@interface QEGestureData : NSObject

/**
 * 手势类型。
 */
/****
 * Type of detected gesture.
 */
@property (nonatomic, assign) kQueenStaticGestureType gesture;

/**
 * 动态手势类型。
 */
/****
 * Type of detected hand movements.
 */
@property (nonatomic, assign) kQueenHandActionType action;

@end

@interface QEFaceExpressionData : NSObject

/**
 * 脸部表情类型。
 */
/****
 * Type of detected face expression.
 */
@property (nonatomic, assign) kQueenFaceExpressionType expression;

@end

@interface QEAutoFaceShapeData : NSObject

/**
 * 智能美型当前状态。
 */
/****
 * The face shaping status.
 */
@property (nonatomic, assign) kQueenAutoFaceShapeStatus status;

/**
 * 围绕X轴旋转,也叫做俯仰角。
 */
/****
 * Face Yaw.
 */
@property (nonatomic, assign) float yaw;

/**
 * 围绕Y轴旋转,也叫偏航角。
 */
/****
 * Face Pitch.
 */
@property (nonatomic, assign) float pitch;

/**
 * 围绕Z轴旋转,也叫翻滚角。
 */
/****
 * Face Roll.
 */
@property (nonatomic, assign) float roll;

@end

@interface QEFaceInfoItem : NSObject

/**
 * 人脸框坐标
 */
/****
 * Face Rect.
 */
@property (nonatomic, assign) CGRect faceRect;

/**
 * 人脸欧拉角.
 */
/****
 * Euler angle.
 */
@property (nonatomic, assign) QEYawPitchRoll faceYPR;

@end

@interface QEFaceInfoData : NSObject

/**
 * 监测到人脸的个数
 */
/****
 * Face Count.
 */
@property (nonatomic, assign) int faceCount;

/**
 * 每个人脸的相关信息
 */
/****
 * Face Info.
 */
@property (nonatomic, strong) NSArray<QEFaceInfoItem *> *faceInfoItems;

@end

@interface QEBodyInfoData : NSObject

@property (nonatomic, assign) int bodyPoseType;
@property (nonatomic, assign) int bodySportType;
@property (nonatomic, assign) int sportCount;

@end

@class QueenEngine;

@protocol QueenEngineDelegate <NSObject>

@optional

/**
 * 检测到手势的回调。
 * @param engine 引擎对象。
 * @param gestureData 手势数据对象。
 */
/****
 * Called when gesture is detected.
 * @param engine The engine object.
 * @param gestureData The detected gesture data.
 */
- (void)queenEngine:(QueenEngine *)engine didDetectGesture:(QEGestureData *)gestureData;

/**
 * 检测到脸部表情的回调。
 * @param engine 引擎对象。
 * @param faceExpressionData 脸部表情数据对象。
 */
/****
 * Called when face expression is detected.
 * @param engine The engine object.
 * @param faceExpressionData The detected face expression data.
 */
- (void)queenEngine:(QueenEngine *)engine didDetectFaceExpression:(QEFaceExpressionData *)faceExpressionData;

/**
 * 检测到人脸信息的回调
 * @param engine 引擎对象
 * @param faceInfoData 人脸信息对象
 */
/****
 * Called when face is detected.
 * @param engine The engine object.
 * @param faceInfoData The detected face data.
 */
- (void)queenEngine:(QueenEngine *)engine didDetectFaceInfo:(QEFaceInfoData *)faceInfoData;

/**
 * 智能美型状态回调。
 * @param engine 引擎对象。
 * @param autoFaceShapeData 智能美型数据对象。
 */
/****
 * Called when auto face shaping.
 * @param engine The engine object.
 * @param autoFaceShapeData The face shaping data.
 */
- (void)queenEngine:(QueenEngine *)engine didChangeAutoFaceShapingStatus:(QEAutoFaceShapeData *)autoFaceShapeData;

/**
 * 检测到人体的回调。
 * @param engine 引擎对象。
 * @param bodyInfoData 人体数据对象。
 */
/****
 * Called when body is detected.
 * @param engine The engine object.
 * @param bodyInfoData The detected body data.
 */
- (void)queenEngine:(QueenEngine *)engine didDetectBodyInfo:(QEBodyInfoData *)bodyInfoData;

@end

@interface QueenEngine : NSObject

/** QueenEngine回调代理对象。
 */
/****
 * Specifies the delegate of QueenEngine.
 */
@property (nonatomic, weak) id <QueenEngineDelegate> delegate;

/**
 * 获取版本号。
 */
/****
 * Get version of the SDK.
 */
+ (NSString *)getVersion;

/**
 * 初始化引擎。
 * @param configInfo 配置信息。
 */
/****
 * Initialize the engine.
 * @param configInfo The engine configuration information.
 */
- (instancetype)initWithConfigInfo:(QueenEngineConfigInfo *)configInfo;

/**
 * 更新引擎配置参数。
 * 注意：仅当initWithConfigInfo的时候，configInfo为nil的时候此接口才生效，而且仅可设置一次有效值。
 * @param configInfo 配置信息。
 */
/****
 * Update the engine configuration information.
 * Note: This method takes effect only when initWithConfigInfo with nil, and the non-null value can only be set once.
 * @param configInfo The engine configuration information.
 */
- (void)updateConfigInfo:(QueenEngineConfigInfo *)configInfo;

/**
 * 销毁引擎。
 * 注意：需要在渲染线程调用。
 */
/****
 * Release the engine.
 * Note: Need to be called on the rendering thread.
 */
- (void)destroyEngine;

/**
 * 高性能模式。
 * 注意：开启后效果会稍变差。
 * @param enabled YES/NO，默认NO。
 */
/****
 * Performance mode.
 * Note: After turning it on, the effect will be slightly worse.
 * @param enabled YES/NO, default is NO.
 */
- (void)powerSavingEnabled:(BOOL)enabled;

/**
 * 设置渲染的viewport。
 * 注意：如果不设置默认使用输入的数据的宽和高。
 * @param originX 默认0
 * @param originY 默认0
 * @param width 默认0
 * @param height 默认0
 */
/****
 * Set the rendered viewport.
 * Note: If not set, the width and height of the input data are used by default.
 * @param originX Default is 0.
 * @param originY Default is 0.
 * @param width Default is 0.
 * @param height Default is 0.
 */
- (void)setScreenViewportWithOriginX:(int)originX withOriginY:(int)originY withWidth:(int)width withHeight:(int)height;

/**
 * 设置输出裁剪矩形。
 * 注意：如果不设置默认使用输入的数据的宽和高。
 * @param originX 默认0
 * @param originY 默认0
 * @param width 默认0
 * @param height 默认0
 */
/****
 * Set the output clipping rectangle.
 * Note: If not set, the width and height of the input data are used by default.
 * @param originX Default is 0.
 * @param originY Default is 0.
 * @param width Default is 0.
 * @param height Default is 0.
 */
- (void)setOutputRectWithOriginX:(int)originX withOriginY:(int)originY withWidth:(int)width withHeight:(int)height;

#pragma mark - "美颜类型和美颜参数API"

/**
 * 打开或者关闭某个功能效果。
 * @param type kQueenBeautyType 类型的一个值。
 * @param enabled YES: 打开，NO:关闭。
 */
/****
 * Enabled features.
 * @param type kQueenBeautyType.
 * @param enabled YES/NO.
 */
- (void)setQueenBeautyType:(kQueenBeautyType)type enable:(BOOL)enabled;

/**
 * 打开或者关闭某个功能效果。
 * @param type kQueenBeautyType 类型的一个值。
 * @param enabled YES: 打开，NO:关闭。
 * @param mode kQueenBeautyFilterMode 类型的一个值。
 */
/****
 * Enabled features.
 * @param type kQueenBeautyType.
 * @param enabled YES/NO.
 * @param mode kQueenBeautyFilterMode.
 */
- (void)setQueenBeautyType:(kQueenBeautyType)type enable:(BOOL)enabled mode:(kQueenBeautyFilterMode)mode;

/**
 * 设置功能参数。
 * @param param 功能参数类型，kQueenBeautyParams 中的一个。
 * @param value 需要设置的数值，参考kQueenBeautyParams的备注。
 */
/****
 * Set value of a specified feature.
 * @param param kQueenBeautyParams.
 * @param value Refer to the description of kQueenBeautyParams.
 */
- (void)setQueenBeautyParams:(kQueenBeautyParams)param value:(float)value;

/**
 * 获取功能参数。
 * @param param 功能参数类型，kQueenBeautyParams 中的一个。
 */
/****
 * Get value of a specified feature.
 * @param param kQueenBeautyParams
 */
- (float)getQueenBeautyParams:(kQueenBeautyParams)param;

/**
 * 获取功能开启状态。
 * @param beautyType 功能类型，kQueenBeautyType 中的一个。
 */
/****
 * Get enabled state of a specified feature.
 * @param beautyType kQueenBeautyType
 */
- (BOOL)getQueenBeautyEnabledState:(kQueenBeautyType)beautyType;

#pragma mark - "滤镜相关API"

/**
 * 设置滤镜资源。
 * 注意：设置滤镜图片前需要将kQueenBeautyTypeLUT打开。
 * @param imagePath 所要设置的滤镜图片的地址。
 */
/****
 * Set resource of filter.
 * Note: kQueenBeautyTypeLUT should be enabled before setting it.
 * @param imagePath Specifies the filter resource file path.
 */
- (void)setLutImagePath:(NSString *)imagePath;

#pragma mark - "美型相关API"

/**
 * 设置美型参数。
 * 注意：设置前需要将kQueenBeautyTypeFaceShape打开。
 * @param faceShapeType 美型类型，kQueenBeautyFaceShapeType 中的一个。
 * @param value 需要设置的数值，参考kQueenBeautyFaceShapeType的备注。
 */
/****
 * Set value of face shape.
 * Note: kQueenBeautyTypeFaceShape should be enabled before setting it.
 * @param faceShapeType kQueenBeautyFaceShapeType.
 * @param value Refer to the description of kQueenBeautyFaceShapeType.
 */
- (void)setFaceShape:(kQueenBeautyFaceShapeType)faceShapeType value:(float)value;

#pragma mark - "美体相关API"

/**
 * 设置美体参数。
 * 注意：设置前需要将kQueenBeautyTypeBodyShape打开。
 * @param bodyShapeType 美体类型，kQueenBeautyBodyShapeType 中的一个。
 * @param value 需要设置的数值，参考kQueenBeautyBodyShapeType的备注。
 */
/****
 * Set value of body shape.
 * Note: kQueenBeautyTypeBodyShape should be enabled before setting it.
 * @param bodyShapeType kQueenBeautyBodyShapeType.
 * @param value Refer to the description of kQueenBeautyBodyShapeType.
 */
- (void)setBodyShape:(kQueenBeautyBodyShapeType)bodyShapeType value:(float)value;

- (void)setBodyDetectSportType:(int)bodyDetectSportType;

- (void)clearBodySportCount;

#pragma mark - "美妆相关api"

/**
 * 设置美妆类型和图片素材路径。
 * 注意：设置前需要将kQueenBeautyTypeMakeup 打开。
 * @param makeupType 美妆类型，kQueenBeautyMakeupType 中的一个。
 * @param imagePaths 美妆素材地址集合。
 * @param blend 混合类型。
 */
/****
 * Set value of makeup.
 * Note: kQueenBeautyTypeMakeup should be enabled before setting it.
 * @param makeupType kQueenBeautyMakeupType.
 * @param imagePaths The resource file paths.
 * @param blend kQueenBeautyBlend.
 */
- (void)setMakeupWithType:(kQueenBeautyMakeupType)makeupType paths:(NSArray<NSString *> *)imagePaths blendType:(kQueenBeautyBlend)blend;

/**
 * 设置美妆类型和图片素材路径。
 * 注意：设置前需要将kQueenBeautyTypeMakeup 打开。
 * @param makeupType 美妆类型，kQueenBeautyMakeupType 中的一个。
 * @param imagePaths 美妆素材地址集合。
 * @param blend 混合类型。
 * @param fps 对应的帧率
 */
/****
 * Set value of makeup.
 * Note: kQueenBeautyTypeMakeup should be enabled before setting it.
 * @param makeupType kQueenBeautyMakeupType.
 * @param imagePaths The resource file paths.
 * @param blend kQueenBeautyBlend.
 * @param fps Specifies the fps of makeup resource.
 */
- (void)setMakeupWithType:(kQueenBeautyMakeupType)makeupType paths:(NSArray<NSString *> *)imagePaths blendType:(kQueenBeautyBlend)blend fps:(int)fps;

/**
 * 设置美妆透明度，可指定性别。
 * @param makeupType 美妆类型。
 * @param isFeMale 是否是女性，女性:YES，男性:NO，（男性为接口预留，这里均传女性即YES即可）。
 * @param alpha 妆容透明度。
 */
/****
 * Set alpha of makeup.
 * @param makeupType kQueenBeautyMakeupType.
 * @param isFeMale NO for male, YES for female, set YES here.
 * @param alpha Specifies the alpha of makeup.
 */
- (void)setMakeupAlphaWithType:(kQueenBeautyMakeupType)makeupType female:(BOOL)isFeMale alpha:(float)alpha;

/**
 * 设置美妆类型的混合类型。
 * @param makeupType 美妆类型。
 * @param blend 混合类型。
 */
/****
 * Set blend of makeup.
 * @param makeupType kQueenBeautyMakeupType.
 * @param blend kQueenBeautyBlend.
 */
- (void)setMakeupBlendWithType:(kQueenBeautyMakeupType)makeupType blendType:(kQueenBeautyBlend)blend;

/**
 * 清除所有美妆。
 */
/****
 * Clear all makeup effect.
 */
- (void)resetAllMakeupType;

#pragma mark - "头发美化相关API"

/**
 * 设置头发颜色。
 * 注意：设置前需要将kQueenBeautyTypeHairColor 打开。
 * @param red [0,1]
 * @param green [0,1]
 * @param blue [0,1]
 */
/****
 * Set hair color.
 * Note: kQueenBeautyTypeHairColor should be enabled before setting it.
 * @param red [0,1]
 * @param green [0,1]
 * @param blue [0,1]
 */
- (void)setHairColorWithRed:(float)red withGreen:(float)green withBlue:(float)blue;

#pragma mark - "贴纸相关API"

/**
 * 增加贴纸/贴图/实景抠图需要替换的背景，素材统一接口，支持GLTF,TAOPAI,MEDIAAI 类型。
 * @param materialPath 要添加的素材的路径。
 */
/****
 * Add sticker.
 * @param materialPath Specifies the sticker resource file path that should be added.
 */
- (void)addMaterialWithPath:(NSString *)materialPath;

/**
 * 删除贴纸/贴图/实景抠图需要替换的背景。
 * @param materialPath 要删除的素材的路径。
 */
/****
 * Remove sticker.
 * @param materialPath Specifies the sticker resource file path that should be removed.
 */
- (void)removeMaterialWithPath:(NSString *)materialPath;

#pragma mark - "抠图相关API"

/**
 * 绿幕/蓝幕抠图。
 * 注意：开启此抠图功能，纯色背景抠图（setPureColorToBackground）会关闭。
 * @param backgroundImagePath 需要绿/蓝幕替换的背景资源路径，传空即为取消抠图功能。
 * @param blueScreenEnabled 是否蓝幕抠图。
 * @param threshold 幕布敏感度[1,10]，默认1。
 * @param autoThresholdEnabled 是否根据环境动态计算幕布敏感度，为true时调节参数threshold失效，为false时调节参数threshold生效。
 */
/****
 * Green/Blue Screen matting.
 * Note: When this is turned on, the pure color background matting (setPureColorToBackground) will be disabled.
 * @param backgroundImagePath Specifies the background resource file path that needs to be replaced by green/blue screen. nil is disabled.
 * @param blueScreenEnabled Set if blue screen matting.
 * @param threshold Specifies the green/blue screen sensitivity, default is 1. Only takes effect when autoThresholdEnabled is NO.
 * @param autoThresholdEnabled Whether to dynamically calculate the screen sensitivity based on the environment.
 */
- (void)setGreenScreen:(NSString *)backgroundImagePath blueScreenEnabled:(BOOL)blueScreenEnabled threshold:(float)threshold autoThresholdEnabled:(BOOL)autoThresholdEnabled;

/**
 * 纯色背景抠图。
 * 注意：开启此抠图功能，绿幕抠图（setGreenScreen）会关闭。
 * @param backgroundImagePath 需要纯色背景替换的背景资源路径，传空即为取消抠图功能。
 * @param colorType 需要被替换的颜色类型，0绿色(green)，1蓝色(blue)，2青色(cyan)，3紫色(purple)，4黄色(yellow)，5红色(red)，默认0。
 * @param threshold 颜色敏感度[-1, 1], 默认0。
 */
/****
 * Pure color Screen matting.
 * Note: When this is turned on, the pure color background matting (setGreenScreen) will be disabled.
 * @param backgroundImagePath Specifies the background resource file path that needs to be replaced by specifies color screen. nil is disabled.
 * @param colorType Specifies the color type that should be matted, 0green, 1blue, 2cyan, 3purple, 4yellow, 5red, default is 0.
 * @param threshold Specifies the closeness of color, [-1, 1], default is 0.
 */
- (void)setPureColorToBackground:(NSString *)backgroundImagePath colorType:(kQueenBeautyBgColorType)colorType threshold:(float)threshold;

/**
 * 实景抠像。
 * @param backgroundImagePath 需要替换的背景资源路径，传空即为取消抠图功能。
 * @param flipX 背景左右翻转。
 * @param flipY 背景上下翻转。
 */
/****
 * AI background segment.
 * @param backgroundImagePath Specifies the background resource file path that needs to be replaced. nil is disabled.
 * @param flipX Set the background to flip horizontally.
 * @param flipY Set the background to flip vertically.
 */
- (void)setSegmentBackground:(NSString *)backgroundImagePath flipX:(BOOL)flipX flipY:(BOOL)flipY;

/**
 * 调整实景抠图的背景处理方式
 * @param backgroundProcessType 背景处理方式，默认为背景虚化
 */
/****
 * Choose process type for background segment.
 * @param backgroundProcessType kQueenBackgroundProcessType, default is kQueenBackgroundBlur.
 */
- (void)setSegmentBackgroundProcessType:(kQueenBackgroundProcessType)backgroundProcessType;

/**
 * 设置实景抠像的性能模式。
 * 注意：在开启实景抠像功能前调用才能生效。
 * @param performanceMode 实景抠图的性能模式。
 */
/****
 * Choose performance mode for background segment.
 * Note: Please call this method before using background segment function.
 * @param performanceMode kQueenSegmentPerformanceMode.
 */
- (void)setSegmentPerformanceMode:(kQueenSegmentPerformanceMode)performanceMode;

/**
 * 调整实景抠图的前景边距。
 * @param foregroundPadding 前景边距，[0,15]，默认0。
 */
/****
 * Set the foreground margins of the background segment.
 * @param foregroundPadding The foreground margins, [0,15], default is 0.
 */
- (void)setAISegmentForegroundPadding:(int)foregroundPadding;

#pragma mark - "互动相关API"

/**
 * AR写字/绘画。
 * @param enabled YES: 打开，NO:关闭。
 * @param mode 1: 写字, 2: 画画。
 */
/****
 * AR writing/painting.
 * @param enabled YES/NO.
 * @param mode 1: write, 2: paint.
 */
- (void)setARWriting:(BOOL)enabled mode:(int)mode;

#pragma mark - "数据处理"

/**
 * 人脸识别信息和渲染是否翻转以Y轴方向翻转。
 * @param faceRenderFlipY 需要关键点确定位置的图像渲染时是否反转，默认NO。
 * @param faceInfoFlipY 关键点数据是否翻转，默认NO。
 */
/****
 * If the detected face/body coordinates need to be flipped on the y-axis.
 * @param faceRenderFlipY Whether images that require key-positioned positions are rendered inverted, default is NO.
 * @param faceInfoFlipY Whether the keypoint data is flipped, default is NO.
 */
- (void)setFaceRenderFlipY:(BOOL)faceRenderFlipY faceInfoFlipY:(BOOL)faceInfoFlipY;

/**
 * 处理PixelBuffer类型数据。
 * @param pixelBufferData 需要处理的数据。
 * @return 接口调用状态码。
 */
/****
 * Process QEPixelBufferData.
 * @param pixelBufferData Data should be processed.
 * @return kQueenResultCode.
 */
- (kQueenResultCode)processPixelBuffer:(QEPixelBufferData *)pixelBufferData;

/**
 * 处理Texture类型数据。
 * @param textureData 需要处理的数据。
 * @return 接口调用状态码。
 */
/****
 * Process QETextureData.
 * @param textureData Data should be processed.
 * @return kQueenResultCode.
 */
- (kQueenResultCode)processTexture:(QETextureData *)textureData;

/**
 * 分析视频帧数据处理。
 * 注意：如处理Texture类型数据时需高级美颜处理需要调用此接口，处理PixelBuffer类型数据不需要调用此接口。
 * @param imageData 帧图片流。
 * @param format 帧图片流颜色格式。
 * @param width 帧图片宽度。
 * @param height 帧图片高度。
 * @param stride 用于检测的图像的跨度(以像素为单位),即每行的字节数, 默认情况下设为 0。
 * @param intputAngle 当前输入帧图片需旋转的角度，计算方式参考Sample工程，注：仅当QueenEngineConfigInfo的autoSettingImgAngle设置为NO才会生效。
 * @param outputAngle 算法输出结果所需旋转的角度，计算方式参考Sample工程，注：仅当QueenEngineConfigInfo的autoSettingImgAngle设置为NO才会生效。
 * @param flipAxis 输出数据的xy轴翻转处理,0为不旋转,1为x轴翻转,2为y轴翻转。
 */
/****
 * For image data analysis.
 * Note: If processing textureData, this method should be called, otherwise this method should not be called.
 * @param imageData Specifies the image raw data.
 * @param format Specifies the color space.
 * @param width Specifies the image width.
 * @param height Specifies the image height.
 * @param stride Specifies the stride (in pixels) of the image, i.e. the number of bytes per line, default is 0.
 * @param intputAngle Specifies the rotation angle of the bufferIn by which the face/body (if any) in the bufferIn needs to be rotated clockwise to the frontal face/body. Examples are 0/90/180/270, default is 0.
 * Note: Only takes effect if QueenEngineConfigInfo's member variable autoSettingImgAngle is set to NO.
 * @param outputAngle Specifies the rotation angle by which the detected frontal face/body needs to be rotated counterclockwise. Examples are 0/90/180/270, default is 0.
 * Note: Only takes effect if QueenEngineConfigInfo's member variable autoSettingImgAngle is set to NO. In general, the value set should be consistent with the inputAngle, so that the coordinate position of the recognized face/human body result fits the original image.
 * @param flipAxis The direction in which the detected face/body coordinates need to be flipped, 0 do nothing,1 Flip on the x-axis, 2 Flip on the y-axis.
 */
- (void)updateInputDataAndRunAlg:(uint8_t *)imageData withImgFormat:(kQueenImageFormat)format withWidth:(int)width withHeight:(int)height withStride:(int)stride
                  withInputAngle:(int)intputAngle withOutputAngle:(int)outputAngle withFlipAxis:(int)flipAxis;

#pragma mark - "调试相关"

/**
 * 展示人脸识别点位。
 * @param show 是否展示。
 */
/****
 * Show key points for face.
 * @param show YES/NO.
 */
- (void)showFaceDetectPoint:(BOOL)show;

/**
 * 展示人体识别点位。
 * @param show 是否展示。
 */
/****
 * Show key points for body.
 * @param show YES/NO.
 */
- (void)showBodyDetectPoint:(BOOL)show;

/**
 * 展示手部识别点位。
 * @param show 是否展示。
 */
/****
 * Show key points for hand.
 * @param show YES/NO.
 */
- (void)showHandDetectPoint:(BOOL)show;

/**
 * 展示人脸区域三角剖分线。
 * @param show 是否展示。
 */
/****
 * Show lines for face.
 * @param show YES/NO.
 */
- (void)showMakeupLine:(BOOL)show;

/**
 * 展示AR绘制关键点。
 * @param show 是否展示。
 */
/****
 * Show key points for ar writing/painting.
 * @param show YES/NO.
 */
- (void)showARWritingDetectPoint:(BOOL)show;

@end
