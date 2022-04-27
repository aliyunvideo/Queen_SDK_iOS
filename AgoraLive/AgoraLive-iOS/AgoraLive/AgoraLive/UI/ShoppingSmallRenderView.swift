//
//  ShppoingSmallRenderView.swift
//  AgoraLive
//
//  Created by CavanSu on 2020/7/30.
//  Copyright © 2020 Agora. All rights reserved.
//

import UIKit

class ShoppingSmallRenderView: RxView {
    private let shadow = UIImageView()
    private let limitMinX: CGFloat = 0
    private let limitMinY: CGFloat = 0
    private var limitMaxX: CGFloat
    private var limitMaxY: CGFloat
    
    let nameLabel = UILabel()
    let renderView = UIView()
    let closeButton = UIButton()
    
    var isCanMove: Bool = false
    
    override init(frame: CGRect) {
        self.limitMaxY = frame.origin.y
        self.limitMaxX = frame.origin.x
        super.init(frame: frame)
        initViews()
    }
    
    required init?(coder: NSCoder) {
        self.limitMaxY = 0
        self.limitMaxX = 0
        super.init(coder: coder)
        initViews()
    }
    
    override func layoutSubviews() {
        super.layoutSubviews()
        let renderX: CGFloat = 10
        let renderY = renderX
        let renderWith: CGFloat = bounds.width - (renderX * CGFloat(2))
        let renderHeight: CGFloat = bounds.height - (renderY * CGFloat(2))
        renderView.frame = CGRect(x: renderX,
                                  y: renderY,
                                  width: renderWith,
                                  height: renderHeight)
        renderView.layer.masksToBounds = true
        renderView.cornerRadius(5)
        
        let shadowHeight: CGFloat = 28
        shadow.frame = CGRect(x: 0,
                              y: renderView.bounds.height - shadowHeight,
                              width: renderView.bounds.width,
                              height: shadowHeight)
        
        nameLabel.frame = shadow.frame
        
        let buttonWidth: CGFloat = 20
        let buttonHeight = buttonWidth
        let buttonX: CGFloat = bounds.width - buttonWidth
        let buttonY: CGFloat = 0
        closeButton.frame = CGRect(x: buttonX,
                                   y: buttonY,
                                   width: buttonWidth,
                                   height: buttonHeight)
    }
    
    override func touchesMoved(_ touches: Set<UITouch>, with event: UIEvent?) {
        guard let touch = touches.first,
            isCanMove else {
            return
        }
        
        let currentPoint = touch.location(in: self)
        let previousPoint = touch.previousLocation(in: self)
        
        var offX: CGFloat = previousPoint.x - currentPoint.x
        var offY: CGFloat =  previousPoint.y - currentPoint.y
        
        let futureX: CGFloat = self.frame.origin.x - offX
        let futureY: CGFloat = self.frame.origin.y - offY
        
        if futureX < limitMinX || futureX > limitMaxX {
            offX = 0
        }
        
        if futureY < limitMinY || futureY > limitMaxY {
            offY = 0
        }
        
        self.transform = self.transform.translatedBy(x: -offX, y: -offY)
    }
    
    func initViews() {
        addSubview(renderView)
        
        shadow.image = UIImage(named: "shadow")
        renderView.addSubview(shadow)
        
        nameLabel.textColor = .white
        nameLabel.font = UIFont.systemFont(ofSize: 11)
        nameLabel.textAlignment = .center
        renderView.addSubview(nameLabel)
        
        closeButton.setImage(UIImage(named: "icon-close video"), for: .normal)
        addSubview(closeButton)
    }
}
