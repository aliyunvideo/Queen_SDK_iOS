//
//  ThirdBeautyEntranceViewController.m
//  TRTC-API-Example-OC
//
//  Created by WesleyLei on 2021/8/17.
//

#import "ThirdBeautyBytedViewController.h"
#import "ThirdBeautyEntranceViewController.h"
#import "ThirdBeautyViewController.h"
#import "ThirdBeautyAliQueenViewController.h"
@interface ThirdBeautyEntranceViewController ()
@property(nonatomic, strong) UIButton *beautyButton;
@property(nonatomic, strong) UIButton *bytedButton;
@property(nonatomic, strong) UIButton *aliQueenButton;

@end

@implementation ThirdBeautyEntranceViewController

- (void)viewDidLoad {
  [super viewDidLoad];
  self.title = Localize(@"TRTC-API-Example.Home.ThirdBeauty");
  [self.view addSubview:self.aliQueenButton];
  [self.view addSubview:self.beautyButton];
  [self.view addSubview:self.bytedButton];
}

- (UIButton *)aliQueenButton {
    if (!_aliQueenButton) {
        _aliQueenButton = [UIButton buttonWithType:UIButtonTypeCustom];
        _aliQueenButton.frame = CGRectMake((self.view.frame.size.width - 300) / 2,
                                           self.view.frame.size.height * 0.5 - 75 - 75, 300, 50);
        [_aliQueenButton setTitleColor:UIColor.whiteColor forState:UIControlStateNormal];
        _aliQueenButton.layer.cornerRadius = 5;
        _aliQueenButton.backgroundColor = [UIColor greenColor];
        [_aliQueenButton setTitle:@"AliQueenButton" forState:UIControlStateNormal];
        [_aliQueenButton addTarget:self
                            action:@selector(clickAliQueenButton)
                  forControlEvents:UIControlEventTouchUpInside];
    }
    return _aliQueenButton;
}

- (UIButton *)beautyButton {
  if (!_beautyButton) {
    _beautyButton = [UIButton buttonWithType:UIButtonTypeCustom];
    _beautyButton.frame = CGRectMake((self.view.frame.size.width - 300) / 2,
                                     self.view.frame.size.height * 0.5 - 75, 300, 50);
    _beautyButton.layer.cornerRadius = 5;
    [_beautyButton setTitleColor:UIColor.whiteColor forState:UIControlStateNormal];
    _beautyButton.backgroundColor = [UIColor greenColor];
    [_beautyButton setTitle:@"beautyButton" forState:UIControlStateNormal];
    [_beautyButton addTarget:self
                      action:@selector(clickBeautyButton)
            forControlEvents:UIControlEventTouchUpInside];
  }
  return _beautyButton;
}

- (UIButton *)bytedButton {
  if (!_bytedButton) {
    _bytedButton = [UIButton buttonWithType:UIButtonTypeCustom];
    _bytedButton.frame = CGRectMake((self.view.frame.size.width - 300) / 2,
                                    self.view.frame.size.height * 0.5 + 15, 300, 50);
    [_bytedButton setTitleColor:UIColor.whiteColor forState:UIControlStateNormal];
    _bytedButton.layer.cornerRadius = 5;
    _bytedButton.backgroundColor = [UIColor greenColor];
    [_bytedButton setTitle:@"BytedButton" forState:UIControlStateNormal];
    [_bytedButton addTarget:self
                     action:@selector(clickBytedButton)
           forControlEvents:UIControlEventTouchUpInside];
  }
  return _bytedButton;
}

- (void)clickBeautyButton {
  UIViewController *controller =
      [[ThirdBeautyViewController alloc] initWithNibName:@"ThirdBeautyViewController" bundle:nil];
  [self.navigationController pushViewController:controller animated:true];
}

- (void)clickBytedButton {
  UIViewController *controller =
      [[ThirdBeautyBytedViewController alloc] initWithNibName:@"ThirdBeautyBytedViewController"
                                                       bundle:nil];
  [self.navigationController pushViewController:controller animated:true];
}

- (void)clickAliQueenButton {
    UIViewController *controller =
    [[ThirdBeautyAliQueenViewController alloc] initWithNibName:@"ThirdBeautyAliQueenViewController"
                                                        bundle:nil];
    [self.navigationController pushViewController:controller animated:true];
}

@end

