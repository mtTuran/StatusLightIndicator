#include "ColorButton.h"

ColorButton::ColorButton(byte pin, Color color) {
  this->pin = pin;
  this->color = color;
}

void ColorButton::init_pin() {
  pinMode(this->pin, INPUT_PULLUP);
}

Color ColorButton::pressed() {
  return this->color;
}
