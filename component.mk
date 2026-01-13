ARDUINO_LIBRARIES := ArduinoJson5

HWCONFIG := ota

CONFIG_VARS := RBOOT_TWO_ROMS 
RBOOT_TWO_ROMS := 1

COMPONENT_SRCDIRS = app lib/application lib/tempsensor lib/binio lib/thermostat
COMPONENT_INCDIRS = include lib/application lib/tempsensor lib/binio lib/thermostat lib/wsbinconst

ENABLE_CMD_EXECUTOR = 0

WEBPACK_OUT = files/index.*.js* files/thermostat.*.js*

.PHONY: webpacker
webpacker: 
	$(vecho) "Bundling js with Webpack"
	$(Q) rm -rf $(WEBPACK_OUT)
	$(Q) webpack
