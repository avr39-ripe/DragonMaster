'use strict';

//import wsBin from './wsBin';
//
//console.log(`wsBin.Const.wsPayLoadStart = ${wsBin.Const.wsPayLoadStart}`);

'use strict';



var appStatus;
//var binStates;
//var tempsensors;
//var tempsensorsHome;

//Websockets
var websocket;

import wsBin from './wsBin';
import AppStatusClass from './appStatus.js';

function onOpen(evt) {
//	console.log.bind(console)("CONNECTED");
	
	appStatus = new AppStatusClass(websocket);
	appStatus.enable(true);
	
//	binStates = new BinStatesClass();
//	binStates.enableButtons(true);
//	binStates.enableStates(true);
//	setTimeout(function() { binStates.enableButtons(true); }, 500);
//	setTimeout(function() { binStates.enableStates(true); }, 850);
}

function onMessage(evt) {
//	console.log.bind(console)("Message recv: " + evt.data);
	if(evt.data instanceof ArrayBuffer) {
    	var bin = new DataView(evt.data);
    	
    	var cmd = bin.getUint8(wsBin.Const.wsCmd);
    	var sysId = bin.getUint8(wsBin.Const.wsSysId);
    	var subCmd = bin.getUint8(wsBin.Const.wsSubCmd);
//    	console.log.bind(console)(`cmd = ${cmd}, sysId = ${sysId}, subCmd = ${subCmd}`);
    	
    	if ( cmd == wsBin.Const.getResponse && sysId == 1 ) {
    		appStatus.wsBinProcess(bin);
    	}
    	
//    	if ( cmd == wsBinConst.getResponse && ( sysId == 2 || sysId == 3) ) {
//    		binStates.wsBinProcess(bin);
//    	}
    		
  	} 
}

function onClose(evt) {
//	console.log.bind(console)("DISCONNECTED");
}

function onError(evt) {
//	console.log.bind(console)("ERROR: " + evt.data);
}

function initWS() {
	var wsUri = "ws://" + "192.168.31.133" + "/";
	websocket = new WebSocket(wsUri);
	websocket.onopen = function(evt) { onOpen(evt) };
	websocket.onclose = function(evt) { onClose(evt) };
	websocket.onmessage = function(evt) { onMessage(evt) };
	websocket.onerror = function(evt) { onError(evt) };
	websocket.binaryType = 'arraybuffer';
}


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