#include "Digit4.h"
#include <tm1637.h>
#include <math.h>

#define THOUSANDS 1000
#define HUNDREDS 100
#define TENS 10
#define ONES 1

const int16_t powers10[Digit4::DIGIT_COUNT] = {THOUSANDS, HUNDREDS, TENS, ONES};

uint8_t fromDigit(int16_t d)
{
  switch (d)
  {
  case 0:
    return Digit4::SEG_0;
  case 1:
    return Digit4::SEG_1;
  case 2:
    return Digit4::SEG_2;
  case 3:
    return Digit4::SEG_3;
  case 4:
    return Digit4::SEG_4;
  case 5:
    return Digit4::SEG_5;
  case 6:
    return Digit4::SEG_6;
  case 7:
    return Digit4::SEG_7;
  case 8:
    return Digit4::SEG_8;
  case 9:
    return Digit4::SEG_9;
  }

  return Digit4::SEG_NULL;
}

Digit4::Digit4(gpio_num_t data_pin, gpio_num_t clock_pin) : Tm1637::Tm1637(data_pin, clock_pin)
{
}

void Digit4::drawInt(int16_t num, DisplayPwm pwm)
{
  setInt(num);
  draw(pwm);
}

void Digit4::setInt(int16_t num, bool leading_zeroes)
{
  {
    if (num > MAX_INT)
    {
      num = MAX_INT;
    }

    if (num < MIN_INT)
    {
      num = MIN_INT;
    }

    // Has any earlier digit been non-zero?
    bool nonzero = false;
    for (size_t i = 0; i < DIGIT_COUNT; i++)
    {
      if (i == 0 && num < 0)
      {
        // First digit is a minus for negative numbers
        _digits[0] = SEG_DASH;
        num *= -1;
        // Invariant already ensures num < 1000 at this point
        continue;
      }

      div_t qr = div(num, powers10[i]);
      if (!leading_zeroes && qr.quot == 0 && i != (DIGIT_COUNT - 1) && !nonzero)
      {
        // Only keep last zero
        _digits[i] = SEG_NULL;
        continue;
      }

      // Either this is true or it doesnt matter any more
      nonzero = true;
      num = qr.rem;
      _digits[i] = fromDigit(qr.quot);
    }
  }
}

void Digit4::getDigits(digit_data_t data[4])
{
  for (size_t i = 0; i < DIGIT_COUNT; i++)
  {
    data[i] = _digits[i];
  }
}

void Digit4::drawFloat(float num, DisplayPwm pwm)
{

  for (int i = 0; i < DIGIT_COUNT; i++)
  {
    int exponent = (DIGIT_COUNT - 1) - i; // 3 (2), 2 (1), 1 (0), 0 (-1)
    float power = exp10f(exponent);       // 1000. (100), 100.0 (10), 10.00 (1), 1.000 (0.1)
    float absnum = abs(num);
    if (absnum >= power || i == DIGIT_COUNT - 1)
    {
      if (num <= -1.0f)
      {
        // When we can't use decimal point at minus
        i -= 1;
        exponent += 1;
      }
      float shift = exp10f(i); // 1 (10), 10 (100), 100 (1000)
      setInt(round(shift * num), true);

      _digits[exponent] |= MASK_DECIMAL_POINT;

      return draw(pwm);
    }
  }
}

void Digit4::draw(DisplayPwm pwm)
{
  writeData(_digits, DIGIT_COUNT, {}, {.pwm = pwm});
}
