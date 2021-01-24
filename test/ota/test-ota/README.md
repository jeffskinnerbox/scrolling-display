
Important advice on sketch design...

* [ESP32 OTA tutorial with tricks (incl. OTA debugging](https://www.youtube.com/watch?v=1pwqS_NUG7Q)

**You'll need to install TelnetStream**
arduino-cli lib search telnetstream
arduino-cli lib install "TelnetStream"

**telnet address**
telnet 192.168.1.44
telnet test-ota.local

to terminnate telnet - `ctrl+]` then `quit`.

to terminnate screen - `ctrl+a` then `:quit`.


$ avahi-browse _arduino._tcp --resolve --parsable --terminate 2>/dev/null | grep -F "=;" | cut -d\; -f7-9 | uniq | tr ";" "\n"
test-ota.local
192.168.1.44
8266


python2 /home/jeff/.arduino15/packages/esp8266/hardware/esp8266/2.5.2/tools/espota.py -d  -i 192.168.1.44 -a 123 -f test-ota.esp8266.esp8266.nodemcuv2.bin


# get ip address, hostname, and ota port
OTAIP = $(avahi-browse _arduino._tcp --resolve --parsable --terminate 2>/dev/null | grep -F "=;" | cut -d\; -f8
| uniq)
OTAHOST = $(avahi-browse _arduino._tcp --resolve --parsable --terminate 2>/dev/null | grep -F "=;" | cut -d\; -f7
| uniq)
OTAPORT = $(avahi-browse _arduino._tcp --resolve --parsable --terminate 2>/dev/null | grep -F "=;" | cut -d\; -f9
| uniq)

# password for ota
PASSWORD = "123"

# location of the espota.py used for ota flashing
ESPOTATOOL =/home/jeff/arduino15/packages/esp8266/hardware/esp8266/2.5.2/tools/espota.py


$(ESPOTATOOL) -d -i $(OTAIP) -a $(PASSWORD) -f $(PROG).$(PACKAGE).$(ARCH).$(BOARD).bin
