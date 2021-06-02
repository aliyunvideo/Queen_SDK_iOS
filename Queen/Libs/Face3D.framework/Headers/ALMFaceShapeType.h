//
//  ALMFaceShapeType.h
//  AliMedia
//
//  Created by 天荫 on 2018/10/2.
//  Copyright © 2018年 alibaba. All rights reserved.
//

#ifndef POSEFaceShapeType_h
#define POSEFaceShapeType_h

typedef enum ALMFaceShapeType{
    ALMFaceShapeTypeCutCheek      = 0,    //颧骨
    ALMFaceShapeTypeCutFace       = 1,    //削脸
    ALMFaceShapeTypeThinFace      = 2,    //瘦脸
    ALMFaceShapeTypeLongFace      = 3,    //脸长
    ALMFaceShapeTypeLowerJaw      = 4,    //下巴缩短
    ALMFaceShapeTypeHigherJaw     = 5,    //下巴拉长
    ALMFaceShapeTypeThinJaw       = 6,    //瘦下巴
    ALMFaceShapeTypeThinMandible  = 7,    //瘦下颌
    ALMFaceShapeTypeBigEye        = 8,    //大眼
    ALMFaceShapeTypeEyeAngle1     = 9,    //眼角1
    ALMFaceShapeTypeCanthus       = 10,   //眼距
    ALMFaceShapeTypeCanthus1      = 11,   //拉宽眼距
    ALMFaceShapeTypeEyeAngle2     = 12,   //眼角2
    ALMFaceShapeTypeEyeTDAngle    = 13,   //眼睛高度
    ALMFaceShapeTypeThinNose      = 14,   //瘦鼻
    ALMFaceShapeTypeNosewing      = 15,   //鼻翼
    ALMFaceShapeTypeNasalHeight   = 16,   //鼻长
    ALMFaceShapeTypeNoseTipHeight = 17,   //鼻头长
    ALMFaceShapeTypeMouthWidth    = 18,   //唇宽
    ALMFaceShapeTypeMouthSize     = 19,   //嘴唇大小
    ALMFaceShapeTypeMouthHigh     = 20,   //唇高
    ALMFaceShapeTypePhiltrum      = 21,   //人中
    ALMFaceShapeTypMAX            = 22    //最大值
}ALMFaceShapeType;

typedef enum ALMFaceType{
    PEAR    = 0,   //梨形脸
    HEART   = 1,   //心形脸
    SQUARE  = 2,   //方形脸
    OVAL    = 3,   //鹅蛋脸
    LONG    = 4,   //长脸
    ROUND   = 5,   //圆脸
}ALMFaceType;

typedef enum ALMMakeupType{
    ALMMakeupWhole     = 0,   //整妆
    ALMMakeupHighlight = 1,   //高光
    ALMMakeupEyeball   = 2,   //美瞳
    ALMMakeupMouth     = 3,   //口红
    ALMMakeupEyeBrow   = 4,   //眼妆
    ALMMakeupBrow      = 5,   //眉毛
    ALMMakeupSticker   = 6,   //美妆106点位
    ALMMakeup3D        = 7,   //3D美妆(由于素材较大，保持为最后一个）
}ALMMakeupType;

typedef enum ALMBlendType{
    ALMBlendMultiply  = 0,   //正片叠底
    ALMBlendOverlay   = 1,   //叠加
    ALMBlendScreen    = 2,   //滤光
    ALMBlendSoftlight = 3,   //柔光
}ALMBlendType;

#endif /* POSEFaceShapeType_h */
