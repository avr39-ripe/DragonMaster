'use strict';

var appStatus
//var binStates;
//var tempsensors;
//var tempsensorsHome;

//import websocket from './websocket';
import AppStatusClass from './appStatus.js';
import BinStatesClass from './binStates.js'
import { initWS, websocket, wsObjects } from './websocket';

//Here we put some initial code which starts after DOM loaded
function onDocumentRedy() {
//	tempsensors = new TempsensorsClass('/temperature.json', 0);
//	tempsensors.enable(true);
//	setInterval(function () { tempsensors.wsGetAllTemperatures(); }, 5000);
//	
//	tempsensorsHome = new TempsensorsClass('/temperatureHome.json', 1);
//	tempsensorsHome.enable(true);
//	setInterval(function () { tempsensorsHome.wsGetAllTemperatures(); }, 5000);

	var appStatus = new AppStatusClass();
	var binStates = new BinStatesClass();
	
	wsObjects[AppStatusClass.sysId] = appStatus;
	wsObjects[BinStatesClass.sysId] = binStates;
	
	initWS();
}

document.addEventListener('DOMContentLoaded', onDocumentRedy);