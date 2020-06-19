//
//  UserSelectorVC.swift
//  VideoChat
//
//  Created by Alex Strup on 1/13/20.
//  Copyright © 2020 SW-Expert. All rights reserved.
//

import UIKit
import AVFoundation

class UserSelectorVC: UIViewController {
    
    @IBOutlet weak var user1Button: UIButton!
    @IBOutlet weak var user2Button: UIButton!
    @IBOutlet weak var confirtButton: UIButton!
    
    var allCameraConfig: [CameraSessionConfig] = []

    override func viewDidLoad() {
        super.viewDidLoad()
        // Do any additional setup after loading the view.
    }
    
    func getCountCameras() -> Int {
        let discoverySession = AVCaptureDevice.DiscoverySession(deviceTypes: [.builtInWideAngleCamera], mediaType: AVMediaType.video, position: .unspecified)
        let result = discoverySession.devices.count
        return result
    }
    
    func setupOpenTokConfig(isUser1: Bool) {
        self.allCameraConfig.removeAll()
        
        let publisherStartIndex = isUser1 ? 0: Constants.maxCountCameras
        let subscriberStartIndex = isUser1 ? Constants.maxCountCameras:  0
        let countCameras = Constants.сountCameras
        var cameraIndex = 0

        // foreach credentials for publisher
        for index in publisherStartIndex..<(publisherStartIndex + countCameras) {
            let credential = OpenTokConfig.credentials[index]
            let cameraConfig = CameraSessionConfig(apiKey: OpenTokConfig.defaultApiKey,
                                                   cameraIndex: cameraIndex,
                                                   session: credential.session,
                                                   token: credential.publisherToken,
                                                   isPublisher: true)
            self.allCameraConfig.append(cameraConfig)
            cameraIndex += 1
        }

        // foreach credentials for subscriber
        for index in subscriberStartIndex..<(subscriberStartIndex + Constants.сountCameras) {
            let credential = OpenTokConfig.credentials[index]
            let cameraConfig = CameraSessionConfig(apiKey: OpenTokConfig.defaultApiKey,
                                                    cameraIndex: index,
                                                    session: credential.session,
                                                    token: credential.subscriberToken,
                                                    isPublisher: false)
            self.allCameraConfig.append(cameraConfig)
            cameraIndex += 1
        }
        
    }
    
    @IBAction func selectUser1() {
        setupOpenTokConfig(isUser1: true)
        
        user1Button.isSelected = true
        user2Button.isSelected = false
        confirtButton.isEnabled = true
    }
    
    @IBAction func selectUser2() {
        setupOpenTokConfig(isUser1: false)
        
        user1Button.isSelected = false
        user2Button.isSelected = true
        confirtButton.isEnabled = true
    }
    
    @IBAction func confirmAction() {
        let storyboard = UIStoryboard(name: Constants.Storyboard.main, bundle: Bundle.main)
        if let videoVC = storyboard.instantiateViewController(withIdentifier: Constants.ViewController.videoVC) as? (VideoVC) {
            videoVC.allCameraConfig = self.allCameraConfig
            self.navigationController?.pushViewController(videoVC, animated: true)
        }
        
    }

}

