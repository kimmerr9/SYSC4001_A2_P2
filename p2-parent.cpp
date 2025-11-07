#include <iostream>
#include <unistd.h>

int main() {
    pid_t pid = fork();

    if(pid < 0) {
        std::cout << "Fork failed" <<std::endl;
        return 1;
    }

    if(pid == 0) {
        // child Process
        std::cout << "child process PID: " << pid << std::endl;
        execlp("./bin/p2-child", "p2-child", NULL);
    } else {
        // parent Process
        std::cout << "parent process PID: " << pid << std::endl;
        int count = 0;
        int cycles = 0;
        while(1) {
            // o/p multiple of three, otherwise show cycle number
            if(count % 3 == 0) {
                std::cout << "Cycle: " << cycles << " — " << count << " is multiple of 3\n";
            }
            
            else {
                std::cout << "Cycle: " << cycles  << "\n";
            }
            count++;
            cycles++;
            sleep(1);
        }
    }
}