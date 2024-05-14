#ifndef H_FLOYD_AUDIO
#define H_FLOYD_AUDIO

int load_sound(const char * file_path);
int start_engine(const char* file_path);
int play_sound();

void do_play_sound();
void do_stop_sound();
void do_rewind_sound();
void do_seek_backward();
void do_seek_forward();

#endif