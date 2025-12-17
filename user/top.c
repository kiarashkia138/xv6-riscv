#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

const char*
get_state_str(enum procstate state)
{
    switch(state) {
        case UNUSED: return "UNUSED";
        case USED: return "USED";
        case SLEEPING: return "SLEEPING";
        case RUNNABLE: return "RUNNABLE";
        case RUNNING: return "RUNNING";
        case ZOMBIE: return "ZOMBIE";
        default: return "UNKNOWN";
    }
}

void
display_simple(void)
{
    printf("PID\tCommand\tSize of Process Memory\n");
    top();
}

static struct process_data procs[64];
static int proc_count;

void
print_tree(int parent_pid, int depth)
{
    int i, d;
    for(i = 0; i < proc_count; i++) {
        if(procs[i].parent_id == parent_pid) {
            for(d = 0; d < depth; d++) {
                if(d == depth - 1)
                    printf("  ├─");
                else
                    printf("  │ ");
            }
            
            printf("[%d] %s (%s) - %d bytes\n", 
                   procs[i].pid,
                   procs[i].name,
                   get_state_str(procs[i].state),
                   procs[i].head_size);
            
            print_tree(procs[i].pid, depth + 1);
        }
    }
}

void
display_tree(void)
{
    int pid = -1;
    
    proc_count = 0;
    
    while(proc_count < 64) {
        if(next_process(pid, &procs[proc_count]) == 1) {
            pid = procs[proc_count].pid;
            proc_count++;
        } else {
            break;
        }
    }
    
    printf("Process Tree (%d processes):\n", proc_count);
    printf("═══════════════════════════════════════════════\n");
    
    print_tree(0, 0);
    
    printf("═══════════════════════════════════════════════\n");
}

int
main(int argc, char *argv[]) 
{
    if(argc > 1 && (strcmp(argv[1], "-t") == 0 || strcmp(argv[1], "--tree") == 0)) {
        display_tree();
    } else {
        display_simple();
    }
    
    exit(0);
}
