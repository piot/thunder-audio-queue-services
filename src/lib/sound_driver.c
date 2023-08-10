/*---------------------------------------------------------------------------------------------
 *  Copyright (c) Peter Bjorklund. All rights reserved.
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------*/
#include <clog/clog.h>
#include <thunder-audio-queue-services/sound_driver.h>
#include <thunder-audio-buffer/sound_buffer.h>

const static int audio_buffer_size = 2 * 1024;

static void fillBufferCallback(void* _self, AudioQueueRef queue, AudioQueueBufferRef buffer)
{
    ThunderAudioQueueService* self = _self;
    ThunderAudioBuffer* sound_buffer = self->buffer;

    buffer->mAudioDataByteSize = buffer->mAudioDataBytesCapacity;
    if ((buffer->mAudioDataBytesCapacity % 4) != 0) {
        CLOG_WARN("ERROR!!!!!")
    }
    UInt32 sample_count = buffer->mAudioDataByteSize / sizeof(ThunderSampleOutputS16);
    ThunderSampleOutputS16* target = (ThunderSampleOutputS16*)buffer->mAudioData;
    thunderAudioBufferRead(sound_buffer, target, sample_count);

    const UInt32 packetCount = 0;
    AudioStreamPacketDescription* packetDescriptions = 0;
    int enqueueResult = AudioQueueEnqueueBuffer(queue, buffer, packetCount, packetDescriptions);
    (void)enqueueResult;
    CLOG_ASSERT(enqueueResult == 0, "AudioQueueEnqueueBuffer failed")
}

static void createAndFillBuffers(ThunderAudioQueueService* self)
{
    const int buffer_count = 4;
    AudioQueueBufferRef buffers[4];

    for (int i = 0; i < buffer_count; ++i) {
        int err = AudioQueueAllocateBuffer(self->zQueue, audio_buffer_size, &buffers[i]);
        (void)err;
        CLOG_ASSERT(err == 0, "Couldn't allocate buffer")
        fillBufferCallback(self, self->zQueue, buffers[i]);
    }
}

static void startPlayback(ThunderAudioQueueService* self)
{
    OSStatus err = AudioQueueStart(self->zQueue, 0);
    (void)err;
    CLOG_ASSERT(err == 0, "Audio queue start failed")
}

static void openOutput(ThunderAudioQueueService* self, float sampleRate)
{
    AudioStreamBasicDescription zFormat;

    tc_mem_clear_type(&zFormat);

    zFormat.mSampleRate = (Float64)sampleRate;
    zFormat.mFormatID = kAudioFormatLinearPCM;
    zFormat.mFormatFlags = kLinearPCMFormatFlagIsSignedInteger | kAudioFormatFlagIsPacked;
    zFormat.mChannelsPerFrame = 2;
    zFormat.mFramesPerPacket = 1;
    zFormat.mBytesPerFrame = zFormat.mChannelsPerFrame * sizeof(ThunderSampleOutputS16);
    zFormat.mBitsPerChannel = sizeof(ThunderSampleOutputS16) * 8;
    zFormat.mBytesPerPacket = zFormat.mBytesPerFrame * zFormat.mFramesPerPacket;

    OSStatus err = AudioQueueNewOutput(
        &zFormat, fillBufferCallback, self, 0, kCFRunLoopCommonModes, 0, &self->zQueue);

    (void)err;
    CLOG_ASSERT(err == 0, "AudioQueueNewOutput err:%d", (unsigned int)err)
}

int thunderAudioQueueServiceInit(ThunderAudioQueueService* self, ThunderAudioBuffer* buffer)
{
    self->buffer = buffer;
    openOutput(self, 48000.0f);
    createAndFillBuffers(self);
    startPlayback(self);

    return 0;
}

void thunderAudioQueueServiceFree(ThunderAudioQueueService* self)
{
    (void)self;
}
