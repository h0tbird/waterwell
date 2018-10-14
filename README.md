# waterwell

## Setup UDEV

```
$ journalctl --since "5 minutes ago" | grep cdc_acm
Oct 14 08:31:18 carbon kernel: cdc_acm 1-1.2:1.0: ttyACM3: USB ACM device
Oct 14 08:31:25 carbon kernel: cdc_acm 3-2:1.0: ttyACM4: USB ACM device
```

```
$ udevadm info -a -n /dev/ttyACM3 | grep serial
ATTRS{serial}=="55731323236351706161"
$ udevadm info -a -n /dev/ttyACM4 | grep serial
ATTRS{serial}=="5573932323135180A092"
```

```
# cat << EOF > /etc/udev/rules.d/99-arduino.rules
SUBSYSTEMS=="usb", KERNEL=="ttyACM[0-9]*", ATTRS{serial}=="55731323236351706161", SYMLINK+="arduino/uno/tx"
SUBSYSTEMS=="usb", KERNEL=="ttyACM[0-9]*", ATTRS{serial}=="5573932323135180A092", SYMLINK+="arduino/uno/rx"
EOF
```

```
udevadm control --reload-rules
```
## Libs

```
git clone git@github.com:PaulStoffregen/OneWire.git libs/OneWire
git clone git@github.com:milesburton/Arduino-Temperature-Control-Library.git libs/DallasTemperature
git clone git@github.com:PaulStoffregen/RadioHead.git libs/RadioHead
```

```
rm -rf libs/*/.git
```

## Build and flash

```
./bin/build tx --flash
./bin/build rx --flash
```

## Console

```
picocom -b 9600 /dev/arduino/uno/tx
picocom -b 9600 /dev/arduino/uno/rx
```

Exit: `Ctrl` + (`a`,`x`)
