# programs name
PROG = hello-world-1

# type of ESP chip and board in use
CHIP = esp8266
BOARD = nodemcuv2

# fully qualified board name (FQBN)
FQBN = $(CHIP):$(CHIP):$(BOARD)

# serial port used by the board
PORT = /dev/ttyUSB0

ESPTOOL = /home/jeff/.arduino15/packages/esp32/tools/esptool_py/2.6.1/esptool.py

# string within names give to .bin and .elf files
VAR = $(shell echo $(FQBN) | tr ':' '.')

# compile and firmware flashing commands
CC = arduino-cli compile --fqbn $(FQBN)
UPLOAD = arduino-cli upload --fqbn $(FQBN) --port $(PORT)


.PHONY: build upload clean erase size

all: build upload

build: $(PROG).$(VAR).bin

$(PROG).$(VAR).bin: $(PROG).ino
	$(CC) $(CURDIR)                             # build the binary executable

upload: $(PROG).$(VAR).bin                      # up load the binary executable
	$(UPLOAD) $(CURDIR)

erase:                                          # erase the entire flash
	$(ESPTOOL) --port $(PORT) erase_flash

size:                                           # determine the flash size
	$(ESPTOOL) --port $(PORT) flash_id

clean:                                          # delete binaries and object files
	rm --force $(PROG).$(VAR).bin $(PROG).$(VAR).elf
