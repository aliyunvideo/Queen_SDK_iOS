//
//  deviceVM.swift
//  AgoraLive
//
//  Created by CavanSu on 2020/4/18.
//  Copyright © 2020 Agora. All rights reserved.
//

import UIKit
import RxSwift
import RxRelay
import AGECamera

class MediaDeviceVM: RxObject {
    var camera: AGESwitch {
        get {
            let mediaKit = ALCenter.shared().centerProvideMediaHelper()
            return mediaKit.capture.video
        }
        
        set {
            let mediaKit = ALCenter.shared().centerProvideMediaHelper()
            try? mediaKit.capture.video(newValue)
        }
    }
    
    var cameraPosition = BehaviorRelay(value: ALCenter.shared().centerProvideMediaHelper().capture.cameraPostion)
    
    var mic: AGESwitch {
        get {
            let mediaKit = ALCenter.shared().centerProvideMediaHelper()
            return mediaKit.capture.audio
        }
        
        set {
            let mediaKit = ALCenter.shared().centerProvideMediaHelper()
            mediaKit.capture.audio = newValue
        }
    }
    
    var localAudioLoop: AGESwitch {
        get {
            let mediaKit = ALCenter.shared().centerProvideMediaHelper()
            return mediaKit.player.isLocalAudioLoop ? .on : .off
        }
        
        set {
            let mediaKit = ALCenter.shared().centerProvideMediaHelper()
            mediaKit.player.localInputAudioLoop(newValue)
        }
    }
    
    var audioOutput: BehaviorRelay<AudioOutputRouting> = BehaviorRelay(value: AudioOutputRouting.default)
    
    func audioLoop(_ action: AGESwitch) {
        let mediaKit = ALCenter.shared().centerProvideMediaHelper()
        mediaKit.player.localInputAudioLoop(action)
    }
    
    func switchCamera() {
        let mediaKit = ALCenter.shared().centerProvideMediaHelper()
        
        do {
            try mediaKit.capture.switchCamera()
            cameraPosition.accept(mediaKit.capture.cameraPostion)
        } catch {
            assert(false)
        }
    }
    
    func cameraCaptureResolution(_ resolution: AVCaptureSession.Preset) {
        let mediaKit = ALCenter.shared().centerProvideMediaHelper()
        mediaKit.capture.videoResolution(.high)
    }
    
    override init() {
        super.init()
        let mediaKit = ALCenter.shared().centerProvideMediaHelper()
        mediaKit.player.audioOutputRouting.bind(to: audioOutput).disposed(by: bag)
        
        cameraPosition.subscribe(onNext: { [unowned mediaKit] (position) in
            mediaKit.capture.cameraPostion = position
        }).disposed(by: bag)
    }
}
