# ASO-F1
Repository from the ASO project

## Pre-requirements

### Digital Devices

A Raspberry Pi, models with WiFi and Bluetooth module included, an SD Card.

### Electrical Devices

A breadboard, 4 pushbuttons, 2 LEDs, 2 1k resistors and 2 330 resistors.

## Instalation

1. Installing Git with Apt:

   - Start by updating the package index:
     ```
     sudo apt update
     ```
   - Then upgrade all yout packages:
     ```
     sudo apt full-upgrade
     ```
   - Run the following command to install Git:
     ```
     sudo apt install git
     ```
   - Verify the installation by typing the following command which will print the Git version:
     ```
     git --version
     ```
     
2. Cloning the repository using the following command:
```
git clone https://github.com/Supersonic2510/ASO-F1.git
```
3. Enter the directory called ASO-F1:
```
cd ASO-F1
```
4. Execute scrpit called install:
```
bash install.sh
```

## Uninstallation

1. Enter the directory called ASO-F1:
```
cd ASO-F1/
```

2. Execute scrpit called uninstall:
```
bash uninstall.sh
```

3. Return to the previous directory:
```
cd ../
```

4. Remove the directory:
```
rm -rf ./ASO-F1
```

## Version

* 0.1

## Author

* **Pol Navarro Solà** - *Main project* - [Supersonic2510](https://github.com/Supersonic2510)
