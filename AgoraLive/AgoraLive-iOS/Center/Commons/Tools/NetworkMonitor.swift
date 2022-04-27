//
//  NetworkMonitor.swift
//  AGECenter
//
//  Created by CavanSu on 2019/7/11.
//  Copyright © 2019 Agora. All rights reserved.
//

import Foundation
import RxSwift
import RxRelay
import Alamofire

typealias ReachabilityStatus = NetworkReachabilityManager.NetworkReachabilityStatus

class NetworkMonitor: NSObject, AGELogBase {
    private lazy var netListener = NetworkReachabilityManager(host: self.host)
    private let host: String
    
    var connect = PublishRelay<ReachabilityStatus>()
    
    var logTube: LogTube {
        get {
            return ALCenter.shared().centerProvideLogTubeHelper()
        }
        set {
        }
    }
    
    init(host: String) {
        self.host = host
    }
}

extension ReachabilityStatus: CustomStringConvertible {
    public var description: String {
        return cusDescription()
    }
    
    func cusDescription() -> String {
        switch self {
        case .unknown:               return "unknown"
        case .notReachable:          return "notReachable"
        case .reachable(let type):   return  ((type == .ethernetOrWiFi ? "ethernetOrWiFi" : "wwan"))
        }
    }
}

extension NetworkMonitor {
    func action(_ action: AGESwitch) {
        guard let netListener = self.netListener else {
            return
        }

        switch action {
        case .on:
            netListener.listener = { [unowned self] status in
                self.connect.accept(status)
                self.log(info: "connection status: \(status.description)")
            }
            netListener.startListening()
        case .off:
            netListener.stopListening()
        }
    }
}

// MARK: Log
private extension NetworkMonitor {
    func log(info: String, extra: String? = nil, funcName: String = #function) {
        let className = NetworkMonitor.self
        logOutputInfo(info, extra: extra, className: className, funcName: funcName)
    }
}
