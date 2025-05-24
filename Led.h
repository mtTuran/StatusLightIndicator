#ifndef LED_H
#define LED_H

#include <Arduino.h>
#include "Color.h"

class Led {
  private:
    byte red_pin;
    byte green_pin;
    byte blue_pin;
    Color curr_color;

    void write_led();

  public:
    Led(byte red_pin, byte green_pin, byte blue_pin);

    void init_pins();

    void off();

    void switch_colors(Color color);
};

#endif