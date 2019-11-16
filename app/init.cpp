#include <app.h>
#include <dragonmaster.h>

BinOutClass* outputs[8];
TempSensorsHttp* tempSensorsHttp;
WeekThermostatClass* weekThermostats[zonesCount];


AppClass App;

void init()
{
        App.init();
        App.start();
}
