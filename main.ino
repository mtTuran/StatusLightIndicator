#include "AiEsp32RotaryEncoder.h"
#include "Color.h"
#include "Led.h"
#include "ColorButton.h"

void IRAM_ATTR readEncoderISR();
void rotary_loop();
void color_selection_service();

#define ROTARY_ENCODER_A_PIN 5
#define ROTARY_ENCODER_B_PIN 17
#define ROTARY_ENCODER_BUTTON_PIN -1
#define ROTARY_ENCODER_VCC_PIN -1
#define ROTARY_ENCODER_STEPS 4

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
AiEsp32RotaryEncoder rotaryEncoder = AiEsp32RotaryEncoder(ROTARY_ENCODER_A_PIN, ROTARY_ENCODER_B_PIN, ROTARY_ENCODER_BUTTON_PIN, ROTARY_ENCODER_VCC_PIN, ROTARY_ENCODER_STEPS);

//************************************** (main) setup - loop **************************************
void setup() {
  led.init_pins();
  led.off();

  red_bt.init_pin();
  orange_bt.init_pin();
  yellow_bt.init_pin();
  green_bt.init_pin();

  rotaryEncoder.begin();
	rotaryEncoder.setup(readEncoderISR);
	rotaryEncoder.setBoundaries(0, 59, true); // true means values form a circle
  rotaryEncoder.disableAcceleration();

  Serial.begin(115200);
}

void loop() {  
  color_selection_service();
  rotary_loop();
  delay(10);  
}

//************************************** function definitions **************************************
void IRAM_ATTR readEncoderISR()
{
	rotaryEncoder.readEncoder_ISR();
}

void rotary_loop()
{
	//dont print anything unless value changed
	if (rotaryEncoder.encoderChanged())
	{
		Serial.print("Value: ");
		Serial.println(rotaryEncoder.readEncoder());
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
