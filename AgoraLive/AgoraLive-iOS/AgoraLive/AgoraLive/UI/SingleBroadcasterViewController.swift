//
//  SingleBroadcasterViewController.swift
//  AgoraLive
//
//  Created by CavanSu on 2020/4/13.
//  Copyright © 2020 Agora. All rights reserved.
//

import UIKit
import RxSwift
import RxRelay

class SingleBroadcasterViewController: MaskViewController, LiveViewController {
    @IBOutlet weak var ownerView: IconTextView!
    @IBOutlet weak var renderView: UIView!
    
    // LiveViewController
    var tintColor = UIColor(red: 0,
                            green: 0,
                            blue: 0,
                            alpha: 0.6)
    
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
        print("deinit SingleBroadcasterViewController")
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
        liveRoom(session: session)
        bottomTools(session)
        
        audience()
        chatList()
        gift()
        chatInput()
        musicList()
        netMonitor()
        
        superResolution()
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
        default:
            break
        }
    }
}

extension SingleBroadcasterViewController {
    // MARK: - Live Room
    func liveRoom(session: LiveSession) {
        ownerView.offsetLeftX = -13
        ownerView.offsetRightX = 5
        ownerView.label.textColor = .white
        ownerView.label.font = UIFont.systemFont(ofSize: 11)
        
        session.owner.subscribe(onNext: { [unowned self] (owner) in
            let images = ALCenter.shared().centerProvideImagesHelper()
            let user = owner.user
            self.ownerView.label.text = user.info.name
            self.ownerView.imageView.image = images.getHead(index: user.info.imageIndex)
            self.playerVM.startRenderVideoStreamOf(user: user,
                                                   on: self.renderView)
        }).disposed(by: bag)
    }
    
    func superResolution() {
        bottomToolsVC?.superRenderButton.rx.tap.subscribe(onNext: { [unowned self] () in
            guard let session = ALCenter.shared().liveSession else {
                assert(false)
                return
            }
            
            guard let vc = self.bottomToolsVC else {
                assert(false)
                return
            }
            
            vc.superRenderButton.isSelected.toggle()
            
            if vc.superRenderButton.isSelected {
                let view = TagImageTextToast(frame: CGRect(x: 0, y: 300, width: 181, height: 44.0), filletRadius: 8)
                view.text = NSLocalizedString("Super_Resolution_Enabled")
                view.tagImage = UIImage(named: "icon-done")
                self.showToastView(view, duration: 1.0)
            }
            
            switch session.owner.value {
            case .otherUser(let remote):
                self.playerVM.enhance(vc.superRenderButton.isSelected ? .on : .off,
                                      videoStreamOf: remote)
            default:
                assert(false)
                break
            }
        }).disposed(by: bag)
    }
}
