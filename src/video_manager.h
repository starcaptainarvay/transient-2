#ifndef VIDEO_MANAGER_H
#define VIDEO_MANAGER_H

#define MAX_VIDEO_PLAYERS 5

#include "video_player.h"

#include <vulkan/vulkan.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>

typedef struct {
    VideoPlayer player;
    int isPlaying; // Flag indicating if the player is currently playing
} VideoPlayerInstance;

// Define a structure to hold instances of video players
typedef struct {
    VideoPlayerInstance instances[MAX_VIDEO_PLAYERS];
} VideoPlayerManager;

// Function prototypes related to video playback
void initVideoManager();
void initVideoPlayers(VideoPlayerManager *manager);

// Load a video into a video player
void loadVideo_private(VideoPlayerManager *manager, const char* filename);
void loadVideo(const char* filename);

// Render frame from all video players
void renderFrame_private(VideoPlayerManager *manager, VkCommandBuffer commandBuffer, VkPipelineLayout pipelineLayout, VkPipeline graphicsPipeline, VkExtent2D swapchainExtent);
void renderFrame(VkCommandBuffer commandBuffer, VkPipelineLayout pipelineLayout, VkPipeline graphicsPipeline, VkExtent2D swapchainExtent);

// Example of how to mark a player as playing
void startPlayback(VideoPlayerManager *manager, int playerIndex);

// Example of how to mark a player as not playing
void stopPlayback(VideoPlayerManager *manager, int playerIndex);


#endif