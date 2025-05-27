#include "ColorButton.h"

bool ColorButton::enabled_all = true;

ColorButton::ColorButton(byte pin, Color color)
  : SimpleButton(pin)
{
  this->color = color;
}

bool ColorButton::is_enabled() {
  return SimpleButton::is_enabled() && enabled_all;
}

Color ColorButton::get_color() {
  return this->color;
}

void ColorButton::disable_all() {
  enabled_all = false;
}

void ColorButton::enable_all() {
  enabled_all = true;
}
