//
//  PKBar.swift
//  AgoraLive
//
//  Created by CavanSu on 2020/4/13.
//  Copyright © 2020 Agora. All rights reserved.
//

import UIKit

class PKBar: UIView {
    fileprivate lazy var rightView: UIView = {
        let view = UIView(frame: CGRect.zero)
        view.backgroundColor = UIColor(hexString: "#FF007D")
        return view
    }()
    
    fileprivate lazy var leftLabel: UILabel = {
        let label = UILabel(frame: CGRect.zero)
        label.textColor = UIColor.white
        label.font = UIFont.systemFont(ofSize: 14)
        label.textAlignment = .left
        return label
    }()
    
    fileprivate lazy var rightLabel: UILabel = {
        let label = UILabel(frame: CGRect.zero)
        label.textColor = UIColor.white
        label.font = UIFont.systemFont(ofSize: 14)
        label.textAlignment = .right
        return label
    }()
    
    var leftValue: Int = 0 {
        didSet {
            leftLabel.text = "\(leftValue)"
        }
    }
    var rightValue: Int = 0 {
        didSet {
            rightLabel.text = "\(rightValue)"
            layoutSubviews()
        }
    }
    
    override func awakeFromNib() {
        super.awakeFromNib()
        initViews()
    }
    
    func initViews() {
        self.backgroundColor = UIColor(hexString: "#0088EB")
        self.layer.borderColor = UIColor.white.cgColor
        self.layer.borderWidth = 1
        self.cornerRadius(4)
        self.layer.masksToBounds = true
        
        self.addSubview(rightView)
        self.addSubview(leftLabel)
        self.addSubview(rightLabel)
    }
    
    override func layoutSubviews() {
        super.layoutSubviews()
        
        if leftValue == 0, rightValue == 0 {
            rightView.frame = CGRect(x: self.bounds.width * 0.5,
            y: 0,
            width: self.bounds.width * 0.5,
            height: self.bounds.height)
        } else {
            let leftScale = CGFloat(leftValue) / CGFloat(leftValue + rightValue)
            let leftLength = self.bounds.width * leftScale
            
            rightView.frame = CGRect(x: leftLength,
                                     y: 0,
                                     width: self.bounds.width - leftLength,
                                     height: self.bounds.height)
        }
        
        let labelWidth: CGFloat = 100
        leftLabel.frame = CGRect(x: 10, y: 0, width: labelWidth, height: self.bounds.height)
        let x = self.bounds.width - 10 - labelWidth
        rightLabel.frame = CGRect(x: x, y: 0, width: labelWidth, height: self.bounds.height)
    }
}
