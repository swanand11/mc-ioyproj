# LoRa Blockchain Messaging System

A secure IoT messaging system that combines LoRa wireless communication with blockchain-based device authentication and message logging.

## Overview

This project implements a distributed messaging system where:
- **ESP32 devices** communicate via LoRa radio
- **Blockchain smart contract** handles device registration and message logging
- **Flask server** acts as a bridge between devices and blockchain
- **Authentication system** ensures only registered devices can send trusted messages

## System Architecture

```
┌─────────────┐    LoRa     ┌─────────────┐
│   Sender    │─────────────│  Receiver   │
│   ESP32     │             │   ESP32     │
└─────────────┘             └─────────────┘
       │                           │
       │ HTTP/WiFi                 │ HTTP/WiFi
       │                           │
       └─────────────┬─────────────┘
                     │
              ┌─────────────┐
              │   Flask     │
              │   Server    │
              └─────────────┘
                     │
                     │ Web3
                     │
              ┌─────────────┐
              │ Blockchain  │
              │ (Hardhat)   │
              └─────────────┘
```

## Components

### 1. Smart Contract (`contracts/Lock.sol`)
- **Device Registration**: Manages authorized devices
- **Message Logging**: Stores messages with timestamps on blockchain
- **Access Control**: Only registered devices can log messages
- **Events**: Emits events for device registration and message logging

### 2. Flask Server (`pyscript/server.py`)
- **API Endpoints**:
  - `POST /log`: Log messages to blockchain
  - `POST /auth`: Check device authentication status
- **Blockchain Integration**: Interacts with smart contract via Web3
- **Auto-registration**: Automatically registers new devices

### 3. ESP32 Sender (`sender/sender.ino`)
- Sends messages via LoRa radio
- Posts messages to Flask server for blockchain logging
- LCD display for status updates
- WiFi connectivity for HTTP requests

### 4. ESP32 Receiver (`receiver/receiver.ino`)
- Receives LoRa messages
- Authenticates sender devices via Flask server
- Displays messages only from authenticated devices
- LCD display for message output

## Hardware Requirements

### For Each ESP32:
- ESP32 development board
- LoRa module (SX1276/SX1278)
- I2C LCD display (16x2)
- Connecting wires and breadboard

### Wiring Diagram:
```
ESP32 Pin  →  LoRa Module
5 (NSS)    →  NSS
15 (RESET) →  RESET
2 (DIO0)   →  DIO0
23 (MOSI)  →  MOSI
19 (MISO)  →  MISO
18 (SCK)   →  SCK
3.3V       →  VCC
GND        →  GND

ESP32 Pin  →  LCD (I2C)
21 (SDA)   →  SDA
22 (SCL)   →  SCL
5V         →  VCC
GND        →  GND
```

## Software Requirements

- Node.js (for Hardhat blockchain)
- Python 3.7+ (for Flask server)
- Arduino IDE (for ESP32 programming)
- Required Python packages: `flask`, `web3`
- Required Arduino libraries: `LoRa`, `WiFi`, `HTTPClient`, `LiquidCrystal_I2C`

## Installation & Setup

### 1. Blockchain Setup
```bash
# Install Hardhat (if not already installed)
npm install --save-dev hardhat

# Start local blockchain
npx hardhat node

# Deploy smart contract
npx hardhat run scripts/deploy.js --network localhost
```

### 2. Flask Server Setup
```bash
# Install dependencies
pip install flask web3

# Update contract address in server.py
# (Use the address from contract deployment)

# Start server
python pyscript/server.py
```

### 3. ESP32 Setup
1. Create `keys.h` file in both `sender/` and `receiver/` directories:
```cpp
// keys.h
const char* ssid = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";
const char* serverUrl = "http://YOUR_FLASK_SERVER_IP:5000/log";
```

2. Install required Arduino libraries:
   - LoRa by Sandeep Mistry
   - LiquidCrystal I2C

3. Upload `sender.ino` to sender ESP32
4. Upload `receiver.ino` to receiver ESP32

## Usage

### Starting the System
1. Start local blockchain: `npx hardhat node`
2. Deploy smart contract and note the contract address
3. Update `server.py` with the contract address
4. Start Flask server: `python pyscript/server.py`
5. Power on ESP32 devices

### Operation Flow
1. **Sender ESP32**: Sends "Hello0" through "Hello9" messages
2. **LoRa Communication**: Messages transmitted via 433MHz LoRa
3. **Blockchain Logging**: Messages logged to blockchain via Flask server
4. **Device Authentication**: Receiver checks sender authentication
5. **Message Display**: Only authenticated messages shown on receiver LCD

## API Endpoints

### POST /log
Log a message to the blockchain
```json
{
  "device_id": "esp32_sender",
  "message": "Hello World"
}
```

### POST /auth
Check device authentication
```json
{
  "device_id": "esp32_sender"
}
```
Returns: `"yes"` or `"no"`

## Smart Contract Functions

- `registerDevice(string deviceId)`: Register a new device
- `logMessage(string deviceId, string content)`: Log message from registered device
- `getMessage(uint256 index)`: Retrieve message by index
- `getMessageCount()`: Get total number of messages
- `registeredDevices(string deviceId)`: Check if device is registered

## Configuration

### LoRa Settings
- **Frequency**: 433MHz
- **Pins**: NSS=5, RESET=15, DIO0=2

### Network Settings
- **WiFi**: Configure in `keys.h`
- **Server**: Default port 5000
- **Blockchain**: Local Hardhat network (port 8545)

## Troubleshooting

### Common Issues:
1. **LoRa not working**: Check wiring and frequency settings
2. **WiFi connection failed**: Verify credentials in `keys.h`
3. **Blockchain connection error**: Ensure Hardhat node is running
4. **Contract interaction fails**: Verify contract address in `server.py`

### Debug Tips:
- Monitor Serial output for both ESP32 devices
- Check Flask server logs for HTTP requests
- Verify blockchain transactions in Hardhat console

## Security Features

- **Device Registration**: Only registered devices can log messages
- **Blockchain Immutability**: Messages stored permanently on blockchain
- **Authentication**: Receiver verifies sender before displaying messages
- **Timestamp Verification**: All messages include blockchain timestamps

## Future Enhancements

- Add message encryption for LoRa communication
- Implement device revocation mechanism
- Add message acknowledgment system
- Support for multiple LoRa channels
- Web dashboard for message monitoring

## Contributing

1. Fork the repository
2. Create a feature branch
3. Commit your changes
4. Push to the branch
5. Create a Pull Request
