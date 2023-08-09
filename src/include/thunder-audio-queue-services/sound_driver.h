#ifndef THUNDER_AUDIO_QUEUE_SERVICE_H
#define THUNDER_AUDIO_QUEUE_SERVICE_H

#include <AudioToolbox/AudioToolbox.h>

struct thunder_audio_buffer;

typedef struct ThunderAudioQueueService {
    AudioQueueRef zQueue;
    struct thunder_audio_buffer* buffer;
} ThunderAudioQueueService;

int thunderAudioQueueServiceInit(
    ThunderAudioQueueService* self, struct thunder_audio_buffer* buffer);
void thunderAudioQueueServiceFree(ThunderAudioQueueService* self);

#endif
