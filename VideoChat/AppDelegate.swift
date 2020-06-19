//
//  AppDelegate.swift
//  VideoChat
//
//  Created by Alex Strup on 1/13/20.
//  Copyright © 2020 SW-Expert. All rights reserved.
//

import UIKit

@UIApplicationMain
class AppDelegate: UIResponder, UIApplicationDelegate {



    var window: UIWindow?

    func application(_ application: UIApplication, didFinishLaunchingWithOptions launchOptions: [UIApplication.LaunchOptionsKey: Any]?) -> Bool {
        let frame = UIScreen.main.bounds
        window = UIWindow(frame: frame)

        guard let rootVC = UIStoryboard(name: "Main", bundle: nil).instantiateInitialViewController() else { return true }
        
        window!.rootViewController = rootVC
        window!.makeKeyAndVisible()
        return true
    }


}

