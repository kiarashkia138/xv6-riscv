#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void
child_process(int level, int max_level)
{
    int pid;
    
    if(level < max_level) {
        pid = fork();
        
        if(pid < 0) {
            printf("Fork failed at level %d\n", level);
        } else if(pid == 0) {
            child_process(level + 1, max_level);
        } else {
            pause(100);
            wait(0);
        }
    } else {
        pause(200);
    }
}

int
main(int argc, char *argv[])
{
    int i;
    int num_children = 3;
    int depth = 2;
    int pid;
    
    if(argc > 1) {
        num_children = atoi(argv[1]);
        if(num_children < 1) num_children = 1;
        if(num_children > 5) num_children = 5;
    }
    
    if(argc > 2) {
        depth = atoi(argv[2]);
        if(depth < 1) depth = 1;
        if(depth > 4) depth = 4;
    }
    
    printf("=== Fork Tree Test ===\n");
    printf("Creating %d children with depth %d\n", num_children, depth);
    printf("Parent PID: %d\n", getpid());
    
    for(i = 0; i < num_children; i++) {
        pid = fork();
        
        if(pid < 0) {
            printf("Fork failed for child %d\n", i);
        } else if(pid == 0) {
            child_process(1, depth);
            exit(0);
        }
    }
    
    printf("\nProcess Tree:\n");
    pause(50);
    
    pid = fork();
    if(pid == 0) {
        char *args[] = {"top", "-t", 0};
        exec("top", args);
        printf("Failed to exec top\n");
        exit(1);
    } else {
        wait(0);
    }
    
    for(i = 0; i < num_children; i++) {
        wait(0);
    }
    
    exit(0);
}
