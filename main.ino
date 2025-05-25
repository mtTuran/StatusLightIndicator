#include "AiEsp32RotaryEncoder.h"
#include "Color.h"
#include "Led.h"
#include "ColorButton.h"

void IRAM_ATTR readChoiceEncoderISR();
void IRAM_ATTR readIncrementEncoderISR();
void choice_encoder_loop();
void increment_encoder_loop();
void color_selection_service();

#define CHOICE_ENCODER_A_PIN 5
#define CHOICE_ENCODER_B_PIN 17
#define INCREMENT_ENCODER_A_PIN 16
#define INCREMENT_ENCODER_B_PIN 4
#define ROTARY_ENCODER_BUTTON_PIN -1
#define ROTARY_ENCODER_VCC_PIN -1
#define ROTARY_ENCODER_STEPS 4

long hours = 0;
long minutes = 0;
long incrementer_selection = 1;

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

//************************************** (main) setup - loop **************************************
void setup() {
  led.init_pins();
  led.off();

  red_bt.init_pin();
  orange_bt.init_pin();
  yellow_bt.init_pin();
  green_bt.init_pin();

  choice_encoder.begin();
	choice_encoder.setup(readChoiceEncoderISR);
	choice_encoder.setBoundaries(0, 1, false); // true means values form a circle
  choice_encoder.disableAcceleration();
  choice_encoder.setEncoderValue(incrementer_selection);

  increment_encoder.begin();
	increment_encoder.setup(readIncrementEncoderISR);
	increment_encoder.setBoundaries(0, 59, true);
  increment_encoder.disableAcceleration();
  increment_encoder.setEncoderValue(minutes);

  Serial.begin(115200);
}

void loop() {  
  color_selection_service();
  choice_encoder_loop();
  increment_encoder_loop();
  delay(10);  
}

//************************************** function definitions **************************************
void IRAM_ATTR readChoiceEncoderISR() {
	choice_encoder.readEncoder_ISR();
}

void IRAM_ATTR readIncrementEncoderISR() {
	increment_encoder.readEncoder_ISR();
}

void choice_encoder_loop() {
	if (choice_encoder.encoderChanged()) {
    incrementer_selection = choice_encoder.readEncoder();
		Serial.print("Choice Value: ");
		Serial.println(incrementer_selection);
    if (incrementer_selection == 1) {
      increment_encoder.setBoundaries(0, 59, true);
      increment_encoder.setEncoderValue(minutes);
    }
    else {
      increment_encoder.setBoundaries(0, 120, true);
      increment_encoder.setEncoderValue(hours);
    }
	}
}

void increment_encoder_loop() {
  if (increment_encoder.encoderChanged()) {
    if (incrementer_selection == 1) minutes = increment_encoder.readEncoder();
    else hours = increment_encoder.readEncoder();
    Serial.print("Hour - Minutes: ");
    Serial.print(hours);
    Serial.print(" - ");
    Serial.println(minutes);
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
