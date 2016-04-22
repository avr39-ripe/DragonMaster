#include <app.h>
#include <user_config.h>
#include <dragonmaster.h>

NtpClient* ntpClient;
OneWire ds(onewire_pin);
#ifdef MCP23S17 //use MCP23S17
MCP* mcp001;
#endif
TempSensors* tempSensor;
BinInClass* input[2];
BinInPollerClass binInPoller;
BinOutClass* output[3];
ThermostatClass* thermostats[2];
FanClass* fan;

#define I2C_LCD_ADDR 0x27
LiquidCrystal_I2C lcd(I2C_LCD_ADDR, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

AppClass App;

void init()
{
        App.init();
        App.start();
}
