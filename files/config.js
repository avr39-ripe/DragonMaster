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

function get_fan_config() {
	getThermostatConfig('fan');
}

function postThermostatConfg(name,jsonData) {
	fetch('/thermostat.'+name, {
		method: 'post',
		headers: {
			'Accept': 'application/json',
			'Content-Type': 'application/json; charset=utf-8'
			},
		body: JSON.stringify(jsonData)
	});
}

function post_fan_config(event) {
	event.preventDefault();
    var formData = {
            'targetTemp'	:	document.getElementById('targetTemp_fan').value * 100,
            'targetTempDelta'	:	document.getElementById('targetTempDelta_fan').value * 100
            };
    postThermostatConfg('fan', formData);
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
			'loopInterval'			:	document.getElementById('loopInterval').value
			};
	post_cfg(formData);
}

//Here we put some initial code which starts after DOM loaded
function onDocumentRedy() {
    //Init
    get_config();
    get_fan_config();
    
    document.getElementById('form_netcfg').addEventListener('submit', post_netcfg);
    document.getElementById('netcfg_cancel').addEventListener('click', get_config);
    document.getElementById('form_settings').addEventListener('submit', post_config);
	document.getElementById('settings_cancel').addEventListener('click', get_config);
	document.getElementById('form_thermostat_fan').addEventListener('submit', post_fan_config);
	document.getElementById('thermostat_fan_cancel').addEventListener('click', get_fan_config);

}

document.addEventListener('DOMContentLoaded', onDocumentRedy);
