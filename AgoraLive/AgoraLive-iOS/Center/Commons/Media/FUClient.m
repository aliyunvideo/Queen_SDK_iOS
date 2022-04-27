//
//  FUClient.m
//  AgoraLive
//
//  Created by CavanSu on 2020/6/22.
//  Copyright © 2020 Agora. All rights reserved.
//

#import "FUClient.h"
#import "FURenderer.h"
#import "authpack.h"

typedef NS_ENUM(NSUInteger, FUItemLevel) {
    FUItemLevelBackground      = 0,
    FUItemLevelFace            = 1,
    FUItemLevelAntiAliasing    = 2,
};

@implementation FUFilterItem
@end

@interface FUClient () {
    int items[3];
    int itemsCount;
    int frameID;
}

@property (nonatomic, strong) NSArray *filterItems;
@property (nonatomic, strong) dispatch_queue_t asyncLoadQueue;

@property (nonatomic, assign) BOOL loadAISuccess;
@property (nonatomic, assign) BOOL loadFilterSuccess;
@property (nonatomic, assign) BOOL loadBackgroundSuccess;
@property (nonatomic, assign) BOOL loadAntiAliasingSuccess;

@property (nonatomic, assign) FUItemLevel filterIndexOfItems;
@property (nonatomic, assign) FUItemLevel backgroundIndexOfItems;
@property (nonatomic, assign) FUItemLevel animojiIndexOfItems;
@property (nonatomic, assign) FUItemLevel aliasingIndexOfItems;
@end

@implementation FUClient

- (instancetype)init {
    self = [super init];
    if (self) {
        [self initAll];
    }
    return self;
}

#pragma mark - Public
- (void)loadFilterWithSuccess:(FUCompletion)success fail:(FUErrorCompletion)fail {
    __weak typeof(self) weakSelf = self;
    
    dispatch_async(_asyncLoadQueue, ^{
        NSError *error = nil;
        
        #if FU_67
        if (![weakSelf checkLoadAIModel:&error]) {
            if (fail) {
                fail(error);
                return;
            }
        }
        #endif
        
        if (![weakSelf loadFilterWithError:&error]) {
            if (fail) {
                fail(error);
                return;
            }
        }
        
        if (success) {
            success();
        }
    });
}

- (FUFilterItem *)getFilterItemWithType:(FUFilterItemType)type {
    FUFilterItem *instance = nil;
    for (int i = 0; i < self.filterItems.count; i++) {
        FUFilterItem *item = self.filterItems[i];
        if (item.type == type) {
            instance = item;
            break;
        }
    }
    return instance;
}

- (void)setFilterValue:(float)value withType:(FUFilterItemType)type {
    __block FUFilterItem *item = [self getFilterItemWithType:type];
    
    if (!item) {
        return;
    }
    
    if (value > item.maxValue
        || value < item.minValue
        || !self.loadFilterSuccess) {
        return;
    }
    
    __weak typeof(self) weakSelf = self;
    
    dispatch_async(_asyncLoadQueue, ^{
        int result = [FURenderer itemSetParam:self->items[weakSelf.filterIndexOfItems]
                                     withName:item.funcName
                                        value:@(value)];
        if (result > 0) {
            item.value = value;
        }
    });
}

- (void)loadBackgroudWithSuccess:(FUCompletion)success fail:(FUErrorCompletion)fail; {
    __weak typeof(self) weakSelf = self;
    
    dispatch_async(_asyncLoadQueue, ^{
        NSError *error = nil;
        #if FU_67
        if (![weakSelf checkLoadAIModel:&error]) {
            if (fail) {
                weakSelf.loadBackgroundSuccess = false;
                fail(error);
                return;
            }
        }
        #endif
        
        if (![self checkLoadBackgroundWithError:&error]) {
            if (fail) {
                weakSelf.loadBackgroundSuccess = false;
                fail(error);
                return;
            }
        }
        
        if (success) {
            success();
        }
    });
}

- (void)loadAnimoji:(NSString *)name success:(FUCompletion)success fail:(FUErrorCompletion)fail; {
    __weak typeof(self) weakSelf = self;
    
    dispatch_async(_asyncLoadQueue, ^{
        NSError *error = nil;
        #if FU_67
        if (![weakSelf checkLoadAIModel:&error]) {
            if (fail) {
                fail(error);
                return;
            }
        }
        #endif
        
        if (![weakSelf checkLoadBackgroundWithError:&error]) {
            if (fail) {
                fail(error);
                return;
            }
        }
        
        if (![weakSelf checkLoadAntiAliasingWithError:&error]) {
            if (fail) {
                fail(error);
                return;
            }
        }
        
        if (![weakSelf loadAnimoji:name withError:&error]) {
            if (fail) {
                fail(error);
                return;
            }
        }
        
        if (success) {
            success();
        }
    });
}

- (void)renderItemsToPixelBuffer:(CVPixelBufferRef)pixelBuffer {
    [[FURenderer shareRenderer] renderPixelBuffer:pixelBuffer
                                      withFrameId:frameID
                                            items:items
                                        itemCount:itemsCount
                                            flipx:true];
    
    frameID += 1;
}

- (void)destoryAllItems {
    [FURenderer destroyAllItems];
    [self resetPropertys];
}

#pragma mark - Private
- (void)initAll {
    self.asyncLoadQueue = dispatch_queue_create("com.faceLoadItem", DISPATCH_QUEUE_SERIAL);
    __weak typeof(self) weakSelf = self;
    
    dispatch_async(_asyncLoadQueue, ^{
        #if FU_67
        [[FURenderer shareRenderer] setupWithData:nil
                                         dataSize:0
                                           ardata:nil
                                      authPackage:&g_auth_package
                                         authSize:sizeof(g_auth_package)
                              shouldCreateContext:YES];
        
        #else
        
        NSString *path = [[NSBundle mainBundle] pathForResource:@"v3.bundle" ofType:nil];
        
        [[FURenderer shareRenderer] setupWithDataPath:path
                                          authPackage:&g_auth_package
                                             authSize:sizeof(g_auth_package)
                                  shouldCreateContext:YES];
        #endif
        
        NSData *tongueData = [weakSelf getModelDataWithResourceName:@"tongue.bundle"];
        fuLoadTongueModel((void *)tongueData.bytes, (int)tongueData.length);
        float flexible = 0.5;
        fuSetFaceTrackParam((char *)[@"mouth_expression_more_flexible" UTF8String], &flexible);
        
        [weakSelf initPropertys];
        [weakSelf initFilterItems];
        
        #if FU_67
        [weakSelf loadAllAIModel:nil];
        #endif
    });
}

- (void)initPropertys {
    itemsCount = 3;
    
    self.backgroundIndexOfItems = FUItemLevelBackground;
    self.filterIndexOfItems = FUItemLevelFace;
    self.animojiIndexOfItems = FUItemLevelFace;
    self.aliasingIndexOfItems = FUItemLevelAntiAliasing;
    
    self.loadAISuccess = false;
    self.loadBackgroundSuccess = false;
    self.loadFilterSuccess = false;
    
    for (int i = 0; i < itemsCount; i++) {
        items[i] = 0;
    }
}

#pragma mark - Private AI Model
#if FU_67
- (BOOL)checkLoadAIModel:(NSError **)error {
    if (!self.loadAISuccess) {
        [FURenderer releaseAIModel:FUAITYPE_FACEPROCESSOR
         | FUAITYPE_HUMANPOSE2D
         | FUAITYPE_HAIRSEGMENTATION
         | FUAITYPE_HANDGESTURE
         | FUAITYPE_FACELANDMARKS209
         | FUAITYPE_BACKGROUNDSEGMENTATION];
        BOOL isSuccess = [self loadAllAIModel:error];
        return isSuccess;
    }
    return self.loadAISuccess;
}

- (BOOL)loadAllAIModel:(NSError **)error {
    BOOL result = [self loadAIModel:@"ai_bgseg.bundle"
                               type:FUAITYPE_BACKGROUNDSEGMENTATION
                          withError:error];
    
    if (!result) {
        self.loadAISuccess = false;
        return result;
    }
    
    result = [self loadAIModel:@"ai_gesture.bundle"
                          type:FUAITYPE_HANDGESTURE
                     withError:error];
    
    if (!result) {
        self.loadAISuccess = false;
        return result;
    }
    
    result = [self loadAIModel:@"ai_hairseg.bundle"
                          type:FUAITYPE_HAIRSEGMENTATION
                     withError:error];
    
    if (!result) {
        self.loadAISuccess = false;
        return result;
    }
    
    result = [self loadAIModel:@"ai_humanpose.bundle"
                          type:FUAITYPE_HUMANPOSE2D
                     withError:error];
    
    if (!result) {
        self.loadAISuccess = false;
        return result;
    }
    
    result = [self loadAIModel:@"ai_face_processor.bundle"
                          type:FUAITYPE_FACEPROCESSOR
                     withError:error];
    
    if (!result) {
        self.loadAISuccess = false;
        return result;
    }
    
    self.loadAISuccess = true;
    return self.loadAISuccess;
}

- (BOOL)loadAIModel:(NSString *)model type:(FUAITYPE)type withError:(NSError **)error {
    NSData *data = [self getModelDataWithResourceName:model];
    int result = [FURenderer loadAIModelFromPackage:(void *)data.bytes
                                               size:(int)data.length
                                             aitype:type];
    if (result <= 0) {
        if (error) {
            *error = [NSError errorWithDomain:[NSString stringWithFormat:@"FU-load-AI-%@", model]
                                         code:result
                                     userInfo:nil];
        }
        
        return false;
    }
    
    return true;
}
#endif

#pragma mark - Private Filter
- (BOOL)checkFilterWithError:(NSError **)error {
    if (!self.loadFilterSuccess) {
        [self loadFilterWithError:error];
        if (*error) {
            return false;
        }
    }
    
    return true;
}

- (BOOL)loadFilterWithError:(NSError **)error {
    int result = [self loadModel:@"face_beautification.bundle"];
    
    if (result <= 0) {
        *error = [NSError errorWithDomain:@"FU-load-face_beautification"
                                     code:result
                                 userInfo:nil];
        self.loadFilterSuccess = false;
        return self.loadFilterSuccess;
    }
    int bundleHandle = result;
    
    /* 默认自定义脸型 */
    [FURenderer itemSetParam:bundleHandle withName:@"face_shape" value:@(4)];
    
    self.loadFilterSuccess = true;
    self.filterIndexOfItems = 1;
    items[self.filterIndexOfItems] = bundleHandle;
    return self.loadFilterSuccess;
}

- (void)initFilterItems {
    FUFilterItem *smooth = [[FUFilterItem alloc] init];
    smooth.type = FUFilterItemTypeSmooth;
    smooth.funcName = @"blur_level";
    smooth.minValue = 0;
    smooth.maxValue = 6.0;
    smooth.defaultValue = 6.0;
    smooth.value = smooth.defaultValue;
    
    FUFilterItem *brighten = [[FUFilterItem alloc] init];
    brighten.type = FUFilterItemTypeBrighten;
    brighten.funcName = @"color_level";
    brighten.minValue = 0;
    brighten.maxValue = 2.0;
    brighten.defaultValue = 0.2;
    brighten.value = brighten.defaultValue;
    
    FUFilterItem *thinning = [[FUFilterItem alloc] init];
    thinning.type = FUFilterItemTypeThinning;
    thinning.funcName = @"cheek_thinning";
    thinning.minValue = 0;
    thinning.maxValue = 1.0;
    thinning.defaultValue = 0;
    thinning.value = thinning.defaultValue;
    
    FUFilterItem *eye = [[FUFilterItem alloc] init];
    eye.type = FUFilterItemTypeEye;
    eye.funcName = @"eye_enlarging";
    eye.minValue = 0;
    eye.maxValue = 1.0;
    eye.defaultValue = 0.5;
    eye.value = eye.defaultValue;
    
    self.filterItems = @[smooth, brighten, thinning, eye];
}

#pragma mark - Private Animoji
- (BOOL)checkLoadBackgroundWithError:(NSError **)error {
    if (!self.loadBackgroundSuccess) {
        [self loadBackgroundWithError:error];
        if (*error) {
            return false;
        }
    }
    
    return true;
}

- (BOOL)loadBackgroundWithError:(NSError **)error {
    int result = [self loadModel:@"bg.bundle"];
    
    if (result <= 0) {
        *error = [NSError errorWithDomain:@"FU-load-background"
                                     code:result
                                 userInfo:nil];
        self.loadBackgroundSuccess = false;
        return self.loadBackgroundSuccess;
    }
    
    self.loadBackgroundSuccess = true;
    items[self.backgroundIndexOfItems] = result;
    return self.loadBackgroundSuccess;
}

- (BOOL)checkLoadAntiAliasingWithError:(NSError **)error {
    if (!self.loadAntiAliasingSuccess) {
        [self loadAntiAliasingWithError:error];
        if (*error) {
            return false;
        }
    }
    
    return true;
}

- (BOOL)loadAntiAliasingWithError:(NSError **)error {
    int result = [self loadModel:@"fxaa.bundle"];
    
    if (result <= 0) {
        *error = [NSError errorWithDomain:@"FU-load-AntiAliasingj"
                                     code:result
                                 userInfo:nil];
        self.loadAntiAliasingSuccess = false;
        return self.loadAntiAliasingSuccess;
    }
    
    self.loadAntiAliasingSuccess = true;
    items[self.loadAntiAliasingSuccess] = result;
    return self.loadAntiAliasingSuccess;
}

- (BOOL)loadAnimoji:(NSString *)name withError:(NSError **)error {
    NSString *bundle = [NSString stringWithFormat:@"%@.bundle", name];
    int result = [self loadModel:bundle];
    
    if (result <= 0) {
        *error = [NSError errorWithDomain:@"FU-load-animoji"
                                     code:result
                                 userInfo:nil];
        return false;
    }
    
    int itemHandle = result;
    items[self.animojiIndexOfItems] = itemHandle;
    
    [FURenderer itemSetParam:itemHandle withName:@"{\"thing\":\"<global>\",\"param\":\"follow\"}" value:@(0)];
    [FURenderer itemSetParam:itemHandle withName:@"isAndroid" value:@(0)];
    
    return true;
}

// return: Handle value
- (int)loadModel:(NSString *)model {
    NSString *path = [[NSBundle mainBundle] pathForResource:model ofType:nil];
    int itemHandle = [FURenderer itemWithContentsOfFile:path];
    return itemHandle;
}

- (void)resetPropertys {
    [self initPropertys];
    
    FUFilterItem *smooth = [self getFilterItemWithType:FUFilterItemTypeSmooth];
    smooth.value = smooth.defaultValue;
    
    FUFilterItem *brighten = [self getFilterItemWithType:FUFilterItemTypeBrighten];
    brighten.value = brighten.defaultValue;
    
    FUFilterItem *thinning = [self getFilterItemWithType:FUFilterItemTypeThinning];
    thinning.value = thinning.defaultValue;
    
    FUFilterItem *eye = [self getFilterItemWithType:FUFilterItemTypeEye];
    eye.value = eye.defaultValue;
}

- (NSData *)getModelDataWithResourceName:(NSString *)resource {
    return [NSData dataWithContentsOfFile:[[NSBundle mainBundle] pathForResource:resource ofType:nil]];
}
@end
