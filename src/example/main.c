/*---------------------------------------------------------------------------------------------
 *  Copyright (c) Peter Bjorklund. All rights reserved.
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------*/
#include <clog/clog.h>
#include <clog/console.h>
#include <imprint/default_setup.h>
#include <thunder-audio-queue-services/sound_driver.h>
#include <thunder/sound_buffer.h>
#include <thunder/sound_module.h>

clog_config g_clog;
char g_clog_temp_str[CLOG_TEMP_STR_SIZE];

int main(int argc, char* argv[])
{
    g_clog.level = CLOG_TYPE_VERBOSE;
    g_clog.log = clog_console;

    CLOG_INFO("startup example")

    ThunderAudioQueueService audioQueue;

    ImprintDefaultSetup memory;

    int err = imprintDefaultSetupInit(&memory, 2 * 1024);

    ThunderSoundModule module;
    thunderSoundModuleInit(&module, &memory.tagAllocator.info);

    thunderSoundModuleDebugSineWave(&module, &memory.tagAllocator.info);

    thunderAudioQueueServiceInit(&audioQueue, &module.compositor.buffer);

    for (int i = 0; i < 3 * 62; ++i) {
        thunderSoundModuleUpdate(&module);
        usleep(16 * 1000);
    }

    thunderAudioQueueServiceFree(&audioQueue);
}