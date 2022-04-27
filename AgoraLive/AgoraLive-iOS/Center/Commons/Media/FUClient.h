//
//  FUClient.h
//  AgoraLive
//
//  Created by CavanSu on 2020/6/22.
//  Copyright © 2020 Agora. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <AVFoundation/AVFoundation.h>

NS_ASSUME_NONNULL_BEGIN

typedef void (^FUCompletion)(void);
typedef void (^FUErrorCompletion)(NSError *error);

typedef NS_ENUM(NSUInteger, FUFilterItemType) {
    FUFilterItemTypeSmooth      = 1,
    FUFilterItemTypeBrighten    = 2,
    FUFilterItemTypeThinning    = 3,
    FUFilterItemTypeEye         = 4
};

@interface FUFilterItem : NSObject
@property (nonatomic, assign) FUFilterItemType type;
@property (nonatomic, assign) float defaultValue;
@property (nonatomic, assign) float minValue;
@property (nonatomic, assign) float maxValue;
@property (nonatomic, assign) float value;
@property (nonatomic, copy) NSString *funcName;
@end

@interface FUClient : NSObject
- (void)loadFilterWithSuccess:(FUCompletion)success fail:(FUErrorCompletion)fail;
- (void)setFilterValue:(float)value withType:(FUFilterItemType)type;
- (FUFilterItem *)getFilterItemWithType:(FUFilterItemType)type;

- (void)loadBackgroudWithSuccess:(FUCompletion)success fail:(FUErrorCompletion)fail;
- (void)loadAnimoji:(NSString *)name success:(FUCompletion)success fail:(FUErrorCompletion)fail;
- (void)renderItemsToPixelBuffer:(CVPixelBufferRef)pixelBuffer;
- (void)destoryAllItems;
@end

NS_ASSUME_NONNULL_END
