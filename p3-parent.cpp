/**
* @author Justin Kim
* @author Dorian Bansoodeb
*/

#include <iostream>
#include <unistd.h>
#include <sys/wait.h> 

int main() {
    pid_t pid = fork();

    if(pid < 0) {
        std::cout << "Fork failed" <<std::endl;
        return 1;
    }

    if(pid == 0) {
        // child Process
        std::cout << "child process pid: " << pid << std::endl;
        execlp("./bin/p3-child", "p3-child", NULL);
    } else {
        // parent Process
        std::cout << "parent process pid: " << pid << std::endl;
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
