//
//  QueenEngineConfigInfo.h
//  queen
//
//  Created by aliyun on 2021/3/14.
//  Copyright © 2021 alibaba-inc. All rights reserved.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

/** 美颜类型，需要哪种美颜需要先打开
 */
typedef NS_ENUM(NSInteger, kQueenBeautyType) {
    /** 基础美颜（主要包括磨皮、锐化）
     */
    kQueenBeautyTypeSkinBuffing    =    0,
    /** 脸部美颜（主要包括去眼袋、法令纹、白牙、口红、腮红）
     */
    kQueenBeautyTypeFaceBuffing    =    1,
    /** 美妆（主要包括整妆、高光、美瞳、口红、眼妆）
     */
    kQueenBeautyTypeMakeup         =    2,
    /** 美型（主要包括瘦脸、瘦下巴、大眼、瘦鼻、美唇等）
     */
    kQueenBeautyTypeFaceShape      =    3,
    /** 基础美颜（美白）
     */
    kQueenBeautyTypeSkinWhiting    =    4,
    /** 滤镜，当设置滤镜后，可以指定一个滤镜图片
     */
    kQueenBeautyTypeLUT            =    5,
    /** 背景处理，实景抠图，使背景虚化
     */
    kQueenBeautyTypeBackgroundProcess = 6,
    /** 最大值
     */
    kQueenBeautyTypeMax            =    7,
};

/**美颜参数，值为float，除基础美颜外需要先将功能打开，对应参数才有效
 */
typedef NS_ENUM(NSInteger, kQueenBeautyParams) {
    /** 基础美颜参数项，基础美颜默认打开，不需要手动调用打开接口
     磨皮，值的范围[0,1] ，默认0
     */
    kQueenBeautyParamsSkinBuffing      = 1,
    /** 基础美颜参数项，基础美颜默认打开，不需要手动调用打开接口
     锐化，值的范围[0,1] ，默认0
     */
    kQueenBeautyParamsSharpen          = 2,
    /** 基础美颜参数项，基础美颜默认打开，不需要手动调用打开接口
     美白，值的范围[0,1] ，默认0
     */
    kQueenBeautyParamsWhitening        = 3,
    /** 脸部美颜参数项，需要先打开QueenBeautyTypeFaceBuffing 项
     去眼袋，值的范围[0,1]，默认0
     */
    kQueenBeautyParamsPouch            = 4,
    /** 脸部美颜参数项，需要先打开QueenBeautyTypeFaceBuffing 项
     去法令纹，值的范围[0,1]，默认0
     */
    kQueenBeautyParamsNasolabialFolds  = 5,
    /** 色卡滤镜参数项，需要先打开QueenBeautyTypeLUT 项
     色卡滤镜强度，值的范围[0,1]，默认0
     */
    kQueenBeautyParamsLUT              = 6,
    /** 脸部美颜参数项，需要先打开QueenBeautyTypeFaceBuffing 项
     白牙，值的范围[0,1]，默认0
     */
    kQueenBeautyParamsWhiteTeeth       = 7,
    /** 脸部美颜参数项，需要先打开QueenBeautyTypeFaceBuffing 项
     口红，值的范围[0,1]，默认0
     */
    kQueenBeautyParamsLipstick         = 8,
    /** 脸部美颜参数项，需要先打开QueenBeautyTypeFaceBuffing 项
     腮红，值的范围[0,1]，默认0
     */
    kQueenBeautyParamsBlush            = 9,
    /** 脸部美颜参数项，需要先打开QueenBeautyTypeFaceBuffing 项
     口红色相 [-0.5,0.5], 默认0
    */
    kQueenBeautyParamsLipstickColorParam  = 10,
    /** 脸部美颜参数项，需要先打开QueenBeautyTypeFaceBuffing 项
     口红饱和度[0,1], 默认0
    */
    kQueenBeautyParamsLipstickGlossParam  = 11,
    /** 脸部美颜参数项，需要先打开QueenBeautyTypeFaceBuffing 项
     口红明度[0,1], 默认0
    */
    kQueenBeautyParamsLipstickBrightnessParam = 12,
    /** 脸部美颜参数项，需要先打开QueenBeautyTypeFaceBuffing 项
     亮眼[0,1], 默认0
    */
    kQueenBeautyParamsBrightenEye         = 13,
    /** 脸部美颜参数项，需要先打开QueenBeautyTypeFaceBuffing 项
     红润[0,1], 默认0
    */
    kQueenBeautyParamsSkinRed             = 14,
    /** 脸部美颜参数项，需要先打开QueenBeautyTypeFaceBuffing 项
     祛皱纹[0,1], 默认0
    */
    kQueenBeautyParamsWrinkles             = 15,
    /** 脸部美颜参数项，需要先打开QueenBeautyTypeFaceBuffing 项
     祛暗沉[0,1], 默认0
    */
    kQueenBeautyParamsBrightenFace         = 16,
};

/**美妆类型，需要先打开QueenBeautyTypeMakeup 类型
 */
typedef NS_ENUM(NSInteger, kQueenBeautyMakeupType) {
    /** 整妆
     */
    kQueenBeautyMakeupTypeWhole       = 0,
    /** 高光
     */
    kQueenBeautyMakeupTypeHighlight   = 1,
    /** 美瞳
     */
    kQueenBeautyMakeupTypeEyeball     = 2,
    /** 口红
     */
    kQueenBeautyMakeupTypeMouth       = 3,
    /** 卧蚕
     */
    kQueenBeautyMakeupTypeWocan       = 4,
    /** 眼妆
     */
    kQueenBeautyMakeupTypeEyeBrow     = 5,
    /** 腮红
     */
    kQueenBeautyMakeupTypeBlush       = 6,
    /** 最大值
     */
    kQueenBeautyMakeupTypeMakeupMax   = 7
};

/**美型类型，需要先打开kQueenBeautyTypeFaceShape 类型
 */
typedef NS_ENUM(NSInteger, kQueenBeautyFaceShapeType) {
    /** 颧骨，值的范围[0,1]，默认0
     */
    kQueenBeautyFaceShapeTypeCutCheek      = 0,
    /** 削脸，值的范围[0,1]，默认0
     */
    kQueenBeautyFaceShapeTypeCutFace       = 1,
    /** 瘦脸，值的范围[0,1]，默认0
     */
    kQueenBeautyFaceShapeTypeThinFace      = 2,
    /** 脸长，值的范围[0,1]，默认0
     */
    kQueenBeautyFaceShapeTypeLongFace      = 3,
    /** 下巴缩短，值的范围[-1,1]，默认0
     */
    kQueenBeautyFaceShapeTypeLowerJaw      = 4,
    /** 下巴拉长，值的范围[-1,1]，默认0
     */
    kQueenBeautyFaceShapeTypeHigherJaw     = 5,
    /** 瘦下巴，值的范围[0,1]，默认0
     */
    kQueenBeautyFaceShapeTypeThinJaw       = 6,
    /** 瘦下颌，值的范围[0,1]，默认0
     */
    kQueenBeautyFaceShapeTypeThinMandible  = 7,
    /** 大眼，值的范围[0,1]，默认0
     */
    kQueenBeautyFaceShapeTypeBigEye        = 8,
    /** 眼角1，值的范围[0,1]，默认0
     */
    kQueenBeautyFaceShapeTypeEyeAngle1     = 9,
    /** 眼距，值的范围[-1,1]，默认0
     */
    kQueenBeautyFaceShapeTypeCanthus       = 10,
    /** 拉宽眼距，值的范围[-1,1]，默认0
     */
    kQueenBeautyFaceShapeTypeCanthus1      = 11,
    /** 眼角2，值的范围[-1,1]，默认0
     */
    kQueenBeautyFaceShapeTypeEyeAngle2     = 12,
    /** 眼睛高度，值的范围[-1,1]，默认0
     */
    kQueenBeautyFaceShapeTypeEyeTDAngle    = 13,
    /** 瘦鼻，值的范围[0,1]，默认0
     */
    kQueenBeautyFaceShapeTypeThinNose      = 14,
    /** 鼻翼，值的范围[0,1]，默认0
     */
    kQueenBeautyFaceShapeTypeNosewing      = 15,
    /** 鼻长，值的范围[-1,1]，默认0
     */
    kQueenBeautyFaceShapeTypeNasalHeight   = 16,
    /** 鼻头长，值的范围[-1,1]，默认0
     */
    kQueenBeautyFaceShapeTypeNoseTipHeight = 17,
    /** 唇宽，值的范围[-1,1]，默认0
     */
    kQueenBeautyFaceShapeTypeMouthWidth    = 18,
    /** 嘴唇大小，值的范围[-1,1]，默认0
     */
    kQueenBeautyFaceShapeTypeMouthSize     = 19,
    /** 唇高，值的范围[-1,1]，默认0
     */
    kQueenBeautyFaceShapeTypeMouthHigh     = 20,
    /** 人中，值的范围[-1,1]，默认0
     */
    kQueenBeautyFaceShapeTypePhiltrum      = 21,
    /** 发际线，值的范围[-1,1]，默认0
     */
    kQueenBeautyFaceShapeTypeHairLine = 22,
    /** 嘴角上扬(微笑)，值的范围[0,1]，默认0
     */
    kQueenBeautyFaceShapeTypeSmile = 23,
    /** 最大值
     */
    kQueenBeautyFaceShapeTypeMAX        = 24
};

/** 美妆混合模式
 */
typedef NS_ENUM(NSInteger, kQueenBeautyBlend) {
    /** 正常
     */
    kQueenBeautyBlendNormal = 0,
    /** 变亮
     */
    kQueenBeautyBlendLighten = 1,
    /** 变暗
     */
    kQueenBeautyBlendDarken = 2,
    /** 正片叠底
     */
    kQueenBeautyBlendMultiply = 3,
    /** 划分
     */
    kQueenBeautyBlendDivide = 4,
    /** 平均
     */
    kQueenBeautyBlendAverage = 5,
    /** 线性减淡
     */
    kQueenBeautyBlendAdd = 6,
    /** 减去
     */
    kQueenBeautyBlendSubtract = 7,
    /** 差值
     */
    kQueenBeautyBlendDifference = 8,
    /** 反向
     */
    kQueenBeautyBlendNegation = 9,
    /** 排除
     */
    kQueenBeautyBlendExclusion = 10,
    /** 滤色
     */
    kQueenBeautyBlendScreen = 11,
    /** 叠加
     */
    kQueenBeautyBlendOverlay = 12,
    /** 柔光
     */
    kQueenBeautyBlendSoftLight = 13,
    /** 强光
     */
    kQueenBeautyBlendHardLight = 14,
    /** 颜色减淡
     */
    kQueenBeautyBlendColorDodge = 15,
    /** 颜色加深
     */
    kQueenBeautyBlendColorBurn = 16,
    /** 线性减淡
     */
    kQueenBeautyBlendLinearDodge = 17,
    /** 线性加深
     */
    kQueenBeautyBlendLinearBurn = 18,
    /** 线性光
     */
    kQueenBeautyBlendLinearLight = 19,
    /** 亮光
     */
    kQueenBeautyBlendVividLight = 20,
    /** 点光
     */
    kQueenBeautyBlendPinLight = 21,
    /** 实色混合
     */
    kQueenBeautyBlendHardMix = 22,
    /** 反射
     */
    kQueenBeautyBlendReflect = 23,
    /** 发光
     */
    kQueenBeautyBlendGlow = 24,
    /** 颗粒
     */
    kQueenBeautyBlendPhoenix = 25,
    /** 色相
     */
    kQueenBeautyBlendHue = 26,
    /** 饱和度
     */
    kQueenBeautyBlendSaturation = 27,
    /** 明度
     */
    kQueenBeautyBlendLuminosity = 28,
    /** 颜色
     */
    kQueenBeautyBlendColor = 29,
    /** 曲线
     */
    kQueenBeautyBlendCurve = 30,
    /**
     * 视觉融合模式
     */
    kQueenBeautyBlendLabMix = 31,
    /** 最大值
     */
    kQueenBeautyBlendMax = 999
};

/** 翻转类型
 */
typedef NS_ENUM(NSInteger, kQueenBeautyFlipAxis) {
    /** 不做翻转
     */
    kQueenBeautyFlipAxisNone       = 0,
    /** X轴翻转
     */
    kQueenBeautyFlipAxisX   = 1,
    /** Y轴翻转
     */
    kQueenBeautyFlipAxisY     = 2,
};

/** 状态码
 */
typedef NS_ENUM(NSInteger, kQueenResultCode) {
    /** 正常
     */
    kQueenResultCodeOK       = 0,
    /** license校验失败
     */
    kQueenResultCodeInvalidLicense   = 1,
    /** 参数错误
     */
    kQueenResultCodeInvalidParam   = 2,
    /** 没有应用任何Queen的特效处理
     */
    kQueenResultCodeNoEffect   = 3,
};

/** 颜色格式
 */
typedef NS_ENUM(NSInteger, kQueenImageFormat) {
    /** RGB
     */
    kQueenImageFormatRGB = 0,
    /** NV21
     */
    kQueenImageFormatNV21 = 1,
    /** NV12
     */
    kQueenImageFormatNV12 = 2,
    /** RGBA
     */
    kQueenImageFormatRGBA = 3
};

/** Queen美颜引擎配置信息类
 */
@interface QueenEngineConfigInfo :NSObject

/**
 * @brief QUEEN使用到的资源根目录，保持不变请为nil
 */
@property (nonatomic, copy) NSString *resRootPath;

/**
 * @brief 是否内部自动创建GLContext，如果要处理CVPixelBuffer，必须传入或者内部创建Context，默认为YES
 */
@property (nonatomic, assign) BOOL withContext;

/**
 * @brief 是否要渲染到屏幕，默认为NO
 */
@property (nonatomic, assign) BOOL toScreen;

/**
 * @brief 是否自动计算图像的inputAngle和outputAngle，默认为NO，API_AVAILABLE(ios(8.0))
 */
@property (nonatomic, assign) BOOL autoSettingImgAngle;

@end

NS_ASSUME_NONNULL_END
