ARDUINO_LIBRARIES := OneWire LiquidCrystal ArduinoJson5

## use rboot build mode
RBOOT_ENABLED := 1

## Use standard hardware config with two ROM slots and two SPIFFS partitions
HWCONFIG := basic_rboot

RBOOT_RTC_ENABLED = 1

SPIFF_SIZE      ?= 524288

COMPONENT_SRCDIRS = app lib/application lib/tempsensor lib/binio lib/thermostat lib/fan
COMPONENT_INCDIRS = include lib/application lib/tempsensor lib/binio lib/thermostat lib/fan lib/wsbinconst

ENABLE_CMD_EXECUTOR = 0

WEBPACK_OUT = files/index.*.js* files/thermostat.*.js*

.PHONY: webpacker
webpacker: 
	$(vecho) "Bundling js with Webpack"
	$(Q) rm -rf $(WEBPACK_OUT)
	$(Q) webpack
