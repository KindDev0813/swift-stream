//
//  UserCamerasView.swift
//  VideoChat
//
//  Created by Alex Strup on 1/15/20.
//  Copyright © 2020 SW-Expert. All rights reserved.
//

import UIKit

@IBDesignable
class UserCamerasView: BaseXibView {
    
    @IBOutlet weak var titleLabel: UILabel!
    @IBOutlet weak var camera1View: UIView!
    @IBOutlet weak var camera2View: UIView!
    @IBOutlet weak var camera3View: UIView!
    @IBOutlet weak var camera4View: UIView!

    @IBInspectable var title: String = "" {
        didSet {
            titleLabel.text = title;
        }
    }

}
