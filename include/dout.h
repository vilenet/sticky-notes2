#ifndef DOUT_H
#define DOUT_H

#include <iostream>
#include <iomanip>
#include <cstring>

extern int debug_counter;

#define FILENAME (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)

#define dbgout(msg) \
    std::cout << "[DEBUG: " \
              << std::setw(4) << debug_counter++ << " | " \
              << std::setw(12) << std::left << FILENAME << " " \
              << std::setw(3) << __LINE__ << " | " \
              << std::setw(18) << std::left << __FUNCTION__ << " - " \
              << std::setw(30) << std::left << msg \
              << "]" << std::endl


#endif // DOUT_H
