//
//  AQEViewController.m
//  Queen_Example
//
//  Created by Aliyun on 2021/3/14.
//  Copyright © 2021 Alibaba-inc. All rights reserved.
//

#import "AQEViewController.h"
#import "AQESimpleQueenExampleViewController.h"

@interface AQEViewController ()

@end

@implementation AQEViewController

- (void)viewDidLoad
{
    [super viewDidLoad];
    self.view.backgroundColor = [UIColor whiteColor];

    UIButton *button = [UIButton buttonWithType:UIButtonTypeCustom];
    button.backgroundColor = [UIColor blackColor];
    CGFloat buttonWidth = 100.0f;
    button.frame = CGRectMake((CGRectGetWidth(self.view.bounds) - buttonWidth) / 2,
                              (CGRectGetHeight(self.view.bounds) - buttonWidth) / 2,
                              buttonWidth,
                              buttonWidth);
    [button setTitle:@"start" forState:UIControlStateNormal];
    [button addTarget:self action:@selector(onStartBtnClick) forControlEvents:UIControlEventTouchUpInside];
    [self.view addSubview:button];
}

- (void)onStartBtnClick
{
    AQESimpleQueenExampleViewController *vc = [[AQESimpleQueenExampleViewController alloc] init];
    vc.modalPresentationStyle = UIModalPresentationFullScreen;
    [self presentViewController:vc animated:true completion:nil];
}

@end
