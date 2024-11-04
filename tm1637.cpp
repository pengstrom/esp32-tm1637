#include <tm1637.h>
#include <driver/gpio.h>
#include <rom/ets_sys.h>
#include <portmacro.h>

#define DATA_HIGH gpio_set_level(_data_pin, 1)
#define DATA_LOW gpio_set_level(_data_pin, 0)

#define CLK_HIGH gpio_set_level(_clock_pin, 1)
#define CLK_LOW gpio_set_level(_clock_pin, 0)

const Tm1637::address_config_t Tm1637::DEFAULT_ADDRESS_CONFIG = {};
const Tm1637::display_config_t Tm1637::DEFAULT_DISPLAY_CONFIG = {};

Tm1637::Tm1637(gpio_num_t data_pin, gpio_num_t clock_pin, uint32_t delay)
{
  _data_pin = data_pin;
  _clock_pin = clock_pin;
  _delay = delay;
}

void Tm1637::begin()
{
  // GPIO_IS_VALID_OUTPUT_GPIO(_clock_pin);
  // GPIO_IS_VALID_OUTPUT_GPIO(_data_pin);

  resetDataPin(1);

  gpio_reset_pin(_clock_pin);
  gpio_pullup_dis(_clock_pin);
  gpio_set_direction(_clock_pin, GPIO_MODE_OUTPUT);
  gpio_set_level(_clock_pin, 1);
}

void Tm1637::end()
{
  gpio_reset_pin(_clock_pin);
  gpio_reset_pin(_data_pin);
}

void Tm1637::writeData(const digit_data_t *data, size_t len, address_config_t addr_cfg, display_config_t display_cfg)
{
  uint8_t data_command = DataCommand | DataWrite | addr_cfg.addr_mode;
  uint8_t addr_command = AddressCommand | addr_cfg.addr;
  uint8_t display_command = DisplayCommand | display_cfg.power | display_cfg.pwm;

  // Start
  sendDataCommand(DataWrite, addr_cfg.addr_mode);

  vPortYield();

  start();
  sendByte(addr_command);
  ack();
  for (size_t i = 0; i < len; i++)
  {
    sendByte(data[i]);
    ack();
  }
  stop();

  vPortYield();

  sendDisplayCommand(display_cfg);
}

void Tm1637::sendDisplayCommand(display_config_t display_cfg)
{
  uint8_t display_command = DisplayCommand | display_cfg.power | display_cfg.pwm;
  start();
  sendByte(display_command);
  ack();
  stop();
}

void Tm1637::sendDataCommand(DataRWFlag rw, DataAddressFlag addr_mode)
{
  uint8_t data_command = DataCommand | rw | addr_mode;

  start();
  sendByte(data_command);
  ack();
  stop();
}

void Tm1637::sendAddressCommand(digit_data_t *data, size_t len, DigitAddress addr, DataAddressFlag addr_mode)
{
  uint8_t addr_command = AddressCommand | addr;

  start();
  sendByte(addr_command);
  ack();
  for (size_t i = 0; i < len; i++)
  {
    sendByte(data[i]);
    ack();

    if (addr_mode == AddressFixed && i != len - 1)
    {
      stop();
      start();
      sendByte(addr_command);
      ack();
    }
  }
  stop();
}

void Tm1637::turnOn()
{
  sendDisplayCommand({.power = PowerOn});
}

void Tm1637::turnOff()
{
  sendDisplayCommand({.power = PowerOff});
}

// Assumes data = clk == H
void Tm1637::start()
{
  DATA_LOW;
  delay();
  CLK_LOW;
}

// Assumes clk = data = L
void Tm1637::stop()
{
  delay();
  CLK_HIGH;
  delay();
  DATA_HIGH;
  delay();
}

// Assumes clk = 0
bool Tm1637::ack()
{
  // gpio_reset_pin(_data_pin);
  gpio_set_direction(_data_pin, GPIO_MODE_INPUT);
  // gpio_set_drive_capability(_data_pin, GPIO_DRIVE_CAP_0);
  gpio_pullup_en(_data_pin);

  delay();

  int level = gpio_get_level(_data_pin);
  bool ack = level == 0;

  CLK_HIGH;
  delay();
  resetDataPin(0);
  CLK_LOW;

  return ack;
}

void Tm1637::sendByte(uint8_t byte)
{
  for (size_t i = 0; i < 8; i++)
  {
    bool lsb = (byte & 1) == 1;
    byte >>= 1;

    sendBit(lsb);
  }
}

void Tm1637::sendBit(bool lsb)
{
  delay();
  if (lsb)
  {
    DATA_HIGH;
  }
  else
  {
    DATA_LOW;
  };
  CLK_HIGH;
  delay();
  CLK_LOW;
}

void Tm1637::delay()
{
  ets_delay_us(_delay);
}

void Tm1637::resetDataPin(uint32_t level)
{
  // gpio_reset_pin(_data_pin);
  gpio_pullup_dis(_data_pin);
  gpio_set_direction(_data_pin, GPIO_MODE_OUTPUT);
  gpio_set_drive_capability(_data_pin, GPIO_DRIVE_CAP_DEFAULT);
  gpio_set_level(_data_pin, level);
}
