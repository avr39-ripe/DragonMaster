#include <app.h>
#include <dragonmaster.h>

BinOutClass* outputs[3];
TempSensorsHttp* tempSensorsHttp;
WeekThermostatClass* weekThermostats[zonesCount];
// TempSensors* tempSensor;
// OneWire ds(onewire_pin);

AppClass App;

void init()
{
        App.init();
        App.start();
}
