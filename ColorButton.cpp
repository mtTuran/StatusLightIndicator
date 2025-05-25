#include "ColorButton.h"

bool ColorButton::enabled_all = true;

ColorButton::ColorButton(byte pin, Color color) {
  this->pin = pin;
  this->color = color;
  this->enabled = true;
}

void ColorButton::init_pin() {
  pinMode(this->pin, INPUT_PULLUP);
}

bool ColorButton::is_pressed() {
  return digitalRead(this->pin) == LOW;
}

bool ColorButton::is_enabled() {
  return this->enabled && enabled_all;
}

Color ColorButton::get_color() {
  return this->color;
}

void ColorButton::disable() {
  this->enabled = false;
}

void ColorButton::enable() {
  this->enabled = true;
}

void ColorButton::disable_all() {
  enabled_all = false;
}

void ColorButton::enable_all() {
  enabled_all = true;
}
