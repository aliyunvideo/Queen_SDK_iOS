//
//  GoodsVM.swift
//  AgoraLive
//
//  Created by CavanSu on 2020/7/28.
//  Copyright © 2020 Agora. All rights reserved.
//

import UIKit
import RxSwift
import RxRelay
import AlamoClient

struct GoodsItem {
    var name: String
    var image: UIImage
    var bigImage: UIImage
    var description: String
    var price: Float
    var id: String
    var isSale: Bool
    
    init(dic: StringAnyDic) throws {
        self.id = try dic.getStringValue(of: "productId")
        self.image = UIImage(named: "pic-商品0\(self.id)")!
        self.bigImage = UIImage(named: "pic-商品B0\(self.id)-slices")!
        self.price = try dic.getFloatInfoValue(of: "price")
        self.isSale = (try dic.getBoolInfoValue(of: "state"))
        
        let name = try dic.getStringValue(of: "productName")
        self.name = NSLocalizedString(name)
        self.description = NSLocalizedString(name + "_Description")
    }
}

class GoodsVM: RTMObserver {
    private var room: Room
    
    let list = BehaviorRelay(value: [GoodsItem]())
    let onShelfList = BehaviorRelay(value: [GoodsItem]())
    let offShelfList = BehaviorRelay(value: [GoodsItem]())
    
    let itemOnShelf = PublishRelay<GoodsItem>()
    let itemOffShelf = PublishRelay<GoodsItem>()
    
    let requestSuccess = PublishRelay<String>()
    let requestError = PublishRelay<String>()
    
    init(room: Room) {
        self.room = room
        super.init()
        observe()
    }
    
    deinit {
        #if !RELEASE
        print("deinit GoodsVM")
        #endif
    }
        
    func itemOnShelf(_ item: GoodsItem) {
        goods(item, onShelf: true, of: room.roomId)
    }
    
    func itemOffShelf(_ item: GoodsItem) {
        goods(item, onShelf: false, of: room.roomId)
    }
    
    func refetchList() {
        let client = ALCenter.shared().centerProvideRequestHelper()
        let task = RequestTask(event: RequestEvent(name: "goods-list"),
                               type: .http(.get, url: URLGroup.goodsList(roomId: room.roomId)),
                               timeout: .medium,
                               header: ["token": ALKeys.ALUserToken])
        
        client.request(task: task, success: ACResponse.json({ [unowned self] (json) in
            let data = try json.getListValue(of: "data")
            let list = try [GoodsItem](dicList: data)
            self.list.accept(list)
        })) { [unowned self] (error) -> RetryOptions in
            self.requestError.accept("fetch product list fail")
            return .resign
        }
    }
    
    func purchase(item: GoodsItem, count: Int = 1) {
        let client = ALCenter.shared().centerProvideRequestHelper()
        let task = RequestTask(event: RequestEvent(name: "goods-purchase"),
                               type: .http(.post, url: URLGroup.goodsPurchase(roomId: room.roomId)),
                               timeout: .medium,
                               header: ["token": ALKeys.ALUserToken],
                               parameters: ["productId": item.id, "count": count])
        
        client.request(task: task, success: ACResponse.blank({ [unowned self] in
            self.requestSuccess.accept(NSLocalizedString("Product_Purchased_Successfully"))
        })) { [unowned self] (_) -> RetryOptions in
            self.requestError.accept("purchase product fail")
            return .resign
        }
    }
}
private extension GoodsVM {
    func goods(_ item: GoodsItem, onShelf: Bool, of roomId: String) {
        let client = ALCenter.shared().centerProvideRequestHelper()
        var name: String
        if onShelf {
            name = "goods-on-shelf"
        } else {
            name = "goods-off-shelf"
        }
        
        let task = RequestTask(event: RequestEvent(name: name),
                               type: .http(.post, url: URLGroup.goodsOnShelf(roomId: roomId, state: onShelf ? 1 : 0, goodsId: item.id)),
                               timeout: .medium,
                               header: ["token": ALKeys.ALUserToken])
        
        client.request(task: task, success: ACResponse.blank({ [weak self] in
            guard let strongSelf = self else {
                return
            }
            
            var message: String
            if onShelf {
                if DeviceAssistant.Language.isChinese {
                    message = "\"\(item.name)\" 已经上架"
                } else {
                    message = "\(item.name) has been added on shelf"
                }
            } else {
                if DeviceAssistant.Language.isChinese {
                    message = "\"\(item.name)\" 已经下架"
                } else {
                    message = "\(item.name) is no longer available"
                }
            }
            strongSelf.requestSuccess.accept(message)
        })) { [weak self] (_) -> RetryOptions in
            guard let strongSelf = self else {
                return .resign
            }
            
            var message: String
            if onShelf {
                if DeviceAssistant.Language.isChinese {
                    message = "\"\(item.name)\" 上架失败"
                } else {
                    message = "add \(item.name) on shelf fail"
                }
            } else {
                if DeviceAssistant.Language.isChinese {
                    message = "\"\(item.name)\" 下架失败"
                } else {
                    message = "put \(item.name) off shelf fail"
                }
            }
            strongSelf.requestError.accept(message)
            return .resign
        }
    }
    
    func observe() {
        let rtm = ALCenter.shared().centerProvideRTMHelper()
        
        rtm.addReceivedChannelMessage(observer: self.address) { [weak self] (json) in
            guard let strongSelf = self else {
                return
            }
            
            guard let command = try? json.getEnum(of: "cmd", type: ALChannelMessage.AType.self),
                command == .goodsOnShelf else {
                return
            }
            
            let data = try json.getDataObject()
            let productId = try data.getStringValue(of: "productId")
            let state = try data.getIntValue(of: "state")
            let new = strongSelf.list.value
            
            let item = new.first(where: { (item) -> Bool in
                return item.id == productId
            })
            
            guard var tItem = item else {
                return
            }
            
            if state == 1 {
                tItem.isSale = true
                strongSelf.itemOnShelf.accept(tItem)
            } else {
                tItem.isSale = false
                strongSelf.itemOnShelf.accept(tItem)
            }
        }
        
        itemOnShelf.subscribe(onNext: { [unowned self] (item: GoodsItem) in
            var new = self.list.value
            let index = new.firstIndex { (goods) -> Bool in
                return goods.id == item.id
            }
            
            guard let tIndex = index else {
                return
            }
            new.remove(at: tIndex)
            new.insert(item, at: 0)
            self.list.accept(new)
        }).disposed(by: bag)
        
        itemOffShelf.subscribe(onNext: { [unowned self] (item: GoodsItem) in
            var new = self.list.value
            let index = new.firstIndex { (goods) -> Bool in
                return goods.id == item.id
            }
            
            guard let tIndex = index else {
                return
            }
            new.remove(at: tIndex)
            new.append(item)
            self.list.accept(new)
        }).disposed(by: bag)
        
        list.subscribe(onNext: { [unowned self] (list) in
            let onShelf = list.filter { (item) -> Bool in
                return item.isSale
            }

            self.onShelfList.accept(onShelf)

            let offShelf = list.filter { (item) -> Bool in
                return !item.isSale
            }

            self.offShelfList.accept(offShelf)
        }).disposed(by: bag)
    }
}

fileprivate extension Array where Element == GoodsItem {
    init(dicList: [StringAnyDic]) throws {
        var array = [GoodsItem]()
        for item in dicList {
            let user = try GoodsItem(dic: item)
            array.append(user)
        }
        self = array
    }
}
