#ifndef DOUT_H
#define DOUT_H

#include <iostream>
#include <iomanip>
#include <cstring>
#include <string>

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

//TODO 
// struct STracer {
//     std::string funcName;
//     STracer(const std::string& func) : funcName(func) 
//     { std::cout << "Enter: " << funcName << std::endl; }
//     ~STracer() 
//     { std::cout << "Exit: " << funcName << std::endl; }
// };

//#define TRACE_FUNCTION STracer tracer(__FUNCTION__)

#endif // DOUT_H
