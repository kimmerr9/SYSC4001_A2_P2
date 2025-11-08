/**
* @author Justin Kim
* @author Dorian Bansoodeb
*/

#include <iostream>
#include <sys/wait.h> 
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <cstdlib>
#include <cstdio>

#include "shared.hpp"

#ifndef SHM_MODE
#define SHM_MODE 0600 
#endif

#define SEM_MODE 0600

int main() {
    // create shared memory segment
    int shmID = shmget(IPC_PRIVATE, sizeof(Shared5), SHM_MODE); //STA_MODE // SHM_MODE
    if (shmID < 0) {
        perror( "Error in shmget()");
    }
    // after the shared memory is created, it must be attached to an address space
    void* addr = shmat(shmID, 0, 0);
    if(addr == (int *)-1) {
        perror( "Error in shmmat()");
    }

    Shared5* shm = static_cast<Shared5*>(addr);
    shm->multiple = 3;  
    shm->counter  = -1; // wait for child to attach

    // create set of semaphores
    int semID = semget(IPC_PRIVATE, 1, SEM_MODE); //STA_MODE // SHM_MODE
    if (semID < 0) {
        perror("Error in semget()");
    }
    union semun arg;
    arg.val = 1;
    if (semctl(semID, 0, SETVAL, arg) < 0) {
        perror("semctl SETVAL"); 
        return 1; 
    }

    struct sembuf P = {0, -1, 0};
    struct sembuf V = {0, +1, 0};

    pid_t pid = fork();

    if(pid < 0) {
        std::cout << "Fork failed" <<std::endl;
        return 1;
    }

    if(pid == 0) {
        // child Process
        std::cout << "child process pid: " << pid << std::endl;

        char shmID_str[32], semID_str[32];
        std::snprintf(shmID_str, sizeof(shmID_str), "%d", shmID); // converts shmID into string
        std::snprintf(semID_str, sizeof(semID_str), "%d", semID); // converts semID into string
        execlp("./bin/p5-child", "p5-child", shmID_str, semID_str, NULL); // need to pass it as args to allow child process to find memory space
    } else {
        // parent Process
        std::cout << "parent process pid: " << pid << std::endl;

        // barrier - wait until child flips counter >= 0
        while(1) {
            if (semop(semID, &P, 1) < 0) { 
                perror("Error semop P"); 
                break; 
            }
            bool ready = (shm->counter >= 0);
            if (semop(semID, &V, 1) < 0) { 
                perror("Error semop V"); 
                break; 
            }
            if (ready) {
                break;
            }
        }
    
        while(1) {
            int lock_count, lock_m;
            if (semop(semID, &P, 1) < 0) { 
                perror("Error semop P"); 
                break; 
            }
            lock_count = (shm->counter)++;      
            lock_m = shm->multiple;        
            if (semop(semID, &V, 1) < 0) { 
                perror("Error semop V"); 
                break; 
            }

            if (lock_count % lock_m == 0) {
                std::cout << "[Process1] counter= " << lock_count << " - is a multiple of " << lock_m << std::endl;
            }
            if (lock_count > 500) {
                std::cout << "[Process1] counter= " << lock_count << " - which is > 500. Terminating" << std::endl;
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
        if (shmctl(shmID, IPC_RMID, NULL) < 0) {
            perror("cant IPC_RMID shared");
        }
        if (semctl(semID, 0, IPC_RMID) < 0) {
            perror("semctl IPC_RMID");
        }
        std::cout << "[Process1] - Terminated" << std::endl;
        return 0;
    }
}

