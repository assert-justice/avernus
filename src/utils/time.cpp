#include "time.hpp"
#include "sys/time.h"

// TODO: write portable version
double getTime(){
    struct timeval current_time;
    gettimeofday(&current_time, 0);
    return (double)current_time.tv_sec + (double)current_time.tv_usec / 1000000; 
}