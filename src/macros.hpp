#ifndef MACROS_HPP
#define MACROS_HPP

#include <iostream>

inline void errExit(const char *msg) {
    std::cerr << msg << std::endl;
    exit(1); 
}

#endif