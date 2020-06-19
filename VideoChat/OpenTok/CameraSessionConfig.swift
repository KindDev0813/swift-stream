//
//  CameraSessionConfig.swift
//  VideoChat
//
//  Created by Alex Strup on 1/13/20.
//  Copyright © 2020 SW-Expert. All rights reserved.
//

import OpenTok
import UIKit

struct CameraSessionConfig {
    
    let apiKey: String
    let cameraIndex: Int
    var view: UIView?

    var sessionId: String
    var token: String
    var session: OTSession?
    private (set) var publisher: OTPublisher?
    private (set) var subscriber: OTSubscriber?

    var isPublisher: Bool
    var error: OTError?
    
    init(apiKey: String, cameraIndex: Int, session: String, token: String, isPublisher: Bool) {
        self.apiKey = apiKey
        self.cameraIndex = cameraIndex
        
        self.sessionId = session
        self.token = token
        self.isPublisher = isPublisher
    }
    
    mutating func createPublisher(delegate: OTPublisherKitDelegate?, settings: OTPublisherSettings) {
        self.publisher = OTPublisher(delegate: delegate, settings: settings)
        guard let publisher = self.publisher else { return }
        error = nil
        self.session?.publish(publisher, error: &error)
        guard error == nil else {
            print("Error publish for inder \(cameraIndex): \(error!)")
            return
        }
    }
    
    mutating func createSubscriber(delegate: OTSubscriberKitDelegate?, stream: OTStream) {
        self.subscriber = OTSubscriber(stream: stream, delegate: delegate)
        guard let subscriber = self.subscriber else { return }
        error = nil
        self.session?.subscribe(subscriber, error: &error)
        guard error == nil else {
            print("Error publish for inder \(cameraIndex): \(error!)")
            return
        }
    }
    
    mutating func clear() {
        if self.isPublisher {
            self.publisher?.view?.removeFromSuperview()
        } else {
            self.subscriber?.view?.removeFromSuperview()
        }
        self.session = nil
        self.publisher = nil
        self.subscriber = nil
        self.view = nil
        self.error = nil
    }
}
