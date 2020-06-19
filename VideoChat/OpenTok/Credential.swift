//
//  Credential.swift
//  VideoChat
//
//  Created by Alex Strup on 1/13/20.
//  Copyright © 2020 SW-Expert. All rights reserved.
//

import Foundation

struct Credential {
    
    let session: String;
    let subscriberToken: String;
    let publisherToken: String;

    init(session: String, subscriberToken: String, publisherToken: String) {
        self.session = session;
        self.publisherToken = publisherToken;
        self.subscriberToken = subscriberToken;
    }
}
