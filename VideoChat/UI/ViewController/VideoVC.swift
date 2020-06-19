//
//  VideoVC.swift
//  VideoChat
//
//  Created by Alex Strup on 1/13/20.
//  Copyright © 2020 SW-Expert. All rights reserved.
//

import UIKit
import OpenTok

class VideoVC: UIViewController {

    @IBOutlet weak var myCamerasView: UserCamerasView!
    @IBOutlet weak var interlocutorCamerasView: UserCamerasView!
    var allCameraConfig: [CameraSessionConfig] = []
    
    override func viewDidLoad() {
        super.viewDidLoad()

        
    }
    
    override func viewWillDisappear(_ animated: Bool) {
        super.viewWillDisappear(animated)
        
        for index in 0..<allCameraConfig.count {
            allCameraConfig[index].clear()
        }
    }
    
    override func viewDidAppear(_ animated: Bool) {
        super.viewDidAppear(animated)
        
        connectToAnOpenTokSessions()
    }
    

    func connectToAnOpenTokSessions() {
        var error: OTError?
        for index in 0..<allCameraConfig.count {
            error = nil
            let config = allCameraConfig[index]
            allCameraConfig[index].view = getCameraWrapper(config: config)
            allCameraConfig[index].session = OTSession(apiKey: config.apiKey, sessionId: config.sessionId, delegate: self)
            allCameraConfig[index].session?.connect(withToken: config.token, error: &error)
            if error != nil {
                print(error!)
            }
        }
    }
    
    func createPublisher(config: inout CameraSessionConfig) {
        let settings = OTPublisherSettings()
        settings.name = UIDevice.current.name
        config.createPublisher(delegate: self, settings: settings)
        guard config.publisher != nil,
            config.error == nil,
            let wrapperView = config.view,
            let publisherView = config.publisher?.view else {
            return
        }
        
        config.publisher?.cameraPosition = config.cameraIndex.isMultiple(of: Constants.сountCameras)
            ? .front
            : .back

        publisherView.frame = CGRect(origin: CGPoint(x: 0, y: 0), size: wrapperView.frame.size )
        wrapperView.addSubview(publisherView)
    }
    
    func createSubscriber(config: inout CameraSessionConfig, stream: OTStream) {
        config.createSubscriber(delegate: self, stream: stream)
        guard config.subscriber != nil,
            config.error == nil,
            let wrapperView = config.view,
            let subscriberView = config.subscriber?.view else {
            return
        }

        subscriberView.frame = CGRect(origin: CGPoint(x: 0, y: 0), size: wrapperView.frame.size )
        wrapperView.addSubview(subscriberView)
    }
    
    func getCameraWrapper(config: CameraSessionConfig) -> UIView? {
        let userCamerasView = config.isPublisher ? myCamerasView : interlocutorCamerasView
        
        switch config.cameraIndex % Constants.maxCountCameras {
        case 0:
            return userCamerasView?.camera1View
        case 1:
            return userCamerasView?.camera2View
        case 2:
            return userCamerasView?.camera3View
        case 3:
            return userCamerasView?.camera4View
        default:
            return nil
        }
    }
    
    func findCameraConfig(by session: OTSession) -> CameraSessionConfig? {
        let index = self.allCameraConfig.firstIndex { (cameraConfig) -> Bool in
            cameraConfig.session == session
        }
        guard index != nil else { return nil }
        return self.allCameraConfig[index!]
    }
    
    func findCameraConfig(by publisher: OTPublisherKit) -> CameraSessionConfig? {
        let index = self.allCameraConfig.firstIndex { (cameraConfig) -> Bool in
            cameraConfig.publisher == publisher
        }
        guard index != nil else { return nil }
        return self.allCameraConfig[index!]
    }
    
    func findCameraConfig(by subscriber: OTSubscriberKit) -> CameraSessionConfig? {
        let index = self.allCameraConfig.firstIndex { (cameraConfig) -> Bool in
            cameraConfig.subscriber == subscriber
        }
        guard index != nil else { return nil }
        return self.allCameraConfig[index!]
    }

}

// MARK: - OTSessionDelegate callbacks
extension VideoVC: OTSessionDelegate {
    
    func sessionDidConnect(_ session: OTSession) {
        print("The client connected to the OpenTok session.")
        guard var cameraConfig = findCameraConfig(by: session) else { return }
        if cameraConfig.isPublisher {
            createPublisher(config: &cameraConfig)
        }
    }

    func sessionDidDisconnect(_ session: OTSession) {
        print("The client disconnected from the OpenTok session.")
    }

    func session(_ session: OTSession, didFailWithError error: OTError) {
        print("The client failed to connect to the OpenTok session: \(error).")
        guard var cameraConfig = findCameraConfig(by: session) else { return }
        cameraConfig.clear()
    }

    func session(_ session: OTSession, streamCreated stream: OTStream) {
        print("A stream was created in the session.")
        guard var cameraConfig = findCameraConfig(by: session) else { return }
        if !(cameraConfig.isPublisher) {
            createSubscriber(config: &cameraConfig, stream: stream)
        }
    }

    func session(_ session: OTSession, streamDestroyed stream: OTStream) {
        print("A stream was destroyed in the session.")
        guard var cameraConfig = findCameraConfig(by: session) else { return }
        cameraConfig.clear()
        
    }
}

// MARK: - OTPublisherDelegate callbacks
extension VideoVC: OTPublisherDelegate {
    func publisher(_ publisher: OTPublisherKit, didFailWithError error: OTError) {
        print("The publisher failed: \(error)")
        guard var cameraConfig = findCameraConfig(by: publisher) else { return }
        cameraConfig.clear()
    
    }
}

// MARK: - OTSubscriberDelegate callbacks
extension VideoVC: OTSubscriberDelegate {
   public func subscriberDidConnect(toStream subscriber: OTSubscriberKit) {
       print("The subscriber did connect to the stream.")
   }

   public func subscriber(_ subscriber: OTSubscriberKit, didFailWithError error: OTError) {
       print("The subscriber failed to connect to the stream.")
    guard var cameraConfig = findCameraConfig(by: subscriber) else { return }
    cameraConfig.clear()
    
   }
}
