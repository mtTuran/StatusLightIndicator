struct Color{
  byte red_scale;
  byte green_scale;
  byte blue_scale;
};

class Led {
  private:
    byte red_pin;
    byte green_pin;
    byte blue_pin;
    Color curr_color;

    void write_led() {
      analogWrite(this->red_pin, this->curr_color.red_scale);
      analogWrite(this->green_pin, this->curr_color.green_scale);
      analogWrite(this->blue_pin, this->curr_color.blue_scale);
    }

  public:
    Led(byte red_pin, byte green_pin, byte blue_pin) {
      this->red_pin = red_pin;
      this->green_pin = green_pin;
      this->blue_pin = blue_pin;
      Color curr_color = {0, 0, 0};
    }

    void init_pins() {
      pinMode(this->red_pin, OUTPUT);
      pinMode(this->green_pin, OUTPUT);
      pinMode(this->blue_pin, OUTPUT);
    }

    void off() {
      this->curr_color.red_scale = 0;
      this->curr_color.green_scale = 0;
      this->curr_color.blue_scale = 0;
      write_led();
    }

    void switch_colors(Color color) {
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
};

class ColorButton {
  private:
    byte pin;
    Color color;
  public:
    ColorButton(byte pin, Color color) {
      this->pin = pin;
      this->color = color;
    }
    
    void init_pin() {
      pinMode(this->pin, INPUT_PULLUP);
    }

    Color pressed() {
      return this->color;
    }
};

const Color RED = {255, 0, 0};
const Color ORANGE = {230, 92, 0};
const Color YELLOW = {255, 255, 0};
const Color GREEN = {0, 255, 0};

Led led(1, 2, 3);
ColorButton bt(5, RED);

void setup() {
  
}

void loop() {

}
