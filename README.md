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

| Function           | GPIO Pin | ESP32 Pin |
|-------------------|----------|-----------|
| LoRa Reset        | IO5      | GPIO5     |
| LoRa MISO         | IO12     | GPIO12    |
| LoRa MOSI         | IO13     | GPIO13    |
| LoRa SCK          | IO14     | GPIO14    |
| LoRa NSS (CS)     | IO15     | GPIO15    |
| LoRa DIO          | IO34     | GPIO34    |
| LED D4            | IO18     | GPIO18    |
| LED D3            | IO19     | GPIO19    |
| LED D2            | IO21     | GPIO21    |
| LED D1            | IO22     | GPIO22    |
| Manual Switch     | IO27     | GPIO27    |

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

# Configure the project
idf.py menuconfig

# Build the project
idf.py build

# Flash the firmware to ESP32
idf.py -p <your-port> flash

# Monitor logs
idf.py monitor

