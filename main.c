#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/resource.h>

bool validate_command(int argc, const char* argv[])
{
    //printf("Number of arguments: %d\n", argc);
    
    //for (int i = 0; i < argc; ++i)
    //{
        //printf("Argument: %s\n", argv[i]);
    //}
    
    //if (strcmp(command, "ls\n"))
    //{
        return true;
    //}
    
    //return false;
}

void execute_command(const char* command, const char* options, const char* arguments)
{
    struct rusage resource_usage;
    pid_t pid = fork();
    int status = 0;
    
    if (pid == -1)
    {
        printf("Failed to fork.\n");
    }
    
    else if (pid == 0)
    {
        char* args[4];
        
        args[0] = command;
        args[1] = options;
        args[2] = arguments;
        args[3] = NULL;
        
        getrusage(RUSAGE_SELF, &resource_usage);
        
        if (execvp(args[0], args) == -1)
        {
            return;
        }
    }
    
    else
    {
        waitpid(pid, &status, 0);
        
        if (status == 0)
        {
            getrusage(RUSAGE_SELF, &resource_usage);
        
            //printf("Child. Command status: %d\n", status);
            printf("Page Faults: %d\n", resource_usage.ru_majflt);
            // TODO: print more statistics.
        }
        
        //printf("Parent. Child status: %d\n", status);
    }
}

int main(int argc, char* argv[])
{
    if (validate_command(argc, argv))
    {
        //printf("Valid command.\n");
        execute_command(argv[1], argv[2], argv[3]);
    }
    
    else
    {
        printf("Invalid command.\n");
    } 
    
    return 0;
}
