//
//  LiveSeatViewController.swift
//  AgoraLive
//
//  Created by CavanSu on 2020/3/23.
//  Copyright © 2020 Agora. All rights reserved.
//

import UIKit
import RxSwift
import RxRelay
import AGEVideoLayout

class InviteButton: UIButton {
    override func titleRect(forContentRect contentRect: CGRect) -> CGRect {
        let w: CGFloat = self.bounds.width
        let h: CGFloat = 17
        let x: CGFloat = 0.0
        let y: CGFloat = (self.bounds.height * 0.5) + 14.5
        return CGRect(x: x, y: y, width: w, height: h)
    }
    
    override func imageRect(forContentRect contentRect: CGRect) -> CGRect {
        let wh: CGFloat = 38.0
        let x: CGFloat = (self.bounds.width - wh) * 0.5
        let y: CGFloat = (self.bounds.height * 0.5) - (38 - 7.5)
        return CGRect(x: x, y: y, width: wh, height: wh)
    }
    
    override init(frame: CGRect) {
        super.init(frame: frame)
        titleLabel?.textAlignment = .center
        titleLabel?.textColor = UIColor(hexString: "#FFFFFF-0.2")
        titleLabel?.font = UIFont.systemFont(ofSize: 12)
    }
    
    required init?(coder: NSCoder) {
        fatalError("init(coder:) has not been implemented")
    }
}

class CommandCell: UICollectionViewCell {
    private lazy var underLine: CALayer = {
        let layer = CALayer()
        layer.backgroundColor = UIColor(hexString: "#FFFFFF").cgColor
        self.contentView.layer.addSublayer(layer)
        return layer
    }()
    
    var needUnderLine: Bool = true
    
    override init(frame: CGRect) {
        super.init(frame: frame)
        self.backgroundColor = UIColor(hexString: "#0088EB")
    }
    
    required init?(coder: NSCoder) {
        super.init(coder: coder)
        self.backgroundColor = UIColor(hexString: "#0088EB")
    }
    
    lazy var titleLabel: UILabel = {
        let label = UILabel(frame: CGRect.zero)
        label.textColor = UIColor(hexString: "#FFFFFF")
        label.textAlignment = .center
        label.font = UIFont.systemFont(ofSize: 14)
        self.contentView.addSubview(label)
        return label
    }()
    
    override func layoutSubviews() {
        super.layoutSubviews()
        self.titleLabel.frame = CGRect(x: 0,
                                       y: 0,
                                       width: self.bounds.width,
                                       height: self.bounds.height)
        
        if needUnderLine {
            self.underLine.frame = CGRect(x: 5,
                                          y: self.bounds.height - 1,
                                          width: self.bounds.width - 10,
                                          height: 1)
        } else {
            self.underLine.frame = CGRect.zero
        }
    }
}

class LiveSeatView: UIView {
    enum Command {
        // 禁麦， 解禁， 封麦，下麦， 解封， 邀请，
        case ban, unban, close, forceToAudience, release, invitation
        // 申请成为主播， 主播下麦
        case application, endBroadcasting
        
        var description: String {
            switch self {
            case .ban:                  return NSLocalizedString("Seat_Ban")
            case .unban:                return NSLocalizedString("Seat_Unban")
            case .forceToAudience:      return NSLocalizedString("End_Broadcasting")
            case .close:                return NSLocalizedString("Seat_Close")
            case .release:              return NSLocalizedString("Seat_Release")
            case .invitation:           return "invite"
            case .application:          return NSLocalizedString("Apply_For_Broadcasting")
            case .endBroadcasting:      return NSLocalizedString("End_Broadcasting")
            }
        }
    }
    
    fileprivate var popover = Popover(options: [.type(.down),
                                                .blackOverlayColor(UIColor.clear),
                                                .cornerRadius(5.0),
                                                .arrowSize(CGSize(width: 8, height: 4))])
    
    fileprivate var commands = BehaviorRelay(value: [Command]())
    
    fileprivate var renderView = LabelShadowRender()
    fileprivate var commandButton = UIButton()
    fileprivate var inviteButton = InviteButton(frame: CGRect.zero)
    fileprivate var closeButton = InviteButton()
    
    private let bag = DisposeBag()
    private var indexView = UILabel()
    private(set) var index: Int = 0 {
        didSet {
            indexView.text = "\(index + 1)"
        }
    }
    
    private(set) var commandFire = PublishRelay<Command>()
    
    private var commandsCollectionView = UICollectionView(frame: CGRect.zero,
                                                          collectionViewLayout: UICollectionViewFlowLayout())
    
    var perspective: LiveRoleType = .audience {
        didSet {
            switch perspective {
            case .audience:
                commandButton.isHidden = true
            case .broadcaster:
                commandButton.isHidden = false
            case .owner:
                commandButton.isHidden = false
            }
        }
    }
    
    init(index: Int, frame: CGRect) {
        super.init(frame: frame)
        self.index = index
        self.initViews()
    }
    
    required init?(coder: NSCoder) {
        super.init(frame: CGRect.zero)
        self.index = 0
        self.initViews()
    }
    
    func initViews() {
        self.backgroundColor = UIColor(red: 0, green: 0, blue: 0, alpha: 0.2)
        
        popover.arrowColor = UIColor(hexString: "#FFFFFF")
        
        indexView.textColor = UIColor.white
        indexView.font = UIFont.systemFont(ofSize: 11)
        indexView.textAlignment = .center
        indexView.backgroundColor = UIColor(red: 0, green: 0, blue: 0, alpha: 0.3)
        indexView.text = "\(index + 1)"
        
        renderView.label.font = UIFont.systemFont(ofSize: 16)
        commandButton.setImage(UIImage(named: "icon-more-gray"), for: .normal)
        
        inviteButton.setTitle(NSLocalizedString("Invite_Broadcasting"), for: .normal)
        inviteButton.setImage(UIImage(named: "icon-join"), for: .normal)
        inviteButton.rx.tap.subscribe(onNext: { [unowned self] in
            switch self.perspective {
            case .owner:
                self.commandFire.accept(.invitation)
            case .audience:
                self.commandFire.accept(.application)
            case .broadcaster:
                break
            }
        }).disposed(by: bag)
        
        closeButton.setTitle(NSLocalizedString("Seat_Close"), for: .normal)
        closeButton.setImage(UIImage(named: "icon-ban"), for: .normal)
        closeButton.rx.tap.subscribe(onNext: { [unowned self] in
            switch self.perspective {
            case .owner:
                self.commandFire.accept(.release)
            default:
                break
            }
        }).disposed(by: bag)
        
        // Commands CollectionView
        let layout = UICollectionViewFlowLayout()
        layout.itemSize = CGSize(width: 54.0, height: 34.0)
        layout.scrollDirection = .vertical
        layout.minimumLineSpacing = 0
        layout.minimumInteritemSpacing = 0
        
        commandsCollectionView.isScrollEnabled = false
        commandsCollectionView.setCollectionViewLayout(layout, animated: false)
        commandsCollectionView.register(CommandCell.self, forCellWithReuseIdentifier: "CommandCell")
        
        commandButton.rx.tap.subscribe(onNext: { [unowned self] in
            self.commandsCollectionView.frame = CGRect(x: 0,
                                                       y: 0,
                                                       width: 54,
                                                       height: 34 * self.commands.value.count)
            self.popover.show(self.commandsCollectionView,
                              fromView: self.commandButton)
        }).disposed(by: bag)
        
        commands.bind(to: commandsCollectionView.rx
            .items(cellIdentifier: "CommandCell",
                   cellType: CommandCell.self)) { (index, command, cell) in
                    cell.titleLabel.text = command.description
                    cell.needUnderLine = true
        }.disposed(by: bag)
        
        commandsCollectionView.rx
            .modelSelected(Command.self)
            .subscribe(onNext: { [unowned self] (command) in
                self.popover.dismiss()
                self.commandFire.accept(command)
        }).disposed(by: bag)
    }
    
    override func layoutSubviews() {
        super.layoutSubviews()
        renderView.frame = self.bounds
        self.addSubview(renderView)
        
        let x: CGFloat = 22.0
        inviteButton.frame = CGRect(x: x,
                                    y: 0,
                                    width: self.bounds.width - (x * 2),
                                    height: self.bounds.height)
        self.addSubview(inviteButton)
        
        closeButton.frame = CGRect(x: x,
                                   y: 0,
                                   width: self.bounds.width - (x * 2),
                                   height: self.bounds.height)
        self.addSubview(closeButton)
        
        let commandWidth: CGFloat = 22.0
        commandButton.frame = CGRect(x: self.bounds.width - commandWidth - 5,
                                     y: 0,
                                     width: commandWidth,
                                     height: 22.0)
        self.addSubview(commandButton)
        
        indexView.frame = CGRect(x: 5, y: 5, width: 14, height: 14)
        indexView.isCycle = true
        indexView.layer.masksToBounds = true
        self.addSubview(indexView)
    }
}

struct LiveSeatAction {
    var seat: LiveSeat
    var command: LiveSeatView.Command
}

class LiveSeatViewController: UIViewController {
    private let seatCount = 6
    private let bag = DisposeBag()
    
    private lazy var seatViews: [LiveSeatView] = {
        var temp = [LiveSeatView]()
        for i in 0 ..< seatCount {
            let seatView = LiveSeatView(index: i,
                                    frame: CGRect.zero)
            seatView.commandFire.subscribe(onNext: { [unowned self] (command) in
                let seat = self.seats.value[seatView.index]
                let action = LiveSeatAction(seat: seat, command: command)
                self.actionFire.accept(action)
            }).disposed(by: bag)
            temp.append(seatView)
        }
        return temp
    }()
    
    private(set) var seats: BehaviorRelay<[LiveSeat]>!
    
    private var contanerView: AGEVideoContainer {
        return self.view as! AGEVideoContainer
    }
    
    private(set) var userRender = PublishRelay<(view: UIView, user: LiveRoleItem)>()
    private(set) var userAudioSilence = PublishRelay<LiveRoleItem>()
    private(set) var actionFire = PublishRelay<LiveSeatAction>()
    
    var perspective: LiveRoleType = .audience
    
    override func viewDidLoad() {
        super.viewDidLoad()
        
        var temp = [LiveSeat]()
        for i in 0 ..< self.seatCount {
            let seat = LiveSeat(index: i, state: .empty)
            temp.append(seat)
        }
        
        seats = BehaviorRelay(value: temp)
        
        seats.subscribe(onNext: { (seats) in
            self.updateSeats(seats)
        }).disposed(by: bag)
        
        let layout = AGEVideoLayout(level: 0)
            .itemSize(.scale(CGSize(width: 0.3, height: 0.5)))

        self.contanerView.listCount { [unowned self] (level) -> Int in
            return self.seatCount
        }.listItem { [unowned self] (index) -> AGEView in
            return self.seatViews[index.item]
        }
        
        self.contanerView.backgroundColor = .clear
        self.contanerView.setLayouts([layout])
    }
    
    func activeSpeaker(_ speaker: Speaker) {
        var agoraUid: UInt
        
        switch speaker {
        case .local:
            guard let uid = ALCenter.shared().liveSession?.role.value.agUId else {
                return
            }
            agoraUid = UInt(uid)
        case .other(agoraUid: let uid):
            agoraUid = uid
        }
        
        for item in seats.value where item.user != nil {
            let seatView = self.seatViews[item.index - 1]
            if let user = item.user, user.agUId == agoraUid {
                seatView.renderView.startSpeakerAnimating()
            }
        }
    }
}

private extension LiveSeatViewController {
    func updateSeats(_ seats: [LiveSeat]) {
        guard seats.count == seatCount else {
            assert(false)
            return
        }
        
        for (index, item) in seats.enumerated() {
            let view = seatViews[index]
            view.popover.dismissWithoutAnimation()
            
            view.perspective = self.perspective
            
            switch item.state {
            case .empty:
                view.inviteButton.isHidden = false
                view.closeButton.isHidden = true
                view.renderView.isHidden = true
                view.renderView.audioSilenceTag.isHidden = true
                view.commands.accept([.close])
            case .close:
                view.closeButton.isHidden = false
                view.inviteButton.isHidden = true
                view.renderView.isHidden = true
                view.renderView.audioSilenceTag.isHidden = true
                view.commands.accept([.release])
            case .normal:
                view.closeButton.isHidden = true
                view.inviteButton.isHidden = true
                view.renderView.isHidden = false
                
                view.renderView.label.text = item.user!.info.name
                view.commands.accept([.ban, .forceToAudience, .close])
                
                // media
                guard let user = item.user else {
                    break
                }
                
                // audio
                view.renderView.audioSilenceTag.isHidden = user.permission.contains(.mic)
                userAudioSilence.accept(user)
                
                if !user.permission.contains(.mic) {
                    view.commands.accept([.unban, .forceToAudience, .close])
                }
                
                // video
                view.renderView.imageView.isHidden = user.permission.contains(.camera)
                view.renderView.imageView.image = ALCenter.shared().centerProvideImagesHelper().getOrigin(index: user.info.imageIndex)
                
                if user.permission.contains(.camera) {
                    userRender.accept((view.renderView.renderView, user))
                }
            }
            
            switch perspective {
            case .owner:
                view.inviteButton.setTitle(NSLocalizedString("Invite_Broadcasting"),
                                           for: .normal)
                view.commandButton.isHidden = false
            case .broadcaster:
                view.inviteButton.setTitle(NSLocalizedString("Apply_For_Broadcasting"),
                                           for: .normal)
                
                view.commandButton.isHidden = true
                
                guard let role = ALCenter.shared().liveSession?.role.value,
                    let user = item.user,
                    role.info == user.info else {
                        break
                }
                view.commandButton.isHidden = false
                view.commands.accept([.endBroadcasting])
            case .audience:
                view.inviteButton.setTitle(NSLocalizedString("Apply_For_Broadcasting"),
                                           for: .normal)
                view.commandButton.isHidden = true
            }
        }
    }
}
