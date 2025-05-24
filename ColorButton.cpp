#include "ColorButton.h"

ColorButton::ColorButton(byte pin, Color color) {
  this->pin = pin;
  this->color = color;
}

void ColorButton::init_pin() {
  pinMode(this->pin, INPUT_PULLUP);
}

bool ColorButton::is_pressed() {
  if (digitalRead(this->pin) == LOW) return true;
  else return false;
}

Color ColorButton::get_color() {
  return this->color;
}
