#include "video_player.h"
#include "video_manager.h"
#include "vulkan_utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // For sleep() function

#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <libavutil/imgutils.h>
#include <vulkan/vulkan.h>

static VideoPlayerManager videoManager;

// Initialize the video manager
void initVideoManager() {
    // Initialize video player instances
    initVideoPlayers(&videoManager);
}

void initVideoPlayers(VideoPlayerManager *manager) {
    // Initialize each video player instance
    for (int i = 0; i < MAX_VIDEO_PLAYERS; ++i) {
        // Initialize video player instance properties here
        // For example, set isPlaying flag to false
        manager->instances[i].isPlaying = 0;
    }
}

void loadVideo_private(VideoPlayerManager *manager, const char* filename) {
    // Find an available video player instance to load the video
    // For simplicity, let's assume we always load into the first available player
    for (int i = 0; i < MAX_VIDEO_PLAYERS; ++i) {
        if (!manager->instances[i].isPlaying) {
            // Loading logic here
            decodeVideo(&manager->instances[i].player, filename);
            // You may need to open the video file with FFmpeg, decode frames, and transfer to Vulkan image
            // Once loaded, set isPlaying flag to true
            manager->instances[i].isPlaying = 1;
            break; // Exit loop after loading the video into the first available player
        }
    }
}

void renderFrame_private(VideoPlayerManager *manager, VkCommandBuffer commandBuffer, VkPipelineLayout pipelineLayout, VkPipeline graphicsPipeline, VkExtent2D swapchainExtent) {
    // Render frames from all video players
    for (int i = 0; i < MAX_VIDEO_PLAYERS; ++i) {
        if (manager->instances[i].isPlaying) {
            // Render frame from the video player instance
            // Implement rendering logic here
        }
    }
}

// Example of how to mark a player as playing
void startPlayback(VideoPlayerManager *manager, int playerIndex) {
    if (playerIndex >= 0 && playerIndex < MAX_VIDEO_PLAYERS) {
        manager->instances[playerIndex].isPlaying = 1;
    }
}

// Example of how to mark a player as not playing
void stopPlayback(VideoPlayerManager *manager, int playerIndex) {
    if (playerIndex >= 0 && playerIndex < MAX_VIDEO_PLAYERS) {
        manager->instances[playerIndex].isPlaying = 0;
    }
}

void loadVideo(const char* filename) {
    return loadVideo_private(&videoManager, filename);
}

void renderFrame(VkCommandBuffer commandBuffer, VkPipelineLayout pipelineLayout, VkPipeline graphicsPipeline, VkExtent2D swapchainExtent) {
    renderFrame_private(&videoManager, commandBuffer, pipelineLayout, graphicsPipeline, swapchainExtent);
}