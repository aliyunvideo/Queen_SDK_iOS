//
//  GoodsListViewController.swift
//  AgoraLive
//
//  Created by CavanSu on 2020/7/29.
//  Copyright © 2020 Agora. All rights reserved.
//

import UIKit
import RxSwift
import RxRelay

protocol GoodsCellDelegate: NSObjectProtocol {
    func cell(_ cell: GoodsCell, didTapButton: UIButton, on index: Int, for event: GoodsCell.ButtonType)
}

class GoodsCell: UITableViewCell {
    enum ButtonType {
        case onShelf, offShelf, detail
    }
    
    @IBOutlet weak var goodsImageView: UIImageView!
    @IBOutlet weak var descriptionLabel: UILabel!
    @IBOutlet weak var priceLabel: UILabel!
    @IBOutlet weak var button: UIButton!
    @IBOutlet weak var filletView: FilletView!
    
    private let bag = DisposeBag()
    
    weak var delegate: GoodsCellDelegate?
    
    var buttonType: ButtonType = .onShelf {
        didSet {
            switch buttonType {
            case .onShelf:
                button.setTitle(NSLocalizedString("Product_Launch"), for: .normal)
            case .offShelf:
                button.setTitle(NSLocalizedString("Product_Off_Shelf"), for: .normal)
            case .detail:
                button.setTitle(NSLocalizedString("Product_Go"), for: .normal)
            }
        }
    }
    
    var index: Int = 0
    
    override func awakeFromNib() {
        super.awakeFromNib()
        filletView.insideBackgroundColor = .white
        filletView.filletRadius = 4
        
        button.layer.borderWidth = 1
        button.cornerRadius(20)
        button.layer.borderColor = UIColor(hexString: "#EEEEEE").cgColor
        
        button.rx.tap.subscribe(onNext: { [unowned self] in
            self.delegate?.cell(self, didTapButton: self.button, on: self.index, for: self.buttonType)
        }).disposed(by: bag)
    }
}

class GoodsListViewController: UIViewController, RxViewController {
    @IBOutlet weak var titleLabel: UILabel!
    @IBOutlet weak var tabView: TabSelectView!
    @IBOutlet weak var tableView: UITableView!
    @IBOutlet weak var tableViewBottom: NSLayoutConstraint!
    @IBOutlet weak var tableViewTop: NSLayoutConstraint!
    
    private var onSelfSubscribe: Disposable?
    private var offSelfSubscribe: Disposable?
    
    var perspective: LiveRoleType = .owner
    var bag = DisposeBag()
    var vm: GoodsVM!
    
    let itemOnShelf = PublishRelay<GoodsItem>()
    let itemOffShelf = PublishRelay<GoodsItem>()
    let itemDetail = PublishRelay<GoodsItem>()
    
    override func viewDidLoad() {
        super.viewDidLoad()
        views()
        vm.refetchList()
    }
}

private extension GoodsListViewController {
    func views() {
        titleLabel.text = NSLocalizedString("Product_List")
        
        if perspective == .owner {
            ownerList()
        } else {
            audienceList()
        }
        
        tableViewBottom.constant = UIScreen.main.heightOfSafeAreaBottom
    }
    
    func ownerList() {
        let goodsTitles = [NSLocalizedString("Product_Pending"),
                            NSLocalizedString("Product_OnShelf")]
        
        tabView.alignment = .center
        tabView.underlineHeight = 2
        tabView.selectedTitle = TabSelectView.TitleProperty(color: UIColor(hexString: "#333333"),
                                                            font: UIFont.systemFont(ofSize: 14, weight: .medium))
        tabView.unselectedTitle = TabSelectView.TitleProperty(color: UIColor(hexString: "#666666"),
                                                              font: UIFont.systemFont(ofSize: 14))
        tabView.titleSpace = 86
        tabView.update(goodsTitles)
                
        tabView.selectedIndex.subscribe(onNext: { [unowned self] (index) in
            switch index {
            case 0:
                if let subscribe = self.onSelfSubscribe {
                    subscribe.dispose()
                }
                
                self.offSelfSubscribe = self.tableViewBindWithList(self.vm.offShelfList)
                self.offSelfSubscribe?.disposed(by: self.bag)
            case 1:
                if let subscribe = self.offSelfSubscribe {
                    subscribe.dispose()
                }
                
                self.onSelfSubscribe = self.tableViewBindWithList(self.vm.onShelfList)
                self.onSelfSubscribe?.disposed(by: self.bag)
            default:
                break
            }
        }).disposed(by: bag)
    }
    
    func audienceList() {
        tabView.isHidden = true
        tableViewTop.constant = 0
        tableViewBindWithList(vm.onShelfList).disposed(by: bag)
    }
}

private extension GoodsListViewController {
    func tableViewBindWithList(_ list: BehaviorRelay<[GoodsItem]>) -> Disposable {
        let subscribe = list.bind(to: self.tableView.rx.items(cellIdentifier: "GoodsCell",
                                                              cellType: GoodsCell.self)) { [unowned self] (index, goods, cell) in
                                                                cell.goodsImageView.image = goods.image
                                                                cell.descriptionLabel.text = goods.name
                                                                cell.priceLabel.text = "\(goods.price)"
                                                                cell.index = index
                                                                cell.delegate = self
                                                                
                                                                if self.perspective == .owner {
                                                                    cell.buttonType = goods.isSale ? .offShelf : .onShelf
                                                                } else {
                                                                    cell.buttonType = .detail
                                                                }
        }
        
        return subscribe
    }
}

extension GoodsListViewController: GoodsCellDelegate {
    func cell(_ cell: GoodsCell, didTapButton: UIButton, on index: Int, for event: GoodsCell.ButtonType) {
        switch event {
        case .onShelf:
            let item = vm.offShelfList.value[index]
            itemOnShelf.accept(item)
        case .offShelf:
            let item = vm.onShelfList.value[index]
            itemOffShelf.accept(item)
        case .detail:
            let item = vm.onShelfList.value[index]
            itemDetail.accept(item)
        }
    }
}
