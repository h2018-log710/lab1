#include <stdio.h>
#include <stdbool.h>

#include "command.h"

int main(int argc, char* argv[])
{
    if (argc > 1)
    {
        return execute_command(argc-1, &argv[1]);
    }
    
    else
    {
        printf("USAGE: %s [cmd]\n", argv[0]);
    }
}
