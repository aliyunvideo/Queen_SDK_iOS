//
//  RTMHelper.swift
//  AGEManAGEr
//
//  Created by CavanSu on 2019/4/23.
//  Copyright © 2019 Agora. All rights reserved.
//

import Foundation
import AlamoClient

enum RTMmessageType: AGEDescription {
    case peer(Int), channel
    
    var description: String {
        return cusDescription()
    }
    
    var debugDescription: String {
        return cusDescription()
    }
    
    func cusDescription() -> String {
        switch self {
        case .peer(let agoraUid):  return "peer: \(agoraUid)"
        case .channel:             return "channel"
        }
    }
}

struct RTMQueueItem {
    var event: ACRequestEvent
    var success: ACResponse?
    var fail: ErrorCompletion
    
    init(event: ACRequestEvent, success: ACResponse?, fail: ErrorCompletion) {
        self.event = event
        self.success = success
        self.fail = fail
    }
}

class RTMRequestQueue: NSObject {
    lazy var all = [Int: RTMQueueItem]()
    
    func push(_ item: RTMQueueItem, id: Int) {
        all[id] = item
    }
    
    @discardableResult func pop(id: Int) -> RTMQueueItem? {
        defer {
            all.removeValue(forKey: id)
        }
        return all[id]
    }
}
