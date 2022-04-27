//
//  RTMClient.swift
//  AGEManAGEr
//
//  Created by CavanSu on 2019/4/10.
//  Copyright © 2019 Agora. All rights reserved.
//

import Foundation
import RxSwift
import RxRelay
import AgoraRtmKit
import AlamoClient

class RTMClient: NSObject, AGELogBase {
    private lazy var kit: AgoraRtmKit = {
        let files = ALCenter.shared().centerProvideFilesGroup()
        let kit = AgoraRtmKit(appId: ALKeys.AgoraAppId, delegate: self)!
        let path = files.logs.folderPath + "/rtm.log"
        kit.setLogPath(path)
        kit.alMode()
        return kit
    }()
    
    private lazy var afterWorkers = [Int: AfterWorker]() // Int: Task id
    private lazy var requestQueue = RTMRequestQueue()
    private lazy var lock = NSObject()
    
    private lazy var loginWorkerId = Date.millisecondTimestamp
    private lazy var joinChannelId = Date.millisecondTimestamp
    
    private lazy var peerMessageObservers = [String: DicEXCompletion]()    // Object address
    private lazy var channelMessageObservers = [String: DicEXCompletion]() // Object address
    
    private(set) var joinedChannel: String?
    
    private let bag = DisposeBag()
    
    private var currentQueue: DispatchQueue {
        if Thread.isMainThread {
            return DispatchQueue.main
        } else {
            return DispatchQueue.main
        }
    }
    
    private var isNeedCache: Bool {
        return true
    }
    
    private(set) var connectStatus = BehaviorRelay(value: AGESocketState.disconnected)
    
    var logTube: LogTube
    
    init(logTube: LogTube) {
        self.logTube = logTube
    }
}

extension RTMClient: SocketProtocol {
    func connect(rtmId: String, token: String?, success: Completion, failRetry: ACErrorRetryCompletion) throws {
        kit.login(rtmId: rtmId, token: token, success: success) { [unowned self] (error) in
            guard let retry = failRetry else {
                return
            }
            
            let option = retry(error)
            
            switch option {
            case .retry(let time, _):
                let worker = self.worker(of: self.loginWorkerId)
                worker.perform(after: time, on: self.currentQueue, {
                    try? self.connect(rtmId: rtmId, token: token, success: success, failRetry: failRetry)
                })
            case .resign:
                break
            }
        }
    }
    
    func disconnect() {
        kit.logout(completion: nil)
    }
    
    func renew(token: String, fail: ErrorCompletion) {
        self.kit.renewToken(token, fail: fail)
    }
    
    func write(message: String, of event: ACRequestEvent, to: String, success: Completion = nil, fail: ErrorCompletion = nil) throws {
        kit.send(message: message, of: event, to: to, success: success, fail: fail)
    }
    
    func addReceivedPeerMessage(observer: String, subscribe: DicEXCompletion) {
        let text = String(format: "%p", observer)
        peerMessageObservers[text] = subscribe
    }
    
    func removeReceivedPeerMessage(observer: String) {
        let text = String(format: "%p", observer)
        peerMessageObservers.removeValue(forKey: text)
    }
    
    func addReceivedChannelMessage(observer: String, subscribe: DicEXCompletion) {
        let text = String(format: "%p", observer)
        channelMessageObservers[text] = subscribe
    }
    
    func removeReceivedChannelMessage(observer: String) {
        let text = String(format: "%p", observer)
        channelMessageObservers.removeValue(forKey: text)
    }
}

extension RTMClient: RTMChannelProtocol {
    func joinChannel(_ channel: String, success: Completion, failRetry: ACErrorRetryCompletion) {
        kit.joinChannel(channel, delegate: self, success: { [unowned self] in
            self.joinedChannel = channel
            
            if let success = success {
                success()
            }
        }) { (error) in
            guard let retry = failRetry else {
                return
            }
            
            let option = retry(error)
            
            switch option {
            case .retry(let time, _):
                let worker = self.worker(of: self.loginWorkerId)
                worker.perform(after: time, on: self.currentQueue, {
                    self.joinChannel(channel, success: success, failRetry: failRetry)
                })
            case .resign:
                break
            }
        }
    }
    
    func leaveChannel() {
        guard let channel = joinedChannel else {
            return
        }
        
        try? kit.leaveChannel(channel)
    }
    
    func writeChannel(message: String, of event: ACRequestEvent, success: Completion = nil, fail: ErrorCompletion) throws {
        guard let channel = joinedChannel else {
            return
        }
        
        do {
            let rtmChannel = try kit.getChannel(id: channel)
            rtmChannel.send(message: message, of: event, success: success, fail: fail)
        } catch {
            log(error: error, extra: "get channel fail")
        }
    }
}

extension RTMClient: RequestClientProtocol {
    func request(task: ACRequestTaskProtocol, responseOnMainQueue: Bool = true, success: ACResponse?, failRetry: ACErrorRetryCompletion) {
        do {
            try prepareRequest(task: task, success: success) { [unowned self] (error) in
                self.log(error: error)
                guard let retry = failRetry else {
                    return
                }
                let option = retry(error)
                
                switch option {
                case .retry(let time, newTask: _):
                    let worker = self.worker(of: task.id)
                    worker.perform(after: time, on: self.currentQueue) {
                        self.request(task: task, success: success, failRetry: failRetry)
                    }
                case .resign:
                    self.afterWorkers.removeValue(forKey: task.id)
                    break
                }
            }
        } catch {
            log(error: AGEError.unknown(), extra: task.event.description)
        }
    }

    func upload(task: ACUploadTaskProtocol, responseOnMainQueue: Bool = true, success: ACResponse?, failRetry: ACErrorRetryCompletion) {
        if let retry = failRetry {
            let error = ACError.fail("rtm doesn't support upload yet")
            _ = retry(error)
        }
    }
}

// MARK: Request
private extension RTMClient {
    @discardableResult func prepareRequest(task: ACRequestTaskProtocol, success: ACResponse?, fail: ErrorCompletion) throws -> Int {
        let failHandle = { (error: Error) in
            if let fail = fail {
                fail(error)
            }
        }
        
        try request(task: task, sendSuccess: {
            let item = RTMQueueItem(event: task.event, success: success, fail: failHandle)
            self.pushRequestItemToQueue(item, timeout: task.timeout, id: task.id)
        }, sendFail: failHandle)

        return task.id
    }

    func request(task: ACRequestTaskProtocol, sendSuccess: Completion, sendFail: ErrorCompletion) throws {
        guard let parameters = task.parameters else {
            throw AGEError.valueNil("paraAGEers")
        }
        
        var remote: String
        let event = task.event
        let id = task.id
        
        let text = try parameters.jsonString()
        
        switch task.requestType {
        case .http:
            throw AGEError.fail("rtm doesn't support http request")
        case .socket(peer: let peer):
            remote = peer
        }
        
        kit.send(message: text, of: event, to: remote, success: sendSuccess, fail: sendFail)
    }

    func pushRequestItemToQueue(_ item: RTMQueueItem, timeout: ACRequestTimeout, id: Int) {
        requestQueue.push(item, id: id)
        let afterWorker = worker(of: id)
        let time = timeout.value
        afterWorker.perform(after: time, on: currentQueue) { [unowned self] in
            if let item = self.requestQueue.pop(id: id), let timeout = item.fail {
                timeout(AGEError.timeout(duration: time,
                                         extra: "event: \(item.event.description), requestId: \(id)"))
            }
            self.afterWorkers.removeValue(forKey: id)
        }
    }

    func worker(of id: Int) -> AfterWorker {
        var work: AfterWorker
        if let tWork = self.afterWorkers[id] {
            work = tWork
        } else {
            work = AfterWorker()
        }
        return work
    }
}

// MARK: Received
private extension RTMClient {
    func received(jsonString: String, type: RTMmessageType) {
        log(info: "received rtm json string: \(jsonString)", extra: "type: \(type.description)")
        
        do {
            var json = try jsonString.json()
            var observers: [String: DicEXCompletion]
            switch type {
            case .peer(let agoraUid):
                var data = try json.getDataObject()
                data["agoraUid"] = agoraUid
                json["data"] = data
                observers = peerMessageObservers
            case .channel:
                observers = channelMessageObservers
            }
            
            for (_, callback) in observers {
                if let cb = callback {
                    try cb(json)
                }
            }
            // task id, call back
        } catch let error {
            log(error: error)
        }
    }
}

// MARK: - AgoraRtmDelegate
extension RTMClient: AgoraRtmDelegate {
    func rtmKit(_ kit: AgoraRtmKit, messageReceived message: AgoraRtmMessage, fromPeer peerId: String) {
        let index = peerId.index(peerId.endIndex, offsetBy: -(peerId.count - 6))
        let new = peerId.suffix(from: index)
        received(jsonString: message.text, type: .peer(Int(new)!))
    }

    func rtmKit(_ kit: AgoraRtmKit, connectionStateChanged state: AgoraRtmConnectionState, reason: AgoraRtmConnectionChangeReason) {
        log(info: "rtmkit connect state: \(state.description), reason: \(reason.description)")
        self.connectStatus.accept(state)
    }

    func rtmKitTokenDidExpire(_ kit: AgoraRtmKit) {
        log(info: "rtmkit token expire")
    }
}

// MARK: - AgoraRtmChannelDelegate
extension RTMClient: AgoraRtmChannelDelegate {
    func channel(_ channel: AgoraRtmChannel, messageReceived message: AgoraRtmMessage, from member: AgoraRtmMember) {
        received(jsonString: message.text, type: .channel)
    }

    func channel(_ channel: AgoraRtmChannel, memberJoined member: AgoraRtmMember) {
        
    }
    
    func channel(_ channel: AgoraRtmChannel, memberLeft member: AgoraRtmMember) {
        
    }
}

private extension RTMClient {
    func log(info: String, funcName: String = #function, extra: String? = nil) {
        let className = RTMClient.self
        logOutputInfo(info, extra: extra, className: className, funcName: funcName)
    }

    func log(error: Error, funcName: String = #function, extra: String? = nil) {
        let className = RTMClient.self
        logOutputError(error, extra: extra, className: className, funcName: funcName)
    }
}
