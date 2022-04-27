//
//  ViewFrame.swift
//  AgoraLive
//
//  Created by CavanSu on 2020/2/11.
//  Copyright © 2020 Agora. All rights reserved.
//

import UIKit

struct ViewFrame {
    enum Property {
        case left(CGFloat), right(CGFloat), top(CGFloat), bottom(CGFloat)
        case width(CGFloat), height(CGFloat)
    }
    
    fileprivate static var standardHeight: CGFloat = 667.0
    fileprivate static var standardWidth: CGFloat = 375.0
    fileprivate static var screenWidth = UIScreen.main.bounds.width
    fileprivate static var screenHeight = UIScreen.main.bounds.height
    
    static func standard(value: ViewFrame.Property) -> CGFloat {
        switch value {
        case .left(let val):
            return (val / standardWidth) * screenWidth
        case .right(let val):
            return (val / standardWidth) * screenWidth
        case .top(let val):
            return (val / standardHeight) * screenHeight
        case .bottom(let val):
            return (val / standardHeight) * screenHeight
        case .width(let val):
            return (val / standardWidth) * screenWidth
        case .height(let val):
            return (val / standardHeight) * screenHeight
        }
    }
}

