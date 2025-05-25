#ifndef COLORBUTTON_H
#define COLORBUTTON_H

#include <Arduino.h>
#include "Color.h"

class ColorButton {
  private:
    static bool enabled_all;
    byte pin;
    Color color;
    bool enabled;
  public:
    ColorButton(byte pin, Color color);
    
    void init_pin();

    bool is_pressed();

    bool is_enabled();

    Color get_color();

    void disable();

    void enable();

    static void disable_all();

    static void enable_all();
};

#endif