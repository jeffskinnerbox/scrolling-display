
#-------------------------------------------------------------------------------
#
# Maintainer:   jeffskinnerbox@yahoo.com / www.jeffskinnerbox.me
# Version:      0.4.0
#
# USAGE
#	To build the executable
#	    make build
#
#	To flash the device
#	    make upload
#
#	makefile syntax
#	    = set the varialbe
#	    ?= set the variable only if it's not set/doesn't have a value
#	    := set variable as the output an executed shell script
#	    += is used for appending more text to variables
#
# SOURCES
#   The Makefile was created with the help of this website:
#   https://learn.sparkfun.com/tutorials/efficient-arduino-programming-with-arduino-cli-and-visual-studio-code/all
#
# DOCUMENTATION
#   https://create.arduino.cc/projecthub/B45i/getting-started-with-arduino-cli-7652a5
#
#-------------------------------------------------------------------------------

# Use Bash shell instead of the default /bin/sh
SHELL := /bin/bash

# name of program being created
PROG = sprites

# type of package, architecture, and board in use
PACKAGE = esp8266
ARCH =    esp8266
BOARD =   nodemcuv2

# serial port used by the board
PORT = /dev/ttyUSB0

#------------------------- names, tools, flags, paths --------------------------

# optional verbose compile/upload flag
#VERBOSE = -v

# fully qualified board name (FQBN)
FQBN = $(PACKAGE):$(ARCH):$(BOARD)

# location of the esptool used for usb flashing
ESPTOOL = /home/jeff/.arduino15/packages/esp32/tools/esptool_py/4.2.1/esptool.py

# string within names given to .bin and .elf files
VAR := $(shell echo $(FQBN) | tr ':' '.')

# path for temp-storage of binary, object, and core.a files
BUILD = /tmp/$(PROG)
BUILD_PATH = $(BUILD)/build

# paths to libraries and include files
LIBS = "/home/jeff/src/arduino/sketchbooks/libraries"

#---------------------------------- compiler -----------------------------------

# compiler and compiler flags
CC = arduino-cli compile
CC_FLAGS = $(VERBOSE) --fqbn $(FQBN) --build-path=$(BUILD_PATH) --build-cache-path=$(BUILD) --libraries $(LIBS)

#--------------------------------- usb upload ----------------------------------

# usb firmware flasher and flags
UPLOAD_USB = arduino-cli upload
UPLOAD_USB_FLAGS = $(VERBOSE) --fqbn $(FQBN) --port $(PORT) --input-dir $(BUILD_PATH)

##--------------------------------- ota upload ----------------------------------
## name of program being created
#PROG = encoder

## type of package, architecture, and board in use
#PACKAGE = esp8266
#ARCH =    esp8266
#BOARD =   nodemcuv2

## serial port used by the board
#PORT = /dev/ttyUSB0

## optional verbose compile/upload flag
##VERBOSE = -v

##-------------------------------------------------------------------------------

## fully qualified board name (FQBN)
#FQBN = $(PACKAGE):$(ARCH):$(BOARD)

## location of the esptool used for flashing
#ESPTOOL = /home/jeff/.arduino15/packages/esp32/tools/esptool_py/2.6.1/esptool.py

## string within names given to .bin and .elf files
#VAR = $(shell echo $(FQBN) | tr ':' '.')

## path for temp-storage of binary, object, and core.a files
#BUILD = /tmp/$(PROG)
#BUILD_PATH = $(BUILD)/build

## paths to libraries and include files
#LIBS = "/home/jeff/src/arduino/sketchbooks/libraries"

## compiler and compiler flags
#CC = arduino-cli compile
#CC_FLAGS = $(VERBOSE) --fqbn $(FQBN) --build-path=$(BUILD_PATH) --build-cache-path=$(BUILD) --libraries $(LIBS)

## firmware flasher and flags
#UPLOAD = arduino-cli upload
#UPLOAD_FLAGS = $(VERBOSE) --fqbn $(FQBN) --port $(PORT)

#-------------------------------------------------------------------------------

.PHONY: build upload clean erase size

all: build upload

all-ota: build upload-ota                       # build and then upload via ota

help:
	@echo ' '
	@echo 'Makefile for scroll-display'
	@echo ' '
	@echo 'Usage:'
	@echo '   make              create all files and flash device via usb'
	@echo '   make all          create all files and flash device via usb'
	@echo '   make all-ota      create all files and flash device via ota'
	@echo '   make build        compile the code and create the *.elf file'
	@echo '   make upload       create the *.bin file and flash device via usb'
	@echo '   make upload-ota   create the *.bin file and flash device via ota'
	@echo '   make erase        erease the entire flash from the device and exit'
	@echo '   make size         print the flash size within the device and exit'
	@echo '   make clean        delete *.bin, *.elf, *.hex files'
	@echo '   make help         print this help message and exit'
	@echo ' '
	@echo 'Pass the option "--debug" to enable trace messaging (e.g. make --debug build)'
	@echo ' '


build: 											# build the binary executable
	$(CC) $(CC_FLAGS) $(CURDIR)

upload: 										# flash the binary executable
	$(UPLOAD_USB) $(UPLOAD_USB_FLAGS) $(CURDIR)

erase:                                          # erase the entire flash
	$(ESPTOOL) erase_flash --port $(PORT)

size:                                           # determine the flash size
	$(ESPTOOL) flash_id --port $(PORT)

clean:                                          # delete all binaries and object files
	rm -r --force $(BUILD)
	rm --force *.bin *.elf *.hex
