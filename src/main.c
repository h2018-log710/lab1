#include <stdio.h>
#include <stdbool.h>

#include "command.h"

int main(int argc, char* argv[])
{
	execute_command(argv[1], argv[2], argv[3]);
    return 0;
}
