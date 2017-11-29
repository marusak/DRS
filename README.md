# DRS
Drifting recognition software


How to connect to BT:
1. sudo dnf install bluez-*
2. sudo pip install pybluez
3. Pair BT device on your computer
3.1.    bluetoothctl
3.2.    pair 98:D3:32:30:AB:5C
3.3.    enter pin code 1234

add `DisablePlugins = pnat` into /etc/bluetooth/main.conf

