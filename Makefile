#
# Using https://github.com/plerup/makeEspArduino
#
# makefile syntax
#     = set the varialbe
#    ?= set the variable only if it's not set/doesn't have a value
#    := set variable as the output an executed shell script
#    += is used for appending more text to variables
#

THIS_DIR := $(realpath $(dir $(realpath $(lastword $(MAKEFILE_LIST)))))
ROOT := $(THIS_DIR)/..

#ESP_ROOT = ~/.arduino15/packages/esp8266
#ESP_ROOT = ~/.arduino15/packages/esp8266/hardware/esp8266

#LIBS = $(ESP_LIBS)/SPI \
#	$(ESP_LIBS)/Wire \
#	$(ESP_LIBS)/ESP8266WiFi \
#	$(ESP_LIBS)/ESP8266WebServer \
#	$(ESP_LIBS)/DNSServer \
#LIBS += $(ARDUINO_LIBS)/PersWiFiManager
LIBS = $(ESP_LIBS)/SPI \
	$(ESP_LIBS)/ESP8266WiFi \
LIBS += $(ARDUINO_LIBS)/MD_MAX72XX \
	$(ARDUINO_LIBS)/MD_Parola

# directory with source files for SPIFFS filesystem
FS_DIR = ./data

CHIP = esp8266
BOARD = nodemcuv2
UPLOAD_PORT = /dev/ttyUSB0
UPLOAD_SPEED = 115200
FLASH_DEF=4M3M



include /home/jeff/src/makeEspArduino/makeEspArduino.mk
