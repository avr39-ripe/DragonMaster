/******/ (function(modules) { // webpackBootstrap
/******/ 	// The module cache
/******/ 	var installedModules = {};

/******/ 	// The require function
/******/ 	function __webpack_require__(moduleId) {

/******/ 		// Check if module is in cache
/******/ 		if(installedModules[moduleId])
/******/ 			return installedModules[moduleId].exports;

/******/ 		// Create a new module (and put it into the cache)
/******/ 		var module = installedModules[moduleId] = {
/******/ 			i: moduleId,
/******/ 			l: false,
/******/ 			exports: {}
/******/ 		};

/******/ 		// Execute the module function
/******/ 		modules[moduleId].call(module.exports, module, module.exports, __webpack_require__);

/******/ 		// Flag the module as loaded
/******/ 		module.l = true;

/******/ 		// Return the exports of the module
/******/ 		return module.exports;
/******/ 	}


/******/ 	// expose the modules object (__webpack_modules__)
/******/ 	__webpack_require__.m = modules;

/******/ 	// expose the module cache
/******/ 	__webpack_require__.c = installedModules;

/******/ 	// identity function for calling harmony imports with the correct context
/******/ 	__webpack_require__.i = function(value) { return value; };

/******/ 	// define getter function for harmony exports
/******/ 	__webpack_require__.d = function(exports, name, getter) {
/******/ 		if(!__webpack_require__.o(exports, name)) {
/******/ 			Object.defineProperty(exports, name, {
/******/ 				configurable: false,
/******/ 				enumerable: true,
/******/ 				get: getter
/******/ 			});
/******/ 		}
/******/ 	};

/******/ 	// getDefaultExport function for compatibility with non-harmony modules
/******/ 	__webpack_require__.n = function(module) {
/******/ 		var getter = module && module.__esModule ?
/******/ 			function getDefault() { return module['default']; } :
/******/ 			function getModuleExports() { return module; };
/******/ 		__webpack_require__.d(getter, 'a', getter);
/******/ 		return getter;
/******/ 	};

/******/ 	// Object.prototype.hasOwnProperty.call
/******/ 	__webpack_require__.o = function(object, property) { return Object.prototype.hasOwnProperty.call(object, property); };

/******/ 	// __webpack_public_path__
/******/ 	__webpack_require__.p = "";

/******/ 	// Load entry module and return exports
/******/ 	return __webpack_require__(__webpack_require__.s = 3);
/******/ })
/************************************************************************/
/******/ ([
/* 0 */
/***/ (function(module, __webpack_exports__, __webpack_require__) {

"use strict";
/* harmony import */ var __WEBPACK_IMPORTED_MODULE_0__wsBin__ = __webpack_require__(1);
/* harmony import */ var __WEBPACK_IMPORTED_MODULE_1__appStatus_js__ = __webpack_require__(2);
/* harmony export (binding) */ __webpack_require__.d(__webpack_exports__, "a", function() { return initWS; });
/* harmony export (binding) */ __webpack_require__.d(__webpack_exports__, "b", function() { return websocket; });

//Websockets
var websocket;
var appStatus;




function onOpen(evt) {
//	console.log.bind(console)("CONNECTED");
	
	appStatus = new __WEBPACK_IMPORTED_MODULE_1__appStatus_js__["a" /* default */]();
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
    	
    	var cmd = bin.getUint8(__WEBPACK_IMPORTED_MODULE_0__wsBin__["a" /* default */].Const.wsCmd);
    	var sysId = bin.getUint8(__WEBPACK_IMPORTED_MODULE_0__wsBin__["a" /* default */].Const.wsSysId);
    	var subCmd = bin.getUint8(__WEBPACK_IMPORTED_MODULE_0__wsBin__["a" /* default */].Const.wsSubCmd);
//    	console.log.bind(console)(`cmd = ${cmd}, sysId = ${sysId}, subCmd = ${subCmd}`);
    	
    	if ( cmd == __WEBPACK_IMPORTED_MODULE_0__wsBin__["a" /* default */].Const.getResponse && sysId == 1 ) {
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
	var wsUri = "ws://" + "10.2.113.118" + "/";
	websocket = new WebSocket(wsUri);
	websocket.onopen = function(evt) { onOpen(evt) };
	websocket.onclose = function(evt) { onClose(evt) };
	websocket.onmessage = function(evt) { onMessage(evt) };
	websocket.onerror = function(evt) { onError(evt) };
	websocket.binaryType = 'arraybuffer';
}



/***/ }),
/* 1 */
/***/ (function(module, __webpack_exports__, __webpack_require__) {

"use strict";

//wsBinProtocol constants
var wsBin = {
	Const :{
//Frame header offsets
	wsCmd			: 0, //Command type
	wsSysId			: 1, //target sysId
	wsSubCmd		: 2, //sub-command type
	wsPayLoadStart	: 3,
	//alternatively if we need argument to Get value
	wsPayLoadStartGetSetArg	: 4,
	wsGetSetArg		: 3,

	reservedCmd		: 0,
	getCmd			: 1,
	setCmd			: 2,
	getResponse		: 3,
	setAck			: 4,
	setNack			: 5,

// sub-command
	scAppSetTime	: 1,
	scAppGetStatus	: 2,
	scAppConfigGet	: 3,
	scAppConfigSet	: 4,
// sub-commands for BinStateHttpClass sysId=2 and BinStatesHttpClass sysId=3
	scBinStateGetName	: 1,
	scBinStateGetState	: 2,
	scBinStateSetState	: 3,
// sub-commands for BinStatesHttpClass sysId=3
	scBinStatesGetAll		: 10,
	scBinStatesGetAllStates	: 20,
	scBinStatesGetAllButtons	: 30,
// BinHttp State/Buttons base numbers
	uidHttpState		: 0,
	uidHttpButton		: 127
	},
	
	Cmd: {
	Get	: function (socket, sysId, subCmd) {
		var ab = new ArrayBuffer(3);
		var bin = new DataView(ab);
		
		bin.setUint8(wsBin.Const.wsCmd, wsBin.Const.getCmd);
		bin.setUint8(wsBin.Const.wsSysId, sysId);
		bin.setUint8(wsBin.Const.wsSubCmd, subCmd);
	
		socket.send(bin.buffer);
//		console.log.bind(console)(`wsGet sysId = ${sysId}, subCmd = ${subCmd}`);
	},
	SetArg: function (socket, sysId, subCmd, setArg, setValue) {
		var ab = new ArrayBuffer(5);
		var bin = new DataView(ab);
		
		bin.setUint8(wsBin.Const.wsCmd, wsBin.Const.setCmd);
		bin.setUint8(wsBin.Const.wsSysId, sysId);
		bin.setUint8(wsBin.Const.wsSubCmd, subCmd);
		bin.setUint8(wsBin.Const.wsGetSetArg, setArg);
		bin.setUint8(wsBin.Const.wsPayLoadStartGetSetArg, setValue);
	
		socket.send(bin.buffer);
//		console.log.bind(console)(`wsGet sysId = ${sysId}, subCmd = ${subCmd}`);
	}
	}
};

/* harmony default export */ __webpack_exports__["a"] = wsBin;

/***/ }),
/* 2 */
/***/ (function(module, __webpack_exports__, __webpack_require__) {

"use strict";
/* harmony import */ var __WEBPACK_IMPORTED_MODULE_0__websocket__ = __webpack_require__(0);
/* harmony import */ var __WEBPACK_IMPORTED_MODULE_1__wsBin__ = __webpack_require__(1);
/* harmony export (immutable) */ __webpack_exports__["a"] = AppStatusClass;






function AppStatusClass() {
	this._counter = 0;
	this._timestamp = 0;
	this._dateStr = "";
	this._timer = 0;
	this._enable = false;
}

AppStatusClass.prototype.wsGetAppStatus = function() {
	__WEBPACK_IMPORTED_MODULE_1__wsBin__["a" /* default */].Cmd.Get(__WEBPACK_IMPORTED_MODULE_0__websocket__["b" /* websocket */], 1, __WEBPACK_IMPORTED_MODULE_1__wsBin__["a" /* default */].Const.scAppGetStatus);
}

AppStatusClass.prototype.wsBinProcess = function (bin) {
	var subCmd = bin.getUint8(__WEBPACK_IMPORTED_MODULE_1__wsBin__["a" /* default */].Const.wsSubCmd);
	if (subCmd == __WEBPACK_IMPORTED_MODULE_1__wsBin__["a" /* default */].Const.scAppGetStatus) {
		this._counter = bin.getUint32(__WEBPACK_IMPORTED_MODULE_1__wsBin__["a" /* default */].Const.wsPayLoadStart, true);
    	this._timestamp = bin.getUint32(__WEBPACK_IMPORTED_MODULE_1__wsBin__["a" /* default */].Const.wsPayLoadStart + 4, true);
		var d = new Date();
		d.setTime(this._timestamp * 1000);
		this._dateStr = d.toLocaleString().replace(/,\ /,'<br>');
		this.renderStatus();
	}
}

AppStatusClass.prototype.render = function () {
	var t = document.querySelector('#AppStatusClass');
	var clone = document.importNode(t.content, true);
	var container = document.getElementById("Container-AppStatusClass");
	container.appendChild(clone);
}

AppStatusClass.prototype.renderStatus = function () {
	document.querySelector('#AppStatusClassCounter').textContent = this._counter;
	document.querySelector('#AppStatusClassDateTime').innerHTML = this._dateStr;
}

AppStatusClass.prototype.remove = function () {
		var removeElement = document.querySelector('#Container-AppStatusClass');
		this.removeChilds(removeElement);
}

AppStatusClass.prototype.removeChilds = function (node) {
    var last;
    while (last = node.lastChild) node.removeChild(last);
}

AppStatusClass.prototype.enable = function( enable ) {
	if ( enable != this._enable ) {
		this._enable = enable;
		if (! this._enable) {
			clearInterval(this._timer);
			this.remove();
		} else {
			this.render();
			this.wsGetAppStatus()
			this._timer = setInterval(this.wsGetAppStatus, 5000);
		}	
	}
}

/***/ }),
/* 3 */
/***/ (function(module, __webpack_exports__, __webpack_require__) {

"use strict";
Object.defineProperty(__webpack_exports__, "__esModule", { value: true });
/* harmony import */ var __WEBPACK_IMPORTED_MODULE_0__websocket__ = __webpack_require__(0);


//import wsBin from './wsBin';
//
//console.log(`wsBin.Const.wsPayLoadStart = ${wsBin.Const.wsPayLoadStart}`);

'use strict';




//var binStates;
//var tempsensors;
//var tempsensorsHome;

//import websocket from './websocket';


//Here we put some initial code which starts after DOM loaded
function onDocumentRedy() {
//	tempsensors = new TempsensorsClass('/temperature.json', 0);
//	tempsensors.enable(true);
//	setInterval(function () { tempsensors.wsGetAllTemperatures(); }, 5000);
//	
//	tempsensorsHome = new TempsensorsClass('/temperatureHome.json', 1);
//	tempsensorsHome.enable(true);
//	setInterval(function () { tempsensorsHome.wsGetAllTemperatures(); }, 5000);

	__webpack_require__.i(__WEBPACK_IMPORTED_MODULE_0__websocket__["a" /* initWS */])();
}

document.addEventListener('DOMContentLoaded', onDocumentRedy);

/***/ })
/******/ ]);