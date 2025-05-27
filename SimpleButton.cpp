#include "SimpleButton.h"

SimpleButton::SimpleButton(byte pin) {
  this->pin = pin;
  this->enabled = true;
}

void SimpleButton::init_pin() {
  pinMode(this->pin, INPUT_PULLUP);
}

bool SimpleButton::is_pressed() {
  return digitalRead(this->pin) == LOW;
}

bool SimpleButton::is_enabled() {
  return this->enabled;
}

void SimpleButton::disable() {
  this->enabled = false;
}

void SimpleButton::enable() {
  this->enabled = true;
}
