
#-------------------------------------------------------------------------------
# Maintainer:   jeffskinnerbox@yahoo.com / www.jeffskinnerbox.me
# Version:      0.1.0
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
#-------------------------------------------------------------------------------

# name of program being created
PROG = sprites

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

# location of the esptool used for flashing
ESPTOOL = /home/jeff/.arduino15/packages/esp32/tools/esptool_py/2.6.1/esptool.py

# string within names give to .bin and .elf files
VAR = $(shell echo $(FQBN) | tr ':' '.')

# path for temp-storage of binary, object, and core.a files
BUILD = /tmp/$(PROG)
BUILD_PATH = $(BUILD)/build

# compiler and compiler flags
CC = arduino-cli compile
CC_FLAGS = $(VERBOSE) --fqbn $(FQBN) --build-cache-path=$(BUILD) --build-path=$(BUILD_PATH)

# firmware flasher and flags
UPLOAD = arduino-cli upload
UPLOAD_FLAGS = $(VERBOSE) --fqbn $(FQBN) --port $(PORT)


.PHONY: build upload clean erase size

all: build upload

build: 											# build the binary executable
	$(CC) $(CC_FLAGS) $(PWD)

upload: 										# up load the binary executable
	$(UPLOAD) $(UPLOAD_FLAGS) $(PWD)

#all: build upload

#build: $(PROG).$(VAR).bin

#$(PROG).$(VAR).bin: $(PROG).ino                 # build the binary executable
	#$(CC) $(CC_FLAGS)

#upload: $(PROG).$(VAR).bin                      # up load the binary executable
	#$(UPLOAD) $(UPLOAD_FLAGS)

erase:                                          # erase the entire flash
	$(ESPTOOL) erase_flash --port $(PORT)

size:                                           # determine the flash size
	$(ESPTOOL) flash_id --port $(PORT)

clean:                                          # delete all binaries and object files
	rm -r --force $(BUILD)
	rm --force *.bin *.elf *.hex