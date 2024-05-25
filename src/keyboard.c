#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <ctype.h>
#include <string.h>
#include <sys/types.h>
#include <termios.h>

#include "keyboard.h"
#include "log.h"
#include "audio.h"

#include "tui.h"
#include "termcui.h"

#define CTRL_KEY(k) ((k)&0x1F)

#define CMD(key, func) { key, (void *)(func) },
#define CMD_DEF_END { 0, NULL }

typedef struct CmdDef {
    int key;
    void *func;
} CmdDef;

//////////////////////////////////////////////////
// void do_hello()
// {
//     printf("Hello there\n\r");
// }
/////////////////////////////////////////////////

CmdDef commands[] = 
{
    // CMD( 'h', &do_hello )
    CMD( 'p', &do_play_sound )
    CMD( 's', &do_stop_sound )
    
    CMD( 'r', &do_rewind_sound )
    CMD( HOME_KEY, &do_rewind_sound )
    
    CMD( 'j', &do_seek_backward )
    CMD( ARROW_LEFT, &do_seek_backward )
    CMD( ';', &do_seek_forward )
    CMD( ARROW_RIGHT, &do_seek_forward )

    CMD_DEF_END,
};

int key_read()
{
    int nread;
    char c;
    nread = read(STDIN_FILENO, &c, 1);
    // while ((nread = read(STDIN_FILENO, &c, 1)) != 1)
    // {
    //     if (nread == -1 && errno != EAGAIN)
    //         panic(__LINE__, "%i: read error", errno);
    // }

    // handle escape sequences
    if (c == '\x1b')
    {
        char seq[3];
        if (read(STDIN_FILENO, &seq[0], 1) != 1)
            return '\x1b';
        if (read(STDIN_FILENO, &seq[1], 1) != 1)
            return '\x1b';
        if (seq[0] == '[')
        {
            if (seq[1] >= '0' && seq[1] <= '9')
            {
                if (read(STDIN_FILENO, &seq[2], 1) != 1)
                    return '\x1b';
                if (seq[2] == '~')
                {
                    switch (seq[1])
                    {
                    case '1':
                        return HOME_KEY;
                    case '3':
                        return DEL_KEY;
                    case '4':
                        return END_KEY;
                    case '5':
                        return PAGE_UP;
                    case '6':
                        return PAGE_DOWN;
                    case '7':
                        return HOME_KEY;
                    case '8':
                        return END_KEY;
                    }
                }
            }
            else
            {
                // escape sequences for arrow keys
                switch (seq[1])
                {
                case 'A':
                    return ARROW_UP;
                case 'B':
                    return ARROW_DOWN;
                case 'C':
                    return ARROW_RIGHT;
                case 'D':
                    return ARROW_LEFT;
                case 'H':
                    return HOME_KEY;
                case 'F':
                    return END_KEY;
                }
            }
        }
        else if (seq[0] == 'O')
        {
            switch (seq[1])
            {
            case 'H':
                return HOME_KEY;
            case 'F':
                return END_KEY;
            }
        }

        return '\x1b';
    }
    else
    {
        return c;
    }
}

int watch_key_press()
{
    int c = key_read();
    CmdDef *d = commands;
    while(d->func != NULL)
    {
        if(d->key == c) 
        {
            void (*fn)() = d->func;
            (*fn)();
        }
        d++;
    }

    switch (c)
    {
    case CTRL_KEY('q'):
    case CTRL_KEY('c'):
    case 'q':
         return 0;
    }
    return 1;
}