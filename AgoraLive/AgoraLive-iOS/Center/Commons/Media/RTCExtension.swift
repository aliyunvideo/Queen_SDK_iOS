//
//  Extension.swift
//  MetManAGEr
//
//  Created by CavanSu on 2019/4/4.
//  Copyright © 2019 Agora. All rights reserved.
//

import Foundation
import AgoraRtcKit

extension AgoraRtcVideoCanvas {
    convenience init(streamId: Int, view: ALView) {
        self.init()
        self.renderMode = .hidden
        self.uid = UInt(streamId)
        self.view = view
    }
}

extension AgoraVideoFrameRate {
    static let list: [AgoraVideoFrameRate] = [.fps15,
                                              .fps24,
                                              .fps30]
    
    var description: String {
        switch self {
        case .fps1:  return "1"
        case .fps7:  return "7"
        case .fps10: return "10"
        case .fps15: return "15"
        case .fps24: return "24"
        case .fps30: return "30"
        case .fps60: return "60"
        @unknown default: fatalError()
        }
    }
}

// Resolution
extension CGSize {
    static let AgoraVideoDimension360x640 = CGSize(width: 360, height: 640)
    static let AgoraVideoDimension480x848 = CGSize(width: 480, height: 848)
    static let AgoraVideoDimension720x1280 = CGSize(width: 720, height: 1280)
    
    static let resolutionList: [CGSize] = [AgoraVideoDimension240x240,
                                           AgoraVideoDimension360x640,
                                           AgoraVideoDimension480x848,
                                           AgoraVideoDimension720x1280]
}

extension AgoraAudioOutputRouting {
    var isSupportLoop: Bool {
        switch self {
        case .default, .headsetNoMic, .loudspeaker, .speakerphone: return false
        default:                                                   return true
        }
    }
}

extension AgoraNetworkQuality {
    func description() -> String {
        switch self {
        case .excellent:   return "excellent"
        case .good:        return "good"
        case .poor:        return "poor"
        case .bad:         return "bad"
        case .vBad:        return "very bad"
        case .down:        return "down"
        case .unknown:     return "unknown"
        case .unsupported: return "unsupported"
        case .detecting:   return "detecting"
        default:           return "unknown"
        }
    }
}

extension AgoraRtcEngineKit: AGELogBase {
    var logTube: LogTube {
        get {
            return ALCenter.shared().centerProvideLogTubeHelper()
        }
        set {
        }
    }
    
    func alMode() {
        self.setLogFilter(AgoraLogFilter.info.rawValue)
        self.setChannelProfile(.liveBroadcasting)
        self.setClientRole(.audience)
        self.enableVideo()
        self.enableDualStreamMode(true)
        self.enableWebSdkInteroperability(true)
        self.enableAudioVolumeIndication(2000, smooth: 3, report_vad: false)
        log(info: "rtc sdk version: \(AgoraRtcEngineKit.getSdkVersion())")
    }
    
    func setupVideo(resolution: CGSize, frameRate: AgoraVideoFrameRate, bitRate: Int) {
        let configuration = AgoraVideoEncoderConfiguration(size: resolution,
                                                           frameRate: frameRate,
                                                           bitrate: bitRate,
                                                           orientationMode: .adaptative)
        self.setVideoEncoderConfiguration(configuration)
    }
    
    func setConfigFromServer(config: [String: Any]) {
        if let channelProfile = config["channelProfile"] as? Int, let profile = AgoraChannelProfile(rawValue: channelProfile) {
            self.setChannelProfile(profile)
            if profile == .liveBroadcasting {
                self.setClientRole(.broadcaster)
            }
        }
        
        if let parameters = config["parameters"] as? [String] {
            for item in parameters {
                self.setParameters(item)
            }
        }
    }
    
    func join(channel: String, token: String? = nil, streamId: Int, success: Completion) {
        self.joinChannel(byToken: token, channelId: channel, info: nil, uid: UInt(streamId)) { [unowned self] (_, _, _) in
            self.log(info: "rtc join channel success", extra: "channel: \(channel)")
            if let success = success {
                success()
            }
        }
    }
}

// MARK: Log
private extension AgoraRtcEngineKit {
    func log(info: String, extra: String? = nil, funcName: String = #function) {
        let className = AgoraRtcEngineKit.self
        logOutputInfo(info, extra: extra, className: className, funcName: funcName)
    }
    
    func log(warning: String, extra: String? = nil, funcName: String = #function) {
        let className = AgoraRtcEngineKit.self
        logOutputWarning(warning, extra: extra, className: className, funcName: funcName)
    }
    
    func log(error: Error, extra: String? = nil, funcName: String = #function) {
        let className = AgoraRtcEngineKit.self
        logOutputError(error, extra: extra, className: className, funcName: funcName)
    }
}
