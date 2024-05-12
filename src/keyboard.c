#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <ctype.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <termios.h>

#include "keyboard.h"
#include "log.h"

#define CTRL_KEY(k) ((k)&0x1F)

struct termios org_termios;

void exit_raw_mode()
{
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &org_termios) == -1)
    {
        panic(__LINE__, "tcsetattr");
    }
}

void enter_raw_mode()
{
    if (tcgetattr(STDIN_FILENO, &org_termios) == -1)
    {
        panic(__LINE__, "tcsetattr");
    }
    atexit(exit_raw_mode);

    struct termios raw = org_termios;
    // flag - IXON turns ctrl+s && ctrl+q software signals off
    // flag - ICRNL turns ctrl+m carriage return off
    raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
    // flag - OPOST turns post-processing of output off
    raw.c_oflag &= ~(OPOST);
    raw.c_cflag |= (CS8);
    // flag - ICANON turns canonical mode off
    // flag - ISIG turns ctrl+c && ctrl+z signals off
    raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
    /* adding timeouts for read */
    raw.c_cc[VMIN] = 0;
    raw.c_cc[VTIME] = 1;

    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1)
    {
        panic(__LINE__, "tcsetattr");
    }
}

int key_read()
{
    int nread;
    char c;
    while ((nread = read(STDIN_FILENO, &c, 1)) != 1)
    {
        if (nread == -1 && errno != EAGAIN)
            panic(__LINE__, "%i: read error", errno);
    }

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

void watch_key_press()
{
    int c = key_read();
    switch (c)
    {
    case CTRL_KEY('q'):
    case CTRL_KEY('c'):
        // write(STDOUT_FILENO, "\x1b[2J", 4);
        // write(STDOUT_FILENO, "\x1b[H", 3);
        exit(0);
        // break;
    case HOME_KEY:
    case END_KEY:
    case DEL_KEY:
    case PAGE_UP:
    case PAGE_DOWN:
    case ARROW_UP:
    case ARROW_DOWN:
    case ARROW_RIGHT:
    case ARROW_LEFT:
        printf("d'know");
    }
}