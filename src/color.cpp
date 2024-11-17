#include "color.h"
#include "dout.h"
#include <sstream>
#include <iomanip>


std::string ColorUtil::color2str(Fl_Color fcolor) { dbgout("");
    unsigned char r, g, b;
    Fl::get_color(fcolor, r, g, b);
    std::ostringstream oss;
    oss << std::hex << std::setw(2) << std::setfill('0') << (int)r
        << std::setw(2) << std::setfill('0') << (int)g
        << std::setw(2) << std::setfill('0') << (int)b;
    return oss.str();
}

Fl_Color ColorUtil::str2color(const std::string &scolor) { dbgout("");
    if (scolor.length() != 6) return FL_BLACK;
    uint32_t ucolor = std::stoul(scolor, nullptr, 16);
    unsigned char r = (ucolor >> 16) & 0xFF;
    unsigned char g = (ucolor >> 8) & 0xFF;
    unsigned char b = ucolor & 0xFF;
    return fl_rgb_color(r, g, b);
}