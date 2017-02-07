'use strict';

//import wsBin from './wsBin';
//
//console.log(`wsBin.Const.wsPayLoadStart = ${wsBin.Const.wsPayLoadStart}`);

'use strict';




//var binStates;
//var tempsensors;
//var tempsensorsHome;

//import websocket from './websocket';
import { initWS, websocket } from './websocket';

//Here we put some initial code which starts after DOM loaded
function onDocumentRedy() {
//	tempsensors = new TempsensorsClass('/temperature.json', 0);
//	tempsensors.enable(true);
//	setInterval(function () { tempsensors.wsGetAllTemperatures(); }, 5000);
//	
//	tempsensorsHome = new TempsensorsClass('/temperatureHome.json', 1);
//	tempsensorsHome.enable(true);
//	setInterval(function () { tempsensorsHome.wsGetAllTemperatures(); }, 5000);

	initWS();
}

document.addEventListener('DOMContentLoaded', onDocumentRedy);