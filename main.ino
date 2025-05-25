#include "Color.h"
#include "Led.h"
#include "ColorButton.h"

const Color RED = {255, 0, 0};
const Color ORANGE = {230, 200, 0};
const Color YELLOW = {255, 255, 0};
const Color GREEN = {0, 255, 0};

/*
not every button connected to an ESP32 GPIO pin can utilize internal pull-up resistors. 
Specifically, GPIO pins 34-39 are input-only and do not have internal pull-up or pull-down resistors
*/

Led led(14, 27, 26);
ColorButton red_bt(18, RED);
ColorButton orange_bt(32, ORANGE);
ColorButton yellow_bt(33, YELLOW);
ColorButton green_bt(25, GREEN);

void setup() {
  led.init_pins();
  led.off();

  red_bt.init_pin();
  orange_bt.init_pin();
  yellow_bt.init_pin();
  green_bt.init_pin();
}

void loop() {  
  if (red_bt.is_pressed()) {
    if (red_bt.is_enabled()) {
      led.switch_colors(red_bt.get_color());
      ColorButton::disable_all();
    }  
  }
  else if (orange_bt.is_pressed()) {
    if (orange_bt.is_enabled()) {
      led.switch_colors(orange_bt.get_color());
      ColorButton::disable_all();
    }
  }
  else if (yellow_bt.is_pressed()) {
    if (yellow_bt.is_enabled()) {
      led.switch_colors(yellow_bt.get_color());
      ColorButton::disable_all();
    }
  }
  else if (green_bt.is_pressed()) {
    if (green_bt.is_enabled()) {
      led.switch_colors(green_bt.get_color());  
      ColorButton::disable_all();
    }
  }
  else {
    ColorButton::enable_all();
  }
  delay(10);  
}
