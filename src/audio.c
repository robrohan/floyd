#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"

#include <stdio.h>
#include "keyboard.h"

ma_engine g_engine;
ma_sound g_sound;

#define ERROR 1
#define NO_ERROR 0

int load_sound(const char * file_path)
{
    if(ma_sound_init_from_file(&g_engine, file_path, 0, NULL, NULL, &g_sound) != MA_SUCCESS) 
    {
        printf("Failed to start playback engine.\n");
        return ERROR;
    }
}

int play_sound()
{
    if(ma_sound_start(&g_sound) != MA_SUCCESS) {
        return ERROR;
    }
    return NO_ERROR;
}

int start_engine(const char* file_path)
{
    ma_result result = ma_engine_init(NULL, &g_engine);
    if (result != MA_SUCCESS)
    {
        return ERROR;
    }

    if (file_path != NULL) 
    {
        int err = load_sound(file_path);
        if(err) {
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

void do_play_sound() 
{
    play_sound();
}

void do_stop_sound() 
{
    if(ma_sound_is_playing(&g_sound)) {
        ma_sound_stop(&g_sound);
    }
}

void do_rewind_sound()
{
    ma_sound_seek_to_pcm_frame(&g_sound, 0);
}

// void do_seek_backwards() {
//     seekPointInFrames = (seekPointInMilliseconds * ma_engine_get_sample_rate(&g_engine)) / 1000;
//     ma_sound_seek_to_pcm_frame(&g_sound, seekPointInFrames);
// }
