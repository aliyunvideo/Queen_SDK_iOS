//
//  MediaKit.swift
//  AGECenter
//
//  Created by CavanSu on 2019/6/23.
//  Copyright © 2019 Agora. All rights reserved.
//

import Foundation
import AgoraRtcKit
import RxSwift
import RxRelay
import AGECamera

typealias AudioOutputRouting = AgoraAudioOutputRouting
typealias ChannelReport = RTCStatistics

class MediaKit: NSObject, AGELogBase {
    enum Speaker {
        case local, other(agoraUid: UInt)
    }
    
    static var rtcKit = AgoraRtcEngineKit.sharedEngine(withAppId: ALKeys.AgoraAppId,
                                                       delegate: nil)
    
    fileprivate let agoraKit = rtcKit
    fileprivate var channelProfile: AgoraChannelProfile = .liveBroadcasting
    
    private(set) var channelReport = BehaviorRelay(value: RTCStatistics(type: .local(RTCStatistics.Local(stats: AgoraChannelStats()))))
    private(set) var activeSpeaker = PublishRelay<Speaker>()
    private(set) var channelStatus: AGEChannelStatus = .out
    
    private var cameraStreamQueue = DispatchQueue(label: "CameraStreamQueue")
    
    lazy var capture = Capture(parent: self)
    lazy var player = Player()
    lazy var enhancement = VideoEnhancement()
    
    var rtcVersion: String {
        return AgoraRtcEngineKit.getSdkVersion()
    }
    
    var consumer: AgoraVideoFrameConsumer?
    var logTube: LogTube
    
    init(log: LogTube) {
        self.logTube = log
        super.init()
        self.reinitRTC()
    }
    
    func reinitRTC() {
        MediaKit.rtcKit = AgoraRtcEngineKit.sharedEngine(withAppId: ALKeys.AgoraAppId,
                                                         delegate: self)
        MediaKit.rtcKit.delegate = self
        MediaKit.rtcKit.alMode()
        MediaKit.rtcKit.setVideoSource(self)
        let files = ALCenter.shared().centerProvideFilesGroup()
        let path = files.logs.folderPath + "/rtc.log"
        MediaKit.rtcKit.setLogFile(path)
    }
    
    func join(channel: String, token: String? = nil, streamId: Int, parameters: [String]? = nil, success: Completion = nil) {
        if let parameters = parameters {
            for item in parameters {
                agoraKit.setParameters(item)
            }
        }
        
        agoraKit.join(channel: channel, token: token, streamId: streamId) { [unowned self] in
            self.channelStatus = .ing
            if let success = success {
                success()
            }
        }
    }
    
    func leaveChannel() {
        self.channelStatus = .out
        agoraKit.leaveChannel(nil)
    }
    
    func setupPublishedVideoStream(resolution: CGSize, frameRate: AgoraVideoFrameRate, bitRate: Int) {
        log(info: "setup video",
            extra: "resolution width: \(resolution.width) x height: \(resolution.height), frameRate: \(frameRate.rawValue), bitRate: \(bitRate)")
        agoraKit.setupVideo(resolution: resolution, frameRate: frameRate, bitRate: bitRate)
        
        var new = self.channelReport.value
        new.fps = frameRate.rawValue
        channelReport.accept(new)
    }
    
    func publishMediaStream(_ action: AGESwitch) {
        agoraKit.muteLocalAudioStream(action.boolValue)
        agoraKit.muteLocalVideoStream(action.boolValue)
    }
    
    func startRelayingMediaStreamOf(currentChannel: String, currentSourceToken: String, to otherChannel: String, with otherChannelToken: String, otherChannelUid: UInt) {
        let configuration = AgoraChannelMediaRelayConfiguration()
        let sourceInfo = AgoraChannelMediaRelayInfo(token: currentSourceToken)
        sourceInfo.channelName = currentChannel
        sourceInfo.uid = 0
        configuration.sourceInfo = sourceInfo
        
        let destinationInfo = AgoraChannelMediaRelayInfo(token: otherChannelToken)
        destinationInfo.uid = otherChannelUid
        destinationInfo.channelName = otherChannel
        configuration.setDestinationInfo(destinationInfo, forChannelName: otherChannel)
        agoraKit.startChannelMediaRelay(configuration)
    }
    
    func stopRelayingMediaStream() {
        agoraKit.stopChannelMediaRelay()
    }
}

extension MediaKit {
    func checkChannelProfile() {
        guard channelProfile == .liveBroadcasting else {
            return
        }
        
        if capture.video == .off,
            capture.audio == .off {
            agoraKit.setClientRole(.audience)
        } else {
            agoraKit.setClientRole(.broadcaster)
        }
    }
}

extension MediaKit: AgoraVideoSourceProtocol {
    func shouldInitialize() -> Bool {
        return true
    }
    
    func shouldStart() {
        
    }
    
    func shouldStop() {
        
    }
    
    func shouldDispose() {
        
    }
    
    func bufferType() -> AgoraVideoBufferType {
        return .pixelBuffer
    }
}

extension MediaKit: AGESingleCameraDelegate {
    func camera(_ camera: AGESingleCamera, position: AGECamera.Position, didOutput sampleBuffer: CMSampleBuffer) {
        cameraStreamQueue.async { [unowned self] in
            guard let pixelBuffer = CMSampleBufferGetImageBuffer(sampleBuffer) else {
                return
            }
            
            CVPixelBufferLockBaseAddress(pixelBuffer, .init(rawValue: 0))
            
            let timeStamp = CMSampleBufferGetPresentationTimeStamp(sampleBuffer)
            
            if self.enhancement.beauty == .on || self.enhancement.appearance != .none {
                self.enhancement.renderItems(to: pixelBuffer)
            }
            
            self.consumer?.consumePixelBuffer(pixelBuffer,
                                              withTimestamp: timeStamp,
                                              rotation: .rotationNone)
            
            CVPixelBufferUnlockBaseAddress(pixelBuffer, .init(rawValue: 0))
        }
    }
}

extension MediaKit: AgoraRtcEngineDelegate {
    func rtcEngineLocalAudioMixingDidFinish(_ engine: AgoraRtcEngineKit) {
        if let block = self.player.mixFileAudioFinish {
            block()
        }
    }
    
    func rtcEngine(_ engine: AgoraRtcEngineKit, didOccurError errorCode: AgoraErrorCode) {
        log(error: AGEError(type: .rtc("did occur error"), code: errorCode.rawValue))
    }
    
    func rtcEngine(_ engine: AgoraRtcEngineKit, didAudioRouteChanged routing: AgoraAudioOutputRouting) {
        player.audioOutputRouting.accept(routing)
    }
    
    func rtcEngine(_ engine: AgoraRtcEngineKit, videoSizeChangedOfUid uid: UInt, size: CGSize, rotation: Int) {
        if uid == 0 {
            var new = self.channelReport.value
            new.dimension = size
            channelReport.accept(new)
        }
    }
    
    func rtcEngine(_ engine: AgoraRtcEngineKit, reportRtcStats stats: AgoraChannelStats) {
        var new = self.channelReport.value
        new.updateChannelStats(stats)
        channelReport.accept(new)
    }
    
    func rtcEngine(_ engine: AgoraRtcEngineKit, channelMediaRelayStateDidChange state: AgoraChannelMediaRelayState, error: AgoraChannelMediaRelayError) {
        if error != .none {
            log(error: AGEError(type: .rtc("channel media relay error"), code: error.rawValue))
        }
    }
    
    func rtcEngine(_ engine: AgoraRtcEngineKit, reportAudioVolumeIndicationOfSpeakers speakers: [AgoraRtcAudioVolumeInfo], totalVolume: Int) {
        for user in speakers {
            let speakerUid = user.uid
            if speakerUid == 0 {
                activeSpeaker.accept(.local)
            } else {
                activeSpeaker.accept(.other(agoraUid: speakerUid))
            }
        }
    }
}

// MARK: Log
private extension MediaKit {
    func log(info: String, extra: String? = nil, funcName: String = #function) {
        let className = MediaKit.self
        logOutputInfo(info, extra: extra, className: className, funcName: funcName)
    }
    
    func log(warning: String, extra: String? = nil, funcName: String = #function) {
        let className = MediaKit.self
        logOutputWarning(warning, extra: extra, className: className, funcName: funcName)
    }
    
    func log(error: Error, extra: String? = nil, funcName: String = #function) {
        let className = MediaKit.self
        logOutputError(error, extra: extra, className: className, funcName: funcName)
    }
}
