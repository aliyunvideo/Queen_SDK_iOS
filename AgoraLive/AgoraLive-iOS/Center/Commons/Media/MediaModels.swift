//
//  MediaModels.swift
//  AGECenter
//
//  Created by CavanSu on 2019/7/9.
//  Copyright © 2019 Agora. All rights reserved.
//

import Foundation
import AgoraRtcKit

enum AGEResolution: AGEDescription {
    case low, standard, high
    
    var rawValue: Int {
        switch self {
        case .low:        return 0
        case .standard:   return 1
        case .high:       return 2
        }
    }
    
    var description: String {
        return cusDescription()
    }
    
    var debugDescription: String  {
        return cusDescription()
    }
    
    func cusDescription() -> String {
        switch self {
        case .low:        return "low"
        case .standard:   return "standard"
        case .high:       return "high"
        }
    }
}

#if os(macOS)
enum MediaDeviceType: Int, AGEDescription {
    case camera, mic, speaker, unknown
    
    var description: String {
        return cusDescription()
    }
    
    var debugDescription: String {
        return cusDescription()
    }
    
    var rtc: AgoraMediaDeviceType {
        switch self {
        case .camera:   return AgoraMediaDeviceType.videoCapture
        case .mic:      return AgoraMediaDeviceType.audioRecording
        case .speaker:  return AgoraMediaDeviceType.audioPlayout
        case .unknown:  return AgoraMediaDeviceType.audioUnknown
        }
    }
    
    func cusDescription() -> String {
        switch self {
        case .camera:   return "camera"
        case .mic:      return "mic"
        case .speaker:  return "speaker"
        case .unknown:  return "unknown"
        }
    }
}

struct MediaDeviceInfo: AGEDescription {
    var description: String {
        return cusDescription()
    }
    
    var debugDescription: String {
        return cusDescription()
    }
    
    var type: MediaDeviceType
    var id: String?
    var name: String?
    
    init(type: MediaDeviceType, id: String?, name: String?) {
        self.type = type
        self.id = id
        self.name = name
    }
    
    func cusDescription() -> String {
        let dic: [String: Any] = ["type": type.rawValue,
                                  "id": OptionsDescription.any(id),
                                  "name": OptionsDescription.any(name)]
        return dic.description
    }
}

enum DeviceStartTest: AGEDescription {
    case recording, playback(String), capture(NSView)
    
    var description: String {
        return cusDescription()
    }
    
    var debugDescription: String {
        return cusDescription()
    }
    
    func cusDescription() -> String {
        switch self {
        case .capture:              return "capture"
        case .recording:            return "recording"
        case .playback(let path):   return "playback, path: \(path)"
        }
    }
}

enum DeviceStopTest: AGEDescription {
    case recording, playback, capture
    
    var description: String {
        return cusDescription()
    }
    
    var debugDescription: String {
        return cusDescription()
    }
    
    func cusDescription() -> String {
        switch self {
        case .capture:              return "capture"
        case .recording:            return "recording"
        case .playback:             return "playback"
        }
    }
}

enum MediaDeviceState: Int {
    case add = 0, remove
}
#endif
