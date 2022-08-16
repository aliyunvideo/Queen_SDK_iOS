# Queen SDK

[![Version](https://img.shields.io/cocoapods/v/Queen.svg?style=flat)](https://cocoapods.org/pods/Queen)
[![License](https://img.shields.io/cocoapods/l/Queen.svg?style=flat)](https://cocoapods.org/pods/Queen)
[![Platform](https://img.shields.io/cocoapods/p/Queen.svg?style=flat)](https://cocoapods.org/pods/Queen)

## 简介
- 本文为您介绍iOS端[阿里云Queen SDK](https://help.aliyun.com/document_detail/211047.html?spm=a2c4g.11186623.6.735.6a1b192eB31nYi)的接口文档、集成操作及简单使用示例，用于实现美颜特效功能。

### 设备和系统版本

- iOS设备：armv7或arm64的移动设备（iPad/iPhone，不包含支持arm64架构的Mac）
- iOS系统版本：iOS 9.0及以上

## FAQ
### 常见问题FAQ，参见：[FAQ](https://github.com/aliyunvideo/Queen_SDK_Android/blob/main/FAQ.md "Queen使用FAQ")

## 集成Queen SDK

支持pods与本地集成两种方式。

### pods集成方式：
#### pro版本：
```ruby
pod 'Queen', '2.3.0-official-pro'
```
#### ultimate版本：
```ruby
pod 'Queen', '2.3.0-official-ultimate'
```
#### full版本：
```ruby
pod 'Queen', '2.3.0-official-full'
```

### 本地集成方式：
1. 下载并解压Sample示例工程，获取以下framework文件:
```
queen.framework
opencv2.framework
```
2. 打开Xcode，在工程target的General页签下，在Frameworks, Libraries, and Embedded Content区域中添加以上framework，并将以上添加的framework的Embed属性设置成Embed & Sign。
3. 将获取到的queen.framework文件中的mnn.metallib添加到工程目录中。(只有full版本和ultimate版本需要)

## 使用示例
### 1. 初始化引擎
```
// 初始化引擎配置信息对象
QueenEngineConfigInfo *configInfo = [QueenEngineConfigInfo new];
// 填写licenseKey和licenseFile，也可以在应用的Info.plist文件加入AlivcLicenseKey字段和AlivcLicenseFile字段，内容类型为字符串，填入相应值即可。优先级：configInfo指定>Info.plist指定
configInfo.licenseKey = @"xoQuEoYgOHs225cwh634fa27df8d3490e88337161a4d5ee92";
// 相当于mainBundlePath的文件路径，示例工程的crt文件放在跟目录，所以直接填写文件名即可。
configInfo.licenseFile = @"AliVideoCert_1649317505780.crt";
// 设置资源根目录
NSString *bundlPath = [[NSBundle mainBundle] bundlePath];
configInfo.resRootPath = [bundlPath stringByAppendingString:@"/res"];
// 引擎初始化
self.beautyEngine = [[QueenEngine alloc] initWithConfigInfo:configInfo];
```
### 2. 启用效果并设置效果参数
```
// 打开磨皮锐化功能开关
[self.beautyEngine setQueenBeautyType:kQueenBeautyTypeSkinBuffing enable:YES];
// 设置磨皮系数
[self.beautyEngine setQueenBeautyParams:kQueenBeautyParamsSkinBuffing value:0.5f];
```
### 3. 处理图像数据
```
// 新建图像处理数据对象
QEPixelBufferData *bufferData = [QEPixelBufferData new];
// 输入待处理图像数据
bufferData.bufferIn = pixelBufferRef;
// 输出处理后的图像数据
bufferData.bufferOut = pixelBufferRef;
// 开始处理数据，执行此方法的线程需要始终是同一条线程
[self.beautyEngine processPixelBuffer:bufferData];
```
### 4. 销毁引擎
```
// 释放引擎，确保当前线程与执行processPixelBuffer:是同一条线程
[self.beautyEngine destroyEngine];
self.beautyEngine = nil;
```
### 更多功能使用示例可参考本项目目录下的Example/Queen/AQESimpleQueenExampleViewController.m文件

## License

Queen is available under the Apache License, Version 2.0. See the LICENSE file for more info.
