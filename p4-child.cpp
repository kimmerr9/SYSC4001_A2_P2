/**
* @author Justin Kim
* @author Dorian Bansoodeb
*/

#include <iostream>
#include <unistd.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/shm.h>

#include "shared.hpp"

int main(int argc, char* argv[]) {
    // get id from parent and attach to address space
    int shmID = std::atoi(argv[1]);             
    void* addr = shmat(shmID, nullptr, 0);      
    if (addr == (void*)-1) { 
        perror("Error in shmmat()");
    }
    Shared4* shm = static_cast<Shared4*>(addr);

    while(shm->counter <= 100) {}

    int last_seen = -1;
    while (1) {
        if (shm->counter != last_seen) {
            if (shm->counter % shm->multiple == 0) {
                std::cout << "[Process2] observed counter=" << shm->counter << " - is a multiple of " << shm->multiple << std::endl;
            }
            last_seen = shm->counter;
        }

        if (shm->counter > 500) {
            std::cout << "[Process2] observed counter=" << shm->counter << " which is > 500. Terminating.\n";
            break;
        }
    }

    if (shmdt(addr) < 0) {
        perror("error when detaching child to shared memory segment");
    }
    std::cout << "[Process2] - Terminated" << std::endl;
    return 0;
}

