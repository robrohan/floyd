#include <stdio.h>
#include "audio.h"

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        printf("No input file.\n");
        return -1;
    }

    // Will block until key exit
    start_engine(argv[1]);
    return 0;
}
