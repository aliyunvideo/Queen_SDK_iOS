//
//  PlayerVM.swift
//  AgoraLive
//
//  Created by CavanSu on 2020/4/27.
//  Copyright © 2020 Agora. All rights reserved.
//

import UIKit
import RxSwift
import RxRelay

typealias Speaker = MediaKit.Speaker

class PlayerVM: RxObject {
    var activeSpeaker = PublishRelay<Speaker>()
    
    override init() {
        super.init()
        self.observe()
    }
    
    func startRenderPreview(on view: UIView, isMirror: Bool = false) {
        let mediaKit = ALCenter.shared().centerProvideMediaHelper()
        mediaKit.player.startRenderLocalVideoStream(id: 0,
                                                    view: view,
                                                    isMirror: isMirror)
    }
    
    func startRenderVideoStreamOf(user: LiveRole, on view: UIView) {
        guard let session = ALCenter.shared().liveSession else {
            return
        }
         
        let mediaKit = ALCenter.shared().centerProvideMediaHelper()
        let local = session.role.value
        
        if local.agUId == user.agUId {
            mediaKit.player.startRenderLocalVideoStream(id: user.agUId,
                                                      view: view)
        } else {
            mediaKit.player.startRenderRemoteVideoStream(id: user.agUId,
                                                       view: view)
        }
    }
    
    func enhance(_ enhance: AGESwitch, videoStreamOf user: LiveRole) {
        let mediaKit = ALCenter.shared().centerProvideMediaHelper()
        mediaKit.player.renderRemoteVideoStream(id: user.agUId, superResolution: enhance)
    }
}

private extension PlayerVM {
    func observe() {
        let media = ALCenter.shared().centerProvideMediaHelper()
        media.activeSpeaker.bind(to: self.activeSpeaker).disposed(by: bag)
    }
}
