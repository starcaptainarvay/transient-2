#ifndef VIDEO_PLAYER_H
#define VIDEO_PLAYER_H

#include <vulkan/vulkan.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>

typedef struct {
    AVFormatContext *formatCtx; // FFmpeg format context
    AVCodecContext *codecCtx;   // FFmpeg codec context
    AVFrame *frame;             // FFmpeg frame for decoded video frame
    struct SwsContext *swsCtx;  // FFmpeg software scaler context
    VkImage image;              // Vulkan image to hold video frame data
    VkDeviceMemory imageMemory; // Memory for the Vulkan image
    VkImageView imageView;      // ImageView for the Vulkan image
    int width;                  // Width of the video frame
    int height;                 // Height of the video frame
    // Add any other necessary fields for video playback control
} VideoPlayer;

void decodeVideo(VideoPlayer *player, const char* filename);

#endif