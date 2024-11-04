#if !defined(_TM1637_H)
#define _TM1637_H

#include <driver/gpio.h>

class Tm1637
{
public:
  typedef uint8_t digit_address_t;
  typedef uint8_t digit_data_t;

  enum CommandType
  {
    DataCommand = 0b01000000,
    DisplayCommand = 0b10000000,
    AddressCommand = 0b11000000
  };

  enum DataRWFlag
  {
    DataWrite = 0,
    DataReadKeyScan = 0b00000010
  };

  enum DataAddressFlag
  {
    AddressAuto = 0,
    AddressFixed = 0b00000100
  };

  enum DigitAddress
  {
    H00 = 0,
    H01,
    H02,
    H03,
    H04,
    H05
  };

  // E.g. Pwm12_16 -> 12/16 duty cycle
  enum DisplayPwm
  {
    Pwm1_16 = 0,
    Pwm2_16,
    Pwm4_16,
    Pwm10_18,
    Pwm11_16,
    Pwm12_16,
    Pwm13_16,
    Pwm14_16
  };

  enum DisplayPower
  {
    PowerOff = 0,
    PowerOn = 0b00001000
  };

  struct address_config_t
  {
    DataAddressFlag addr_mode = AddressAuto;
    DigitAddress addr = H00;
  };

  struct display_config_t
  {
    DisplayPwm pwm = Pwm14_16;
    DisplayPower power = PowerOn;
  };

  // Spec says max Hz is 250 kHz, do delay is minimum 4
  static const uint32_t DEFAULT_DELAY_US = 5;

  static const address_config_t DEFAULT_ADDRESS_CONFIG;
  static const display_config_t DEFAULT_DISPLAY_CONFIG;

  Tm1637(gpio_num_t data_pin, gpio_num_t clock_pin, uint32_t delay = DEFAULT_DELAY_US);

  void begin();
  void end();

  void writeData(const digit_data_t *data, size_t len, address_config_t addr_cfg = DEFAULT_ADDRESS_CONFIG, display_config_t display_cfg = DEFAULT_DISPLAY_CONFIG);

  void sendDisplayCommand(display_config_t display_cfg = DEFAULT_DISPLAY_CONFIG);
  void sendDataCommand(DataRWFlag rw = DataWrite, DataAddressFlag addr_mode = AddressAuto);
  void sendAddressCommand(digit_data_t *data, size_t len, DigitAddress addr, DataAddressFlag addr_mode = AddressAuto);

  void turnOn();
  void turnOff();

private:
  gpio_num_t _data_pin;
  gpio_num_t _clock_pin;
  uint32_t _delay;

  void start();
  void stop();
  bool ack();
  void sendByte(uint8_t byte);
  void sendBit(bool lsb);
  void delay();
  void resetDataPin(uint32_t level);
};

#endif // _TM1637_H