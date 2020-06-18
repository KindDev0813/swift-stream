//
//  OTVideoKit.h
//
//  Copyright (c) 2014 TokBox, Inc. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <CoreMedia/CMTime.h>
#import <CoreVideo/CoreVideo.h>
#import <CoreGraphics/CoreGraphics.h>

@class OTError;

/**
 * Defines values for video orientations (up, down, left, right) for the
 * orieintation property of an <OTVideoFrame> object.
 */
typedef NS_ENUM(int32_t, OTVideoOrientation) {
    /** The video is oriented top up. No rotation is applies. */
    OTVideoOrientationUp = 1,
    /** The video is rotated 180 degrees. */
    OTVideoOrientationDown = 2,
   /** The video is rotated 90 degrees. */
    OTVideoOrientationLeft = 3,
   /** The video is rotated 270 degrees. */
    OTVideoOrientationRight = 4,
};

/**
 * Defines values for pixel format for the pixelFormat property of an
 * <OTVideoFrame> object.
 */
typedef NS_ENUM(int32_t, OTPixelFormat) {
    /** I420 format. */
    OTPixelFormatI420 = 'I420',
    /** ARGB format. */
    OTPixelFormatARGB = 'ARGB',
    /** NV12 format. */
    OTPixelFormatNV12 = 'NV12',
};

/**
 * Defines values for the <[OTSubscriber viewScaleBehavior]> and
 * <[OTPublisher viewScaleBehavior]> methods.
 */
typedef NS_ENUM(NSInteger, OTVideoViewScaleBehavior) {
    /**
     * The video shrinks, as needed, so that the entire video is visible
     * with pillarboxing.
     */
    OTVideoViewScaleBehaviorFit,
    /**
     * The video scales to fill the entire area of the renderer, with cropping
     * as needed.
     */
    OTVideoViewScaleBehaviorFill,
};

/**
 * Defines the video format assigned to an instance of an <OTVideoFrame> object.
 */
@interface OTVideoFormat : NSObject

/**
 * The name you assign to the video format
 */
@property(nonatomic, copy) NSString* _Nonnull name;
/**
 * The pixel format. Valid values are defined in the <OTPixelFormat> enum.
 */
@property(nonatomic, assign) OTPixelFormat pixelFormat;
/**
 * The number of bytes per row of the video.
 */
@property(nonatomic, strong) NSMutableArray* _Nonnull bytesPerRow;
/**
 * The width of the video, in pixels.
 */
@property(nonatomic, assign) uint32_t imageWidth;
/**
 * The height of the video, in pixels.
 */
@property(nonatomic, assign) uint32_t imageHeight;
/**
 * The estimated number of frames per second in the video.
 */
@property(nonatomic, assign) double estimatedFramesPerSecond;
/**
 * The estimated capture delay, in milliseconds, of the video.
 */
@property(nonatomic, assign) double estimatedCaptureDelay;

+ (nonnull OTVideoFormat*)videoFormatI420WithWidth:(uint32_t)width
                                            height:(uint32_t)height;

+ (nonnull OTVideoFormat*)videoFormatNV12WithWidth:(uint32_t)width
                                            height:(uint32_t)height;

+ (nonnull OTVideoFormat*)videoFormatARGBWithWidth:(uint32_t)width
                                            height:(uint32_t)height;

@end

/**
 * Defines a frame of a video. See <[OTVideoRender renderVideoFrame:]> and
 * <[OTVideoCaptureConsumer consumeFrame:]>.
 */
@interface OTVideoFrame : NSObject

/** @name Properties of OTVideoFrame objects */

/**
 * An array of planes in the video frame.
 */
@property(nonatomic, strong) NSPointerArray* _Nullable planes;
/**
 * A timestap of the video frame.
 */
@property(nonatomic, assign) CMTime timestamp;
/**
 * The orientation of the video frame.
 */
@property(nonatomic, assign) OTVideoOrientation orientation;
/**
 * The format of the video frame.
 */
@property(nonatomic, strong) OTVideoFormat* _Nullable format;
/**
 * The metadata associated with this video frame, if any.
 */
@property(nonatomic, readonly) NSData* _Nullable metadata;

/** @name Instantiating OTVideoFrame objects */

/**
 * Initializes an OTVideoFrame object.
 */
- (nonnull id)init;

/**
 * Initializes an OTVideoFrame object with a specified format.
 *
 * @param videoFormat The video format used by the video frame.
 */
- (nonnull id)initWithFormat:(nonnull OTVideoFormat*)videoFormat;
/**
 * Sets planes for the video frame.
 *
 * @param planes The planes to assign.
 * @param numPlanes The number of planes to assign.
 */
- (void)setPlanesWithPointers:(uint8_t* _Nonnull[_Nonnull])planes numPlanes:(int)numPlanes;
/**
 * Cleans the planes in the video frame.
 */
- (void)clearPlanes;

/**
 * Sets the metadata associated with this video frame.
 *
 * @param data The metadata to assign.
 * @param error If the size of the metadata passed is bigger than 32 bytes
 * this value is set to an OTError object with the `code`  property set to
 * OTNullOrInvalidParameter.
 */
- (void)setMetadata:(nonnull NSData *)data error:(out OTError* _Nullable*)error;

@end

/**
 * Defines a the consumer of an OTVideoCapture object.
 */
@protocol OTVideoCaptureConsumer <NSObject>

/**
 * Consumes a frame.
 *
 * @param frame The frame to consume.
 */
- (void)consumeFrame:(nonnull OTVideoFrame*)frame;

/**
 * Consumes a CoreVideo image buffer.
 *
 * This is a beta feature.
 *
 * @param frame The CVImageBufferRef to consume. The frame's pixel type must be one of the following
 * 			kCVPixelFormatType_420YpCbCr8BiPlanarVideoRange, kCVPixelFormatType_420YpCbCr8BiPlanarFullRange,
 * 			kCVPixelFormatType_420YpCbCr8Planar, kCVPixelFormatType_420YpCbCr8PlanarFullRange,
 * 			kCVPixelFormatType_32ARGB, kCVPixelFormatType_32BGRA, or kCVPixelFormatType_24RGB.
 * @param orientation The orientation of the frame.
 * @param ts The frame timestamp.
 * @param metadata The frame metadata.
 *
 * @return YES if the image buffer consumed successfully, or NO upon failure.
 */
- (BOOL)consumeImageBuffer:(nonnull CVImageBufferRef)frame orientation:(OTVideoOrientation)orientation
                 timestamp:(CMTime)ts metadata:(NSData* _Nullable)metadata;

@end

/**
 * Defines a video capturer to be used by an <OTPublisherKit> object.
 * See the `videoCapture` property of an <OTPublisherKit> object.
 */
@protocol OTVideoCapture <NSObject>

/**
 * The <OTVideoCaptureConsumer> object that consumes frames for the video
 * capturer.
 */
@property(atomic, weak) id<OTVideoCaptureConsumer> _Nullable videoCaptureConsumer;

/**
 * Initializes the video capturer.
 */
- (void)initCapture NS_SWIFT_NAME(initCapture());
/**
 * Releases the video capturer.
 */
- (void)releaseCapture NS_SWIFT_NAME(releaseCapture());
/**
 * Starts capturing video.
 */
- (int32_t)startCapture;
/**
 * Stops capturing video.
 */
- (int32_t)stopCapture;
/**
 * Whether video is being captured.
 */
- (BOOL)isCaptureStarted;
/**
 * The video format of the video capturer.
 * @param videoFormat The video format used.
 */
- (int32_t)captureSettings:(nonnull OTVideoFormat*)videoFormat;

@end

/**
 * Defines a video renderer to be used by an <OTPublisherKit> object or an
 * <OTSubscriberKit> object. See the `videoRender` properties of
 * <OTPublisherKit> object and <OTSubscriberKit>.
 */
@protocol OTVideoRender <NSObject>

/**
 * Renders a frame to the video renderer.
 *
 * @param frame The frame to render.
 */
- (void)renderVideoFrame:(nonnull OTVideoFrame*) frame;

@end
