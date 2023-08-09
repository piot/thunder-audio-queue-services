/*---------------------------------------------------------------------------------------------
 *  Copyright (c) Peter Bjorklund. All rights reserved.
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------*/
#ifndef THUNDER_AUDIO_QUEUE_SERVICE_H
#define THUNDER_AUDIO_QUEUE_SERVICE_H

#include <AudioToolbox/AudioToolbox.h>

struct ThunderAudioBuffer;

typedef struct ThunderAudioQueueService {
    AudioQueueRef zQueue;
    struct ThunderAudioBuffer* buffer;
} ThunderAudioQueueService;

int thunderAudioQueueServiceInit(ThunderAudioQueueService* self, struct ThunderAudioBuffer* buffer);
void thunderAudioQueueServiceFree(ThunderAudioQueueService* self);

#endif
