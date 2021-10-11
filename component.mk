ARDUINO_LIBRARIES := OneWire ArduinoJson6

SPI_SIZE                ?= 4M

RBOOT_ENABLED           ?= 1

RBOOT_ROM0_ADDR         := 0x008000
RBOOT_ROM1_ADDR         := 0x108000

RBOOT_SPIFFS_0          := 0x200000
RBOOT_SPIFFS_1          := 0x300000

RBOOT_RTC_ENABLED       = 1
RBOOT_GPIO_SKIP_ENABLED ?= 1

SPIFF_SIZE              ?= 524288

COMPONENT_SRCDIRS = app lib/application lib/tempsensor lib/binio lib/thermostat lib/fan
COMPONENT_INCDIRS = include lib/application lib/tempsensor lib/binio lib/thermostat lib/fan lib/wsbinconst

ENABLE_CMD_EXECUTOR = 0

WEBPACK_OUT = files/index.*.js* files/thermostat.*.js*

.PHONY: webpacker
webpacker: 
	$(vecho) "Bundling js with Webpack"
	$(Q) rm -rf $(WEBPACK_OUT)
	$(Q) webpack