#ifndef H_FLOYD_AUDIO
#define H_FLOYD_AUDIO

#include "miniaudio.h"

int sound_frame_length(ma_sound sound);

void do_play_sound();
void do_stop_sound();
void do_rewind_sound();
void do_seek_backward();
void do_seek_forward();

typedef int (*engine_loop)(ma_sound sound, int frame, int end);

int load_sound(const char * file_path);
int start_engine(const char* file_path, engine_loop callback);
void stop_engine();
int play_sound();


#endif