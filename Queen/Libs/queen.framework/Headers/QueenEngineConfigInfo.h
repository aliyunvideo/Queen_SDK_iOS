//
//  QueenEngineConfigInfo.h
//  queen
//
//  Created by aliyun on 2021/3/14.
//  Copyright © 2021 alibaba-inc. All rights reserved.
//

#ifndef QueenEngineConfigInfo_H
#define QueenEngineConfigInfo_H

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

/** 美颜类型，需要哪种美颜需要先打开
 */
/****
 * Beauty Type, effects should be enabled before they can be used.
 */
typedef NS_ENUM(NSInteger, kQueenBeautyType) {
    /** 基础美颜（主要包括磨皮、锐化）
     */
    /****
     * Feature: Buffing and Sharpen
     */
    kQueenBeautyTypeSkinBuffing    =    0,
    /** 脸部美颜（主要包括去眼袋、法令纹、白牙、口红、腮红）
     */
    /****
     * Feature: Advanced Buffing
     */
    kQueenBeautyTypeFaceBuffing    =    1,
    /** 美妆（主要包括整妆、高光、美瞳、口红、眼妆）
     */
    /****
     * Feature: Makeup
     */
    kQueenBeautyTypeMakeup         =    2,
    /** 美型（主要包括瘦脸、瘦下巴、大眼、瘦鼻、美唇等）
     */
    /****
     * Feature: Face Shaping
     */
    kQueenBeautyTypeFaceShape      =    3,
    /** 基础美颜（美白）
     */
    /****
     * Feature: Skin Whiting
     */
    kQueenBeautyTypeSkinWhiting    =    4,
    /** 滤镜，当设置滤镜后，可以指定一个滤镜图片
     */
    /****
     * Feature: Filters
     */
    kQueenBeautyTypeLUT            =    5,
    /** 背景处理，实景抠图，使背景虚化
     */
    /****
     * Feature: Background With Blur
     */
    kQueenBeautyTypeBackgroundProcess = 6,
    /** 功能类型: 智能美颜
     */
    /****
     * Feature: Auto Beauty
     */
    kQueenBeautyTypeAutoFilter     =    7,
    /** 功能类型: 美体
     */
    /****
     * Feature: Body Shaping
     */
    kQueenBeautyTypeBodyShape      =    8,
    /** 功能类型: 手势检测
     */
    /****
     * Feature: Gesture Detecting
     */
    kQueenBeautyTypeHandGestureDetect = 9,
    /** 功能类型: 脸部表情检测
     */
    /****
     * Feature: Face Expression Detecting
     */
    kQueenBeautyTypeFaceExpressionDetect = 10,
    /** 功能类型: 换发色
     */
    /****
     * Feature: Change Hair Color
     */
    kQueenBeautyTypeHairColor = 11,
    /**
     * 功能类型: 智能美型
     */
    /****
     * Feature: Auto Face Shaping
     */
    kQueenBeautyTypeAutoFaceShape = 12,
    /** 功能类型: 人脸检测
     */
    /****
     * Feature: Face Detecting
     */
    kQueenBeautyTypeFaceDetect = 13,
    /** 功能类型: 人体检测
     */
    /****
     * Feature: Body Detecting
     */
    kQueenBeautyTypeBodyDetect = 14,
    /** 功能类型: 色相饱和度明度
     */
    /****
     * Feature: HSV
     */
    kQueenBeautyTypeHSV = 15,
    /** 功能类型: 人脸打马赛克
     */
    /****
     * Feature: FaceMosaicing
     */
    kQueenBeautyTypeFaceMosaicing = 16,
    /**
     * 功能类型：隔空写字特效
     */
    /****
     * Feature: 隔空写字 Effects
     */
    kQueenBeautyTypeARWriting = 17,
    /** 功能类型: 专注度检测
     */
    /****
     * Feature: Concentration Detect
     */
    kQueenBeautyTypedConcentrationDetect = 18,
    /** 功能类型: 异常行为(物体)检测
     */
    /****
     * Feature: Abnormal action Detect
     */
    kQueenBeautyTypedAbnormalActionDetect = 19,
    /** 功能类型: 活体检测
     */
    /****
     * Feature: Living human Detect
     */
    kQueenBeautyTypedLivingHumanDetect = 20,
    /** 最大值
     */
    /****
     * Beauty type count.
     */
    kQueenBeautyTypeMax = 21
};

/** 对应美颜类型的模式
 */
/****
 * Function Mode
 */
typedef NS_ENUM(NSInteger, kQueenBeautyFilterMode) {
    /** 默认值
     */
    /****
     * Default
     */
    kQueenBeautyFilterModeDefault = 0,
    /** 磨皮-自然
     */
    /****
     * Function Mode: Face Buffing Mode, Nature
     */
    kQueenBeautyFilterModeSkinBuffing_Natural = 1,
    /** 磨皮-明显
     */
    /****
     * Function Mode: Face Buffing Mode, Strong
     */
    kQueenBeautyFilterModeSkinBuffing_Strong = 2,
    /** 美型-基准线
     */
    /****
     * Function Mode: Face Shaping Mode, Baseline
     */
    kQueenBeautyFilterModeFaceShape_Baseline = 11,
    /** 美型-主流
     */
    /****
     * Function Mode: Face Shaping Mode, Main
     */
    kQueenBeautyFilterModeFaceShape_Main = 12,
    /** 美型-偏大变化
     */
    /****
     * Function Mode: Face Shaping Mode, High
     */
    kQueenBeautyFilterModeFaceShape_High = 13,
    /** 美型-夸张
     */
    /****
     * Function Mode: Face Shaping Mode, Max
     */
    kQueenBeautyFilterModeFaceShape_Max = 14,
    /** 美妆-基准线
     */
    /****
     * Function Mode: Makeup Mode, Baseline
     */
    kQueenBeautyFilterModeFaceMakeup_Baseline = 21,
    /** 美妆-强烈
     */
    /****
     * Function Mode: Makeup Mode, High
     */
    kQueenBeautyFilterModeFaceMakeup_High = 22,
    /**
    * 功能类型: AR隔空写字-写字模式
    */
    /****
     * Function Mode: AR_Writing Mode-Writer
     */
    kQueenBeautyFilterModeArWriting_Writer = 31,
    /**
     * 功能类型: AR隔空写字-绘画模式
     */
    /****
     * Function Mode: AR_Writing Mode-Drawer
     */
    kQueenBeautyFilterModeArWriting_Drawer = 32

};


/**美颜参数，值为float
 */
/****
 * Beauty Params, float value
 */
typedef NS_ENUM(NSInteger, kQueenBeautyParams) {
    /** 基础美颜参数项，需要先打开kQueenBeautyTypeSkinBuffing 项
     磨皮，值的范围[0,1] ，默认0
     */
    /****
     * Feature: Buffing
     * Parameter Range:[0,1]
     */
    kQueenBeautyParamsSkinBuffing      = 1,
    /** 基础美颜参数项，需要先打开kQueenBeautyTypeSkinBuffing 项
     锐化，值的范围[0,1] ，默认0
     */
    /****
     * Feature: Sharpen
     * Parameter Range:[0,1]
     */
    kQueenBeautyParamsSharpen          = 2,
    /** 基础美颜参数项，需要先打开kQueenBeautyTypeSkinBuffing 项
     美白，值的范围[0,1] ，默认0
     */
    /****
     * Feature: Whitening
     * Parameter Range:[0,1]
     */
    kQueenBeautyParamsWhitening        = 3,
    /** 脸部美颜参数项，需要先打开QueenBeautyTypeFaceBuffing 项
     去眼袋，值的范围[0,1]，默认0
     */
    /****
     * Feature: Remove Pouch
     * Parameter Range:[0,1]
     */
    kQueenBeautyParamsPouch            = 4,
    /** 脸部美颜参数项，需要先打开QueenBeautyTypeFaceBuffing 项
     去法令纹，值的范围[0,1]，默认0
     */
    /****
     * Feature: Remove NasolabialFolds
     * Parameter Range:[0,1]
     */
    kQueenBeautyParamsNasolabialFolds  = 5,
    /** 色卡滤镜参数项，需要先打开QueenBeautyTypeLUT 项
     色卡滤镜强度，值的范围[0,1]，默认0
     */
    /****
     * Feature: Filter Alpha
     * Parameter Range:[0,1]
     */
    kQueenBeautyParamsLUT              = 6,
    /** 脸部美颜参数项，需要先打开QueenBeautyTypeFaceBuffing 项
     白牙，值的范围[0,1]，默认0
     */
    /****
     * Feature: White Teeth
     * Parameter Range:[0,1]
     */
    kQueenBeautyParamsWhiteTeeth       = 7,
    /** 脸部美颜参数项，需要先打开QueenBeautyTypeFaceBuffing 项
     口红，值的范围[0,1]，默认0
     */
    /****
     * Feature: Lipstick
     * Parameter Range:[0,1]
     */
    kQueenBeautyParamsLipstick         = 8,
    /** 脸部美颜参数项，需要先打开QueenBeautyTypeFaceBuffing 项
     腮红，值的范围[0,1]，默认0
     */
    /****
     * Feature: Blush
     * Parameter Range:[0,1]
     */
    kQueenBeautyParamsBlush            = 9,
    /** 脸部美颜参数项，需要先打开QueenBeautyTypeFaceBuffing 项
     口红色相 [-0.5,0.5], 默认0
    */
    /****
     * Feature: Color Param for Lipstick
     * Parameter Range:[-0.5,0.5]
     */
    kQueenBeautyParamsLipstickColorParam  = 10,
    /** 脸部美颜参数项，需要先打开QueenBeautyTypeFaceBuffing 项
     口红饱和度[0,1], 默认0
    */
    /****
     * Feature: Gloss Param for Lipstick
     * Parameter Range:[0,1]
     */
    kQueenBeautyParamsLipstickGlossParam  = 11,
    /** 脸部美颜参数项，需要先打开QueenBeautyTypeFaceBuffing 项
     口红明度[0,1], 默认0
    */
    /****
     * Feature: Brightness for Lipstick
     * Parameter Range:[0,1]
     */
    kQueenBeautyParamsLipstickBrightnessParam = 12,
    /** 脸部美颜参数项，需要先打开QueenBeautyTypeFaceBuffing 项
     亮眼[0,1], 默认0
    */
    /****
     * Feature: Brighten Eye
     * Parameter Range:[0,1]
     */
    kQueenBeautyParamsBrightenEye         = 13,
    /** 脸部美颜参数项，需要先打开QueenBeautyTypeFaceBuffing 项
     红润[0,1], 默认0
    */
    /****
     * Feature: Skin Red
     * Parameter Range:[0,1]
     */
    kQueenBeautyParamsSkinRed             = 14,
    /** 脸部美颜参数项，需要先打开QueenBeautyTypeFaceBuffing 项
     祛皱纹[0,1], 默认0
    */
    /****
     * Feature: Wrinkles
     * Parameter Range:[0,1]
     */
    kQueenBeautyParamsWrinkles             = 15,
    /** 脸部美颜参数项，需要先打开QueenBeautyTypeFaceBuffing 项
     祛暗沉[0,1], 默认0
    */
    /****
     * Feature: Brighten Face
     * Parameter Range:[0,1]
     */
    kQueenBeautyParamsBrightenFace         = 16,
    /** 脸部美颜参数项，需要先打开QueenBeautyTypeFaceBuffing 项
     祛脖颈纹[0,1], 默认0
    */
    /****
     * Feature: Neck
     * Parameter Range:[0,1]
     */
    kQueenBeautyParamsNeck       = 20,
    /** 脸部美颜参数项，需要先打开QueenBeautyTypeFaceBuffing 项
     祛额头纹[0,1], 默认0
    */
    /****
     * Feature: Forehead
     * Parameter Range:[0,1]
     */
    kQueenBeautyParamsForehead      = 21,
    /** 需要先打开QueenBeautyTypeHSV 项
     饱和度[-1,1], 默认0
     */
    /****
     * Feature: Saturation
     * Parameter Range:[-1,1]
     */
    kQueenBeautyParamsHSVSaturation      = 22,
    /** 需要先打开QueenBeautyTypeHSV 项
     对比度[-1,1], 默认0
     */
    /****
     * Feature: Contrast
     * Parameter Range:[-1,1]
     */
    kQueenBeautyParamsHSVContrast      = 23,
    /** 需要先打开QueenBeautyTypeFaceMosaicing 项
     人脸马赛克[0,1], 默认0
     */
    /****
     * Feature: FaceMosaicing
     * Parameter Range:[0,1]
     */
    kQueenBeautyParamsFaceMosaicing      = 24

};

/**美妆类型，需要先打开QueenBeautyTypeMakeup 类型
 */
/****
 * Makeup Types
 */
typedef NS_ENUM(NSInteger, kQueenBeautyMakeupType) {
    /** 整妆
     */
    /****
     * Makeup Type: Whole
     */
    kQueenBeautyMakeupTypeWhole       = 0,
    /** 高光
     */
    /****
     * Makeup Type: Highlight
     */
    kQueenBeautyMakeupTypeHighlight   = 1,
    /** 美瞳
     */
    /****
     * Makeup Type: Contact lenses
     */
    kQueenBeautyMakeupTypeEyeball     = 2,
    /** 口红
     */
    /****
     * Makeup Type: Lipsticker
     */
    kQueenBeautyMakeupTypeMouth       = 3,
    /** 卧蚕
     */
    /****
     * Makeup Type: Lying silkworm
     */
    kQueenBeautyMakeupTypeWocan       = 4,
    /** 眉毛
     */
    /****
     * Makeup Type: Eyebrow
     */
    kQueenBeautyMakeupTypeEyeBrow     = 5,
    /** 腮红
     */
    /****
     * Makeup Type: Blush
     */
    kQueenBeautyMakeupTypeBlush       = 6,
    /** 眼影
     */
    /****
     * Makeup Type: Eye shadow
     */
    kQueenBeautyMakeupTypeEyeShadow   = 7,
    /** 眼线
     */
    /****
     * Makeup Type: Eyeliner
     */
    kQueenBeautyMakeupTypeEyeliner    = 8,
    /** 睫毛
     */
    /****
     * Makeup Type: Eyelash
     */
    kQueenBeautyMakeupTypeEyelash     = 9,
    /** 最大值
     */
    /****
     * Makeup Type count.
     */
    kQueenBeautyMakeupTypeMakeupMax   = 10
};

/**美型类型，需要先打开kQueenBeautyTypeFaceShape 类型
 */
/****
 * Face Shaping Types
 */
typedef NS_ENUM(NSInteger, kQueenBeautyFaceShapeType) {
    /** 颧骨，值的范围[0,1]，默认0
     */
    /****
     * Shaping Type：CutCheek
     * Range：[0,1]
     */
    kQueenBeautyFaceShapeTypeCutCheek      = 0,
    /** 削脸，值的范围[0,1]，默认0
     */
    /****
     * Shaping Type：CutFace
     * Range：[0,1]
     */
    kQueenBeautyFaceShapeTypeCutFace       = 1,
    /** 瘦脸，值的范围[0,1]，默认0
     */
    /****
     * Shaping Type：ThinFace
     * Range：[0,1]
     */
    kQueenBeautyFaceShapeTypeThinFace      = 2,
    /** 脸长，值的范围[0,1]，默认0
     */
    /****
     * Shaping Type：LongFace
     * Range：[0,1]
     */
    kQueenBeautyFaceShapeTypeLongFace      = 3,
    /** 下巴缩短，值的范围[-1,1]，默认0
     */
    /****
     * Shaping Type：LowerJaw
     * Range：[-1,1]
     */
    kQueenBeautyFaceShapeTypeLowerJaw      = 4,
    /** 下巴拉长，值的范围[-1,1]，默认0
     */
    /****
     * Shaping Type：HigherJaw
     * Range：[-1,1]
     */
    kQueenBeautyFaceShapeTypeHigherJaw     = 5,
    /** 瘦下巴，值的范围[0,1]，默认0
     */
    /****
     * Shaping Type：ThinJaw
     * Range：[0,1]
     */
    kQueenBeautyFaceShapeTypeThinJaw       = 6,
    /** 瘦下颌，值的范围[0,1]，默认0
     */
    /****
     * Shaping Type：ThinMandible
     * Range：[0,1]
     */
    kQueenBeautyFaceShapeTypeThinMandible  = 7,
    /** 大眼，值的范围[0,1]，默认0
     */
    /****
     * Shaping Type：BigEye
     * Range：[0,1]
     */
    kQueenBeautyFaceShapeTypeBigEye        = 8,
    /** 眼角1，值的范围[0,1]，默认0
     */
    /****
     * Shaping Type：EyeAngle1
     * Range：[0,1]
     */
    kQueenBeautyFaceShapeTypeEyeAngle1     = 9,
    /** 眼距，值的范围[-1,1]，默认0
     */
    /****
     * Shaping Type：Canthus
     * Range：[-1,1]
     */
    kQueenBeautyFaceShapeTypeCanthus       = 10,
    /** 拉宽眼距，值的范围[-1,1]，默认0
     */
    /****
     * Shaping Type：Canthus1
     * Range：[-1,1]
     */
    kQueenBeautyFaceShapeTypeCanthus1      = 11,
    /** 眼角2，值的范围[-1,1]，默认0
     */
    /****
     * Shaping Type：EyeAngle2
     * Range：[-1,1]
     */
    kQueenBeautyFaceShapeTypeEyeAngle2     = 12,
    /** 眼睛高度，值的范围[-1,1]，默认0
     */
    /****
     * Shaping Type：Eye Height
     * Range：[-1,1]
     */
    kQueenBeautyFaceShapeTypeEyeTDAngle    = 13,
    /** 瘦鼻，值的范围[0,1]，默认0
     */
    /****
     * Shaping Type：ThinNose
     * Range：[0,1]
     */
    kQueenBeautyFaceShapeTypeThinNose      = 14,
    /** 鼻翼，值的范围[0,1]，默认0
     */
    /****
     * Shaping Type：Nosewing
     * Range：[0,1]
     */
    kQueenBeautyFaceShapeTypeNosewing      = 15,
    /** 鼻长，值的范围[-1,1]，默认0
     */
    /****
     * Shaping Type：NasalHeight
     * Range：[-1,1]
     */
    kQueenBeautyFaceShapeTypeNasalHeight   = 16,
    /** 鼻头长，值的范围[-1,1]，默认0
     */
    /****
     * Shaping Type：NoseTipHeight
     * Range：[-1,1]
     */
    kQueenBeautyFaceShapeTypeNoseTipHeight = 17,
    /** 唇宽，值的范围[-1,1]，默认0
     */
    /****
     * Shaping Type：Mouth Width
     * Range：[-1,1]
     */
    kQueenBeautyFaceShapeTypeMouthWidth    = 18,
    /** 嘴唇大小，值的范围[-1,1]，默认0
     */
    /****
     * Shaping Type：Mouth Size
     * Range：[-1,1]
     */
    kQueenBeautyFaceShapeTypeMouthSize     = 19,
    /** 唇高，值的范围[-1,1]，默认0
     */
    /****
     * Shaping Type：Mouth High
     * Range：[-1,1]
     */
    kQueenBeautyFaceShapeTypeMouthHigh     = 20,
    /** 人中，值的范围[-1,1]，默认0
     */
    /****
     * Shaping Type：Philtrum
     * Range：[-1,1]
     */
    kQueenBeautyFaceShapeTypePhiltrum      = 21,
    /** 发际线，值的范围[-1,1]，默认0
     */
    /****
     * Shaping Type：Hair Line
     * Range：[-1,1]
     */
    kQueenBeautyFaceShapeTypeHairLine = 22,
    /** 嘴角上扬(微笑)，值的范围[0,1]，默认0
     */
    /****
     * Shaping Type：Smile
     * Range：[0,1]
     */
    kQueenBeautyFaceShapeTypeSmile = 23,
    /** 最大值
     */
    /****
     * Face Shaping Type count.
     */
    kQueenBeautyFaceShapeTypeMAX        = 24
};

/**美体类型，需要先打开kQueenBeautyTypeBodyShape 类型
 */
/****
 * Body Shaping Types
 */
typedef NS_ENUM(NSInteger, kQueenBeautyBodyShapeType) {
    /** 全身，值的范围[-1,1]，默认0
     */
    /****
     * Feature：Body
     * Range：[-1,1]
     */
    kQueenBeautyBodyShapeTypeFullBody = 0,
    /** 长腿，值的范围[-1,1]，默认0
     */
    /****
     * Feature：Leg
     * Range：[-1,1]
     */
    kQueenBeautyBodyShapeTypeLongLag,
    /** 小头，值的范围[-1,1]，默认0
     */
    /****
     * Feature：Head
     * Range：[-1,1]
     */
    kQueenBeautyBodyShapeTypeSmallHead,
    /** 瘦腿，值的范围[-1,1]，默认0
     */
    /****
     * Feature：ThinLag
     * Range：[-1,1]
     */
    kQueenBeautyBodyShapeTypeThinLag,
    /** 脖子，值的范围[-1,1]，默认0
     */
    /****
     * Feature：neck
     * Range：[-1,1]
     */
    kQueenBeautyBodyShapeTypeLongNeck,
    /** 瘦腰，值的范围[-1,1]，默认0
     */
    /****
     * Feature：Thin Waist
     * Range：[-1,1]
     */
    kQueenBeautyBodyShapeTypeThinWaist,
    /** 丰胸，值的范围[-1,1]，默认0
     */
    /****
     * Feature：Enhance Breast
     * Range：[-1,1]
     */
    kQueenBeautyBodyShapeTypeEnhanceBreast,
    /** 手臂，值的范围[-1,1]，默认0
     */
    /****
     * Feature：Thin Arm
     * Range：[-1,1]
     */
    kQueenBeautyBodyShapeTypeThinArm,
    /** 最大值
     */
    /****
     * Body Shaping Type count.
     */
    kQueenBeautyBodyShapeTypeMAX
};

/** 美妆混合模式
 */
/****
 * Makeup Blend Modes
 */
typedef NS_ENUM(NSInteger, kQueenBeautyBlend) {
    /** 正常
     */
    /****
     * Blend Mode：Normal
     */
    kQueenBeautyBlendNormal = 0,
    /** 变亮
     */
    /****
     * Blend Mode：Lighten
     */
    kQueenBeautyBlendLighten = 1,
    /** 变暗
     */
    /****
     * Blend Mode：Darken
     */
    kQueenBeautyBlendDarken = 2,
    /** 正片叠底
     */
    /****
     * Blend Mode：Multiply
     */
    kQueenBeautyBlendMultiply = 3,
    /** 划分
     */
    /****
     * Blend Mode：Divide
     */
    kQueenBeautyBlendDivide = 4,
    /** 平均
     */
    /****
     * Blend Mode：Average
     */
    kQueenBeautyBlendAverage = 5,
    /** 线性减淡
     */
    /****
     * Blend Mode：Add
     */
    kQueenBeautyBlendAdd = 6,
    /** 减去
     */
    /****
     * Blend Mode：Subtract
     */
    kQueenBeautyBlendSubtract = 7,
    /** 差值
     */
    /****
     * Blend Mode：Difference
     */
    kQueenBeautyBlendDifference = 8,
    /** 反向
     */
    /****
     * Blend Mode：Negation
     */
    kQueenBeautyBlendNegation = 9,
    /** 排除
     */
    /****
     * Blend Mode：Exclusion
     */
    kQueenBeautyBlendExclusion = 10,
    /** 滤色
     */
    /****
     * Blend Mode：color filter
     */
    kQueenBeautyBlendScreen = 11,
    /** 叠加
     */
    /****
     * Blend Mode：Overlay
     */
    kQueenBeautyBlendOverlay = 12,
    /** 柔光
     */
    /****
     * Blend Mode：SoftLight
     */
    kQueenBeautyBlendSoftLight = 13,
    /** 强光
     */
    /****
     * Blend Mode：HardLight
     */
    kQueenBeautyBlendHardLight = 14,
    /** 颜色减淡
     */
    /****
     * Blend Mode：ColorDodge
     */
    kQueenBeautyBlendColorDodge = 15,
    /** 颜色加深
     */
    /****
     * Blend Mode：ColorBurn
     */
    kQueenBeautyBlendColorBurn = 16,
    /** 线性减淡
     */
    /****
     * Blend Mode：Linear Dodge
     */
    kQueenBeautyBlendLinearDodge = 17,
    /** 线性加深
     */
    /****
     * Blend Mode：Linear Burn
     */
    kQueenBeautyBlendLinearBurn = 18,
    /** 线性光
     */
    /****
     * Blend Mode：Linear Light
     */
    kQueenBeautyBlendLinearLight = 19,
    /** 亮光
     */
    /****
     * Blend Mode：Vivid Light
     */
    kQueenBeautyBlendVividLight = 20,
    /** 点光
     */
    /****
     * Blend Mode：Pin Light
     */
    kQueenBeautyBlendPinLight = 21,
    /** 实色混合
     */
    /****
     * Blend Mode：Hard Mix
     */
    kQueenBeautyBlendHardMix = 22,
    /** 反射
     */
    /****
     * Blend Mode：Reflect
     */
    kQueenBeautyBlendReflect = 23,
    /** 发光
     */
    /****
     * Blend Mode：Glow
     */
    kQueenBeautyBlendGlow = 24,
    /** 颗粒
     */
    /****
     * Blend Mode：Phonenix
     */
    kQueenBeautyBlendPhoenix = 25,
    /** 色相
     */
    /****
     * Blend Mode：Hue
     */
    kQueenBeautyBlendHue = 26,
    /** 饱和度
     */
    /****
     * Blend Mode：Saturation
     */
    kQueenBeautyBlendSaturation = 27,
    /** 明度
     */
    /****
     * Blend Mode：Luminosity
     */
    kQueenBeautyBlendLuminosity = 28,
    /** 颜色
     */
    /****
     * Blend Mode：Color
     */
    kQueenBeautyBlendColor = 29,
    /** 曲线
     */
    /****
     * Blend Mode：Curve
     */
    kQueenBeautyBlendCurve = 30,
    /**
     * 视觉融合模式
     */
    /****
     * Blend Mode：Mix
     */
    kQueenBeautyBlendLabMix = 31,
    /** 最大值
     */
    /****
     * Makeup Blend Mode Count
     */
    kQueenBeautyBlendMax
};

/** 翻转类型
 */
/****
 * Flip Types
 */
typedef NS_ENUM(NSInteger, kQueenBeautyFlipAxis) {
    /** 不做翻转
     */
    /****
     * No Flip
     */
    kQueenBeautyFlipAxisNone       = 0,
    /** X轴翻转
     */
    /****
     * Flip Axis X
     */
    kQueenBeautyFlipAxisX   = 1,
    /** Y轴翻转
     */
    /****
     * Flip Axis Y
     */
    kQueenBeautyFlipAxisY     = 2,
};

/** 状态码
 */
/****
 * Result Codes
 */
typedef NS_ENUM(NSInteger, kQueenResultCode) {
    /** QueenEngine内部错误
     */
    /****
     * UnKnown Error
     */
    kQueenResultCodeUnKnown   = -1,
    /** 正常
     */
    /****
     * Success
     */
    kQueenResultCodeOK       = 0,
    /** license校验失败
     */
    /****
     * Invalid License
     */
    kQueenResultCodeInvalidLicense   = 1,
    /** 参数错误
     */
    /****
     * Invalid Param
     */
    kQueenResultCodeInvalidParam   = 2,
    /** 没有应用任何Queen的特效处理
     */
    /****
     * None of the effects are enabled
     */
    kQueenResultCodeNoEffect   = 3,
    /** 引擎没有配置信息
     */
    /****
     * Missing configuration information
     */
    kQueenResultCodeNoConfigInfo   = 4
};

/** 颜色格式
 */
/****
 * Color Space
 */
typedef NS_ENUM(NSInteger, kQueenImageFormat) {
    /** RGB
     */
    /****
     * Color Space: RGB
     */
    kQueenImageFormatRGB = 0,
    /** NV21
     */
    /****
     * Color Space: NV21
     */
    kQueenImageFormatNV21 = 1,
    /** NV12
     */
    /****
     * Color Space: NV12
     */
    kQueenImageFormatNV12 = 2,
    /** RGBA
     */
    /****
     * Color Space: RGBA
     */
    kQueenImageFormatRGBA = 3
};

/** 手势识别的类别
 */
/****
 * Gesture Type
 */
typedef NS_ENUM(NSInteger, kQueenStaticGestureType) {
    /** 没有识别到
     */
    /****
     * Gesture Type: None
     */
    kQueenStaticGestureTypeNone = -1,
    /** 其他手势
     */
    /****
     * Gesture Type: Others
     */
    kQueenStaticGestureTypeOther,
    /** 数字1 / 申食指
     */
    /****
     * Gesture Type: Number1
     */
    kQueenStaticGestureTypeNumber1,
    /** 数字2 / yeah / 剪刀手
     */
    /****
     * Gesture Type: Number2
     */
    kQueenStaticGestureTypeNumber2,
    /** 数字3
     */
    /****
     * Gesture Type: Number3
     */
    kQueenStaticGestureTypeNumber3,
    /** 数字4
     */
    /****
     * Gesture Type: Number4
     */
    kQueenStaticGestureTypeNumber4,
    /** 数字5
     */
    /****
     * Gesture Type: Number5
     */
    kQueenStaticGestureTypeNumber5,
    /** 数字6 / 电话
     */
    /****
     * Gesture Type: Number6
     */
    kQueenStaticGestureTypeNumber6,
    /** 数字7
     */
    /****
     * Gesture Type: Number7
     */
    kQueenStaticGestureTypeNumber7,
    /** 数字8 / 单指手枪
     */
    /****
     * Gesture Type: Number8
     */
    kQueenStaticGestureTypeNumber8,
    /** 数字9
     */
    /****
     * Gesture Type: Number9
     */
    kQueenStaticGestureTypeNumber9,
    /** 拳头
     */
    /****
     * Gesture Type: Fist
     */
    kQueenStaticGestureTypeFist,
    /** OK
     */
    /****
     * Gesture Type: OK
     */
    kQueenStaticGestureTypeOK,
    /** 单手比心
     */
    /****
     * Gesture Type: love
     */
    kQueenStaticGestureTypeHeart,
    /** 伸大拇指
     */
    /****
     * Gesture Type: ThumbUp
     */
    kQueenStaticGestureTypeThumbUp,
    /** 申小指和无名指
     */
    /****
     * Gesture Type: RingPinky
     */
    kQueenStaticGestureTypeRingPinky,
    /** 申小指
     */
    /****
     * Gesture Type: Pinky
     */
    kQueenStaticGestureTypePinky,
    /** 双指手枪
     */
    /****
     * Gesture Type: Gun
     */
    kQueenStaticGestureTypeGun,
    /** 摇滚 / 我爱你
     */
    /****
     * Gesture Type: ILoveyRock
     */
    kQueenStaticGestureTypeILoveyRock,
    /** 爪子
     */
    /****
     * Gesture Type: Paw
     */
    kQueenStaticGestureTypePaw,
    /** 中指
     */
    /****
     * Gesture Type: MidFinger
     */
    kQueenStaticGestureTypeMidFinger,
    /** VULCAN SALUTE 星际迷航手势
     */
    /****
     * Gesture Type: VulcanSalute
     */
    kQueenStaticGestureTypeVulcanSalute,
    /** 掐中指
     */
    /****
     * Gesture Type: MidTip
     */
    kQueenStaticGestureTypeMidTip,
    /** 掐无名指
     */
    /****
     * Gesture Type: RingTip
     */
    kQueenStaticGestureTypeRingTip,
    /** 杯子
     */
    /****
     * Gesture Type: Cup
     */
    kQueenStaticGestureTypeCup,
    /** 蛇
     */
    /****
     * Gesture Type: Snake
     */
    kQueenStaticGestureTypeSnake,
    /** 手指并拢
     */
    /****
     * Gesture Type: FingersTogether
     */
    kQueenStaticGestureTypeFingersTogether,
    /** 手势识别类型数量
     */
    /****
     * Gesture Type count
     */
    kQueenStaticGestureTypeMax,
};

/** 手部动作识别的类别
 */
/****
 * Dynamic gesture Types
 */
typedef NS_ENUM(NSInteger, kQueenHandActionType) {
    /** 未知类型
     */
    /****
     * Dynamic Gesture Type: UnKnown
     */
    kQueenHandActionTypeUnKnown = -2,
    /** 没有识别到
     */
    /****
     * Dynamic Gesture Type: None
     */
    kQueenHandActionTypeNone = -1,
    /** 拇指指左
     */
    /****
     * Dynamic Gesture Type: Thumb Left
     */
    kQueenHandActionTypeLeft,
    /** 拇指指右
     */
    /****
     * Dynamic Gesture Type: Thumb Right
     */
    kQueenHandActionTypeRight,
    /** 拜拜
     */
    /****
     * Dynamic Gesture Type: Goodbye
     */
    kQueenHandActionTypeGoodbye,
    /** 左滑
     */
    /****
     * Dynamic Gesture Type: SwipLeft
     */
    kQueenHandActionTypeSwipLeft,
    /** 右滑
     */
    /****
     * Dynamic Gesture Type: SwipRight
     */
    kQueenHandActionTypeSwipRight,
    /** 上滑
     */
    /****
     * Dynamic Gesture Type: SwipUp
     */
    kQueenHandActionTypeSwipUp,
    /** 下滑
     */
    /****
     * Dynamic Gesture Type: SwipDown
     */
    kQueenHandActionTypeSwipDown,
    /** 伸出大拇指
     */
    /****
     * Dynamic Gesture Type: ThumbUpDynamic
     */
    kQueenHandActionTypeThumbUpDynamic,
    /** 放大
     */
    /****
     * Dynamic Gesture Type: ZoomIn
     */
    kQueenHandActionTypeZoomIn,
    /** 缩小
     */
    /****
     * Dynamic Gesture Type: ZoomOut
     */
    kQueenHandActionTypeZoomOut,
    /** 手部动作识别类型数量
     */
    /****
     * Dynamic Gesture Type count
     */
    kQueenHandActionTypeMax,
};

/** 脸部表情识别的类别
 */
/****
 * Face Expression Types
 */
typedef NS_ENUM(NSInteger, kQueenFaceExpressionType) {
    /** 无表情
     */
    /****
     * Face Expression Types: None
     */
    kQueenFaceExpressionTypeNone = 0,
    /** 眨眼
     */
    /****
     * Face Expression Types: Blink
     */
    kQueenFaceExpressionTypeBlink,
    /** 摇头
     */
    /****
     * Face Expression Types: ShakingHead
     */
    kQueenFaceExpressionTypeShakingHead,
    /** 挑眉
     */
    /****
     * Face Expression Types: EyebrowRaise
     */
    kQueenFaceExpressionTypeEyebrowRaise,
    /** 张嘴
     */
    /****
     * Face Expression Types: OpenMouth
     */
    kQueenFaceExpressionTypeOpenMouth,
    /** 说话
     */
    /****
     * Face Expression Types: Speaking
     */
    kQueenFaceExpressionTypeSaying,
    /** 脸部表情识别的数量
     */
    /****
     * Face Expression Type count
     */
    kQueenFaceExpressionTypeMax,
};

/** 实景抠像背景处理方式
 */
/****
 * Background Process Types For AI Segment
 */
typedef NS_ENUM(NSInteger, kQueenBackgroundProcessType) {
    /** 背景虚化
     */
    /****
     * Background Process Type: Blur
     */
    kQueenBackgroundBlur        = 0,
    /** 背景透明
     */
    /****
     * Background Process Type: Transparent
     */
    kQueenBackgroundTransparent = 1,
    /** 背景正常替换
     */
    /****
     * Background Process Type: Normal
     */
    kQueenBackgroundNormal = 2,

};

/** 实景抠像性能模式
 */
/****
 * Performance Modes For AI Segment
 */
typedef NS_ENUM(NSInteger, kQueenSegmentPerformanceMode) {
    /** 自动模式（默认）
    */
    /****
     * Performance Mode For AI Segment: Auto
     */
    kQueenSegmentPMAuto        = 0,
    /** 最佳画质模式
    */
    /****
     * Performance Mode For AI Segment: Quality
     */
    kQueenSegmentPMQuality     = 1,
    /** 平衡模式
    */
    /****
     * Performance Mode For AI Segment: Balanced
     */
    kQueenSegmentPMBalanced    = 2,
    /** 最佳性能模式
    */
    /****
     * Performance Mode For AI Segment: Performance
     */
    kQueenSegmentPMPerformance = 3,
};

/** 颜色类型
 */
/****
 * Color Types For Pure Color Screen Matting
 */
typedef NS_ENUM(NSInteger, kQueenBeautyBgColorType) {
    /** 绿色
     */
    /****
     * Green
     */
    kQueenBeautyBgColorTypeGreen = 0,
    /** 蓝色
     */
    /****
     * Blue
     */
    kQueenBeautyBgColorTypeBlue = 1,
    /** 青色
     */
    /****
     * Cyan
     */
    kQueenBeautyBgColorTypeCyan = 2,
    /** 紫色
     */
    /****
     * Purple
     */
    kQueenBeautyBgColorTypePurple = 3,
    /** 黄色
     */
    /****
     * Yellow
     */
    kQueenBeautyBgColorTypeYellow = 4,
    /** 红色
     */
    /****
     * Red
     */
    kQueenBeautyBgColorTypeRed = 5,
};

/** 智能美型状态类型
 */
/****
 * Auto Face Shaping Status
 */
typedef NS_ENUM(NSInteger, kQueenAutoFaceShapeStatus) {
    /** 已激活
     */
    /****
     * Auto Face Shaping Status: Activated
     */
    kQueenAutoFSActivated = 0,
    /** 检测中
     */
    /****
     * Auto Face Shaping Status: Capturing
     */
    kQueenAutoFSCapturing = 1,
    /** 检测完成
     */
    /****
     * Auto Face Shaping Status: Captured
     */
    kQueenAutoFSCaptured = 2,
    /** 关闭
     */
    /****
     * Auto Face Shaping Status: Deactivated
     */
    kQueenAutoFSDeActivated = 3
};

/** 算法参数输入模式
 */
/****
 * Algorithm parameter input mode.
 */
typedef NS_ENUM(NSInteger, kQueenAlgInputMode) {
    /** 算法参数输入模式: 半自动。仅需要输入旋转角度
     */
    /****
     * Algorithm parameter input mode: semi automatic. Only need input rotation angle.
     */
    kQueenAlgInputModeSemiAutomatic = 0,
    /** 算法参数输入模式: 全自动。无需提供其他角度参数
     */
    /****
     * Algorithm parameter input mode: fully automatic. No need to provide other angle parameters.
     */
    kQueenAlgInputModeAutomatic = 1,
    /** 算法参数输入模式: 手动。需提供输入旋转角度、输出旋转角度、翻转参数等
     */
    /****
     * Algorithm parameter input mode: manual. Input rotation angle, output rotation angle, turnover parameters shall be provided.
     */
    kQueenAlgInputModeManual = 2
};

typedef struct{
    /** 俯仰角, 围绕X轴旋转
     */
    /****
     * Rotation around the vertical axis is called yaw.
     */
    float yaw;
    /** 偏航角, 围绕Y轴旋转
     */
    /****
     * Rotation around the side-to-side axis is called pitch.
     */
    float pitch;
    /** 翻滚角, 围绕Z轴旋转
     */
    /****
     * Rotation around the front-to-back axis is called roll.
     */
    float roll;
}QEYawPitchRoll;

/** 授权版本
 */
/****
 * License versions.
 */
typedef NS_ENUM(NSInteger, kQueenLicenseVer) {
    /** 默认版本
     */
    /****
     * Default version.
     */
    kQueenLicenseVerDefault        = 0,
    /** 版本1
     */
    /****
     * Version 1.
     */
    kQueenLicenseVer1 = 1,
};

/** 专注度类型
 */
/****
 * Concentration types.
 */
typedef NS_ENUM(NSInteger, kConcentrationType) {
    /** 眨眼
     */
    /****
     * Blink.
     */
    kConcentrationTypeBlink = 0,
    /** 张嘴
     */
    /****
     * Open mouth.
     */
    kConcentrationTypeOpenMouth = 1,
    /** 摇头
     */
    /****
     * Head shaking.
     */
    kConcentrationTypeShakingHead = 2,
    /** 头上下移动
     */
    /****
     * Head moves up and down.
     */
    kConcentrationTypeYPR_Head_UpDown = 3,
    /** 头左右移动
     */
    /****
     * Head moves left and right.
     */
    kConcentrationTypeYPR_Head_LeftRight = 4
};

/** 异常动作(物体)类型
 */
/****
 * Abnormal action types.
 */
typedef NS_ENUM(NSInteger, kAbnormalActionType) {
    /** 设备在左侧
     */
    /****
     * Device on the left side.
     */
    kAbnormalActionTypeScene_Video_Left = 0,
    /** 光线过暗
     */
    /****
     * Light is too dark.
     */
    kAbnormalActionTypeScene_Light_Dark_Out = 1,
    /** 光线过亮
     */
    /****
     * Light is too bright.
     */
    kAbnormalActionTypeScene_Light_Bright_Out = 2,
    /** 人物进入
     */
    /****
     * Someone enter.
     */
    kAbnormalActionTypeScene_Person_Enter = 3,
    /** 人物离开
     */
    /****
     * Someone exit.
     */
    kAbnormalActionTypeScene_Person_Exit = 4,
    /** 人物画面占比
     */
    /****
     * The ratio of the person to the screen.
     */
    kAbnormalActionTypeScene_Person_In_Rect_Ratio = 5,
    /** 发现帽子
     */
    /****
     * Hat detected.
     */
    kAbnormalActionTypeObjectDetect_Hat = 31,
    /** 发现手机
     */
    /****
     * Cellphone detected.
     */
    kAbnormalActionTypeObjectDetect_CellPhone = 32,
    /** 发现手表
     */
    /****
     * Watch detected.
     */
    kAbnormalActionTypeObjectDetect_Watch = 33,
    /** 发现头戴式耳机
     */
    /****
     * Headphone detected.
     */
    kAbnormalActionTypeObjectDetect_Headphone = 34,
    /** 发现入耳式耳机
     */
    /****
     * Earphone detected.
     */
    kAbnormalActionTypeObjectDetect_EarPhone = 35,
    /** 发现图片
     */
    /****
     * Picture detected.
     */
    kAbnormalActionTypeObjectDetect_Picture = 36,
    /** 发现显示器
     */
    /****
     * Moniter detected.
     */
    kAbnormalActionTypeObjectDetect_Moniter = 37,
    /** 发现手镯
     */
    /****
     * Bracelet detected.
     */
    kAbnormalActionTypeObjectDetect_Bracelet = 38,
    /** 发现围巾
     */
    /****
     * Scarf detected.
     */
    kAbnormalActionTypeObjectDetect_Scarf = 39,
    /** 低抬头
     */
    /****
     * Head moves up and down.
     */
    kAbnormalActionTypeAction_Head_UpDown = 100,
    /** 左右摆头
     */
    /****
     * Head moves left and right.
     */
    kAbnormalActionTypeAction_Head_LeftRight = 101,
    /** 摇头
     */
    /****
     * Head shaking.
     */
    kAbnormalActionTypeAction_Head_Shaking = 102,
    /** 起立
     */
    /****
     * Stand up.
     */
    kAbnormalActionTypeAction_Pose_Standup = 103,
    /** 坐下
     */
    /****
     * Sit down.
     */
    kAbnormalActionTypeAction_Pose_Sitting = 104,
    /** 举手
     */
    /****
     * Hands up.
     */
    kAbnormalActionTypeAction_Pose_Handup = 105,
    /** 人声说话
     */
    /****
     * Someone speaking.
     */
    kAbnormalActionTypeAction_Person_Speech = 106
};

/** 活体动作类型
 */
/****
 * Living human action types.
 */
typedef NS_ENUM(NSInteger, kLivingHumanActionType) {
    /** 眯眼
     */
    /****
     * Blink.
     */
    kLivingHumanActionType_Blink = 0,
    /** 张嘴哈欠
     */
    /****
     * Open mouth.
     */
    kLivingHumanActionType_OpenMouth = 1,
    /** 摇头晃脑
     */
    /****
     * Head shaking.
     */
    kLivingHumanActionType_ShakingHead = 2,
    /** 低抬头
     */
    /****
     * Head moves up and down.
     */
    kLivingHumanActionType_YPR_Head_UpDown = 3
};

@interface QueenEngineConfigInfo :NSObject

/**
 * licenseKey 在申请 Queen 授权时获得。如果是老版本的客户，之前申请授权时没有收到 licenseKey 的，请联系商务获取。
 */
/****
 * You can get a License Key after applying for authorization.
 */
@property (nonatomic, copy) NSString *licenseKey;

/**
 * 内置证书文件路径
 */
/****
 * Built-in certificate file path.
 */
@property (nonatomic, copy) NSString *licenseFile;

/**
 * 授权版本
 */
/****
 * License version.
 */
@property (nonatomic, assign) kQueenLicenseVer licenseVer;

/**
 * Queen使用到的资源根目录，保持不变请为nil
 */
/****
 * The root directory of QueenEngine resource, default is nil, the main bundle path.
 */
@property (nonatomic, copy) NSString *resRootPath;

/**
 * 是否内部自动创建GLContext，如果要处理CVPixelBuffer数据，需要设置为YES，默认为YES。
 */
/****
 * Whether to automatically create GLContext by QueenEngine, if you want to process CVPixelBufferDataRef, you need to set it to YES, the default is YES.
 */
@property (nonatomic, assign) BOOL withContext;

/**
 * 需要内部自动创建上下文的前提下，需要在创建引擎的时候同步创建GLContext，则设置为YES，默认为NO
 */
/****
 * If you need to create a GLContext by QueenEngine synchronously when you create an engine, set it to YES, the default to NO.
 */
@property (nonatomic, assign) BOOL createContextSync;

/**
 * 是否要渲染到屏幕，默认为NO
 */
/****
 * Whether to render to the screen, the default to NO.
 */
@property (nonatomic, assign) BOOL toScreen;

/**
 * 是否自动计算图像的inputAngle和outputAngle，默认为NO，API_AVAILABLE(ios(8.0))
 */
/****
 * Whether to set the inputAngle and outputAngle of the image automatically, the default is NO.
 */
@property (nonatomic, assign) BOOL autoSettingImgAngle;

/**
 * 算法参数输入模式
 */
/****
 * Algorithm parameter input mode, the default is kQueenAlgInputModeSemiAutomatic.
 */
@property (nonatomic, assign) kQueenAlgInputMode algInputMode;

/**
 * 是否运行在自定义线程，默认为YES
 */
/****
 * Whether to run in a custom thread, the default is YES.
 */
@property (nonatomic, assign) BOOL runOnCustomThread;

/**
 * 是否允许数据统计，默认为YES
 */
/****
 * Whether to allow data statistics , the default is YES.
 */
@property (nonatomic, assign) BOOL enableStats;

/**
 * 是否允许 SDK 往 控制台 输出详细日志，建议只在 Debug 包开启，避免日志输出过多影响性能，默认为NO
 */
/****
 * Whether to allow print more debug information , the default is NO.
 */
@property (nonatomic, assign) BOOL enableDebugLog;

/**
 * 预留字段，选填
 */
/****
 * Reserved field, optional.
 */
@property (nonatomic, copy) NSString *extraInfo;

@end

NS_ASSUME_NONNULL_END

#endif
