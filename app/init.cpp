#include <app.h>
#include <user_config.h>
#include <dragonmaster.h>

NtpClient* ntpClient;
//OneWire ds(onewire_pin);

//TempSensors* tempSensor;
TempSensorsHttp *tempSensorsHttp;
WeekThermostatClass *weekThermostats[maxWeekThermostats];
BinOutClass* output;

AppClass App;

void init()
{
        App.init();
        App.start();
}
