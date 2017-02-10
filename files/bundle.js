/******/ (function(modules) { // webpackBootstrap
/******/ 	// The module cache
/******/ 	var installedModules = {};
/******/
/******/ 	// The require function
/******/ 	function __webpack_require__(moduleId) {
/******/
/******/ 		// Check if module is in cache
/******/ 		if(installedModules[moduleId])
/******/ 			return installedModules[moduleId].exports;
/******/
/******/ 		// Create a new module (and put it into the cache)
/******/ 		var module = installedModules[moduleId] = {
/******/ 			i: moduleId,
/******/ 			l: false,
/******/ 			exports: {}
/******/ 		};
/******/
/******/ 		// Execute the module function
/******/ 		modules[moduleId].call(module.exports, module, module.exports, __webpack_require__);
/******/
/******/ 		// Flag the module as loaded
/******/ 		module.l = true;
/******/
/******/ 		// Return the exports of the module
/******/ 		return module.exports;
/******/ 	}
/******/
/******/
/******/ 	// expose the modules object (__webpack_modules__)
/******/ 	__webpack_require__.m = modules;
/******/
/******/ 	// expose the module cache
/******/ 	__webpack_require__.c = installedModules;
/******/
/******/ 	// identity function for calling harmony imports with the correct context
/******/ 	__webpack_require__.i = function(value) { return value; };
/******/
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
/******/
/******/ 	// getDefaultExport function for compatibility with non-harmony modules
/******/ 	__webpack_require__.n = function(module) {
/******/ 		var getter = module && module.__esModule ?
/******/ 			function getDefault() { return module['default']; } :
/******/ 			function getModuleExports() { return module; };
/******/ 		__webpack_require__.d(getter, 'a', getter);
/******/ 		return getter;
/******/ 	};
/******/
/******/ 	// Object.prototype.hasOwnProperty.call
/******/ 	__webpack_require__.o = function(object, property) { return Object.prototype.hasOwnProperty.call(object, property); };
/******/
/******/ 	// __webpack_public_path__
/******/ 	__webpack_require__.p = "";
/******/
/******/ 	// Load entry module and return exports
/******/ 	return __webpack_require__(__webpack_require__.s = 4);
/******/ })
/************************************************************************/
/******/ ([
/* 0 */
/***/ (function(module, __webpack_exports__, __webpack_require__) {

"use strict";
/* harmony import */ var __WEBPACK_IMPORTED_MODULE_0__wsBin__ = __webpack_require__(1);
/* harmony export (binding) */ __webpack_require__.d(__webpack_exports__, "c", function() { return initWS; });
/* harmony export (binding) */ __webpack_require__.d(__webpack_exports__, "d", function() { return websocket; });
/* harmony export (binding) */ __webpack_require__.d(__webpack_exports__, "a", function() { return wsEnablers; });
/* harmony export (binding) */ __webpack_require__.d(__webpack_exports__, "b", function() { return wsBinProcessors; });

//Websockets
var websocket;
var wsEnablers = [];
var wsBinProcessors = {};



function onOpen(evt) {
//	console.log.bind(console)("CONNECTED");
	
	Object.keys(wsEnablers).forEach( function(Id) { 
		wsEnablers[Id](true);
	});
	
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
    	
//    	if ( cmd == wsBin.Const.getResponse && sysId == 1 ) {
//    		appStatus.wsBinProcess(bin);
//    	}

    	wsBinProcessors[sysId](bin);
    	
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

AppStatusClass.sysId = 1;

AppStatusClass.prototype.wsGetAppStatus = function() {
	__WEBPACK_IMPORTED_MODULE_1__wsBin__["a" /* default */].Cmd.Get(__WEBPACK_IMPORTED_MODULE_0__websocket__["d" /* websocket */], 1, __WEBPACK_IMPORTED_MODULE_1__wsBin__["a" /* default */].Const.scAppGetStatus);
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
/* harmony import */ var __WEBPACK_IMPORTED_MODULE_0__websocket__ = __webpack_require__(0);
/* harmony import */ var __WEBPACK_IMPORTED_MODULE_1__wsBin__ = __webpack_require__(1);
/* harmony export (immutable) */ __webpack_exports__["a"] = BinStatesClass;





//BinStateHttpClass

function BinStateClass (uid) {
	this.uid = uid;
	this._state = 0; //false
	this._name = "";
	this.render();
}

BinStateClass.sysId = 2;

BinStateClass.prototype.wsGet = function (cmd) {
	var ab = new ArrayBuffer(3);
	var bin = new DataView(ab);
	
	bin.setUint8(__WEBPACK_IMPORTED_MODULE_1__wsBin__["a" /* default */].Const.wsCmd, __WEBPACK_IMPORTED_MODULE_1__wsBin__["a" /* default */].Const.getCmd);
	bin.setUint8(__WEBPACK_IMPORTED_MODULE_1__wsBin__["a" /* default */].Const.wsSysId, BinStateClass.sysId); //BinStateHttpClass.sysId = 2
	bin.setUint8(__WEBPACK_IMPORTED_MODULE_1__wsBin__["a" /* default */].Const.wsSubCmd, cmd);

	__WEBPACK_IMPORTED_MODULE_0__websocket__["d" /* websocket */].send(bin.buffer);
//	console.log.bind(console)(`wsGet cmd = ${cmd}`);
}

BinStateClass.prototype.wsGetName = function () {
	this.wsGet(1);
}

BinStateClass.prototype.wsGetState = function () {
	this.wsGet(2);
}

BinStateClass.prototype.wsSetState = function (state) {
	__WEBPACK_IMPORTED_MODULE_1__wsBin__["a" /* default */].Cmd.SetArg(__WEBPACK_IMPORTED_MODULE_0__websocket__["d" /* websocket */], BinStateClass.sysId, __WEBPACK_IMPORTED_MODULE_1__wsBin__["a" /* default */].Const.scBinStateSetState, this.uid, state);
}

BinStateClass.prototype.wsGotName = function (bin) {
	var strBuffer = new Uint8Array(bin.byteLength -(__WEBPACK_IMPORTED_MODULE_1__wsBin__["a" /* default */].Const.wsPayLoadStart + 1));
	console.log.bind(console)(`uid = ${this.uid}, bin.byteLength = ${bin.byteLength}`);

    for (var i = 0; i < strBuffer.length; i++) {
        strBuffer[i] = bin.getUint8(__WEBPACK_IMPORTED_MODULE_1__wsBin__["a" /* default */].Const.wsPayLoadStart + 1 + i);
//        console.log.bind(console)(`uid = ${this.uid}, strBuffer[${i}] = ${bin.getUint8(wsBin.Const.wsPayLoadStart + 1 + i)}`);
    }

    this._name = new TextDecoder().decode(strBuffer)
    this.renderName();
}

BinStateClass.prototype.wsGotState = function (bin) {
	this._state = bin.getUint8(__WEBPACK_IMPORTED_MODULE_1__wsBin__["a" /* default */].Const.wsPayLoadStart + 1, true);
	this.renderState();
}

BinStateClass.prototype.render = function () {
	if ( this.isState() ) {
		var t = document.querySelector('#BinStateHttpClass');
		var clone = document.importNode(t.content, true);
//		clone.querySelector('#binState').textContent = this._name;
		clone.querySelector('#binStateDiv').id = `binStateDiv${this.uid}`;
		clone.querySelector('#binStatePanel').id = `binStatePanel${this.uid}`;
		clone.querySelector('#binState').id = `binState${this.uid}`
		var container = document.getElementById("Container-BinStateHttpClassStates");
	}
			
	if ( this.isButton() ) {
		var t = document.querySelector('#BinStateHttpClassButton');
		var clone = document.importNode(t.content, true);
//		clone.querySelector('#binStateButton').textContent = this._name;
		clone.querySelector('#binStateButtonDiv').id = `binStateButtonDiv${this.uid}`
		clone.querySelector('#binStateButton').addEventListener('mousedown', this);
		clone.querySelector('#binStateButton').addEventListener('mouseup', this);
		clone.querySelector('#binStateButton').id = `binStateButton${this.uid}`
		var container = document.getElementById("Container-BinStateHttpClassButtons");
	}
	
	container.appendChild(clone);	
}

BinStateClass.prototype.renderName = function () {
	if ( this.isState() ) {
		document.querySelector(`#binState${this.uid}`).textContent = this._name;
	}
	
	if ( this.isButton() ) {
		document.querySelector(`#binStateButton${this.uid}`).textContent = this._name;
	}
}

BinStateClass.prototype.renderState = function () {
	if ( this.isState()) {
	    	var panel = document.querySelector(`#binStatePanel${this.uid}`);
	    	
	    	if (this._state) {
	    		panel.classList.remove("panel-primary");
	    		panel.classList.add("panel-danger");	
	    	} else {
	    		panel.classList.remove("panel-danger");
	    		panel.classList.add("panel-primary");
	    	}
    	}
    	
    	if ( this.isButton()) {
    		var panel = document.querySelector(`#binStateButton${this.uid}`);
	    	
	    	if (this._state) {
	    		panel.classList.remove("btn-primary");
	    		panel.classList.add("btn-warning");	
	    	} else {
	    		panel.classList.remove("btn-warning");
	    		panel.classList.add("btn-primary");
	    	}
    	}	
}

BinStateClass.prototype.remove = function () {
		var selector = this.isState() ?  `#binStateDiv${this.uid}` : `#binStateButtonDiv${this.uid}`

		var removeElement = document.querySelector(selector);
		this.removeChilds(removeElement);
		removeElement.remove();
}

BinStateClass.prototype.removeChilds = function (node) {
    var last;
    while (last = node.lastChild) node.removeChild(last);
}

BinStateClass.prototype.wsBinProcess = function (bin) {
	var subCmd = bin.getUint8(__WEBPACK_IMPORTED_MODULE_1__wsBin__["a" /* default */].Const.wsSubCmd);
	
	if ( subCmd == __WEBPACK_IMPORTED_MODULE_1__wsBin__["a" /* default */].Const.scBinStateGetName) {
		this.wsGotName(bin);
	}
	
	if ( subCmd == __WEBPACK_IMPORTED_MODULE_1__wsBin__["a" /* default */].Const.scBinStateGetState) {
		this.wsGotState(bin);
	}
	
}

BinStateClass.prototype.isButton = function () { return this.uid >= __WEBPACK_IMPORTED_MODULE_1__wsBin__["a" /* default */].Const.uidHttpButton; }
BinStateClass.prototype.isState = function () { return this.uid < __WEBPACK_IMPORTED_MODULE_1__wsBin__["a" /* default */].Const.uidHttpButton; }

BinStateClass.prototype.handleEvent = function(event) {
	switch(event.type) {
		case 'mousedown':
	        this.wsSetState(1);
	        break;
        case 'mouseup':
            this.wsSetState(0);
            break;
	}
}

//BinStatesHttpClass
// @param states - render/process states
// @param  buttons - render/process buttons

function BinStatesClass () {
	this._binStatesHttp = {};
	this._statesEnable = false;
	this._buttonsEnable = false;
	this._enable = false;
}

BinStatesClass.sysId = 2;

BinStatesClass.prototype.enableStates = function( statesEnable ) {
	if ( statesEnable != this._statesEnable ) {
		this._statesEnable = statesEnable;
		if (! this._statesEnable) {
			var self = this
			Object.keys(this._binStatesHttp).forEach(function(uid) {
				if ( self.isState(uid) ) {
					self._binStatesHttp[uid].remove();
					delete self._binStatesHttp[uid];
				}
			});
		} else {
			this.wsGetAllStates();
		}	
	}
}

BinStatesClass.prototype.enableButtons = function( buttonsEnable ) {
	if ( buttonsEnable != this._buttonsEnable ) {
		this._buttonsEnable = buttonsEnable;
		if (! this._buttonsEnable) {
			var self = this
			Object.keys(this._binStatesHttp).forEach(function(uid) {
				if ( self.isButton(uid) ) {
					self._binStatesHttp[uid].remove();
					delete self._binStatesHttp[uid];
				}
			});
		} else {
			this.wsGetAllButtons();
		}	
	}
}

BinStatesClass.prototype.enable = function( Enable ) {
	if ( Enable != this._enable ) {
		this._enable = Enable;
		this.enableStates(this._enable);
		this.enableButtons(this._enable);
	}
}
BinStatesClass.prototype.wsGetAll = function() {
	__WEBPACK_IMPORTED_MODULE_1__wsBin__["a" /* default */].Cmd.Get(__WEBPACK_IMPORTED_MODULE_0__websocket__["d" /* websocket */], BinStatesClass.sysId, __WEBPACK_IMPORTED_MODULE_1__wsBin__["a" /* default */].Const.scBinStatesGetAll);
}

BinStatesClass.prototype.wsGetAllStates = function() {
	__WEBPACK_IMPORTED_MODULE_1__wsBin__["a" /* default */].Cmd.Get(__WEBPACK_IMPORTED_MODULE_0__websocket__["d" /* websocket */], BinStatesClass.sysId, __WEBPACK_IMPORTED_MODULE_1__wsBin__["a" /* default */].Const.scBinStatesGetAllStates);
}

BinStatesClass.prototype.wsGetAllButtons = function() {
	__WEBPACK_IMPORTED_MODULE_1__wsBin__["a" /* default */].Cmd.Get(__WEBPACK_IMPORTED_MODULE_0__websocket__["d" /* websocket */], BinStatesClass.sysId, __WEBPACK_IMPORTED_MODULE_1__wsBin__["a" /* default */].Const.scBinStatesGetAllButtons);
}

BinStatesClass.prototype.wsBinProcess = function (bin) {
	var subCmd = bin.getUint8(__WEBPACK_IMPORTED_MODULE_1__wsBin__["a" /* default */].Const.wsSubCmd);
	var uid = bin.getUint8(__WEBPACK_IMPORTED_MODULE_1__wsBin__["a" /* default */].Const.wsPayLoadStart);
	
	if ( (this.isState(uid) && this._statesEnable) || (this.isButton(uid) && this._buttonsEnable ) ) {
		if (subCmd == __WEBPACK_IMPORTED_MODULE_1__wsBin__["a" /* default */].Const.scBinStateGetName) {
			if ( !this._binStatesHttp.hasOwnProperty(uid) ) {
				this._binStatesHttp[uid] = new BinStateClass(uid);
			}
			this._binStatesHttp[uid].wsGotName(bin);
		}
		
		if (subCmd == __WEBPACK_IMPORTED_MODULE_1__wsBin__["a" /* default */].Const.scBinStateGetState) {
			this._binStatesHttp[uid].wsGotState(bin);
		}
	}
	
	
}

BinStatesClass.prototype.isButton = function (uid) { return uid >= __WEBPACK_IMPORTED_MODULE_1__wsBin__["a" /* default */].Const.uidHttpButton; }
BinStatesClass.prototype.isState = function (uid) { return uid < __WEBPACK_IMPORTED_MODULE_1__wsBin__["a" /* default */].Const.uidHttpButton; }

/***/ }),
/* 4 */
/***/ (function(module, __webpack_exports__, __webpack_require__) {

"use strict";
Object.defineProperty(__webpack_exports__, "__esModule", { value: true });
/* harmony import */ var __WEBPACK_IMPORTED_MODULE_0__appStatus_js__ = __webpack_require__(2);
/* harmony import */ var __WEBPACK_IMPORTED_MODULE_1__binStates_js__ = __webpack_require__(3);
/* harmony import */ var __WEBPACK_IMPORTED_MODULE_2__websocket__ = __webpack_require__(0);


//var appStatus
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

	var appStatus = new __WEBPACK_IMPORTED_MODULE_0__appStatus_js__["a" /* default */]();
	var binStates = new __WEBPACK_IMPORTED_MODULE_1__binStates_js__["a" /* default */]();
	
	__WEBPACK_IMPORTED_MODULE_2__websocket__["a" /* wsEnablers */].push(appStatus.enable.bind(appStatus));
	__WEBPACK_IMPORTED_MODULE_2__websocket__["a" /* wsEnablers */].push(binStates.enableStates.bind(binStates));
//	wsEnablers.push(binStates.enableButtons.bind(binStates));
	
	__WEBPACK_IMPORTED_MODULE_2__websocket__["b" /* wsBinProcessors */][__WEBPACK_IMPORTED_MODULE_0__appStatus_js__["a" /* default */].sysId] = appStatus.wsBinProcess.bind(appStatus);
	__WEBPACK_IMPORTED_MODULE_2__websocket__["b" /* wsBinProcessors */][__WEBPACK_IMPORTED_MODULE_1__binStates_js__["a" /* default */].sysId] = binStates.wsBinProcess.bind(binStates);
	
	__webpack_require__.i(__WEBPACK_IMPORTED_MODULE_2__websocket__["c" /* initWS */])();
}

document.addEventListener('DOMContentLoaded', onDocumentRedy);

/***/ })
/******/ ]);
//# sourceMappingURL=bundle.js.map