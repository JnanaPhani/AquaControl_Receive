# AquaController Receiver

Receiver firmware for the **AquaController** project.  
This module is responsible for receiving water level data over LoRa and controlling the motor accordingly.  
It supports both automatic and manual control modes, along with system status indication via onboard LEDs.

## Features Implemented

- **LoRa Initialization**
- **Automatic Motor Control** (Auto ON/OFF based on water level)
- **Manual Motor Control**
- **LoRa Connection Status Indicator**
  - **LED 02 ON**: LoRa connected
  - **LED 02 OFF**: LoRa not connected
- **Water Level Indicator**
  - **LED 01 ON**: Water level below 40%
  - **LED 01 OFF**: Water level above 41%
- **Relay Status Indicator**
  - **LED 04 Toggle**: Relay ON
  - **LED 04 OFF**: Relay OFF
- **Manual Switch Control**
  - **First Press**: Relay ON
  - **Second Press**: Relay OFF

## Pin Configuration

| Function Pin    | ESP32 Pin   |
|-----------------|-------------|
| LoRa Reset      | GPIO5       |
| LoRa MISO       | GPIO12      |
| LoRa MOSI       | GPIO13      |
| LoRa SCK        | GPIO14      |
| LoRa NSS (CS)   | GPIO15      |
| LoRa DIO        | GPIO34      |
| LED D4          | GPIO18      |
| LED D3          | GPIO19      |
| LED D2          | GPIO21      |
| LED D1          | GPIO22      |
| Manual Switch   | GPIO27      |

## Getting Started

### Prerequisites

- ESP32 Development Board
- LoRa SX1278 Module
- ESP-IDF installed ([ESP-IDF Installation Guide](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/get-started/))

### Build and Flash

```bash
# Clone the repository
git clone <repository-url>
cd AquaController_Receiver

# Build the project
. $HOME/esp/esp-idf/export.sh
idf.py set-target esp32
idf.py build

# Flash the firmware to ESP32
sudo chmod a+rw <port>
idf.py -p <port> build flash monitor

# Monitor logs
idf.py monitor





