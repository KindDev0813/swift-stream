//
//  OTSession.h
//
//  Copyright (c) 2014 Tokbox, Inc. All rights reserved.
//

#import <Foundation/Foundation.h>

@class OTError, OTConnection, OTPublisherKit, OTSubscriberKit, OTStream,
OTSessionCapabilities;

@protocol OTSessionDelegate;

/**
 * The connection status codes, available through 
 * <[OTSession sessionConnectionStatus]>.
 */
typedef NS_ENUM(int32_t, OTSessionConnectionStatus) {
    /**  The session is not connected. */
    OTSessionConnectionStatusNotConnected,
    /** The session is connected. */
    OTSessionConnectionStatusConnected,
    /** The session is connecting. */
    OTSessionConnectionStatusConnecting,
    /** The session is reconnecting. */
    OTSessionConnectionStatusReconnecting,
    /** The session is disconnecting. */
    OTSessionConnectionStatusDisconnecting,
    /** The session has experienced a fatal error.  */
    OTSessionConnectionStatusFailed,
};

/**
 * This is part of the configurable TURN feature, which is
 * available as an add-on feature. See the
 * <a href="https://tokbox.com/pricing" target="_blank">pricing page</a>.
 */
typedef NS_ENUM(int32_t, OTSessionICEIncludeServers) {
    /**
     * This is part of the configurable TURN feature, which is
     * available as an add-on feature. See the
     * <a href="https://tokbox.com/pricing" target="_blank">pricing page</a>.
     */
    OTSessionICEIncludeServersAll,
    
    /**
     * This is part of the configurable TURN feature, which is
     * available as an add-on feature. See the
     * <a href="https://tokbox.com/pricing" target="_blank">pricing page</a>.
     */
    OTSessionICEIncludeServersCustom,
};

/**
 * This is part of the configurable TURN feature, which is
 * available as an add-on feature. See the
 * <a href="https://tokbox.com/pricing" target="_blank">pricing page</a>.
 */
typedef NS_ENUM(int32_t, OTSessionICETransportPolicy) {
    /**
     * This is part of the configurable TURN feature, which is
     * available as an add-on feature. See the
     * <a href="https://tokbox.com/pricing" target="_blank">pricing page</a>.
     */
    OTSessionICETransportAll,
    
    /**
     * This is part of the configurable TURN feature, which is
     * available as an add-on feature. See the
     * <a href="https://tokbox.com/pricing" target="_blank">pricing page</a>.
     */
    OTSessionICETransportRelay,
};

/**
 * This is part of the configurable TURN feature, which is
 * available as an add-on feature. See the
 * <a href="https://tokbox.com/pricing" target="_blank">pricing page</a>.
 */
@interface OTSessionICEConfig : NSObject

// True :  OpenTok TURN servers won't be used.
// False (Default) : Use both OpenTok and Custom TURN servers. WebRTC will choose a turn server with best connectivity.
@property(nonatomic, assign) enum OTSessionICEIncludeServers includeServers;

// True :  Force connectivity through TURN server always.
// False : Use TURN sever as a fallback when direct connectivity to media server fails.
@property(nonatomic, assign) enum OTSessionICETransportPolicy transportPolicy;

// Returns an NSarry with TURN servers added by addICEServerWithURL:
// Each array row will have an NSDictionary with URL, UserName and Credential as keys.
@property(readonly) NSArray * _Nullable customIceServers;

// This will be a constant and we need to decide on maximum limit
+ (NSInteger) maxTURNServersLimit;

// The errorPtr will be populated with error description (and ICE server won't be added) if
// 1. URL is not valid
// 2. User name and/or credential empty
// 3. Maximum TURN servers limit reached already. (see maxUserTurnServersLimit)
// if there are no TURN servers added, it will use OpenTok internal servers
- (void) addICEServerWithURL:(NSString *_Nonnull)turn_url
                    userName:(NSString *_Nonnull)username
                  credential:(NSString *_Nonnull)credential
                       error:(NSError *_Nonnull*_Nullable)errorPtr;

@end

/**
 * Defines settings to be used when initializing an OTSession object using the
 * <[OTSession initWithApiKey:sessionId:delegate:settings:]> method.
 */
@interface OTSessionSettings : NSObject

/**
 * Prevent connection events (such as
 * <[OTSessionDelegate session:connectionCreated:]>) from being dispatched.
 *
 * The default value is NO.
 */
@property(nonatomic, assign) BOOL connectionEventsSuppressed;

/**
 * This is part of the configurable TURN feature, which is
 * available as an add-on feature. See the
 * <a href="https://tokbox.com/pricing" target="_blank">pricing page</a>.
 */
@property(nonatomic, strong) OTSessionICEConfig * _Nullable iceConfig;

/**
 * This feature is available for
 * <a href="https://tokbox.com/pricing" target="_blank">enterprise</a> accounts only.
 */
@property(nonatomic, strong) NSURL * _Nullable apiURL;

/**
 * Set this to <code>YES</code> if IP white listing is enabled for your project.
 * (This is available as an add-on feature for
 * <a href="https://tokbox.com/pricing" target="_blank">enterprise</a>
 * accounts.) The default value is <code>NO</code>.
 */
@property(nonatomic, assign) BOOL ipWhitelist;

/**
 * This property is part of a private beta program.
 */
@property(nonatomic, strong) NSString* _Nullable proxyURL;

@end

/**
 * The first step in using the OpenTok iOS SDK is to initialize
 * an OTSession object with your API key and a valid
 * [session ID](http://tokbox.com/opentok/tutorials/create-session)
 * Use the OTSession object to connect to OpenTok using your developer
 * [API key](https://tokbox.com/account) and a valid
 * [token](http://tokbox.com/opentok/tutorials/create-token).
 */
@interface OTSession : NSObject

- (instancetype)init NS_UNAVAILABLE;

/** @name Getting information about the session */

/**
 * The status of this OTSession instance. Useful for ad-hoc queries about
 * session status.
 *
 * Valid values are defined in OTSessionConnectionStatus:
 *
 * - `OTSessionConnectionStatusNotConnected` - The session is not connected.
 * - `OTSessionConnectionStatusConnected` - The session is connected.
 * - `OTSessionConnectionStatusConnecting` - The session is connecting.
 * - `OTSessionConnectionStatusDisconnecting` - The session is disconnecting.
 * - `OTSessionConnectionStatusFailed` - The session has experienced a fatal
 *    error 
 *
 * On instantiation, expect the `sessionConnectionStatus` to have the value 
 * `OTSessionConnectionStatusNotConnected`.
 *
 * You can use a key-value observer to monitor this property. However, the 
 * <[OTSessionDelegate sessionDidConnect:]>
 * and <[OTSessionDelegate sessionDidDisconnect:]> messages are sent to the
 * session's delegate when the session
 * connects and disconnects.
 */
@property(readonly) OTSessionConnectionStatus sessionConnectionStatus;

/**
 * The [session ID](http://tokbox.com/opentok/tutorials/create-session)
 * of this instance. This is an immutable value.
 */
@property(readonly) NSString* _Nonnull sessionId;

/**
 * The streams that are a part of this session, keyed by streamId.
 */
@property(readonly) NSDictionary<NSString*, OTStream*>* _Nonnull streams;

/**
 * The <OTConnection> object for this session. The connection property is only
 * available
 * once the <[OTSessionDelegate sessionDidConnect:]> message is sent. If the 
 * session fails to connect,
 * this property shall remain nil.
 */
@property(readonly) OTConnection* _Nullable connection;

/**
 * The <OTSessionDelegate> object that serves as a delegate object for this
 * OTSession object,
 * handling messages on behalf of this session.
 */
@property(nonatomic, assign) id<OTSessionDelegate> _Nullable delegate;

/**
 * The delegate callback queue is application-definable. The GCD queue for
 * issuing callbacks to the delegate may be overridden to allow integration with
 * XCTest (new in XCode 5) or other frameworks that need the to operate in the
 * main thread.
 */
@property(nonatomic, assign) dispatch_queue_t _Nonnull apiQueue;

/**
 * An <OTSessionCapabilities> object, which indicates whether the client can
 * publish and subscribe to streams in the session, based on the role assigned
 * to the token used to connect to the session. This property is set to `nil`
 * until you have connected to a session and the
 * <[OTSessionDelegate sessionDidConnect:]> method has been called.
 */
@property(readonly) OTSessionCapabilities* _Nullable capabilities;

/** @name Initializing and connecting to a session */

/**
 * Initialize this session with your OpenTok API key , a
 * [session ID](http://tokbox.com/opentok/tutorials/create-session),
 * and delegate before connecting to OpenTok. Send the
 * <[OTSession connectWithToken:error:]> message
 * to connect to the session.
 *
 * @param apiKey Your OpenTok API key.
 * @param sessionId The session ID of this instance.
 * @param delegate The delegate (OTSessionDelegate) that handles messages on
 * behalf of this session.
 *
 * @return The OTSession object, or nil if initialization fails.
 */
- (nullable id)initWithApiKey:(nonnull NSString*)apiKey
                    sessionId:(nonnull NSString*)sessionId
                     delegate:(nullable id<OTSessionDelegate>)delegate;

/**
 * Initialize the OTSession object with settings defined by an
 * <OTSessionSettings> object.
 *
 * @param apiKey Your OpenTok API key.
 *
 * @param sessionId The session ID of this instance.
 *
 * @param delegate The delegate (<OTSessionDelegate>) object for the
 * session.
 *
 * @param settings The (<OTSessionSettings>) object that defines settings
 * for the session.
 */
- (nullable id)initWithApiKey:(nonnull NSString*)apiKey
                    sessionId:(nonnull NSString*)sessionId
                     delegate:(nullable id<OTSessionDelegate>)delegate
                     settings:(nullable OTSessionSettings *)settings;

/**
 * Once your application has a valid 
 * [token]( http://tokbox.com/opentok/tutorials/create-token ),
 * connect with your [API key](https://tokbox.com/account) to begin
 * participating in an OpenTok session.
 *
 * When the session connects successfully, the
 * <[OTSessionDelegate sessionDidConnect:]> message is sent to
 * the session's delegate.
 *
 * If the session cannot connect, the
 * <[OTSessionDelegate session:didFailWithError:]> message is sent to
 * the session's delegate.
 *
 * When the session disconnects, the <[OTSessionDelegate sessionDidDisconnect:]>
 * message is sent to the session's delegate.
 *
 * Note that sessions automatically disconnect when the app is suspended.
 *
 * Be sure to set up a delegate method for the 
 * <[OTSessionDelegate session:didFailWithError:]> message.
 *
 * @param token The token generated for this connection.
 *
 * @param error Set if an error occurs synchronously while processing the
 * request. The `OTSessionErrorCode` enum (defined in the OTError.h file)
 * defines values for the `code` property of this object. This object is NULL
 * if no synchronous error occurs.
 *
 * If an asynchronous error occurs, the
 * <[OTSessionDelegate session:didFailWithError:]> message is sent to
 * the session's delegate.
 */
- (void)connectWithToken:(nonnull NSString*)token
                   error:(OTError* _Nullable* _Nullable)error;

/**
 * Disconnect from an active OpenTok session.
 *
 * This method tears down all OTPublisher and OTSubscriber objects that have 
 * been initialized.
 *
 * When the session disconnects, the <[OTSessionDelegate sessionDidDisconnect:]>
 * message is sent to the
 * session's delegate.
 *
 * @param error Set if an error occurs synchronously while processing the
 * request. The `OTSessionErrorCode` enum (defined in the OTError.h file)
 * defines values for the `code` property of this object. This object is NULL
 * if no error occurs.
 */
- (void)disconnect:(OTError* _Nullable* _Nullable)error;

- (void)disconnect
__attribute__((deprecated("use disconnect: instead")));

/** @name Publishing audio-video streams to a session */

/**
 * Adds a publisher to the session.
 *
 * When the publisher begins streaming data, the 
 * <[OTPublisherKitDelegate publisher:streamCreated:]> message
 * is sent to the publisher delegate delegate.
 *
 * If publishing fails, 
 * <[OTPublisherKitDelegate publisher:didFailWithError:]>
 * is sent to the publisher delegate and no session delegate message will be
 * passed.
 *
 * Note that multiple publishers are not supported.
 *
 * @param publisher The <OTPublisherKit> object to stream with.
 *
 * @param error Set if an error occurs synchronously while processing the
 * request. The `OTPublisherErrorCode` enum (defined in the OTError.h file)
 * defines values for the `code` property of this object. This object is NULL
 * if no error occurs.
 *
 * If an asynchronous error occurs, the
 * <[OTPublisherKitDelegate publisher:didFailWithError:]> message is sent to
 * the publisher's delegate.
 */
- (void)publish:(nonnull OTPublisherKit*)publisher
          error:(OTError* _Nullable* _Nullable)error;

- (void)publish:(nonnull OTPublisherKit*)publisher
__attribute__((deprecated("use publish:error: instead")));

/**
 * Removes a publisher from the session.
 *
 * Upon removing the publisher, the 
 * <[OTPublisherKitDelegate publisher:streamDestroyed:]> message is sent
 * to the publisher delegate after streaming has stopped.
 *
 * @param publisher The <OTPublisher> object to remove from the session.
 *
 * @param error Set if an error occurs synchronously while processing the
 * request. The `OTPublisherErrorCode` enum (defined in the OTError.h file)
 * defines values for the `code` property of this object. This object is NULL
 * if no error occurs.
 */
- (void)unpublish:(nonnull OTPublisherKit*)publisher
            error:(OTError* _Nullable* _Nullable)error;

- (void)unpublish:(nonnull OTPublisherKit*)publisher
__attribute__((deprecated("use unpublish:error: instead")));

/** @name Subscribing to audio-video streams */

/**
 * Connects this subscriber instance to the session and begins subscribing.
 * If the subscriber passed is created from an `OTStream` instance from a 
 * different `OTSession` instance, the behavior of this function is undefined.
 *
 * @param subscriber The subscriber to connect and begin subscribing.
 *
* @param error Set if an error occurs synchronously while processing the
 * request. The `OTSubscriberErrorCode` enum (defined in the OTError.h file)
 * defines values for the `code` property of this object. This object is NULL
 * if no error occurs.
 *
 * If an asynchronous error occurs, the
 * <[OTSubscriberKitDelegate subscriber:didFailWithError:]> message is sent to
 * the subscriber's delegate.
 */
- (void)subscribe:(nonnull OTSubscriberKit*)subscriber
            error:(OTError* _Nullable* _Nullable)error;

- (void)subscribe:(nonnull OTSubscriberKit*)subscriber
__attribute__((deprecated("use subscribe:error: instead")));

/**
 * Disconnects this subscriber instance from the session and begins object 
 * cleanup.
 * @param subscriber The subscriber to disconnect and remove from this session.
 *
 * @param error Set if an error occurs synchronously while processing the
 * request. The `OTSubscriberErrorCode` enum (defined in the OTError.h file)
 * defines values for the `code` property of this object. This object is NULL
 * if no error occurs.
 */
- (void)unsubscribe:(nonnull OTSubscriberKit*)subscriber
              error:(OTError* _Nullable* _Nullable)error;

- (void)unsubscribe:(nonnull OTSubscriberKit*)subscriber
__attribute__((deprecated("use unsubscribe:error: instead")));

/** @name Sending and receiving signals in a session */

/**
 * Sends a signal to one or more clients in a session.
 *
 * See <[OTSession signalWithType:string:connection:retryAfterReconnect:error:]>
 * and
 * <[OTSessionDelegate session:receivedSignalType:fromConnection:withString:]>.
 *
 * @param type The type of the signal. The type is also set in the
 * <[OTSessionDelegate session:receivedSignalType:fromConnection:withString:]>
 * message. The maximum length of the type string is 128 characters, and it must
 * contain only letters (A-Z and a-z), numbers (0-9), "-", "_", and "~".
 *
 * @param string The data to send. The limit to the size of data is 8KB.
 *
 * @param connection A destination OTConnection object.
 * Set this parameter to nil to signal all participants in the session.
 *
 * @param error If sending a signal fails, this value is set to an OTError
 * object. The OTSessionErrorCode enum (in OTError.h) includes
 * OTSessionInvalidSignalType and OTSessionSignalDataTooLong constants for these
 * errors. Note that success indicates that the options passed into the method
 * are valid and the signal was sent. It does not indicate that the signal was
 * sucessfully received by any of the intended recipients.
 */
- (void)signalWithType:(NSString* _Nullable)type
                string:(NSString* _Nullable)string
            connection:(OTConnection* _Nullable)connection
                 error:(OTError* _Nullable* _Nullable)error;

/**
* Sends a signal to one or more clients in a session. This version of the method
* includes a <code>retryAfterReconnect</code> parameter.
*
* @param type The type of the signal. The type is also set in the
* <[OTSessionDelegate session:receivedSignalType:fromConnection:withString:]>
* message. The maximum length of the type string is 128 characters, and it must
* contain only letters (A-Z and a-z), numbers (0-9), "-", "_", and "~".
*
* See <[OTSession signalWithType:string:connection:retryAfterReconnect:error:]>,
* <[OTSessionDelegate session:receivedSignalType:fromConnection:withString:]>,
* and <[OTSessionDelegate sessionDidBeginReconnecting:]>.
*
* @param string The data to send. The limit to the size of data is 8KB.
*
* @param connection A destination OTConnection object.
* Set this parameter to nil to signal all participants in the session.
*
* @param retryAfterReconnect Upon reconnecting to the session, whether to send
* any signals that were initiated while disconnected. If your client loses its
* connection to the OpenTok session, due to a drop in network connectivity, the
* client attempts to reconnect to the session, and the
* <[OTSessionDelegate sessionDidBeginReconnecting:]> message is sent.
* By default, signals initiated while disconnected are sent when (and if)
* the client reconnects to the OpenTok session. You can prevent this by setting
* the <code>retryAfterReconnect</code> parameter to <code>false</code>.
* (The default value is <code>true</code>.)
*
* @param error If sending a signal fails, this value is set to an OTError
* object. The OTSessionErrorCode enum (in OTError.h) includes
* OTSessionInvalidSignalType and OTSessionSignalDataTooLong constants for these
* errors. Note that success indicates that the options passed into the method
* are valid and the signal was sent. It does not indicate that the signal was
* sucessfully received by any of the intended recipients.
*/
- (void)signalWithType:(NSString* _Nullable) type
                string:(NSString* _Nullable)string
            connection:(OTConnection* _Nullable)connection
   retryAfterReconnect:(BOOL)retryAfterReconnect
                 error:(OTError* _Nullable* _Nullable)error;

/** @name Reporting an issue */

/**
 * Report that your app experienced an issue. You can use the issue ID with the
 * <a href="https://tokbox.com/developer/tools/Inspector">Inspector</a> or when
 * discussing an issue with the TokBox support team.
 *
 * @param issueId A pointer to a string that will be set the unique identifier
 * for the reported issue. If the call to the method fails (for example, because
 * of no network connection), this value is set to nil.
 */
- (void)reportIssue:(NSString* _Nullable* _Nullable)issueId;

@end

/**
 * Used to send messages for an OTSession instance. The OTSession class 
 * includes a
 * `delegate` property. When you send the 
 * <[OTSession initWithApiKey:sessionId:delegate:]> message,
 * you specify an OTSessionDelegate object.
 */
@protocol OTSessionDelegate <NSObject>

/** @name Connecting to a session */

/**
 * Sent when the client connects to the session.
 *
 * @param session The <OTSession> instance that sent this message.
 */
- (void)sessionDidConnect:(nonnull OTSession*)session;

/**
 * Sent when the client disconnects from the session.
 *
 * @param session The <OTSession> instance that sent this message.
 */
- (void)sessionDidDisconnect:(nonnull OTSession*)session;

/**
 * Sent if the attempt to connect to the session fails or if the connection
 * to the session drops due to an error after a successful connection.
 *
 * This message is sent after your application calls
 * <[OTSession connectWithToken:error:]>.
 *
 * If this message is sent because the connection to the session drops after
 * a successful connection, the message is sent just before the
 * <[OTSessionDelegate sessionDidDisconnect:]> message is sent.
 *
 * @param session The <OTSession> instance that sent this message.
 * @param error An <OTError> object describing the issue. The
 * <OTSessionErrorCode> enum defines values for the `code` property of
 * this object.
 */
- (void)session:(nonnull OTSession*)session
didFailWithError:(nonnull OTError*)error;

/** @name Monitoring streams in a session */

/**
 * Sent when a new stream is created in this session.
 *
 * Note that if your application publishes to this session, your own session
 * delegate will not receive the [OTSessionDelegate session:streamCreated:]
 * message for its own published stream. For that event, see the delegate
 * callback [OTPublisherKitDelegate publisher:streamCreated:].
 *
 * @param session The OTSession instance that sent this message.
 * @param stream The stream associated with this event.
 */
- (void)session:(nonnull OTSession*)session
  streamCreated:(nonnull OTStream*)stream;

/**
 * Sent when a stream is no longer published to the session.
 *
 * @param session The <OTSession> instance that sent this message.
 * @param stream The stream associated with this event.
 */
- (void)session:(nonnull OTSession*)session
streamDestroyed:(nonnull OTStream*)stream;

@optional

/** @name Monitoring connections in a session */

/**
 * Sent when another client connects to the session. The `connection` object
 * represents the client's connection.
 *
 * This message is not sent when your own client connects to the session. 
 * Instead, the <[OTSessionDelegate sessionDidConnect:]>
 * message is sent when your own client connects to the session.
 *
 * @param session The <OTSession> instance that sent this message.
 * @param connection The new <OTConnection> object.
 */
- (void)  session:(nonnull OTSession*) session
connectionCreated:(nonnull OTConnection*) connection;

/**
 * Sent when another client disconnects from the session. The `connection`
 * object represents the connection that the client had to the session.
 *
 * This message is not sent when your own client disconnects from the session. 
 * Instead, the <[OTSessionDelegate sessionDidDisconnect:]>
 * message is sent when your own client connects to the session.
 *
 * @param session The <OTSession> instance that sent this message.
 * @param connection The <OTConnection> object for the client that disconnected
 * from the session.
 */
- (void)    session:(nonnull OTSession*) session
connectionDestroyed:(nonnull OTConnection*) connection;

/**
 * Sent when a message is received in the session.
 * @param session The <OTSession> instance that sent this message.
 * @param type The type string of the signal.
 * @param connection The connection identifying the client that sent the
 * message. This value can be `nil`.
 * @param string The signal data.
 */
- (void)   session:(nonnull OTSession*)session
receivedSignalType:(NSString* _Nullable)type
    fromConnection:(OTConnection* _Nullable)connection
        withString:(NSString* _Nullable)string;

/** @name Monitoring archiving events */

/**
 * Sent when an archive recording of a session starts. If you connect to a
 * session in which recording is already in progress, this message is sent
 * when you connect.
 *
 * In response to this message, you may want to add a user interface
 * notification (such as an icon in the Publisher view) that indicates
 * that the session is being recorded.
 *
 * For more information see the OpenTok
 * [Archiving Overview](http://www.tokbox.com/opentok/tutorials/archiving).
 *
 * @param session The <OTSession> instance that sent this message.
 * @param archiveId The unique ID of the archive.
 * @param name The name of the archive (if one was provided when the archive
 * was created).
 */
- (void)     session:(nonnull OTSession*)session
archiveStartedWithId:(nonnull NSString*)archiveId
                name:(NSString* _Nullable)name;

/**
 * Sent when an archive recording of a session stops.
 *
 * In response to this message, you may want to change or remove a user
 * interface notification (such as an icon in the Publisher view) that
 * indicates that the session is being recorded.
 *
 * For more information, see the OpenTok
 * [Archiving Overview](http://www.tokbox.com/opentok/tutorials/archiving).
 *
 * @param session The <OTSession> instance that sent this message.
 * @param archiveId The unique ID of the archive.
 */
- (void)     session:(nonnull OTSession*)session
archiveStoppedWithId:(nonnull NSString*)archiveId;

/** @name Reconnecting to a session */

/**
 * Sent when the local client has lost its connection to an OpenTok session and
 * is trying to reconnect. This results from a loss in network connectivity.
 * If the client can reconnect to the session, the
 * <[OTSessionDelegate sessionDidReconnect:]> message is sent. Otherwise, if the
 * client cannot reconnect, the <[OTSessionDelegate sessionDidDisconnect:]>
 * message is sent.
 *
 * In response to this message being sent, you may want to provide a user
 * interface notification, to let the user know that the app is trying to
 * reconnect to the session and that audio-video streams are temporarily
 * disconnected.
 *
 * @param session The <OTSession> instance that sent this message.
 */
- (void)sessionDidBeginReconnecting:(nonnull OTSession*)session;

/**
 * Sent when the local client has reconnected to the OpenTok session after its
 * network connection was lost temporarily. When the connection is lost, the
 * <[OTSessionDelegate sessionDidBeginReconnecting:]> message is sent, prior to
 * the <code>[OTSessionDelegate sessionDidReconnect:]</code> messsage.
 * If the client cannot reconnect to the session, the
 * <[OTSessionDelegate sessionDidDisconnect:]> message is sent.
 *
 * Any existing publishers and subscribers are automatically reconnected when
 * the client reconnects and this message is sent.
 *
 * By default, any signals initiated by the the local client using the
 * <[OTSession signalWithType:string:connection:error:]> method are sent when
 * the client reconnects. To prevent any signals initiated while disconnected
 * from being sent, use the
 * <[OTSession signalWithType:string:connection:retryAfterReconnect:error:]>
 * method to send the signal, and set the <code>retryAfterReconnect</code>
 * parameter to <code>NO</code>. (All signals sent by other clients while your
 * client was disconnected are received upon reconnecting.)
 *
 * See <[OTSessionDelegate sessionDidBeginReconnecting:]>.
 *
 * @param session The <OTSession> instance that sent this message.
 */
- (void)sessionDidReconnect:(nonnull OTSession*)session;

@end

/**
 * Defines the <[OTSession capabilities]> property. After you have
 * connected to a session and the <[OTSessionDelegate sessionDidConnect:]>
 * method has been called, this property identifies whether the client can
 * publish and subscribe to streams in the session, based on the role assigned
 * to the token used to connect to the session. For example, if you connect to a
 * session using a token that is assigned a Subscriber role, the `canPublish`
 * property of this object is set to `NO` (and the client does not have
 * privileges to publish to the session). For more information, see the OpenTok
 * [token creation](https://tokbox.com/developer/guides/create-token/)
 * documentation.
 */
@interface OTSessionCapabilities : NSObject
/**
 * Whether the client can publish streams to the session (`YES`) or not (`NO`).
 * This is set to `NO` for clients that connect to a session using a token that
 * is assigned a Subscriber role.
 */
@property (readonly) BOOL canPublish;
/**
 * Whether the client can subscribe to streams in the session (`YES`) or not
 * (`NO`). This is set to `YES` for all clients (since all tokens allow clients
 * to subscribe to streams).
 */
@property (readonly) BOOL canSubscribe;

- (nonnull instancetype)initWithCanPublish:(BOOL)publish
                      canSubscribe:(BOOL)subscribe;
@end
