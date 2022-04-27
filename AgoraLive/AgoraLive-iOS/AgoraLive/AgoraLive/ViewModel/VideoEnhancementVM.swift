//
//  VideoEnhancementVM.swift
//  AgoraLive
//
//  Created by CavanSu on 2020/4/28.
//  Copyright © 2020 Agora. All rights reserved.
//

import UIKit
import RxSwift
import RxRelay

class VideoEnhancementVM: NSObject {
    private let bag = DisposeBag()
    
    // Rx
    private(set) lazy var beauty = BehaviorRelay(value: enhancement.beauty)
    private(set) lazy var smooth = BehaviorRelay(value: enhancement.getFilterItem(with: .smooth).value)
    private(set) lazy var brighten = BehaviorRelay(value: enhancement.getFilterItem(with: .brighten).value)
    private(set) lazy var thinning = BehaviorRelay(value: enhancement.getFilterItem(with: .thinning).value)
    private(set) lazy var eyeEnlarge = BehaviorRelay(value: enhancement.getFilterItem(with: .eye).value)
    
    private(set) lazy var virtualAppearance = BehaviorRelay(value: VirtualAppearance.girl)
    
    var minSmooth: Float {
        return enhancement.getFilterItem(with: .smooth).minValue
    }
    
    var maxSmooth: Float {
        return enhancement.getFilterItem(with: .smooth).maxValue
    }
    
    var minBrighten: Float {
        return enhancement.getFilterItem(with: .brighten).minValue
    }
    
    var maxBrighten: Float {
        return enhancement.getFilterItem(with: .brighten).maxValue
    }
    
    var minThinning: Float {
        return enhancement.getFilterItem(with: .thinning).minValue
    }
    
    var maxThinning: Float {
        return enhancement.getFilterItem(with: .thinning).maxValue
    }
    
    var minEyeEnlarge: Float {
        return enhancement.getFilterItem(with: .eye).minValue
    }
    
    var maxEyeEnlarge: Float {
        return enhancement.getFilterItem(with: .eye).maxValue
    }
    
    override init() {
        super.init()
        observerPropertys()
    }
}

extension VideoEnhancementVM {
    private var enhancement: VideoEnhancement {
       return  ALCenter.shared().centerProvideMediaHelper().enhancement
    }
    
    func beauty(_ action: AGESwitch) {
        switch action {
        case .on:
            self.enhancement.beauty(.on, success: { [unowned self] in
                DispatchQueue.main.async { [weak self] in
                    self?.beauty.accept(.on)
                }
            }) { [unowned self] in
                DispatchQueue.main.async { [weak self] in
                    self?.beauty.accept(.off)
                }
            }
        case .off:
            self.enhancement.beauty(.off)
            self.beauty.accept(.off)
            smooth.accept(enhancement.getFilterItem(with: .smooth).value)
            brighten.accept(enhancement.getFilterItem(with: .brighten).value)
            thinning.accept(enhancement.getFilterItem(with: .thinning).value)
            eyeEnlarge.accept(enhancement.getFilterItem(with: .eye).value)
        }
    }
    
    func virtualAppearance(_ appearance: VirtualAppearance, success: Completion, fail: Completion) {
        enhancement.virtualAppearance(appearance, success: { [weak self] in
            UserDefaults.standard.set(appearance.item, forKey: "VirtualAppearance")
            DispatchQueue.main.async { [weak self] in
                self?.virtualAppearance.accept(appearance)
                if let success = success {
                    success()
                }
            }
        }) { [weak self] in
            DispatchQueue.main.async { [weak self] in
                self?.virtualAppearance.accept(.none)
                if let fail = fail {
                    fail()
                }
            }
        }
    }
    
    func localVirtualAppearance(success: Completion = nil, fail: Completion = nil) {
        guard let string = UserDefaults.standard.value(forKey: "VirtualAppearance") as? String else {
            return
        }
        
        let item = VirtualAppearance.item(string)
        virtualAppearance(item, success: success, fail: fail)
    }
    
    func reset() {
        beauty.accept(.off)
        virtualAppearance.accept(.none)
        enhancement.reset()
    }
}

extension VideoEnhancementVM {
    func observerPropertys() {
        smooth.subscribe(onNext: { [unowned self] (value) in
            self.enhancement.setFilterValue(value, with: .smooth)
        }).disposed(by: bag)
        
        brighten.subscribe(onNext: { [unowned self] (value) in
            self.enhancement.setFilterValue(value, with: .brighten)
        }).disposed(by: bag)
        
        thinning.subscribe(onNext: { [unowned self] (value) in
            self.enhancement.setFilterValue(value, with: .thinning)
        }).disposed(by: bag)
        
        eyeEnlarge.subscribe(onNext: { [unowned self] (value) in
            self.enhancement.setFilterValue(value, with: .eye)
        }).disposed(by: bag)
    }
}
