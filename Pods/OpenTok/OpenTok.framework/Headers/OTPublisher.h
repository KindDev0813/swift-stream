//
//  OTPublisher.h
//  OpenTok iOS SDK
//
//  Copyright (c) 2014 Tokbox, Inc. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#import <AVFoundation/AVFoundation.h>

@class OTError, OTPublisherKit;

@protocol OTPublisherDelegate;
@protocol OTPublisherKitDelegate;

/**
 * Defines values for the <code>cameraResolution</code> parameter of the
 * <[OTPublisher initWithDelegate:name:cameraResolution:cameraFrameRate:]> method.
 */
typedef NS_ENUM(NSInteger, OTCameraCaptureResolution) {
    /**
     * The lowest available camera capture resolution supported in the OpenTok iOS SDK (352x288)
     * or the closest resolution supported on the device.
     */
    OTCameraCaptureResolutionLow,
    /**
     * VGA resolution (640x480) or the closest resolution supported on the device.
     */
    OTCameraCaptureResolutionMedium, // AVCaptureSessionPreset640x480
    /**
		 * The highest available camera capture resolution supported in the OpenTok iOS SDK
		 * (1280x720) or the closest resolution supported on the device.
		 */
    OTCameraCaptureResolutionHigh, // AVCaptureSessionPreset1280x720
};

/**
 * Defines values for the <code>cameraFrameRate</code> parameter of the
 * <[OTPublisher initWithDelegate:name:cameraResolution:cameraFrameRate:]>
 * method.
 *
 * Note that in sessions that use the OpenTok Media Router (sessions with the
 * [media mode](http://tokbox.com/opentok/tutorials/create-session/#media-mode)
 * set to routed), lowering the frame rate proportionally reduces the bandwidth
 * the stream uses. However, in sessions that have the media mode set to
 * relayed, lowering the frame rate does not reduce the stream's bandwidth.
 */
typedef NS_ENUM(NSInteger, OTCameraCaptureFrameRate) {
    /**
     * 30 frames per second.
     */
    OTCameraCaptureFrameRate30FPS = 30,
    /**
     * 15 frames per second.
     */
    OTCameraCaptureFrameRate15FPS = 15,
    /**
     * 7 frames per second.
     */
    OTCameraCaptureFrameRate7FPS = 7,
    /**
     * 1 frame per second.
     */
    OTCameraCaptureFrameRate1FPS = 1
};

/**
 * Defines settings to be used when initializing a publisher using the
 * <[OTPublisher initWithDelegate:settings:]> method.
 *
 * This includes settings for the resolution and the frame rate of the
 * publisher's video, in addition to settings inherited from the
 * <OTPublisherKitSettings> class.
 *
 * For sessions that use [the OpenTok Media
 * Router](https://tokbox.com/developer/guides/create-session/#media-mode )
 * (sessions with the media mode set to routed), lowering the frame
 * rate or lowering the resolution reduces the maximum bandwidth the stream can
 * use. However, in sessions with the media mode set to relayed, lowering the
 * frame rate or resolution may not reduce the stream's bandwidth.
 */
@interface OTPublisherSettings : OTPublisherKitSettings

/** @name Defining publisher settings */

/**
 * The <OTVideoCapture> instance used to capture video to stream to the OpenTok
 * session. If this value is nil, the publisher uses the built-in video capturer (which
 * uses the built-in camera as a video source).
 *
 * If an existing publisher is using the built-in camera as the video capturer,
 * set this property to an <OTVideoCapture> object that uses another video source.
 */
@property(nonatomic, strong) id<OTVideoCapture> _Nullable videoCapture;

/**
 * The resolution of the published video. Set this to a value defined in the
 * <OTCameraCaptureResolution> enum. The default resolution is
 * `OTCameraCaptureResolutionMedium`.
 */
@property(nonatomic) OTCameraCaptureResolution cameraResolution;

/**
 * The frame rate of the published video. Set this to a value defined in the
 * <OTCameraCaptureFrameRate> enum. If the device does not support the specified
 * frame rate, it will set the frame rate to 30 frames per second.
 *
 * Note that in sessions that use the OpenTok Media Router (sessions with the
 * [media mode](https://tokbox.com/developer/guides/create-session/#media-mode)
 * set to routed), lowering the frame rate proportionally reduces the bandwidth
 * the stream uses. However, in sessions that have the media mode set to
 * relayed, lowering the frame rate does not reduce the stream's bandwidth.
 *
 * The default frame rate is `OTCameraCaptureFrameRate20FPS`.
 */
@property(nonatomic) OTCameraCaptureFrameRate cameraFrameRate;

@end

/**
 * A publisher captures an audio-video stream from the device's microphone and 
 * camera. You can then publish the audio-video stream to an OpenTok session by
 * sending the <[OTSession publish:error:]> message.
 *
 * The OpenTok iOS SDK supports publishing on all multi-core iOS devices.
 * See "Developer and client requirements" in the README file for the
 * [OpenTok iOS SDK](http://tokbox.com/opentok/libraries/client/ios ).
 *
 * Note that when testing in the iOS Simulator, an OTPublisher object uses
 * a demo video, since the camera is not available in the Simulator.
 */
@interface OTPublisher : OTPublisherKit

/** @name Initializing a publisher */

/**
 * Initialize the publisher with settings defined by an
 * <OTPublisherSettings> object.
 *
 * @param delegate The delegate (<OTPublisherKitDelegate>) object for the
 * publisher.
 *
 * @param settings The (<OTPublisherSettings>) object that defines settings for
 * the publisher.
 */
-(nullable instancetype)initWithDelegate:(nullable id<OTPublisherKitDelegate>)delegate
                                settings:(nonnull OTPublisherSettings *)settings;

/**
 * Initialize a Publisher object.
 *
 * This method is deprecated. Use <[OTPublisher initWithDelegate:settings:]>
 * instead.
 *
 * This method includes parameters for setting the resolution and the frame rate of the
 * publisher's video. If you use an initializer method inherited from <OTPublisherKit>, the
 * resolution will be VGA, and the frame rate will be 30 frames per second.
 *
 * For sessions that use [the OpenTok Media
 * Router](http://tokbox.com/opentok/tutorials/create-session/#media-mode )
 * (sessions with the media mode set to routed), lowering the frame
 * rate or lowering the resolution reduces the maximum bandwidth the stream can
 * use. However, in sessions with the media mode set to relayed, lowering the
 * frame rate or resolution may not reduce the stream's bandwidth.
 *
 * @param delegate The delegate (<OTPublisherKitDelegate>) object for the
 * publisher.
 *
 * @param name The name to appear at the bottom of the video.
 *
 * @param cameraResolution The resolution of the published video. Set this to a value defined in the
 * <OTCameraCaptureResolution> enum.
 *
 * @param cameraFrameRate The frame rate of the published video. Set this to a value defined in the
 * <OTCameraCaptureFrameRate> enum. If the device does not support the specified
 * frame rate, it will set the frame rate to 30 frames per second.
 * Note that in sessions that use the OpenTok Media Router (sessions with the
 * [media mode](http://tokbox.com/opentok/tutorials/create-session/#media-mode)
 * set to routed), lowering the frame rate proportionally reduces the bandwidth
 * the stream uses. However, in sessions that have the media mode set to
 * relayed, lowering the frame rate does not reduce the stream's bandwidth.
 *
 * @return The pointer to the instance, or `nil` if initialization failed.
 */
-(nullable instancetype)initWithDelegate:(nullable id<OTPublisherKitDelegate>)delegate
                                    name:(nullable NSString*)name
                        cameraResolution:(OTCameraCaptureResolution)cameraResolution
                         cameraFrameRate:(OTCameraCaptureFrameRate)cameraFrameRate __attribute((deprecated(("Use initWithDelegate: settings: instead"))));


/** @name Displaying the video */

/**
 * The view for this publisher. If this view becomes visible, it will
 * display a preview of the active camera feed.
 */
@property(readonly) UIView* _Nullable view;

/**
* The scaling of the rendered video, as defined by the
* <OTVideoViewScaleBehavior> enum. The default value is
* OTVideoViewScaleBehaviorFill. Set it to OTVideoViewScaleBehaviorFit to
* have the video shrink, as needed, so that the entire video is visible
* (with pillarboxing). This property only affects the local publisher's
* video. Subscribers to the stream must set the
* <[OTSubscriber viewScaleBehavior]> property.
*/
@property(nonatomic) OTVideoViewScaleBehavior viewScaleBehavior;

/** @name Setting publisher device configuration */

/**
 * The preferred camera position. When setting this property, if the change is 
 * possible, the publisher will use the camera with the specified position. If
 * the publisher has begun publishing, getting this property returns the current
 * camera position; if the publisher has not yet begun publishing,
 * getting this property returns the preferred camera position.
 */
@property(nonatomic) AVCaptureDevicePosition cameraPosition;

@end

/**
 * Used for sending messages for an OTPublisher instance.
 */
@protocol OTPublisherDelegate <OTPublisherKitDelegate>

@optional
/**
 * Sent when the camera device is changed.
 *
 * @param publisher The publisher that signalled this event.
 * @param position The position of the new camera.
 */
-(void)publisher:(nonnull OTPublisher*)publisher
didChangeCameraPosition:(AVCaptureDevicePosition)position;


@end
