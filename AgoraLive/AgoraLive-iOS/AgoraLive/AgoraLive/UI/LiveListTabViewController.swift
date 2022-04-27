//
//  LiveListTabViewController.swift
//  AgoraLive
//
//  Created by CavanSu on 2020/2/19.
//  Copyright © 2020 Agora. All rights reserved.
//

import UIKit
import RxSwift
import RxRelay
import MJRefresh
import MBProgressHUD
import AgoraRtcKit

class LiveListTabViewController: MaskViewController {
    @IBOutlet weak var tabView: TabSelectView!
    @IBOutlet weak var createButton: UIButton!
    
    private let listVM = LiveListVM()
    private let bag = DisposeBag()
    private let monitor = NetworkMonitor(host: "www.apple.com")
    private var listVC: LiveListViewController?
    private var timer: Timer?
    
    override func viewWillAppear(_ animated: Bool) {
        super.viewWillAppear(animated)
        roomListRefresh(false)
        perMinuterRefresh()
    }
    
    override func viewWillDisappear(_ animated: Bool) {
        super.viewWillDisappear(animated)
        cancelScheduelRefresh()
    }
    
    override func viewDidLoad() {
        super.viewDidLoad()
        updateViews()
        
        // TabSelectView
        updateTabSelectView()
        // LiveListViewController
        updateLiveListVC()
        
        netMonitor()
    }
    
    override func prepare(for segue: UIStoryboardSegue, sender: Any?) {
        guard let segueId = segue.identifier else {
            return
        }
        
        switch segueId {
        case "LiveListViewController":
            listVC = segue.destination as? LiveListViewController
        case "CreateLiveNavigation":
            guard let type = sender as? LiveType,
                let navi = segue.destination as? UINavigationController,
                let vc = navi.viewControllers.first as? CreateLiveViewController else {
                    assert(false)
                    return
            }
            
            vc.liveType = type
        default:
            break
        }
        
        guard let sender = sender,
            let info = sender as? LiveSession.JoinedInfo,
            let vc = segue.destination as? LiveViewController else {
            return
        }
        
        vc.userListVM = LiveUserListVM(room: info.room)
        vc.userListVM.updateGiftListWithJson(list: info.giftAudience)
        
        vc.giftVM = GiftVM(room: info.room)
        
        switch segueId {
        case "MultiBroadcastersViewController":
            guard let seatInfo = info.seatInfo,
                let vm = try? LiveSeatVM(room: info.room, list: seatInfo) else {
                    assert(false)
                    return
            }
            
            let vc = segue.destination as? MultiBroadcastersViewController
            vc?.multiHostsVM = MultiHostsVM(room: info.room)
            vc?.seatVM = vm
        case "PKBroadcastersViewController":
            guard let pkInfo = info.pkInfo,
                let vm = try? PKVM(room: info.room, type: .pk, state: pkInfo) else {
                    assert(false)
                    return
            }
            
            let vc = segue.destination as? PKBroadcastersViewController
            vc?.pkVM = vm
        case "VirtualBroadcastersViewController":
            guard let seatInfo = info.seatInfo,
                let seatVM = try? LiveSeatVM(room: info.room, list: seatInfo),
                let session = ALCenter.shared().liveSession,
                let vc = segue.destination as? VirtualBroadcastersViewController else {
                    assert(false)
                    return
            }
            
            vc.multiHostsVM = MultiHostsVM(room: info.room)
            vc.seatVM = seatVM
            
            var hostCount: HostCount
            
            if seatVM.list.value.count == 1,
                let remote = seatVM.list.value[0].user {
                hostCount = .multi([session.owner.value.user, remote])
            } else {
                hostCount = .single(session.owner.value.user)
            }
            vc.hostCount = BehaviorRelay(value: hostCount)
            
            vc.enhancementVM.localVirtualAppearance(fail: { [unowned self, unowned vc] in
                vc.leave()
                self.showTextToast(text: "Load Animoji fail")
            })
        case "LiveShoppingViewController":
            guard let seatInfo = info.seatInfo,
                let seatVM = try? LiveSeatVM(room: info.room, list: seatInfo),
                let pkInfo = info.pkInfo,
                let pkVM = try? PKVM(room: info.room, type: .shopping, state: pkInfo),
                let session = ALCenter.shared().liveSession else {
                    assert(false)
                    return
            }
            
            let vc = segue.destination as? LiveShoppingViewController
            vc?.userListVM.updateGiftListWithJson(list: info.giftAudience)
            vc?.multiHostsVM = MultiHostsVM(room: info.room)
            vc?.goodsVM = GoodsVM(room: info.room)
            vc?.seatVM = seatVM
            vc?.pkVM = pkVM
            
            var hostCount: HostCount
            
            if seatVM.list.value.count == 1,
                let remote = seatVM.list.value[0].user {
                hostCount = .multi([session.owner.value.user, remote])
            } else {
                hostCount = .single(session.owner.value.user)
            }
            vc?.hostCount = BehaviorRelay(value: hostCount)
        default:
            break
        }
    }
}

private extension LiveListTabViewController {
    func updateViews() {
        createButton.layer.shadowOpacity = 0.3
        createButton.layer.shadowOffset = CGSize(width: 0, height: 3)
        createButton.layer.shadowColor = UIColor(hexString: "#BD3070").cgColor
        
        createButton.rx.tap.subscribe(onNext: { [unowned self] in
            if self.listVM.presentingType != .virtual {
                self.performSegue(withIdentifier: "CreateLiveNavigation", sender: self.listVM.presentingType)
            } else {
                self.performSegue(withIdentifier: "VirtualCreatNavigation", sender: nil)
            }
        }).disposed(by: bag)
    }
    
    func updateTabSelectView() {
        tabView.underlineHeight = 3
        tabView.titleSpace = 28
        
        let titles = LiveType.list.map { (item) -> String in
            return item.description
        }
        
        tabView.update(titles)
        
        tabView.selectedIndex.subscribe(onNext: { [unowned self] (index) in
            var type: LiveType
            
            switch index {
            case 0: type = .multi
            case 1: type = .single
            case 2: type = .pk
            case 3: type = .virtual
            case 4: type = .shopping
            default: fatalError()
            }
            
            self.listVM.presentingType = type
            
            self.roomListRefresh(false)
        }).disposed(by: bag)
    }
    
    func updateLiveListVC() {
        guard let vc = listVC else {
            assert(false)
            return
        }
        
        // placeHolderView tap
        vc.placeHolderView.tap.subscribe(onNext: { [unowned self] (_) in
            self.roomListRefresh(true)
        }).disposed(by: bag)
        
        // placeHolderView if need hidden
        listVM.presentingList.subscribe(onNext: { [unowned vc] (list) in
            vc.collectionView.isHidden = list.count == 0 ? true : false
        }).disposed(by: bag)
        
        // Cell Reload
        listVM.presentingList
            .bind(to: vc.collectionView.rx.items(cellIdentifier: "RoomCell",
                                                 cellType: RoomCell.self)) { index, item, cell in
                                                    cell.briefView.label.text = item.name
                                                    cell.personCountView.label.text = "\(item.personCount)"
                                                    cell.briefView.imageView.image = ALCenter.shared().centerProvideImagesHelper().getRoom(index: item.imageIndex)
        }.disposed(by: bag)
        
        // MJRefresh
        vc.collectionView.mj_header = MJRefreshNormalHeader(refreshingBlock: { [unowned self, unowned vc] in
            self.listVM.refetch(success: { [unowned vc] in
                vc.collectionView.mj_header?.endRefreshing()
            }) { [unowned vc] in // fail
                vc.collectionView.mj_header?.endRefreshing()
            }
        })
        
        vc.collectionView.mj_footer = MJRefreshBackFooter(refreshingBlock: { [unowned self, unowned vc] in
            self.listVM.fetch(success: { [unowned vc] in
                vc.collectionView.mj_footer?.endRefreshing()
            }) { [unowned vc] in // fail
                vc.collectionView.mj_footer?.endRefreshing()
            }
        })
        
        vc.collectionView.rx.modelSelected(Room.self).subscribe(onNext: { [unowned self] (room) in
            let type = self.listVM.presentingType
            
            var media = VideoConfiguration()
            
            switch type {
            case .multi:
                media.resolution = AgoraVideoDimension240x240
                media.frameRate = .fps15
                media.bitRate = 200
            case .single:
                media.resolution = CGSize.AgoraVideoDimension360x640
                media.frameRate = .fps15
                media.bitRate = 600
            case .pk:
                media.resolution = CGSize.AgoraVideoDimension360x640
                media.frameRate = .fps15
                media.bitRate = 800
            case .virtual:
                media.resolution = CGSize.AgoraVideoDimension720x1280
                media.frameRate = .fps15
                media.bitRate = 1000
            case .shopping:
                media.resolution = CGSize.AgoraVideoDimension360x640
                media.frameRate = .fps15
                media.bitRate = 600
            }
            
            let local = ALCenter.shared().centerProvideLocalUser()
            let role = LiveLocalUser(type: .audience,
                                     info: local.info.value,
                                     permission: [],
                                     agUId: 0)
            let session = LiveSession(room: room,
                                      videoConfiguration: media,
                                      type: type,
                                      owner: LiveSession.Owner.otherUser(room.owner),
                                      role: role)
            self.joinLiving(session: session)
        }).disposed(by: bag)
        
        vc.collectionView.rx.willBeginDragging.subscribe(onNext: { [unowned self] in
            self.cancelScheduelRefresh()
        }).disposed(by: bag)
        
        vc.collectionView.rx.didEndDragging.subscribe(onNext: { [unowned self] (done) in
            if done {
                self.perMinuterRefresh()
            }
        }).disposed(by: bag)
    }
        
    func netMonitor() {
        monitor.action(.on)
        monitor.connect.subscribe(onNext: { [unowned self] (status) in
            switch status {
            case .notReachable: self.listVC?.placeHolderView.viewType = .lostConnection
            case .reachable:    self.listVC?.placeHolderView.viewType = .noRoom
            default: break
            }
        }).disposed(by: bag)
    }
    
    func perMinuterRefresh() {
        guard timer == nil else {
            return
        }
        
        timer = Timer(fireAt: Date(timeIntervalSinceNow: 60.0),
                      interval: 60.0,
                      target: self,
                      selector: #selector(roomListRefresh),
                      userInfo: nil,
                      repeats: true)
        RunLoop.main.add(timer!, forMode: .common)
        timer?.fire()
    }
    
    @objc func roomListRefresh(_ hasHUD: Bool = false) {
        guard !self.isShowingHUD() else {
            return
        }
        
        if let isRefreshing = self.listVC?.collectionView.mj_header?.isRefreshing,
            isRefreshing {
            return
        }
        
        let end: Completion = { [unowned self] in
            if hasHUD {
                self.hiddenHUD()
            }
        }

        if hasHUD {
            self.showHUD()
        }
        
        listVM.refetch(success: end, fail: end)
    }
    
    func cancelScheduelRefresh() {
        timer?.invalidate()
        timer = nil
    }
}

extension LiveListTabViewController {
    func joinLiving(session: LiveSession) {
        self.showHUD()
        
        let center = ALCenter.shared()
        center.liveSession = session
        session.join(success: { [unowned session, unowned self] (info: LiveSession.JoinedInfo) in
            self.hiddenHUD()
            
            switch session.type {
            case .multi:
                self.performSegue(withIdentifier: "MultiBroadcastersViewController", sender: info)
            case .single:
                self.performSegue(withIdentifier: "SingleBroadcasterViewController", sender: info)
            case .pk:
                self.performSegue(withIdentifier: "PKBroadcastersViewController", sender: info)
            case .virtual:
                self.performSegue(withIdentifier: "VirtualBroadcastersViewController", sender: info)
            case .shopping:
                self.performSegue(withIdentifier: "LiveShoppingViewController", sender: info)
            }
        }) { [unowned self] in
            self.hiddenHUD()
            self.showAlert(message:"join live fail")
            self.roomListRefresh()
        }
    }
}
