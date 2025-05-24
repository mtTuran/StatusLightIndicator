#include "Led.h"

Led::Led(byte red_pin, byte green_pin, byte blue_pin) {
  this->red_pin = red_pin;
  this->green_pin = green_pin;
  this->blue_pin = blue_pin;
  this->curr_color = {0, 0, 0};
}

void Led::write_led() {
  analogWrite(this->red_pin, this->curr_color.red_scale);
  analogWrite(this->green_pin, this->curr_color.green_scale);
  analogWrite(this->blue_pin, this->curr_color.blue_scale);
}

void Led::init_pins() {
  pinMode(this->red_pin, OUTPUT);
  pinMode(this->green_pin, OUTPUT);
  pinMode(this->blue_pin, OUTPUT);
}

void Led::off() {
  this->curr_color.red_scale = 0;
  this->curr_color.green_scale = 0;
  this->curr_color.blue_scale = 0;
  write_led();
}

void Led::switch_colors(Color color) {
  if (this->curr_color.red_scale == color.red_scale && this->curr_color.green_scale == color.green_scale && this->curr_color.blue_scale == color.blue_scale) {
    off();
  }
  else {
    this->curr_color.red_scale = color.red_scale;
    this->curr_color.green_scale = color.green_scale;
    this->curr_color.blue_scale = color.blue_scale;
    write_led();
  }
}
