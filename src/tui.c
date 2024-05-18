#include <ctype.h>
#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <termios.h>
#include <unistd.h>

#include "termcui.h"
#include "log.h"

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
    printf(ESC_SET_ATTRIBUTE_MODE_1, 0);

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
