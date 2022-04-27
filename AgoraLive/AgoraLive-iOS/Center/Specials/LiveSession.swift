//
//  LiveSession.swift
//  AgoraLive
//
//  Created by CavanSu on 2020/3/19.
//  Copyright © 2020 Agora. All rights reserved.
//

import UIKit
import RxSwift
import RxRelay
import AlamoClient
import AgoraRtcKit

struct VideoConfiguration {
    var resolution: CGSize = AgoraVideoDimension640x360
    var frameRate: AgoraVideoFrameRate = .fps15
    var bitRate: Int = AgoraVideoBitrateStandard
}

enum LiveType: Int {
    case single = 1, multi, pk, virtual, shopping
    
    var description: String {
        switch self {
        case .multi:
            return NSLocalizedString("Multi_Broadcasters")
        case .single:
            return NSLocalizedString("Single_Broadcaster")
        case .pk:
            return NSLocalizedString("PK_Live")
        case .virtual:
            return NSLocalizedString("Virtual_Live")
        case .shopping:
            return NSLocalizedString("Live_Shopping")
        }
    }
    
    static let list: [LiveType] = [.multi,
                                   .single,
                                   .pk,
                                   .virtual,
                                   .shopping]
}

class LiveSession: RTMObserver {
    enum Owner {
        case localUser(LiveRole), otherUser(LiveRole)
        
        var isLocal: Bool {
            switch self {
            case .localUser: return true
            case .otherUser: return false
            }
        }
        
        var user: LiveRole {
            switch self {
            case .otherUser(let user): return user
            case .localUser(let user): return user
            }
        }
    }
    
    private(set) var owner: BehaviorRelay<Owner>
    private(set) var rtcChannelReport: BehaviorRelay<ChannelReport>?
    private(set) var end = PublishRelay<()>()
    
    private(set) var room: Room
    private(set) var type: LiveType
    private(set) var role: BehaviorRelay<LiveLocalUser>
    
    var videoConfiguration: VideoConfiguration
    
    init(room: Room, videoConfiguration: VideoConfiguration, type: LiveType, owner: Owner, role: LiveLocalUser) {
        self.room = room
        self.videoConfiguration = videoConfiguration
        self.type = type
        self.owner = BehaviorRelay(value: owner)
        self.role = BehaviorRelay(value: role)
        super.init()
        self.observe()
    }
    
    deinit {
        print("deinit LiveSession")
    }
    
    typealias JoinedInfo = (room: Room, seatInfo: [StringAnyDic]?, giftAudience: [StringAnyDic]?, pkInfo: StringAnyDic?)
    
    static func create(roomName: String, videoConfiguration: VideoConfiguration, type: LiveType, ownerInfo: BasicUserInfo, success: ((LiveSession) -> Void)? = nil, fail: Completion = nil) {
        let url = URLGroup.liveCreate
        let event = RequestEvent(name: "live-session-create")
        let parameter: StringAnyDic = ["roomName": roomName,
                                       "type": type.rawValue]
        
        let task = RequestTask(event: event,
                               type: .http(.post, url: url),
                               timeout: .medium,
                               header: ["token": ALKeys.ALUserToken],
                               parameters: parameter)
        
        let successCallback: DicEXCompletion = { (json: ([String: Any])) throws in
            let roomId = try json.getStringValue(of: "data")
            
            let role = LiveLocalUser(type: .owner,
                                     info: ownerInfo,
                                     permission: [.camera, .mic, .chat],
                                     agUId: 0)
            
            let owner = Owner.localUser(role)
            let room = Room(name: roomName,
                            roomId: roomId,
                            imageURL: "",
                            personCount: 0,
                            owner: role)
            
            let session = LiveSession(room: room,
                                      videoConfiguration: videoConfiguration,
                                      type: type,
                                      owner: owner,
                                      role: role)
            
            if let success = success {
                success(session)
            }
        }
        let response = ACResponse.json(successCallback)
        
        let retry: ACErrorRetryCompletion = { (error: Error) -> RetryOptions in
            if let fail = fail {
                fail()
            }
            return .resign
        }
        
        let alamo = ALCenter.shared().centerProvideRequestHelper()
        alamo.request(task: task, success: response, failRetry: retry)
    }
    
    func join(success: ((JoinedInfo) throws -> Void)? = nil, fail: Completion = nil ) {
        let client = ALCenter.shared().centerProvideRequestHelper()
        let url = URLGroup.joinLive(roomId: self.room.roomId)
        let event = RequestEvent(name: "live-session-join")
        let task = RequestTask(event: event,
                               type: .http(.post, url: url),
                               timeout: .low,
                               header: ["token": ALKeys.ALUserToken])
        
        let response = ACResponse.json { [unowned self] (json) in
            let data = try json.getDataObject()
            
            // Local User
            let localUserJson = try data.getDictionaryValue(of: "user")
            let user = try LiveLocalUser(dic: localUserJson)
            self.role = BehaviorRelay(value: user)
            
            // Live Room
            let liveRoom = try data.getDictionaryValue(of: "room")
            try self.updateLiveRoomInfoWhenJoingWith(info: liveRoom)
            
            // join rtc, rtm channel
            ALKeys.AgoraRtcToken = try localUserJson.getStringValue(of: "rtcToken")
            
            let channel = try liveRoom.getStringValue(of: "channelName")
            let agUId = try localUserJson.getIntValue(of: "uid")
            let mediaKit = ALCenter.shared().centerProvideMediaHelper()
            self.setupPublishedVideoStream(self.videoConfiguration)
            
            mediaKit.join(channel: channel, token: ALKeys.AgoraRtcToken, streamId: agUId) { [unowned self] in
                mediaKit.channelReport.subscribe(onNext: { [weak self] (statistic) in
                    guard let strongSelf = self else {
                        return
                    }
                    strongSelf.rtcChannelReport = BehaviorRelay(value: statistic)
                }).disposed(by: self.bag)
            }
            
            let rtm = ALCenter.shared().centerProvideRTMHelper()
            
            // multiBroadcasters, virtualBroadcasters have seatInfo
            var seatInfo: [StringAnyDic]?
            if let info = try? liveRoom.getListValue(of: "coVideoSeats") {
                seatInfo = info
            }
            
            // only pkBroadcaster has pkInfo
            var pkInfo: StringAnyDic?
            if let info = try? liveRoom.getDictionaryValue(of: "pk") {
                pkInfo = info
            }
            
            let giftAudience = try? liveRoom.getListValue(of: "rankUsers")
            
            rtm.joinChannel(channel, success: { [weak self] in
                guard let strongSelf = self else {
                    return
                }
                
                guard let success = success else {
                    return
                }
                do {
                    try success((strongSelf.room, seatInfo, giftAudience, pkInfo))
                } catch {
                    if let fail = fail {
                        fail()
                    }
                }
            }) { [unowned mediaKit] (error) -> RetryOptions in
                mediaKit.leaveChannel()
                return .resign
            }
        }
        
        let retry: ACErrorRetryCompletion = { (error: Error) -> RetryOptions in
            if let fail = fail {
                fail()
            }
            return .resign
        }
        
        client.request(task: task, success: response, failRetry: retry)
    }
    
    func setupPublishedVideoStream(_ settings: VideoConfiguration) {
        let mediaKit = ALCenter.shared().centerProvideMediaHelper()
        
        mediaKit.setupPublishedVideoStream(resolution: settings.resolution,
                                           frameRate: settings.frameRate,
                                           bitRate: settings.bitRate)
    }
    
    func leave() {
        let mediaKit = ALCenter.shared().centerProvideMediaHelper()
        let rtm = ALCenter.shared().centerProvideRTMHelper()
        let client = ALCenter.shared().centerProvideRequestHelper()
        mediaKit.leaveChannel()
        try? mediaKit.capture.video(.off)
        mediaKit.capture.audio = .off
        
        rtm.leaveChannel()
        
        let event = RequestEvent(name: "live-session-leave")
        let url = URLGroup.leaveLive(roomId: room.roomId)
        let task = RequestTask(event: event, type: .http(.post, url: url), header: ["token": ALKeys.ALUserToken])
        client.request(task: task)
    }
}

extension LiveSession {
    @discardableResult func audienceToBroadcaster() -> LiveRole {
        let audience = self.role.value
        
        let media = ALCenter.shared().centerProvideMediaHelper()
        media.capture.audio = .on
        try! media.capture.video(.on)
        var permission = audience.permission
        permission.insert(.camera)
        permission.insert(.mic)
        
        let role = LiveLocalUser(type: .broadcaster,
                                 info: audience.info,
                                 permission: permission,
                                 agUId: audience.agUId,
                                 giftRank: audience.giftRank)
        
        self.role.accept(role)
        self.setupPublishedVideoStream(videoConfiguration)
        return role
    }
    
    @discardableResult func broadcasterToAudience() -> LiveRole {
        let broadcaster = self.role.value
        
        let media = ALCenter.shared().centerProvideMediaHelper()
        media.capture.audio = .off
        try! media.capture.video(.off)
        var permission = broadcaster.permission
        permission.remove(.camera)
        permission.remove(.mic)
        
        let role = LiveLocalUser(type: .audience,
                                 info: broadcaster.info,
                                 permission: permission,
                                 agUId: broadcaster.agUId,
                                 giftRank: broadcaster.giftRank)
        
        self.role.accept(role)
        return role
    }
    
    func muteAudio(user: LiveRole, fail: Completion) {
        let parameters: StringAnyDic = ["enableAudio": 0,
                                        "enableVideo": user.permission.contains(.camera) ? 1 : 0,
                                        "enableChat": user.permission.contains(.chat) ? 1 : 0]
       
        userMediaOperation(user: user,
                           parameters: parameters,
                           fail: fail)
    }
    
    func unmuteAudio(user: LiveRole, fail: Completion) {
        let parameters: StringAnyDic = ["enableAudio": 1,
                                        "enableVideo": user.permission.contains(.camera) ? 1 : 0,
                                        "enableChat": user.permission.contains(.chat) ? 1 : 0]
        userMediaOperation(user: user,
                           parameters: parameters,
                           fail: fail)
    }
}

private extension LiveSession {
    func updateLiveRoomInfoWhenJoingWith(info: StringAnyDic) throws {
        // Live room owner
        var ownerJson = try info.getDictionaryValue(of: "owner")
        ownerJson["avatar"] = "Fake"
        ownerJson["role"] = 1
        
        let ownerObj = try LiveRoleItem(dic: ownerJson)
        
        if ownerObj.info == self.role.value.info {
            self.owner.accept(.localUser(ownerObj))
        } else {
            self.owner.accept(.otherUser(ownerObj))
        }
        
        // Live type check
        let liveType = try info.getEnum(of: "type", type: LiveType.self)
        
        guard self.type == liveType else {
            throw AGEError.fail("local live type is not equal to server live type")
        }
    }
    
    func userMediaOperation(user: LiveRole, parameters: StringAnyDic, success: Completion = nil, fail: Completion = nil) {
        let client = ALCenter.shared().centerProvideRequestHelper()
        let url = URLGroup.userCommand(userId: user.info.userId, roomId: room.roomId)
        let event = RequestEvent(name: "live-user-media-operation")
        
        let token = ["token": ALKeys.ALUserToken]
        let task = RequestTask(event: event,
                               type: .http(.post, url: url),
                               header: token,
                               parameters: parameters)
        let successCallback: DicEXCompletion = { (json) in
            try json.getCodeCheck()
            let isSuccess = try json.getBoolInfoValue(of: "data")
            if isSuccess, let callback = success {
                callback()
            } else if !isSuccess, let callback = fail {
                callback()
            }
        }
        let response = ACResponse.json(successCallback)
        
        let fail: ACErrorRetryCompletion = { (_) in
            if let callback = fail {
                callback()
            }
            return .resign
        }
        
        client.request(task: task, success: response, failRetry: fail)
    }
}

private extension LiveSession {
    func observe() {
        let rtm = ALCenter.shared().centerProvideRTMHelper()
        rtm.addReceivedChannelMessage(observer: self.address) { [weak self] (json) in
            guard let cmd = try? json.getEnum(of: "cmd", type: ALChannelMessage.AType.self) else {
                return
            }
            
            guard let strongSelf = self else {
                return
            }
            
            switch cmd {
            case .liveEnd:
                strongSelf.end.accept(())
            case .owner:
                let data = try json.getDataObject()
                let owner = try LiveRoleItem(dic: data)
                
                if strongSelf.owner.value.isLocal {
                    strongSelf.owner.accept(.localUser(owner))
                } else {
                    strongSelf.owner.accept(.otherUser(owner))
                }
            default:
                break
            }
        }
    }
}
