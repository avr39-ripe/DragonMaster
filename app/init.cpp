#include <app.h>
#include <user_config.h>
#include <dragonmaster.h>

uint8_t state = HIGH;
OneWire ds(onewire_pin);
TempSensorsOW tempSensor(ds, 4000);
BinInGPIOClass inputGPIO(500);

#define I2C_LCD_ADDR 0x27
LiquidCrystal_I2C lcd(I2C_LCD_ADDR, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

AppClass App;

void init()
{
        App.init();
        App.start();
}
