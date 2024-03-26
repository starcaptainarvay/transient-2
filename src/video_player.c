#include "video_player.h"

#include <vulkan/vulkan.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>

void ffmpegError(int ret) {
    if (ret < 0) {
        char errorBuf[AV_ERROR_MAX_STRING_SIZE] = {0};
        av_strerror(ret, errorBuf, AV_ERROR_MAX_STRING_SIZE);
        fprintf(stderr, "FFmpeg error: %s\n", errorBuf);
    }
}

// Decode video frames using FFmpeg and load into VideoPlayer struct
void decodeVideo(VideoPlayer *player, const char* filename) {
    // Open the video file
    int ret = avformat_open_input(&player->formatCtx, filename, NULL, NULL);
    if (ret < 0) {
        ffmpegError(ret);
        return;
    }

    // Retrieve stream information
    ret = avformat_find_stream_info(player->formatCtx, NULL);
    if (ret < 0) {
        ffmpegError(ret);
        return;
    }

    // Find the first video stream
    int videoStreamIndex = -1;
    for (int i = 0; i < player->formatCtx->nb_streams; i++) {
        if (player->formatCtx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
            videoStreamIndex = i;
            break;
        }
    }
    if (videoStreamIndex == -1) {
        fprintf(stderr, "No video stream found in the input file\n");
        return;
    }

    // Get a pointer to the codec context for the video stream
    AVCodecParameters *codecParams = player->formatCtx->streams[videoStreamIndex]->codecpar;
    AVCodec *codec = avcodec_find_decoder(codecParams->codec_id);
    if (!codec) {
        fprintf(stderr, "Failed to find decoder for the video stream\n");
        return;
    }
    player->codecCtx = avcodec_alloc_context3(codec);
    if (!player->codecCtx) {
        fprintf(stderr, "Failed to allocate codec context\n");
        return;
    }
    ret = avcodec_parameters_to_context(player->codecCtx, codecParams);
    if (ret < 0) {
        ffmpegError(ret);
        return;
    }
    ret = avcodec_open2(player->codecCtx, codec, NULL);
    if (ret < 0) {
        ffmpegError(ret);
        return;
    }

    // Allocate video frame
    player->frame = av_frame_alloc();
    if (!player->frame) {
        fprintf(stderr, "Failed to allocate video frame\n");
        return;
    }

    // Read frames from the video stream and decode
    AVPacket packet;
    while (av_read_frame(player->formatCtx, &packet) >= 0) {
        if (packet.stream_index == videoStreamIndex) {
            ret = avcodec_send_packet(player->codecCtx, &packet);
            if (ret < 0) {
                ffmpegError(ret);
                break;
            }
            while (ret >= 0) {
                ret = avcodec_receive_frame(player->codecCtx, player->frame);
                if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF) {
                    break;
                } else if (ret < 0) {
                    ffmpegError(ret);
                    break;
                }

                // Process the decoded frame (e.g., transfer to Vulkan image)
                // You'll need to implement this part based on your requirements

            }
        }
        av_packet_unref(&packet);
    }

    // Free resources
    avcodec_free_context(&player->codecCtx);
    av_frame_free(&player->frame);
    avformat_close_input(&player->formatCtx);
}
