//
//  PKViewController.swift
//  AgoraLive
//
//  Created by CavanSu on 2020/8/9.
//  Copyright © 2020 Agora. All rights reserved.
//

import UIKit

class PKViewController: UIViewController {
    @IBOutlet weak var pkTimeView: IconTextView!
    @IBOutlet weak var leftRenderView: UIView!
    @IBOutlet weak var rightRenderView: UIView!
    @IBOutlet weak var intoOtherButton: UIButton!
    @IBOutlet weak var rightLabel: UILabel!
    @IBOutlet weak var giftBar: PKBar!
    
    private lazy var resultImageView: UIImageView = {
        let wh: CGFloat = 110
        let y: CGFloat = UIScreen.main.bounds.height
        let x: CGFloat = ((self.view.bounds.width - wh) * 0.5)
        let view = UIImageView(frame: CGRect.zero)
        view.contentMode = .scaleAspectFit
        view.frame = CGRect(x: x, y: y, width: wh, height: wh)
        return view
    }()
    
    private var timer: Timer!
    
    deinit {
        #if !RELEASE
        print("deinit PKViewController")
        #endif
    }
    
    override func viewDidLoad() {
        super.viewDidLoad()
        self.view.backgroundColor = .clear
        pkTimeView.offsetLeftX = -10
        pkTimeView.offsetRightX = 10
        pkTimeView.imageView.image = UIImage(named: "icon-time")
        pkTimeView.label.textColor = .white
        pkTimeView.label.font = UIFont.systemFont(ofSize: 11)
        pkTimeView.label.adjustsFontSizeToFitWidth = true
        pkTimeView.backgroundColor = UIColor(red: 0, green: 0, blue: 0, alpha: 0.6)
    }
    
    var countDown: Int = 0
    
    func startCountingDown() {
        guard timer == nil else {
            return
        }
        timer = Timer(timeInterval: 1.0,
                      target: self,
                      selector: #selector(countingDown),
                      userInfo: nil,
                      repeats: true)
        RunLoop.main.add(timer, forMode: .common)
        timer.fire()
    }
    
    func stopCountingDown() {
        guard timer != nil else {
            return
        }
        timer.invalidate()
        timer = nil
    }
    
    @objc private func countingDown() {
        DispatchQueue.main.async { [unowned self] in
            if self.countDown >= 0 {
                let miniter = self.countDown / (60 * 1000)
                let second = (self.countDown / 1000) % 60
                let secondString = String(format: "%0.2d", second)
                self.pkTimeView.label.textAlignment = .left
                self.pkTimeView.label.text = "   \(NSLocalizedString("PK_Remaining")): \(miniter):\(secondString)"
                self.countDown -= 1000
            } else {
                self.stopCountingDown()
            }
        }
    }
    
    func showWinner(isLeft: Bool, completion: Completion = nil) {
        resultImageView.image = UIImage(named: "pic-Winner")
        
        let wh: CGFloat = 110
        let y: CGFloat = 127
        var x: CGFloat
        
        if isLeft {
            x = (leftRenderView.bounds.width - wh) * 0.5
        } else {
            x = leftRenderView.frame.maxX + (rightRenderView.bounds.width - wh) * 0.5
        }
        
        self.showResultImgeView(newFrame: CGRect(x: x, y: y, width: wh, height: wh),
                                completion: completion)
    }
    
    func showDraw(completion: Completion = nil) {
        resultImageView.image = UIImage(named: "pic-平局")
        
        let wh: CGFloat = 110
        let y: CGFloat = 127
        let x: CGFloat = ((self.view.bounds.width - wh) * 0.5)
        self.showResultImgeView(newFrame: CGRect(x: x, y: y, width: wh, height: wh),
                                completion: completion)
    }
    
    private func showResultImgeView(newFrame: CGRect, completion: Completion = nil) {
        self.view.addSubview(resultImageView)
        resultImageView.isHidden = false
        
        UIView.animate(withDuration: TimeInterval.animation, animations: { [unowned self] in
            self.resultImageView.frame = newFrame
        }) { [unowned self] (finish) in
            guard finish else {
                return
            }
            
            DispatchQueue.main.asyncAfter(deadline: .now() + 3.0) { [unowned self] in
                self.resultImageView.isHidden = true
                if let completion = completion {
                    completion()
                }
            }
        }
    }
}
