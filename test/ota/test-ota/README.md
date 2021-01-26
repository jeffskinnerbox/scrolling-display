<!--
Maintainer:   jeffskinnerbox@yahoo.com / www.jeffskinnerbox.me
Version:      0.4.0
-->

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


```bash
$ avahi-browse _arduino._tcp --resolve --parsable --terminate 2>/dev/null | grep -F "=;" | cut -d\; -f7-9 | uniq | tr ";" "\n"
test-ota.local
192.168.1.44
8266
```

```bash
python2 /home/jeff/.arduino15/packages/esp8266/hardware/esp8266/2.5.2/tools/espota.py -d  -i 192.168.1.44 -a 123 -f test-ota.esp8266.esp8266.nodemcuv2.bin
```


# get ip address, hostname, and ota port
```bash
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
```





# Expect Script
The `expect` command, or Expect scripting language,
is a language that talks with your interactive programs or scripts that require user dialog.
`expect` scripting language works by expecting input,
then the `expect` script will send the response without any user interaction.
The primary `expect` command which used for interaction are:

* **spawn** - The spawn command starts a script or a program like the shell, FTP, Telnet, SSH, SCP, and so on.
* **interact** - The interact command allows you to define a predefined user interaction.
* **expect** - The `expect` command waits for input.
* **send** - The send command sends a reply to a script or a program.

`expect` also has a companion utility called `autoexpect`.
You can use it to build an `expect` script automatically,
but no need to use it here because our dialog with `telnet` is simple.

```bash
# install the expect tools
sudo apt-get install expect
```

In our case here,
we want to reach out to the ESP8266 device via `telnet`,
and once connected, send it the character `r`.
That is it.
With that, the ESP8266 should reboot and we can proceed with the OTA of the firmware.

Here is the `expect` script I need:

```
#!/usr/bin/expect -f

# grab the arguments off the command-line of this script
set timeout 10               # timeout if you get no dialog
set ip [lindex $argv 0]      # grab first argument
set port [lindex $argv 1]    # grab second argument

# begin your interaction
spawn telnet $ip $port       # startup telnet
expect "'^]'." sleep .1;     # wait for the characters "'^]'." and sleep before sending
send "R\r";                  # send 'R' to request reboot

# terminate the connection
close
```

This script is placed in the file `` and made executable (ie. ).

Sources of inspiration:

* [Expect command and how to automate shell scripts like magic](https://likegeeks.com/expect-command/)
* [Expect examples and tips](https://www.pantz.org/software/expect/expect_examples_and_tips.html)
