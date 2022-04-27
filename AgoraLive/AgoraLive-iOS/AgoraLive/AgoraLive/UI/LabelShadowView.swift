//
//  LabelShadowView.swift
//  AgoraLive
//
//  Created by CavanSu on 2020/4/23.
//  Copyright © 2020 Agora. All rights reserved.
//

import UIKit
import FLAnimatedImage

class LabelShadowView: UIView {
    fileprivate var shadow = UIImageView(frame: CGRect.zero)
    var shadowOffSetX: CGFloat = 0.0
    var imageView = UIImageView(frame: CGRect.zero)
    var label = UILabel(frame: CGRect.zero)
    
    override init(frame: CGRect) {
        super.init(frame: frame)
        initViews()
    }
    
    required init?(coder: NSCoder) {
        super.init(coder: coder)
        initViews()
    }
    
    override func layoutSubviews() {
        super.layoutSubviews()
        self.imageView.frame = CGRect(x: 0,
                                      y: 0,
                                      width: self.bounds.width,
                                      height: self.bounds.height)
        
        let shadowH: CGFloat = 25.0
        let shadowW: CGFloat = self.bounds.width - (shadowOffSetX * 2)
        let shadowX: CGFloat = shadowOffSetX
        let shadowY: CGFloat = self.bounds.height - shadowH
        self.shadow.frame = CGRect(x: shadowX,
                                   y: shadowY,
                                   width: shadowW,
                                   height: shadowH)
        
        let labelH: CGFloat = 25.0
        let labelW: CGFloat = self.bounds.width - (shadowOffSetX * 2)
        let labelX: CGFloat = shadowOffSetX
        let labelY: CGFloat = self.bounds.height - labelH
        
        self.label.frame = CGRect(x: labelX, y: labelY, width: labelW, height: labelH)
    }
}

let shadowRoundImage = UIImage(named: "shadow-round")!
let shadowImage = UIImage(named: "shadow")!
let shadowRondStrechImage = shadowRoundImage.stretchableImage(withLeftCapWidth: Int(shadowImage.size.width * 0.5),
                                                              topCapHeight: Int(shadowImage.size.height * 0.5))
let shadowStrechImage = shadowImage.stretchableImage(withLeftCapWidth: Int(shadowImage.size.width * 0.5),
                                                     topCapHeight: Int(shadowImage.size.height * 0.5))

fileprivate extension LabelShadowView {
    @objc func initViews() {
        self.addSubview(imageView)
        
        self.shadow.image = shadowRondStrechImage
        self.addSubview(shadow)
        
        self.label.textColor = UIColor.white
        self.label.font = UIFont.systemFont(ofSize: 11)
        self.label.textAlignment = .center
        self.addSubview(label)
    }
}

class LabelShadowRender: LabelShadowView {
    private lazy var speakerView: GIFView = {
        let gif = Bundle.main.url(forResource: "sound", withExtension: "gif")
        let data = try! Data(contentsOf: gif!)
        let view = GIFView(frame: CGRect.zero, gif: data)
        return view
    }()
    
    var renderView = UIView(frame: CGRect.zero)
    var audioSilenceTag = UIImageView()
    
    @objc override func initViews() {
        super.initViews()
        
        self.shadow.image = shadowStrechImage
        
        self.insertSubview(renderView, at: 0)
        
        audioSilenceTag.image = UIImage(named: "icon-Microphone off")
        audioSilenceTag.isHidden = true
        self.addSubview(audioSilenceTag)
        
        speakerView.isHidden = true
        self.addSubview(speakerView)
    }
    
    override func layoutSubviews() {
        super.layoutSubviews()
        
        renderView.frame = CGRect(x: 0,
                                  y: 0,
                                  width: self.bounds.width,
                                  height: self.bounds.height)
        
        speakerView.frame = CGRect(x: self.bounds.width - 20,
                                   y: self.bounds.height - 20,
                                   width: 20,
                                   height: 20)
        
        audioSilenceTag.frame = CGRect(x: self.bounds.width - 20,
                                       y: self.bounds.height - 20,
                                       width: 20,
                                       height: 20)
    }
    
    func startSpeakerAnimating() {
        audioSilenceTag.isHidden = true
        speakerView.isHidden = false
        speakerView.startAnimating { [unowned speakerView] in
            speakerView.isHidden = true
        }
    }
}
