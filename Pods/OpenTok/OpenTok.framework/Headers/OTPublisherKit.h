//
//  OTPublisherKit.h
//
//  Copyright (c) 2014 Tokbox, Inc. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <AVFoundation/AVFoundation.h>

@class OTPublisherKit, OTSession, OTError, OTStream, OTPublisherKitVideoNetworkStats, OTPublisherKitAudioNetworkStats;

@protocol OTVideoCapture;
@protocol OTVideoRender;
@protocol OTPublisherKitDelegate;
@protocol OTPublisherKitAudioLevelDelegate;
@protocol OTPublisherKitNetworkStatsDelegate;

/**
* Defines values for the <[OTPublisherKit videoType]> property.
*/
typedef NS_ENUM(int32_t, OTPublisherKitVideoType) {
    /**
     * Defines the camera video mode setting for the
     * <[OTPublisherKit videoType]> property.
     */
    OTPublisherKitVideoTypeCamera = 1,
    /**
     * Defines the screen video mode setting for the
     * <[OTPublisherKit videoType]> property.
     */
    OTPublisherKitVideoTypeScreen = 2,
};

/**
 * Defines settings to be used when initializing a publisher using the
 * <[OTPublisherKit initWithDelegate:settings:]> method.
 */
@interface OTPublisherKitSettings : NSObject

/** @name Defining publisher settings */

/**
 * The name of the publisher video. The <[OTStream name]> property
 * for a stream published by this publisher will be set to this value
 * (on all clients). The default value is `nil`.
 */
@property(nonatomic, copy, nullable) NSString *name;

/**
 * Whether to publish audio (YES, the default) or not (NO).
 *
 * If this property is set to NO, the audio subsystem will not be initialized
 * for the publisher, and setting the <[OTPublisherKit publishAudio]> property
 * will have no effect. If your application does not require the use of audio,
 * it is recommended to set this Builder property rather than use the
 * <[OTPublisherKit publishAudio]> property, which only temporarily disables
 * the audio track.
 */
@property(nonatomic) BOOL audioTrack;

/**
 * Whether to publish video (YES, the default) or not (NO).
 *
 * If this property is set to NO, the video subsystem will not be initialized
 * for the publisher, and setting the <[OTPublisherKit publishVideo]> property
 * will have no effect. If your application does not require the use of video,
 * it is recommended to set this Builder property rather than use the
 * <[OTPublisherKit publishVideo]> property, which only temporarily disables
 * the video track.
 */
@property(nonatomic) BOOL videoTrack;

/**
 * The desired bitrate for the published audio, in bits per second.
 * The supported range of values is 6,000 - 510,000. (Invalid values are
 * ignored.) Set this value to enable high-quality audio (or to reduce
 * bandwidth usage with lower-quality audio).
 *
 * The following are recommended settings:
 *
 * * 8,000 - 12,000 for narrowband (NB) speech
 * * 16,000 - 20,000 for wideband (WB) speech
 * * 28,000 - 40,000 for full-band (FB) speech
 * * 48,000 - 64,000 for full-band (FB) mono music
 * * 64,000 - 128,000 for full-band (FB) stereo music
 *
 * The default value is 40,000.
 *
 * To set other audio settings, see the <OTAudioDeviceManager> interface.
 */
@property(nonatomic) int audioBitrate;

@end

/**
 * A publisher captures an audio-video stream from the sources you specify. You
 * can then publish the audio-video stream to an OpenTok session by sending the
 * <[OTSession publish:error:]> message.
 *
 * The OpenTok iOS SDK supports publishing on all multi-core iOS devices.
 * See "Developer and client requirements" in the README file for the
 * [OpenTok iOS SDK](http://tokbox.com/opentok/libraries/client/ios).
 */
@interface OTPublisherKit : NSObject

- (instancetype)init NS_UNAVAILABLE;

/** @name Initializing a publisher */

/**
 * Initialize a publisher object and specify the delegate object.
 *
 * When running in the XCode iOS Simulator, this method returns `nil`.
 *
 * @param delegate The delegate (<OTPublisherKitDelegate>) object for the
 * publisher.
 *
 * @return The pointer to the instance, or `nil` if initialization failed.
 */
- (nullable instancetype)initWithDelegate:(nullable id<OTPublisherKitDelegate>)delegate;

/**
 * Initialize the publisher with settings defined by an
 * <OTPublisherKitSettings> object.
 *
 * @param delegate The delegate (<OTPublisherKitDelegate>) object for the
 * publisher.
 *
 * @param settings The (<OTPublisherKitSettings>) object that defines settings
 * for the publisher.
 */
-(nullable instancetype)initWithDelegate:(nullable id<OTPublisherKitDelegate>)delegate
                       settings:(nonnull OTPublisherKitSettings *)settings;

/**
 * Initialize a publisher object, and specify the delegate object and the 
 * stream's name.
 *
 * This method is deprecated. Use <[OTPublisherKit initWithDelegate:settings:]>
 * instead.
 *
 * When running in the XCode iOS Simulator, this method returns `nil`.
 *
 * @param delegate The delegate (<OTPublisherKitDelegate>) object for the 
 * publisher.
 *
 * @param name The name of the publisher video. The <[OTStream name]> property
 * for a stream published by this publisher will be set to this value
 * (on all clients).
 *
 * @return The pointer to the instance, or `nil` if initialization failed.
 */
- (nullable instancetype)initWithDelegate:(nullable id<OTPublisherKitDelegate>)delegate
                                    name:(nullable NSString*)name __attribute((deprecated(("Use initWithDelegate: settings: instead"))));

/**
 * Initialize the publisher, and specify whether audio and video will be
 * enabled for this instance.
 *
 * This method is deprecated. Use <[OTPublisherKit initWithDelegate:settings:]>
 * instead.
 *
 * If either audioTrack or videoTrack are set to NO,
 * the respective subsystem will not be initialized, and setting the
 * corresponding <[OTPublisherKit publishAudio]> and
 * <[OTPublisherKit publishVideo]> properties will have no effect.
 *
 * If your application does not require the use of audio or video, it is
 * recommended to set these values rather than use the
 * <[OTPublisherKit publishAudio]> and <[OTPublisherKit publishVideo]>
 * properties, which only temporarily disable the tracks.
 *
 * @param delegate The delegate (<OTPublisherKitDelegate>) object for the
 * publisher.
 *
 * @param name The name of the publisher video. The <[OTStream name]> property
 * for a stream published by this publisher will be set to this value
 * (on all clients).
 *
 * @param audioTrack Whether to publish audio (YES) or not (NO).
 *
 * @param videoTrack Whether to publish video (YES) or not (NO).
 */
- (nullable instancetype)initWithDelegate:(nullable id<OTPublisherKitDelegate>)delegate
                                    name:(nullable NSString*)name
                              audioTrack:(BOOL)audioTrack
                              videoTrack:(BOOL)videoTrack __attribute((deprecated(("Use initWithDelegate: settings: instead"))));

/** @name Getting information about the publisher */

/**
 * The <OTPublisherDelegate> object, which is the delegate for the OTPublisher
 * object.
 */
@property(nonatomic, assign) id<OTPublisherKitDelegate> _Nullable delegate;

/**
 * Periodically receives reports of audio levels for this publisher.
 *
 * This is a separate delegate object from that set as the delegate property
 * (the OTPublisherKitDelegate object).
 *
 * If you do not set this property, the audio sampling subsystem is disabled.
 */
@property (nonatomic, assign)
id<OTPublisherKitAudioLevelDelegate> _Nullable audioLevelDelegate;

/**
 * The <OTPublisherKitNetworkStatsDelegate> object that periodically
 * receives publisher quality statistics.
 *
 * This is a beta feature.
 *
 * This delegate object is sent messages reporting the following:
 *
 * * Total audio and video packets lost
 * * Total audio and video packets received
 * * Total audio and video bytes received
 *
 * This is a separate delegate object from that set as the
 * <[OTPublisherKit delegate]> property (the OTPublisherKitDelegate object).
 */
@property (nonatomic, assign)
id<OTPublisherKitNetworkStatsDelegate> _Nullable networkStatsDelegate;

/**
 * The session that owns this publisher.
 */
@property(readonly) OTSession* _Nullable session;

/**
 * The <OTStream> object associated with the publisher.
 */
@property(readonly) OTStream* _Nullable stream;

/**
 * A string that will be associated with this publisher's stream. This string is
 * displayed at the bottom of subscriber videos associated with the published
 * stream, if an overlay to display the name exists. 
 *
 * The name must be set at initialization, when you when you send the
 * <[OTPublisherKit initWithDelegate:settings:]> message.
 *
 * This value defaults to an empty string.
 */
@property(readonly) NSString* _Nullable name;

/** @name Controlling audio and video output for a publisher */

/**
 * Whether to publish audio.
 *
 * The default value is TRUE.
 */
@property(nonatomic) BOOL publishAudio;

/**
 * Whether to publish video.
 *
 * The default value is TRUE.
 */
@property(nonatomic) BOOL publishVideo;

/** @name Setting publisher device configuration */

/**
 * The <OTVideoCapture> instance used to capture video to stream to the OpenTok
 * session.
 */
@property(nonatomic, strong) id<OTVideoCapture> _Nullable videoCapture;

/**
 * Specifies the type of video for the published stream.
 *
 * Set this to one of the following values:
 *
 * * <code>OTPublisherKitVideoTypeScreen</code> &mdash; Optimizes the video
 *   encoding for screen sharing. To publish a screen-sharing stream, you
 *   need to implement a custom video capturer for the OTPublisherKit object.
 *   See <[OTPublisherKit videoCapture]>. It is recommended to use a low frame
 *   rate (5 frames per second or lower) with this video type. When using the
 *   screen video type in a session that uses the OpenTok Media Server, the
 *   <[OTPublisherKit audioFallbackEnabled]> property is set to NO by default;
 *   this disables the audio-only fallback feature, so that the video does not
 *   drop out in subscribers. See [the OpenTok Media
 *   Router](http://tokbox.com/opentok/tutorials/create-session/#media-mode ).
 *
 * * <code>OTPublisherKitVideoTypeCamera</code> &mdash;
 *   This is the default setting, which you should use when using a camera
 *   as the video source.
 */
@property (nonatomic, assign) OTPublisherKitVideoType videoType;

/**
 * The <OTVideoRender> instance used to render video to stream to the OpenTok
 * session.
 */
@property(nonatomic, strong) id<OTVideoRender> _Nullable videoRender;

/** @name Setting the audio-only fallback mode */

/**
 * Whether the stream will use the audio-fallback feature (<code>YES</code>) or
 * not (<code>NO</code>). The audio-fallback feature is available in sessions
 * that use the the OpenTok Media Router. With the audio-fallback feature
 * enabled, when the OpenTok Media Router determines that a stream's quality has
 * degraded significantly for a specific subscriber, it disables the video in
 * that subscriber in order to preserve call quality.
 *
 * The default setting is <code>YES</code> (the audio-fallback feature is
 * enabled) for publishers using the camera. To turn off the audio-fallback
 * feature, set this property to <code>NO</code> <i>before</i> calling the
 * <[OTSession publish:error:]> method. However, When using the
 * screen video type in a session that uses the OpenTok Media Server, the
 * audioFallbackEnabled property is set to NO by default; this disables the
 * audio-only fallback feature, so that the video does not drop out in
 * subscribers.
 *
 * For more information, see <[OTPublisherKit videoType]>,
 * <[OTSubscriberKitDelegate subscriberVideoDisabled:reason:]>,
 * <OTSubscriberVideoEventReason>, [the OpenTok Media
 * Router](http://tokbox.com/opentok/tutorials/create-session/#media-mode ).
 */
@property (nonatomic, assign) BOOL audioFallbackEnabled;

@end


/**
 * Used for sending messages for an OTPublisher instance. The OTPublisher class
 * includes a `delegate` property. When you send the
 * <[OTPublisherKit initWithDelegate:]> message or the
 * <[OTPublisherKit initWithDelegate:settings:]> message, you specify an
 * OTPublisherKitDelegate object.
 */
@protocol OTPublisherKitDelegate <NSObject>

/**
 * Sent if the publisher encounters an error. After this message is sent,
 * the publisher can be considered fully detached from a session and may
 * be released.
 * @param publisher The publisher that signalled this event.
 * @param error The error (an <OTError> object). The `OTPublisherErrorCode` 
 * enum (defined in the OTError class)
 * defines values for the `code` property of this object.
 */
- (void)publisher:(nonnull OTPublisherKit*)publisher
 didFailWithError:(nonnull OTError*)error;

@optional

/**
 * Sent when the publisher starts streaming.
 *
 * @param publisher The publisher of the stream.
 * @param stream The stream that was created.
 */
- (void)publisher:(nonnull OTPublisherKit*)publisher
    streamCreated:(nonnull OTStream*)stream;

/**
 * Sent when the publisher stops streaming.
 * @param publisher The publisher that stopped sending this stream.
 * @param stream The stream that ended.
 */
- (void)publisher:(nonnull OTPublisherKit*)publisher
  streamDestroyed:(nonnull OTStream*)stream;

@end

/**
 * Used for monitoring the audio levels of the publisher.
 */
@protocol OTPublisherKitAudioLevelDelegate <NSObject>

/**
 * Sent on a regular interval with the recent representative audio level.
 *
 * @param publisher The publisher instance being represented.
 * @param audioLevel A value between 0 and 1, representing the audio level.
 * Adjust this value logarithmically for use in a user interface
 * visualization of the volume (such as a volume meter).
 */
- (void)publisher:(nonnull OTPublisherKit*)publisher
audioLevelUpdated:(float)audioLevel;

@end

/**
 * Used to monitor audio and video statistics for the publisher. See
 * <[OTPublisherKit networkStatsDelegate]>.
 *
 * This is a beta feature.
 */
@protocol OTPublisherKitNetworkStatsDelegate <NSObject>

@optional

/**
 * Sent periodically to report video statistics for the publisher.
 *
 * @param publisher The publisher these statistic apply to.
 *
 * @param stats An array of <OTPublisherKitVideoNetworkStats> objects.
 * For a publisher in a routed session (one that uses the
 * <a href="https://tokbox.com/developer/guides/create-session/#media-mode">OpenTok
 * Media Router</a>), this array includes one object, defining the statistics
 * for the single video media stream that is sent to the OpenTok Media Router.
 * In a relayed session, the array includes an object for each subscriber
 * to the published stream.
 *
 * The <OTPublisherKitVideoNetworkStats> object includes properties.
 * properties for the video bytes received, video packets lost, and video
 * packets sent for the publisher.
 */
- (void)publisher:(nonnull OTPublisherKit*)publisher
videoNetworkStatsUpdated:(nonnull NSArray<OTPublisherKitVideoNetworkStats*>*)stats;

/**
 * Sent periodically to report audio statistics for the publisher.
 *
 * @param publisher The publisher these statistic apply to.
 *
 * @param stats An array of <OTPublisherKitAudioNetworkStats> objects.
 * For a publisher in a routed session (one that uses the
 * <a href="https://tokbox.com/developer/guides/create-session/#media-mode">OpenTok
 * Media Router</a>), this array includes one object, defining the statistics
 * for the single audio media stream that is sent to the OpenTok Media Router.
 * In a relayed session, the array includes an object for each subscriber
 * to the published stream.
 *
 * The <OTPublisherKitAudioNetworkStats> object includes properties.
 * properties for the audio bytes received, audio packets lost, and audio
 * packets sent for the publisher.
 */
- (void)publisher:(nonnull OTPublisherKit*)publisher
audioNetworkStatsUpdated:(nonnull NSArray<OTPublisherKitAudioNetworkStats*>*)stats;

@end
