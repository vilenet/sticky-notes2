#ifndef COLOR_H
#define COLOR_H

#include <FL/Fl.H>
#include <string>

class ColorUtil {
public:
    // Fl_Color -> "RRGGBB"
    static std::string color2str(Fl_Color fcolor);

    // "RRGGBB" -> Fl_Color
    static Fl_Color str2color(const std::string& scolor);
};

#endif // COLOR_H
