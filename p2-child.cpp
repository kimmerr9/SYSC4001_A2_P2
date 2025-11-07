#include <iostream>
#include <unistd.h>

int main() {
    int count = 0;
    int cycles = 0;
    while(1) {
        if(count % 3 == 0)
            std::cout << "[Process2] Cycle: " << cycles << " — " << count << " is multiple of 3\n";
        count--;
        cycles++;
        sleep(1);
    }
}
