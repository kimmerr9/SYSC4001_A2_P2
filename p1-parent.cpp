/**
* @author Justin Kim
* @author Dorian Bansoodeb
*/

#include <iostream>
#include <unistd.h>

int main() {
    pid_t pid = fork();

    if(pid < 0) {
        std::cerr << "Fork failed\n";
        return 1;
    }

    if(pid == 0) {
        // Child Process
        int count = 0;
        std::cout << "child process PID: " << pid << std::endl;
        while(1) {
            std::cout << "[Child] count = " << count++ << std::endl;
            sleep(1);
        }
    } else {
        // Parent Process
        int count = 0;
        std::cout << "parent process PID: " << pid << std::endl;
        while(1) {
            std::cout << "[Parent] count = " << count++ << std::endl;
            sleep(1);
        }
    }

}
