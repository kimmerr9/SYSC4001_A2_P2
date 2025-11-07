#ifndef SHARED_HPP
#define SHARED_HPP

struct Shared4 {
    int multiple;
    int counter;
};

struct Shared5 {
    int multiple;
    volatile int counter; 
};

union semun {
    int val;
    struct semid_ds* buf;
    unsigned short* array;
};
#endif