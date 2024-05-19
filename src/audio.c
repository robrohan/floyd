#include <stdio.h>
#include <math.h>
#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"

// #include "keyboard.h"
// #include "termcui.h"
// #include "tui.h"
#include "audio.h"

ma_engine g_engine;
ma_sound g_sound;
ma_device g_device;

int g_current_frame = 0;

#define ERROR 1
#define NO_ERROR 0

int load_sound(const char *file_path)
{
    if (ma_sound_init_from_file(&g_engine, file_path, 0, NULL, NULL, &g_sound) != MA_SUCCESS)
    {
        printf("Failed to start playback engine.\n");
        return ERROR;
    }
}

int play_sound()
{
    if (ma_sound_start(&g_sound) != MA_SUCCESS)
    {
        return ERROR;
    }
    return NO_ERROR;
}

/////////////////////////////////////////////////////

int milliseconds_to_frames(float mseconds)
{
    int seekPointInFrames = (mseconds * ma_engine_get_sample_rate(&g_engine)) / 1000;
    return seekPointInFrames;
}

int sound_frame_length(ma_sound sound)
{
    ma_uint64 length;
    ma_result result = ma_sound_get_length_in_pcm_frames(&sound, &length);
    if (result != MA_SUCCESS)
    {
        return 0;
    }
    return (int)length;
}

/////////////////////////////////////////////////////

void data_callback(ma_device *pDevice, void *pOutput, const void *pInput, ma_uint32 frameCount)
{
    (void)pInput;
    if (!ma_sound_at_end(&g_sound) && ma_sound_is_playing(&g_sound))
    {
        g_current_frame += frameCount;
    }
    ma_engine_read_pcm_frames((ma_engine *)pDevice->pUserData, pOutput, frameCount, NULL);
    // printf("%i\n\r", g_current_frame);
}

void stop_engine()
{
    ma_engine_uninit(&g_engine);
    ma_sound_uninit(&g_sound);
    ma_device_uninit(&g_device);
}

int start_engine(const char *file_path, engine_loop callback)
{
    ma_result result;
    ma_context context;
    ma_resource_manager resource_manager;

    // Context
    result = ma_context_init(NULL, 0, NULL, &context);
    if (result != MA_SUCCESS)
    {
        printf("Failed to initialize context.");
        goto cleanup;
    }

    // Resource manager
    ma_resource_manager_config resource_manager_config = ma_resource_manager_config_init();
    resource_manager_config.decodedFormat = ma_format_f32;
    resource_manager_config.decodedChannels = 0;
    resource_manager_config.decodedSampleRate = 44100;

    result = ma_resource_manager_init(&resource_manager_config, &resource_manager);
    if (result != MA_SUCCESS)
    {
        printf("Failed to initialize resource manager.");
        goto cleanup;
    }

    // Device Config
    ma_device_config device_config = ma_device_config_init(ma_device_type_playback);
    device_config.playback.format = resource_manager.config.decodedFormat;
    device_config.playback.channels = 0;
    device_config.sampleRate = resource_manager.config.decodedSampleRate;
    device_config.dataCallback = data_callback;
    device_config.pUserData = &g_engine;

    // Device
    result = ma_device_init(&context, &device_config, &g_device);
    if (result != MA_SUCCESS)
    {
        printf("Failed to initialize device\n");
        goto cleanup;
    }

    // Engine
    ma_engine_config engine_config = ma_engine_config_init();
    engine_config.pDevice = &g_device;
    engine_config.pResourceManager = &resource_manager;
    // engineConfig.noAutoStart = MA_TRUE;

    result = ma_engine_init(&engine_config, &g_engine);
    if (result != MA_SUCCESS)
    {
        return ERROR;
    }

    if (file_path != NULL)
    {
        int err = load_sound(file_path);
        if (err)
        {
            printf("Failed to start playback engine.\n");
            goto cleanup;
        }
        play_sound();
    }

    int end = sound_frame_length(g_sound);
    while(callback(g_sound, g_current_frame, end))
    {
    }

cleanup:
    stop_engine();

    return NO_ERROR;
}

/////////////////////////////////////////////////////

void do_play_sound()
{
    play_sound();
}

void do_stop_sound()
{
    if (ma_sound_is_playing(&g_sound))
    {
        ma_sound_stop(&g_sound);
    }
}

void do_rewind_sound()
{
    ma_sound_seek_to_pcm_frame(&g_sound, 0);
    g_current_frame = 0;
}

void do_seek_backward()
{
    int seekPoint = milliseconds_to_frames(500.0);
    g_current_frame -= seekPoint;
    if (g_current_frame < 0)
    {
        g_current_frame = 0;
    }
    ma_sound_seek_to_pcm_frame(&g_sound, g_current_frame);
}

void do_seek_forward()
{
    do_stop_sound();

    int end = sound_frame_length(g_sound);
    int frames_seek = milliseconds_to_frames(500.0);

    int seek_frame = g_current_frame + frames_seek;
    if (seek_frame < (end - frames_seek) && g_current_frame < (end - frames_seek))
    {
        ma_sound_seek_to_pcm_frame(&g_sound, seek_frame);
        g_current_frame = seek_frame;
    }
    if (g_current_frame < end-1000)
    {
        do_play_sound();
    }
}
