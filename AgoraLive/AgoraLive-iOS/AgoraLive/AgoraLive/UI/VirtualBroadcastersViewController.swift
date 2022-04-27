//
//  VirtualBroadcastersViewController.swift
//  AgoraLive
//
//  Created by CavanSu on 2020/5/29.
//  Copyright © 2020 Agora. All rights reserved.
//

import UIKit
import RxSwift
import RxRelay
import AGEVideoLayout

class VirtualBroadcastersViewController: MaskViewController, LiveViewController {
    @IBOutlet weak var ownerView: IconTextView!
    @IBOutlet weak var videoContainer: AGEVideoContainer!
    @IBOutlet weak var inviteButton: UIButton!
    @IBOutlet weak var chatViewHeight: NSLayoutConstraint!
    
    private var ownerRenderView = UIView()
    private var broadcasterRenderView = UIView()
    
    var multiHostsVM: MultiHostsVM!
    var seatVM: LiveSeatVM!
    var hostCount: BehaviorRelay<HostCount>!
    
    // LiveViewController
    var tintColor = UIColor(red: 0,
                            green: 0,
                            blue: 0,
                            alpha: 0.08)
    
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
        print("deinit VirtualBroadcastersViewController")
        #endif
    }
    
    override func viewDidLoad() {
        super.viewDidLoad()
        guard let session = ALCenter.shared().liveSession else {
            assert(false)
            return
        }
        
        setIdleTimerActive(false)
        liveSession(session)
        liveRole(session)
        extralLiveRole(session)
        liveRoom(session: session)
        bottomTools(session)
        
        audience()
        chatList()
        gift()
        chatInput()
        musicList()
        netMonitor()
        
        multiHostCount()
        liveSeat()
        multiHosts()
        netMonitor()
        
        updateViews()
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
            
            let vc = segue.destination as! BottomToolsViewController
            vc.liveType = session.type
            vc.perspective = session.role.value.type
            self.bottomToolsVC = vc
        case "ChatViewController":
            let vc = segue.destination as! ChatViewController
            vc.cellColor = tintColor
            vc.contentColor = UIColor(hexString: "#333333")
            self.chatVC = vc
        default:
            break
        }
    }
}

extension VirtualBroadcastersViewController {
    func updateViews() {
        videoContainer.backgroundColor = .white
        ownerRenderView.backgroundColor = .white
        broadcasterRenderView.backgroundColor = .white
        
        ownerView.backgroundColor = tintColor
        ownerView.offsetLeftX = -13
        ownerView.offsetRightX = 5
        ownerView.label.textColor = UIColor(hexString: "#333333")
        ownerView.label.font = UIFont.systemFont(ofSize: 11)
        
        personCountView.backgroundColor = tintColor
        personCountView.imageView.image = UIImage(named: "icon-mine-black")
        personCountView.label.textColor = UIColor(hexString: "#333333")
        
        let chatViewMaxHeight = UIScreen.main.bounds.height * 0.25
        if chatViewHeight.constant > chatViewMaxHeight {
            chatViewHeight.constant = chatViewMaxHeight
        }
    }
    
    // MARK: - Live Room
    func liveRoom(session: LiveSession) {
        let images = ALCenter.shared().centerProvideImagesHelper()
        
        session.owner.subscribe(onNext: { [unowned self] (owner) in
            self.ownerView.label.text = owner.user.info.name
            self.ownerView.imageView.image = images.getHead(index: owner.user.info.imageIndex)
            
            self.playerVM.startRenderVideoStreamOf(user: owner.user,
                                                   on: self.ownerRenderView)
        }).disposed(by: bag)
        
        inviteButton.rx.tap.subscribe(onNext: { [unowned self] in
            guard let session = ALCenter.shared().liveSession else {
                assert(false)
                return
            }
            
            switch (self.hostCount.value, session.owner.value) {
            case (.single, .localUser):
                self.presentInvitationList()
            case (.multi, .localUser):
                self.forceBroadcasterToBeAudience()
            case (.multi, .otherUser):
                guard session.role.value.type == .broadcaster else {
                    return
                }
                self.presentEndBroadcasting()
            default: break
            }
        }).disposed(by: bag)
    }
    
    func extralLiveRole(_ session: LiveSession) {
        let role = session.role
        role.subscribe(onNext: { [unowned self] (local) in
            switch local.type {
            case .owner:
                self.inviteButton.isHidden = false
                self.inviteButton.setTitle(NSLocalizedString("Invite_Broadcasting"), for: .normal)
            case .broadcaster:
                self.inviteButton.isHidden = false
                self.inviteButton.setTitle(NSLocalizedString("End_Broadcasting"), for: .normal)
            case .audience:
                self.inviteButton.isHidden = true
            }
            
        }).disposed(by: bag)
    }
    
    func liveSeat() {
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
    
    func multiHosts() {
        // owner
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
                assert(false)
                return
            }
            self.hiddenMaskView()
            session.broadcasterToAudience()
        }).disposed(by: bag)
        
        // audience
        multiHostsVM.receivedInvitation.subscribe(onNext: { [unowned self] (invitation) in
            self.showAlert(NSLocalizedString("Broadcasting_Invitation"),
                           message: NSLocalizedString("Confirm_Accept_Broadcasting_Invitation"),
                           action1: NSLocalizedString("Reject"),
                           action2: NSLocalizedString("Accept"),
                           handler1: { [unowned self] (_) in
                            self.multiHostsVM.reject(invitation: invitation)
            }) { [unowned self] (_) in
                self.presentVirtualAppearance(close: { [unowned self] in
                    self.multiHostsVM.reject(invitation: invitation)
                }) { [unowned self] in
                    self.multiHostsVM.accept(invitation: invitation, success: {
                        guard let session = ALCenter.shared().liveSession else {
                            assert(false)
                            return
                        }
                        session.audienceToBroadcaster()
                    }) { [unowned self] (_) in
                        self.showTextToast(text: "accept invitation fail")
                    }
                }
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
    }
    
    func multiHostCount() {
        hostCount.subscribe(onNext: { [unowned self] (hostCount) in
            guard let session = ALCenter.shared().liveSession else {
                    assert(false)
                    return
            }
            
            switch hostCount {
            case .multi(let list):
                if session.owner.value.isLocal {
                    self.inviteButton.isHidden = false
                    self.inviteButton.setTitle(NSLocalizedString("End_Broadcasting"), for: .normal)
                }
                
                for user in list where user.info != session.owner.value.user.info {
                    self.playerVM.startRenderVideoStreamOf(user: user,
                                                           on: self.broadcasterRenderView)
                }
            case .single:
                if session.owner.value.isLocal {
                    self.inviteButton.isHidden = false
                    self.inviteButton.setTitle(NSLocalizedString("Invite_Broadcasting"), for: .normal)
                }
            }
            
            // Video Layout
            self.updateVideoLayout(onlyOwner: hostCount.isSingle)
        }).disposed(by: bag)
    }
    
    func updateVideoLayout(onlyOwner: Bool, animated: Bool = true) {
        var layout: AGEVideoLayout
        
        if onlyOwner {
            layout = AGEVideoLayout(level: 0)
        } else {
            let width = UIScreen.main.bounds.width
            let height = width * 10 / 16
            
            layout = AGEVideoLayout(level: 0)
                .size(.constant(CGSize(width: width, height: height)))
                .itemSize(.scale(CGSize(width: 0.5, height: 1)))
                .startPoint(x: 0, y: 160 + UIScreen.main.heightOfSafeAreaTop)
        }
        
        videoContainer.listItem { [unowned self] (index) -> AGEView in
            if onlyOwner {
                return self.ownerRenderView
            } else {
                switch index.item {
                case 0: return self.ownerRenderView
                case 1: return self.broadcasterRenderView
                default: assert(false); return UIView()
                }
            }
        }
        
        videoContainer.listCount { (_) -> Int in
            return onlyOwner ? 1 : 2
        }
        
        videoContainer.setLayouts([layout], animated: true)
    }
}

extension VirtualBroadcastersViewController {
    func presentInvitationList() {
        self.showMaskView(color: UIColor.clear)
        
        let vc = UIStoryboard.initViewController(of: "CVUserListViewController",
                                                 class: CVUserListViewController.self,
                                                 on: "Popover")
        
        vc.userListVM = userListVM
        vc.multiHostsVM = multiHostsVM
        vc.showType = .onlyInvitationOfMultiHosts
        vc.view.cornerRadius(10)
        
        let presenetedHeight: CGFloat = 526.0 + UIScreen.main.heightOfSafeAreaTop
        let y = UIScreen.main.bounds.height - presenetedHeight
        let presentedFrame = CGRect(x: 0,
                                    y: y,
                                    width: UIScreen.main.bounds.width,
                                    height: presenetedHeight)
        
        vc.inviteUser.subscribe(onNext: { [unowned self] (user) in
            self.hiddenMaskView()
            
            var message: String
            if DeviceAssistant.Language.isChinese {
                message = "你是否想邀请\"\(user.info.name)\"上麦?"
            } else {
                message = "Do you send a invitation to \(user.info.name)?"
            }
            
            self.showAlert(message: message,
                           action1: NSLocalizedString("Cancel"),
                           action2: NSLocalizedString("Confirm")) { [unowned self] (_) in
                            self.multiHostsVM.sendInvitation(to: user, on: 1)
            }
        }).disposed(by: bag)
        
        self.presentChild(vc,
                          animated: true,
                          presentedFrame: presentedFrame)
    }
    
    func presentVirtualAppearance(close: Completion, confirm: Completion) {
        let vc = UIStoryboard.initViewController(of: "VirtualAppearanceViewController",
                                                 class: VirtualAppearanceViewController.self)
        
        self.present(vc, animated: true) { [unowned vc, unowned self] in
            vc.closeButton.rx.tap.subscribe(onNext: {
                if let close = close {
                    close()
                }
            }).disposed(by: self.bag)
            
            vc.confirmButton.rx.tap.subscribe(onNext: {
                if let confirm = confirm {
                    confirm()
                }
            }).disposed(by: self.bag)
        }
    }
    
    // Owner
    func forceBroadcasterToBeAudience() {
        self.showAlert(NSLocalizedString("End_Broadcasting"),
                       message: NSLocalizedString("Confirm_End_Broadcasting"),
                       action1: NSLocalizedString("Cancel"),
                       action2: NSLocalizedString("Confirm"),
                       handler1: { [unowned self] (_) in
                        self.hiddenMaskView()
        }) { [unowned self] (_) in
            guard let session = ALCenter.shared().liveSession,
                session.owner.value.isLocal else {
                return
            }
            
            guard let user = self.seatVM.list.value.first?.user else {
                return
            }
            
            self.multiHostsVM.forceEndBroadcasting(user: user,
                                                   on: 1) { (_) in
                                                    self.showTextToast(text: "force user end broadcasting fail")
            }
        }
    }
    
    // Broadcaster
    func presentEndBroadcasting() {
        self.showAlert(NSLocalizedString("End_Broadcasting"),
                       message: NSLocalizedString("Confirm_End_Broadcasting"),
                       action1: NSLocalizedString("Cancel"),
                       action2: NSLocalizedString("Confirm"),
                       handler1: { [unowned self] (_) in
                        self.hiddenMaskView()
        }) { [unowned self] (_) in
            self.hiddenMaskView()
            
            guard let role = ALCenter.shared().liveSession?.role.value,
                role.type == .broadcaster else {
                return
            }
            
            self.multiHostsVM.endBroadcasting(seatIndex: 1, user: role, success: {
                guard let session = ALCenter.shared().liveSession else {
                    assert(false)
                    return
                }
                session.broadcasterToAudience()
            })
        }
    }
}
