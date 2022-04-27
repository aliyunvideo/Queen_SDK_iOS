//
//  PKVM.swift
//  AgoraLive
//
//  Created by CavanSu on 2020/4/13.
//  Copyright © 2020 Agora. All rights reserved.
//

import UIKit
import RxSwift
import RxRelay
import AlamoClient

struct Battle: TimestampModel {
    var id: Int
    var timestamp: TimeInterval
    var initatorRoom: Room
    var receiverRoom: Room
    
    init(id: Int, initatorRoom: Room, receiverRoom: Room) {
        self.id = id
        self.initatorRoom = initatorRoom
        self.receiverRoom = receiverRoom
        self.timestamp = NSDate().timeIntervalSince1970
    }
}

struct PKInfo {
    struct RemoteRoom {
        var roomId: String
        var channel: String
        var owner: LiveRole
        
        init(dic: StringAnyDic) throws {
            let roomId = try dic.getStringValue(of: "roomId")
            let channel = try dic.getStringValue(of: "channelName")
            let ownerJson = try dic.getDictionaryValue(of: "owner")
            let owner = try LiveRoleItem(dic: ownerJson)
            
            self.roomId = roomId
            self.channel = channel
            self.owner = owner
        }
    }
    
    var remoteRoom: RemoteRoom
    var startTime: Int
    var countDown: Int
    var localRank: Int
    var remoteRank: Int
}

enum PKResult: Int {
    case lose = 0, win = 1, draw = 2
}

enum PKEvent {
    case start(MediaRelayConfiguration), end(PKResult), rankChanged(local: Int, remote: Int)
}

enum PKState {
    case none, inviting, isBeingInvited, duration(PKInfo)
    
    var isDuration: Bool {
        switch self {
        case .duration: return true
        default:        return false
        }
    }
    
    var pkInfo: PKInfo? {
        switch self {
        case .duration(let info): return info
        default:                  return nil
        }
    }
}

struct MediaRelayConfiguration {
    var currentId: Int
    var currentChannelName: String
    var cuurentChannelToken: String
    var myIdOnRemoteChannel: Int
    var myTokenOnRemoteChannel: String
    
    var remoteChannelName: String
    var remoteIdOnCurrentChannel: Int
    
    init(dic: StringAnyDic) throws {
        let local = try dic.getDictionaryValue(of: "local")
        let proxy = try dic.getDictionaryValue(of: "proxy")
        let remote = try dic.getDictionaryValue(of: "remote")
        
        self.currentId = try local.getIntValue(of: "uid")
        self.currentChannelName = try local.getStringValue(of: "channelName")
        self.cuurentChannelToken = try local.getStringValue(of: "token")
        
        self.myIdOnRemoteChannel = try proxy.getIntValue(of: "uid")
        self.myTokenOnRemoteChannel = try proxy.getStringValue(of: "token")
    
        self.remoteChannelName = try proxy.getStringValue(of: "channelName")
        self.remoteIdOnCurrentChannel = try remote.getIntValue(of: "uid")
    }
}

fileprivate enum RelayState {
    case none, duration
}

class PKVM: RTMObserver {
    fileprivate var relayState = RelayState.none
    private(set) var mediaRelayConfiguration: MediaRelayConfiguration?
    private var room: Room
    private var type: LiveType
    
    let invitationQueue = TimestampQueue(name: "pk-invitation")
    let applicationQueue = TimestampQueue(name: "pk-application")
    
    let invitingRoomList = BehaviorRelay(value: [Room]())
    let applyingRoomList = BehaviorRelay(value: [Room]())
    
    let availableRooms = BehaviorRelay(value: [Room]())
    
    let requestError = PublishRelay<String>()
    
    let receivedInvitation = PublishRelay<Battle>()
    let invitationIsByRejected = PublishRelay<Battle>()
    let invitationIsByAccepted = PublishRelay<Battle>()
    let invitationTimeout = PublishRelay<Battle>()
    
    let state = BehaviorRelay(value: PKState.none)
    let event = PublishRelay<PKEvent>()
    
    init(room: Room, type: LiveType, state: StringAnyDic) throws {
        self.type = type
        self.room = room
        super.init()
        try self.parseJson(dic: state)
        self.observe()
    }
    
    deinit {
        #if !RELEASE
        print("deinit PKVM")
        #endif
    }
    
    func sendInvitationTo(room: Room) {
        request(type: 1, roomId: self.room.roomId, to: room.roomId, success: { [weak self] (json) in
            guard let strongSelf = self else {
                return
            }
            
            let id = try json.getIntValue(of: "data")
            let invitation = Battle(id: id,
                                    initatorRoom: strongSelf.room,
                                    receiverRoom: room)
            strongSelf.invitationQueue.append(invitation)
        }) { [unowned self] (_) in
            self.requestError.accept("pk invitation fail")
        }
    }
    
    func accept(invitation: Battle) {
        request(type: 2, roomId: room.roomId, to: invitation.initatorRoom.roomId, success: { (json) in
            self.applicationQueue.remove(invitation)
        }) { [unowned self] (_) in
            self.requestError.accept("pk accept fail")
        }
    }
    
    func reject(invitation: Battle) {
        request(type: 3, roomId: room.roomId, to: invitation.initatorRoom.roomId, success: { (json) in
            self.applicationQueue.remove(invitation)
        }) { [unowned self] (_) in
            self.requestError.accept("pk reject fail")
        }
    }
}

private extension PKVM {
    func request(type: Int, roomId: String, to destinationRoomId: String, success: DicEXCompletion = nil, fail: ErrorCompletion) {
        let client = ALCenter.shared().centerProvideRequestHelper()
        let task = RequestTask(event: RequestEvent(name: "pk-action: \(type)"),
                               type: .http(.post, url: URLGroup.pkLiveBattle(roomId: roomId)),
                               timeout: .medium,
                               header: ["token": ALKeys.ALUserToken],
                               parameters: ["roomId": destinationRoomId, "type": type])
        client.request(task: task, success: ACResponse.json({ (json) in
            if let success = success {
                try success(json)
            }
        })) { (error) -> RetryOptions in
            if let fail = fail {
                fail(error)
            }
            return .resign
        }
    }
    
    func observe() {
        let rtm = ALCenter.shared().centerProvideRTMHelper()
        
        rtm.addReceivedChannelMessage(observer: self.address) { [weak self] (json) in
            guard let strongSelf = self else {
                return
            }
            
            guard let cmd = try? json.getEnum(of: "cmd", type: ALChannelMessage.AType.self) else {
                return
            }
            
            guard cmd == .pkEvent else  {
                return
            }
            
            let data = try json.getDataObject()
            try strongSelf.parseJson(dic: data)
        }
        
        rtm.addReceivedPeerMessage(observer: self.address) { [weak self] (json) in
            guard let strongSelf = self else {
                return
            }
            
            guard let cmd = try? json.getEnum(of: "cmd", type: ALPeerMessage.AType.self) else {
                return
            }
            
            guard cmd == .pk else  {
                return
            }
            
            let data = try json.getDataObject()
            let type = try data.getIntValue(of: "type")
            let id = try data.getIntValue(of: "processId")
            let room = try data.getDictionaryValue(of: "fromRoom")
            let remoteRoom = try Room(dic: room)
            
            // 1.邀请pk 2接受pk 3拒绝pk 4超时
            switch type {
            case 1:
                let battle = Battle(id: id, initatorRoom: remoteRoom, receiverRoom: strongSelf.room)
                strongSelf.applicationQueue.append(battle)
                strongSelf.receivedInvitation.accept(battle)
            case 2:
                let battle = Battle(id: id, initatorRoom: strongSelf.room, receiverRoom: remoteRoom)
                strongSelf.invitationIsByAccepted.accept(battle)
            case 3:
                let battle = Battle(id: id, initatorRoom: strongSelf.room, receiverRoom: remoteRoom)
                strongSelf.invitationQueue.remove(battle)
                strongSelf.invitationIsByRejected.accept(battle)
            case 4:
                let battle = Battle(id: id, initatorRoom: strongSelf.room, receiverRoom: remoteRoom)
                strongSelf.invitationQueue.remove(battle)
                strongSelf.applicationQueue.remove(battle)
            default:
                break
            }
        }
        
        invitationQueue.queueChanged.subscribe(onNext: { [unowned self] (list) in
            guard let tList = list as? [Battle] else {
                return
            }
            
            let rooms = tList.map { (invitation) -> Room in
                return invitation.receiverRoom
            }
            
            self.invitingRoomList.accept(rooms)
        }).disposed(by: bag)
                
        applicationQueue.queueChanged.subscribe(onNext: { [unowned self] (list) in
            guard let tList = list as? [Battle] else {
                return
            }
            
            let rooms = tList.map { (invitation) -> Room in
                return invitation.initatorRoom
            }
            
            self.applyingRoomList.accept(rooms)
        }).disposed(by: bag)
    }
    
    func parseJson(dic: StringAnyDic) throws {
        guard let session = ALCenter.shared().liveSession else {
            return
        }
        
        let owner = session.owner
        
        // Event
        if let eventInt = try? dic.getIntValue(of: "event") {
            var event: PKEvent
            
            switch eventInt {
            case 0:
                let result = try dic.getEnum(of: "result", type: PKResult.self)
                event = .end(result)
                guard owner.value.isLocal else {
                    break
                }
                stopRelayingMediaStream()
            case 1:
                let relayConfig = try dic.getDictionaryValue(of: "relayConfig")
                let configuration = try MediaRelayConfiguration(dic: relayConfig)
                event = .start(configuration)
                self.mediaRelayConfiguration = configuration
                guard owner.value.isLocal else {
                    break
                }
                startRelayingMediaStream(configuration)
            case 2:
                let local = try dic.getIntValue(of: "localRank")
                let remote = try dic.getIntValue(of: "remoteRank")
                event = .rankChanged(local: local, remote: remote)
                self.event.accept(event)
                return
            default:
                assert(false)
                return
            }
            
            self.event.accept(event)
        } else if let relayConfig = try? dic.getDictionaryValue(of: "relayConfig"),
            room.owner.info == ALCenter.shared().centerProvideLocalUser().info.value  {
            let info = try MediaRelayConfiguration(dic: relayConfig)
            startRelayingMediaStream(info)
        }
        
        // State
        let stateInt = try dic.getIntValue(of: "state")
        var state: PKState
        
        switch stateInt {
        case 0:
            state = .none
        case 1:
            let relayConfig = try dic.getDictionaryValue(of: "relayConfig")
            let configuration = try MediaRelayConfiguration(dic: relayConfig)
            
            let roomJson = try dic.getDictionaryValue(of: "remoteRoom")
            var room = try PKInfo.RemoteRoom(dic: roomJson)
            var remoteOwner = room.owner
            remoteOwner.agUId = configuration.remoteIdOnCurrentChannel
            room.owner = remoteOwner
            
            let startTime = try dic.getIntValue(of: "startTime")
            let countDown = try dic.getIntValue(of: "countDown")
            let localRank = try dic.getIntValue(of: "localRank")
            let remoteRank = try dic.getIntValue(of: "remoteRank")
            let info = PKInfo(remoteRoom: room,
                              startTime: startTime,
                              countDown: countDown,
                              localRank: localRank,
                              remoteRank: remoteRank)
            state = .duration(info)
        case 2:
            state = .inviting
        case 3:
            state = .isBeingInvited
        default:
            assert(false)
            return
        }
        
        self.state.accept(state)
    }
}

private extension PKVM {
    func startRelayingMediaStream(_ info: MediaRelayConfiguration) {
        let media = ALCenter.shared().centerProvideMediaHelper()
        
        let currentToken = info.cuurentChannelToken
        let currentChannel = info.currentChannelName
        let otherChannel = info.remoteChannelName
        let otherToken = info.myTokenOnRemoteChannel
        let otherUid = info.myIdOnRemoteChannel
        media.startRelayingMediaStreamOf(currentChannel: currentChannel,
                                         currentSourceToken: currentToken,
                                         to: otherChannel,
                                         with: otherToken,
                                         otherChannelUid: UInt(otherUid))
    }
    
    func stopRelayingMediaStream() {
        let media = ALCenter.shared().centerProvideMediaHelper()
        media.stopRelayingMediaStream()
    }
}

extension PKVM {
    func fetch(count: Int = 10, success: Completion = nil, fail: Completion = nil) {
        guard let lastRoom = self.availableRooms.value.last else {
            return
        }
        
        let client = ALCenter.shared().centerProvideRequestHelper()
        let parameters: StringAnyDic = ["nextId": lastRoom.roomId,
                                        "count": count,
                                        "type": type.rawValue,
                                        "pkState": 0]
        
        let url = URLGroup.roomPage
        let event = RequestEvent(name: "room-page")
        let task = RequestTask(event: event,
                               type: .http(.get, url: url),
                               timeout: .low,
                               header: ["token": ALKeys.ALUserToken],
                               parameters: parameters)
        
        let successCallback: DicEXCompletion = { [weak self] (json: ([String: Any])) in
            guard let strongSelf = self else {
                return
            }
            
            let object = try json.getDataObject()
            let jsonList = try object.getValue(of: "list", type: [StringAnyDic].self)
            let list = try [Room](dicList: jsonList).filter({ [unowned strongSelf] (room) -> Bool in
                return room.roomId != strongSelf.room.roomId
            })
            
            var new = strongSelf.availableRooms.value
            new.append(contentsOf: list)
            strongSelf.availableRooms.accept(new)
            
            if let success = success {
                success()
            }
        }
        let response = ACResponse.json(successCallback)
        
        let retry: ACErrorRetryCompletion = { (error: Error) -> RetryOptions in
            if let fail = fail {
                fail()
            }
            return .resign
        }
        
        client.request(task: task, success: response, failRetry: retry)
    }
    
    func refetch(success: Completion = nil, fail: Completion = nil) {
        let client = ALCenter.shared().centerProvideRequestHelper()
        
        let currentCount = availableRooms.value.count <= 10 ? 10 : availableRooms.value.count
        let parameters: StringAnyDic = ["count": currentCount,
                                        "type": type.rawValue,
                                        "pkState": 0]
        
        let url = URLGroup.roomPage
        let event = RequestEvent(name: "room-page-refetch")
        let task = RequestTask(event: event,
                               type: .http(.get, url: url),
                               timeout: .low,
                               header: ["token": ALKeys.ALUserToken],
                               parameters: parameters)
        
        let successCallback: DicEXCompletion = { [weak self] (json: ([String: Any])) in
            guard let strongSelf = self else {
                return
            }
            
            try json.getCodeCheck()
            let object = try json.getDataObject()
            let jsonList = try object.getValue(of: "list", type: [StringAnyDic].self)
            let list = try [Room](dicList: jsonList).filter({ [unowned strongSelf] (room) -> Bool in
                return room.roomId != strongSelf.room.roomId
            })
            
            strongSelf.availableRooms.accept(list)
            
            if let success = success {
                success()
            }
        }
        let response = ACResponse.json(successCallback)
        
        let retry: ACErrorRetryCompletion = { (error: Error) -> RetryOptions in
            if let fail = fail {
                fail()
            }
            return .resign
        }
        
        client.request(task: task, success: response, failRetry: retry)
    }
}

fileprivate extension Array where Element == Room {
    init(dicList: [StringAnyDic]) throws {
        var array = [Room]()
        for item in dicList {
            let room = try Room(dic: item)
            array.append(room)
        }
        self = array
    }
}
