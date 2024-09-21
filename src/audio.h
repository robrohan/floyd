#ifndef H_FLOYD_AUDIO
#define H_FLOYD_AUDIO

#include "miniaudio.h"

int sound_frame_length(ma_sound sound);

int play_sound(void);
void stop_engine(void);

void do_play_sound(void);
void do_stop_sound(void);
void do_rewind_sound(void);
void do_seek_backward(void);
void do_seek_forward(void);

typedef int (*engine_loop)(ma_sound sound, int frame, int end);

int load_sound(const char * file_path);
int start_engine(const char* file_path, engine_loop callback);
void stop_engine(void);
int play_sound(void);

#endif
