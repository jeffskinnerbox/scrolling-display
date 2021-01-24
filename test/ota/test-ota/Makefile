
#-------------------------------------------------------------------------------
#
# Maintainer:   jeffskinnerbox@yahoo.com / www.jeffskinnerbox.me
# Version:      0.5.0
#
# USAGE
#   To build the executable
#       make build
#
#   To flash the device
#       make upload
#       make upload-ota
#
#   To monitor execution of the device
#       screen /dev/ttyUSB0 9600,cs8cls
#       to terminate Ctrl-a :quit
#
#       telnet test-ota.local
#       to terminate Ctrl-a quit
#
#   makefile syntax
#       = set the variable
#       ?= set the variable only if it's not set/doesn't have a value
#       := set variable as the output an executed shell script
#       += is used for appending more text to variables
#
# SOURCES
#   The Makefile was created with the help of this website:
#   https://learn.sparkfun.com/tutorials/efficient-arduino-programming-with-arduino-cli-and-visual-studio-code/all
#
# DOCUMENTATION
#   https://create.arduino.cc/projecthub/B45i/getting-started-with-arduino-cli-7652a5
#
#-------------------------------------------------------------------------------

DEBUG ?= 0

# Use Bash shell instead of the default /bin/sh
SHELL := /bin/bash

# name of program being created
PROG = test-ota

# type of package, architecture, and board in use
PACKAGE = esp8266
ARCH =    esp8266
BOARD =   nodemcuv2

# serial port used by the board
PORT = /dev/ttyUSB0

# optional verbose compile/upload flag
#VERBOSE = -v

#-------------------------------------------------------------------------------

# fully qualified board name (FQBN)
FQBN = $(PACKAGE):$(ARCH):$(BOARD)

# location of the esptool used for usb flashing
ESPTOOL = /home/jeff/.arduino15/packages/esp32/tools/esptool_py/2.6.1/esptool.py

# location of the espota.py used for ota flashing
ESPOTATOOL = /home/jeff/.arduino15/packages/esp8266/hardware/esp8266/2.5.2/tools/espota.py

# string within names given to .bin and .elf files
VAR = $(shell echo $(FQBN) | tr ':' '.')

# path for temp-storage of binary, object, and core.a files
BUILD = /tmp/$(PROG)
BUILD_PATH = $(BUILD)/build

# paths to libraries and include files
LIBS = "/home/jeff/src/arduino/sketchbooks/libraries"

# compiler and compiler flags
CC = arduino-cli compile
CC_FLAGS = $(VERBOSE) --fqbn $(FQBN) --build-path=$(BUILD_PATH) --build-cache-path=$(BUILD) --libraries $(LIBS)

# set ota password, ip address, and port for device
OTAPASS = "123"
OTAIP = $(shell (avahi-browse _arduino._tcp --resolve --parsable --terminate 2>/dev/null | grep -F "=;" | grep $(PROG).local | cut -d\; -f8 | uniq))
OTAPORT = $(shell (avahi-browse _arduino._tcp --resolve --parsable --terminate 2>/dev/null | grep -F "=;" | grep $(PROG).local | cut -d\; -f9 | uniq))

# usb firmware flasher and flags 
UPLOAD_USB = arduino-cli upload
UPLOAD_USB_FLAGS = $(VERBOSE) --fqbn $(FQBN) --port $(PORT)

# ota firmware flasher and flags 
UPLOAD_OTA = python2 $(ESPOTATOOL) 
UPLOAD_OTA_FLAGS = -d -i $(OTAIP) -p $(OTAPORT) -a $(OTAPASS) -f $(PROG).$(PACKAGE).$(ARCH).$(BOARD).bin

DIR := ${CURDIR}

#-------------------------------------------------------------------------------

all: upload                                     # build and then upload via usb

all-usb: upload                                 # build and then upload via usb

all-ota: upload                                 # build and then upload via ota

help:
	@echo ' '
	@echo 'Makefile for OTA Test'
	@echo ' '
	@echo 'Usage:'
	@echo '   make              create all files and flash device via usb'
	@echo '   make all          create all files and flash device via usb'
	@echo '   make all-usb      create all files and flash device via usb'
	@echo '   make all-ota      create all files and flash device via ota'
	@echo '   make build        compile the code and create the *.elf file'
	@echo '   make upload       create the *.bin file and flash device via usb'
	@echo '   make upload-usb   create the *.bin file and flash device via usb'
	@echo '   make upload-ota   create the *.bin file and flash device via ota'
	@echo '   make erase        erease the entire flash from the device and exit'
	@echo '   make size         print the flash size within the device and exit'
	@echo '   make clean        delete *.bin, *.elf, *.hex files'
	@echo '   make help         print this help message and exit'
	@echo ' '
	@echo 'Set the DEBUG variable to 1 to enable debugging (e.g. make DEBUG=1)'
	@echo ' '

build:                                          # build the binary executable
	$(CC) $(CC_FLAGS) $(DIR)

upload: build                                   # flash the binary executable via usb
upload-usb: build
	$(UPLOAD_USB) $(UPLOAD_USB_FLAGS) $(DIR)

upload-ota: build                               # flash the binary executable via ota
	$(UPLOAD_OTA) $(UPLOAD_OTA_FLAGS)

erase:                                          # erase the entire flash
	$(ESPTOOL) erase_flash --port $(PORT)

size:                                           # determine the flash size
	$(ESPTOOL) flash_id --port $(PORT)

clean:                                          # delete all binaries and object files
	rm -r --force $(BUILD)
	rm --force *.bin *.elf *.hex

# explicitly declare these target to be phony
.PHONY: help build upload upload-usb upload-ota clean erase size


