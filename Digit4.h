#if !defined(_DIGIT4_H)
#define _DIGIT4_H

#include <driver/gpio.h>
#include <tm1637.h>

class Digit4 : public Tm1637
{
public:
  static const size_t DIGIT_COUNT = 4;

  static const digit_data_t MASK_DECIMAL_POINT = (1 << 7);

  static const digit_data_t SEG_NULL = 0;

  static const digit_data_t SEG_0 = 0b0111111;
  static const digit_data_t SEG_1 = 0b0000110;
  static const digit_data_t SEG_2 = 0b1011011;
  static const digit_data_t SEG_3 = 0b1001111;
  static const digit_data_t SEG_4 = 0b1100110;
  static const digit_data_t SEG_5 = 0b1101101;
  static const digit_data_t SEG_6 = 0b1111101;
  static const digit_data_t SEG_7 = 0b0000111;
  static const digit_data_t SEG_8 = 0b1111111;
  static const digit_data_t SEG_9 = 0b1101111;

  static const digit_data_t SEG_A = 0b1110111;
  static const digit_data_t SEG_B = 0b1111100;
  static const digit_data_t SEG_C = 0b0111001;
  static const digit_data_t SEG_D = 0b1011110;
  static const digit_data_t SEG_E = 0b1111001;
  static const digit_data_t SEG_F = 0b1110001;
  static const digit_data_t SEG_G = 0b0111101;
  static const digit_data_t SEG_H = 0b1110110;
  static const digit_data_t SEG_I = 0b0110000;
  static const digit_data_t SEG_J = 0b0011110;
  static const digit_data_t SEG_K = SEG_H;
  static const digit_data_t SEG_L = 0b0111000;
  static const digit_data_t SEG_M = SEG_NULL;
  static const digit_data_t SEG_N = 0b1010100;
  static const digit_data_t SEG_O = 0b1011100;
  static const digit_data_t SEG_P = 0b1110001;
  static const digit_data_t SEG_Q = SEG_O | MASK_DECIMAL_POINT;
  static const digit_data_t SEG_R = 0b1010000;
  static const digit_data_t SEG_S = SEG_5;
  static const digit_data_t SEG_T = 0b1111000;
  static const digit_data_t SEG_U = 0b0111110;
  static const digit_data_t SEG_V = SEG_U;
  static const digit_data_t SEG_X = SEG_NULL;
  static const digit_data_t SEG_Y = 0b1101110;
  static const digit_data_t SEG_Z = SEG_2;

  static const digit_data_t SEG_DASH = 0b1000000;
  static const digit_data_t SEG_LOWER_DASH = 0b0010000;

  static const int16_t MAX_INT = 9999;
  static const int16_t MIN_INT = -999;
  static constexpr float MAX_FLOAT = 9999.0f;
  static constexpr float MIN_FLOAT_POS = 0.001f;
  static constexpr float MAX_FLOAT_NEG = -0.001f;
  static constexpr float MIN_FLOAT = -999.0f;

  Digit4(gpio_num_t data_pin, gpio_num_t clock_pin);

  void drawInt(int16_t num, DisplayPwm pwm = Pwm14_16);
  void drawFloat(float num, DisplayPwm pwm = Pwm14_16);

  void draw(DisplayPwm pwm = Pwm14_16);
  void setInt(int16_t num, bool leading_zeroes = false);

  void getDigits(digit_data_t data[4]);

private:
  digit_data_t _digits[DIGIT_COUNT];
};

#endif // _DIGIT4_H
