#include <iostream>
#include <unistd.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/sem.h>

#include "shared.hpp"

int main(int argc, char* argv[]) {
    // get id from parent and attach to address space
    int shmID = std::atoi(argv[1]);
    int semID = std::atoi(argv[2]);              
    void* addr = shmat(shmID, nullptr, 0);      
    if(addr == (void*)-1) { 
        std::cout << "error when re-attaching child to the shared memory space" << std::endl; 
        return 1; 
    }
    Shared5* shm = static_cast<Shared5*>(addr);

    struct sembuf P = {0, -1, 0};
    struct sembuf V = {0, +1, 0};

    if(semop(semID, &P, 1) < 0) { 
        perror("semop P"); 
        return 1; 
    }
    if (shm->counter < 0) {
        shm->counter = 0;
    }
    int m = shm->multiple;                 
    if(semop(semID, &V, 1) < 0) { 
        perror("semop V"); 
        return 1; 
    }

    // wait for counter to get to 100
    while(1) {
        int lock_count;
        if(semop(semID, &P, 1) < 0) { 
            perror("semop P"); 
            return 1; 
        }
        lock_count = shm->counter;
        if(semop(semID, &V, 1) < 0) { 
            perror("semop V"); 
            return 1; 
        }
        if(lock_count > 100) {
            break;
        }
    }
    // main look keep logic but protect
    int last_seen = -1;
    while(1) {
        int c;
        if (semop(semID, &P, 1) < 0) { 
            perror("semop P"); 
            return 1; 
        }
        c = shm->counter;
        if (semop(semID, &V, 1) < 0) { 
            perror("semop V"); 
            return 1; 
        }

        if (c != last_seen) {
            if (c % m == 0) {
                std::cout << "[Process2] observed counter= " << c << " - is a multiple of " << m << std::endl;
            }
            last_seen = c;
        }
        if (c > 500) {
            std::cout << "[Process2] observed counter= " << c << " which is > 500. Terminating.\n";
            break;
        }
    }

    if(shmdt(addr) < 0) {
        std::cout << "error when deattaching child to shared memory space" << std::endl;
        return 1;
    }
    std::cout << "[Process2] - Terminated" << std::endl;
    return 0;
}