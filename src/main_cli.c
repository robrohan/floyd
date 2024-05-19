#include <stdio.h>
#include <math.h>
#include "audio.h"

#include "miniaudio.h"
#include "keyboard.h"
#include "termcui.h"
#include "tui.h"

char progress_bar[50];

int game_loop(ma_sound g_sound, int g_current_frame, int end)
{
    if(!watch_key_press()) 
    {
        return 0;
    }
    if(ma_sound_at_end(&g_sound)) {
        return 0;
    }
    if (g_current_frame >= end)
    {
        do_stop_sound();
        return 0;
    }
    // Dodgy UI
    int percent = (int)round(((float)g_current_frame / (float)end)*100);
    printf(ESC_ERASE_LINE);
    for(int i=0; i<50; i++)
    {
        if(i < (percent/2)) 
        {
            progress_bar[i] = '*';
        }
        else
        {
            progress_bar[i] = ' ';
        }
    }
    printf("%3d%% [%s]\n", percent, progress_bar);
    printf(ESC_CURSOR_UP, 1);
    printf(ESC_CURSOR_BACKWARD, 57);

    return 1;
}

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        printf("No input file.\n");
        return -1;
    }

    enter_raw_mode();
    printf(ESC_HIDE_CURSOR);
    
    // Will block until key exit
    start_engine(argv[1], &game_loop);

    printf(ESC_ERASE_LINE);
    printf(ESC_SHOW_CURSOR);
    exit_raw_mode();

    return 0;
}
