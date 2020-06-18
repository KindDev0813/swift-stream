//
//  OTNetworkStatsKit.h
//  otkit-objc-libs
//
//  Created by Charley Robinson on 12/1/14.
//
//

#import <Foundation/Foundation.h>

/**
 * Describes video statistics for a subscriber. See
 * <[OTSubscriberKit setNetworkStatsDelegate:]>.
 */
@interface OTSubscriberKitVideoNetworkStats : NSObject

/**
 * The estimated number of video packets lost by this subscriber.
 */
@property (readonly) uint64_t videoPacketsLost;

/**
 * The number of video packets received by this subscriber.
 */
@property (readonly) uint64_t videoPacketsReceived;

/**
 * The number of video bytes received by this subscriber.
 */
@property (readonly) uint64_t videoBytesReceived;

/**
 * The timestamp, in milliseconds since the Unix epoch, for when these stats
 * were gathered.
 */
@property (readonly) double timestamp;

- (nonnull instancetype)initWithPacketsLost:(uint64_t)packetsLost
                            packetsReceived:(uint64_t)packetsReceived
                              bytesReceived:(uint64_t)bytesReceived
                                  timestamp:(double)timestamp;

@end


/**
 * Describes audio statistics for a subscriber. See
 * <[OTSubscriberKit setNetworkStatsDelegate:]>.
 */
@interface OTSubscriberKitAudioNetworkStats : NSObject

/**
 * The estimated number of audio packets lost by this subscriber.
 */
@property (readonly) uint64_t audioPacketsLost;

/**
 * The number of audio packets received by this subscriber.
 */
@property (readonly) uint64_t audioPacketsReceived;

/**
 * The number of audio bytes received by this subscriber.
 */
@property (readonly) uint64_t audioBytesReceived;

/**
 * The timestamp, in milliseconds since the Unix epoch, for when these stats
 * were gathered.
 */
@property (readonly) double timestamp;


- (nonnull instancetype)initWithPacketsLost:(uint64_t)packetsLost
                            packetsReceived:(uint64_t)packetsReceived
                              bytesReceived:(uint64_t)bytesReceived
                                  timestamp:(double)timestamp;

@end


/**
 * Describes video statistics for a publisher. See
 * <[OTPublisherKit setNetworkStatsDelegate:]>.
 */
@interface OTPublisherKitVideoNetworkStats : NSObject

/**
 * The connection ID of the client subscribing to the stream.
 */
@property (nonnull, readonly) NSString* connectionId;

/**
 * The subscriber ID of the client subscribing to the stream
 * (in a relayed session). In a routed session, this is set to
 * an empty string.
 */
@property (nonnull, readonly) NSString* subscriberId;

/**
 * The estimated number of video packets that did not reach the subscriber
 * (or the OpenTok Media Router).
 */
@property (readonly) int64_t videoPacketsLost;

/**
 * The number of video packets packets sent to the subscriber
 * (or to the OpenTok Media Router).
 */
@property (readonly) int64_t videoPacketsSent;

/**
 * The number of video bytes sent to the subscriber (or to
 * the OpenTok Media Router).
 */
@property (readonly) int64_t videoBytesSent;

/**
 * The timestamp, in milliseconds since the Unix epoch, for when these stats
 * were gathered.
 */
@property (readonly) double timestamp;

/**
 * The timestamp, in milliseconds since the Unix epoch, from which 
 * the cumulative totals started accumulating.
 */
@property (readonly) double startTime;

- (nonnull instancetype)initWithConnectionId:(NSString* _Nonnull)connectionId
                                 subscriberId:(NSString* _Nonnull)subscriberId
                           packetsLost:(int64_t)packetsLost
                           packetsSent:(int64_t)packetsSent
                             bytesSent:(int64_t)bytesSent
                             timestamp:(double)timestamp
                             startTime:(double)startTime;

@end


/**
 * Describes audio statistics for a publisher. See
 * <[OTPublisherKit setNetworkStatsDelegate:]>.
 */
@interface OTPublisherKitAudioNetworkStats : NSObject

/**
 * The connection ID of the subscribing client.
 */
@property (nonnull, readonly) NSString* connectionId;

/**
 * The subscriber ID of the client subscribing to the stream
 * (in a relayed session). In a routed session, this is set to
 * an empty string.
 */
@property (nonnull, readonly) NSString* subscriberId;

/**
 * The estimated number of audio packets that did not reach the subscriber
 * (or the OpenTok Media Router).
 */
@property (readonly) int64_t audioPacketsLost;

/**
 * The number of audio packets sent to the subscriber (or to
 * the OpenTok Media Router).
 */
@property (readonly) int64_t audioPacketsSent;

/**
 * The number of audio bytes sent to the subscriber (or to
 * the OpenTok Media Router)
 */
@property (readonly) int64_t audioBytesSent;

/**
 * The timestamp, in milliseconds since the Unix epoch, for when these stats
 * were gathered.
 */
@property (readonly) double timestamp;

/**
 * The timestamp, in milliseconds since the Unix epoch, from which 
 * the cumulative totals started accumulating.
 */
@property (readonly) double startTime;

- (nonnull instancetype)initWithConnectionId:(NSString* _Nonnull)connectionId
                                 subscriberId:(NSString* _Nonnull)subscriberId
                           packetsLost:(int64_t)packetsLost
                           packetsSent:(int64_t)packetsSent
                             bytesSent:(int64_t)bytesSent
                             timestamp:(double)timestamp
                             startTime:(double)startTime;

@end
