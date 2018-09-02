# waterwell

## Setup UDEV

```
$ lsusb | grep Arduino
Bus 002 Device 003: ID 2341:0043 Arduino SA Uno R3 (CDC ACM)
```

```
udevadm info -a -n /dev/ttyACM3 | grep manu
    ATTRS{manufacturer}=="Arduino (www.arduino.cc)"
    ATTRS{manufacturer}=="Linux 4.18.5-arch1-1-ARCH xhci-hcd"
```

```
# cat << EOF > /etc/udev/rules.d/99-arduino.rules
SUBSYSTEMS=="usb", KERNEL=="ttyACM[0-9]*", ATTRS{idProduct}=="0043", ATTRS{idVendor}=="2341", SYMLINK+="arduino/uno"
EOF
```

```
udevadm control --reload-rules
```
## Libs

```
git clone git@github.com:PaulStoffregen/OneWire.git libs/OneWire
git clone git@github.com:milesburton/Arduino-Temperature-Control-Library.git libs/DallasTemperature
```

```
rm -rf libs/*/.git
```

## Build and flash

```
./bin/build
```

## Console

```
picocom -b 9600 /dev/arduino/uno
```

Exit: `Ctrl` + `a` release `Ctrl` + `x`
