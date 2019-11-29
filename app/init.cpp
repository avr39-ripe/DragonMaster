#include <app.h>
#include <dragonmaster.h>

BinOutClass* outputs[3];
TempSensorsHttp* tempSensorsHttp;
WeekThermostatClass* weekThermostat;
BinInPollerClass* binInPoller;
ThermostatClass* thermostats[3];
TempSensors* tempSensor;
FanClass* fan;
OneWire ds(onewire_pin);

const uint8_t i2cLCDAddr{0x27};
//const uint8_t i2cLCDAddr{0x3f}; // another version lcd
LiquidCrystal_I2C lcd(i2cLCDAddr, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

AppClass App;

void init()
{
        App.init();
        App.start();
}
