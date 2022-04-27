//
//  LiveShoppingViewController.swift
//  AgoraLive
//
//  Created by CavanSu on 2020/7/22.
//  Copyright © 2020 Agora. All rights reserved.
//

import UIKit
import MJRefresh
import RxSwift
import RxRelay

class LiveShoppingViewController: MaskViewController, LiveViewController {
    @IBOutlet weak var ownerView: IconTextView!
    @IBOutlet weak var pkContainerView: UIView!
    @IBOutlet weak var renderView: UIView!
    @IBOutlet weak var chatViewHeight: NSLayoutConstraint!
    
    private var popover = Popover(options: [.type(.up),
                                            .blackOverlayColor(UIColor.clear),
                                            .cornerRadius(5.0),
                                            .arrowSize(CGSize(width: 8, height: 4))])
    private var popoverContent = UILabel(frame: CGRect.zero)
    
    private lazy var broadcasteRender: ShoppingSmallRenderView = {
        let width: CGFloat = 140
        let height: CGFloat = 180
        let x: CGFloat = self.view.bounds.width - width
        let y: CGFloat = self.view.bounds.height - self.bottomToolsVC!.view.bounds.height - height - UIScreen.main.heightOfSafeAreaBottom - 15
        let frame = CGRect(x: x, y: y, width: width, height: height)
        let view = ShoppingSmallRenderView(frame: frame)
        
        guard let session = ALCenter.shared().liveSession else {
            assert(false)
            return view
        }
        
        
        return view
    }()
    
    private weak var pkView: PKViewController?
    
    private var roomListVM = LiveListVM()
    private var broadcasterSubscribe: Disposable?
    var hostCount: BehaviorRelay<HostCount>!
    var goodsVM: GoodsVM!
    var multiHostsVM: MultiHostsVM!
    var seatVM: LiveSeatVM!
    var pkVM: PKVM!
    var applicationSubscribe: Disposable?
    
    // LiveViewController
    var tintColor = UIColor(red: 0,
                            green: 0,
                            blue: 0,
                            alpha: 0.4)
    
    var bag: DisposeBag = DisposeBag()
    
    // ViewController
    var giftAudienceVC: GiftAudienceViewController?
    var bottomToolsVC: BottomToolsViewController?
    var chatVC: ChatViewController?
    
    // View
    @IBOutlet weak var personCountView: RemindIconTextView!
    
    internal lazy var chatInputView: ChatInputView = {
        let chatHeight: CGFloat = 50.0
        let frame = CGRect(x: 0,
                           y: UIScreen.main.bounds.height,
                           width: UIScreen.main.bounds.width,
                           height: chatHeight)
        let view = ChatInputView(frame: frame)
        view.isHidden = true
        return view
    }()
    
    // ViewModel
    var userListVM: LiveUserListVM!
    var giftVM: GiftVM!
    var musicVM = MusicVM()
    var chatVM = ChatVM()
    var deviceVM = MediaDeviceVM()
    var playerVM = PlayerVM()
    var enhancementVM = VideoEnhancementVM()
    var monitor = NetworkMonitor(host: "www.apple.com")
    
    deinit {
        #if !RELEASE
        print("deinit LiveShoppingViewController")
        #endif
    }
    
    override func viewDidLoad() {
        super.viewDidLoad()
        let image = UIImage(named: "live-bg")
        self.view.layer.contents = image?.cgImage
        
        guard let session = ALCenter.shared().liveSession else {
            assert(false)
            return
        }
        
        setIdleTimerActive(false)
        liveSession(session)
        liveRole(session)
        liveRoom(session)
        bottomTools(session)
        extralBottomTools(session)
        
        audience()
        chatList()
        gift()
        chatInput()
        musicList()
        netMonitor()
        
        goods(session: session)
        PK(session: session)
        multiHosts()
        multiHostCount()
        seat()
        userList()
    }
    
    override func prepare(for segue: UIStoryboardSegue, sender: Any?) {
        guard let identifier = segue.identifier else {
            return
        }
        
        switch identifier {
        case "GiftAudienceViewController":
            let vc = segue.destination as! GiftAudienceViewController
            self.giftAudienceVC = vc
        case "BottomToolsViewController":
            guard let session = ALCenter.shared().liveSession else {
                assert(false)
                return
            }
            let role = session.role.value
            let vc = segue.destination as! BottomToolsViewController
            vc.liveType = session.type
            vc.perspective = role.type
            self.bottomToolsVC = vc
        case "ChatViewController":
            let vc = segue.destination as! ChatViewController
            vc.cellColor = tintColor
            self.chatVC = vc
        case "PKViewController":
            let vc = segue.destination as! PKViewController
            self.pkView = vc
        default:
            break
        }
    }
}

extension LiveShoppingViewController {
    // MARK: - Live Room
    func liveRoom(_ session: LiveSession) {
        let owner = session.owner
        
        ownerView.offsetLeftX = -14
        ownerView.offsetRightX = 5
        ownerView.label.textColor = .white
        ownerView.label.font = UIFont.systemFont(ofSize: 11)
        ownerView.backgroundColor = tintColor
        
        owner.subscribe(onNext: { [unowned self] (owner) in
            let images = ALCenter.shared().centerProvideImagesHelper()
            let user = owner.user
            self.ownerView.label.text = user.info.name
            self.ownerView.imageView.image = images.getHead(index: user.info.imageIndex)
            self.pkView?.intoOtherButton.isHidden = owner.isLocal
        }).disposed(by: bag)
    }
    
    func userList() {
        personCountView.rx.controlEvent(.touchUpInside).subscribe(onNext: { [unowned self] in
            guard let owner = ALCenter.shared().liveSession?.owner.value else {
                return
            }
            
            if owner.isLocal, !self.pkVM.state.value.isDuration {
                self.presentUserList(type: .multiHosts)
            } else {
                self.presentUserList(type: .onlyUser)
            }
        }).disposed(by: bag)
    }
    
    func extralBottomTools(_ session: LiveSession) {
        guard let bottomToolsVC = self.bottomToolsVC else {
            return
        }
        
        bottomToolsVC.shoppingButton.rx.tap.subscribe(onNext: { [unowned self] in
            self.presentGoodsList()
        }).disposed(by: bag)
        
        bottomToolsVC.pkButton.rx.tap.subscribe(onNext: { [unowned self] in
            self.presentRoomList()
        }).disposed(by: bag)
        
        bottomToolsVC.closeButton.rx.tap.subscribe(onNext: { [unowned self] () in
            if self.pkVM.state.value.isDuration {
                self.showAlert(NSLocalizedString("End_PK"),
                               message: NSLocalizedString("End_PK_Message"),
                               action1: NSLocalizedString("Cancel"),
                               action2: NSLocalizedString("End")) { [unowned self] (_) in
                                self.leave()
                                self.dimissSelf()
                }
            } else {
                self.showAlert(NSLocalizedString("Live_End"),
                               message: NSLocalizedString("Confirm_End_Live"),
                               action1: NSLocalizedString("Cancel"),
                               action2: NSLocalizedString("Confirm")) { [unowned self] (_) in
                                self.leave()
                                self.dimissSelf()
                }
            }
        }).disposed(by: bag)
    }
}

// MARK: - Product
private extension LiveShoppingViewController {
    func goods(session: LiveSession) {
        goodsVM.refetchList()
        
        goodsVM.requestSuccess.subscribe(onNext: { [unowned self] (text) in
            self.showTextToast(text: text)
        }).disposed(by: bag)
        
        goodsVM.requestError.subscribe(onNext: { [unowned self] (text) in
            self.showTextToast(text: text)
        }).disposed(by: bag)
        
        let localIsOwner = session.owner.value.isLocal
        
        // audience
        goodsVM.itemOnShelf.subscribe(onNext: { [unowned self] (item) in
            if self.presentingChild is GoodsListViewController {
                return
            }
            
            guard !localIsOwner,
                item.isSale,
                let shoppingButton = self.bottomToolsVC?.shoppingButton else {
                return
            }
            
            let notification = item.name + " " + NSLocalizedString("Product_On_Shelf_Notification")
            let popoverContentHeight: CGFloat = 30
            let font = UIFont.systemFont(ofSize: 14)
            let size = notification.size(font: font,
                                         drawRange: CGSize(width: CGFloat(MAXFLOAT), height: popoverContentHeight))
            self.popoverContent.textAlignment = .center
            self.popoverContent.font = font
            self.popoverContent.text = notification
            self.popoverContent.frame = CGRect(x: 0,
                                               y: 0,
                                               width: size.width + 10,
                                               height: popoverContentHeight)
            
            self.popover.dismiss()
            self.popover.show(self.popoverContent, fromView: shoppingButton)
        }).disposed(by: bag)
    }
    
    func presentGoodsList() {
        guard let role = ALCenter.shared().liveSession?.role.value,
            let owner = ALCenter.shared().liveSession?.owner.value.user else {
                return
        }
        
        self.showMaskView(color: UIColor.clear)
        
        let vc = UIStoryboard.initViewController(of: "GoodsListViewController",
                                                 class: GoodsListViewController.self,
                                                 on: "Popover")
        vc.perspective = role.type
        vc.vm = goodsVM
        vc.view.cornerRadius(10)
        
        let presenetedHeight: CGFloat = 526.0
        let y = UIScreen.main.bounds.height - presenetedHeight
        let presentedFrame = CGRect(x: 0,
                                    y: y,
                                    width: UIScreen.main.bounds.width,
                                    height: presenetedHeight)
        
        self.presentChild(vc,
                          animated: true,
                          presentedFrame: presentedFrame)
        
        if role.type == .owner {
            vc.itemOnShelf.subscribe(onNext: { [unowned self] (item) in
                self.hiddenMaskView()
                var message: String
                if DeviceAssistant.Language.isChinese {
                    message = "你是否要上架\"\(item.name)\"?"
                } else {
                    message = "Lauch \(item.name)?"
                }
                
                self.showAlert(message: message,
                               action1: NSLocalizedString("Cancel"),
                               action2: NSLocalizedString("Confirm")) { [unowned self] (_) in
                                self.goodsVM.itemOnShelf(item)
                }
            }).disposed(by: bag)
            
            vc.itemOffShelf.subscribe(onNext: { [unowned self] (item) in
                self.hiddenMaskView()
                var message: String
                if DeviceAssistant.Language.isChinese {
                    message = "你是否要下架\"\(item.name)\"?"
                } else {
                    message = "Retire \(item.name)?"
                }
                
                self.showAlert(message: message,
                               action1: NSLocalizedString("Cancel"),
                               action2: NSLocalizedString("Confirm")) { [unowned self] (_) in
                                self.goodsVM.itemOffShelf(item)
                }
            }).disposed(by: bag)
        } else {
            vc.itemDetail.subscribe(onNext: { [unowned self] (item) in
                let vc = UIStoryboard.initViewController(of: "ProductDetailViewController",
                                                         class: ProductDetailViewController.self,
                                                         on: "Live")
                vc.product = item
                vc.owner = owner
                vc.playerVM = self.playerVM
                vc.purchase.subscribe(onNext: { [unowned self] (item) in
                    self.goodsVM.purchase(item: item)
                }).disposed(by: self.bag)
                
                vc.close.subscribe(onNext: { [unowned self] in
                    if self.pkVM.state.value.isDuration {
                        self.playerVM.startRenderVideoStreamOf(user: owner,
                                                               on: self.pkView!.leftRenderView)
                    } else {
                        self.playerVM.startRenderVideoStreamOf(user: owner,
                                                               on: self.renderView)
                    }
                }).disposed(by: self.bag)
                
                self.navigationController?.pushViewController(vc, animated: true)
            }).disposed(by: bag)
        }
    }
}

// MARK: - Multi Hosts
private extension LiveShoppingViewController {
    func multiHosts() {
        // owner
        multiHostsVM.receivedApplication.subscribe(onNext: { [unowned self] (application) in
            self.personCountView.needRemind = true
        }).disposed(by: bag)
        
        multiHostsVM.invitationByRejected.subscribe(onNext: { [unowned self] (invitation) in
            if DeviceAssistant.Language.isChinese {
                self.showTextToast(text: invitation.receiver.info.name + "拒绝了这次邀请")
            } else {
                self.showTextToast(text: invitation.receiver.info.name + "rejected this invitation")
            }
        }).disposed(by: bag)
        
        // broadcaster
        multiHostsVM.receivedEndBroadcasting.subscribe(onNext: { [unowned self] in
            if DeviceAssistant.Language.isChinese {
                self.showTextToast(text: "房主强迫你下麦")
            } else {
                self.showTextToast(text: "Owner forced you to becmoe a audience")
            }
            
            guard let session = ALCenter.shared().liveSession else {
                return
            }
            
            session.broadcasterToAudience()
        }).disposed(by: bag)
        
        // audience
        multiHostsVM.receivedInvitation.subscribe(onNext: { [unowned self] (invitation) in
            self.showAlert(message: NSLocalizedString("Confirm_Accept_Broadcasting_Invitation"),
                           action1: NSLocalizedString("Reject"),
                           action2: NSLocalizedString("Confirm"),
                           handler1: { [unowned self] (_) in
                            self.multiHostsVM.reject(invitation: invitation)
            }) { [unowned self] (_) in
                self.multiHostsVM.accept(invitation: invitation, success: {
                    guard let session = ALCenter.shared().liveSession else {
                        return
                    }
                    session.audienceToBroadcaster()
                }) { [unowned self] (_) in
                    self.showTextToast(text: "accept invitation fail")
                }
            }
        }).disposed(by: bag)
        
        multiHostsVM.applicationByAccepted.subscribe(onNext: { [unowned self] (_) in
            guard let session = ALCenter.shared().liveSession else {
                return
            }
            self.hiddenMaskView()
            session.audienceToBroadcaster()
        }).disposed(by: bag)
        
        multiHostsVM.applicationByRejected.subscribe(onNext: { [unowned self] (application) in
            if DeviceAssistant.Language.isChinese {
                self.showTextToast(text: "房间拒绝你的申请")
            } else {
                self.showTextToast(text: "Owner rejected your application")
            }
        }).disposed(by: bag)
        
        // role update
        multiHostsVM.audienceBecameBroadcaster.subscribe(onNext: { [unowned self] (user) in
            if DeviceAssistant.Language.isChinese {
                let chat = Chat(name: user.info.name, text: " 上麦")
                self.chatVM.newMessages([chat])
            } else {
                let chat = Chat(name: user.info.name, text: " became a broadcaster")
                self.chatVM.newMessages([chat])
            }
        }).disposed(by: bag)
        
        multiHostsVM.broadcasterBecameAudience.subscribe(onNext: { [unowned self] (user) in
            if DeviceAssistant.Language.isChinese {
                let chat = Chat(name: user.info.name, text: " 下麦")
                self.chatVM.newMessages([chat])
            } else {
                let chat = Chat(name: user.info.name, text: " became a audience")
                self.chatVM.newMessages([chat])
            }
        }).disposed(by: bag)
        
        presentedChild.subscribe(onNext: { [unowned self] (vc) in
            guard let extensionVC = vc as? ExtensionViewController else {
                return
            }
            self.applicationSubscribe = extensionVC.broadcastingButton.rx.tap.subscribe(onNext: { [unowned self] in
                self.hiddenMaskView()
                
                guard let local = ALCenter.shared().liveSession?.role.value else {
                    return
                }
                self.showAlert(message: NSLocalizedString("Confirm_Apply_For_Broadcasting"),
                               action1: NSLocalizedString("Cancel"),
                               action2: NSLocalizedString("Confirm")) { [unowned self] (_) in
                                self.multiHostsVM.sendApplication(by: local,
                                                                  for: 1) { [unowned self] (_) in
                                                                    self.showTextToast(text: "send application fail")
                                }
                }
            })
        }).disposed(by: bag)
        
        dimissChild.subscribe(onNext: { [unowned self] (vc) in
            guard vc is ExtensionViewController else {
                return
            }
            self.applicationSubscribe?.dispose()
        }).disposed(by: bag)
    }
    
    func seat() {
        seatVM.list.subscribe(onNext: { [unowned self] (list) in
            guard let session = ALCenter.shared().liveSession else {
                assert(false)
                return
            }
            
            if list.count == 1, let remote = list[0].user {
                self.hostCount.accept(.multi([session.owner.value.user, remote]))
            } else {
                self.hostCount.accept(.single(session.owner.value.user))
            }
        }).disposed(by: bag)
    }
    
    func multiHostCount() {
        hostCount.subscribe(onNext: { [unowned self] (hostCount) in
            guard let session = ALCenter.shared().liveSession else {
                return
            }
            
            self.bottomToolsVC?.pkButton.isEnabled = hostCount.isSingle
            
            switch hostCount {
            case .single(let user):
                self.broadcasteRender.removeFromSuperview()
                self.playerVM.startRenderVideoStreamOf(user: user,
                                                       on: self.renderView)
            case .multi(let list):
                guard let user = list.last else {
                    return
                }
                
                if let subscribe = self.broadcasterSubscribe {
                    subscribe.dispose()
                }
                
                self.view.addSubview(self.broadcasteRender)
                self.playerVM.startRenderVideoStreamOf(user: user,
                                                       on: self.broadcasteRender.renderView)
                
                switch session.role.value.type {
                case .owner:
                    self.broadcasterSubscribe = self.broadcasteRender.closeButton.rx.tap.subscribe(onNext: { [unowned self] in
                        var message: String
                        if DeviceAssistant.Language.isChinese {
                            message = "确定\"\(user.info.name)\"下麦?"
                        } else {
                            message = "Stop \(user.info.name) hosting"
                        }
                        
                        self.showAlert(message: message,
                                       action1: NSLocalizedString("Cancel"),
                                       action2: NSLocalizedString("Confirm")) { [unowned self] (_) in
                                        self.multiHostsVM.forceEndBroadcasting(user: user,
                                                                               on: 1) { [unowned self] (_) in
                                                                                self.showTextToast(text: "force user end broadcasting fail")
                                        }
                        }
                    })
                case .broadcaster:
                    self.broadcasterSubscribe = self.broadcasteRender.closeButton.rx.tap.subscribe(onNext: { [unowned self] in
                        self.showAlert(message: NSLocalizedString("Confirm_End_Broadcasting"),
                                       action1: NSLocalizedString("Cancel"),
                                       action2: NSLocalizedString("Confirm")) { [unowned self] (_) in
                                        self.multiHostsVM.endBroadcasting(seatIndex: 1, user: user, success: {
                                            guard let session = ALCenter.shared().liveSession else {
                                                assert(false)
                                                return
                                            }
                                            session.broadcasterToAudience()
                                        })
                        }
                    })
                case .audience:
                    self.broadcasteRender.closeButton.isHidden = true
                }
            }
        }).disposed(by: bag)
    }
    
    func presentUserList(type: CVUserListViewController.ShowType) {
        guard (type == .multiHosts) || (type == .onlyUser) else {
            return
        }
        
        self.showMaskView(color: UIColor.clear)
        
        let vc = UIStoryboard.initViewController(of: "CVUserListViewController",
                                                 class: CVUserListViewController.self,
                                                 on: "Popover")
        
        vc.userListVM = userListVM
        vc.multiHostsVM = multiHostsVM
        vc.showType = type
        vc.view.cornerRadius(10)
        
        vc.inviteUser.subscribe(onNext: { [unowned self] (user) in
            self.hiddenMaskView()
            
            var message: String
            if DeviceAssistant.Language.isChinese {
                message = "你是否要邀请\"\(user.info.name)\"上麦?"
            } else {
                message = "Do you send a invitation to \(user.info.name)?"
            }
            
            self.showAlert(message: message,
                           action1: NSLocalizedString("Cancel"),
                           action2: NSLocalizedString("Confirm")) { [unowned self] (_) in
                            self.multiHostsVM.sendInvitation(to: user, on: 1) { [unowned self] (_) in
                                self.showTextToast(text: NSLocalizedString("Invite_Broadcasting_Fail"))
                            }
            }
        }).disposed(by: bag)
        
        if type == .multiHosts {
            vc.tabView.needRemind(personCountView.needRemind,
                                  index: 1)
            
            vc.tabView.selectedIndex.subscribe(onNext: { [unowned self] (index) in
                if index == 1 {
                    self.personCountView.needRemind = false
                }
            }).disposed(by: bag)
        }
        
        vc.rejectApplicationOfUser.subscribe(onNext: { [unowned self] (application) in
            self.hiddenMaskView()
            
            var message: String
            if DeviceAssistant.Language.isChinese {
                message = "你是否要拒绝\"\(application.initiator.info.name)\"的上麦申请?"
            } else {
                message = "Do you reject \(application.initiator.info.name)'s application?"
            }
            
            self.showAlert(message: message,
                           action1: NSLocalizedString("Cancel"),
                           action2: NSLocalizedString("Confirm")) { [unowned self] (_) in
                            self.multiHostsVM.reject(application: application) { [unowned self] (_) in
                                self.showTextToast(text: "Reject application fail")
                            }
            }
        }).disposed(by: bag)
        
        vc.acceptApplicationOfUser.subscribe(onNext: { [unowned self] (application) in
            self.hiddenMaskView()
            
            var message: String
            if DeviceAssistant.Language.isChinese {
                message = "你是否要接受\"\(application.initiator.info.name)\"的上麦申请?"
            } else {
                message = "Do you accept \(application.initiator.info.name)'s application?"
            }
            
            self.showAlert(message: message,
                           action1: NSLocalizedString("Cancel"),
                           action2: NSLocalizedString("Confirm")) { [unowned self] (_) in
                            self.multiHostsVM.accept(application: application) { [unowned self] (_) in
                                self.showTextToast(text: "Accept application fail")
                            }
            }
        }).disposed(by: bag)
        
        let presenetedHeight: CGFloat = 526.0
        let y = UIScreen.main.bounds.height - presenetedHeight
        let presentedFrame = CGRect(x: 0,
                                    y: y,
                                    width: UIScreen.main.bounds.width,
                                    height: presenetedHeight)
        
        self.presentChild(vc,
                          animated: true,
                          presentedFrame: presentedFrame)
    }
}

// MARK: - PK
private extension LiveShoppingViewController {
    func PK(session: LiveSession) {
        // View
        pkView?.intoOtherButton.rx.tap.subscribe(onNext: { [unowned self] in
            self.intoRemoteRoom()
        }).disposed(by: bag)
        
        // VM
        pkVM.event.subscribe(onNext: { [unowned self] (event) in
            switch event {
            case .start:
                break
            case .end(let result):
                self.show(result: result) { [unowned self] in
                    self.renderView.isHidden = false
                    self.pkContainerView.isHidden = true
                }
                self.show(result: result)
                
            case .rankChanged(let local, let remote):
                self.pkView?.giftBar.leftValue = local
                self.pkView?.giftBar.rightValue = remote
            }
        }).disposed(by: bag)
        
        pkVM.state.map { (state) -> Bool in
            return !state.isDuration
        }.bind(to: bottomToolsVC!.pkButton.rx.isEnabled).disposed(by: bag)
        
        pkVM.state.subscribe(onNext: { [unowned self] (state) in
            guard let session = ALCenter.shared().liveSession else {
                return
            }
            
            let owner = session.owner.value
            
            switch state {
            case .duration(let info):
                guard let leftRender = self.pkView?.leftRenderView,
                    let rightRender = self.pkView?.rightRenderView else {
                    return
                }
                self.renderView.isHidden = true
                self.pkContainerView.isHidden = false
                
                self.playerVM.startRenderVideoStreamOf(user: owner.user,
                                                       on: leftRender)
                self.playerVM.startRenderVideoStreamOf(user: info.remoteRoom.owner,
                                                       on: rightRender)
                
                self.pkView?.startCountingDown()
                self.pkView?.giftBar.leftValue = info.localRank
                self.pkView?.giftBar.rightValue = info.remoteRank
                self.pkView?.rightLabel.text = info.remoteRoom.owner.info.name
                self.pkView?.countDown = info.countDown
                let height = UIScreen.main.bounds.height - self.pkContainerView.frame.maxY - UIScreen.main.heightOfSafeAreaBottom - 20 - self.bottomToolsVC!.view.bounds.height
                self.chatViewHeight.constant = height
            case .none:
                self.playerVM.startRenderVideoStreamOf(user: owner.user,
                                                       on: self.renderView)
                self.pkView?.stopCountingDown()
                self.chatViewHeight.constant = 219
            default:
                break
            }
        }).disposed(by: bag)
        
        pkVM.receivedInvitation.subscribe(onNext: { [unowned self] (battle) in
            self.bottomToolsVC?.pkButton.needRemind = true
        }).disposed(by: bag)
        
        pkVM.invitationIsByRejected.subscribe(onNext: { [unowned self] (battle) in
            self.showTextToast(text: NSLocalizedString("PK_Invite_Reject"))
        }).disposed(by: bag)
        
        pkVM.requestError.subscribe(onNext: { [unowned self] (text) in
            self.showTextToast(text: text)
        }).disposed(by: bag)
    }
        
    func presentRoomList() {
        self.showMaskView(color: UIColor.clear)
        
        let vc = UIStoryboard.initViewController(of: "CVUserListViewController",
                                                 class: CVUserListViewController.self,
                                                 on: "Popover")
        
        vc.pkVM = pkVM
        vc.showType = .pk
        vc.view.cornerRadius(10)
        
        vc.tabView.needRemind(bottomToolsVC!.pkButton.needRemind,
                              index: 1)
        
        vc.tabView.selectedIndex.subscribe(onNext: { [unowned self] (index) in
            if index == 1 {
                self.bottomToolsVC?.pkButton.needRemind = false
            }
        }).disposed(by: bag)
        
        vc.inviteRoom.subscribe(onNext: { [unowned self] (room) in
            self.hiddenMaskView()
            
            var message: String
            if DeviceAssistant.Language.isChinese {
                message = "你是否要邀请\"\(room.name)\"PK?"
            } else {
                message = "Do you send a invitation to \(room.name)?"
            }
            
            self.showAlert(message: message,
                           action1: NSLocalizedString("Cancel"),
                           action2: NSLocalizedString("Confirm")) { [unowned self] (_) in
                            self.pkVM.sendInvitationTo(room: room)
            }
        }).disposed(by: bag)
        
        vc.rejectApplicationOfRoom.subscribe(onNext: { [unowned self] (application) in
            self.hiddenMaskView()
            
            var message: String
            if DeviceAssistant.Language.isChinese {
                message = "你是否要拒绝\"\(application.initatorRoom.name)\"的PK邀请?"
            } else {
                message = "Do you reject \(application.initatorRoom.name)'s application?"
            }
            
            self.showAlert(message: message,
                           action1: NSLocalizedString("Cancel"),
                           action2: NSLocalizedString("Confirm")) { [unowned self] (_) in
                            self.pkVM.reject(invitation: application)
            }
        }).disposed(by: bag)
        
        vc.accepteApplicationOfRoom.subscribe(onNext: { [unowned self] (application) in
            self.hiddenMaskView()
            
            var message: String
            if DeviceAssistant.Language.isChinese {
                message = "你是否要接受\"\(application.initatorRoom.name)\"的PK邀请?"
            } else {
                message = "Do you accept \(application.initatorRoom.name)'s application?"
            }
            
            self.showAlert(message: message,
                           action1: NSLocalizedString("Cancel"),
                           action2: NSLocalizedString("Confirm")) { [unowned self] (_) in
                            self.pkVM.accept(invitation: application)
            }
        }).disposed(by: bag)
        
        let presenetedHeight: CGFloat = 526.0
        let y = UIScreen.main.bounds.height - presenetedHeight
        let presentedFrame = CGRect(x: 0,
                                    y: y,
                                    width: UIScreen.main.bounds.width,
                                    height: presenetedHeight)
        
        self.presentChild(vc,
                          animated: true,
                          presentedFrame: presentedFrame)
    }
    
    func intoRemoteRoom() {
        guard let session = ALCenter.shared().liveSession,
            let pkInfo = self.pkVM.state.value.pkInfo else {
            assert(false)
            return
        }
        
        session.leave()
        
        let owner = pkInfo.remoteRoom.owner
        let role = session.role.value
        let room = Room(name: "",
                        roomId: pkInfo.remoteRoom.roomId,
                        imageURL: "",
                        personCount: 0,
                        owner: owner)
        
        let newSession = LiveSession(room: room,
                                     videoConfiguration: VideoConfiguration(),
                                     type: .pk,
                                     owner: .otherUser(owner),
                                     role: role)
        
        newSession.join(success: { [unowned newSession, unowned self] (joinedInfo) in
            guard let pkInfo = joinedInfo.pkInfo,
                let vm = try? PKVM(room: joinedInfo.room, type: .shopping, state: pkInfo),
                let navigation = self.navigationController else {
                    assert(false)
                    return
            }
            
            ALCenter.shared().liveSession = newSession
            let newPk = UIStoryboard.initViewController(of: "PKBroadcastersViewController",
                                                        class: PKBroadcastersViewController.self,
                                                        on: "Live")
            newPk.userListVM = LiveUserListVM(room: joinedInfo.room)
            newPk.userListVM.updateGiftListWithJson(list: joinedInfo.giftAudience)
            newPk.pkVM = vm
            
            navigation.popViewController(animated: false)
            navigation.pushViewController(newPk, animated: false)
        }) { [weak self] in
            self?.showTextToast(text: NSLocalizedString("Join_Other_Live_Room_Fail"))
        }
    }
    
    func show(result: PKResult, completion: Completion = nil) {
        let tCompletion = { [weak self] in
            if let completion = completion {
                completion()
            }
            
            let view = TextToast(frame: CGRect(x: 0, y: 200, width: 0, height: 44), filletRadius: 8)
            view.text = NSLocalizedString("PK_End")
            self?.showToastView(view, duration: 0.2)
        }
        
        switch result {
        case .win:
            self.pkView?.showWinner(isLeft: true, completion: tCompletion)
        case .draw:
            self.pkView?.showWinner(isLeft: false, completion: tCompletion)
        case .lose:
            self.pkView?.showDraw(completion: completion)
        }
    }
}
