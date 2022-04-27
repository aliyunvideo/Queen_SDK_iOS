//
//  MediaSettingsViewController.swift
//  AgoraLive
//
//  Created by CavanSu on 2020/3/9.
//  Copyright © 2020 Agora. All rights reserved.
//

import UIKit
import AgoraRtcKit
import RxSwift
import RxRelay

class MediaSettingsViewController: UITableViewController {
    @IBOutlet weak var titleLabel: UILabel!
    
    @IBOutlet weak var resolutionLabel: UILabel!
    @IBOutlet weak var resolutionValueLabel: UILabel!
    @IBOutlet weak var frameRateLabel: UILabel!
    @IBOutlet weak var frameRateValueLabel: UILabel!
    
    @IBOutlet weak var bitRateLabel: UILabel!
    @IBOutlet weak var bitRateValueLabel: UILabel!
    @IBOutlet weak var bitrateSlider: UISlider!
    
    @IBOutlet weak var resolutionContentView: UIView!
    @IBOutlet weak var frameRateContentView: UIView!
    private let bag = DisposeBag()
    
    var settings: BehaviorRelay<VideoConfiguration>?
    
    override func viewDidLoad() {
        super.viewDidLoad()
        let color = UIColor(hexString: "#D8D8D8")
        let x: CGFloat = 15.0
        let width = UIScreen.main.bounds.width - (x * 2)
        self.titleLabel.containUnderline(color,
                                         x: x,
                                         width: width)
        
        self.resolutionContentView.containUnderline(color,
                                                    x: x,
                                                    width: width)
        
        self.frameRateContentView.containUnderline(color,
                                                   x: x,
                                                   width: width)
        
        self.titleLabel.text = NSLocalizedString("Live_Room_Settings")
        self.resolutionLabel.text = NSLocalizedString("Resolution")
        self.frameRateLabel.text = NSLocalizedString("FrameRate")
        self.bitRateLabel.text = NSLocalizedString("BitRate")
        
        guard let settings = settings?.value else {
            fatalError()
        }
        
        self.resolutionValueLabel.text = settings.resolution.description
        self.frameRateValueLabel.text = "\(settings.frameRate.description)"
        self.bitrateSlider.value = Float(settings.bitRate)
        
        self.bitrateSlider.rx.value.subscribe(onNext: { [unowned self] (value) in
            guard let settings = self.settings else {
                fatalError()
            }
            
            var media = settings.value
            media.bitRate = Int(value)
            self.settings?.accept(media)
            self.bitRateValueLabel.text = "\(media.bitRate) Kbps"
        }).disposed(by: bag)
    }
    
    override func tableView(_ tableView: UITableView, didSelectRowAt indexPath: IndexPath) {
        guard let settings = settings?.value else {
            fatalError()
        }
        
        let resolution = 0
        let frameRate = 1
        switch indexPath.row {
        case resolution:
            self.performSegue(withIdentifier: "MediaSettingsSubTable",
                              sender: MediaSettingsSubTable.DataType.resolution(settings.resolution))
        case frameRate:
            self.performSegue(withIdentifier: "MediaSettingsSubTable",
                              sender: MediaSettingsSubTable.DataType.frameRate(settings.frameRate))
        default:
            break
        }
    }
    
    override func prepare(for segue: UIStoryboardSegue, sender: Any?) {
        guard let segueId = segue.identifier,
            segueId == "MediaSettingsSubTable",
            let vc = segue.destination as? MediaSettingsSubTable,
            let data = sender as? MediaSettingsSubTable.DataType  else {
            fatalError()
        }
        
        switch data {
        case .resolution:
            let list = MediaSettingsSubTable.DataType.listOf(resolution: CGSize.resolutionList)
            vc.list = BehaviorRelay(value: list)
            vc.title = NSLocalizedString("Resolution")
        case .frameRate:
            let list = MediaSettingsSubTable.DataType.listOf(frameRate: AgoraVideoFrameRate.list)
            vc.list = BehaviorRelay(value: list)
            vc.title = NSLocalizedString("FrameRate")
        }
        
        vc.selectedValue = BehaviorRelay(value: data)
        vc.selectedValue?.subscribe(onNext: { [unowned self] (data) in
            guard let settings = self.settings else {
                fatalError()
            }
            
            switch data {
            case .resolution(let size):
                var media = settings.value
                media.resolution = size
                self.settings?.accept(media)
                self.resolutionValueLabel.text = size.description
            case .frameRate(let rate):
                var media = settings.value
                media.frameRate = rate
                self.settings?.accept(media)
                self.frameRateValueLabel.text = rate.description
            }
        }).disposed(by: bag)
    }
}

fileprivate extension CGSize {
    var description: String {
        return "\(Int(self.width)) X \(Int(self.height))"
    }
}

class MediaSettingsSubCell: UITableViewCell {
    @IBOutlet var nameLabel: UILabel!
    
    var underline: CALayer?
    
    var isSelectedByDataSource: Bool = false {
        didSet {
            if isSelectedByDataSource {
                nameLabel.textColor = UIColor.white
                contentView.backgroundColor = UIColor(hexString: "#0088EB")
                underline?.isHidden = true
            } else {
                nameLabel.textColor = UIColor(hexString: "#666666")
                contentView.backgroundColor = UIColor.white
                underline?.isHidden = false
            }
        }
    }
    
    override func awakeFromNib() {
        super.awakeFromNib()
        let color = UIColor(hexString: "#D8D8D8")
        let x: CGFloat = 15.0
        let width = UIScreen.main.bounds.width - (x * 2)
        underline = self.contentView.containUnderline(color,
                                                      x: x,
                                                      width: width)
    }
}

class MediaSettingsSubTable: UIViewController {
    enum DataType {
        case resolution(CGSize), frameRate(AgoraVideoFrameRate)
        
        var description: String {
            switch self {
            case .resolution(let size): return size.description
            case .frameRate(let rate):  return "\(rate.description)"
            }
        }
        
        static func listOf(frameRate: [AgoraVideoFrameRate]) -> [DataType] {
            var list = [DataType]()
            for rate in frameRate {
                let item = DataType.frameRate(rate)
                list.append(item)
            }
            return list
        }
        
        static func listOf(resolution: [CGSize]) -> [DataType] {
            var list = [DataType]()
            for size in resolution {
                let item = DataType.resolution(size)
                list.append(item)
            }
            return list
        }
    }
    
    @IBOutlet weak var titleLabel: UILabel!
    @IBOutlet weak var tableView: UITableView!
    
    private let bag = DisposeBag()
    var list: BehaviorRelay<[DataType]>?
    var selectedValue: BehaviorRelay<DataType>?
    
    override func viewDidLoad() {
        super.viewDidLoad()
        let color = UIColor(hexString: "#D8D8D8")
        let x: CGFloat = 15.0
        let width = UIScreen.main.bounds.width - (x * 2)
        self.titleLabel.containUnderline(color,
                                         x: x,
                                         width: width)
        self.titleLabel.text = self.title
        
        guard let list = list else {
            return
        }
        
        list.bind(to: tableView.rx.items(cellIdentifier: "MediaSettingsSubCell",
                                         cellType: MediaSettingsSubCell.self)) { index, data, cell in
                                            cell.nameLabel.text = data.description
                                            if let selectedValue = self.selectedValue {
                                                cell.isSelectedByDataSource = (selectedValue.value.description == data.description)
                                            }
        }.disposed(by: bag)
        
        tableView.rx.modelSelected(DataType.self).subscribe(onNext: { [unowned self] (data) in
            self.tableView.reloadData()
            self.selectedValue?.accept(data)
        }).disposed(by: bag)
    }
    
    @IBAction func doBackPressed(_ sender: UIButton) {
        self.navigationController?.popViewController(animated: true)
    }
}
