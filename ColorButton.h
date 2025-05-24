#ifndef COLORBUTTON_H
#define COLORBUTTON_H

#include <Arduino.h>
#include "Color.h"

class ColorButton {
  private:
    byte pin;
    Color color;
  public:
    ColorButton(byte pin, Color color);
    
    void init_pin();

    bool is_pressed();

    Color get_color();
};

#endif