#include <iostream>
#include <unistd.h>

int main(int argc, char* argv[]) {
    int count = 0;
    int cycles = 0;
    while(1) {
        if(count % 3 == 0)
            std::cout << "[Process2] Cycle: " << cycles << " — " << count << " is multiple of 3\n";
        count--;
        cycles++;
        sleep(0.1);

        if(count <= -500) {
            std::cout << "[Process2] Reached - " << count << std::endl;
            break;
        }
    }
    std::cout << "[Process2] - Terminated" << std::endl; // push
    return 0;
}
