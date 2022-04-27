//
//  VirtualAppearanceViewController.swift
//  AgoraLive
//
//  Created by CavanSu on 2020/5/27.
//  Copyright © 2020 Agora. All rights reserved.
//

import UIKit
import RxSwift
import RxRelay

class VirtualAppearanceViewController: UIViewController, RxViewController, ShowAlertProtocol {
    @IBOutlet weak var titleLabel: UILabel!
    @IBOutlet weak var bigImageView: UIImageView!
    @IBOutlet weak var leftButton: UIButton!
    @IBOutlet weak var rightButton: UIButton!
    @IBOutlet weak var confirmButton: UIButton!
    @IBOutlet weak var closeButton: UIButton!
    
    private var selectedAppearance = BehaviorRelay(value: VirtualAppearance.girl)
    private let enhancementVM = VideoEnhancementVM()
    private var virtualAppearanceSubscribe: Disposable?
    
    var presentingAlert: UIAlertController?
    var bag = DisposeBag()
    
    deinit {
        #if !RELEASE
        print("deinit VirtualAppearanceViewController")
        #endif
    }
    
    override func viewDidLoad() {
        super.viewDidLoad()
        
        titleLabel.text = NSLocalizedString("Virtual_Appearance_Select")
        confirmButton.setTitle(NSLocalizedString("Confirm"), for: .normal)
        confirmButton.setTitleColor(.white, for: .normal)
        
        leftButton.imageView?.contentMode = .scaleAspectFit
        rightButton.imageView?.contentMode = .scaleAspectFit
        
        leftButton.rx.tap.subscribe(onNext: { [unowned self] in
            self.selectedAppearance.accept(.dog)
        }).disposed(by: bag)
        
        rightButton.rx.tap.subscribe(onNext: { [unowned self] in
            self.selectedAppearance.accept(.girl)
        }).disposed(by: bag)
        
        confirmButton.rx.tap.subscribe(onNext: { [unowned self] in
            self.enhancementVM.virtualAppearance(self.selectedAppearance.value,
                                                 success: { [unowned self] in
                                                    self.push()
            }) { [unowned self] in
                self.showAlert(message: "Load Animoji fail") { [unowned self] (_) in
                    self.close()
                }
            }
        }).disposed(by: bag)
        
        closeButton.rx.tap.subscribe(onNext: { [unowned self] in
            self.close()
        }).disposed(by: bag)
        
        selectedAppearance.subscribe(onNext: { (appearance) in
            switch appearance {
            case .girl:
                self.rightButton.isDeselected = false
                self.leftButton.isDeselected = true
                self.bigImageView.image = appearance.image
            case .dog:
                self.rightButton.isDeselected = true
                self.leftButton.isDeselected = false
                self.bigImageView.image = appearance.image
            case .none:
                assert(false)
                break
            }
        }).disposed(by: bag)
    }
}

private extension VirtualAppearanceViewController {
    func push() {
        if let navigation = self.navigationController {
            let vc = UIStoryboard.initViewController(of: "CreateLiveViewController",
                                                     class: CreateLiveViewController.self)
            vc.liveType = .virtual
            navigation.pushViewController(vc, animated: true)
        } else {
            self.dismiss(animated: true, completion: nil)
        }
    }
    
    func close() {
        virtualAppearanceSubscribe?.dispose()
        enhancementVM.reset()
        if let navigation = self.navigationController {
            navigation.dismiss(animated: true, completion: nil)
        } else {
            self.dismiss(animated: true, completion: nil)
        }
    }
}

fileprivate extension UIButton {
    var isDeselected: Bool {
        set {
            layer.borderColor = newValue ? UIColor.clear.cgColor : UIColor(hexString: "#008AF3").cgColor
            layer.borderWidth = newValue ? 0 : 1
            
            layer.shadowOpacity = newValue ? 0 : 0.3
            layer.shadowOffset = CGSize(width: 0, height: 0.5)
        }
        get {
            assert(false)
            return true
        }
    }
}
