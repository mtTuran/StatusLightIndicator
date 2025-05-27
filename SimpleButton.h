#ifndef SIMPLEBUTTON_H
#define SIMPLEBUTTON_H

#include <Arduino.h>

class SimpleButton {
  protected:
    byte pin;
    bool enabled;
  public:
    SimpleButton(byte pin);

    void init_pin();

    bool is_pressed();

    virtual bool is_enabled();

    void disable();

    void enable();
};

#endif
