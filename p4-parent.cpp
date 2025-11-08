#include <iostream>
#include <sys/wait.h> 
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <cstdlib>
#include <cstdio>

#include "shared.hpp"

#ifndef SHN_MODE
#define SHM_MODE 0600 
#endif


int main() {
    // create shared memory segment
    int shmID = shmget(IPC_PRIVATE, sizeof(Shared4), SHM_MODE); 
    if (shmID < 0) {
        perror("Error in shmget()");
    }
    // after the shared memory is created, it must be attached to an address space
    void* addr = shmat(shmID, nullptr, 0);
    if(addr == (void*)-1) {
       perror("Error in shmmat()");
    }

    Shared4* shm = static_cast<Shared4*>(addr);
    shm->multiple = 3;  
    shm->counter  = 0;

    pid_t pid = fork();

    if(pid < 0) {
        std::cout << "Fork failed" <<std::endl;
        return 1;
    }

    if(pid == 0) {
        // child Process
        std::cout << "child process pid: " << pid << std::endl;

        char shmID_str[32];
        std::snprintf(shmID_str, sizeof(shmID_str), "%d", shmID); // converts shmID into string
        execlp("./bin/p4-child", "p4-child", shmID_str, NULL); // need to pass it as args to allow child process to find memory space
    } else {
        // parent Process
        std::cout << "parent process pid: " << pid << std::endl;

        while(shm->counter < 0) {}
        
        while(1) {
            int count = (shm->counter)++;
            if (count % shm->multiple == 0) {
                std::cout << "[Process1] counter= " << count << " - is a multiple of " << shm->multiple << std::endl;
            }
            if (count > 500) {
                std::cout << "[Process1] counter= " << count << " - which is > 500. Terminating" << std::endl;
                break;
            }
            sleep(0.1); 
        }
        
        // wait return's a child process pid; if return's < 0 then error in child process. wait() waits for 0
        int status;
        pid_t child_pid = wait(&status);

        if(child_pid == -1) {
            std::cout << "child process terminated with a error" << std::endl;
            return 1;
        }
        std::cout << "[Process1] - Terminated" << std::endl;
        return 0;
    }
}
