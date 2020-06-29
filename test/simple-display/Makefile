# programs name
PROG = simple-display

# fully qualified board name (FQBN)
FQBN = esp8266:esp8266:nodemcuv2

# serial port used by the board
PORT = /dev/ttyUSB0

ESPTOOL = /home/jeff/.arduino15/packages/esp32/tools/esptool_py/2.6.1/esptool.py

# string within names give to .bin and .elf files
VAR = $(shell echo $(FQBN) | tr ':' '.')

# compile and firmware flashing commands
CC = arduino-cli compile --fqbn $(FQBN)
UPLOAD = arduino-cli upload --fqbn $(FQBN) --port $(PORT)


.PHONY: build upload clean erase

build:                                          # build the binary executable
	$(CC) $(CURDIR)

upload:                                         # up load the binary executable
	$(UPLOAD) $(CURDIR)

erase:                                          # erase the entire flash
	$(ESPTOOL) --port $(PORT) erase_flash

size:                                           # determine the flash size
	$(ESPTOOL) --port $(PORT) flash_id

clean:                                          # delete binaries and object files
	rm --force $(PROG).$(VAR).bin $(PROG).$(VAR).elf
