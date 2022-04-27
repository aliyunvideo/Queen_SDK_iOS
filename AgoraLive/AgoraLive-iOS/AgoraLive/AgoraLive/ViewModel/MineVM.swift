//
//  MineVM.swift
//  AgoraLive
//
//  Created by CavanSu on 2020/3/10.
//  Copyright © 2020 Agora. All rights reserved.
//

import UIKit
import RxSwift
import RxRelay

class MineVM: NSObject {
    private var abstraction: CurrentUser {
        get {
            ALCenter.shared().centerProvideLocalUser()
        }
    }
    
    private let bag = DisposeBag()
    
    var userName = BehaviorRelay(value: "")
    var head = BehaviorRelay(value: UIImage())
    
    override init() {
        super.init()
        
        ALCenter.shared().isWorkNormally.subscribe(onNext: { [unowned self] (isWork) in
            guard isWork else {
                return
            }
            
            self.observe()
        }).disposed(by: bag)
    }
    
    func updateNewName(_ new: String, completion: Completion) {
        let abstraction = self.abstraction
        let info = CurrentUser.UpdateInfo(userName: new)
        abstraction.updateInfo(info, success: completion, fail: completion)
    }
}

private extension MineVM {
    func observe() {
        let abstraction = self.abstraction
        let images = ALCenter.shared().centerProvideImagesHelper()
        
        abstraction.info.subscribe(onNext: { [weak self] (newInfo) in
            guard let strongSelf = self else {
                return
            }
            
            strongSelf.userName.accept(newInfo.name)
            let head = images.getHead(index: newInfo.imageIndex)
            strongSelf.head.accept(head)
        }).disposed(by: bag)
    }
}
