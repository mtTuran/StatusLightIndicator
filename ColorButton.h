#ifndef COLORBUTTON_H
#define COLORBUTTON_H

#include <Arduino.h>
#include "Color.h"
#include "SimpleButton.h"

class ColorButton : public SimpleButton {
  private:
    static bool enabled_all;
    Color color;
  public:
    ColorButton(byte pin, Color color);
    
    bool is_enabled() override;

    Color get_color();

    static void disable_all();

    static void enable_all();
};

#endif
