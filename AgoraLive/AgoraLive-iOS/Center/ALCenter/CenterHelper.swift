//
//  CenterHelper.swift
//  AgoraLive
//
//  Created by CavanSu on 2020/3/9.
//  Copyright © 2020 Agora. All rights reserved.
//

import Foundation
import AlamoClient

protocol CenterHelper where Self: ALCenter {
    func centerProvideLocalUser() -> CurrentUser
    func centerProvideRequestHelper() -> AlamoClient
    func centerProvideImagesHelper() -> ImageFiles
    func centerProvideMediaHelper() -> MediaKit
    func centerProvideFilesGroup() -> FilesGroup
    func centerProvideRTMHelper() -> RTMClient
    func centerProvideLogTubeHelper() -> LogTube
    func centerProvideUserDataHelper() -> UserDataHelper
    func centerProvideOSSClient() -> AGOSSClient
}
