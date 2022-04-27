//
//  MainTabBarViewController.swift
//  AgoraLive
//
//  Created by CavanSu on 2020/3/3.
//  Copyright © 2020 Agora. All rights reserved.
//

import UIKit
import RxSwift

class MainTabBarViewController: MaskTabBarController {
    let bag = DisposeBag()
    
    override func viewDidLoad() {
        super.viewDidLoad()
        tabBar.isUserInteractionEnabled = false
        
        showHUD()
        ALCenter.shared().isWorkNormally.subscribe(onNext: { [unowned self] (normal) in
            if normal {
                self.tabBar.isUserInteractionEnabled = true
                self.hiddenHUD()
            }
        }).disposed(by: bag)
    }
    
    func findFirstChild<T: Any>(of class: T.Type) -> T? {
        var target: T?
        
        for child in children {
            if child is T {
                target = child as? T
                break
            }
            
            for item in child.children where item is T {
                target = item as? T
                break
            }
            
            if let _ = target {
                break
            }
        }
        
        return target
    }
}
