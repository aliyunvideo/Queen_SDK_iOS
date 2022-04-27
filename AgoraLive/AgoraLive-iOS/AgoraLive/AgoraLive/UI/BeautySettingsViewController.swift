//
//  BeautySettingsViewController.swift
//  AgoraLive
//
//  Created by CavanSu on 2020/3/12.
//  Copyright © 2020 Agora. All rights reserved.
//

import UIKit
import RxSwift

class BeautySettingsViewController: UITableViewController {
    @IBOutlet weak var titleLabel: UILabel!
    @IBOutlet weak var workSwitch: UISwitch!
    
    @IBOutlet weak var nameLabel1: UILabel!
    @IBOutlet weak var nameLabel2: UILabel!
    @IBOutlet weak var nameLabel3: UILabel!
    @IBOutlet weak var nameLabel4: UILabel!
    
    @IBOutlet weak var valueLabel1: UILabel!
    @IBOutlet weak var valueLabel2: UILabel!
    @IBOutlet weak var valueLabel3: UILabel!
    @IBOutlet weak var valueLabel4: UILabel!
    
    @IBOutlet weak var slider1: UISlider!
    @IBOutlet weak var slider2: UISlider!
    @IBOutlet weak var slider3: UISlider!
    @IBOutlet weak var slider4: UISlider!
    
    private let bag = DisposeBag()
    let enhanceVM = VideoEnhancementVM()
    
    override func viewDidLoad() {
        super.viewDidLoad()
        
        let color = UIColor(hexString: "#D8D8D8")
        let x: CGFloat = 15.0
        let width = UIScreen.main.bounds.width - (x * 2)
        titleLabel.containUnderline(color,
                                         x: x,
                                         width: width)
        
        titleLabel.text = NSLocalizedString("Beauty")
        
        nameLabel1.text = NSLocalizedString("BlurLevel")
        nameLabel2.text = NSLocalizedString("ColorLevel")
        nameLabel3.text = NSLocalizedString("CheekThining")
        nameLabel4.text = NSLocalizedString("EyeEnlarging")
        
        nameLabel1.adjustsFontSizeToFitWidth = true
        nameLabel2.adjustsFontSizeToFitWidth = true
        nameLabel3.adjustsFontSizeToFitWidth = true
        nameLabel4.adjustsFontSizeToFitWidth = true
        
        // Beauty Switch
        enhanceVM.beauty.subscribe(onNext: { [unowned self] (work) in
            self.workSwitch.isOn = work.boolValue
            self.slider1.isEnabled = work.boolValue
            self.slider2.isEnabled = work.boolValue
            self.slider3.isEnabled = work.boolValue
            self.slider4.isEnabled = work.boolValue
        }).disposed(by: bag)
        
        workSwitch.rx.value.subscribe(onNext: { [unowned self] (value) in
            self.enhanceVM.beauty(value ? .on : .off)
        }).disposed(by: bag)
        
        // Smooth
        slider1.minimumValue = enhanceVM.minSmooth
        slider1.maximumValue = enhanceVM.maxSmooth
        
        enhanceVM.smooth.subscribe(onNext: { (value) in
            self.slider1.value = value
            self.valueLabel1.text = String(format: "%0.1f", value)
        }).disposed(by: bag)
        
        self.slider1.rx.value.subscribe(onNext: { [unowned self] (value) in
            self.enhanceVM.smooth.accept(value)
        }).disposed(by: bag)
        
        // Brighten
        slider2.minimumValue = enhanceVM.minBrighten
        slider2.maximumValue = enhanceVM.maxBrighten
        
        enhanceVM.brighten.subscribe(onNext: { [unowned self] (value) in
            self.slider2.value = value
            self.valueLabel2.text = String(format: "%0.1f", value)
        }).disposed(by: bag)
        
        slider2.rx.value.subscribe(onNext: { [unowned self] (value) in
            self.enhanceVM.brighten.accept(value)
        }).disposed(by: bag)
        
        // Face Thinning
        slider3.minimumValue = enhanceVM.minThinning
        slider3.maximumValue = enhanceVM.maxThinning
        
        enhanceVM.thinning.subscribe(onNext: { [unowned self] (value) in
            self.slider3.value = value
            self.valueLabel3.text = String(format: "%0.1f", value)
        }).disposed(by: bag)

        slider3.rx.value.subscribe(onNext: { [unowned self] (value) in
            self.enhanceVM.thinning.accept(value)
        }).disposed(by: bag)
        
        // Eye enlarge
        slider4.minimumValue = enhanceVM.minEyeEnlarge
        slider4.maximumValue = enhanceVM.maxEyeEnlarge
        
        enhanceVM.eyeEnlarge.subscribe(onNext: { [unowned self] (value) in
            self.slider4.value = value
            self.valueLabel4.text = String(format: "%0.1f", value)
        }).disposed(by: bag)

        self.slider4.rx.value.subscribe(onNext: { [unowned self] (value) in
            self.enhanceVM.eyeEnlarge.accept(value)
        }).disposed(by: bag)
    }
}
