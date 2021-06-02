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
 * @brief 输入图像的角度，值为0/90/180/270，默认值为0
 */
@property (nonatomic, assign) size_t inputAngle;

/**
 * @brief 输出算法结果数据需要旋转的角度，值为0/90/180/270，默认值为0
 */
@property (nonatomic, assign) size_t outputAngle;

/**
 * @brief 输出算法结果数据需要翻转的方向，默认值为kQueenBeautyFlipAxisNone
 */
@property (nonatomic, assign) kQueenBeautyFlipAxis outputFlipAxis;

@end

/** Queen美颜引擎类
 */
@interface QueenEngine : NSObject

- (instancetype)initWithConfigInfo:(QueenEngineConfigInfo *)configInfo;

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
 * @param isFeMale 是否是女性，女性:YES，男性:NO
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

#pragma mark - "数据处理"

/** 处理PixelBuffer类型数据
 * @param pixelBufferData 需要处理的数据
 * @return 接口调用状态码
 */
- (kQueenResultCode)processPixelBuffer:(QEPixelBufferData *)pixelBufferData;

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
