//
//  QueenMaterial.h
//  queen
//
//  Created by zhangjc on 2022/6/30.
//  Copyright © 2022 alibabaAliyun. All rights reserved.
//

#ifndef QueenMaterial_H
#define QueenMaterial_H

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

/** 资源类型
 */
/****
 * Material type
 */
typedef NS_ENUM(NSUInteger, kQueenMaterialType) {
    /** 模型资源
     */
    /****
     * Model resources
     */
    kQueenMaterialModel = 0,
    /** 贴纸
     */
    /****
     * Sticker resources
     */
    kQueenMaterialSticker,
    /** 滤镜
     */
    /****
     * Filter resources
     */
    kQueenMaterialLUT,
    /** 美妆
     */
    /****
     * Makeup resources
     */
    kQueenMaterialMakeup,
    /** 背景
     */
    /****
     * Background resources
     */
    kQueenMaterialBackground
};

@protocol QueenMaterialDelegate <NSObject>

@optional

/**
 * 资源下载完成的回调。
 * @param type 资源类型。
 */
/****
 * Callback of resource download completion.
 * @param type materialType.
 */
- (void)queenMaterialOnReady:(kQueenMaterialType)type;

/**
 * 资源下载进度的回调。
 * @param currentSize 已下载的资源大小。
 * @param totalSize 需要下载资源的总大小。
 * @param type 资源类型。
 */
/****
 * Callback of resource download progress.
 * @param currentSize Downloaded size of resources.
 * @param totalSize The total size of resources.
 * @param type materialType.
 */
- (void)queenMaterialOnProgress:(kQueenMaterialType)type withCurrentSize:(int)currentSize withTotalSize:(int)totalSize withProgess:(float)progress;

/**
 * 资源下载错误的回调，应该检查网络与磁盘空间是否出现问题。
 * @param type 资源类型。
 */
/****
 * For the callback of resource download errors, you should check whether there are problems in the network and disk space.
 * @param type materialType.
 */
- (void)queenMaterialOnError:(kQueenMaterialType)type;

@end

@interface QueenMaterial : NSObject

/** QueenMaterial回调代理对象。
 */
/****
 * Specifies the delegate of QueenMaterial.
 */
@property (nonatomic, weak) id <QueenMaterialDelegate> delegate;

/** QueenMaterial的通用对象。
 */
/****
 * The shared instance of QueenMaterial.
 */
+ (instancetype)sharedInstance;

/** 获取已下载好的资源路径。
 * @param type 资源类型。
 * @return 资源路径。
 */
/****
 * Get the local path of the resource package.
 * @param type materialType.
 * @return The local path of the resource package.
 */
- (NSString *)getMaterialPath:(kQueenMaterialType)type;

/** 请求下载资源包。
 * @param type 资源类型。
 * @return 资源是否需要下载。
 */
/** Request to download resource package.
 * @param type materialType.
 * @return Whether the resource need to be downloaded.
 */
- (BOOL)requestMaterial:(kQueenMaterialType)type;

/**
 * 设置自定义资源包链接，必须先于资源下载的请求。
 * @param type 资源类型。
 * @param materialURL 资源的下载地址。
 */
/****
 * Setting the custom resource package link must precede the resource download request
 * @param type materialType.
 * @param materialURL The URL of resource package.
 */
- (void)setMaterialURL:(kQueenMaterialType)type withURL:(NSString *)materialURL;

@end

NS_ASSUME_NONNULL_END

#endif
