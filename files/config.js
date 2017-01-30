function getFanConfig() {
    fetch('/fan')
  	.then(function(response) {
      if (response.status >= 200 && response.status < 300) return response.json();
	})
	.then(function(configJson) {
		Object.keys(configJson).forEach(function(key) {
			document.getElementById(key).value = configJson[key];
		});
		document.getElementById('periodicTempDelta').value /= 100;
	});
}


function postFanConfig(event) {
	event.preventDefault();
    var formData = {
            'startDuration'			:	document.getElementById('startDuration').value,
            'stopDuration'			:	document.getElementById('stopDuration').value,
            'periodicInterval'		:	document.getElementById('periodicInterval').value,
            'periodicDuration'		:	document.getElementById('periodicDuration').value,
            'periodicTempDelta'		:	document.getElementById('periodicTempDelta').value * 100,
            'checkerInterval'		:	document.getElementById('checkerInterval').value
            };
   	fetch('/fan', {
		method: 'post',
		headers: {
			'Accept': 'application/json',
			'Content-Type': 'application/json; charset=utf-8'
			},
		body: JSON.stringify(formData)
	}); 
}

function getThermostatConfig(name) {
    fetch('/thermostat.'+name)
  	.then(function(response) {
      if (response.status >= 200 && response.status < 300) return response.json();
	})
	.then(function(configJson) {
		Object.keys(configJson).forEach(function(key) {
			document.getElementById(key+'_'+name).value = configJson[key] / 100;
		});
	});
}

function postThermostatConfg(name) {
    var jsonData = {
            'targetTemp'		:	document.getElementById(`targetTemp_${name}`).value * 100,
            'targetTempDelta'	:	document.getElementById(`targetTempDelta_${name}`).value * 100
            };
    
	fetch('/thermostat.'+name, {
		method: 'post',
		headers: {
			'Accept': 'application/json',
			'Content-Type': 'application/json; charset=utf-8'
			},
		body: JSON.stringify(jsonData)
	});
}


function get_config() {
    fetch('/config.json')
  	.then(function(response) {
      if (response.status >= 200 && response.status < 300) return response.json();
	})
	.then(function(configJson) {
		Object.keys(configJson).forEach(function(key) {
			document.getElementById(key).value = configJson[key];
		});
		if (configJson.StaEnable == 1) {
			document.getElementById('StaEnable').checked = true;
		} else {
			document.getElementById('StaEnable').checked = false;
		}
	});
}

function post_cfg(jsonData) {
	fetch('/config', {
		method: 'post',
		headers: {
			'Accept': 'application/json',
			'Content-Type': 'application/json; charset=utf-8'
			},
		body: JSON.stringify(jsonData)
	});
}

function post_netcfg(event) {
	event.preventDefault();
    var formData = {
            'StaSSID'                    :   document.getElementById('StaSSID').value,
            'StaPassword'                :   document.getElementById('StaPassword').value,
            'StaEnable'                  :   (document.getElementById('StaEnable').checked ? 1 : 0)
            };
    post_cfg(formData);
}

function post_config(event) {
	event.preventDefault();
	var formData = {
			'loopInterval'			:	document.getElementById('loopInterval').value,
			'updateURL'				:	document.getElementById('updateURL').value
			};
	post_cfg(formData);
}

function post_fw(action) {
// action should be either "update" or "switch"
	var json = {};
	json[action] = 1;
	
	fetch('/update', {
		method: 'post',
		headers: {
			'Accept': 'application/json',
			'Content-Type': 'application/json; charset=utf-8'
			},
		body: JSON.stringify(json)
	});
}

//Websockets
var websocket;
function onOpen(evt) {
	console.log.bind(console)("CONNECTED");
//	appConfig = new AppConfigClass();
//	appConfig.enable(true);;
//	websocket.send("Sming love WebSockets");
}

function onClose(evt) {
	console.log.bind(console)("DISCONNECTED");
}

function onMessage(evt) {
	console.log.bind(console)("Message recv: " + evt.data);
	if(evt.data instanceof ArrayBuffer) {
		var bin = new DataView(evt.data);
		
		var cmd = bin.getUint8(wsBinConst.wsCmd);
		var sysId = bin.getUint8(wsBinConst.wsSysId);
		var subCmd = bin.getUint8(wsBinConst.wsSubCmd);
		console.log.bind(console)(`cmd = ${cmd}, sysId = ${sysId}, subCmd = ${subCmd}`);
		
//		if ( cmd == wsBinConst.getResponse && sysId == 1 ) {
//			appConfig.wsBinProcess(bin);
//		}	
	}
	// var json = JSON.parse(evt.data);
	// console.log.bind(console)("Json recv: " + json);
// 	
	// if (json.response == "getRandom") {
		// onGetRandom(json);
	// }
	//websocket.close();
}

function onError(evt) {
	console.log.bind(console)("ERROR: " + evt.data);
}

function initWS() {
	var wsUri = "ws://" + location.host + "/";
	websocket = new WebSocket(wsUri);
	websocket.onopen = function(evt) { onOpen(evt) };
	websocket.onclose = function(evt) { onClose(evt) };
	websocket.onmessage = function(evt) { onMessage(evt) };
	websocket.onerror = function(evt) { onError(evt) };
	websocket.binaryType = 'arraybuffer';
}

function closeWS() {
	websocket.close();
}

function sendTime(event) {
	event.preventDefault();
	var ab = new ArrayBuffer(8);
	var bin = new DataView(ab);
	var d = new Date();
	
	bin.setUint8(wsBinConst.wsCmd, wsBinConst.setCmd);
	bin.setUint8(wsBinConst.wsSysId, 1); //AppClass.sysId = 1
	bin.setUint8(wsBinConst.wsSubCmd, wsBinConst.scAppSetTime);
	
	bin.setUint32(wsBinConst.wsPayLoadStart,Math.round(d.getTime() / 1000),true);
	bin.setUint8(wsBinConst.wsPayLoadStart + 4, Math.abs(d.getTimezoneOffset()/60));	
	console.log.bind(console)(bin.getUint8(1),bin.getUint8(2),bin.getUint8(3),bin.getUint8(4));
	websocket.send(bin.buffer);
}

//Here we put some initial code which starts after DOM loaded
function onDocumentRedy() {
    //Init
	initWS();
	get_config();
    getThermostatConfig('fan');
    getThermostatConfig('pump');
    getThermostatConfig('pump_cooler');
    getThermostatConfig('pump_safety');
    getFanConfig();
    
    document.getElementById('form_netcfg').addEventListener('submit', post_netcfg);
    document.getElementById('netcfg_cancel').addEventListener('click', get_config);
    document.getElementById('form_settings').addEventListener('submit', post_config);
	document.getElementById('settings_cancel').addEventListener('click', get_config);
	document.getElementById('settings_update_fw').addEventListener('click', function() { post_fw("update"); });
	document.getElementById('settings_switch_fw').addEventListener('click', function() { post_fw("switch"); });
	document.getElementById('form_thermostat_fan').addEventListener('submit', function(event) { event.preventDefault(); postThermostatConfg('fan'); });
	document.getElementById('thermostat_fan_cancel').addEventListener('click', function () { getThermostatConfig('fan'); });
	document.getElementById('form_thermostat_pump').addEventListener('submit', function(event) { event.preventDefault(); postThermostatConfg('pump'); });
	document.getElementById('thermostat_pump_cancel').addEventListener('click', function () { getThermostatConfig('pump'); });
	document.getElementById('form_thermostat_pump_safety').addEventListener('submit', function(event) { event.preventDefault(); postThermostatConfg('pump_safety'); });
	document.getElementById('thermostat_pump_safety_cancel').addEventListener('click', function () { getThermostatConfig('pump_safety'); });
	document.getElementById('form_thermostat_pump_cooler').addEventListener('submit', function(event) { event.preventDefault(); postThermostatConfg('pump_cooler'); });
	document.getElementById('thermostat_pump_cooler_cancel').addEventListener('click', function () { getThermostatConfig('pump_cooler'); });	
	document.getElementById('form_fan').addEventListener('submit', postFanConfig);
	document.getElementById('fan_cancel').addEventListener('click', getFanConfig);
	
	document.getElementById('sync_datetime').addEventListener('click', sendTime);

}

document.addEventListener('DOMContentLoaded', onDocumentRedy);
