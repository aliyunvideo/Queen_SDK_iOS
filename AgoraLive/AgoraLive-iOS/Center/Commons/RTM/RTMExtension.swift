//
//  RTMExtension.swift
//  AGECenter
//
//  Created by CavanSu on 2019/6/23.
//  Copyright © 2019 Agora. All rights reserved.
//

import Foundation
import AgoraRtmKit
import AlamoClient

typealias AGESocketState = AgoraRtmConnectionState

extension AgoraRtmKit: AGELogBase {
    var logTube: LogTube {
        get {
            return ALCenter.shared().centerProvideLogTubeHelper()
        }
        set {
        }
    }
    
    func setLogPath(_ path: String) {
        let parameters = "{\"rtm.log_file\": \"\(path)\"}"
        setParameters(parameters)
    }
    
    func alMode() {
        #if DEBUG
        setParameters("{\"rtm.log_filter\": 0xffff}")
        setParameters("{\"rtm.msg.report_enabled\": \(true)}")
        setParameters("{\"rtm.msg.payload_report_enabled\": \(true)}")
        #endif
        log(info: "rtm sdk version: \(AgoraRtmKit.getSDKVersion())")
    }
    
    func login(rtmId: String, token: String?, success: Completion, fail: ErrorCompletion) {
        login(byToken: token, user: rtmId) { [unowned self] (errorCode) in
            switch errorCode {
            case .ok:
                self.log(info: "rtm login success", extra: "rtmId: \(rtmId)")
                if let success = success {
                    success()
                }
            default:
                let error = AGEError.rtm("login fail",
                                         code: errorCode.rawValue)
                
                self.log(error: error)
                if let fail = fail {
                    fail(error)
                }
            }
        }
    }
    
    func send(message: String, of event: ACRequestEvent, to peer: String, success: Completion, fail: ErrorCompletion) {
        let messageObj = AgoraRtmMessage(text: message)
        
        send(messageObj, toPeer: peer) { [unowned self] (errorCode) in
            let extra = "event: \(event), message: \(message), peer: \(peer)"
            
            switch errorCode {
            case .ok:
                self.log(info: "rtm send peer message success", extra: extra)
                if let success = success {
                    success()
                }
            default:
                let error = AGEError.rtm("send peer message fail",
                                         code: errorCode.rawValue,
                                         extra: extra)
                
                self.log(error: error)
                if let fail = fail {
                    fail(error)
                }
            }
        }
    }
    
    func joinChannel(_ id: String, delegate: AgoraRtmChannelDelegate, success: Completion, fail: ErrorCompletion) {
        do {
            let channel = try createChannel(id: id, delegate: delegate)
            channel.join { (errorCode) in
                switch errorCode {
                case .channelErrorOk:
                    self.log(info: "rtm join channel success", extra: "channel id: \(id)")
                    if let success = success {
                        success()
                    }
                default:
                    let error = AGEError.rtm("join channel fail",
                                             code: errorCode.rawValue,
                                             extra: "channel: \(id)")
                    
                    self.log(error: error)
                    if let fail = fail {
                        fail(error)
                    }
                }
            }
        } catch {
            log(error: error, extra: "create channel fail")
            if let fail = fail {
                fail(error)
            }
        }
    }
    
    func leaveChannel(_ id: String) throws {
        let channel = try getChannel(id: id)
        channel.leave(completion: nil)
        self.destroyChannel(withId: id)
        
        self.log(info: "rtm leave channel")
    }
    
    func getChannel(id: String) throws -> AgoraRtmChannel {
        guard let channels = self.value(forKey: "channels") as? [String: AgoraRtmChannel] else {
            throw AGEError.rtm("channels dic nil")
        }
        guard let channel = channels[id] else {
            throw AGEError.rtm("channel instance nil")
        }
        return channel
    }
    
    func renewToken(_ token: String, fail: ErrorCompletion) {
        self.renewToken(token) { (nil, errorCode) in
            switch errorCode {
            case .ok:
                self.log(info: "renew token success")
            default:
                let error = AGEError.rtm("renew token fail",
                                         code: errorCode.rawValue)
                
                self.log(error: error)
                if let fail = fail {
                    fail(error)
                }
            }
        }
    }
}

private extension AgoraRtmKit {
    func createChannel(id: String, delegate: AgoraRtmChannelDelegate) throws -> AgoraRtmChannel {
        guard let channel = self.createChannel(withId: id, delegate: delegate) else {
            throw AGEError.rtm("create fail")
        }
        return channel
    }
}

extension AgoraRtmChannel: AGELogBase {
    var logTube: LogTube {
        get {
            return ALCenter.shared().centerProvideLogTubeHelper()
        }
        set {
        }
    }
    
    func send(message: String, of event: ACRequestEvent, success: Completion, fail: ErrorCompletion) {
        let messageObj = AgoraRtmMessage(text: message)
        
        send(messageObj) { [unowned self] (errorCode) in
            let extra = "event: \(event), message: \(message)"
            switch errorCode {
            case .errorOk:
                self.log(info: "send message success",
                         extra: extra)
                if let success = success {
                    success()
                }
            default:
                let error = AGEError.rtm("send channel message fail",
                                         code: errorCode.rawValue,
                                         extra: extra)
                
                self.log(error: error)
                if let fail = fail {
                    fail(error)
                }
            }
        }
    }
}

extension AgoraRtmConnectionState {
    var description: String {
        switch self {
        case .reconnecting: return "reconnecting"
        case .disconnected: return "disconnected"
        case .connecting:   return "connecting"
        case .connected:    return "connected"
        case .aborted:      return "aborted"
        @unknown default:   return "unknown"
        }
    }
}

extension AgoraRtmConnectionChangeReason {
    var description: String {
        switch self {
        case .bannedByServer: return "bannedByServer"
        case .loginFailure:   return "loginFailure"
        case .loginSuccess:   return "loginSuccess"
        case .loginTimeout:   return "loginTimeout"
        case .remoteLogin:    return "remoteLogin"
        case .interrupted:    return "interrupted"
        case .logout:         return "logout"
        case .login:          return "login"
        @unknown default:     return "unknown"
        }
    }
}

// MARK: Log
private extension AgoraRtmKit {
    func log(info: String, extra: String? = nil, funcName: String = #function) {
        let className = AgoraRtmKit.self
        logOutputInfo(info, extra: extra, className: className, funcName: funcName)
    }
    
    func log(warning: String, extra: String? = nil, funcName: String = #function) {
        let className = AgoraRtmKit.self
        logOutputWarning(warning, extra: extra, className: className, funcName: funcName)
    }
    
    func log(error: Error, extra: String? = nil, funcName: String = #function) {
        let className = AgoraRtmKit.self
        logOutputError(error, extra: extra, className: className, funcName: funcName)
    }
}

// MARK: Log
private extension AgoraRtmChannel {
    func log(info: String, extra: String? = nil, funcName: String = #function) {
        let className = AgoraRtmChannel.self
        logOutputInfo(info, extra: extra, className: className, funcName: funcName)
    }
    
    func log(warning: String, extra: String? = nil, funcName: String = #function) {
        let className = AgoraRtmChannel.self
        logOutputWarning(warning, extra: extra, className: className, funcName: funcName)
    }
    
    func log(error: Error, extra: String? = nil, funcName: String = #function) {
        let className = AgoraRtmChannel.self
        logOutputError(error, extra: extra, className: className, funcName: funcName)
    }
}
