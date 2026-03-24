#include "audio.h"
#include <getopt.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "keyboard.h"
#include "miniaudio.h"
#include "termcui.h"
#include "tui.h"

char progress_bar[50];

int game_loop(ma_sound g_sound, int g_current_frame, int end)
{
    if (!watch_key_press())
    {
        return 0;
    }
    if (ma_sound_at_end(&g_sound))
    {
        return 0;
    }
    if (g_current_frame >= end)
    {
        do_stop_sound();
        return 0;
    }
    // Dodgy UI
    int percent = (int)round(((float)g_current_frame / (float)end) * 100);
    printf(ESC_ERASE_LINE);
    for (int i = 0; i < 50; i++)
    {
        if (i < (percent / 2))
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

int headless_loop(ma_sound g_sound, int g_current_frame, int end)
{
    if (ma_sound_at_end(&g_sound))
    {
        return 0;
    }
    if (g_current_frame >= end)
    {
        do_stop_sound();
        return 0;
    }
    return 1;
}

static void usage(void)
{
    printf("Usage: floyd [-d] [-h] <file>\n");
    printf("\n");
    printf("  <file>            Audio file to play (.mp3, .wav, .flac)\n");
    printf("\n");
    printf("Options:\n");
    printf("  -d, --headless    Play without TUI (suitable for non-interactive use)\n");
    printf("  -h, --help        Show this help message\n");
}

static struct option long_opts[] = {
    { "headless", no_argument, NULL, 'd' },
    { "help",     no_argument, NULL, 'h' },
    { NULL,       0,           NULL,  0  },
};

int main(int argc, char **argv)
{
    int headless = 0;
    int opt;

    while ((opt = getopt_long(argc, argv, "dh", long_opts, NULL)) != -1)
    {
        switch (opt)
        {
        case 'd':
            headless = 1;
            break;
        case 'h':
            usage();
            return 0;
        default:
            usage();
            return 1;
        }
    }

    const char *file_path = argv[optind];

    if (file_path == NULL)
    {
        usage();
        return 1;
    }

    if (headless)
    {
        start_engine(file_path, &headless_loop);
        return 0;
    }

    enter_raw_mode();
    printf(ESC_HIDE_CURSOR);

    // Will block until key exit
    start_engine(file_path, &game_loop);

    printf(ESC_ERASE_LINE);
    printf(ESC_SHOW_CURSOR);
    exit_raw_mode();

    return 0;
}
