#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"

#include "keyboard.h"
#include <stdio.h>

ma_engine g_engine;
ma_sound g_sound;

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

int start_engine(const char *file_path)
{
    ma_result result = ma_engine_init(NULL, &g_engine);
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

    enter_raw_mode();
    while (watch_key_press())
    {
    }

cleanup:
    ma_engine_uninit(&g_engine);
    ma_sound_uninit(&g_sound);
    exit_raw_mode();

    return NO_ERROR;
}

/////////////////////////////////////////////////////

int milliseconds_to_frames(float mseconds)
{
    int seekPointInFrames = (mseconds * ma_engine_get_sample_rate(&g_engine)) / 1000;
    return seekPointInFrames;
}

int get_current_frame()
{
    int currentFrame = 0;
    // ma_playback_state state;
    // ma_sound_get_playback_state(&g_sound, &state);
    // if (state == MA_SOUND_STATE_PLAYING)
    // {
    //     currentFrame = ma_sound_get_currentFrame(&g_sound);
    // }
    return currentFrame;
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
}

void do_seek_backward()
{
    int seekPoint = milliseconds_to_frames(500.0);
    int current = get_current_frame();
    ma_sound_seek_to_pcm_frame(&g_sound, current - seekPoint);
}

void do_seek_forward()
{
    int seekPoint = milliseconds_to_frames(500.0);
    int current = get_current_frame();
    ma_sound_seek_to_pcm_frame(&g_sound, current + seekPoint);
}
