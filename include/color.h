#ifndef COLOR_H
#define COLOR_H

#include <FL/Fl.H>
#include <string>

class ColorUtil {
public:
    static constexpr const char* GREEN   = "C0DEB9";
    static constexpr const char* BLUE    = "BFD9F4";
    static constexpr const char* PINK    = "F1B4D2";
    static constexpr const char* GREY    = "CCCCCC";
    static constexpr const char* YELLOW  = "F2DD65";
    static constexpr const char* BEIDGE  = "FFF8D1";

public:
    // Fl_Color -> "RRGGBB"
    static std::string color2str(Fl_Color fcolor);

    // "RRGGBB" -> Fl_Color
    static Fl_Color str2color(const std::string& scolor);
};

#endif // COLOR_H
