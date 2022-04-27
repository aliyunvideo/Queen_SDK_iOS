//
//  MultiBroadcastersViewController.swift
//  AgoraLive
//
//  Created by CavanSu on 2020/3/23.
//  Copyright © 2020 Agora. All rights reserved.
//

import UIKit
import RxSwift
import RxRelay
import MJRefresh

class MultiBroadcastersViewController: MaskViewController, LiveViewController {
    @IBOutlet weak var ownerRenderView: LabelShadowRender!
    @IBOutlet weak var roomLabel: UILabel!
    @IBOutlet weak var roomNameLabel: UILabel!
    
    private weak var seatVC: LiveSeatViewController?
    
    var multiHostsVM: MultiHostsVM!
    var seatVM: LiveSeatVM!
    
    // LiveViewController
    var tintColor = UIColor(red: 0,
                            green: 0,
                            blue: 0,
                            alpha: 0.4)
    
    var bag = DisposeBag()
    
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
        print("deinit MultiBroadcastersViewController")
        #endif
    }
    
    override var preferredStatusBarStyle: UIStatusBarStyle {
        return .lightContent
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
        extralLiveRole(session)
        liveRoom(session)
        bottomTools(session)
        
        audience()
        chatList()
        gift()
        chatInput()
        musicList()
        netMonitor()
        
        multiHosts(session)
        liveSeat()
        activeSpeaker()
    }
    
    override func prepare(for segue: UIStoryboardSegue, sender: Any?) {
        guard let identifier = segue.identifier else {
            return
        }
        
        switch identifier {
        case "LiveSeatViewController":
            guard let type = ALCenter.shared().liveSession?.role.value.type else {
                assert(false)
                return
            }
            
            let vc = segue.destination as! LiveSeatViewController
            vc.perspective = type
            self.seatVC = vc
        case "GiftAudienceViewController":
            let vc = segue.destination as! GiftAudienceViewController
            self.giftAudienceVC = vc
        case "BottomToolsViewController":
            guard let type = ALCenter.shared().liveSession?.role.value.type else {
                assert(false)
                return
            }
            
            let vc = segue.destination as! BottomToolsViewController
            vc.perspective = type
            self.bottomToolsVC = vc
        case "ChatViewController":
            let vc = segue.destination as! ChatViewController
            vc.cellColor = tintColor
            self.chatVC = vc
        default:
            break
        }
    }
    
    func activeSpeaker() {
        playerVM.activeSpeaker.subscribe(onNext: { [unowned self] (speaker) in
            guard let session = ALCenter.shared().liveSession else {
                    return
            }
            
            switch (speaker, session.owner.value) {
            case (.local, .localUser):
                self.ownerRenderView.startSpeakerAnimating()
            case (.other(agoraUid: let uid), .otherUser(let user)):
                if uid == user.agUId {
                    self.ownerRenderView.startSpeakerAnimating()
                } else {
                    fallthrough
                }
            default:
                self.seatVC?.activeSpeaker(speaker)
            }
        }).disposed(by: bag)
    }
    
    func extralLiveRole(_ session: LiveSession) {
        session.role.subscribe(onNext: { [unowned self] (local) in
            self.seatVC?.perspective = local.type
        }).disposed(by: bag)
    }
}

//MARK: - Specail MultiBroadcasters
extension MultiBroadcastersViewController {
    //MARK: - Live Seat
    func liveSeat() {
        guard let seatVC = self.seatVC else {
            assert(false)
            return
        }
        
        // Media
        seatVC.userRender.subscribe(onNext: { [unowned self] (viewUser) in
            self.playerVM.startRenderVideoStreamOf(user: viewUser.user, on: viewUser.view)
        }).disposed(by: bag)
        
        seatVC.userAudioSilence.subscribe(onNext: { [unowned self] (user) in
            guard let session = ALCenter.shared().liveSession,
                session.role.value.agUId == user.agUId else {
                    return
            }
            
            self.deviceVM.mic = user.permission.contains(.mic) ? .on : .off
        }).disposed(by: bag)
        
        // Live Seat List
        seatVM.list.bind(to: seatVC.seats).disposed(by: bag)
            
        // Live Seat Command
        seatVC.actionFire.subscribe(onNext: { [unowned self] (action) in
            guard let session = ALCenter.shared().liveSession else {
                return
            }
            
            switch action.command {
            // seat state
            case .release, .close:
                let handler: ((UIAlertAction) -> Void)? = { [unowned self] (_) in
                    let update = { [unowned self] in
                        self.seatVM.update(state: action.command == .release ? .empty : .close,
                                           index: action.seat.index) { [unowned self] (_) in
                                            self.showTextToast(text: "update seat fail")
                        }
                    }
                    
                    if action.command == .release {
                        update()
                    } else {
                        if let user = action.seat.user {
                            self.multiHostsVM.forceEndBroadcasting(user: user, on: action.seat.index, success: {
                                update()
                            }) { [unowned self] (_) in
                                self.showTextToast(text: "owner end broadcasting fail")
                            }
                        } else {
                            update()
                        }
                    }
                }
                
                let message = self.alertMessageOfSeatCommand(action.command,
                                                             with: action.seat.user?.info.name)
                
                self.showAlert(message: message,
                               action1: NSLocalizedString("Cancel"),
                               action2: NSLocalizedString("Confirm"),
                               handler2: handler)
            // owner
            case .invitation:
                self.presentInvitationList { [unowned self] (user) in
                    self.hiddenMaskView()
                    
                    let handler: ((UIAlertAction) -> Void)? = { [unowned self] (_) in
                        self.multiHostsVM.sendInvitation(to: user, on: action.seat.index) { [unowned self] (_) in
                            self.showTextToast(text: NSLocalizedString("Invite_Broadcasting_Fail"))
                        }
                    }
                    let message = self.alertMessageOfSeatCommand(action.command, with: user.info.name)
                    self.showAlert(message: message,
                                   action1: NSLocalizedString("Cancel"),
                                   action2: NSLocalizedString("Confirm"),
                                   handler2: handler)
                }
            case .ban, .unban, .forceToAudience:
                guard let user = action.seat.user else {
                    return
                }
                
                let handler: ((UIAlertAction) -> Void)? = { [unowned self] (_) in
                    guard let session = ALCenter.shared().liveSession else {
                        return
                    }
                    
                    if action.command == .ban {
                        session.muteAudio(user: action.seat.user!) { [unowned self] in
                            self.showTextToast(text: "mute user audio fail")
                        }
                    } else if action.command == .unban {
                        session.unmuteAudio(user: action.seat.user!) { [unowned self] in
                            self.showTextToast(text: "ummute user audio fail")
                        }
                    } else if action.command == .forceToAudience {
                        self.multiHostsVM.forceEndBroadcasting(user: user,
                                                               on: action.seat.index) { [unowned self] (_) in
                                                                self.showTextToast(text: "force user end broadcasting fail")
                        }
                    }
                }
                
                let message = self.alertMessageOfSeatCommand(action.command,
                                                             with: action.seat.user?.info.name)
                
                self.showAlert(message: message,
                               action1: NSLocalizedString("Cancel"),
                               action2: NSLocalizedString("Confirm"),
                               handler2: handler)
            // broadcster
            case .endBroadcasting:
                self.showAlert(message: NSLocalizedString("Confirm_End_Broadcasting"),
                               action1: NSLocalizedString("Cancel"),
                               action2: NSLocalizedString("Confirm")) { [unowned self] (_) in
                                guard let user = action.seat.user else {
                                    return
                                }
                                
                                self.multiHostsVM.endBroadcasting(seatIndex: action.seat.index, user: user, success: {
                                    guard let session = ALCenter.shared().liveSession else {
                                        assert(false)
                                        return
                                    }
                                    session.broadcasterToAudience()
                                })
                }
            // audience
            case .application:
                self.showAlert(message: NSLocalizedString("Confirm_Apply_For_Broadcasting"),
                               action1: NSLocalizedString("Cancel"),
                               action2: NSLocalizedString("Confirm")) { [unowned self] (_) in
                                self.multiHostsVM.sendApplication(by: session.role.value,
                                                                  for: action.seat.index) { [unowned self] (_) in
                                                                    self.showTextToast(text: "send application fail")
                                }
                }
            }
        }).disposed(by: bag)
    }
    
    //MARK: - User List
    func presentInvitationList(selected: ((LiveRole) -> Void)? = nil) {
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
        
        vc.inviteUser.subscribe(onNext: { (user) in
            if let selected = selected {
                selected(user)
            }
        }).disposed(by: bag)
        
        self.presentChild(vc,
                          animated: true,
                          presentedFrame: presentedFrame)
    }
}

private extension MultiBroadcastersViewController {
    // MARK: - Live Room
    func liveRoom(_ session: LiveSession) {
        let owner = session.owner
        ownerRenderView.cornerRadius(5)
        ownerRenderView.layer.masksToBounds = true
        ownerRenderView.imageView.isHidden = true
        ownerRenderView.backgroundColor = tintColor
        
        owner.subscribe(onNext: { [unowned self] (owner) in
            let images = ALCenter.shared().centerProvideImagesHelper()
            let user = owner.user
            self.ownerRenderView.imageView.image = images.getOrigin(index: user.info.imageIndex)
            self.ownerRenderView.label.text = user.info.name
            self.playerVM.startRenderVideoStreamOf(user: user,
                                                   on: self.ownerRenderView.renderView)
            
            self.ownerRenderView.imageView.isHidden = user.permission.contains(.camera)
            self.ownerRenderView.audioSilenceTag.isHidden = user.permission.contains(.mic)
        }).disposed(by: bag)
        
        self.roomLabel.text = NSLocalizedString("Live_Room") + ": "
        self.roomNameLabel.text = session.room.name
    }
    
    func multiHosts(_ session: LiveSession) {
        // owner
        multiHostsVM.receivedApplication.subscribe(onNext: { [unowned self] (application) in
            self.showAlert(message: "\"\(application.initiator.info.name)\" " + NSLocalizedString("Apply_For_Broadcasting"),
                           action1: NSLocalizedString("Reject"),
                           action2: NSLocalizedString("Confirm"),
                           handler1: { [unowned self] (_) in
                            self.multiHostsVM.reject(application: application)
            }) { [unowned self] (_) in
                self.multiHostsVM.accept(application: application)
            }
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
                self.showTextToast(text: chat.content.string)
            } else {
                let chat = Chat(name: user.info.name, text: " became a broadcaster")
                self.chatVM.newMessages([chat])
                self.showTextToast(text: chat.content.string)
            }
        }).disposed(by: bag)
        
        multiHostsVM.broadcasterBecameAudience.subscribe(onNext: { [unowned self] (user) in
            if DeviceAssistant.Language.isChinese {
                let chat = Chat(name: user.info.name, text: " 下麦")
                self.chatVM.newMessages([chat])
                self.showTextToast(text: chat.content.string)
            } else {
                let chat = Chat(name: user.info.name, text: " became a audience")
                self.chatVM.newMessages([chat])
                self.showTextToast(text: chat.content.string)
            }
        }).disposed(by: bag)
        
        let owner = session.owner.value
        
        multiHostsVM.invitationTimeout.subscribe(onNext: { [unowned self] (_) in
            guard owner.isLocal else {
                return
            }
            self.showTextToast(text: NSLocalizedString("User_Invitation_Timeout"))
        }).disposed(by: bag)
    }
}

private extension MultiBroadcastersViewController {
    func alertMessageOfSeatCommand(_ command: LiveSeatView.Command, with userName: String?) -> String {
        switch command {
        case .ban:
            if DeviceAssistant.Language.isChinese {
                return "禁止\"\(userName!)\"发言?"
            } else {
                return "Mute \(userName!)?"
            }
        case .unban:
            if DeviceAssistant.Language.isChinese {
                return "解除\"\(userName!)\"禁言?"
            } else {
                return "Unmute \(userName!)?"
            }
        case .forceToAudience:
            if DeviceAssistant.Language.isChinese {
                return "确定\"\(userName!)\"下麦?"
            } else {
                return "Stop \(userName!) hosting"
            }
        case .close:
            if DeviceAssistant.Language.isChinese {
                return "将关闭该麦位，如果该位置上有用户，将下麦该用户"
            } else {
                return "Block this position"
            }
        case .release:
            return NSLocalizedString("Seat_Release_Description")
        case .invitation:
            if DeviceAssistant.Language.isChinese {
                return "你是否要邀请\"\(userName!)\"上麦?"
            } else {
                return "Do you send a invitation to \(userName!)?"
            }
        default:
            assert(false)
            return ""
        }
    }
}
