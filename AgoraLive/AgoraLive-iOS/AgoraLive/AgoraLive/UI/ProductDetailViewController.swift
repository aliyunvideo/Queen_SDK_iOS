//
//  ProductDetailViewController.swift
//  AgoraLive
//
//  Created by CavanSu on 2020/8/8.
//  Copyright © 2020 Agora. All rights reserved.
//

import UIKit
import RxSwift
import RxRelay

class ProductDetailViewController: UIViewController, ShowAlertProtocol {
    @IBOutlet weak var imageView: UIImageView!
    @IBOutlet weak var label: UILabel!
    @IBOutlet weak var purchaseButton: UIButton!
    @IBOutlet weak var backButton: UIButton!
    @IBOutlet weak var bottomViewHeight: NSLayoutConstraint!
    
    private let bag = DisposeBag()
    let purchase = PublishRelay<GoodsItem>()
    let close = PublishRelay<()>()
    var owner: LiveRole!
    var playerVM: PlayerVM!
    var product: GoodsItem!
    
    override func viewDidLoad() {
        super.viewDidLoad()
        imageView.image = product.image
        label.text = product.description
        
        purchaseButton.setTitle(NSLocalizedString("Buy_Now"),
                                for: .normal)
        
        purchaseButton.rx.tap.subscribe(onNext: { [unowned self] in
            self.showAlert(message: NSLocalizedString("Buy_This_Product"),
                           action1: NSLocalizedString("Cancel"),
                           action2: NSLocalizedString("Confirm")) { [unowned self] (_) in
                            self.purchase.accept(self.product)
            }
        }).disposed(by: bag)
        
        backButton.rx.tap.subscribe(onNext: { [unowned self] in
            self.close.accept(())
            self.navigationController?.popViewController(animated: true)
        }).disposed(by: bag)
        
        let width: CGFloat = 140
        let height: CGFloat = 180
        let x: CGFloat = self.view.bounds.width - width
        let y: CGFloat = self.view.bounds.height - bottomViewHeight.constant - height - UIScreen.main.heightOfSafeAreaBottom - 15
        let frame = CGRect(x: x, y: y, width: width, height: height)
        let view = ShoppingSmallRenderView(frame: frame)
        view.nameLabel.text = owner.info.name
        view.isCanMove = true
        self.view.addSubview(view)
        
        view.closeButton.rx.tap.subscribe(onNext: { [unowned view] in
            view.removeFromSuperview()
        }).disposed(by: bag)
        
        self.playerVM.startRenderVideoStreamOf(user: owner,
                                               on: view.renderView)
    }
}
