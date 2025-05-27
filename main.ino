#include <Ticker.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "AiEsp32RotaryEncoder.h"
#include "Color.h"
#include "Led.h"
#include "ColorButton.h"

void display_timer(const String &buf, int x, int y);
void display_selector();
void IRAM_ATTR read_choice_encoder_ISR();
void IRAM_ATTR read_increment_encoder_ISR();
void choice_encoder_loop();
void increment_encoder_loop();
void color_selection_service();
void start_cancel_pressed();
void start_countdown();
void finish_countdown();
void countdown();

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C
#define TIMER_CURSOR_X 64
#define TIMER_CURSOR_Y 32

#define CHOICE_ENCODER_A_PIN 5
#define CHOICE_ENCODER_B_PIN 17
#define INCREMENT_ENCODER_A_PIN 16
#define INCREMENT_ENCODER_B_PIN 4
#define ROTARY_ENCODER_BUTTON_PIN -1
#define ROTARY_ENCODER_VCC_PIN -1
#define ROTARY_ENCODER_STEPS 4

long hours = 0;
long minutes = 0;
byte seconds = 0;
long incrementer_selection = 1;
String timer_str;

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
AiEsp32RotaryEncoder choice_encoder = AiEsp32RotaryEncoder(CHOICE_ENCODER_A_PIN, CHOICE_ENCODER_B_PIN, ROTARY_ENCODER_BUTTON_PIN, ROTARY_ENCODER_VCC_PIN, ROTARY_ENCODER_STEPS);
AiEsp32RotaryEncoder increment_encoder = AiEsp32RotaryEncoder(INCREMENT_ENCODER_A_PIN, INCREMENT_ENCODER_B_PIN, ROTARY_ENCODER_BUTTON_PIN, ROTARY_ENCODER_VCC_PIN, ROTARY_ENCODER_STEPS);
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
Ticker periodic_ticker;

//************************************** (main) setup - loop **************************************
void setup() {
  Serial.begin(115200);

  led.init_pins();
  led.off();

  red_bt.init_pin();
  orange_bt.init_pin();
  yellow_bt.init_pin();
  green_bt.init_pin();

  choice_encoder.begin();
	choice_encoder.setup(read_choice_encoder_ISR);
	choice_encoder.setBoundaries(0, 1, false); // true means values form a circle
  choice_encoder.disableAcceleration();
  choice_encoder.setEncoderValue(incrementer_selection);

  increment_encoder.begin();
	increment_encoder.setup(read_increment_encoder_ISR);
	increment_encoder.setBoundaries(0, 59, true);
  increment_encoder.disableAcceleration();
  increment_encoder.setEncoderValue(minutes);

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  else {
    display.clearDisplay();
    display.setTextSize(2);
    display.setTextColor(WHITE);
    timer_str = (hours < 10 ? "0" : "") + String(hours) + ":" + (minutes < 10 ? "0" : "") + String(minutes);
    display.clearDisplay();
    display_timer(timer_str);
    display_selector();
    display.display();
  }
}

void loop() {  
  color_selection_service();
  choice_encoder_loop();
  increment_encoder_loop();
  delay(10);  
}

//************************************** function definitions **************************************
void display_timer(const String &buf) {
    int16_t x1, y1;
    uint16_t w, h;
    display.getTextBounds(buf, TIMER_CURSOR_X, TIMER_CURSOR_Y, &x1, &y1, &w, &h); //calc width and height of new string
    display.setCursor(TIMER_CURSOR_X - (w / 2), TIMER_CURSOR_Y - (h / 2));
    display.print(buf);
}

void display_selector() {
  int16_t x0, y0, x1, y1;
  uint16_t w, h;
  display.getTextBounds("00:00", TIMER_CURSOR_X, TIMER_CURSOR_Y, &x1, &y1, &w, &h);
  if (incrementer_selection == 0) {
  x0 = TIMER_CURSOR_X - w / 2 - 5;
  x1 = TIMER_CURSOR_X - 5;
  } 
  else {
  x0 = TIMER_CURSOR_X + w / 2 + 2;
  x1 = TIMER_CURSOR_X + 2;
  }
  y0 = TIMER_CURSOR_Y + (h / 2);
  y1 = TIMER_CURSOR_Y + (h / 2);
  display.drawLine(x0, y0, x1, y1, WHITE);
}

void IRAM_ATTR read_choice_encoder_ISR() {
	choice_encoder.readEncoder_ISR();
}

void IRAM_ATTR read_increment_encoder_ISR() {
	increment_encoder.readEncoder_ISR();
}

void choice_encoder_loop() {
	if (choice_encoder.encoderChanged()) {
    incrementer_selection = choice_encoder.readEncoder();
    if (incrementer_selection == 1) {
      increment_encoder.setBoundaries(0, 59, true);
      increment_encoder.setEncoderValue(minutes);
    }
    else {
      increment_encoder.setBoundaries(0, 72, true);
      increment_encoder.setEncoderValue(hours);
    }
    display.clearDisplay();
    display_timer(timer_str);
    display_selector();
    display.display();
	}
}

void increment_encoder_loop() {
  if (increment_encoder.encoderChanged()) {
    if (incrementer_selection == 1) minutes = increment_encoder.readEncoder();
    else hours = increment_encoder.readEncoder();
    timer_str = (hours < 10 ? "0" : "") + String(hours) + ":" + (minutes < 10 ? "0" : "") + String(minutes);
    display.clearDisplay();
    display_timer(timer_str);
    display_selector();
    display.display();
  }
}

void color_selection_service() {
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
}

void start_cancel_pressed() {
  
}

void start_countdown() {
  if (hours > 0 || minutes > 0) {
    ColorButton::disable_all();
    choice_encoder.disable();
    increment_encoder.disable();
    periodic_ticker.attach(1, countdown);
  }
}

void finish_countdown() {
  periodic_ticker.detach();
  ColorButton::enable_all();
  hours = 0;
  minutes = 0;
  seconds = 0;
  timer_str = (hours < 10 ? "0" : "") + String(hours) + ":" + (minutes < 10 ? "0" : "") + String(minutes);
  display_timer(timer_str);
  display_selector();
  display.display();
}

void countdown() {
  if (seconds > 0) seconds = seconds - 1;

  else if (minutes > 0) {
    minutes = minutes - 1;
    seconds = 59;
  }

  else if (hours > 0) {
    minutes = 59;
    seconds = 59;
    hours = hours - 1;
  }

  else {
    finish_countdown();
  }

  timer_str = (hours < 10 ? "0" : "") + String(hours) + ":" + (minutes < 10 ? "0" : "") + String(minutes) + (seconds < 10 ? "0" : "") + String(seconds);
  display_timer(timer_str);
  display.display();
}

