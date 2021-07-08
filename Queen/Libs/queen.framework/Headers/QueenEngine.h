//
//  QueenEngine.h
//  queen
//
//  Created by aliyun on 2021/3/14.
//  Copyright © 2021 alibaba-inc. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <CoreVideo/CoreVideo.h>
#import <queen/QueenEngineConfigInfo.h>

/** Queen美颜引擎处理Pixelbuffer数据类
 */

@interface QEPixelBufferData : NSObject

/**
 * @brief 需要处理的CVPixelBufferRef
 */
@property (nonatomic, assign) CVPixelBufferRef bufferIn;

/**
 * @brief 处理完后的CVPixelBufferRef
 */
@property (nonatomic, assign) CVPixelBufferRef bufferOut;

/**
 * @brief 输入图像的角度，值为0/90/180/270，默认值为0，注：仅当QueenEngineConfigInfo的autoSettingImgAngle设置为NO才会生效
 */
@property (nonatomic, assign) size_t inputAngle;

/**
 * @brief 输出算法结果数据需要旋转的角度，值为0/90/180/270，默认值为0，注：仅当QueenEngineConfigInfo的autoSettingImgAngle设置为NO才会生效
 */
@property (nonatomic, assign) size_t outputAngle;

/**
 * @brief 输出算法结果数据需要翻转的方向，默认值为kQueenBeautyFlipAxisNone
 */
@property (nonatomic, assign) kQueenBeautyFlipAxis outputFlipAxis;

@end

/** Queen美颜引擎处理Texture数据类
 */

@interface QETextureData : NSObject

@property (nonatomic, assign) uint32_t inputTextureID;
@property (nonatomic, assign) uint32_t outputTextureID;
@property (nonatomic, assign) int width;
@property (nonatomic, assign) int height;
@property (nonatomic, assign) bool isOes;

@end

/** Queen美颜引擎类
 */
@interface QueenEngine : NSObject

/**
 * @brief 初始化引擎
 * @param configInfo 初始化配置
 *
 */
- (instancetype)initWithConfigInfo:(QueenEngineConfigInfo *)configInfo;

/**
 * @brief 销毁引擎，需要在渲染线程调用
 *
 */
- (void)destroyEngine;

#pragma mark - "美颜类型和美颜参数API"

/**
 * @brief 打开或者关闭某个美颜类型
 * @param type QueenBeautyType 类型的一个值
 * @param isOpen YES: 打开，NO:关闭
 *
 */
- (void)setQueenBeautyType:(kQueenBeautyType)type enable:(BOOL)isOpen;

/**
 * @brief 设置美颜参数
 * @param param 美颜参数类型，QueenBeautyParams 中的一个
 * @param value 需要设置的数值，值的范围都是[0,1],小于0的置0，大于1的置1
 */
- (void)setQueenBeautyParams:(kQueenBeautyParams)param value:(float)value;

#pragma mark - "滤镜相关API"

/**
 * @brief 设置滤镜图片,设置滤镜图片前需要将kQueenBeautyTypeLUT打开
 * @param imagePath 所要设置的滤镜图片的地址
 */
- (void)setLutImagePath:(NSString *)imagePath;

#pragma mark - "美型相关API"
/**
 *  @brief 设置美型类型,设置前需要将kQueenBeautyTypeFaceShape打开
 *  @param faceShapeType  需要设置美型的类型，参考QueenBeautyFaceShapeType
 *  @param value 需要设置的值
 */
- (void)setFaceShape:(kQueenBeautyFaceShapeType)faceShapeType value:(float)value;

#pragma mark - "美妆相关api"

/**
 * @brief 设置美妆类型和图片素材路径，设置美妆需要将kQueenBeautyTypeMakeup 打开
 * @param makeupType 美妆类型
 * @param imagePaths 美妆素材地址集合
 * @param blend 混合类型
 */
- (void)setMakeupWithType:(kQueenBeautyMakeupType)makeupType paths:(NSArray<NSString *> *)imagePaths blendType:(kQueenBeautyBlend)blend;

/**
 * @brief 设置美妆类型和图片素材路径
 * @param makeupType 美妆类型
 * @param imagePaths 美妆素材地址集合
 * @param blend 混合类型
 * @param fps 对应的帧率
 */
- (void)setMakeupWithType:(kQueenBeautyMakeupType)makeupType paths:(NSArray<NSString *> *)imagePaths blendType:(kQueenBeautyBlend)blend fps:(int)fps;

/**
 * @brief 设置美妆透明度，可指定性别
 * @param makeupType 美妆类型
 * @param isFeMale 是否是女性，女性:YES，男性:NO，（男性为接口预留，这里均传女性即YES即可）
 * @param alpha 妆容透明度
 */
- (void)setMakeupAlphaWithType:(kQueenBeautyMakeupType)makeupType female:(BOOL)isFeMale alpha:(float)alpha;

/**
 * @brief 设置美妆类型的混合类型
 * @param makeupType 美妆类型
 * @param blend 混合类型
 */
- (void)setMakeupBlendWithType:(kQueenBeautyMakeupType)makeupType blendType:(kQueenBeautyBlend)blend;

/**
 * @brief 清除所有美妆
 */
- (void)resetAllMakeupType;

#pragma mark - "贴纸相关API"
/**
 * @brief 增加贴纸/贴图，素材统一接口，支持GLTF,TAOPAI,MEDIAAI 类型
 * @param materialPath 要添加的素材的路径
 */
- (void)addMaterialWithPath:(NSString *)materialPath;

/**
 * @brief 删除贴纸/贴图
 * @param materialPath 要删除的素材的路径
 */
- (void)removeMaterialWithPath:(NSString *)materialPath;

#pragma mark - "抠图相关API"
/**
 * @brief 绿幕/蓝幕抠图
 * @param backgroundImagePath 需要绿/蓝幕替换的背景资源路径，传空即为取消抠图功能
 * @param blueScreenEnabled 是否蓝幕抠图
 * @param threshold 幕布敏感度[1,10]，默认1
 * @param autoThresholdEnabled 是否根据环境动态计算幕布敏感度，为true时调节参数threshold失效，为false时调节参数threshold生效
 */
- (void)setGreenScreen:(NSString *)backgroundImagePath blueScreenEnabled:(BOOL)blueScreenEnabled threshold:(float)threshold autoThresholdEnabled:(BOOL)autoThresholdEnabled;

#pragma mark - "数据处理"

/** 处理PixelBuffer类型数据
 * @param pixelBufferData 需要处理的数据
 * @return 接口调用状态码
 */
- (kQueenResultCode)processPixelBuffer:(QEPixelBufferData *)pixelBufferData;

/** 处理Texture类型数据
 * @param textureData 需要处理的数据
 * @return 接口调用状态码
 */
- (kQueenResultCode)processTexture:(QETextureData *)textureData;

/** 视频帧数据更新处理，用于做engine算法层的数据处理，注：如处理Texture类型数据时需高级美颜处理需要调用此接口，处理PixelBuffer类型数据不需要调用此接口
 * @param imageData 帧图片流
 * @param format 帧图片流格式
 * @param width 帧图片宽度
 * @param height 帧图片高度
 * @param stride 用于检测的图像的跨度(以像素为单位),即每行的字节数, 默认情况下设为 0
 * @param intputAngle 当前输入帧图片需旋转的角度，计算方式参考Sample工程，注：仅当QueenEngineConfigInfo的autoSettingImgAngle设置为NO才会生效
 * @param outputAngle 算法输出结果所需旋转的角度，计算方式参考Sample工程，注：仅当QueenEngineConfigInfo的autoSettingImgAngle设置为NO才会生效
 * @param flipAxis 输出数据的xy轴翻转处理,0为不旋转,1为x轴翻转,2为y轴翻转
 */
- (void)updateInputDataAndRunAlg:(uint8_t *)imageData withImgFormat:(kQueenImageFormat)format withWidth:(int)width withHeight:(int)height withStride:(int)stride
                  withInputAngle:(int)intputAngle withOutputAngle:(int)outputAngle withFlipAxis:(int)flipAxis;

#pragma mark - "调试相关"

/**
 * @brief 展示人脸识别点位
 * @param show 是否展示
 */
- (void)showFaceDetectPoint:(BOOL)show;

/**
 * @brief 展示人脸区域三角剖分线
 * @param show 是否展示
 */
- (void)showMakeupLine:(BOOL)show;

@end
